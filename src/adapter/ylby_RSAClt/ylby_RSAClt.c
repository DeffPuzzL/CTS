/**************************************************************************************************
    文 件 名：export_txn.pcpp
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：demo实现业务模块
 **************************************************************************************************/
#include    "face.h"
#include    <curl/curl.h>  

/**************************************************************************************************
	宏定义
 **************************************************************************************************/
#define HTTP_THUNK                     1
#define HTTP_LENTH                     0
#define BODY_LEN                       20480
#define MAX_CLIENT                     500
#define PACK_SIGN                      "&signature="
#define LENG_SIGN                      11    // "strlen("&sign=")
#define TAIL_CRLF                      "\r\n"
#define HTTP_TAIL                      "\r\n\r\n"
#define FILE_SIGN                      "fileContentDetail="
#define FILE_LENG                      18

char                                   g_szFile[512] = {0};
/**************************************************************************************************
	交易结构定义
 **************************************************************************************************/
typedef struct curl_slist scurlist;
typedef struct __SOCK_CLIENT
{
	BSock       m_skSock;
    BOOL        m_bHead;
	BOOL		m_bChunk;
    long        m_lRecv;
    long        m_lBody;
    long        m_lOffset;
    char        *m_pvData;
    char        m_szBuffer[BODY_LEN];
	HCMIE       ce;
}SKClient;

/**************************************************************************************************
	全局变量		
 **************************************************************************************************/
SKClient                               g_stClt[MAX_CLIENT] = {0};
BSock                                  g_skSock = -1, g_epfd = -1;
/**************************************************************************************************
    函数定义
 **************************************************************************************************/
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

    fprintf(stdout, "current version:%s\n", g_szVersion);

    exit(0);
}

/**************************************************************************************************
    函 数 名：sockclose(BOOL blink, BSock skSock)
    功能描述：解析结算返回的响应
    返回说明：
		无
 **************************************************************************************************/
void	sockclose(BOOL blink, BSock skSock)
{
	close(skSock);
	if(blink)  g_skSock = -1;			
}

/**************************************************************************************************
	函 数 名：vWriteFile(char *pszData, char lLen)
	功能描述：解析结算返回的响应
	返回说明：
	    无
 **************************************************************************************************/
void    vWriteFile(char *pszData, char lLen)
{
    FILE    *fp = NULL;
    char    szContext[10240], szPath[512];

    memset(szPath, 0, sizeof(szPath));
    memset(szContext, 0, sizeof(szContext));
    snprintf(szPath, sizeof(szPath), "%s.gz", g_szFile);
    if(-1 == (lLen = base64_decode(pszData, lLen, szContext, sizeof(szContext))))
        return ;

    if(NULL == (fp = fopen(szPath, "a+")))
        return ;

    fwrite(szContext, 1, lLen, fp);
    fclose(fp);
}

