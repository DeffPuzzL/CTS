#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

//char *g_send_msg = "0032|800000|110|iboxpay|cts|company|";
char *g_send_msg = "POST /clearment/algo HTTP/1.1\r\nHost: 172.30.1.12:2900\r\nAccept: */*\r\nContent-Type:application/json;charset=UTF-8\r\nContent-Length: 517\r\n\r\n{\"id\":\"QK20171128000000009\",\"tx_date\":\"20171128\",\"tx_time\":\"112848\",\"channel_no\":\"ylby\",\"pay_type\":\"vidata.quick.backpay\",\"seq_order\":\"000000000007\",\"tx_amt\":100.00,\"disc_cycle\":\"1\",\"currency\":\"CNY\",\"card_type\":1,\"app_code\":\"800440054111002\",\"mert_no\":\"015440395110988\",\"mert_nm\":\"维生数测试商户\",\"brh_code\":\"001001\",\"cert_type\":\"2\",\"cert_no\":\"341126197709218366\",\"mobile_no\":\"13800001111\",\"order_id\":\"20171127173303\",\"product_code\":\"Q8173957\",\"product_name\":\"API普通快捷\",\"pay_kind\":\"\",\"status\":\"success\"}";

//char *g_send_msg = "0323{ \"verNo\": \"9.00\", \"orderNo\": \"982326789216383\", \"transType\": \"41\", \"transAmt\": \"5.00\", \"cardType\": \"02\", \"transDisc\": \"0\", \"mchtNo\": \"001440396451407\", \"brhNo\": \"001001\", \"regionCode\": \"331302\", \"termType\": \"21\", \"licenseType\": \"010\", \"scanMode\": \"2\", \"cardBank\": \"1234\", \"mchtType\": \"E1\", \"txCode\": \"12\", \"reqType\": \"0\" }";

int g_send_len = 0;

typedef struct __client_conn
{
    int  socket_fd;
    int  if_connect: 1;
    int  total_len;
    int  send_len;
} client_conn, *p_client_conn;

int g_run_flag = 1;
int g_epoll_fd = -1;
int g_succ_count = 0;
client_conn g_conn[65535];
struct epoll_event g_events[65535];

static void
sig_handler(int sig_no)
{
    g_run_flag = 0;
}

static int
pfm_set_unblock(int socket_fd)
{
    int option = 0;

    if (0 > (option = fcntl(socket_fd, F_GETFL)))
    {
        return -1;
    }

    option = option | O_NONBLOCK;

    if (0 > fcntl(socket_fd, F_SETFL, option))
    {
        return -1;
    }

    return 0;
}

static int
pfm_connect(int socket_fd, char *ip, int port)
{
    struct sockaddr_in client_addr;

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);

    if (0 == inet_aton(ip, &client_addr.sin_addr))
    {
        return -1;
    }

    if (0 != pfm_set_unblock(socket_fd))
    {
        return -1;
    }

    if (0 != connect(socket_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)))
    {
        if (errno != EINPROGRESS)
        {
            return -1;
        }
        else
        {
            g_conn[socket_fd].if_connect = 0;
            g_conn[socket_fd].total_len = g_send_len;
            g_conn[socket_fd].send_len = 0;
            epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, socket_fd, &g_events[socket_fd]);
            return 0;
        }
    }

    g_conn[socket_fd].if_connect = 1;
    g_conn[socket_fd].total_len = g_send_len;
    g_conn[socket_fd].send_len = 0;
    epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, socket_fd, &g_events[socket_fd]);
    return 0;
}

