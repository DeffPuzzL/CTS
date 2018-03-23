#include    "tmain.h"

/*************************************************************************************************
 *
 *************************************************************************************************/
CMList  *g_pstDomain = NULL;
extern char **environ;
static char *g_Last = NULL;
static char **g_Argv = NULL;

/*************************************************************************************************
 *************************************************************************************************/
#define    IBPrace(...)        vSetTVMLog(__FILE__, __LINE__, __VA_ARGS__)

/*************************************************************************************************
    函数说明：关闭套接字
    参数说明：
        skSock                    --套接字
    返回值：
        无
 *************************************************************************************************/
void    vSetTVMLog(const char *pszFile, long lLine, const char *fmt, ...)
{
    va_list ap;
    FILE    *fp = NULL;
    struct  tm      *ptm = NULL;
    struct  timeb   tb;
    char    szLog[512], szMsg[5120];

    memset(szLog, 0, sizeof(szLog));
    memset(szMsg, 0, sizeof(szMsg));
    snprintf(szLog, sizeof(szLog), "%s/log/tvm.log", getenv("HOME"));

    va_start(ap, fmt);
    vsnprintf(szMsg, sizeof(szMsg), fmt, ap);
    va_end(ap);

    if(NULL == (fp = fopen(szLog, "a+")))
    {
        fprintf(stderr, "P(%d), 打开文件(%s)失败, err:%s,[%s]\n", getpid(), szLog,
            strerror(errno), szMsg);
        return ;
    }

    ftime(&tb);
    ptm = localtime(&tb.time);
    fprintf(fp, "F=%-10s L=%-5d P=%-7d H=%-7d  T=%04d%02d%02d %02d%02d%02d:%03d  %s\n",
        pszFile, lLine, getpid(), syscall(SYS_gettid), ptm->tm_year + 1900, ptm->tm_mon + 1,
         ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tb.millitm, szMsg);
    fclose(fp);
    return ;
}

void    vCloseSocket(BSock skSock)
{
    shutdown(skSock, SHUT_RDWR);
    close(skSock);
}

void    vPrintProgresss(int nValid, int nMax)
{
    double  dPer;
    int     i, nPer;
    static  int len = 0;
    char    szPrint[64];

    for(i = len + 50; i > 0 && nValid > 1; i --)
        fprintf(stdout, "\b");

    dPer = nValid * 100.0 / nMax;
    nPer = nValid * 50 / nMax > 0 ? nValid * 50 / nMax : 1;
    if(dPer < 60.00)
        fprintf(stdout, "\033[42;32m");
    else if(dPer < 70.00)
        fprintf(stdout, "\033[45;35m");
    else if(dPer < 80.00)
        fprintf(stdout, "\033[46;36m");
    else  if(dPer < 90.00)
        fprintf(stdout, "\033[43;33m");
    else
        fprintf(stdout, "\033[41;31m");

    fflush(stdout);
    for(i = 0; i < nPer; i ++)
        fprintf(stdout, "|");

    fprintf(stdout, "\033[0m");
    for(i; i < 50; i ++)
        fprintf(stdout, " ");

    memset(szPrint, 0, sizeof(szPrint));
    len = snprintf(szPrint, sizeof(szPrint), "] %6.3f%%, (%d/%d)", dPer, nValid, nMax);
    fprintf(stdout, "%s", szPrint);
    fflush(stdout);
}

void    vInitTitle(int argc, char **argv, char **envp)
{
    int    i = 0;

    for(i = 0; envp[i] != NULL; i++) // calc envp num
           continue;

    environ = (char **) malloc(sizeof (char *) * (i + 1));
    for (i = 0; envp[i] != NULL; i++)
    {
        environ[i] = malloc(sizeof(char) * strlen(envp[i]) + 1);
        memset(environ[i], 0, sizeof(char) * strlen(envp[i]) + 1);
        strcpy(environ[i], envp[i]);
    }

    environ[i] = NULL;
    g_Argv = argv;
    if (i > 0)
      g_Last = envp[i - 1] + strlen(envp[i - 1]);
    else
      g_Last = argv[argc - 1] + strlen(argv[argc - 1]);
}

void    vSetTitile(const char *pname)
{
    int     i;
    char    *p, name[16];
    extern char **g_Argv;
    extern char *g_Last;

    strncpy(name, pname, 16);
    i = strlen(name);
    if (i > g_Last - g_Argv[0] - 2)
    {
        i = g_Last - g_Argv[0] - 2;
        name[i] = '\0';
    }

    (void) strcpy(g_Argv[0], name);
    p = &g_Argv[0][i];
    while (p < g_Last)
        *p++ = '\0';
    g_Argv[1] = NULL;
    prctl(PR_SET_NAME, name);
}

uint64_t get_tick_time()
{
    struct timeval tval;
    uint64_t tick;

    gettimeofday(&tval, NULL);

    tick = tval.tv_sec * 1000L + tval.tv_usec / 1000L;

    return tick;
}

