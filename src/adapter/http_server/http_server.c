/*************************************************************************************************
    文 件 名  : http_server.c
    作    者  : DeffPuzzL
    版    本  : V1.0.0.0
    创建日期  : 2017-10-20
    描    述  : 用http报文服务端
 ************************************************************************************************/
#include    "face.h"

/**************************************************************************************************
    全局进程信息
 **************************************************************************************************/
#define        BODY_LEN                    10240
#define        MAX_HTTP                    2500
#define        TAIL_CRLF                   "\r\n"
#define        HTTP_TAIL                   "\r\n\r\n"
#define        ASYN_RESP                   "{\"ret_code\":\"000\",\"ret_msg\":\"交易成功\"}"

/**************************************************************************************************
    HTTP报文包定义 
 **************************************************************************************************/
typedef struct __SOCK_RECV
{
    BOOL    m_bHead;
    long    m_lRecv;
    llSEQ   m_lSeqNo;    
    long    m_lBody;
    long    m_lOffset;
    char    *m_pvData;
    char    m_szCode[FILED_INDEX_LEN];    
    char    m_szBuffer[BODY_LEN];
}HttpRecv;

CtsHost     g_stHost = {0};
HttpRecv    g_stHttp[MAX_HTTP] = {0};

/**************************************************************************************************
    函 数 名：vPrintVersion(char *pszParm)
    功能描述：显示该应用的版本
    返回说明：
        无
 **************************************************************************************************/
void    vPrintVersion(char *pszParm)
{
    if(!pszParm)    return ;
    if(strcmp(sUpper(pszParm), "-V"))   return ;

    fprintf(stdout, "%s\n", g_szVersion);

    exit(0);
}