static int
pfm_client_init(char *ip, int port)
{
    int socket_fd = -1;
    struct linger opt_linger;

    if (0 > (socket_fd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        return -1;
    }

    memset(&opt_linger, 0, sizeof(opt_linger));
    opt_linger.l_onoff = 0;
    opt_linger.l_linger = 0;

    if (0 > setsockopt(socket_fd,
                       SOL_SOCKET,
                       SO_LINGER,
                       &opt_linger,
                       sizeof(opt_linger)))
    {
        close(socket_fd);
        return -1;
    }

    if (0 != pfm_connect(socket_fd, ip, port))
    {
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

static int
pfm_tcp_recv(int socket_fd, char *buffer, int buf_len, int flag)
{
    int recv_len = 0;
    int total_recv_len = 0;

    while (1)
    {
        recv_len = recv(socket_fd,
                        buffer + total_recv_len,
                        buf_len - total_recv_len,
                        flag);

        if (recv_len < 0)
        {
            if (EAGAIN == errno)
            {
                return total_recv_len;
            }
            else
            {
                return -1;
            }
        }
        else if (0 == recv_len)
        {
            return total_recv_len;
        }

        total_recv_len += recv_len;

        if (total_recv_len == buf_len)
        {
            return total_recv_len;
        }
    }
}

static int
pfm_tcp_send(int socket_fd, const char *buffer, int buf_len, int flag)
{
    int send_len = 0;
    int total_send_len = 0;

    while (1)
    {
        send_len = send(socket_fd,
                        buffer + total_send_len,
                        buf_len - total_send_len,
                        flag);

        if (send_len < 0)
        {
            if (errno == EAGAIN)
            {
                return total_send_len;
            }

            return -1;
        }
        else if (0 == send_len)
        {
            return 0;
        }

        total_send_len += send_len;

        if (total_send_len == buf_len)
        {
            return total_send_len;
        }
    }
}

static int
pfm_conn_refresh(int socket_fd, char *ip, int port)
{
    epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL);
    close(socket_fd);
    return pfm_client_init(ip, port);
}

static int
pfm_epoll_run(char *ip, int port)
{
    int i = 0, n = 10;
    int iret = 0;
    int error = 0;
    int fd_num = 0;
    char *p, buffer[1024] = {0}, szIdx[30];
    socklen_t len = sizeof(error);
    p_client_conn p_cc = NULL;
    struct epoll_event events[65535];

    while (g_run_flag)
    {
        fd_num = epoll_wait(g_epoll_fd, events, 65535, 500);

        for (i = 0; i < fd_num; i++)
        {
            p_cc = (p_client_conn)events[i].data.ptr;

            if (events[i].events & EPOLLIN)
            {
                memset(buffer, 0, sizeof(buffer));
                iret = pfm_tcp_recv(p_cc->socket_fd, buffer, sizeof(buffer), 0);

                if (iret < 0)
                {
                    fprintf(stderr, "pfm_tcp_recv error, errstr:%s.\n", strerror(errno));
                    return -1;
                }
                else if (iret > 0)
                {
                    fprintf(stdout, "recv data:%d, %s.\n", p_cc->socket_fd, buffer);
                    g_succ_count++;
                }

                if (0 > pfm_conn_refresh(p_cc->socket_fd, ip, port))
                {
                    fprintf(stderr, "pfm_conn_refresh error, errstr:%s.\n", strerror(errno));
                    return -1;
                }
            }

            if (events[i].events & EPOLLOUT)
            {
                if (0 == p_cc->if_connect)
                {
                    getsockopt(p_cc->socket_fd, SOL_SOCKET, SO_ERROR, (void *)&error, &len);

                    if (0 != error)
                    {
                        if (0 > pfm_conn_refresh(p_cc->socket_fd, ip, port))
                        {
                            fprintf(stderr, "pfm_conn_refresh error, errstr:%s.\n", strerror(errno));
                            return -1;
                        }

                        continue;
                    }

                    p_cc->if_connect = 1;
                }

                if (p_cc->send_len < p_cc->total_len)
                {
					p = strstr(g_send_msg, "QK20171128000000009");
                    p_cc->send_len += pfm_tcp_send(p_cc->socket_fd,
                                                   g_send_msg + p_cc->send_len,
                                                   p_cc->total_len - p_cc->send_len,
                                                   0);
                }
            }
        }
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    int i = 0;
    int client_num = 0;
    int user_time = 0;
    time_t begin_time;

    if (argc != 3 && argc != 4)
    {
        fprintf(stderr, "Usage: %s ip port [client_num]\n", argv[0]);
        exit(-1);
    }

    if (argc == 4)
    {
        client_num = atoi(argv[3]);
    }
    else
    {
        client_num = 1;
    }

    signal(SIGINT, sig_handler);

    g_send_len = strlen(g_send_msg);

    for (i = 0; i < 65535; i++)
    {
        g_conn[i].socket_fd = i;
        g_conn[i].if_connect = 0;
        g_conn[i].total_len = g_send_len;
        g_conn[i].send_len = 0;

        g_events[i].data.fd = i;
        g_events[i].data.ptr = &g_conn[i];
        g_events[i].events = EPOLLIN | EPOLLOUT | EPOLLET;
    }

    if (0 > (g_epoll_fd = epoll_create(1024)))
    {
        fprintf(stderr, "epoll_create error, errstr:%s.\n", strerror(errno));
        exit(-2);
    }

    begin_time = time(NULL);

    for (i = 0; i < client_num; i++)
    {
        if (0 > pfm_client_init(argv[1], atoi(argv[2])))
        {
            fprintf(stderr, "pfm_client_init error, errstr:%s.\n", strerror(errno));
            exit(-3);
        }
    }

    pfm_epoll_run(argv[1], atoi(argv[2]));

    user_time = time(NULL) - begin_time;

    if (user_time > 0)
    {
        fprintf(stdout, "\ntps:%d.\n", g_succ_count / user_time);
    }

    return 0;
}