/*************************************************************************************************
    函数说明：设置非阻塞
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lSetUnBlock(BSock skSock)
{
    int     nOpt = 0;

    if(fcntl(skSock, F_GETFL) < 0)
        return RC_FAIL;

    nOpt = nOpt | O_NONBLOCK;
    if(fcntl(skSock, F_SETFL, nOpt) < 0)
        return RC_FAIL;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：设置阻塞
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lSetBlock(BSock skSock)
{
    int     nOpt = 0;

    if(fcntl(skSock, F_GETFL) < 0)
        return RC_FAIL;

    nOpt &= ~O_NONBLOCK;
    if(fcntl(skSock, F_SETFL, nOpt) < 0)
        return RC_FAIL;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：连接服务器    
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
BSock    skConnectServer(SATvm *pstSavm, char *pszIp, long lPort, BOOL bf, long lTime)
{
    fd_set    set, exp;
    struct  timeval tv;
    struct  linger  lin;
    BSock   skSock;
    struct  sockaddr_in stAdr;
    int        error = -1, len = sizeof(int);

    memset(&stAdr, 0, sizeof(struct sockaddr_in));
    stAdr.sin_family      = AF_INET;
    stAdr.sin_addr.s_addr = inet_addr(pszIp);
    stAdr.sin_port        = htons((u_short)lPort);

    if((skSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        pstSavm->m_lErrno = SOCK_ERR_CRTE;
        return RC_FAIL;
    }

    memset(&lin, 0, sizeof(lin));
    lin.l_onoff = TRUE;
    lin.l_linger = 10;
    if (0 > setsockopt(skSock, SOL_SOCKET, SO_LINGER, &lin, sizeof(lin)))
    {
        close(skSock);
        return RC_FAIL;
    }

    if(RC_SUCC != lSetUnBlock(skSock))
    {
        close(skSock);
        return RC_FAIL;
    }

    if(RC_SUCC == connect(skSock, (struct sockaddr *)(&stAdr), sizeof(struct sockaddr_in)))
    {
        if(!bf)    lSetBlock(skSock);    //    设置非阻塞
        return skSock;
    }

    if(errno != EINPROGRESS)
    {
        close(skSock);
        pstSavm->m_lErrno = SOCK_CONN_ERR;
        return RC_FAIL;
    }

    FD_ZERO(&set);
    FD_ZERO(&exp);
    FD_SET(skSock, &set);
    tv.tv_sec = lTime;
    tv.tv_usec = 0;
    errno = 0;
    if(RC_SUCC >= select(skSock + 1, NULL, &set, &exp, &tv))
    {
        close(skSock);
        pstSavm->m_lErrno = SOCK_CONN_TMO;
        return RC_FAIL;
    }

    if(!FD_ISSET(skSock, &set) ||  FD_ISSET(skSock, &exp))  //异常套接字就绪
    {
        close(skSock);
        pstSavm->m_lErrno = SOCK_CONN_ERR;
        return RC_FAIL;
    }

#ifdef HP_UX
    getsockopt(skSock, SOL_SOCKET, SO_ERROR, &error, &len);
#else    // linux
    getsockopt(skSock, SOL_SOCKET, SO_ERROR, &error,(socklen_t*)&len);
#endif
    if(!bf)    lSetBlock(skSock);    //    设置非阻塞
    if(0 == error)    return skSock;
    
    pstSavm->m_lErrno = SOCK_CONN_ERR;
    close(skSock);
    return RC_FAIL;
}

/*************************************************************************************************
    函数说明：初始化服务
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
BSock    skServerInitail(SATvm *pstSavm, int lPort)
{
    int     iret = -1;
    int     optval = 1;
    BSock    skSock = -1;
    struct sockaddr_in serveraddr;

    memset(&serveraddr, 0, sizeof(struct sockaddr_in));
    if((skSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        pstSavm->m_lErrno = SOCK_ERR_CRTE;
        return RC_FAIL;
    }

    if(RC_SUCC != lSetUnBlock(skSock))
    {
        close(skSock);
        return RC_FAIL;
    }

    if (0 > setsockopt(skSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
    {
        close(skSock);
        return RC_FAIL;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(lPort);

    if (0 > bind(skSock, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)))
    {
        close(skSock);
        pstSavm->m_lErrno = SOCK_BIND_ERR;
        return RC_FAIL;
    }

    if (0 > listen(skSock, 1024))
    {
        close(skSock);
        pstSavm->m_lErrno = SOCK_LSEN_ERR;
        return RC_FAIL;
    }

    return skSock;
}

/*************************************************************************************************
    函数说明：非阻塞发送socket数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
int        lSendUnblock(SATvm *pstSavm, int skSock, char *ss, int len, int *plTime)
{
    long     lByte = 0, lLeft = len, lWrite = 0;
    uint64_t utime = get_tick_time();

    while(lLeft > 0)
    {
        if((lWrite = send(skSock, ss + lByte, lLeft, MSG_DONTWAIT)) <= 0)
        {
            if(EWOULDBLOCK == errno|| EAGAIN == errno)
            {
                if((get_tick_time() - utime) / 1000 > *plTime)
                {
                    *plTime = 0;
                    pstSavm->m_lErrno = SOCK_SEND_TMO;
                    return lByte;
                }
                usleep(5);
                continue;
            }
            else
            {
                pstSavm->m_lErrno = SOCK_SEND_ERR;
                return RC_FAIL;
            }
        }
        else
        {
            lLeft -= lWrite;
            lByte += lWrite;
        }
    }

    *plTime -= (get_tick_time() - utime) / 1000;

    return lByte;
}

/*************************************************************************************************
    函数说明：非阻塞获取socket数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
int     lRecvUnblock(SATvm *pstSavm, int skSock, char *so, int read, int *plTime)
{
    int     lByte = 0, lRecv = 0;
    uint64_t utime = get_tick_time();

    while(read > lRecv)
    {
        lByte = recv(skSock, so + lRecv, read - lRecv, MSG_DONTWAIT);
        if(lByte < 0)
        {
            if(EAGAIN == errno || EWOULDBLOCK == errno)
            {
                if((get_tick_time() - utime) / 1000 > *plTime)
                {
                    *plTime = 0;
                    pstSavm->m_lErrno = SOCK_READ_TMO;
                    return lRecv;
                }

                usleep(5);
                continue;
            }

            if(errno == ECONNRESET || ENETRESET == errno || ENETDOWN == errno ||
                EINTR == errno)
            {
                pstSavm->m_lErrno = SOCK_IO_RESET;
                return RC_FAIL;
            }
            else
            {
                pstSavm->m_lErrno = SOCK_READ_ERR;
                return RC_FAIL;
            }
        }
        else if(lByte == 0)     //  认为是关闭了连接
        {
            pstSavm->m_lErrno = SOCK_IO_RESET;
            return RC_FAIL;
        }

        lRecv += lByte;
    }

    *plTime    -= (get_tick_time() - utime) / 1000;

    return lRecv;
}

/*************************************************************************************************
    函数说明：非阻塞获取socket流中的数据，不等待返回
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
int        lRecvBuffer(int skSock, char *pszRecv, int lRead)
{
    int     lByte = 0, lRecv = 0;

    errno = 0;
    while(lRead > lRecv)
    {
        lByte = recv(skSock, pszRecv + lRecv, lRead - lRecv, 0);
        if(lByte < 0)
        {
            if(EAGAIN == errno || EWOULDBLOCK == errno)
                return lRecv;

            //  Connection reset by peer
            if(errno == ECONNRESET || ENETRESET == errno || ENETDOWN == errno ||
                EINTR == errno)
                return RC_FAIL;
            else
                return RC_FAIL;
        }
        else if(lByte == 0) //  认为是关闭了连接
            return RC_CLOSE;

        lRecv += lByte;
    }

    return lRecv;
}

/*************************************************************************************************
    函数说明：阻塞发送socket数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
int     lSendBuffer(BSock skSock, void *pszSend, int lSend)
{
    long    lByte = 0, lLeft = lSend, lWrite = 0;

    errno = 0;
    while(lLeft > 0)
    {
        if((lWrite = send(skSock, pszSend + lByte, lLeft, 0)) <= 0)
            return lByte;
        else
        {
            lLeft -= lWrite;
            lByte += lWrite;
        }
    }

    return lByte;
}

/*************************************************************************************************
    函数说明：阻塞获取socket数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
int        lReadBlock(SATvm *pstSavm, int skSock, char *pszRecv, int lRead)
{
    int     lByte = 0, lRecv = 0;

    while(lRead > lRecv)
    {
        lByte = recv(skSock, pszRecv + lRecv, lRead - lRecv, 0);
        if(lByte < 0)
        {
            pstSavm->m_lErrno = SOCK_READ_ERR;
            return RC_FAIL;
        }
        else if(lByte == 0) //  认为是关闭了连接
        {
            pstSavm->m_lErrno = SOCK_IO_RESET;
            return lRecv;
        }

        lRecv += lByte;
    }

    return lRecv;
}

/*************************************************************************************************
    函数说明：在epoll中添加待监听的套接字
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
int     epoll_add_event(SATvm *pstSavm, int epoll_fd, int skSock, void *ptr)
{
    epollevt    event;

    if (0 > epoll_fd)
    {
        if (0 > (epoll_fd = epoll_create(1024)))
        {
            pstSavm->m_lErrno = EPOLL_CRT_ERR;
            return RC_FAIL;
        }
    }

    memset(&event, 0, sizeof(event));
    event.data.fd = skSock;
    event.data.ptr = ptr;
    event.events = EPOLLIN | EPOLLET;

    if(0 != epoll_ctl(epoll_fd, EPOLL_CTL_ADD, skSock, &event))
    {
        pstSavm->m_lErrno = EPOLL_ADD_ERR;
        return RC_FAIL;
    }

    return epoll_fd;
}

long    lEventOperate(SATvm *pstSavm, BSock skSock, TFace *pstFace, char *pvData)
{
    long    lLen = 0;
    void    *pvOut = NULL;

    if(RC_SUCC != lInitSATvm(pstSavm, pstFace->m_table))
    {
        pstFace->m_lRows  = 0;
        pstFace->m_lErrno = pstSavm->m_lErrno;
        return RC_FAIL;
    }
        
//IBPrace("开始查询内存表(%d)(%d), pid(%d)", pstFace->m_table, pstSavm->m_lErrno, getpid());
    pstSavm->pstVoid = pvData;
    pstSavm->lFind   = pstFace->m_lFind;
    pstSavm->lSize   = pstFace->m_lDLen;
    switch(pstFace->m_enum)
    {
    case  OPERATE_SELECT:
        if(RC_SUCC != lSelect(pstSavm, (void *)pvData))
        {
            pstFace->m_lErrno = pstSavm->m_lErrno;
            return RC_FAIL;
        }

        lLen = pstFace->m_lDLen + sizeof(TFace);
        pstFace->m_lRows  = pstSavm->m_lEffect;
        lSendBuffer(skSock, (void *)pstFace, lLen);
//        IBPrace("发送数据成功，长度(%d)===, socket:%d, pid:%d", lLen, skSock, getpid());
        return RC_SUCC;
    case  OPERATE_QUERY:
        if(RC_SUCC != lQuery(pstSavm, (size_t *)&pstFace->m_lRows, (void *)&pvOut))
            pstFace->m_lErrno = pstSavm->m_lErrno;

        lLen = pstFace->m_lDLen * pstFace->m_lRows;
        if(sizeof(TFace) != lSendBuffer(skSock, (void *)pstFace, sizeof(TFace)))
            return RC_FAIL;

        lSendBuffer(skSock, pvOut, lLen);
        IBPFree(pvOut);
        IBPrace("发送数据成功，长度(%d)===, socket:%d, pid:%d", lLen, skSock, getpid());
        return RC_SUCC;
    case  OPERATE_UPDATE:
        pvOut = (void *)(pvData + pstFace->m_lDLen);
        if(RC_SUCC != lUpdate(pstSavm, pvOut, false))
            pstFace->m_lErrno = pstSavm->m_lErrno;

        pstFace->m_lRows  = pstSavm->m_lEffect;
        lSendBuffer(skSock, (void *)pstFace, sizeof(TFace));
        IBPrace("发送数据成功，长度(%d)===, socket:%d, pid:%d", sizeof(TFace), skSock, getpid());
        return RC_SUCC;
    case  OPERATE_DELETE:
        if(RC_SUCC != lDelete(pstSavm))
            pstFace->m_lErrno = pstSavm->m_lErrno;

        pstFace->m_lRows  = pstSavm->m_lEffect;
        lSendBuffer(skSock, (void *)pstFace, sizeof(TFace));
        IBPrace("发送数据成功，长度(%d)===, socket:%d, pid:%d", sizeof(TFace), skSock, getpid());
        return RC_SUCC;
    case  OPERATE_INSERT:
        if(RC_SUCC != lInsert(pstSavm))
            pstFace->m_lErrno = pstSavm->m_lErrno;

        pstFace->m_lRows  = pstSavm->m_lEffect;
        lSendBuffer(skSock, (void *)pstFace, sizeof(TFace));
        IBPrace("发送数据成功，长度(%d)===, socket:%d, pid:%d", sizeof(TFace), skSock, getpid());
        return RC_SUCC;
    default:
        pstFace->m_lRows  = 0;
        pstFace->m_lErrno = RMT_NOT_SUPPT;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：从epoll中删除套接字
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
int     epoll_del_event(SATvm *pstSavm, int epoll_fd, int skSock)
{
    epollevt    event;

    memset(&event, 0, sizeof(event));
    event.data.fd = skSock;

    if (0 != epoll_ctl(epoll_fd, EPOLL_CTL_DEL, skSock, &event))
    {
        pstSavm->m_lErrno = EPOLL_DEL_ERR;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：accepte处理
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lEpollAccept(SATvm *pstSavm, BSock epfd, SKCon *pc)
{
    socklen_t   kLen;
    epollevt    event;
    BSock       skAccept;
    SKCon       *pstCon = NULL;
    struct sockaddr_in cAddr;

    while (1)
    {
        kLen = sizeof(struct sockaddr_in);
//    多进程下产生的“惊群”现象, 只有一个进程返回succ 其他进程都返回errno=EAGAIN
        if(0 > (skAccept = accept(pc->m_skSock, (struct sockaddr *)&cAddr, &kLen)))
            break;

        if (RC_SUCC != lSetUnBlock(skAccept))
        {
            close(skAccept);
            continue;
        }

        if(NULL == (pstCon = (SKCon *)calloc(sizeof(SKCon), 1)))
        {
            close(skAccept);
            fprintf(stderr, "创建内存, err:(%d)(%s)", errno, strerror(errno));
            return RC_FAIL;
        }

        pstCon->m_skSock   = skAccept;
        pstCon->m_lCltPort = ntohs(cAddr.sin_port);
        strncpy(pstCon->m_szCltIp, inet_ntoa(cAddr.sin_addr), sizeof(pstCon->m_szCltIp));

        memset(&event, 0, sizeof(event));
        event.data.ptr = pstCon;
        event.events   = EPOLLIN | EPOLLET;
        if(0 != epoll_ctl(epfd, EPOLL_CTL_ADD, skAccept, &event))
        {
            close(skAccept);
            pstSavm->m_lErrno = EPOLL_ADD_ERR;
            fprintf(stderr, "新增套接字(%d)事件失败, err:(%d)(%s)", skAccept,
                errno, strerror(errno));
            return RC_FAIL;
        }
    }   
    
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取事件请求
    参数说明：
        pstSavm                    --句柄
    返回值：
        无
 *************************************************************************************************/
long    lPollRequest(SATvm *pstSovm, SKCon *pstCon, TFace *pstFace, char *pvData)
{
    long    lRet;
    RunTime *pstRun = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    lRet = lRecvBuffer(pstCon->m_skSock, (char *)pstFace, sizeof(TFace));
    if(RC_CLOSE == lRet)
        return RC_SUCC;
    else if(sizeof(TFace) != lRet)
    {
        if(lRet == 0 && errno == 0)
            return RC_SUCC;
        
        return RC_FAIL;
    }

    if(OPERATE_DMKEEP == pstFace->m_enum)
    {
        lSendBuffer(pstCon->m_skSock, (void *)pstFace, sizeof(TFace));
        return RC_SUCC;
    }

    lRet = pstFace->m_lDLen * pstFace->m_lRows;
    if(lRet > READ_MAX_LEN)
    {
        if(lRet > DATA_MAX_LEN)
            return RC_FAIL;

        if(NULL == (pstFace = (TFace *)realloc(pstFace, lRet)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_SUCC;
        }
    }

    if(lRet != lRecvBuffer(pstCon->m_skSock, pvData, lRet))
        return RC_FAIL;

    //    获取任务后，获取主句柄连接
    pstRun = (RunTime *)pGetRunTime(pstSovm, pstFace->m_table);
    pstRun->m_bAttch = pstSavm->stRunTime[pstFace->m_table].m_bAttch;
    pstRun->m_pvAddr = pstSavm->stRunTime[pstFace->m_table].m_pvAddr;
    lRet = lEventOperate(pstSovm, pstCon->m_skSock, pstFace, pvData);
    pstSavm->stRunTime[pstFace->m_table].m_bAttch = pstRun->m_bAttch;
    pstSavm->stRunTime[pstFace->m_table].m_pvAddr = pstRun->m_pvAddr;

    if(lRet == RC_SUCC)        return RC_SUCC;

    lSendBuffer(pstCon->m_skSock, (void *)pstFace, sizeof(TFace));

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：socket事件处理
    参数说明：
        pstSavm                    --句柄
    返回值：
        无
 *************************************************************************************************/
void*    vEpollListen(void *pvParam)
{
    long        i, nWait = 0;
    SKCon       *pstCon = NULL;
    epollevt    events[MAX_EVENTS];
    BSock       epfd = *((long *)pvParam);
    char        *pvData = NULL, *pvFace = NULL;
    SATvm       stSavm, *pstSavm = (SATvm *)pGetSATvm();

    pthread_detach(pthread_self());
    memset(&stSavm, 0, sizeof(SATvm));
    vTvmHoldConnect(&stSavm);
    if(NULL == (pvFace = (void *)calloc(READ_MAX_LEN, 1)))
    {
        fprintf(stderr, "创建内存, err:(%d)(%s)", errno, strerror(errno));
        return ;
    }

    pvData = pvFace + sizeof(TFace);
    while(1)
    {   
        nWait = epoll_wait(epfd, events, MAX_EVENTS, 5000);
        for(i = 0; i < nWait; i++)
        {   
            pstCon = (SKCon *)events[i].data.ptr;
            if(pstCon->m_isListen)      //  新的套接字
            {
                lEpollAccept(&stSavm, epfd, pstCon);
                fprintf(stderr, "监听事件(SKSOCK:%d)\n", pstCon->m_skSock);
            }
            else if(events[i].events & EPOLLIN)
            {
                if(RC_FAIL == lPollRequest(&stSavm, pstCon, (TFace *)pvFace, pvData))
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, pstCon->m_skSock, &events[i]);
                    close(pstCon->m_skSock);
                }
            }
        }
    }

    close(epfd);
    IBPFree(pvFace);

    return ;
}

