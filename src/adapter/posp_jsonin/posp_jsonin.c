/*************************************************************************************************
	文 件 名  : posp_jsonin.c
	作    者  : DeffPuzzL
	版    本  : V2.0.0.0
	创建日期  : 2016-03-07
	描    述  : 用http报文调用结算接口
 ************************************************************************************************/
#include	"face.h"


/**************************************************************************************************
    全局进程信息
 **************************************************************************************************/
#define        BODY_LENG                   10240
#define        MAX_CLIENT                  2500
#define        ASYN_RESP                   "{\"ret_code\":\"000\",\"ret_msg\":\"交易成功\"}"

/**************************************************************************************************
 *     全局进程信息
 **************************************************************************************************/
typedef struct __SOCK_RECV
{
    long    m_lHead;
    long    m_lRecv;
    llSEQ   m_lSeqNo;
    long    m_lBody;
    char    m_szCode[FILED_INDEX_LEN];
    char    m_szBuffer[BODY_LENG];
}SockRecv;

SockRecv    g_stRecv[MAX_CLIENT + 1] = {0};

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
void	vErrorReturn(BSock skSock)
{
    long    lLen, lWrite = 0;
    char    szWrite[512], szLen[5];

    memset(szLen, 0, sizeof(szLen));
    memset(szWrite, 0, sizeof(szWrite));
    lLen = snprintf(szWrite + 4, sizeof(szWrite) - 4, "{\"ret_code\":\"%s\",\"ret_msg\":\"%s\"}", 
		sGetRetCode(), sGetRetMsg());
    snprintf(szLen, sizeof(szLen), "%04d", lLen);
    memcpy(szWrite, szLen, 4);
    lWrite = lLen + 4;

    if(lWrite != lSendBuffer(skSock, szWrite, lWrite))
    {
        IBPerror("发送任务响应失败, err:%s", strerror(errno));
        return ;
    }

    return ;
}

/**************************************************************************************************
	函 数 名：lRecvMessage(BSock skSock, char *pszOut, int lOut, int flag)
	功能描述：接收核心请求事件任务
	返回说明：
		RC_SUCC             --成功
		RC_FAIL             --失败
 **************************************************************************************************/
long	lRecvMessage(SATvm *pstSavm, void *arg)
{
    long     lRet;
	SockRecv *pv = NULL;
	SKCon    *pstCon = (SKCon *)arg;

	if(pstCon->m_skSock > MAX_CLIENT)
	{
		IBPwarn("交易接入超限:%d", pstCon->m_skSock);
		return RC_CLOSE;
	}

	pv = &g_stRecv[pstCon->m_skSock];
    if(pv->m_lHead < 4)
    {
        if(4 != lRecvBuffer(pstCon->m_skSock, pv->m_szBuffer, 4))
        {
    		memset(pv, 0, sizeof(long));
			return RC_CLOSE;
        }
		
		pv->m_lRecv = 0;
		pv->m_szBuffer[4] = 0x00;
		pv->m_lBody = atol(pv->m_szBuffer);
		pv->m_szBuffer[pv->m_lBody] = 0x00;
		if(pv->m_lBody <= 0 || pv->m_lBody > BODY_LENG)
		{
            SYSError(RET_SYSTEM, "请求长度异常");
			goto ERROR_SERVER;
		}
	}

	if(pv->m_lRecv < pv->m_lBody)
	{
    	if(0 > (lRet = lRecvBuffer(pstCon->m_skSock, pv->m_szBuffer + pv->m_lRecv, 
			pv->m_lBody - pv->m_lRecv)))
		{
            memset(pv, 0, sizeof(long) * 10);
			return RC_CLOSE;
		}

		pv->m_lRecv += lRet;		
        if(pv->m_lRecv < pv->m_lBody)
            return pv->m_lBody;
	}

	memcpy(pv->m_szCode, pv->m_szBuffer, 9);
	pv->m_szCode[9] = 0x00;

    if(RC_FAIL == (pv->m_lSeqNo = lInPutRequest(pstSavm, pv->m_szBuffer + 9, pv->m_lBody - 9, 
		pstCon->m_skSock, pv->m_szCode, "")))
		goto ERROR_SERVER;

	return pv->m_lBody;	//同步不能关闭

ERROR_SERVER:
    vErrorReturn(pstCon->m_skSock);
    memset(pv, 0, sizeof(long));
	return RC_FAIL;
}