/**************************************************************************************************
    函 数 名：lValuesEncode(char *pszPack, long lLen, char *pszOut, long lSize)
    功能描述：对values字段urlencode
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lValuesEncode(char *s, long l, char *o, long x)
{
    long    k, m, n;
    char    *p, *q, *d = s, v[1024];

    for(k = 0; (p = strstr(d, "&")) && k < x; d = (p + 1))
    {
        if((m = p - d) > sizeof(v))
            return RC_FAIL;

        memcpy(v, d, m);
        v[m] = 0x00;
        if(NULL == (q = strstr(v, "=")))
            continue;

        n = (q - v) + 1;
        memcpy(o + k, v, n);
        k += n;
        k += lUrlencode(v + n, m - n, o + k, x - k);
        memcpy(o + k, "&", 1);
        k ++;
    }

    if(*d != 0x00)
    {
        p = s + l;
        if((m = p - d) > sizeof(v))
            return RC_FAIL;

        memcpy(v, d, m);
        v[m] = 0x00;
        if(NULL == (q = strstr(v, "=")))
            return k;

        n = (q - v) + 1;
        memcpy(o + k, v, n);
        k += n;
        k += lUrlencode(v + n, m - n, o + k, x - k);
    }

    return k;
}

/**************************************************************************************************
    函 数 名：lWriteRemote(SATvm *pstSavm, CtsHost *pstHost, CtsEvent *pstEvent, char *pszWrite, 
                  long lLen)
    功能描述：组织报文发送socket请求数据
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long	lWriteRemote(SATvm *pstSavm, CtsHost *pstHost, HHCMIE cm, char *pszWrite, long lLen)
{
	epollevt    event;
    SKClient    *pv = NULL;

	if(pstHost->m_bLTLink)	// 长连接
	{
		if(g_skSock <= 0)
		{
    		if(RC_FAIL == (g_skSock = skConnectServer(pstSavm, pstHost->m_szHostIp, pstHost->m_lHostPort,
           		true, pstHost->m_lTimeOut)))
        	return RC_FAIL;
		}
	}
	else
	{
    	if(RC_FAIL == (g_skSock = skConnectServer(pstSavm, pstHost->m_szHostIp, pstHost->m_lHostPort,
        	true, pstHost->m_lTimeOut)))
        	return RC_FAIL;
	}

    if(MAX_CLIENT < g_skSock)
    {
        SYSTrror(cm, RET_SYSTEM, "链接银联超限");
        return RC_FAIL;
	}        

    if(lLen != lSendBuffer(g_skSock, pszWrite, lLen))
    {
        sockclose(true, g_skSock);
        return RC_FAIL;
    }    

    memset(&event, 0, sizeof(event));
    pv = &g_stClt[g_skSock];
    pv->m_skSock = g_skSock;
	memcpy(&pv->ce, cm, sizeof(HCMIE));
	cm->m_Roop = NULL;
    
    event.data.fd = g_skSock;
    event.data.ptr = pv;
    event.events = EPOLLIN | EPOLLET;
    return epoll_ctl(g_epfd, EPOLL_CTL_ADD, g_skSock, &event);
}

/**************************************************************************************************
    函 数 名：lPutTcpRequest(SATvm *pstSavm, GCts *pstCts, CtsEvent *pstEvent, CtsHost *pstHost)
    功能描述：组织报文发送socket请求数据
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lPutTcpRequest(SATvm *pstSavm, HHCMIE cm, CtsHost *pstHost, char *pszHeader)
{
    GCts    *pstCts = (GCts *)pCmieGloble(cm);
    long    lLen, lOffset, lWrite = lGetUserSize();
    char    szBody[BODY_LEN], *pszWrite = (char *)pGetBuffer();

    IBTrace(cm, "开始组织交易(%d)发往服务器请求报文(%d)超时(%d)", pstCts->m_lSeqNo, 
		pstCts->m_lToMid, pstHost->m_lTimeOut);
	if(!strcmp(pstCts->m_szCallCode, "FILE.ZIP"))
	{
    	if(RC_SUCC != IBTgetstring(cm, "TR_File", szBody, "文件名"))
		{
			SYSTrror(cm, RET_SYSTEM, "获取文件名失败");
			return RC_FAIL;
		}

    	snprintf(g_szFile, sizeof(g_szFile), "%s/file/%s/%s\x00", getenv("HOME"), 
			pstCts->m_szCallSub, szBody);
		unlink(g_szFile);
    	IBTrace(cm, "下载账单:%s", g_szFile);
	}

    if(RC_SUCC != lPackWriteAt(pstSavm, cm, pstCts->m_lToMid, pszWrite, &lWrite))
    {
        SYSTrror(cm, RET_SYSTEM, "组织接出请求报文(%d)失败", pstCts->m_lToMid); 
		return RC_FAIL;
    }

	lOffset = lWrite + LENG_SIGN;
	lLen    = lGetUserSize() - lOffset;
    if(RC_SUCC != lPackRSAsign(pszWrite, lWrite, pszWrite + lOffset, &lLen))
	{
        SYSTrror(cm, RET_SYSTEM, "对接出请求字符串签名失败");
		return RC_FAIL;
	}
    memcpy(pszWrite + lWrite, PACK_SIGN, LENG_SIGN);

	//	先原文签名  后对字段urlencode 在发送
	if(0 > (lWrite = lValuesEncode(pszWrite, lOffset + lLen, szBody, BODY_LEN)))
	{
        SYSTrror(cm, RET_SYSTEM, "组织银行请求数据失败");
		return RC_FAIL;
	}

	lLen = snprintf(pszWrite, lGetUserSize(), "%s%d\r\n\r\n%s", pszHeader, lWrite, szBody);
	pszWrite[lLen] = 0x00;
    IBTrace(cm, "发送服务端(%s%s)的请求数据如下(%d):", pstHost->m_szDomain, 
		pstHost->m_szService, lLen);
    IBTcem(cm, pszWrite, lLen);

	return lWriteRemote(pstSavm, pstHost, cm, pszWrite, lLen);
}

/**************************************************************************************************
	函 数 名：lParseHttp(HttpRecv *pv)
	功能描述：解析http请求头
	返回说明：
	    RC_SUCC             --成功
	    RC_FAIL             --失败
 **************************************************************************************************/
