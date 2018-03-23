/*************************************************************************************************
    文 件 名  : cts_manage.c
    作    者  : DeffPuzzL
    版    本  : V2.0.0.0
    创建日期  : 2016-03-07
    描    述  : 用http报文调用结算接口
 ************************************************************************************************/
#include    "face.h"

char        *g_pszWrite = NULL;

/**************************************************************************************************
    心跳包宏定义
 **************************************************************************************************/
#define        vFieldGet(pptr, dst, dst_len) { \
    long lLen = 0; \
    char szLen[6] = {0}; \
    memcpy(szLen, *pptr, 5); \
    lLen = atol(szLen); \
    if (lLen > dst_len) \
    { \
        SYSError(RET_SYSTEM, "字段超长"); \
        return RC_FAIL; \
    } \
    *pptr += 5; \
    memcpy(dst, *pptr, lLen); \
    *pptr += lLen; \
    }

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
    函 数 名：vSocketReturn(BSock skSock)
    功能描述：接收核心请求事件任务
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
void    vSocketReturn(BSock skSock, char *pszMsg)
{
    char    szLen[7];
    long    i, lLen, lRet, lWrite;

    memset(szLen, 0, sizeof(szLen));
    if(pszMsg)
    {
        lLen = snprintf(g_pszWrite + 6, SETUP_MAX_SIZE - 6, "%05d%s%05d%s%s",
            strlen(sGetRetCode()), sGetRetCode(), strlen(sGetRetMsg()), sGetRetMsg(), pszMsg);
    }
    else
    {
        lLen = snprintf(g_pszWrite + 6, SETUP_MAX_SIZE - 6, "%05d%s%05d%s",
            strlen(sGetRetCode()), sGetRetCode(), strlen(sGetRetMsg()), sGetRetMsg());
    }

    snprintf(szLen, sizeof(szLen), "%06d", lLen);
    memcpy(g_pszWrite, szLen, 6);
    lWrite = lLen + 6;

    IBPdebug("响应数据如下(%d):\n%s", lWrite, g_pszWrite);
    if(lWrite != lSendBuffer(skSock, g_pszWrite, lWrite))
    {
        IBPerror("发送任务响应失败, err:%s", strerror(errno));
        return ;
    }

    return ;
}