/**************************************************************************************************
    函 数 名：vErrorReturn(BSock skSock)
    功能描述：接收核心请求事件任务
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
void    vHttpReturn(BSock skSock, char *pszBuffer, long lLen)
{
    HttpRecv    *pstHttp = &g_stHttp[skSock];

	if(lLen == 0)
	{
    	pstHttp->m_lBody = snprintf(pstHttp->m_szBuffer, sizeof(pstHttp->m_szBuffer), 
        	"HTTP/1.1 400 bad request\r\nCache-Control: no-cache\r\nContent-Type: text/plain; "
			"charset=UTF-8" "\r\nConnection: close\r\nContent-Length: %d\r\n\r\n", lLen);
	}	
	else
	{
    	pstHttp->m_lBody = snprintf(pstHttp->m_szBuffer, sizeof(pstHttp->m_szBuffer), 
        	"HTTP/1.1 200 OK\r\nCache-Control: no-cache\r\nContent-Type: text/plain; charset=UTF-8"
        	"\r\nConnection: close\r\nContent-Length: %d\r\n\r\n%s", lLen, pszBuffer);
	}

//  IBPrace("响应数据如下(%d):", pstHttp->m_lBody);
//  IBPcem(pstHttp->m_szBuffer, pstHttp->m_lBody);
    if(pstHttp->m_lBody != lSendBuffer(skSock, pstHttp->m_szBuffer, pstHttp->m_lBody))
    {
        close(skSock);
        memset(pstHttp, 0, sizeof(long) * 5);
        IBPerror("发送http响应数据失败, %s", strerror(errno));
        return ;
    }

    close(skSock);
    memset(pstHttp, 0, sizeof(long) * 5);
    return ;
}

/**************************************************************************************************
    函 数 名：lParseHttp(HttpRecv *pv)
    功能描述：解析http请求头
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
long    lParseHttp(HttpRecv *pv)
{
    long    i = 0;
    char    *s = NULL, *q = NULL, szLen[20];
    
    for(s = pv->m_szBuffer, pv->m_pvData = strstr(s, HTTP_TAIL); s < pv->m_pvData; 
        s = strstr(s + 2, TAIL_CRLF) + 2)
    {
        if(!memcmp(s, "POST", 4))
        {
            for(s += 4; s < pv->m_pvData && *s == ' '; s ++);
            for(q = s; s < pv->m_pvData && *s != ' '; s ++);
           
            memset(pv->m_szCode, 0, FILED_INDEX_LEN);
            memcpy(pv->m_szCode, q, MIN(s - q, FILED_INDEX_LEN));
        }
        else if(!memcmp(s, "GET", 3))
        {
            if(!(pv->m_pvData = strstr(s, "?")))
                continue;
            pv->m_pvData += 1;
            if(!(q = strstr(pv->m_pvData, " HTTP")))
                continue;

            pv->m_lBody = q - pv->m_pvData; 
            pv->m_lRecv = pv->m_lBody;
            pv->m_bHead = true;    
            return RC_SUCC;
        }
        else if(!memcmp(s, "Content-Length:", 15))
        {
            for(s += 15; s < pv->m_pvData && *s == ' '; s ++);
            for(q = s; s < pv->m_pvData && *s != ' '; s ++);
            memset(szLen, 0, sizeof(szLen));
            memcpy(szLen, q, MIN(s - q, 20));
            pv->m_lBody = atol(szLen);    
        }
    }

	if(!pv->m_pvData)	return RC_FAIL;

    pv->m_pvData += 4;
    pv->m_lOffset = pv->m_pvData - pv->m_szBuffer;
    pv->m_lRecv   = pv->m_lRecv - pv->m_lOffset;
    pv->m_lOffset = BODY_LEN - pv->m_lOffset;
    if(pv->m_lRecv < 0) return RC_FAIL;

    pv->m_bHead = true;    

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lHttpRequest(SATvm *pstSavm, void *arg)
    功能描述：接收核心请求事件任务
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
long    lHttpRequest(SATvm *pstSavm, void *arg)
{
    long        lRet;
	static char	szResp[512];
    SKCon       *pc = (SKCon *)arg;
    HttpRecv    *pv = &g_stHttp[pc->m_skSock];

    if(!pv || !pc || MAX_HTTP <= pc->m_skSock) return RC_CLOSE;

    if(!pv->m_bHead)    //    接收头
    {
        if(0 > (pv->m_lRecv = lRecvBuffer(pc->m_skSock, pv->m_szBuffer, 
            BODY_LEN)))
            goto HTTP_ERROR;

        if(RC_SUCC != lParseHttp(pv))
            goto HTTP_ERROR;
    }


    if(pv->m_lRecv < pv->m_lBody)
    {
        if(0 > (lRet = lRecvBuffer(pc->m_skSock, pv->m_pvData + pv->m_lRecv, 
            pv->m_lOffset - pv->m_lRecv)))
            goto HTTP_ERROR;

        if(pv->m_lBody != (pv->m_lRecv += lRet))
            return pv->m_lRecv;
    }

    if(g_stHost.m_bAsyn)
    {
		lAsPutRequest(pstSavm, pv->m_pvData, pv->m_lRecv, pc->m_skSock, pv->m_szCode, "");
        vHttpReturn(pc->m_skSock, ASYN_RESP, strlen(ASYN_RESP));
		goto HTTP_ERROR;
    }

	if(RC_SUCC != lInPutRequest(pstSavm, pv->m_pvData, pv->m_lRecv, pc->m_skSock, pv->m_szCode, ""))
    {
		lRet = snprintf(szResp, sizeof(szResp), "{\"ret_code\":\"%s\",\"ret_msg\":\"%s\"}", 
			sGetRetCode(), sGetRetMsg()); 
        vHttpReturn(pc->m_skSock, szResp, lRet);
        goto HTTP_ERROR;
    }

    pv->m_lSeqNo = lGetSeqNo();
    return RC_SUCC;

HTTP_ERROR:
    memset(pv, 0, sizeof(long) * 5);
    return RC_CLOSE;
}

/**************************************************************************************************
    函 数 名：vHttpRespond(void *args)
    功能描述：处理核心响应信息线程
    返回说明：
        无
 **************************************************************************************************/