void	vWriteResult(SATvm *pstSavm, HHCMIE ce, char *pszBuffer, long lLen, GCts *pstCts)
{
	long	n;
	char	*s, *p;

	IBTrace(ce, "收到服务端的响应数据如下(%d):", lLen);
    IBTcem(ce, pszBuffer, lLen);
    if(RC_SUCC != lPackReadAt(pstSavm, ce, pstCts->m_lToInMid, pszBuffer, &lLen))
    {
        SYSTrror(ce, RET_SYSTEM, "组织接出请求报文(%d)失败", pstCts->m_lToMid); 
        return ;
    }

    if(!strcmp(pstCts->m_szCallCode, "FILE.ZIP") && (p = strstr(pszBuffer, FILE_SIGN)))
    {
        if(NULL == (s = strstr(p + FILE_LENG, "&")))
            n = lLen - FILE_LENG;
        else
            n = (s - p) - FILE_LENG;

        vWriteFile(p + FILE_LENG, n);
    }
}

/**************************************************************************************************
	函 数 名：lParseHttp(HttpRecv *pv)
	功能描述：解析http请求头
	返回说明：
	    RC_SUCC             --成功
	    RC_FAIL             --失败
 **************************************************************************************************/
long    lParseResp(SKClient *pv, HHCMIE ce)
{
    int     n;
    char    *s = NULL, *q = NULL, szLen[64];

	if(NULL == (pv->m_pvData = strstr(pv->m_szBuffer, HTTP_TAIL)))
	{
		IBTerror(ce, "http报文格式异常");
		return RC_FAIL;
	}
	pv->m_pvData += 4;

    if(s = strstr(pv->m_szBuffer, "Content-Length:"))
	{
        for(s += 15; s < pv->m_pvData && *s == ' '; s ++);
        for(q = s; s < pv->m_pvData && *s != '\r'; s ++);
        
        n = s - q;
        memcpy(szLen, q, n);
        szLen[n] = 0x00;
        pv->m_lBody = atol(szLen);
        pv->m_lOffset = pv->m_pvData - pv->m_szBuffer;
		pv->m_bChunk = false;
	}
    else if(s = strstr(pv->m_szBuffer, "Transfer-Encoding:"))
	{
		// 新HTTP 规范里，只定义了一种编码传输：chunked
		for(s += 18; s < pv->m_pvData && *s == ' '; s ++);
		for(q = s; s < pv->m_pvData && *s != '\r'; s ++);
    	n = s - q;
		memcpy(szLen, q, n);
		szLen[n] = 0x00;
		if(strcmp(szLen, "chunked"))	// 长度+buffer格式
		{
			IBTerror(ce, "不支持的编码传输类型");
			return RC_FAIL;
		}
			
		pv->m_bChunk = true;
		if(NULL == (s = strstr(pv->m_pvData, TAIL_CRLF)))
		{
    		pv->m_bHead = true;
			pv->m_lRecv = pv->m_lBody = 0;
			pv->m_lOffset = BODY_LEN;
			pv->m_pvData = pv->m_szBuffer;
			pv->m_pvData[0] = 0x00;
			return RC_CONTINUE;
		}

    	n = s - pv->m_pvData;
		memcpy(szLen, pv->m_pvData, n);
		szLen[n] = 0x00;
		pv->m_lBody = hxtol(szLen, n);
    	pv->m_lOffset = (s - pv->m_szBuffer) + 2;
		pv->m_pvData += n + 2;	
	}
	else
        return RC_FAIL;

	pv->m_lRecv = pv->m_lRecv - pv->m_lOffset; // 已收报文体大小
    pv->m_lOffset = BODY_LEN - pv->m_lOffset;

    if(pv->m_lRecv < 0)   return RC_FAIL;
    pv->m_bHead = true;

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lParseBody(char *pvData, long *pn)
    功能描述：解析chunked待接收报文数据长度
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
long	lParseBody(char *pvData, long *pn)
{
    char    *s, szLen[64];

    if(NULL == (s = strstr(pvData, TAIL_CRLF)))
		return -1;

    *pn = s - pvData;
    memcpy(szLen, pvData, *pn);
    szLen[*pn] = 0x00;
		
    return hxtol(szLen, *pn);
}

/**************************************************************************************************
    函 数 名：lHttpRespond(SATvm *pstSavm, CtsHost *pstHost, void *arg)
    功能描述：接收核心请求事件任务
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
long    lHttpRespond(SATvm *pstSavm, CtsHost *pstHost, SKClient *pv, GCts *pstCts)
{
    long    lRet, n;

    if(!pv->m_bHead)    //    接收头
    {
        if(0 > (pv->m_lRecv = lRecvBuffer(pv->m_skSock, pv->m_szBuffer, BODY_LEN)))
    		return RC_CLOSE;

		memset(pv->m_szBuffer + pv->m_lRecv, 0, BODY_LEN - pv->m_lRecv);
        if(RC_FAIL == (lRet = lParseResp(pv, &pv->ce)))
    		return RC_CLOSE;
		else if(lRet == RC_CONTINUE)
			return 1;
    }
	
	if(pv->m_lBody == 0 && pv->m_bChunk)
	{
        if(0 > (lRet = lRecvBuffer(pv->m_skSock, pv->m_pvData + pv->m_lRecv,
            pv->m_lOffset - pv->m_lRecv)))
    		return RC_CLOSE;

		pv->m_pvData[pv->m_lRecv + lRet] = 0x00;
		if(-1 == (pv->m_lBody = lParseBody(pv->m_pvData + pv->m_lRecv, &n)))
		{
			pv->m_pvData = pv->m_szBuffer;
			pv->m_lBody = pv->m_lRecv = 0;
			pv->m_lOffset = BODY_LEN;
			pv->m_pvData[0] = 0x00;
			return 1;
		}	

		n += 2;
		pv->m_pvData = pv->m_pvData + n;
		pv->m_lRecv = pv->m_lRecv + lRet - n;
		pv->m_lOffset = BODY_LEN - pv->m_lRecv;
		if(0 == pv->m_lBody)
    		return RC_CLOSE;
        else if(pv->m_lRecv < pv->m_lBody)
            return pv->m_lBody;
	}
    else if(pv->m_lRecv < pv->m_lBody)
    {
        if(0 > (lRet = lRecvBuffer(pv->m_skSock, pv->m_pvData + pv->m_lRecv,
            pv->m_lOffset - pv->m_lRecv)))
    		return RC_CLOSE;

		pv->m_lRecv += lRet;
		pv->m_pvData[pv->m_lRecv] = 0x00;
        if(pv->m_lBody > pv->m_lRecv)
            return 1;
    }
	
	vWriteResult(pstSavm, &pv->ce, pv->m_pvData, pv->m_lBody, pstCts);
	if(!pv->m_bChunk)    return RC_CLOSE;

	pv->m_lBody += 2;
	if(-1 == (pv->m_lBody = lParseBody(pv->m_pvData + pv->m_lBody, &n)))
	{
		pv->m_pvData = pv->m_szBuffer;
		pv->m_lBody = pv->m_lRecv = 0;
		pv->m_lOffset = BODY_LEN;
		pv->m_pvData[0] = 0x00;
		return 1;
	}	
	else if(0 != pv->m_lBody)
	{
		n += pv->m_lBody + 2;
		memmove(pv->m_szBuffer, pv->m_pvData + n, pv->m_lRecv - n);		
		pv->m_pvData = pv->m_szBuffer;
		pv->m_lRecv -= n;
		return lHttpRespond(pstSavm, pstHost, pv, pstCts);
	}

    return RC_CLOSE;
}

/**************************************************************************************************
    函 数 名：vHttpRespond(CtsHost *pstHost)
    功能描述：处理核心响应信息线程
    返回说明：
        无
 **************************************************************************************************/
void    vHttpRespond(CtsHost *pstHost)
{
    CtsProcess  stProcess;
	long        i, nWait = 0;
	SKClient    *pv = NULL;
	GCts        *pstCts = NULL;
	epollevt    events[MAX_EVENTS];
	char		szCmd[512], szFile[256];
    SATvm       *pstSavm = (SATvm *)calloc(1, sizeof(SATvm));

    pthread_detach(pthread_self());
	memset(szCmd, 0, sizeof(szCmd));
	memset(szFile, 0, sizeof(szFile));
    if(!pstSavm)
    {
        IBPerror("初始化线程参数失败, err:%s", strerror(errno));
		exit(-1);
        return ;
    }

    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", sGetProgram());
		exit(-1);
        return ;
    }

	g_epfd = epoll_create(MAX_EVENTS);
    while(1)
    {
        nWait = epoll_wait(g_epfd, events, MAX_EVENTS, 5000);
        for(i = 0; i < nWait; i++)
        {
            pv = (SKClient *)events[i].data.ptr;
			pstCts = (GCts *)pCmieGloble(&pv->ce);
            if(events[i].events & EPOLLIN)
            {
                if(0 < lHttpRespond(pstSavm, pstHost, pv, pstCts))
					continue;

				if(!strcmp(pstCts->m_szCallCode, "FILE.ZIP"))
				{
        			memset(szCmd, 0, sizeof(szCmd));
        			snprintf(szCmd, sizeof(szCmd), "gunzip -c %s.gz > %s 1>/dev/null 2>&1", 
						g_szFile, g_szFile);
        			system(szCmd);
				}	

				lOutPutResponse(pstSavm, &pv->ce);
				epoll_ctl(g_epfd, EPOLL_CTL_DEL, pv->m_skSock, &events[i]);
				sockclose(pstHost->m_bLTLink, pv->m_skSock);
				memset(pv, 0, sizeof(long) * 7);
				continue;
            }
        }
    }

    pthread_exit(NULL);
	exit(-1);
}