/**************************************************************************************************
    函 数 名：lLogin(BSock skSock, char *pszBuffer)
    功能描述：用户登录
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lLogin(BSock skSock, char *pszBuffer)
{
    CtsUser stUser;
    char    *ptr = pszBuffer;
    SATvm    *pstSavm = (SATvm *)pGetSATvm();

    memset(&stUser, 0, sizeof(CtsUser));
    if(!pszBuffer)
    {
        SYSError(RET_SYSTEM, "系统故障");
        return RC_FAIL;
    }

    vFieldGet(&ptr, stUser.m_szUser, sizeof(stUser.m_szUser))
    vFieldGet(&ptr, stUser.m_szPasswd, sizeof(stUser.m_szPasswd));

    pstSavm->pstVoid = &stUser;
    pstSavm->lFind   = CHK_SELECT;
    pstSavm->tblName = CTS_MANAGER_USER;
    pstSavm->lSize   = sizeof(CtsUser);
    if(RC_SUCC != lSelect(pstSavm, &stUser))
    {
        SYSError(RET_SYSTEM, "用户名或密码错误");
        return RC_FAIL;
    }
    
    vSocketReturn(skSock, NULL);
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lConfigDown(BSock skSock)
    功能描述：从内存读取配置文件
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
long    lConfigDown(BSock skSock)
{
    char    szHead[1024], szLen[10];
    long    i, lWrite, lRet, lLen = 0;

    memset(szLen, 0, sizeof(szLen));
    memset(szHead, 0, sizeof(szHead));
    memset(g_pszWrite, 0, SETUP_MAX_SIZE);
    if (RC_FAIL == (lWrite = lExportEngine(&g_pszWrite, SETUP_MAX_SIZE)))
    {
        SYSError(RET_SYSTEM, "获取引擎数据(%s)失败, err:(%s)", sGetTError(lGetTErrno()));
        return RC_FAIL;
    }

    lLen = snprintf(szHead + 6, sizeof(szHead) - 6, "%05d%s%05d%s%05d",
        strlen(sGetRetCode()), sGetRetCode(), strlen(sGetRetMsg()), sGetRetMsg(), lWrite);
    snprintf(szLen, sizeof(szLen), "%06d", lLen + lWrite);
    memcpy(szHead, szLen, 6);
    lLen += 6;

    if(lLen != (lSendBuffer(skSock, szHead, lLen)))
    {
        SYSError(RET_SYSTEM, "发送数据(%s)失败, err:(%s)", szHead, strerror(errno));
        return RC_FAIL;
    }

//  IBPcem(g_pszWrite, lWrite);
    for(i = 0, lLen = 0; lLen < lWrite; i ++, usleep(100))
    {
        if((lRet = send(skSock, g_pszWrite + lLen, lWrite - lLen, 0)) <= 0)
	{
            if(EAGAIN == errno) continue;
            return RC_FAIL;
	}

        lLen += lRet;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lConfigSave(BSock skSock)
    功能描述：从内存读取配置文件
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lConfigSave(BSock skSock)
{
    long        i = 0;
    FILE        *fp = NULL;

    if (!getenv("SETUPCFG"))
    {
        SYSError(RET_SYSTEM, "系统故障");
        return RC_FAIL;
    }

    memset(g_pszWrite, 0, SETUP_MAX_SIZE);
    if (RC_FAIL == lExportEngine(&g_pszWrite, SETUP_MAX_SIZE))
        return RC_FAIL;

    if (!(fp = fopen(getenv("SETUPCFG"), "w")))
    {
        SYSError(RET_SYSTEM, "系统故障");
        return RC_FAIL;
    }

    fprintf(fp, "%s", g_pszWrite);
    fclose(fp);

    vSocketReturn(skSock, NULL);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lHostUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
    功能描述：同步主机信息
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lHostUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
{
    long    lRet = RC_SUCC;
    char    szMsg[128] = {0};
    CtsHost    stHost;

    memset(&stHost, 0, sizeof(CtsHost));
    vGetHostCfg(pszBuffer + 5, &stHost);

    switch (lTxCode)
    {
        case 3000:
            lRet = lUpdateHost(pstSavm, &stHost);
            break;
        case 3010:
            lRet = lInsertHost(pstSavm, &stHost);
            break;
        case 3020:        //    删除主机，同时需要删除主机下的所有配置信息
            lRet = lRemoveHost(pstSavm, &stHost);
            break;
        case 3030:        //    获取主机编号
            snprintf(szMsg, sizeof(szMsg), "%05d%0*d", 6, 6, lGenHostIdx());
            break;
        default:
            SYSError(RET_SYSTEM, "系统不支持");
            return RC_FAIL;
    }

    if(RC_SUCC != lRet)
    {
        SYSError(RET_SYSTEM, "%s", sGetTError(pstSavm->m_lErrno));
        return lRet;
    }
    else
    {
        vSocketReturn(skSock, szMsg);
        return RC_SUCC;
    }
}

/**************************************************************************************************
    函 数 名：lClassUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
    功能描述：同步组件配置信息
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lClassUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
{
    CtsClass    stClass;
    long        lRet = RC_SUCC;
    char        szMsg[128] = {0};

    memset(&stClass, 0, sizeof(CtsClass));
    vGetClassCfg(pszBuffer + 5, &stClass);

    switch (lTxCode)
    {
    case 4000:
        lRet = lUpdateClass(pstSavm, &stClass);
        break;
    case 4010:
        lRet = lInsertClass(pstSavm, &stClass);
        break;
    case 4020:
        lRet = lRemoveClass(pstSavm, &stClass);
        break;
    case 4030:        //    获取主机编号
        snprintf(szMsg, sizeof(szMsg), "%05d%0*d", 6, 6, lGenClassIdx());
        break;
    default:
        SYSError(RET_SYSTEM, "系统不支持");
        return RC_FAIL;
    }

    if(RC_SUCC != lRet)
    {
        SYSError(RET_SYSTEM, sGetTError(pstSavm->m_lErrno));
        return lRet;
    }
    else
    {
        vSocketReturn(skSock, szMsg);
        return RC_SUCC;
    }
}

/**************************************************************************************************
    函 数 名：lServiceUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
    功能描述：同步服务配置信息
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lServiceUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
{
    CtsService    stService;
    long        lRet = RC_SUCC;
    char        szMsg[128] = {0};

    memset(&stService, 0, sizeof(CtsService));
    vGetServiceCfg(pszBuffer + 5, &stService);

    switch (lTxCode)
    {
    case 5000:
        lRet = lUpdateService(pstSavm, &stService);
        break;
    case 5010:
        lRet = lInsertService(pstSavm, &stService);
        break;
    case 5020:
        lRet = lRemoveService(pstSavm, &stService);
        break;
    case 5030:
        snprintf(szMsg, sizeof(szMsg), "%05d%0*d", 6, 6, lGenServiceIdx());
        break;
    default:
        SYSError(RET_SYSTEM, "系统不支持");
        return RC_FAIL;
    }

    if(RC_SUCC != lRet)
    {
        SYSError(RET_SYSTEM, sGetTError(pstSavm->m_lErrno));
        return lRet;
    }
    else
    {
        vSocketReturn(skSock, szMsg);
        return RC_SUCC;
    }
}

/**************************************************************************************************
    函 数 名：lFlowUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
    功能描述：同步流程配置信息
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lFlowUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
{
    char    szFlow[1024];
    CtsFlow    *pstFlow = NULL;
    long    i, lRet = RC_SUCC, lCount = 0;

    memset(szFlow, 0, sizeof(szFlow));
    if(0 == (lCount = lGetValidNum(pszBuffer + 5, "\n")))
    {
        SYSError(RET_SYSTEM, "无有效流程配置");
        return RC_FAIL;
    }

    if (!(pstFlow = (CtsFlow *)calloc(lCount, sizeof(CtsFlow))))
    {
        SYSError(RET_SYSTEM, "系统故障");
        return RC_FAIL;
    }

    for (i = 1; i <= lCount; i++)
    {
        strncpy(szFlow, sGetValueByIdx(pszBuffer + 5, "\n", i), sizeof(szFlow) - 1);
        vGetFlowCfg(szFlow, &pstFlow[i - 1]);
    }

    switch (lTxCode)
    {
    case 6000:
        lRet = lUpdateFlow(pstSavm, pstFlow, lCount);
        break;
    case 6010:
        lRet = lInsertFlow(pstSavm, pstFlow, lCount);
        break;
    //    流程删除在服务中处理
    default:
        SYSError(RET_SYSTEM, "系统不支持");
        free(pstFlow);
        return RC_FAIL;
    }
    IBPFree(pstFlow);

    if(RC_SUCC != lRet)
    {
        SYSError(RET_SYSTEM, sGetTError(pstSavm->m_lErrno));
        return lRet;
    }
    else
    {
        vSocketReturn(skSock, NULL);
        return RC_SUCC;
    }
}

/**************************************************************************************************
    函 数 名：lPackUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
    功能描述：同步流程配置信息
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lPackUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
{
    CtsPack     *pstPack = NULL;
    long        i, lRet = RC_SUCC, lCount = 0;
    char        szPack[1024], szMsg[128] = {0};

    if(9030 == lTxCode)
    {
        snprintf(szMsg, sizeof(szMsg), "%05d%0*d", 6, 6, lGenPackIdx());
        vSocketReturn(skSock, szMsg);
        return RC_SUCC;
    }

    memset(szPack, 0, sizeof(szPack));
    if(0 == (lCount = lGetValidNum(pszBuffer + 5, "\n")))
    {
        SYSError(RET_SYSTEM, "无有效报文配置");
        return RC_FAIL;
    }
    
    if (!(pstPack = (CtsPack *)calloc(lCount, sizeof(CtsPack))))
    {
        SYSError(RET_SYSTEM, "系统故障");
        return RC_FAIL;
    }
    
    for (i = 1; i <= lCount; i++)
    {
        strncpy(szPack, sGetValueByIdx(pszBuffer + 5, "\n", i), sizeof(szPack) - 1);
        vGetPackCfg(szPack, &pstPack[i - 1]);
    }

    switch (lTxCode)
    {
    case 9000:
        lRet = lUpdatePack(pstSavm, pstPack, lCount);
        break;
    case 9010:
        lRet = lInsertPack(pstSavm, pstPack, lCount);
        break;
    case 9020:
        lRet = lDeletePack(pstSavm, pstPack[0].m_lMid);
        break;
    default:
        IBPFree(pstPack);
        SYSError(RET_SYSTEM, "系统不支持");
        return RC_FAIL;
    }
    lRefreshPack(pstPack[0].m_lMid);
    IBPFree(pstPack);

    if(RC_SUCC != lRet)
    {
        SYSError(RET_SYSTEM, "%d, %s", lTxCode, sGetTError(pstSavm->m_lErrno));
        return lRet;
    }
    else
    {
        vSocketReturn(skSock, szMsg);
        return RC_SUCC;
    }
}

/**************************************************************************************************
    函 数 名：lMapCodeUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
    功能描述：同步交易码配置信息
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lMapCodeUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
{   
    CtsMapCode  stMCode;
	char		szMsg[128];
    long        lRet = RC_SUCC;

    memset(szMsg, 0, sizeof(szMsg));
    memset(&stMCode, 0, sizeof(CtsMapCode));
    vGetMapCodeCfg(pszBuffer + 5, &stMCode);

    switch (lTxCode)
    {
        case 9100:
            lRet = lInsertMapCode(pstSavm, &stMCode);
            break;
        case 9110:
            lRet = lUpdateMapCode(pstSavm, &stMCode);
            break;
        case 9120:
            lRet = lDeleteMapCode(pstSavm, &stMCode);
            break;
        case 9130:
        	snprintf(szMsg, sizeof(szMsg), "%05d%0*d", 6, 6, lGenMapCodeIdx());
            break;
        default:
            SYSError(RET_SYSTEM, "系统不支持");
            return RC_FAIL;
    }

    if(RC_SUCC != lRet)
    {
        SYSError(RET_SYSTEM, sGetTError(pstSavm->m_lErrno));
        return lRet;
    }
    else
    {
        vSocketReturn(skSock, szMsg);
        return RC_SUCC;
    }
}

/**************************************************************************************************
    函 数 名：lTrCodeUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
    功能描述：同步交易码配置信息
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lTrCodeUpLoad(BSock skSock, long lTxCode, char *pszBuffer, SATvm *pstSavm)
{
    long    lRet = RC_SUCC;
    char    szMsg[128] = {0};
    CtsTrCode stTrCode;

    memset(&stTrCode, 0, sizeof(CtsTrCode));
    vGetTrCodeCfg(pszBuffer + 5, &stTrCode);

    switch (lTxCode)
    {
        case 7000:
            lRet = lUpdateTrCode(pstSavm, &stTrCode);
            break;
        case 7010:
            lRet = lInsertTrCode(pstSavm, &stTrCode);
            break;
        case 7020:
            lRet = lDeleteTrCode(pstSavm, &stTrCode);
            break;
        case 7030:        //    获取主机编号
            snprintf(szMsg, sizeof(szMsg), "%05d%0*d", 6, 6, lGenTrCodeIdx());
            break;
        default:
            SYSError(RET_SYSTEM, "系统不支持");
            return RC_FAIL;
    }

    if(RC_SUCC != lRet)
    {
        SYSError(RET_SYSTEM, sGetTError(pstSavm->m_lErrno));
        return lRet;
    }
    else
    {
        vSocketReturn(skSock, szMsg);
        return RC_SUCC;
    }
}

/**************************************************************************************************
    函 数 名：lDebugMethod(BSock skSock, char *pszBuffer)
    功能描述：调试交易
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lDebugMethod(BSock skSock, char *pszBuffer)
{
    FILE    *fp = NULL;
    long    lRec = 0, i = 0;
    char    *pvData = NULL;
	HHCMIE  ce = pMallocCmie();
    char    szTemp[1024], szLog[512], szFlag[TARGET_NAME_LEN];    //    20K日志
    char    szPGName[PROCESS_NAME_LEN], szClass[FILED_INDEX_LEN], szMethod[FILED_INDEX_LEN];
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

	if(!ce)	return RC_FAIL;

    memset(szLog, 0, sizeof(szLog));
    memset(szTemp, 0, sizeof(szTemp));
    memset(szClass, 0, sizeof(szClass));
    memset(szPGName, 0, sizeof(szPGName));
    memset(szMethod, 0, sizeof(szMethod));

    sDropCharset(pszBuffer, '\r');
    sDropCharset(pszBuffer, '\"');
    lRec = lGetValidNum(pszBuffer, "\n");
    strcpy(szTemp, sGetValueByIdx(pszBuffer, "\n", ++ i));
    strncpy(szPGName, sGetValueByIdx(szTemp, ";", 1), sizeof(szPGName) - 1);
    strncpy(szClass, sGetValueByIdx(szTemp, ";", 2), sizeof(szClass) - 1);
    strncpy(szMethod, sGetValueByIdx(szTemp, ";", 3), sizeof(szMethod) - 1);

    IBTresetpool(ce);
    for(i; i < lRec; i ++)
    {
        memset(szTemp, 0, sizeof(szTemp));
        memset(szFlag, 0, sizeof(szFlag));
        strncpy(szTemp, sGetValueByIdx(pszBuffer, "\n", i + 1), sizeof(szTemp) - 1);
        strncpy(szFlag, sGetValueByIdx(szTemp, "=", 1), sizeof(szFlag) - 1);
        if(!strlen(szFlag))    continue;
        IBTputstring(ce, szFlag, sGetValueByIdx(szTemp, "=", 2));
    }

    vLogSetPath();
    vLogSetName(ce, "%s/.%d.log", sLogGetPath(), lCreateSeqNo(pstSavm));
    if(RC_SUCC != lTraceMethod(pstSavm, ce, szPGName, szClass, szMethod))
    {
		IBPFree(ce);
        unlink(szLog);    //    清理日志
        SYSError(RET_SYSTEM, "单步测试(%s)组件(%s)(%s)失败", szPGName, szClass, szMethod);
        return RC_FAIL;
    }
    if(IBPstatok(lGmtStatus(ce)))
        IBPrace("组建(%s)(%s)处理成功!", szClass, szMethod);
    else
        IBPrace("组建(%s)(%s)处理失败!", szClass, szMethod);
	IBPFree(ce);

    //    等待该笔交易处理完成
    if(RC_FAIL == (lRec = lGetFileSize(szLog)))
        return RC_FAIL;

    if(NULL == (pvData = (char *)malloc(lRec + 1)))
    {
        IBPerror("进程%s初始化失败!", sGetProgram());
        return RC_FAIL;
    }
    memset(pvData, 0, lRec);
    
    if(NULL == (fp = fopen(szLog, "r")))
    {
        unlink(szLog);    //    清理日志
        IBPFree(pvData);
        SYSError(RET_SYSTEM, "获取处理文件失败, err:%s", strerror(errno));
        return RC_FAIL;
    }

    fread(pvData, lRec, 1, fp);
    fclose(fp);
    unlink(szLog);    //    清理日志

    vSocketReturn(skSock, pvData);
    IBPFree(pvData);
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lDebugTrade(BSock skSock, char *pszBuffer)
    功能描述：调试交易
    返回说明：
        RC_SUCC             --成功
           RC_FAIL             --失败
 **************************************************************************************************/
