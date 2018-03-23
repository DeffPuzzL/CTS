/*************************************************************************************************
    文 件 名：trade.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：交易模块代码
 *************************************************************************************************/
#include    "face.h"

/**************************************************************************************************
    短信系统调用
    调用接口http
 **************************************************************************************************/
/**************************************************************************************************
    函 数 名：lHttpCallBack(char *data, size_t size, size_t nmemb, char *write_data)
    功能描述：http响应回调函数
    返回说明：
    无
 **************************************************************************************************/
size_t  lHttpCallBack(char *ptr, size_t size, size_t nmenb, char *stream)
{
    long    lRecv;
    char    szRecv[10240];
    cJSON    *root = NULL, *pChild = NULL;

    lRecv = sizeof(szRecv) > size * nmenb ? size * nmenb : sizeof(szRecv); 
    memcpy(szRecv, ptr, lRecv);
    szRecv[lRecv] = 0x00;
    if (!(root = cJSON_Parse(szRecv)))
    {
        SYSError(RET_SYSTEM, "JSON解包失败, err:(%s)", strerror(errno));
        return RC_FAIL;
    }

    if(NULL == (pChild = cJSON_GetObjectItem(root, "status")))
    {
        cJSON_Delete(root);
        SYSError(RET_SYSTEM, "获取JSON包字段status失败失败, err:(%s)", strerror(errno));
        return RC_FAIL;
    }

    if(1 != pChild->valueint)
    {
        cJSON_Delete(root);
        SYSError(RET_SYSTEM, "%s", "推送短信失败");
        return RC_FAIL;
    }

    cJSON_Delete(root);
    IBPrace("短信推送成功");

    return size * nmenb;    //  必须返回这个大小, 否则只回调一次
}

/**************************************************************************************************
    函 数 名：vHttpPostSms(char *pszUrl, char *pszUtf8)
    功能描述：调用短信接口
    返回说明：
        无
 **************************************************************************************************/
void    vHttpPostSms(char *pszUrl, char *pszUtf8)
{
    CURLcode    code;
    CURL        *curl = NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    if(NULL == (curl = curl_easy_init()))
    {
        SYSError(RET_SYSTEM, "初始化HTTP失败");
        return ;
    }

    if(CURLE_OK != (code = curl_easy_setopt(curl, CURLOPT_URL, pszUrl)))
    {
        SYSError(RET_SYSTEM, "设置http参数错误, err:(%d)(%s)", code,
            curl_easy_strerror(code));
        return ;
    }

    if(CURLE_OK != (code = curl_easy_setopt(curl, CURLOPT_POST, 1L)))
    {
        SYSError(RET_SYSTEM, "设置http参数错误, err:(%d)(%s)", code,
            curl_easy_strerror(code));
        return ;
    }

    if(CURLE_OK != (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L)))
    {
        SYSError(RET_SYSTEM, "设置http参数错误, err:(%d)(%s)", code,
            curl_easy_strerror(code));
        return ;
    }

    if(CURLE_OK != (code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pszUtf8)))
    {
        SYSError(RET_SYSTEM, "设置http参数错误, err:(%d)(%s)", code,
            curl_easy_strerror(code));
        return ;
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &lHttpCallBack);

    if(CURLE_OK != (code = curl_easy_perform(curl)))
    {
        curl_easy_cleanup(curl);
        SYSError(RET_SYSTEM, "同步调用http接口失败, err:(%d)(%s)", code,
            curl_easy_strerror(code));
        return ;
    }

    curl_easy_cleanup(curl);
    return ;
}

/**************************************************************************************************
    函 数 名：vSetTradePool()
    功能描述：设置交易公共信息
    返回说明：
        无
 **************************************************************************************************/
void    vSetTradePool(HHCMIE cm)
{
    GCts    *pstCts = (GCts *)pCmieGloble(cm);

    IBTputstring(cm, "TR_TxDate", pstCts->m_szTxDate);
    IBTputstring(cm, "TR_TxTime", pstCts->m_szTxTime);
    IBTputlong(cm, "TR_SeqNo", pstCts->m_lSeqNo);
    IBTputstring(cm, "TR_SvrCode", pstCts->m_szInCode);
    IBTputstring(cm, "TR_SubCode", pstCts->m_szInSCode);
    IBTputstring(cm, "TR_TrCode", pstCts->m_szTrCode);
    IBTputstring(cm, "TR_SvrName", pstCts->m_szSvrName);
    IBTputstring(cm, "TR_ClearDate", pstCts->m_szCrDate);
    IBTputlong(cm, "TR_Status", pstCts->m_lState);
}