/*************************************************************************************************
    函数说明：socket事件处理
    参数说明：
        pstSavm                    --句柄
    返回值：
        无
 *************************************************************************************************/
void    vMultListen(SATvm *pstSavm, BSock epfd, FUNCEXEC plMultListen)
{
    long        i, nWait = 0;
    SKCon       *pstCon = NULL;
    epollevt    events[MAX_EVENTS];

    pthread_detach(pthread_self());
    vTvmHoldConnect(pstSavm);
    while(1)
    {   
        nWait = epoll_wait(epfd, events, MAX_EVENTS, 5000);
        for(i = 0; i < nWait; i++)
        {   
            pstCon = (SKCon *)events[i].data.ptr;
            if(pstCon->m_isListen)      //  新的套接字
                lEpollAccept(pstSavm, epfd, pstCon);
            else if(events[i].events & EPOLLIN)
            {
                if(RC_CLOSE == plMultListen(pstSavm, pstCon))
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, pstCon->m_skSock, &events[i]);
                    close(pstCon->m_skSock);
                }
            }
        }
    }

    close(epfd);
    vTvmHoldRelease(pstSavm);
    return ;
}

/*************************************************************************************************
    函数说明：fork多进程
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lMultListen(SATvm *pstSavm, long lPort, long lProcess, FUNCEXEC plMultInitail, void *arg, 
            FUNCEXEC plMultListen)
{
    int            i;
    pid_t        lPid;
    epollevt    event;
    BSock        epfd = -1;
    SKCon        *pstCon = NULL;

    if(NULL == (pstCon = (SKCon *)calloc(sizeof(SKCon), 1)))
    {
        fprintf(stderr, "创建内存, err:(%d)(%s)", errno, strerror(errno));
        return RC_FAIL;
    }

    pstCon->m_isListen = 1;
    if(0 > (pstCon->m_skSock = skServerInitail(pstSavm, lPort)))
        return RC_FAIL;

    for(i = 0; i < lProcess; i ++)
    {
        if(0 > (lPid = fork()))
        {
            close(pstCon->m_skSock);
            return RC_FAIL;
        }
        else if(lPid > 0)
            continue;
    
        epfd = epoll_create(MAX_EVENTS);

        memset(&event, 0, sizeof(event));
        event.data.ptr = pstCon;
        event.events   = EPOLLIN | EPOLLET;
        if(0 != epoll_ctl(epfd, EPOLL_CTL_ADD, pstCon->m_skSock, &event))
        {
            pstSavm->m_lErrno = EPOLL_ADD_ERR;
            return RC_FAIL;
        }

        if(plMultInitail && RC_SUCC != plMultInitail(pstSavm, (void *)arg))
        {
            pstSavm->m_lErrno = INI_ERR_CHLDP;
            return RC_FAIL;
        }
        
        vMultListen(pstSavm, epfd, plMultListen);
        exit(0);
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：fork多进程监听
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lBootLocal(SATvm *pstSavm, TBoot *pstBoot)
{
    int            i;
    pid_t        lPid;
    pthread_t    tPid;
    epollevt    event;
    BSock        epfd = -1;
    pthread_attr_t  tattr;
    SKCon        *pstCon = NULL;

    if(!pstBoot || pstBoot->m_lSvrBoot < 1)    //    线程数量
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(TVM_BOOT_SIMPLE == pstBoot->m_lBootType)
        return RC_SUCC;

/*
    pthread_mutex_init(&mutex_thread_read, NULL);
    pthread_cond_init(&cond_thread_read, NULL);
*/
    if(NULL == (pstCon = (SKCon *)calloc(sizeof(SKCon), 1)))
    {
        fprintf(stderr, "创建内存, err:(%d)(%s)", errno, strerror(errno));
        return RC_FAIL;
    }

    pstCon->m_isListen = 1;
    if(0 > (pstCon->m_skSock = skServerInitail(pstSavm, pstBoot->m_lSvrPort)))
        return RC_FAIL;

    vSetTitile(TVM_LOCAL_SERV);
    fprintf(stdout, "Boot process(%s), total %d process\n", TVM_LOCAL_SERV, pstBoot->m_lSvrBoot);
    if(0 > (lPid = fork()))
    {
        close(pstCon->m_skSock);
        return RC_FAIL;
    }
    else if(lPid > 0)
    {
        usleep(500);
        return RC_SUCC;
    }

    epfd = epoll_create(MAX_EVENTS);

    memset(&event, 0, sizeof(event));
    event.data.ptr = pstCon;
    event.events   = EPOLLIN | EPOLLET;
    if(0 != epoll_ctl(epfd, EPOLL_CTL_ADD, pstCon->m_skSock, &event))
    {
        fprintf(stderr, "新增套接字(%d)事件失败, err:(%d)(%s)", pstCon->m_skSock,
            errno, strerror(errno));
        return -1;
    }
    
    vTvmHoldConnect(pstSavm);
    for(i = 0; i < pstBoot->m_lSvrBoot; i ++)
    {
//        if(0 != pthread_create(&tPid, &tattr, vEpollListen, (void*)&epfd))
        if(0 != pthread_create(&tPid, NULL, vEpollListen, (void*)&epfd))
        {
            fprintf(stderr, "pthread_create error, errstr:%s.\n", strerror(errno));
            exit(-1);
        }
    }

    fprintf(stdout, "   process %s id=%d ... success\n", TVM_LOCAL_SERV, getpid());
    fflush(stdout);
    while(1)
    {
        pause();
        fprintf(stderr, "线程异常退出， 重新创建线程"); 
    //    ……
    }

    vTvmDisconnect(pstSavm);

    exit(-1);
}





























BOOL    bCheckDomain(CMList *pstRoot, TDomain *pstDom)
{
    TDomain    *pm = NULL;
    CMList  *pstNode = NULL;

    for(pstNode = pstRoot; pstNode && pstDom; pstNode = pstNode->pstNext) 
    {
        pm = (TDomain *)pstNode->m_psvData;
        if(!strcmp(pm->m_szTable, pstDom->m_szTable) && !strcmp(pm->m_szPart, pstDom->m_szPart) 
            && pm->m_table != pstDom->m_table)
        {
            fprintf(stderr, "配置错误，映射本地表序号不一致\n");
            return false;
        }
    }

    return true;
}

BOOL    bExistDomain(CMList *pstRoot, TDomain *pstDom, BOOL bf)
{
    TDomain    *pm = NULL;
    CMList  *pstNode = NULL;

    for(pstNode = pstRoot; pstNode && pstDom; pstNode = pstNode->pstNext) 
    {
        pm = (TDomain *)pstNode->m_psvData;
        if(bf)
        {
            if(!strcmp(pm->m_szIp, pstDom->m_szIp) && !strcmp(pm->m_szTable, pstDom->m_szTable) 
                && !strcmp(pm->m_szPart, pstDom->m_szPart) && pm->m_lRDPort == pstDom->m_lRDPort)
                return true;

            if(!strcmp(pm->m_szIp, pstDom->m_szIp) && !strcmp(pm->m_szTable, pstDom->m_szTable) 
                && !strcmp(pm->m_szPart, pstDom->m_szPart) && pm->m_lRDPort == pstDom->m_lRDPort)
                return true;
        }
        else
        {
            if(!strcmp(pm->m_szIp, pstDom->m_szIp) && pm->m_lRDPort == pstDom->m_lRDPort)
                return true;
        }
    }

    return false;
}

TDomain*    pGetDomain(char *pszIp, long lPort)
{
    CMList  *pstNode = NULL;
    TDomain *pstDomain = NULL;

    if(!g_pstDomain)    return NULL;

    for(pstNode = g_pstDomain; pstNode; pstNode = pstNode->pstNext)
    {
        pstDomain = (TDomain *)pstNode->m_psvData;
        if(!strcmp(pstDomain->m_szIp, pszIp) && pstDomain->m_lRDPort == lPort)
            return pstDomain;
    }

    return NULL;
}

long    lUpdateDomain(SATvm *pstSavm, char *pszIp, long lPort, long lStatus)
{
    size_t    i, lOut = 0;
    TDomain   *pstDomain = NULL, stDomain;

    memset(&stDomain, 0, sizeof(TDomain));
    if(RC_SUCC != lInitSATvm(pstSavm, SYS_TVM_DOMAIN))
        return RC_FAIL;

    stDomain.m_lRDPort = lPort;
    strncpy(stDomain.m_szIp, pszIp, sizeof(stDomain.m_szIp));

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(TDomain);
    pstSavm->pstVoid = (void *)&stDomain;
    if(RC_SUCC != lQuery(pstSavm, &lOut, (void *)&pstDomain))
        return RC_FAIL;

    for(i = 0; i < lOut; i ++)
    {
        memcpy(&stDomain, &pstDomain[i], sizeof(TDomain));
        stDomain.m_lStatus = lStatus;

        pstSavm->pstVoid = (void *)&pstDomain[i];
        if(RC_SUCC != lUpdate(pstSavm, (void *)&stDomain, true))
        {
            IBPFree(pstDomain);    
            return RC_FAIL;
        }
    }

    IBPFree(pstDomain);

    return RC_SUCC;
}

void    vResetRemote(SATvm *pstSavm, char *pszIp, long lPort, BSock skSock)
{
    CMList  *pstNode = NULL;
    TDomain *pstDomain = NULL;

    if(!g_pstDomain)    return ;

    for(pstNode = g_pstDomain; pstNode; pstNode = pstNode->pstNext)
    {
        pstDomain = (TDomain *)pstNode->m_psvData;
        if(strcmp(pstDomain->m_szIp, pszIp) || pstDomain->m_lRDPort != lPort)
            continue;

        pstDomain->m_lTryTimes = 0;
        pstDomain->m_skSock = skSock;
        pstDomain->m_lLastTime = (long)time(NULL);
        if(RESOURCE_ABLE != pstDomain->m_lStatus)
        {
            pstDomain->m_lStatus = RESOURCE_ABLE;
            lUpdateDomain(pstSavm, pszIp, lPort, RESOURCE_ABLE);
        }
    }

    return ;
}