void    vHttpRespond(void *args)
{
    long        lLen;
    CtsProcess  stProcess;
    HHCMIE      me = (HHCMIE)pMallocCmie();
    char        *pszBuffer = (char *)pGetBuffer();
    GCts        *pstCts = (GCts *)pCmieGloble(me);
    CtsEvent    *pstEvent = (CtsEvent *)pCmieEvent(me);
    SATvm       *pstSavm = (SATvm *)calloc(1, sizeof(SATvm));

    pthread_detach(pthread_self());
    memset(&stProcess, 0, sizeof(stProcess));
    if(!pstSavm || !pszBuffer || !me)
    {
        IBPerror("初始化线程参数失败, err:%s", strerror(errno));
        return ;
    }

    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", sGetProgram());
        return ;
    }

    while(1)
    {
        if(RC_SUCC != lInGetRespond(pstSavm, &stProcess, me, pszBuffer, &lLen))
        {
            SYSError(RET_SYSTEM, "获取交易响应失败, %s", sGetTError(pstSavm->m_lErrno));
            continue;
        }
    
        if(IBPevcache(pstEvent))
            continue;

        if(g_stHttp[pstEvent->m_lResv].m_lSeqNo != pstCts->m_lSeqNo)
            continue;

        if(lLen == 0 && IBPstaterr(pstCts->m_lState))
        {
            lLen = snprintf(pszBuffer, lGetUserSize(), "retCode=%s&retMsg=%s", pstCts->m_szRetCode,
                pstCts->m_szRetMsg);
        }

		pszBuffer[lLen] = 0x00;
        vHttpReturn(pstEvent->m_lResv, pszBuffer, lLen);
    }
    
    pthread_exit(NULL);
}

/**************************************************************************************************
    函 数 名：lProcInitail(SATvm *pstSavm, void *arg)
    功能描述：初始化子进程
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lProcInitail(SATvm *pstSavm, void *arg)
{
    pthread_t   tPid = 0;
    long        lHost = *((long *)arg);

    if(RC_SUCC != lRegisterProcess(sGetProgram(), lHost))
    {   
        fprintf(stderr, "登记任务(%s)(%d)的进程失败\n", sGetProgram(), lHost);
        exit(RC_FAIL);
    }

    IBPrace("进程%s启动，当前版本：%s ...", sGetProgram(), g_szVersion);

    fprintf(stdout, "child %s process id=%d ... success\n", sGetProgram(), getpid());
    fflush(stdout);

    if(0 != pthread_create(&tPid, NULL, (void *)vHttpRespond, NULL))
    {
        fprintf(stderr, "pthread_create error, err:%s.\n", strerror(errno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：main
    功能描述：进程监控主模块
    返回说明：
        RC_SUCC         --成功
           RC_FAIL         --失败
 **************************************************************************************************/
int main(int argc, char *argv[])
{
    CtsProcess  stProcess;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    vPrintVersion(argv[1]);
    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));
    memset(&g_stHttp, 0, sizeof(HttpRecv) * MAX_HTTP);
    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败, %s", stProcess.m_szPGName, sGetError());
        return RC_FAIL;
    }

    if(RC_SUCC != lGetHost(pstSavm, &g_stHost, stProcess.m_lHost))
    {
        SYSError(RET_SYSTEM, "获取执行码(%s)对应的主机信息失败, err:%s",
            stProcess.m_szPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    lUnregisterProcess(&stProcess);

    if(RC_SUCC != lMultListen(pstSavm, g_stHost.m_lHostPort, g_stHost.m_lMaxNum, lProcInitail, 
        &stProcess.m_lHost, lHttpRequest))
    {
        fprintf(stderr, "boot %d,%d, tcp_epoll_multi_run error:%s.\n", g_stHost.m_lHostPort,
            g_stHost.m_lMaxNum, strerror(errno));
        exit(-1);
    }

    exit(0);
}

/**************************************************************************************************
    code end
 **************************************************************************************************/
