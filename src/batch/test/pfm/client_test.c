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

typedef	unsigned char	uchar;


char *g_send_msg = "POST /clearment/algo HTTP/1.1\r\nHost: 172.30.1.12:2900\r\nAccept: */*\r\nContent-Type:application/json;charset=UTF-8\r\nContent-Length: 517\r\n\r\n{\"id\":\"QK20171128000000009\",\"tx_date\":\"20171128\",\"tx_time\":\"112848\",\"channel_no\":\"ylby\",\"pay_type\":\"vidata.quick.backpay\",\"seq_order\":\"000000000007\",\"tx_amt\":100.00,\"disc_cycle\":\"1\",\"currency\":\"CNY\",\"card_type\":1,\"app_code\":\"800440054111002\",\"mert_no\":\"015440395110988\",\"mert_nm\":\"维生数测试商户\",\"brh_code\":\"001001\",\"cert_type\":\"2\",\"cert_no\":\"341126197709218366\",\"mobile_no\":\"13800001111\",\"order_id\":\"20171127173303\",\"product_code\":\"Q8173957\",\"product_name\":\"API普通快捷\",\"pay_kind\":\"\",\"status\":\"success\"}";

//112
//char *g_send_msg = "0121test00004\x60\x00\x03\x00\x00\x60\x31\x00\x31\x07\x30\x02\x00\x30\x20\x04\xC0\x20\xC0\x98\x11\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x03\x49\x02\x10\x00\x12\x30\x62\x25\x82\x21\x12\x99\x63\x01\x5D\x15\x11\x10\x10\x00\x00\x35\x36\x38\x35\x32\x33\x31\x34\x32\x33\x35\x32\x31\x34\x35\x32\x36\x38\x35\x39\x32\x33\x36\x31\x35\x36\xC6\x24\x83\x4D\x36\x7E\x9E\x9E\x20\x00\x00\x00\x00\x00\x00\x00\x00\x13\x22\x00\x00\x08\x00\x05\x00\x36\x37\x41\x32\x32\x39\x39\x41";
//char *g_send_msg = "0323{ \"verNo\": \"9.00\", \"orderNo\": \"982326789216383\", \"transType\": \"41\", \"transAmt\": \"5.00\", \"cardType\": \"02\", \"transDisc\": \"0\", \"mchtNo\": \"001440396451407\", \"brhNo\": \"001001\", \"regionCode\": \"331302\", \"termType\": \"21\", \"licenseType\": \"010\", \"scanMode\": \"2\", \"cardBank\": \"1234\", \"mchtType\": \"E1\", \"txCode\": \"12\", \"reqType\": \"0\" }";

int		g_send_len = 0;

static int pfm_connect(int socket_fd, char *ip, int port)
{
    struct sockaddr_in client_addr;

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);

    if (0 == inet_aton(ip, &client_addr.sin_addr))
    {
        return -1;
    }

    if (0 != connect(socket_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)))
    {
		fprintf(stdout, "connect error, %s", strerror(errno));
		return -1;
    }

    return 0;
}

static int pfm_client_init(char *ip, int port)
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

void    vPrintIBPHex(char *s, int k)
{   
    register    int i, j, n = 0, a, len;
    char        szMsg[512];
    
    memset(szMsg, 0, sizeof(szMsg));
    for(i = 0, n = 0; i < k; n ++)
    {   
        len = sprintf(szMsg, "%07d - %07d: ", n * 16,  
            (n + 1) * 16 - 1 > k - 1 ? k - 1 : (n + 1) * 16 - 1);
        for(j = 0, a = 0; j < 16; j ++,i++)
        {   
            if(i < k)
            {   
                len += sprintf(szMsg + len, "%02X ", (uchar)s[i]);
                ++ a;
            }
            else
            {   
                len += sprintf(szMsg + len, "   ");
                i = k - 1;
            }
        }
        
        strcat(szMsg, " ");
        len ++;
        
        for(a; a > 0; a --)
        {
            if(32 <= (uchar)s[i - a] && s[i - a] <= 126)
                len += sprintf(szMsg + len, "%c", (uchar)s[i - a]);
            else
                len += sprintf(szMsg + len, ".");
        }
        szMsg[len] = 0x00;
		fprintf(stdout, "%s\n", szMsg);
    }
    return ;
}


int		main(int argc, char *argv[])
{
    int		i = 0, sock, len;
    char	buffer[5024] = {0}, *p = NULL;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(-1);
    }

	memset(buffer, 0, sizeof(buffer));
	g_send_len = strlen(g_send_msg);
	if(0 > (sock = pfm_client_init(argv[1], atoi(argv[2]))))
  	{
   		fprintf(stderr, "pfm_client_init error, errstr:%s.\n", strerror(errno));
		exit(-3);
 	}

	if(g_send_len != pfm_tcp_send(sock, g_send_msg, g_send_len, 0))
	{
		fprintf(stdout, "pfm_tcp_send, %s", strerror(errno));
		return -1;
	}

	if((len = pfm_tcp_recv(sock, buffer, sizeof(buffer), 0)) < 0)
	{
		fprintf(stderr, "pfm_tcp_recv error, errstr:%s.\n", strerror(errno));
		return -1;
	}
	close(sock);
	fprintf(stdout, "%s\n", buffer);
	
	p = strstr(g_send_msg, "QK20171128000000009");
	fprintf(stdout, "id:%s\n", p);

	fprintf(stdout, "----------------------------------------------------------------------------(%4d)--\n", len);
	vPrintIBPHex(buffer, len);
	fprintf(stdout, "------------------------------------------------------------------------------------\n");
	fflush(stdout);

    return 0;
}