/**************************************************************************************************
    函 数 名：main
    功能描述：进程监控主模块
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
int     main(int argc, char *argv[])
{
	pthread_t   tPid;
    CtsHost     stHost;
    CtsProcess  stProcess;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();
	CtsEvent    *pstEvent = (CtsEvent *)pGetEvent();
	char        szPvCert[512], szPuCert[512], szHeader[256];

    vPrintVersion(argv[1]);
    memset(szHeader, 0, sizeof(szHeader));
	memset(szPvCert, 0, sizeof(szPvCert));
    memset(szPuCert, 0, sizeof(szPuCert));
    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));
    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", stProcess.m_szPGName);
        return RC_FAIL;
    }

    if(RC_SUCC != lGetHost(pstSavm, &stHost, stProcess.m_lHost))
    {
        SYSError(RET_SYSTEM, "获取执行码(%s)对应的主机信息失败, %s",
            stProcess.m_szPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

	snprintf(szHeader, sizeof(szHeader), "POST %s%s HTTP/1.1\r\nHost: %s:%d\r\nAccept: */*\r\n"
		"Content-Type:application/x-www-form-urlencoded;charset=UTF-8\r\nContent-Length: ", 
		stHost.m_szDomain, stHost.m_szService, stHost.m_szHostIp, stHost.m_lHostPort);
    snprintf(szPuCert, sizeof(szPuCert), "%s/%s", getenv("CERTDIR"), stHost.m_szAuthUser);
    snprintf(szPvCert, sizeof(szPvCert), "%s/%s", getenv("CERTDIR"), stHost.m_szAuthPwd);
    if(RC_SUCC != lInitRsa(szPvCert, szPuCert))
    {
        fprintf(stderr, "初始化RSA密钥对失败\n");
        return RC_FAIL;
    }

    IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, g_szVersion);
    if(0 != pthread_create(&tPid, NULL, (void *)vHttpRespond, (void *)&stHost))
    {
        fprintf(stderr, "pthread_create error, err:%s.\n", strerror(errno));
        return RC_FAIL;
    }

    while(1)
    {
        if(RC_SUCC == lOutGetRequest(pstSavm, &stProcess, pGetCmie()))
        {
            if(IBPevcache(pstEvent))
			{
    			memset(szHeader, 0, sizeof(szHeader));
    			lGetHost(pstSavm, &stHost, stProcess.m_lHost);
				snprintf(szHeader, sizeof(szHeader), "POST %s%s HTTP/1.1\r\nHost: %s:%d\r\n"
					"Accept: */*\r\nContent-Type:application/x-www-form-urlencoded;charset=UTF-8"
					"\r\nContent-Length: ", stHost.m_szDomain, stHost.m_szService, stHost.m_szHostIp, 
					stHost.m_lHostPort);
                continue;
			}

            if(RC_FAIL == lPutTcpRequest(pstSavm, pGetCmie(), &stHost, szHeader))
				lOutPutResponse(pstSavm, pGetCmie());
			continue;
        }
    }

    return RC_FAIL;
}

/*************************************************************************************************
    code end
**************************************************************************************************/
