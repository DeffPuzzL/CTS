/**************************************************************************************************
    文 件 名：export_txn.pcpp
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：demo实现业务模块
 **************************************************************************************************/
#include    "face.h"
#include    <curl/curl.h>  

typedef struct curl_slist scurlist;
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
    函 数 名：lHttpRespond(char *ptr, size_t size, size_t nmenb, char *stream)
    功能描述：解析结算返回的响应
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
size_t  lHttpRespond(char *ptr, size_t size, size_t nmenb, char *stream)
{
    long    lRecv = lGetUserSize();
    GCts    *pstCts = (GCts *)pGetGloble();
    SATvm   *pstSavm = (SATvm *)pGetSATvm();
    char    *pszRecv = (char *)pGetBuffer();

    lRecv = size * nmenb;
    if(lRecv > lGetUserSize())
    {
        SYSError(RET_SYSTEM, "报文长度超限(%d)>(%d)", lRecv, lGetUserSize());
        return lRecv;
    }
        
    memcpy(pszRecv, ptr, lRecv);    
    IBPrace("收到HTTP响应数据如下:(%d):", lRecv);
    IBPcem(pszRecv, lRecv);
    if(RC_SUCC != lPackRead(pstSavm, pstCts->m_lToInMid, pszRecv, &lRecv))
    {
        SYSError(RET_SYSTEM, "解读报文(%d)失败, %s", pstCts->m_lInMid, 
            sGetTError(pstSavm->m_lErrno));
        return lRecv;
    }

    return lRecv;
}

/**************************************************************************************************
    函 数 名：lPutTcpRequest(SATvm *pstSavm, GCts *pstCts, HHCMIE cm, CtsHost *pstHost)
    功能描述：组织报文发送socket请求数据
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lPutTcpRequest(SATvm *pstSavm, HHCMIE cm, CtsHost *pstHost)
{
    CURLcode    code;
    CURL        *curl = NULL;
    scurlist    *pList = NULL;
    long        lWrite = lGetUserSize();
    GCts        *pstCts = (GCts *)pGetGloble();
    char        szUrl[256], *pszWrite = (char *)pGetBuffer();

    memset(szUrl, 0, sizeof(szUrl));
	IBPgetstring("TR_NotifyUrl", szUrl, "通知请求地址");
    if(IBPIserr())
        return ;

    IBPrace("开始组织交易(%d)发往服务器请求报文(%d) (%d)", pstCts->m_lSeqNo, pstCts->m_lToMid, 
        pstHost->m_lTimeOut);
    if(RC_SUCC != lPackWrite(pstSavm, pstCts->m_lToMid, pszWrite, &lWrite))
    {
        SYSError(RET_SYSTEM, "组织接出请求报文(%d)失败", pstCts->m_lToMid); 
        return RC_IGNORE;
    }
    IBPrace("发送服务端(%s)的请求数据如下(%d):", szUrl, lWrite);
    IBPcem(pszWrite, lWrite);

    if(NULL == (curl = curl_easy_init()))
    {
        SYSError(RET_SYSTEM, "初始化HTTP失败");
        return RC_FAIL;
    }

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, szUrl);
    pList = curl_slist_append(pList, "Content-Type:text/plain;charset=UTF-8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pList); 
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, pstHost->m_lTimeOut);
    if(CURLE_OK != (code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pszWrite)))
    {
        SYSError(RET_SYSTEM, "设置http参数错误, (%d)(%s)", code, curl_easy_strerror(code));
        return RC_FAIL;
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &lHttpRespond);
    if(CURLE_OK != (code = curl_easy_perform(curl)))
    {
        SYSError(RET_SYSTEM, "同步调用http接口失败, (%d)(%s)", code, curl_easy_strerror(code));
        curl_easy_cleanup(curl);
        return RC_FAIL;
    }   
    
    curl_easy_cleanup(curl);

    return RC_SUCC;
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
    CtsHost     stHost;
    CtsProcess  stProcess;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();
    CtsEvent    *pstEvent = (CtsEvent *)pGetEvent();

    vPrintVersion(argv[1]);
    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));
    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", stProcess.m_szPGName);
        return RC_FAIL;
    }

    if(RC_SUCC != lGetHost(pstSavm, &stHost, stProcess.m_lHost))
    {
        SYSError(RET_SYSTEM, "获取执行码(%s)对应的主机信息失败, %s", stProcess.m_szPGName, 
			sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, g_szVersion);
    curl_global_init(CURL_GLOBAL_ALL);
    while(1)
    {
        if(RC_SUCC == lOutGetRequest(pstSavm, &stProcess, pGetCmie()))
        {
            if(IBPevcache(pstEvent))
                continue;

            lPutTcpRequest(pstSavm, pGetCmie(), &stHost);
        }

        lOutPutResponse(pstSavm, pGetCmie());
    }
    curl_global_cleanup();

    return RC_FAIL;
}

/*************************************************************************************************
    code end
**************************************************************************************************/