long    _lRemodeIndex(SATvm *pstSavm, TRSync *pstSync, TDomain *pstDomain)
{
    long    i, lRecv = 0;
    TIndex    *pstIndex = NULL;    
    TDomain    stDomain, stRemote;

    //    无论是否有记录，先将远程LISP 保存
    pstDomain->m_lRIPort  = pstSync->m_lOffset;
    if(0 == pstSync->m_lRows)
           return RC_SUCC;

    if(NULL == (pstIndex = (TIndex *)calloc(pstSync->m_lRows, sizeof(TIndex))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    lRecv = pstSync->m_lRows * sizeof(TIndex);
    if(lRecv != lRecvBuffer(pstDomain->m_skSock, (void *)pstIndex, lRecv))
    {
        IBPFree(pstIndex);
        return RC_FAIL;
    }

    if(RC_SUCC != lInitSATvm(pstSavm, SYS_TVM_DOMAIN))
    {
        IBPFree(pstIndex);
        return RC_FAIL;
    }

    //    更新本地表记录
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(TDomain);
    pstSavm->pstVoid = (void *)&stDomain;
    for(i = 0; i < pstSync->m_lRows; i ++)
    {
        memset(&stDomain, 0, sizeof(TDomain));
        memset(&stRemote, 0, sizeof(TDomain));
        stDomain.m_lRDPort = pstDomain->m_lRDPort;
        strncpy(stDomain.m_szIp, pstDomain->m_szIp, sizeof(stDomain.m_szIp));
        strncpy(stDomain.m_szPart, pstIndex[i].m_szPart, sizeof(stDomain.m_szPart));
        strncpy(stDomain.m_szTable, pstIndex[i].m_szTable, sizeof(stDomain.m_szTable));

        stRemote.m_lRIPort  = pstSync->m_lOffset;
        stRemote.m_skSock   = pstDomain->m_skSock;
        stRemote.m_lPers    = pstIndex[i].m_lPers;
        stRemote.m_mtable   = pstIndex[i].m_table;
        stRemote.m_lRowSize = pstIndex[i].m_lRowSize;

        if(RC_SUCC != lUpdate(pstSavm, (void *)&stRemote, false))
        {
            if(NO_DATA_FOUND == pstSavm->m_lErrno)
                continue;    

            IBPFree(pstIndex);
            return RC_FAIL;
        }
    }
    IBPFree(pstIndex);

    return RC_SUCC;
}

long    _lLocalIndex(SATvm *pstSavm, TRSync *pstSync, BSock skSock, int time)
{
    long    lLen;
    TIndex  stIndex;
    void    *pvOut = NULL;

    if(RC_SUCC != lInitSATvm(pstSavm, pstSync->m_table))
    {
        pstSync->m_lRows  = 0;
        pstSync->m_lErrno = pstSavm->m_lErrno;
        return RC_CONTINUE;
    }

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_lType = TYPE_CLIENT;
    stIndex.m_lLocal = RES_LOCAL_SID;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;
    if(RC_SUCC != lQuery(pstSavm, (size_t *)&pstSync->m_lRows, (void *)&pvOut))
        pstSync->m_lErrno = pstSavm->m_lErrno;

    lLen = pstSync->m_lDLen * pstSync->m_lRows;
    if(sizeof(TRSync) != lSendBuffer(skSock, (void *)pstSync, sizeof(TRSync)))
    {
        IBPFree(pvOut);
        return RC_FAIL;
    }

    if(lLen != lSendBuffer(skSock, pvOut, lLen))
    {
        IBPFree(pvOut);
        return RC_FAIL;
    }

    IBPFree(pvOut);

    return RC_SUCC;
}

long    lDomainNotify(SATvm *pstSavm, TDomain *pstDomain)
{
    int     ntime = 5;
    static    TRSync    stSync;

    close(pstDomain->m_skSock);
    ntime = pstDomain->m_lTimeOut;
    pstDomain->m_skSock = skConnectServer(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, 
        false, ntime);
    if(RC_FAIL == pstDomain->m_skSock)
    {
        pstDomain->m_lStatus = RESOURCE_STOP;
        lUpdateDomain(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, RESOURCE_STOP);
        return RC_FAIL;
    }

    memset(&stSync, 0, sizeof(TRSync));
    stSync.m_enum  = OPERATE_DOMPUL;
    stSync.m_lDLen = sizeof(TIndex);
    stSync.m_table = SYS_TVM_INDEX;
    stSync.m_lPort = pstDomain->m_lLDPort;

    if(sizeof(TRSync) != lSendBuffer(pstDomain->m_skSock, (void *)&stSync, sizeof(TRSync)))
    {
        pstDomain->m_lStatus = RESOURCE_EXCP;
        goto EXCP_NOTIFY;
    }

    if(sizeof(TRSync) != lRecvBuffer(pstDomain->m_skSock, (char *)&stSync, sizeof(TRSync)))
    {
        pstDomain->m_lStatus = RESOURCE_EXCP;
        goto EXCP_NOTIFY;
    }

       pstSavm->m_lErrno = stSync.m_lErrno;
       pstSavm->m_lEffect = stSync.m_lRows;
    if(DOM_NOT_REGST == pstSavm->m_lErrno)
    {
        pstDomain->m_lStatus = RESOURCE_AUTH;    
        goto EXCP_NOTIFY;
    }
    else if(0 != pstSavm->m_lErrno && NO_DATA_FOUND != pstSavm->m_lErrno)
    {    
        pstDomain->m_lStatus = RESOURCE_EXCP;
        goto EXCP_NOTIFY;
    }

IBPrace("发送事件请求 OPERATE_DOMPUL：%s:%d, Row(%d)", pstDomain->m_szIp, pstDomain->m_lRDPort, stSync.m_lRows);
    if(RC_SUCC != _lRemodeIndex(pstSavm, &stSync, pstDomain))
    {
        pstDomain->m_lStatus = RESOURCE_EXCP;
        goto EXCP_NOTIFY;
    }

    memset(&stSync, 0, sizeof(TRSync));
    stSync.m_enum   = OPERATE_DOMPSH;
    stSync.m_lDLen  = sizeof(TIndex);
    stSync.m_table  = SYS_TVM_INDEX;
    stSync.m_lPort  = pstDomain->m_lLDPort;
    stSync.m_lOffset= pstDomain->m_lLIPort;

IBPrace("发送事件请求 OPERATE_DOMPSH：%s:%d, enum(%d)", pstDomain->m_szIp, pstDomain->m_lRDPort, stSync.m_enum);
    if(RC_SUCC != _lLocalIndex(pstSavm, &stSync, pstDomain->m_skSock, ntime))
        return RC_FAIL;

    pstDomain->m_lTryTimes = 0;
    pstDomain->m_lLastTime = (long)time(NULL);
    pstDomain->m_lStatus = RESOURCE_ABLE;
    lUpdateDomain(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, RESOURCE_ABLE);
    return RC_SUCC;

EXCP_NOTIFY:
IBPrace("发送事件请求：%s:%d,  (%s)", pstDomain->m_szIp, pstDomain->m_lRDPort, sGetTError(pstSavm->m_lErrno));
    lUpdateDomain(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, pstDomain->m_lStatus);
    pstDomain->m_lTryTimes = pstDomain->m_lTryMax;
    return RC_FAIL;
}

long    lActiveOffline(SATvm *pstSavm)
{
    int        ntime;
    TRSync    stSync;
    CMList  *pstNode = NULL;
    TDomain    *pstDomain = NULL;

    for(pstNode = g_pstDomain; pstNode; pstNode = pstNode->pstNext)
    {
        pstDomain = (TDomain *)pstNode->m_psvData;
        if(!pstDomain)    continue;

        if(RESOURCE_ABLE != pstDomain->m_lStatus)
        {
            close(pstDomain->m_skSock);
            pstDomain->m_skSock = -1;
            continue;
        }

        while(DATA_TRUCK_LOCK == pstDomain->m_lLock)
            usleep(10);

        pstDomain->m_lLock = DATA_TRUCK_LOCK;
        ntime = pstDomain->m_lTimeOut;
        memset(&stSync, 0, sizeof(TRSync));
        stSync.m_enum = OPERATE_DOMOFF;
        stSync.m_lPort = pstDomain->m_lLDPort;
        lSendBuffer(pstDomain->m_skSock, (void *)&stSync, sizeof(TRSync));
        lRecvBuffer(pstDomain->m_skSock, (char *)&stSync, sizeof(TRSync));
        close(pstDomain->m_skSock);

        pstDomain->m_lLock = DATA_TRUCK_NULL;
        pstDomain->m_skSock = -1;
    }

    return RC_SUCC;
}

long    lRefreshDomain(SATvm *pstSavm)
{
    CMList  *pstNode = NULL;
    TDomain *pstDomain = NULL;

    for(pstNode = g_pstDomain; pstNode; pstNode = pstNode->pstNext)
    {
        pstDomain = (TDomain *)pstNode->m_psvData;
        if(!pstDomain)  continue;

        if(RESOURCE_ABLE != pstDomain->m_lStatus)
        {
            close(pstDomain->m_skSock);
            pstDomain->m_skSock = -1;
            continue;
        }

        while(DATA_TRUCK_LOCK == pstDomain->m_lLock)
            usleep(10);
        pstDomain->m_lLock = DATA_TRUCK_LOCK;
        lDomainNotify(pstSavm, (TDomain *)pstNode->m_psvData);
        pstDomain->m_lLock = DATA_TRUCK_NULL;
    }

    return RC_SUCC;
}

long    lPullNotify(SATvm *pstSavm, TDomain *pstDomain, TABLE t)
{
    TblDef  stDef;
    BSock   skSock;
    TRSync  stSync;
    void    *pvData = NULL;
    int     i, j, ntime, nRecord;

    memset(&stDef, 0, sizeof(TblDef));
    memset(&stSync, 0, sizeof(TRSync));
    if(NULL == (pvData = (char *)malloc(pstDomain->m_lRowSize)))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    stSync.m_enum  = OPERATE_PULTBL;
    stSync.m_lDLen = pstDomain->m_lRowSize;
    stSync.m_table = pstDomain->m_mtable;
    stSync.m_lPort = pstDomain->m_lLDPort;

    ntime = pstDomain->m_lTimeOut;
    skSock = skConnectServer(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, false, ntime);
    if(RC_FAIL == skSock)
        return RC_FAIL;

    if(sizeof(TRSync) != lSendBuffer(skSock, (void *)&stSync, sizeof(TRSync)))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    if(sizeof(TblDef) != lRecvBuffer(skSock, (char *)&stDef, sizeof(TblDef)))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    nRecord = stDef.m_lValid;
    stDef.m_lValid = 0;
    stDef.m_lGroup = 0;
    if(RC_SUCC != lCustomTable(pstSavm, t, stDef.m_lMaxRow, &stDef))
    {
        fprintf(stderr, "创建表(%d)失败, err:(%d)(%s)\n", t, pstSavm->m_lErrno, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    fprintf(stdout, "\n克隆表(%d)空间成功, 最大支持记录(%d), completed ..\n", t, stDef.m_lMaxRow);
    fflush(stdout);

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(t)))
    {
        fprintf(stderr, "初始化表(%s)错误, err:(%d)(%s)\n", pstDomain->m_szTable, lGetTErrno(), 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = stSync.m_lDLen;
    pstSavm->pstVoid = (void *)pvData;
 
    fprintf(stdout, "开始拷贝表(%s)(%s)记录:[", pstDomain->m_szTable, pstDomain->m_szPart);
    fprintf(stdout, "\033[?25l");
    for(i = 1; i <= nRecord; i ++)
    {
        ntime = pstDomain->m_lTimeOut;
        if(stSync.m_lDLen != lRecvBuffer(skSock, (char *)pvData, stSync.m_lDLen))
            goto ERR_PULLNOTIFY;
        
        if(RC_SUCC != lInsert(pstSavm))
            goto ERR_PULLNOTIFY;

        vPrintProgresss(i, nRecord);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "\033[?25h");
    fprintf(stdout, "拷贝表(%s)(%s)成功，导入记录(%d), completed successfully!!!\n", 
        pstDomain->m_szTable, pstDomain->m_szPart, nRecord);
    fflush(stdout);

    vCloseSocket(skSock);
    return RC_SUCC;

ERR_PULLNOTIFY:
    fprintf(stdout, "\n");
    fprintf(stdout, "\033[?25h");
    fflush(stdout);
    vCloseSocket(skSock);
    return RC_FAIL;
}

long    lPushTable(SATvm *pstSavm, TRSync *pstSync, BSock skSock)
{
    int     lRet, ntimes;
    void    *pvData = NULL, *p = pGetTblDef(pstSync->m_table);

    if(pstSync->m_lDLen <= 0 || pstSync->m_table <= 0)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pvData = (char *)malloc(pstSync->m_lDLen)))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(pstSync->m_table)))
        return RC_FAIL;

    pstSavm->lFind   = CHK_SELECT;
    pstSavm->lSize   = pstSync->m_lDLen;
    if(RC_SUCC != lTableDeclare(pstSavm, pstSync->m_table))
        return RC_FAIL;

    /*  发送表头节点 */
    if(sizeof(TblDef) != lSendBuffer(skSock, (void *)p, sizeof(TblDef)))
        return RC_FAIL;

    while(1)
    {
        lRet = lTableFetch(pstSavm, pstSync->m_table, (void *)pvData);
        if(RC_FAIL == lRet)
        {
            vTableClose(pstSavm);
            return RC_FAIL;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        ntimes = 5;
        if(pstSync->m_lDLen != lSendUnblock(pstSavm, skSock, (void *)pvData, 
            pstSync->m_lDLen, &ntimes))
        {
            vTableClose(pstSavm);
            return RC_FAIL;
        }
    }

    vTableClose(pstSavm);

    return RC_SUCC;
}

long    lDomainOperate(SATvm *pstSavm, SKCon *pstCon, TRSync *pstSync)
{
    long    lRet;
    TDomain stDomain, *pstDomain = NULL;

    switch(pstSync->m_enum)
    {
    case  OPERATE_DMKEEP:
        lSendBuffer(pstCon->m_skSock, (void *)pstSync, sizeof(TRSync));
        vResetRemote(pstSavm, pstCon->m_szCltIp, pstSync->m_lPort, pstCon->m_skSock);
IBPrace("RDS: 回复心跳包数据成功");
        return RC_SUCC;
    case  OPERATE_DOMPSH:
        if(NULL == (pstDomain = pGetDomain(pstCon->m_szCltIp, pstSync->m_lPort)))
            return RC_FAIL;

        pstDomain->m_skSock = pstCon->m_skSock;
        pstDomain->m_lRIPort= pstSync->m_lOffset;
        pstSync->m_lPort    = pstDomain->m_lLDPort;
        pstSync->m_lOffset  = pstDomain->m_lLIPort;

IBPrace("获取客户端事件[OPERATE_DOMPSH]请求, 记录(%d)===, socket:%d, (%s:%d)", pstSync->m_lRows, pstCon->m_skSock, stDomain.m_szIp, stDomain.m_lRDPort);

        if(RC_SUCC != _lRemodeIndex(pstSavm, pstSync, pstDomain))
            return RC_FAIL;

        pstDomain->m_lTryTimes = 0;
        pstDomain->m_lLastTime = (long)time(NULL);

        return RC_SUCC;
    case  OPERATE_DOMPUL:
IBPrace("获取客户端事件[OPERATE_DOMPUL]请求, 记录(%d)===, socket:%d, (%s:%d)", pstSync->m_lRows, pstCon->m_skSock, pstCon->m_szCltIp, pstSync->m_lPort);
        if(NULL == (pstDomain = pGetDomain(pstCon->m_szCltIp, pstSync->m_lPort)))
            return RC_FAIL;

        //  无论是否有记录，先将远程LISP 保存
        pstDomain->m_skSock = pstCon->m_skSock;
        pstDomain->m_lRIPort= pstSync->m_lOffset;
        pstSync->m_lPort    = pstDomain->m_lLDPort;
        pstSync->m_lOffset  = pstDomain->m_lLIPort;

        lRet = _lLocalIndex(pstSavm, pstSync, pstCon->m_skSock, pstDomain->m_lTimeOut);
        if(RC_CONTINUE == lRet || RC_FAIL == lRet)
            return lRet;

        pstDomain->m_lTryTimes = 0;
        pstDomain->m_lLastTime = (long)time(NULL);
        if(RESOURCE_ABLE != pstDomain->m_lStatus)
        {
            pstDomain->m_lStatus = RESOURCE_ABLE;
            lUpdateDomain(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, RESOURCE_ABLE);
        }

        return RC_SUCC;
    case  OPERATE_DMRECN:
        memset(&stDomain, 0, sizeof(TDomain));
        if(sizeof(TDomain) != lRecvBuffer(pstCon->m_skSock, (void *)&stDomain, sizeof(TDomain)))
            return RC_FAIL;

IBPrace("获取客户端事件[OPERATE_DMRECN]请求(%s:%d)", stDomain.m_szIp, stDomain.m_lRDPort);
        if(NULL == (pstDomain = pGetDomain(stDomain.m_szIp, stDomain.m_lRDPort)))
            return RC_FAIL;

        lDomainNotify(pstSavm, pstDomain);
        lSendBuffer(pstCon->m_skSock, (void *)pstSync, sizeof(TRSync));
        return RC_SUCC;
    case  OPERATE_REFRESH:
        lSendBuffer(pstCon->m_skSock, (void *)pstSync, sizeof(TRSync));
        lRefreshDomain(pstSavm);
        return RC_SUCC;
    case  OPERATE_DOMOFF:
        if(!strcmp(LOCAL_HOST_IP, pstCon->m_szCltIp))
        {
            memset(&stDomain, 0, sizeof(TDomain));
            if(sizeof(TDomain) != lRecvBuffer(pstCon->m_skSock, (void *)&stDomain, sizeof(TDomain)))
                return RC_FAIL;

IBPrace("获取客户端事件[OPERATE_DOMOFF](%s)", pstCon->m_szCltIp);
            lActiveOffline(pstSavm);

            lSendBuffer(pstCon->m_skSock, (void *)pstSync, sizeof(TRSync));
            return RC_SUCC;
        }

        if(NULL == (pstDomain = pGetDomain(pstCon->m_szCltIp, pstSync->m_lPort)))
            return RC_FAIL;

        pstDomain->m_lStatus = RESOURCE_STOP;
        pstDomain->m_lTryTimes = pstDomain->m_lTryMax;
        if(RC_SUCC != lUpdateDomain(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, RESOURCE_STOP))
            return lRet;

IBPrace("获取客户端事件[OPERATE_DOMOFF]请求, 设置主机(%s:%d)离线完毕", pstDomain->m_szIp, pstDomain->m_lRDPort);
        lSendBuffer(pstCon->m_skSock, (void *)pstSync, sizeof(TRSync));
        return RC_SUCC;
    case  OPERATE_PULTBL:
IBPrace("获取客户端事件[OPERATE_PULTBL](%s)", pstCon->m_szCltIp);
        if(RC_SUCC != lPushTable(pstSavm, pstSync, pstCon->m_skSock))
            return RC_FAIL;

        return RC_SUCC;
    default:
        pstSync->m_lRows  = 0;
        pstSync->m_lErrno = RMT_NOT_SUPPT;
        return RC_CONTINUE;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：数据处理
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lDomainEvent(SATvm *pstSavm, SKCon *pstCon)
{
    long    lRet;
    char    szBuffer[READ_MAX_LEN];    
    TRSync    *pstSync = (TRSync *)szBuffer;

    lRet = lRecvBuffer(pstCon->m_skSock, (char *)pstSync, sizeof(TRSync));
    if(RC_CLOSE == lRet)
        return RC_SUCC;
    else if(sizeof(TRSync) != lRet)
    {
        fprintf(stderr, "读取数据失败, err:(lRet)(%d)(%s)", errno, strerror(errno));
        return RC_FAIL;
    }

IBPrace("lDomainEvent pid:%d, m_enum[%d],m_lFind[%d],m_table[%d],m_lRows[%d],m_lDLen[%d],m_lNLen[%d],"
    "m_lCLine[%d],m_lCType[%d],m_lErrno[%d], m_lPort[%d], SOCK:%d", errno, getpid(), 
    pstSync->m_enum, pstSync->m_lFind, pstSync->m_table, pstSync->m_lRows, pstSync->m_lDLen, pstSync->m_lNLen, pstSync->m_lCLine, 
    pstSync->m_lCType, pstSync->m_lErrno, pstSync->m_lPort, pstCon->m_skSock);

    if(strcmp(LOCAL_HOST_IP, pstCon->m_szCltIp) && 
        NULL == pGetDomain(pstCon->m_szCltIp, pstSync->m_lPort))
    {
IBPrace("ADDREE:%s:%d 未登记", pstCon->m_szCltIp, pstSync->m_lPort);
        pstSync->m_lErrno = DOM_NOT_REGST;
        lSendBuffer(pstCon->m_skSock, (void *)pstSync, sizeof(TRSync));
        return RC_FAIL;
    }

    if(RC_SUCC != lDomainOperate(pstSavm, pstCon, pstSync))
        return RC_SUCC;

IBPrace("异常时间，恢复包!!!!!!!!!!!!!!!!!");
    lSendBuffer(pstCon->m_skSock, (void *)pstSync, sizeof(TRSync));

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：domain事件处理
    参数说明：
        pstSavm                    --句柄
    返回值：
        无
 *************************************************************************************************/
void    vDomainListen(SATvm *pstSavm, int epfd)
{
    long        i, nWait = 0;
    SKCon       *pstCon = NULL;
    epollevt    stEvent[MAX_EVENTS];

    while(1)
    {   
        nWait = epoll_wait(epfd, stEvent, MAX_EVENTS, 500);
        for (i = 0; i < nWait; i++)
        {   
            pstCon = (SKCon *)stEvent[i].data.ptr;
            if(pstCon->m_isListen)
                lEpollAccept(pstSavm, epfd, pstCon);
            else
            {
                if(RC_SUCC > lDomainEvent(pstSavm, pstCon))
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, pstCon->m_skSock, &stEvent[i]);
                    close(pstCon->m_skSock);
                }
            }
        }
    }

    close(epfd);

    return ;
}

long    lOfflineNotify(SATvm *pstSavm, long lPort)
{
    BSock    skSock;
    char    szBuffer[2048];
    int        lLen, times = 5;
    TDomain    *pstDomain = (TDomain *)(szBuffer + sizeof(TRSync));

    memset(szBuffer, 0, sizeof(szBuffer));
    ((TRSync *)szBuffer)->m_enum   = OPERATE_DOMOFF;

    pstDomain->m_lLDPort = lPort;
    strcpy(pstDomain->m_szIp, LOCAL_HOST_IP);

IBPrace("开始发送离线指令LDS:%s:%d状态", pstDomain->m_szIp, pstDomain->m_lLDPort);
    skSock = skConnectServer(pstSavm, pstDomain->m_szIp, pstDomain->m_lLDPort, false, times);
    if(RC_FAIL == skSock)
        return RC_FAIL;

    lLen = sizeof(TDomain) + sizeof(TRSync);
    if(lLen != lSendBuffer(skSock, (void *)szBuffer, lLen))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    if(sizeof(TRSync) != lRecvBuffer(skSock, (char *)szBuffer, sizeof(TRSync)))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    vCloseSocket(skSock);

    pstSavm->m_lErrno = ((TRSync *)szBuffer)->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    return RC_SUCC;
}

long    lRefreshNotify(SATvm *pstSavm, long lPort)
{
    BSock    skSock;
    TRSync    stSync;
    int        ntime = 5;

    memset(&stSync, 0, sizeof(TRSync));
    stSync.m_enum   = OPERATE_REFRESH;

IBPrace("开始发送刷新指令LDS:%s:%d状态, ntime(%d)", LOCAL_HOST_IP, lPort, ntime);
    skSock = skConnectServer(pstSavm, LOCAL_HOST_IP, lPort, false, ntime);
    if(RC_FAIL == skSock)
        return RC_FAIL;

    if(sizeof(TRSync) != lSendBuffer(skSock, (void *)&stSync, sizeof(TRSync)))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    if(sizeof(TRSync) != lRecvBuffer(skSock, (char *)&stSync, sizeof(TRSync)))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }
    vCloseSocket(skSock);

    pstSavm->m_lErrno = stSync.m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    return RC_SUCC;
}

long    lConnectNotify(SATvm *pstSavm, TDomain *pstDomain)
{
    BSock    skSock;
    TRSync    stSync;
    int        ntime;

    ntime = pstDomain->m_lTimeOut;
    memset(&stSync, 0, sizeof(TRSync));
    stSync.m_enum   = OPERATE_DMRECN;
    stSync.m_lPort  = pstDomain->m_lLDPort;
    stSync.m_lOffset= pstDomain->m_lLIPort;

IBPrace("开始发送指令LIS:127.0.0.1:%d状态, ntime(%d)", pstDomain->m_lLDPort, ntime);
    skSock = skConnectServer(pstSavm, LOCAL_HOST_IP, pstDomain->m_lLDPort, false, ntime);
    if(RC_FAIL == skSock)
        return RC_FAIL;

    if(sizeof(TRSync) != lSendBuffer(skSock, (void *)&stSync, sizeof(TRSync)))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    if(sizeof(TDomain) != lSendBuffer(skSock, (void *)pstDomain, sizeof(TDomain)))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    if(sizeof(TRSync) != lRecvBuffer(skSock, (char *)&stSync, sizeof(TRSync)))
    {
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    vCloseSocket(skSock);

    pstSavm->m_lErrno = stSync.m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    return RC_SUCC;
}

long    lBreakHeart(SATvm *pstSavm, TDomain *pstDomain)
{
    BSock    skSock;
    TRSync    stSync;
    int        ntime;

    ntime = pstDomain->m_lTimeOut;
    memset(&stSync, 0, sizeof(TRSync));
    stSync.m_enum   = OPERATE_DMKEEP;
    stSync.m_lPort  = pstDomain->m_lLDPort;
    stSync.m_lOffset= pstDomain->m_lLIPort;
    pstDomain->m_lLastTime = (long)time(NULL);        

IBPrace("开始检查RDS:%s:%d状态, try(%d)/%d", pstDomain->m_szIp, pstDomain->m_lRDPort, pstDomain->m_lTryTimes, pstDomain->m_lTryMax);
    if(sizeof(TRSync) != lSendBuffer(pstDomain->m_skSock, (void *)&stSync, sizeof(TRSync)))
    {
IBPrace("发送心跳包失败RDS:%s:%d状态, errno:(%d)", pstDomain->m_szIp, pstDomain->m_lRDPort, errno);
        pstDomain->m_lTryTimes ++;
        return RC_FAIL;
    }

    if(sizeof(TRSync) != lRecvBuffer(pstDomain->m_skSock, (char *)&stSync, sizeof(TRSync)))
    {
IBPrace("接收心跳包失败RDS:%s:%d状态, errno:(%d)", pstDomain->m_szIp, pstDomain->m_lRDPort, errno);
        pstDomain->m_lTryTimes ++;
        return RC_FAIL;
    }

    ntime = pstDomain->m_lTimeOut;
IBPrace("开始检查LIS:%s:%d状态, try(%d)/%d, ntime(%d)", pstDomain->m_szIp, pstDomain->m_lRIPort, pstDomain->m_lTryTimes, pstDomain->m_lTryMax, ntime);
    skSock = skConnectServer(pstSavm, pstDomain->m_szIp, pstDomain->m_lRIPort, false, ntime);
    if(RC_FAIL == skSock)
    {
        pstDomain->m_lStatus = RESOURCE_STOP;
        lUpdateDomain(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, RESOURCE_STOP);
        return RC_FAIL;
    }

    if(sizeof(TRSync) != lSendBuffer(skSock, (void *)&stSync, sizeof(TRSync)))
    {
IBPrace("发送心跳包失败LIS:%s:%d状态, errno:(%d), (%d)", pstDomain->m_szIp, pstDomain->m_lRIPort, errno, ntime);
        pstDomain->m_lTryTimes ++;
        vCloseSocket(skSock);
        return RC_FAIL;
    }

    if(sizeof(TRSync) != lRecvBuffer(skSock, (char *)&stSync, sizeof(TRSync)))
    {
IBPrace("接收心跳包失败LIS:%s:%d状态, errno:(%d), (%d)", pstDomain->m_szIp, pstDomain->m_lRIPort, errno, ntime);
        pstDomain->m_lTryTimes ++;
        vCloseSocket(skSock);
        return RC_FAIL;
    }
    
    vCloseSocket(skSock);
    return RC_SUCC;
}

//    发送心跳包
void    vDomainEvent(SATvm *pstSavm, CMList *pstRoot)
{
    CMList    *pstNode = NULL;
    TDomain    *pstDomain = NULL;

    while(1)
    {
        for(sleep(5), pstNode = pstRoot; pstNode; pstNode = pstNode->pstNext)
        {
            if(NULL == (pstDomain = (TDomain *)pstNode->m_psvData))
                continue;
IBPrace("开始轮询RDS:%s:%d状态(%d), time(%d), diff(%d), try(%d)", pstDomain->m_szIp, pstDomain->m_lRDPort, pstDomain->m_lStatus, pstDomain->m_lLastTime,
    (long)time(NULL) - pstDomain->m_lLastTime, pstDomain->m_lTryTimes);
            if(RESOURCE_ABLE != pstDomain->m_lStatus)
                continue;
    
            if(pstDomain->m_lKeepLive > ((long)time(NULL) - pstDomain->m_lLastTime))
                continue;    
            
            if(pstDomain->m_lTryMax > 0 && pstDomain->m_lTryMax <= pstDomain->m_lTryTimes)
                continue;

            if(DATA_TRUCK_LOCK == pstDomain->m_lLock)
                continue;
            
IBPrace("开始发送心跳包");
            pstDomain->m_lLock = DATA_TRUCK_LOCK;     
            if(RC_SUCC == lBreakHeart(pstSavm, pstDomain))
            {
                pstDomain->m_lTryTimes = 0;
                pstDomain->m_lLock = DATA_TRUCK_NULL;     
                continue;
            }
                
            pstDomain->m_lLock = DATA_TRUCK_NULL;     
            if(pstDomain->m_lTryMax > 0 && pstDomain->m_lTryMax <= pstDomain->m_lTryTimes)
            {
                pstDomain->m_lStatus = RESOURCE_STOP;
                lUpdateDomain(pstSavm, pstDomain->m_szIp, pstDomain->m_lRDPort, RESOURCE_STOP);
            }
        }
    }
IBPrace("================结束vDomainEvent======================");

    return ;
}

void*    vDomainRemote(void *plMode)
{
    SATvm    stSavm;
    CMList   *pstNode = NULL;
    TDomain  *pstDomain = NULL, stDomain;
    size_t   i, lOut = 0, lMode = *((size_t *)plMode);

    pthread_detach(pthread_self());
    memset(&stSavm, 0, sizeof(SATvm));
    if(RC_SUCC != lExportTable(SYS_TVM_DOMAIN, &lOut, (void **)&pstDomain))
    {
        if(NO_DATA_FOUND != lGetTErrno())
        {
            fprintf(stderr, "get domain node info failed, err:(%d)(%s)", 
                lGetTErrno(), sGetTError(lGetTErrno()));
            exit(-1);
        }

        pthread_exit(NULL);
        return ;
    }

    if(RC_SUCC != lInitSATvm(&stSavm, SYS_TVM_DOMAIN))
    {
        fprintf(stderr, "init tvm handle failed, err:(%d)(%s)", lGetTErrno(), 
            sGetTError(lGetTErrno()));
        exit(-1);
    }

    for(i = 0; i < lOut; i ++)
    {
        if(bExistDomain(g_pstDomain, &pstDomain[i], false))
            continue;

        memcpy(&stDomain, &pstDomain[i], sizeof(TDomain));
        stDomain.m_skSock = -1;    
        stDomain.m_table = 0;
        stDomain.m_lLastTime = 0;
        stDomain.m_lTryTimes = 0;
        
        if(NULL == (g_pstDomain = pInsertList(g_pstDomain, (void *)&stDomain, sizeof(TDomain))))
        {
            fprintf(stderr, "insert demain info failed\n");
            goto THREAD_ERROR;
        }

        if(NULL == (pstNode = pSearchNode(g_pstDomain, (void *)&stDomain, sizeof(TDomain))))
        {
            fprintf(stderr, "demain list data error\n");
            goto THREAD_ERROR;
        }

        if(lMode)    continue;

        lDomainNotify(&stSavm, (TDomain *)pstNode->m_psvData);
    }

    IBPFree(pstDomain);
    vDomainEvent(&stSavm, g_pstDomain);
    exit(-1);

THREAD_ERROR:
    IBPFree(pstDomain);
    vDestroyList(g_pstDomain);
    pthread_exit(NULL);
    exit(-1);
}

/*************************************************************************************************
    函数说明：fork多进程监听
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lBootDomain(SATvm *pstSavm, TBoot *pstBoot, long lMode)
{
    pid_t       lPid;
    pthread_t   tPid;
    epollevt    event;
    BSock       epfd = -1;
    SKCon       *pstCon = NULL;

    if(!pstBoot)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(TVM_BOOT_CLUSTER != pstBoot->m_lBootType)
        return RC_SUCC;

    if(NULL == (pstCon = (SKCon *)calloc(sizeof(SKCon), 1)))
    {
        fprintf(stderr, "创建内存, err:(%d)(%s)", errno, strerror(errno));
        return RC_FAIL;
    }

    pstCon->m_isListen = 1;
    if(0 > (pstCon->m_skSock = skServerInitail(pstSavm, pstBoot->m_lDomPort)))
        return RC_FAIL;

    vSetTitile(TVM_REMOTE_DOM);
    fprintf(stdout, "Boot process(%s), total 1 process, pid(%d)\n", TVM_REMOTE_DOM, getpid());
    if(0 > (lPid = fork()))
    {
        close(pstCon->m_skSock);
        return RC_FAIL;
    }
    else if(lPid > 0)
    {
        usleep(500);
        return RC_SUCC;
    }

    epfd = epoll_create(MAX_EVENTS);

    memset(&event, 0, sizeof(event));
    event.data.ptr = pstCon;
    event.events   = EPOLLIN | EPOLLET;
    if(0 != epoll_ctl(epfd, EPOLL_CTL_ADD, pstCon->m_skSock, &event))
    {
        fprintf(stderr, "新增套接字(%d)事件失败, err:(%d)(%s)", pstCon->m_skSock,
            errno, strerror(errno));
        return -1;
    }

    if(0 != pthread_create(&tPid, NULL, vDomainRemote, (void *)&lMode))
    {
        fprintf(stderr, "pthread_create error, errstr:%s.\n", strerror(errno));
        exit(-1);
    }

    fprintf(stdout, "   process %s id=%d ... success\n", TVM_REMOTE_DOM, getpid());
    fflush(stdout);
    vDomainListen(pstSavm, epfd);
    exit(-1);
}












/*************************************************************************************************
    函数说明：执行操作
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    _lRemoteSelect(SATvm *pstSavm, BSock skSock, TRSync *pstSync, void *psvOut, int lTime)
{
    static    long lRecv = 0;
    static    int  size =  sizeof(TRSync);

    if(size != lSendBuffer(skSock, (void *)pstSync, size))
        return RC_FAIL;

    if(pstSavm->lSize != lSendBuffer(skSock, pstSavm->pstVoid, pstSavm->lSize))
        return RC_FAIL;
        
    if(size != lRecvBuffer(skSock, (char *)pstSync, size))
        return RC_FAIL;

    pstSavm->m_lErrno = pstSync->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    if(0 == pstSync->m_lRows)    return RC_SUCC;

    pstSavm->m_lEffect = pstSync->m_lRows;
    lRecv = pstSync->m_lRows * pstSync->m_lDLen;
    if(lRecv != lRecvBuffer(skSock, (char *)psvOut, lRecv))
        return RC_FAIL;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取远程数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lSelectByRt(SATvm *pstSavm, RunTime *pstRun, TABLE t, void *psvOut)
{
    BSock    skSock;
    static    TRSync    stSync;

    memset(&stSync, 0, sizeof(TRSync));
    skSock = skConnectServer(pstSavm, pstRun->m_szIP, pstRun->m_lPort, false, 2);
    if(RC_FAIL == skSock) return RC_FAIL;

    stSync.m_lRows  = 1;
    stSync.m_enum   = OPERATE_SELECT;
    stSync.m_lFind  = pstSavm->lFind;
    stSync.m_lDLen  = pstSavm->lSize;  
    stSync.m_table  = pstSavm->tblName;
    stSync.m_lCLine = pstRun->m_lCurLine;
    stSync.m_lCType = pstRun->m_lCurType;

    if(RC_SUCC != _lRemoteSelect(pstSavm, skSock, &stSync, psvOut, pstRun->m_lTime))
    {
        close(skSock);
        return RC_FAIL;
    }

    close(skSock);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取远程数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    _lRemoteQuery(SATvm *pstSavm, BSock skSock, TRSync *pstSync, int *plOut, void **ppsvOut, 
            int lTime)
{
    static    long lRecv = 0;
    static    int  size =  sizeof(TRSync);

    if(size != lSendUnblock(pstSavm, skSock, (void *)pstSync, size, &lTime))
        return RC_FAIL;

    if(pstSavm->lSize != lSendUnblock(pstSavm, skSock, pstSavm->pstVoid, pstSavm->lSize, &lTime))
        return RC_FAIL;
        
    if(size != lRecvUnblock(pstSavm, skSock, (char *)pstSync, size, &lTime))
        return RC_FAIL;

    pstSavm->m_lErrno = pstSync->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    if(0 == pstSync->m_lRows)    return RC_SUCC;

    *plOut = pstSync->m_lRows;
    pstSavm->m_lEffect = pstSync->m_lRows;
    lRecv = pstSync->m_lRows * pstSync->m_lDLen;
    if(NULL == (*ppsvOut = (char *)realloc(*ppsvOut, lRecv)))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(lRecv != lRecvUnblock(pstSavm, skSock, (char *)*ppsvOut, lRecv, &lTime))
        return RC_FAIL;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取远程数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lQueryByRt(SATvm *pstSavm, RunTime *pstRun, TABLE t, int *plOut, void **ppsvOut)
{
    BSock    skSock;
    static    TRSync    stSync;

    memset(&stSync, 0, sizeof(TRSync));
    skSock = skConnectServer(pstSavm, pstRun->m_szIP, pstRun->m_lPort, false, 2);
    if(RC_FAIL == skSock) return RC_FAIL;

    stSync.m_lRows  = 1;
    stSync.m_enum   = OPERATE_QUERY;
    stSync.m_lFind  = pstSavm->lFind;
    stSync.m_lDLen  = pstSavm->lSize;  
    stSync.m_table  = pstSavm->tblName;
    stSync.m_lCLine = pstRun->m_lCurLine;
    stSync.m_lCType = pstRun->m_lCurType;

    if(RC_SUCC != _lRemoteQuery(pstSavm, skSock, &stSync, plOut, ppsvOut, pstRun->m_lTime))
    {
        close(skSock);
        return RC_FAIL;
    }

    close(skSock);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：执行操作
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    _lRemoteDelete(SATvm *pstSavm, BSock skSock, TRSync *pstSync, int lTime)
{
    static    long lRecv = 0;
    static    int  size =  sizeof(TRSync);

    if(size != lSendUnblock(pstSavm, skSock, (void *)pstSync, size, &lTime))
        return RC_FAIL;

    if(pstSavm->lSize != lSendUnblock(pstSavm, skSock, pstSavm->pstVoid, pstSavm->lSize, &lTime))
        return RC_FAIL;
        
    if(size != lRecvUnblock(pstSavm, skSock, (char *)pstSync, size, &lTime))
        return RC_FAIL;

    pstSavm->m_lErrno = pstSync->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    pstSavm->m_lEffect = pstSync->m_lRows;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：删除远程数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lDeleteByRt(SATvm *pstSavm, RunTime *pstRun, TABLE t)
{
    BSock    skSock;
    static    TRSync    stSync;

    memset(&stSync, 0, sizeof(TRSync));
    if(pstSavm->m_bWork)
    {
        pstSavm->m_lErrno = DMWORK_NOTSUP;
        return RC_FAIL;
    }

    skSock = skConnectServer(pstSavm, pstRun->m_szIP, pstRun->m_lPort, false, 2);
    if(RC_FAIL == skSock) return RC_FAIL;

    stSync.m_lRows  = 1;
    stSync.m_enum   = OPERATE_DELETE;
    stSync.m_lFind  = pstSavm->lFind;
    stSync.m_lDLen  = pstSavm->lSize;  
    stSync.m_table  = pstSavm->tblName;
    stSync.m_lCLine = pstRun->m_lCurLine;
    stSync.m_lCType = pstRun->m_lCurType;

    if(RC_SUCC != _lRemoteDelete(pstSavm, skSock, &stSync, pstRun->m_lTime))
    {
        close(skSock);
        return RC_FAIL;
    }

    close(skSock);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：更新远程数据同步
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    _lRemoteUpdate(SATvm *pstSavm, BSock skSock, TRSync *pstSync, void *psvData, int lTime)
{
    static    long lRecv = 0;
    static    int  size =  sizeof(TRSync);

    if(size != lSendUnblock(pstSavm, skSock, (void *)pstSync, size, &lTime))
        return RC_FAIL;

    if(pstSavm->lSize != lSendUnblock(pstSavm, skSock, pstSavm->pstVoid, pstSavm->lSize, &lTime))
        return RC_FAIL;

    if(pstSavm->lSize != lSendUnblock(pstSavm, skSock, psvData, pstSavm->lSize, &lTime))
        return RC_FAIL;
        
    if(size != lRecvUnblock(pstSavm, skSock, (char *)pstSync, size, &lTime))
        return RC_FAIL;

    pstSavm->m_lErrno = pstSync->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    pstSavm->m_lEffect = pstSync->m_lRows;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：更新远程数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lUpdateByRt(SATvm *pstSavm, RunTime *pstRun, TABLE t, void *psvData)
{
    BSock    skSock;
    static    TRSync    stSync;

    memset(&stSync, 0, sizeof(TRSync));
    if(pstSavm->m_bWork)
    {
        pstSavm->m_lErrno = DMWORK_NOTSUP;
        return RC_FAIL;
    }

    skSock = skConnectServer(pstSavm, pstRun->m_szIP, pstRun->m_lPort, false, 2);
    if(RC_FAIL == skSock) return RC_FAIL;

    stSync.m_lRows  = 2;
    stSync.m_enum   = OPERATE_UPDATE;
    stSync.m_lFind  = pstSavm->lFind;
    stSync.m_lDLen  = pstSavm->lSize;  
    stSync.m_table  = pstSavm->tblName;
    stSync.m_lCLine = pstRun->m_lCurLine;
    stSync.m_lCType = pstRun->m_lCurType;

    if(RC_SUCC != _lRemoteUpdate(pstSavm, skSock, &stSync, psvData, pstRun->m_lTime))
    {
        close(skSock);
        return RC_FAIL;
    }

    close(skSock);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：新增远程数据同步
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    _lRemoteInsert(SATvm *pstSavm, BSock skSock, TRSync *pstSync, int lTime)
{
    static    long lRecv = 0;
    static    int  size =  sizeof(TRSync);

    if(size != lSendUnblock(pstSavm, skSock, (void *)pstSync, size, &lTime))
        return RC_FAIL;

    if(pstSavm->lSize != lSendUnblock(pstSavm, skSock, pstSavm->pstVoid, pstSavm->lSize, &lTime))
        return RC_FAIL;

    if(size != lRecvUnblock(pstSavm, skSock, (char *)pstSync, size, &lTime))
        return RC_FAIL;

    pstSavm->m_lErrno = pstSync->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    pstSavm->m_lEffect = pstSync->m_lRows;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：新增远程数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lInsertByRt(SATvm *pstSavm, RunTime *pstRun, TABLE t)
{
    BSock    skSock;
    static    TRSync    stSync;

    memset(&stSync, 0, sizeof(TRSync));
    if(pstSavm->m_bWork)
    {
        pstSavm->m_lErrno = DMWORK_NOTSUP;    
        return RC_FAIL;
    }

    skSock = skConnectServer(pstSavm, pstRun->m_szIP, pstRun->m_lPort, false, 2);
    if(RC_FAIL == skSock) return RC_FAIL;

    stSync.m_lRows  = 1;
    stSync.m_enum   = OPERATE_INSERT;
    stSync.m_lFind  = pstSavm->lFind;
    stSync.m_lDLen  = pstSavm->lSize;  
    stSync.m_table  = pstSavm->tblName;
    stSync.m_lCLine = pstRun->m_lCurLine;
    stSync.m_lCType = pstRun->m_lCurType;

    if(RC_SUCC != _lRemoteInsert(pstSavm, skSock, &stSync, pstRun->m_lTime))
    {
        close(skSock);
        return RC_FAIL;
    }

    close(skSock);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：vTvmDisconnect(SATvm *pstSavm)
    参数说明：
        pstSavm                    --句柄
    返回值：
        无
 *************************************************************************************************/
void    vTvmDisconnect(SATvm *pstSavm)
{
    if(!pstSavm)    return ;

    close(pstSavm->m_skSock);
    IBPFree(pstSavm->pstVoid);

    vTvmHoldRelease(pstSavm);
}

/*************************************************************************************************
    函数说明：新增远程数据
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lTvmConnect(SATvm *pstSavm, char *pszIp, long lPort, int times)
{
    if(!pstSavm || !pszIp)
        return RC_FAIL;

    pstSavm->m_skSock = skConnectServer(pstSavm, pszIp, lPort, false, times);
    if(RC_FAIL == pstSavm->m_skSock)
        return RC_FAIL;

    pstSavm->m_lTimes = times;

    if(NULL == (pstSavm->pstVoid = (void *)calloc(READ_MAX_LEN, 1)))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：API接入select
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lTvmSelect(SATvm *pstSavm, TABLE t, void *pvVoid, size_t nSize, void *pvOut)
{
    int        lLen = sizeof(TFace) + nSize;

    if(!pstSavm || !pvVoid || !pvOut)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    ((TFace *)pstSavm->pstVoid)->m_table  = t;
    ((TFace *)pstSavm->pstVoid)->m_lRows  = 1;
    ((TFace *)pstSavm->pstVoid)->m_lDLen  = nSize;
    ((TFace *)pstSavm->pstVoid)->m_enum   = OPERATE_SELECT;
    ((TFace *)pstSavm->pstVoid)->m_lFind  = pstSavm->lFind;

    if(lLen > READ_MAX_LEN)
    {
        if(lLen > DATA_MAX_LEN)
        {
            pstSavm->m_lErrno = RECD_TOO_LONG;
            return RC_FAIL;
        }

        if(NULL == (pstSavm->pstVoid = (void *)realloc(pstSavm->pstVoid, lLen)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_SUCC;
        }
    }

    memcpy(pstSavm->pstVoid + sizeof(TFace), pvVoid, nSize);

    if(lLen != lSendBuffer(pstSavm->m_skSock, (void *)pstSavm->pstVoid, lLen))
        return RC_FAIL;
        
    if(sizeof(TFace) != lRecvBuffer(pstSavm->m_skSock, (char *)pstSavm->pstVoid, sizeof(TFace)))
        return RC_FAIL;

    pstSavm->m_lErrno = ((TFace *)pstSavm->pstVoid)->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    if(0 == ((TFace *)pstSavm->pstVoid)->m_lRows)    return RC_SUCC;

    pstSavm->m_lEffect = ((TFace *)pstSavm->pstVoid)->m_lRows;
    if(nSize != lRecvBuffer(pstSavm->m_skSock, (char *)pvOut, nSize))
        return RC_FAIL;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：API接入query
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lTvmQuery(SATvm *pstSavm, TABLE t, void *pvVoid, size_t nSize, long *plOut, void **ppvOut)
{
    int        lRecv, lLen = sizeof(TFace) + nSize;

    if(!pstSavm || !pvVoid || !plOut)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    ((TFace *)pstSavm->pstVoid)->m_table  = t;
    ((TFace *)pstSavm->pstVoid)->m_lRows  = 1;
    ((TFace *)pstSavm->pstVoid)->m_lDLen  = nSize;
    ((TFace *)pstSavm->pstVoid)->m_enum   = OPERATE_QUERY;
    ((TFace *)pstSavm->pstVoid)->m_lFind  = pstSavm->lFind;

    if(lLen > READ_MAX_LEN)
    {
        if(lLen > DATA_MAX_LEN)
        {
            pstSavm->m_lErrno = RECD_TOO_LONG;
            return RC_FAIL;
        }

        if(NULL == (pstSavm->pstVoid = (void *)realloc(pstSavm->pstVoid, lLen)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_SUCC;
        }
    }

    memcpy(pstSavm->pstVoid + sizeof(TFace), pvVoid, nSize);

    if(lLen != lSendBuffer(pstSavm->m_skSock, (void *)pstSavm->pstVoid, lLen))
        return RC_FAIL;
        
    if(sizeof(TFace) != lRecvBuffer(pstSavm->m_skSock, (char *)pstSavm->pstVoid, sizeof(TFace)))
        return RC_FAIL;

    pstSavm->m_lErrno = ((TFace *)pstSavm->pstVoid)->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    if(0 == (*plOut = ((TFace *)pstSavm->pstVoid)->m_lRows))
        return RC_SUCC;

    pstSavm->m_lEffect = ((TFace *)pstSavm->pstVoid)->m_lRows;
    lRecv = nSize * pstSavm->m_lEffect;
    if(NULL == (*ppvOut = (void *)malloc(lRecv)))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(lRecv != lRecvBuffer(pstSavm->m_skSock, (char *)*ppvOut, lRecv))
        return RC_FAIL;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：API接入insert
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lTvmInsert(SATvm *pstSavm, TABLE t, void *pvVoid, size_t nSize)
{
    int        lRecv, lLen = sizeof(TFace) + nSize;

    if(!pstSavm || !pvVoid)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    ((TFace *)pstSavm->pstVoid)->m_table  = t;
    ((TFace *)pstSavm->pstVoid)->m_lRows  = 1;
    ((TFace *)pstSavm->pstVoid)->m_lDLen  = nSize;
    ((TFace *)pstSavm->pstVoid)->m_enum   = OPERATE_INSERT;
    ((TFace *)pstSavm->pstVoid)->m_lFind  = pstSavm->lFind;

    if(lLen > READ_MAX_LEN)
    {
        if(lLen > DATA_MAX_LEN)
        {
            pstSavm->m_lErrno = RECD_TOO_LONG;
            return RC_FAIL;
        }

        if(NULL == (pstSavm->pstVoid = (void *)realloc(pstSavm->pstVoid, lLen)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_SUCC;
        }
    }

    memcpy(pstSavm->pstVoid + sizeof(TFace), pvVoid, nSize);

    if(lLen != lSendBuffer(pstSavm->m_skSock, (void *)pstSavm->pstVoid, lLen))
        return RC_FAIL;
        
    if(sizeof(TFace) != lRecvBuffer(pstSavm->m_skSock, (char *)pstSavm->pstVoid, sizeof(TFace)))
        return RC_FAIL;

    pstSavm->m_lErrno = ((TFace *)pstSavm->pstVoid)->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    pstSavm->m_lEffect = ((TFace *)pstSavm->pstVoid)->m_lRows;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：API接入delete
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lTvmDelete(SATvm *pstSavm, TABLE t, void *pvVoid, size_t nSize)
{
    int        lRecv, lLen = sizeof(TFace) + nSize;

    if(!pstSavm || !pvVoid)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    ((TFace *)pstSavm->pstVoid)->m_table  = t;
    ((TFace *)pstSavm->pstVoid)->m_lRows  = 1;
    ((TFace *)pstSavm->pstVoid)->m_lDLen  = nSize;
    ((TFace *)pstSavm->pstVoid)->m_enum   = OPERATE_DELETE;
    ((TFace *)pstSavm->pstVoid)->m_lFind  = pstSavm->lFind;

    if(lLen > READ_MAX_LEN)
    {
        if(lLen > DATA_MAX_LEN)
        {
            pstSavm->m_lErrno = RECD_TOO_LONG;
            return RC_FAIL;
        }

        if(NULL == (pstSavm->pstVoid = (void *)realloc(pstSavm->pstVoid, lLen)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_SUCC;
        }
    }

    memcpy(pstSavm->pstVoid + sizeof(TFace), pvVoid, nSize);

    if(lLen != lSendBuffer(pstSavm->m_skSock, (void *)pstSavm->pstVoid, lLen))
        return RC_FAIL;
        
    if(sizeof(TFace) != lRecvBuffer(pstSavm->m_skSock, (char *)pstSavm->pstVoid, sizeof(TFace)))
        return RC_FAIL;

    pstSavm->m_lErrno = ((TFace *)pstSavm->pstVoid)->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    pstSavm->m_lEffect = ((TFace *)pstSavm->pstVoid)->m_lRows;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：API接入update
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
         RC_FAIL                    --失败
 *************************************************************************************************/
long    lTvmUpdate(SATvm *pstSavm, TABLE t, void *pvVoid, size_t nSize, void *pvData)
{
    int        lRecv, lLen = sizeof(TFace) + nSize * 2;

    if(!pstSavm || !pvVoid || !pvData)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    ((TFace *)pstSavm->pstVoid)->m_table  = t;
    ((TFace *)pstSavm->pstVoid)->m_lRows  = 2;
    ((TFace *)pstSavm->pstVoid)->m_lDLen  = nSize;
    ((TFace *)pstSavm->pstVoid)->m_enum   = OPERATE_UPDATE;
    ((TFace *)pstSavm->pstVoid)->m_lFind  = pstSavm->lFind;

    if(lLen > READ_MAX_LEN)
    {
        if(lLen > DATA_MAX_LEN)
        {
            pstSavm->m_lErrno = RECD_TOO_LONG;
            return RC_FAIL;
        }

        if(NULL == (pstSavm->pstVoid = (void *)realloc(pstSavm->pstVoid, lLen)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_SUCC;
        }
    }

    memcpy(pstSavm->pstVoid + sizeof(TFace), pvVoid, nSize);
    memcpy(pstSavm->pstVoid + sizeof(TFace) + nSize, pvData, nSize);

    if(lLen != lSendBuffer(pstSavm->m_skSock, (void *)pstSavm->pstVoid, lLen))
        return RC_FAIL;
        
    if(sizeof(TFace) != lRecvBuffer(pstSavm->m_skSock, (char *)pstSavm->pstVoid, sizeof(TFace)))
        return RC_FAIL;

    pstSavm->m_lErrno = ((TFace *)pstSavm->pstVoid)->m_lErrno;
    if(0 != pstSavm->m_lErrno)
        return RC_FAIL;

    pstSavm->m_lEffect = ((TFace *)pstSavm->pstVoid)->m_lRows;

    return RC_SUCC;
}

/*************************************************************************************************
 * code end
 *************************************************************************************************/