/**************************************************************************************************
	函 数 名：vGetRespond(void *args)
	功能描述：提高TCP接入并发接入API
	返回说明：
		RC_SUCC         --成功
	  	RC_FAIL         --失败
 **************************************************************************************************/
void*	vGetRespond(void *args)
{
    long		lLen;
	CtsProcess  stProcess;
	HHCMIE		me = (HHCMIE)pMallocCmie();
	CtsEvent    *pstEvent = pCmieEvent(me);
    SATvm       *pstSavm = (SATvm *)calloc(1, sizeof(SATvm));
    char		*pszBuffer = (char *)pGetBuffer(), szLen[64];

    pthread_detach(pthread_self());
    if(!pstSavm || !pszBuffer || !me)
	{
        IBPerror("初始化线程参数失败, err:%s", strerror(errno));
        return ;
	}

	memset(&stProcess, 0, sizeof(CtsProcess));
	if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
	{
		IBPerror("进程%s启动初始化失败!", sGetProgram());
		return ;
	}

    while(1)
	{
		if(RC_SUCC != lInGetRespond(pstSavm, &stProcess, me, pszBuffer + 4, &lLen))
        {
            SYSError(RET_SYSTEM, "获取交易最终响应失败, err:%s", sGetTError(pstSavm->m_lErrno));
            continue;
        }
	
   		if(IBPevcache(pstEvent))
			continue;

        snprintf(szLen, sizeof(szLen), "%04d", lLen);
		memcpy(pszBuffer, szLen, 4);
		lLen += 4;

//    	IBTrace(me, "发送最终响应数据如下(%d):", lLen);
//		IBTcem(me, pszBuffer, lLen);
        if(lLen != lSendBuffer(pstEvent->m_lResv, pszBuffer, lLen))
            IBTerror(me, "发送任务响应失败, err:%s", strerror(errno));

		close(pstEvent->m_lResv);		
		memset(&g_stRecv[pstEvent->m_lResv], 0, sizeof(long));
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
long	lProcInitail(SATvm *pstSavm, void *arg)
{
	pthread_t   tPid = 0;
	long		lHost = *((long *)arg);

    if(RC_SUCC != lRegisterProcess(sGetProgram(), lHost))
    {   
        fprintf(stderr, "登记任务(%s)(%d)的进程失败\n", sGetProgram(), lHost);
        exit(RC_FAIL);
    }

	IBPrace("进程%s启动，当前版本：%s ...", sGetProgram(), g_szVersion);

    fprintf(stdout, "child %s process id=%d ... success\n", sGetProgram(), getpid());
	fflush(stdout);

    if(0 != pthread_create(&tPid, NULL, vGetRespond, NULL))
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
    CtsHost     stHost;
	CtsProcess  stProcess;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    memset(&stHost, 0, sizeof(CtsHost));
	memset(&stProcess, 0, sizeof(stProcess));
	if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
	{
		IBPerror("进程%s启动初始化失败!", basename(argv[0]));
		return RC_FAIL;
	}

    if(RC_SUCC != lGetHost(pstSavm, &stHost, stProcess.m_lHost))
    {
        SYSError(RET_SYSTEM, "获取执行码(%s)对应的主机信息失败, err:%s",
            stProcess.m_szPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

	lUnregisterProcess(&stProcess);

    if(RC_SUCC != lMultListen(pstSavm, stHost.m_lHostPort, stHost.m_lMaxNum, lProcInitail, 
		&stProcess.m_lHost, lRecvMessage))
    {
        fprintf(stderr, "boot %d,%d, tcp_epoll_multi_run error:%s.\n", stHost.m_lHostPort,
            stHost.m_lMaxNum, strerror(errno));
		exit(-1);
    }

    exit(0);
}

/**************************************************************************************************
	code end
 **************************************************************************************************/