/**************************************************************************************************
    函 数 名：lResetTrans(SATvm *pstSavm, HHCMIE cm)
    功能描述：释放交易缓存
    返回说明：
        TRUE                --成功
        FALSE                --失败
 **************************************************************************************************/
long    lResetTrans(SATvm *pstSavm, HHCMIE cm)
{
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(RC_SUCC != lFreeTrans(pstSavm, pstEvent->m_lSeqNo, pstEvent->m_uData))
	{
        IBTerror(cm, "清理交易(%d)运行流水失败, %s", pstEvent->m_lSeqNo, 
			sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
	}

    //    监控发起的交易，说明已经记录交易信息
    switch(pstCts->m_lLaunch)
    {
    case LAUNCH_BY_NORMAL:
    case LAUNCH_BY_NRMLAS:
    case LAUNCH_BY_SYCALL:
    case LAUNCH_BY_ASCALL:
    case LAUNCH_BY_HASYNC:
        vLogWriteList(pstCts);
        break;
    default:
        break;
    }

    if(!IBPasyn(pstCts->m_lAsyn))
        return RC_SUCC;

    if(RC_SUCC != lResetAsyn(pstSavm, pstEvent->m_lSeqNo, pstCts->m_szCrDate))
        return RC_FAIL;

    pstCts->m_lAsyn = TRADE_DISABLE_ASYN;

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lResetTrade(SATvm *pstSavm, CtsEvent *pstEvent, GCts *pstCts)
    功能描述：释放交易缓存
    返回说明：
        TRUE                --成功
        FALSE                --失败
 **************************************************************************************************/
long    lResetTrade(SATvm *pstSavm, HHCMIE cm)
{
	IBTresetpool(cm);
    //    其他参数的释放

    return lResetTrans(pstSavm, cm);
}

/**************************************************************************************************
    函 数 名：lRecoverByAsyn()
    功能描述：根据异步标识恢复原交易
    返回说明：
        TRUE                --成功
        FALSE                --失败
 **************************************************************************************************/
long    lRecoverByAsyn(SATvm *pstSavm, HHCMIE cm, char *pszIdx)
{
    CtsAsyn   stIdx;
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(RC_SUCC != lGetAsynIdx(pstSavm, &stIdx, pszIdx))
        return RC_FAIL;

    pstEvent->m_uData  = stIdx.m_uData;
    pstEvent->m_lFlow  = stIdx.m_lFlow;
    pstEvent->m_lResv  = stIdx.m_lResv;
    pstEvent->m_lState = EVENT_EVPOOL;
    pstEvent->m_lSeqNo = stIdx.m_lSeqNo;
    if(RC_SUCC != lGetGlobleCts(pstSavm, cm, TRUE))
    {
        SYSTrror(cm, RET_SYSTEM, "获取交易(%d)运行交换参数失败, err:%s", pstEvent->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        //  非必要不要退出进程，以免造成业务的堵塞
        return RC_FAIL;
    }

    if(RC_SUCC != lDeleteAsyn(pstSavm, &stIdx))
    {
        IBPwarn("清理交易(%d)异步标识(%s)失败, err:%s", pstEvent->m_lSeqNo, pszIdx,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    IBPdebug("清理交易(%d)异步标识(%s)成功!", pstEvent->m_lSeqNo, pszIdx);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lExportGCts(CtsEvent *pstEvent)
    功能描述：展示当前系统交易列表
    返回说明：
        TRUE                --成功
        FALSE                --失败
 **************************************************************************************************/
long    lExportGCts(CtsEvent *pstEvent, GCts *pstCts)
{
    size_t    lOffset;
    RunTime *pstRun = NULL;
    SHTruck *pstTruck = NULL;
    SATvm    *pstSavm = (SATvm *)pGetSATvm();

    if(!pstEvent)
    {
        fprintf(stdout, "   ------------------------------------------------------------------"
            "-------------------\n");
        fprintf(stdout, "   交易流水号 服务编号           服务名称           接入交易码/子码"
            "  交易日期 超时 耗时\n");
        fprintf(stdout, "   -------------------------------------------------------------------"
            "------------------\n");
    }

    pstSavm->tblName = CTS_RUNTIME_POOL;
    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, pstSavm->tblName)))
    {
        IBPerror("定义表共享池游标失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(lOffset = lGetTblData(pstSavm->tblName); lOffset <= lGetTableSize(pstSavm->tblName);
        lOffset += lGetRowTruck(pstSavm->tblName))
    {
        pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, lOffset);
        if(IS_TRUCK_NULL(pstTruck))
            continue;

        memcpy(pstCts, pstTruck->m_pvData, sizeof(GCts));
        pstCts->m_lUseTime = lGetUnixTime() - pstCts->m_lInTime;
        if(pstEvent)
        {
            if(pstCts->m_lSeqNo != pstEvent->m_lSeqNo)
                continue;
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return RC_SUCC;
        }
        
        fprintf(stdout, "    %-10ld%-12s%-26s%10s/%6s%8s %4d %4d\n", pstCts->m_lSeqNo, 
            pstCts->m_szTrCode, pstCts->m_szSvrName, pstCts->m_szInCode, pstCts->m_szInSCode, 
            pstCts->m_szTxDate, pstCts->m_lTimeOut, pstCts->m_lUseTime);
        fflush(stdout);
    }
    vTblDisconnect(pstSavm, pstSavm->tblName);
    fprintf(stdout, "   ------------------------------------------------------------------------"
        "-------------\n");

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lCallSmsModule(const char* fmt, ...)
    功能描述：发送短信
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 *************************************************************************************************/
long    lCallSmsModule(const char* fmt, ...)
{
    va_list args;
    long    lLen = 0;
    char    szMsg[1024], szBuffer[2048];
    char    szPhone[1101], szUrl[1024], szSub[10];

    memset(szMsg, 0, sizeof(szMsg));
    memset(szSub, 0, sizeof(szSub));
    memset(szUrl, 0, sizeof(szUrl));
    memset(szPhone, 0, sizeof(szPhone));
    memset(szBuffer, 0, sizeof(szBuffer));

    va_start(args, fmt);
    vsnprintf(szMsg, sizeof(szMsg), fmt, args);
    va_end(args);

    strncpy(szSub, sGetDbsEnv("SMS_SUB_CODE"), sizeof(szSub) - 1);
    strncpy(szUrl, sGetDbsEnv("SMS_URL_ADDR"), sizeof(szUrl) - 1);
    strncpy(szPhone, sGetDbsEnv("SMS_TEL_PHONE"), sizeof(szPhone) - 1); 

    /*  手机号最多支持100个, 短信内容最大329个*/
    lLen = snprintf(szBuffer, sizeof(szBuffer), "mobile=%s&content=%s&subType=%s", szPhone, 
        szMsg, szSub);

    IBPrace("向地址[%s]推送内容如下(%d):\n%s", szUrl, lLen, szBuffer);

    vHttpPostSms(szUrl, szBuffer);
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lcsCall(SATvm *pstSavm, char *pszCode)
    功能描述：发起新的平台交易
        pstSavm            --运行内存引擎句柄    
        pstCts            --交易运行结构
        pszTrCode        --需要同步调起的交易码
    返回说明：
        >0                    --新交易的流水号
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long	lCall(SATvm *pstSavm, char *pszTrCode)
{
    struct stat st;
	CtsGroup    stGroup;
    CtsService  stService;
	HHCMIE      cm = (HHCMIE)pGetCmie();
    GCts        *pstCts   = (GCts *)pGetGloble(cm);
    CtsEvent    *pstEvent = (CtsEvent *)pGetEvent(cm);
	char        szLog[PATH_LOGNMAE_LEN], szBak[PATH_LOGNMAE_LEN];

    if(!pszTrCode)
    {
        IBPerror("交易码不能为空");
        return RC_FAIL;
    }

    if(!bIsSysNormal(pstSavm))
    {
        SYSError(RET_SYSTEM, "系统已暂停服务, 请稍后再试！");
        return RC_FAIL;
    }

	memset(szLog, 0, sizeof(szLog));
	memset(szBak, 0, sizeof(szBak));
    memset(pstCts, 0, sizeof(GCts));
    memset(pstEvent, 0, sizeof(CtsEvent));
    if(RC_SUCC != lGetService(pstSavm, &stService, pszTrCode))
        return RC_FAIL;

    if(!IBPIsable(stService.m_lState))
    {
        IBPerror("服务(%s)已停用", pszTrCode);
        return RC_FAIL;
    }

    vFillTime(pstCts);
    vSetSuccess(pstCts);
    pstCts->m_lInPid  = getpid();
    pstCts->m_lLaunch = LAUNCH_BY_SYCALL;
    pstCts->m_lInQid  = lGetInQueue();
    pstCts->m_lInTime = lGetUnixTime();
    pstCts->m_lHost   = stService.m_lHost;
    pstCts->m_lSvrID  = stService.m_lSvrID;
    pstCts->m_lSType  = stService.m_lSvrType;
    pstCts->m_lLogLev = stService.m_lLogLevel;
    pstCts->m_lTimeOut = stService.m_lTimeOut > 0 ? stService.m_lTimeOut : 60;
    strncpy(pstCts->m_szTrCode, stService.m_szTrCode, sizeof(pstCts->m_szTrCode));
    strncpy(pstCts->m_szSvrName, stService.m_szSvrName, sizeof(pstCts->m_szSvrName));
    strncpy(pstCts->m_szLogName, stService.m_szLogName, sizeof(pstCts->m_szLogName));
    snprintf(szLog, sizeof(szLog), "%s/%s", sLogGetPath(), pstCts->m_szLogName);
    if(0 == stat(szLog, &st) && st.st_size >= lLogGetSize())
    {
        snprintf(szBak, sizeof(szBak), "%s.%s", szLog, pstCts->m_szTxTime);
        rename(szLog, szBak);
    }

    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, CTS_CORE, DEFAUT_HOST))
    {
        SYSTrror(cm, RET_SYSTEM, "获取进程(cts_core)信息失败, %s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

	vSetInQueue(stGroup.m_lQid);
	vSetToQueue(stGroup.m_lQid);
	pstCts->m_lInQid = stGroup.m_lQid;
    if(RC_SUCC != lGetIdleProcess(pstSavm, CTS_CORE, stGroup.m_lAppPos, stGroup.m_lMaxNum, 
		&pstEvent->m_lMType))
        return RC_FAIL;

    vSetTradePool(cm);
    pstCts->m_lPid = pstEvent->m_lMType;
    if(RC_SUCC != lInitCall(pstSavm, cm, pstCts, &pstEvent->m_uData))
        return RC_FAIL;

	vLogStore(cm);
    pstEvent->m_lState = EVENT_EVPOOL;
    pstEvent->m_lFlow  = CTS_FLOW_BEGIN;
    pstEvent->m_lSeqNo = pstCts->m_lSeqNo;
    if(RC_SUCC != _lLaunchEvent(pstSavm, cm, cm, pstCts->m_lLaunch, true))
	{
		vLogRestore(cm);
        return RC_FAIL;
	}

	vLogRestore(cm);
    IBTsysm(cm, "交易(%s)返回：处理标志(%d)响应码(%s)响应信息(%s)", pstCts->m_szTrCode, 
        pstCts->m_lState, pstCts->m_szRetCode, pstCts->m_szRetMsg);

	return pstCts->m_lSeqNo;
}

/**************************************************************************************************
    函 数 名：lStartTrade(SATvm *pstSavm, char *pszCode)
    功能描述：发起新的平台交易
        pstSavm            --运行内存引擎句柄    
        pstCts            --交易运行结构
        pszTrCode        --需要同步调起的交易码
    返回说明：
        >0                    --新交易的流水号
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long	lStartTrade(SATvm *pstSavm, HHCMIE cm, char *pszTrCode)
{
    struct stat st;
    CtsService  stService;
    GCts        *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent    *pstEvent = (CtsEvent *)pCmieEvent(cm);
    char        szLog[PATH_LOGNMAE_LEN], szBak[PATH_LOGNMAE_LEN];

    if(!pszTrCode)
    {
        IBPerror("交易码不能为空");
        return RC_FAIL;
    }

    if(!bIsSysNormal(pstSavm))
    {
        SYSTrror(cm, RET_SYSTEM, "系统已暂停服务, 请稍后再试！");
        return RC_FAIL;
    }

    if(RC_SUCC != lGetService(pstSavm, &stService, pszTrCode))
        return RC_FAIL;

    if(!IBPIsable(stService.m_lState))
    {
        IBPerror("服务(%s)已停用", pszTrCode);
        return RC_FAIL;
    }

    vFillTime(pstCts);
    vSetSuccess(pstCts);
    pstCts->m_lInQid  = lGetInQueue();
    pstCts->m_lInTime = lGetUnixTime();
    pstCts->m_lInHost = pstCts->m_lHost;
    pstCts->m_lLaunch = LAUNCH_BY_SYCALL;
    pstCts->m_lHost   = stService.m_lHost;
    pstCts->m_lSvrID  = stService.m_lSvrID;
    pstCts->m_lSType  = stService.m_lSvrType;
    pstCts->m_lLogLev = stService.m_lLogLevel;
    pstCts->m_lInPid  = lGetMaxPid() + getpid();
    pstCts->m_lTimeOut = stService.m_lTimeOut > 0 ? stService.m_lTimeOut : 60;
    strncpy(pstCts->m_szTrCode, stService.m_szTrCode, sizeof(pstCts->m_szTrCode));
    strncpy(pstCts->m_szSvrName, stService.m_szSvrName, sizeof(pstCts->m_szSvrName));
    strncpy(pstCts->m_szLogName, stService.m_szLogName, sizeof(pstCts->m_szLogName));
    snprintf(szLog, sizeof(szLog), "%s/%s", sLogGetPath(), pstCts->m_szLogName);
    if(0 == stat(szLog, &st) && st.st_size >= lLogGetSize())
    {
        snprintf(szBak, sizeof(szBak), "%s.%s", szLog, pstCts->m_szTxTime);
        rename(szLog, szBak);
    }

    if(RC_SUCC != lGetIdleProcess(pstSavm, CTS_CORE, lGetToPos(), lGetToNum(), &pstEvent->m_lMType))
        return RC_FAIL;

    vSetTradePool(cm);
    pstCts->m_lPid = pstEvent->m_lMType;
    pstEvent->m_lResv  = pstCts->m_lResv;
    if(RC_SUCC != lInitCall(pstSavm, cm, pstCts, &pstEvent->m_uData))
        return RC_FAIL;

	vLogStore(cm);
    pstEvent->m_lState = EVENT_EVPOOL;
    pstEvent->m_lFlow  = CTS_FLOW_BEGIN;
    pstEvent->m_lSeqNo = pstCts->m_lSeqNo;
    if(RC_SUCC != _lLaunchEvent(pstSavm, cm, cm, pstCts->m_lLaunch, true))
	{
		vLogRestore(cm);
        return RC_FAIL;
	}

	vLogRestore(cm);
    IBTsysm(cm, "交易(%s)返回：处理标志(%d)响应码(%s)响应信息(%s)", pstCts->m_szTrCode, 
        pstCts->m_lState, pstCts->m_szRetCode, pstCts->m_szRetMsg);

	return pstCts->m_lSeqNo;
}

/**************************************************************************************************
    函 数 名：_lLaunchTrade(SATvm *pstSavm, GCts *pstCts, char *pszTrCode, Benum eType, BOOL bPool)
    功能描述：发起新的平台交易
        pstSavm                --运行内存引擎句柄    
        pstCts                 --交易运行结构
        pszTrCode              --需要同步调起的交易码
        eType                  --同步/异步标识    
    返回说明：
        >0                     --新交易的流水号
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lLaunchTrade(SATvm *pstSavm, HHCMIE cm, char *pszTrCode, Benum eType, BOOL bPool)
{
    struct stat st;
	HCMIE      mce;
    CtsService stService;
    char       szBak[PATH_LOGNMAE_LEN];
    GCts       *pstCts   = (GCts *)pCmieGloble(cm);
    GCts       *pstNts   = (GCts *)pCmieGloble(&mce);
    CtsEvent   *pstEvent = (CtsEvent *)pCmieEvent(&mce);

    if(!pszTrCode)
    {
        IBPerror("交易码不能为空");
        return RC_FAIL;
    }

    if(!bIsSysNormal(pstSavm))
    {
        SYSTrror(cm, RET_SYSTEM, "系统已暂停服务, 请稍后再试！");
        return RC_FAIL;
    }

	memset(&mce, 0, sizeof(HCMIE));
    if(RC_SUCC != lGetService(pstSavm, &stService, pszTrCode))
        return RC_FAIL;

    if(!IBPIsable(stService.m_lState))
    {
        IBPerror("服务(%s)已停用", pszTrCode);
        return RC_FAIL;
    }

	vFillTime(pstNts);
    vSetSuccess(pstNts);
    pstNts->m_lLaunch = eType;
    pstNts->m_lInQid  = lGetInQueue();
    pstNts->m_lInTime = lGetUnixTime();
    pstNts->m_lInHost = pstCts->m_lHost;
    pstNts->m_lHost   = stService.m_lHost;
    pstNts->m_lSvrID  = stService.m_lSvrID;
    pstNts->m_lSType  = stService.m_lSvrType;
    pstNts->m_lLogLev = stService.m_lLogLevel;
    pstNts->m_lInPid  = lGetMaxPid() + getpid();
    pstNts->m_lTimeOut = stService.m_lTimeOut > 0 ? stService.m_lTimeOut : 60;
    strncpy(pstNts->m_szInCode, pstCts->m_szTrCode, sizeof(pstNts->m_szInCode));
    strncpy(pstNts->m_szTrCode, stService.m_szTrCode, sizeof(pstNts->m_szTrCode));
    strncpy(pstNts->m_szSvrName, stService.m_szSvrName, sizeof(pstNts->m_szSvrName));
    strncpy(pstNts->m_szLogName, stService.m_szLogName, sizeof(pstNts->m_szLogName));
	snprintf(mce.m_szLog, sizeof(mce.m_szLog), "%s/%s", sLogGetPath(), pstNts->m_szLogName);
    if(0 == stat(mce.m_szLog, &st) && st.st_size >= lLogGetSize())
    {
        snprintf(szBak, sizeof(szBak), "%s.%s", mce.m_szLog, pstCts->m_szTxTime);
        rename(mce.m_szLog, szBak);
    }

    if(RC_SUCC != lGetIdleProcess(pstSavm, CTS_CORE, lGetToPos(), lGetToNum(), &pstEvent->m_lMType))
        return RC_FAIL;

    vSetTradePool(&mce);
	vCombinePool(&mce, &cm->m_Roop);
    pstNts->m_lPid = pstEvent->m_lMType;
    if(RC_SUCC != lInitCall(pstSavm, &mce, pstNts, &pstEvent->m_uData))
        return RC_FAIL;

	IBTresetpool(&mce);
    pstEvent->m_lState = EVENT_EVPOOL;
    pstEvent->m_lFlow  = CTS_FLOW_BEGIN;
    pstEvent->m_lSeqNo = pstNts->m_lSeqNo;
    if(RC_SUCC != _lLaunchEvent(pstSavm, cm, &mce, pstNts->m_lLaunch, bPool))
        return RC_FAIL;

    if(LAUNCH_BY_SYCALL == eType)    //    同步
    {
        IBTsysm(cm, "交易(%s)返回：处理标志(%d)响应码(%s)响应信息(%s)", pstNts->m_szTrCode, 
            pstNts->m_lState, pstNts->m_szRetCode, pstNts->m_szRetMsg);
        if(IBPstaterr(pstNts->m_lState))
        {
            IBTsysm(cm, "交易(%s)同步发起系统交易(%s)流水号(%d)调用失败", pstCts->m_szTrCode, 
                pszTrCode, pstNts->m_lSeqNo);
        }

        pstCts->m_lState = pstNts->m_lState;
        strcpy(pstCts->m_szRetMsg, pstNts->m_szRetMsg);
        strcpy(pstCts->m_szRetCode, pstNts->m_szRetCode);

        return pstNts->m_lSeqNo;
    }

    return pstNts->m_lSeqNo;
}

/**************************************************************************************************
    函 数 名：lNewTrade()
    功能描述：同步调用平台交易并等待结果
        pstSavm            --运行内存引擎句柄    
        pszTrCode        --需要同步调起的交易码
        bIsInherit        --变量池是否继承
        bPool            --是否回传变量池
    返回说明：
        新交易失败，将会
        >0                    --新交易的流水号
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lNewTrade(SATvm *pstSavm, HHCMIE cm, char *pszTrCode, BOOL bPool)
{
    return _lLaunchTrade(pstSavm, cm, pszTrCode, LAUNCH_BY_SYCALL, bPool);
}

/**************************************************************************************************
    函 数 名：lCallTrade()
    功能描述：异步调用平台交易并等待结果
    返回说明：
        >0                    --新交易的流水号
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lCallTrade(SATvm *pstSavm, HHCMIE cm, char *pszTrCode)
{
    return _lLaunchTrade(pstSavm, cm, pszTrCode, LAUNCH_BY_ASCALL, false);
}

/*************************************************************************************************
    code end
 *************************************************************************************************/