long    lDebugTrade(BSock skSock, char *pszBuffer)
{
    FILE    *fp = NULL;
    char    *pvData = NULL;
    char    szTemp[1024], szLog[256];
    long    i = 0, lRec = 0, lSeqNo = 0;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();
    GCts    *pstCts = (GCts *)pGetGloble();
    char    szTrCode[MAX_INDEX_LEN], szFlag[TARGET_NAME_LEN];

    memset(szLog, 0, sizeof(szLog));
    memset(szTemp, 0, sizeof(szTemp));
    memset(szFlag, 0, sizeof(szFlag));
    memset(szTrCode, 0, sizeof(szTrCode));

    sDropCharset(pszBuffer, '\r');
    sDropCharset(pszBuffer, '\"');
    lRec = lGetValidNum(pszBuffer, "\n");
    strcpy(szTemp, sGetValueByIdx(pszBuffer, "\n", ++ i));
    strncpy(szTrCode, sGetValueByIdx(szTemp, ";", 1), sizeof(szTrCode) - 1);
    snprintf(szLog, sizeof(szLog), "%s", pGetCmie()->m_szLog);

    IBPresetpool();
    for(i; i < lRec; i ++)
    {
        memset(szTemp, 0, sizeof(szTemp));
        memset(szFlag, 0, sizeof(szFlag));
        strncpy(szTemp, sGetValueByIdx(pszBuffer, "\n", i + 1), sizeof(szTemp) - 1);
        strncpy(szFlag, sGetValueByIdx(szTemp, "=", 1), sizeof(szFlag) - 1);
        if(!strlen(szFlag))    continue;
        IBPputstring(szFlag, sGetValueByIdx(szTemp, "=", 2));
    }

    if(RC_FAIL == (lSeqNo = lStartTrade(pstSavm, pGetCmie(), szTrCode)))
    {
        SYSError(RET_SYSTEM, "测试交易(%s)失败", szTrCode);
        return RC_FAIL;
    }

    memset(szFlag, 0, sizeof(szFlag));
    snprintf(szFlag, sizeof(szFlag), "N=%d", lSeqNo);
    if(NULL == (pvData = (char *)malloc(SETUP_MAX_SIZE)))
    {
        IBPerror("进程%s初始化失败!", sGetProgram());
        return RC_FAIL;
    }

    memset(pvData, 0, SETUP_MAX_SIZE);
    if(NULL == (fp = fopen(szLog, "r")))
    {
        IBPFree(pvData);
        SYSError(RET_SYSTEM, "获取处理文件失败, err:%s", strerror(errno));
        return RC_FAIL;
    }

    memset(szTemp, 0, sizeof(szTemp));
    while(fgets(szTemp, sizeof(szTemp), fp))
    {
        if(!strstr(szTemp, szFlag))
            continue;

        strcat(pvData, szTemp);
    }
    fclose(fp);

    //  响应码设置成功，让界面显示调用成功
    vSetRetCode(RET_SUCCESS);
    vSocketReturn(skSock, pvData);
    IBPFree(pvData);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lDebugMethod(BSock skSock, long lTxCode, char *pszBuffer)
    功能描述：流程测试
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDebugTrace(BSock skSock, long lTxCode, char *pszBuffer)
{
    long    lRet = RC_FAIL;

    switch (lTxCode)
    {
        case 8000:
            return lDebugMethod(skSock, pszBuffer + 5);
        case 8001:
            return lDebugTrade(skSock, pszBuffer + 5);
        default:
            break;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lWriteRespond(BSock skSock, char *pszBuffer, int nIn)
    功能描述：处理心跳包和平台业务接入
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
long    lWriteConfig(BSock skSock, char *pszBuffer, int nIn)
{
    char        szTxCode[5];
    long        lRet = RC_SUCC, bUpdate = true;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();
    GCts        *pstCts = (GCts *)pGetGloble();

    memset(szTxCode, 0, sizeof(szTxCode));
    vSetSuccess(pstCts);
    //    登录            1000
    //    上传配置        2000

    memcpy(szTxCode, pszBuffer, 4);
    IBPdebug("请求报文(%s)", pszBuffer);

    switch (atol(szTxCode))
    {
        case 1000:
            bUpdate = false;
            lRet = lLogin(skSock, pszBuffer + 4);
            break;
        case 2000:
            bUpdate = false;
            lRet = lConfigDown(skSock);
            break;
        case 2010:
            bUpdate = false;
            lRet = lConfigSave(skSock);
            break;
        case 3000:
        case 3010:
        case 3020:
        case 3030:
            lRet = lHostUpLoad(skSock, atol(szTxCode), pszBuffer + 4, pstSavm);
            break;
        case 4000:
        case 4010:
        case 4020:
        case 4030:
            lRet = lClassUpLoad(skSock, atol(szTxCode), pszBuffer + 4, pstSavm);
            break;
        case 5000:
        case 5010:
        case 5020:
        case 5030:
            lRet = lServiceUpLoad(skSock, atol(szTxCode), pszBuffer + 4, pstSavm);
            break;
        case 6000:
        case 6010:
        case 6020:
            lRet = lFlowUpLoad(skSock, atol(szTxCode), pszBuffer + 4, pstSavm);
            break;
        case 7000:
        case 7010:
        case 7020:
        case 7030:
            lRet = lTrCodeUpLoad(skSock, atol(szTxCode), pszBuffer + 4, pstSavm);
            break;
        case 8000:
        case 8001:
            lRet = lDebugTrace(skSock, atol(szTxCode), pszBuffer + 4);
            break;
        case 9000:
        case 9010:
        case 9020:
        case 9030:
            lRet = lPackUpLoad(skSock, atol(szTxCode), pszBuffer + 4, pstSavm);
            break;
        case 9100:
        case 9110:
        case 9120:
        case 9130:
            lRet = lMapCodeUpLoad(skSock, atol(szTxCode), pszBuffer + 4, pstSavm);
            break;
        default:
            lRet = RC_FAIL;
            SYSError(RET_SYSTEM, "系统故障");
            break;
    }

    if(RC_SUCC != lRet) 
    {
        vSocketReturn(skSock, NULL);
        return true;
    }

    if(bUpdate)        lNotifyPack(CTS_MSGEX, DEFAUT_HOST);

    return true;
}

/**************************************************************************************************
    函 数 名：lManageConf(SATvm *pstSavm, void *arg)
    功能描述：接收Socket报文
    返回说明：
        len                    --返回从socket读取的字节数
 **************************************************************************************************/
long    lManageConf(SATvm *pstSavm, void *arg)
{
    SKCon   *pstCon = (SKCon *)arg;
    long    lLen = 0, i = 0, lRecv, lTotal = 6;
    static    char    szBuffer[MAX_LOGBUFF_LEN] = {0};

    vLogSetPath();
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), sGetProgram());
    lTotal = lRecvBuffer(pstCon->m_skSock, szBuffer, lTotal);
    if(RC_CLOSE == lTotal)
        return RC_CLOSE;
    else if(6 != lTotal)
    {
        SYSError(RET_SYSTEM, "接收请求报文长度异常, err:%d", errno);
        vSocketReturn(pstCon->m_skSock, NULL);
        return RC_CLOSE;
    }
    szBuffer[lTotal] = 0x00;

    //    默认超时时间30s
    for(i = 0, lLen = atol(szBuffer), lTotal = 0; (i < 30 * 20) && (lTotal < lLen); 
        i ++, usleep(50000))
    {
        if(0 > (lRecv = lRecvBuffer(pstCon->m_skSock, szBuffer + lTotal, lLen - lTotal)))
        {
            //  表示没数据了
            if(EAGAIN == errno || EWOULDBLOCK == errno)
                continue;
            else
                break;
        }
        lTotal += lRecv;
    }

    if(lLen != lTotal)
    {
        SYSError(RET_SYSTEM, "请求长度与数据不符");
        vSocketReturn(pstCon->m_skSock, NULL);
        return RC_FAIL;
    }
    szBuffer[lTotal] = 0x00;

    return lWriteConfig(pstCon->m_skSock, szBuffer, lTotal);
}

/**************************************************************************************************
    函 数 名：vGetRespond(void *args)
    功能描述：提高TCP接入并发接入API
    返回说明：
        RC_SUCC         --成功
           RC_FAIL         --失败
 **************************************************************************************************/
void*   vGetRespond(void *args)
{
    CtsProcess  stProcess;
    SATvm       *pstSavm = (SATvm *)calloc(1, sizeof(SATvm));

    pthread_detach(pthread_self());
    if(!pstSavm)
    {
        IBPerror("初始化线程参数失败, err:%s", strerror(errno));
        return ;
    }

    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", sGetProgram());
        return ;
    }

    while (1)
    {
        lOutGetRequest(pstSavm, &stProcess, pGetCmie());
        if(IBPevcache(pGetEvent()))
            continue;
    }

     pthread_exit(NULL);
}

/**************************************************************************************************
    函 数 名：lServerInit()
    功能描述：初始化子进程处理
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
long    lServerInit(SATvm *pstSavm, void *arg)
{
    pthread_t   tPid = 0;

    if(RC_SUCC != lRegisterProcess(sGetProgram(), DEFAUT_HOST))
    {   
        fprintf(stderr, "登记任务(%s)的进程失败!\n", sGetProgram());
        exit(RC_FAIL);
    }

    if(NULL == (g_pszWrite = (char *)calloc(SETUP_MAX_SIZE, sizeof(char))))
    {
        IBPerror("进程%s初始化失败!", sGetProgram());
        return RC_FAIL;
    }

    vLogSetPath();
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), sGetProgram());
    IBPrace("进程%s启动，当前版本：%s ...", sGetProgram(), g_szVersion);

    fprintf(stdout, "child %s process id=%d ... success\n", sGetProgram(), getpid());
    fflush(stdout);

    if (0 != pthread_create(&tPid, NULL, vGetRespond, NULL))
    {
        fprintf(stderr, "pthread_create error, errstr:%s.\n", strerror(errno));
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
int        main(int argc, char *argv[])
{
    CtsProcess    stProcess;
    long        lPort, lExec = 0;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));
    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", basename(argv[0]));
        return RC_FAIL;
    }

    //    父进程退出注销
    lUnregisterProcess(&stProcess);

    lPort = atol(sGetDbsEnv("MAN_TPORT"));
    if(lPort < 100)    lPort = 31000;

    lExec = atol(sGetDbsEnv("MAN_EXEC"));
    if(lExec < 0 || lExec > 10)    lExec = 1;

    if(RC_SUCC != lMultListen(pstSavm, lPort, lExec, lServerInit, NULL, lManageConf))
    {
        fprintf(stderr, "boot %s child(%d) failed, port:%d, err:%s\n", argv[0], lPort, lExec,
            sGetTError(pstSavm->m_lErrno));
        exit(-1);
    }

    return RC_SUCC;
}

/*************************************************************************************************
 *     code end
 **************************************************************************************************/

