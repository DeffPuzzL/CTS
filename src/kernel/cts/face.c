/*************************************************************************************************
    文 件 名：face.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：系统提供对外API
 *************************************************************************************************/
#include    "face.h"


long    lRemoveClass(SATvm *pstSavm, CtsClass *pstClass);
/**************************************************************************************************
    函 数 名：vDebugCts(GCts *pstCts)
    功能描述：调试全局共享信息
    返回说明：
        无
 **************************************************************************************************/
void    vDebugCts(GCts *pstCts)
{
    IBPrace("---------------------------------------------");
    IBPrace("m_lSeqNo=[%d]", pstCts->m_lSeqNo);
    IBPrace("m_lInHost=[%d]", pstCts->m_lInHost);
    IBPrace("m_lInPid=[%d]", pstCts->m_lInPid);
    IBPrace("m_lInMid=[%d]", pstCts->m_lInMid);
    IBPrace("m_lOutMid=[%d]", pstCts->m_lOutMid);
    IBPrace("m_lErrMid=[%d]", pstCts->m_lErrMid);
    IBPrace("m_lInTime=[%d]", pstCts->m_lInTime);
    IBPrace("m_lTimeOut=[%d]", pstCts->m_lTimeOut);
    IBPrace("m_lUseTime=[%d]", pstCts->m_lUseTime);
    IBPrace("m_szInCode=[%s]", pstCts->m_szInCode);
    IBPrace("m_szInSCode=[%s]", pstCts->m_szInSCode);
    IBPrace("m_szTrCode=[%s]", pstCts->m_szTrCode);
    IBPrace("m_szCallCode=[%s]", pstCts->m_szCallCode);
    IBPrace("m_szCallSub=[%s]", pstCts->m_szCallSub);
    IBPrace("m_szSvrName=[%s]", pstCts->m_szSvrName);
    IBPrace("m_lLogLev=[%d]", pstCts->m_lLogLev);
    IBPrace("m_lSType=[%d]", pstCts->m_lSType);
    IBPrace("m_lPid=[%d]", pstCts->m_lPid);
    IBPrace("m_szLogName=[%s]", pstCts->m_szLogName);
    IBPrace("m_lHost=[%d]", pstCts->m_lHost);
    IBPrace("m_lSvrID=[%d]", pstCts->m_lSvrID);
    IBPrace("m_szTxDate=[%s]", pstCts->m_szTxDate);
    IBPrace("m_szCrDate=[%s]", pstCts->m_szCrDate);
    IBPrace("m_szTxTime=[%s]", pstCts->m_szTxTime);
    IBPrace("m_lState=[%d]", pstCts->m_lState);
    IBPrace("m_szRetCode=[%s]", pstCts->m_szRetCode);
    IBPrace("m_szRetMsg=[%s]", pstCts->m_szRetMsg);
    IBPrace("m_lLastState=[%d]", pstCts->m_lLastState);
    IBPrace("m_szLastRet=[%s]", pstCts->m_szLastRet);
    IBPrace("m_szLastMsg=[%s]", pstCts->m_szLastMsg);
    IBPrace("=================================================================");
}

/**************************************************************************************************
    函 数 名：pGetParam()
    功能描述：获取系统参数信息
    返回说明：
        无
 **************************************************************************************************/
CtsParam*    pGetParam()
{
    CtsParam  stParam;
	void	  *pvData = NULL;
    SATvm     *pstSavm = (SATvm *)pGetSATvm();

    memset(&stParam, 0, sizeof(stParam));
    strncpy(stParam.m_szSysName, SYSTEM_NAME, sizeof(stParam.m_szSysName));

    pstSavm->pstVoid = &stParam;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_PARAM;
    pstSavm->lSize   = sizeof(CtsParam);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
        return NULL;

    if(RC_SUCC != lNextFetch(pstSavm, CTS_PLAT_PARAM, (void **)&pvData))
	{
		vTableClose(pstSavm);
        return NULL;
    }

	return (CtsParam *)pvSetVTData(pvData);
}

/**************************************************************************************************
    函 数 名：bIsSysNormal(SATvm *pstSavm)
    功能描述：获取系统参数信息
    返回说明：
        无
 **************************************************************************************************/
BOOL    bIsSysNormal(SATvm *pstSavm)
{
	CtsParam	*pstPara = (CtsParam *)pGetVTData();

	if(!pstPara)
	{
		if(NULL == (pstPara = (CtsParam *)pGetParam()))
	    	return false;
	}

	if(SYSTEM_PLT_START == pstPara->m_lState)
		return true;

    return false;
}

/**************************************************************************************************
    函 数 名：lGetSysParam(SATvm *pstSavm, CtsParam *pstParam, char *pszName)
    功能描述：获取系统参数信息
    返回说明：
        无
 **************************************************************************************************/
long    lGetSysParam(SATvm *pstSavm, CtsParam *pstParam)
{
	CtsParam  *pstPara = (CtsParam *)pGetVTData();

	if(!pstPara)
	{
		if(NULL == (pstPara = (CtsParam *)pGetParam()))
	    	return RC_FAIL;
	}

	memcpy(pstParam, pstPara, sizeof(CtsParam));

	return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lSetSysState(long eType)
    功能描述：设置CTS平台状态
    返回说明：
        无
 **************************************************************************************************/
long    lSetSysState(SATvm *pstSavm, long eType)
{
	CtsParam  *pstPara = (CtsParam *)pGetVTData();

	if(!pstPara)
	{
		if(NULL == (pstPara = (CtsParam *)pGetParam()))
	    	return RC_FAIL;
	}

	pstPara->m_lState = eType;

	return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lSetSysSeqNo(SATvm *pstSavm, long lSeqNo)
    功能描述：设置平台流水号
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long	lSyncParam(SATvm *pstSavm, CtsParam *pstParam)
{
    CtsParam  stParam;
    RunTime   *pstRun = NULL;
	void	  *pvData = NULL;

    memset(&stParam, 0, sizeof(stParam));
    strncpy(stParam.m_szSysName, SYSTEM_NAME, sizeof(stParam.m_szSysName));

    pstSavm->pstVoid = &stParam;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_PARAM;
    pstSavm->lSize   = sizeof(CtsParam);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
        return RC_FAIL;

    if(RC_SUCC != lNextFetch(pstSavm, CTS_PLAT_PARAM, (void **)&pvData))
	{
		vTableClose(pstSavm);
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    ((CtsParam *)pvData)->m_lSeqNo = ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ;
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

	memcpy(pstParam, pvData, sizeof(CtsParam));
    vTblDisconnect(pstSavm, CTS_PLAT_PARAM);

	return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lSetSysSeqNo(SATvm *pstSavm, long lSeqNo)
    功能描述：设置平台流水号
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lSetSysSeqNo(SATvm *pstSavm, llSEQ lSeqNo)
{
    RunTime     *pstRun = NULL;
	CtsParam    *pstPara = (CtsParam *)pGetVTData();

	if(!pstPara)
	{
		if(NULL == (pstPara = (CtsParam *)pGetParam()))
	    	return RC_FAIL;
	}

	pstPara->m_lSeqNo = lSeqNo > 0 ? lSeqNo : 1;
    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ = pstPara->m_lSeqNo;
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lSetSysCrDate(SATvm *pstSavm, char* pszCrDate)
    功能描述：设置平台清算日期
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lSetSysCrDate(SATvm *pstSavm, char *pszCrDate)
{
    RunTime     *pstRun = NULL;
	CtsParam    *pstPara = (CtsParam *)pGetVTData();

	if(!pstPara)
	{
		if(NULL == (pstPara = (CtsParam *)pGetParam()))
	    	return RC_FAIL;
	}

	strncpy(pstPara->m_szCrDate, pszCrDate, 8);
    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    memcpy(((TblDef *)pstRun->m_pvAddr)->m_szPart, pstPara->m_szCrDate, 8); 
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

	return lUpdIndexPart(pstSavm, CTS_RUNTIME_POOL, pstPara->m_szCrDate);
}

/**************************************************************************************************
    函 数 名：lSetSysTxDate(SATvm *pstSavm, char* pszTxDate)
    功能描述：测试时候内存加载关联系统日期
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lSetSysTxDate(SATvm *pstSavm, char *pszTxDate)
{
    RunTime     *pstRun = NULL;
	CtsParam    *pstPara = (CtsParam *)pGetVTData();

	if(!pstPara)
	{
		if(NULL == (pstPara = (CtsParam *)pGetParam()))
	    	return RC_FAIL;
	}

	strncpy(pstPara->m_szTxDate, pszTxDate, 8);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lDayendParam(SATvm *pstSavm, CtsParam *pstParam)
    功能描述：系统相关配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lDayendParam(SATvm *pstSavm, CtsParam *pstParam)
{
    CtsParam    stParam;
    RunTime     *pstRun = NULL;

    if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
        return RC_FAIL;
    
    pstSavm->pstVoid = &stParam;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_PARAM;
    pstSavm->lSize   = sizeof(CtsParam);
    if(RC_SUCC != lUpdate(pstSavm, pstParam, false))
        return RC_FAIL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ = pstParam->m_lSeqNo;
    memcpy(((TblDef *)pstRun->m_pvAddr)->m_szPart, pstParam->m_szCrDate, 8); 
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

	return lUpdIndexPart(pstSavm, CTS_RUNTIME_POOL, pstParam->m_szCrDate);
}

/**************************************************************************************************
    函 数 名：lInitRunTest(char *pszPGName)
    功能描述：初始化进程启动参数
    返回说明：
        无
 **************************************************************************************************/
long    lInitRunTest(char *pszPGName)
{
    long        lRet = 0;
    CtsParam    stParam;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    sigset(SIGUSR1, vExitAbnormal);
    sigset(SIGTERM, vExitAbnormal);
//    sigset(SIGSEGV, vExitAbnormal);

    srand(time(NULL));
    vLogSetPath();
    vLogSetSize(0);
    vSetSysLevel(pGetCmie(), LOG_TRACE);    
    vSetProgram("");
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), pszPGName);
    if(RC_SUCC != lInitAllSatvm(pstSavm))
    {
        IBPerror("初始化TVM各表信息失败");
        return RC_FAIL;
    }

    for(;;usleep(50000))
    {
        memset(&stParam, 0, sizeof(stParam));
        if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
        {
            IBPerror("获取系统配置信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        if(SYSTEM_PLT_BING != stParam.m_lState)     //  非平台启动中
            break;
    }
	vSetSysMaster(stParam.m_lMaster);
    vSetSysLevel(pGetCmie(), stParam.m_lLogLevel);        //    设置系统默认日志级别
    vSetUserSize(stParam.m_lUserSize);    //    启动时初始化共享池大小
    vSetProgram(pszPGName);

    vSetToPos(0);
    vSetToNum(0);
    vSetInQueue(0);
    vSetToQueue(0);

    lRet = lGetCmdReturn("ipcs -l|grep \"max size of queue\"|awk -F '=' '{print $2}'");
    if(RC_FAIL >= lRet)
    {
        IBPerror("获取系统队列最大字节失败, err:%s", strerror(errno));
        return RC_FAIL;
    }

    vSetMaxQueue(lRet / sizeof(CtsEvent));

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lInitRunDemo(long lPid, CtsProcess *pstProcess, char *pszPGName)
    功能描述：初始化进程启动参数
    返回说明：
        无
 **************************************************************************************************/
long    _lInitRunDemo(SATvm *pstSavm, long lPid, CtsProcess *pstProcess, char *pszPGName)
{
    CtsParam    stParam;
    CtsGroup    stGroup;
    long        lQueue = 0;

    sigset(SIGUSR1, vExitAbnormal);
    sigset(SIGTERM, vExitAbnormal);
//    sigset(SIGSEGV, vExitAbnormal);

    if(!pstSavm)
    {
        IBPerror("待初始化TVM句柄不能为空");
        return RC_FAIL;
    }

    srand(time(NULL));
    vLogSetPath();
    vLogSetSize(0);
    vSetSysLevel(pGetCmie(), LOG_TRACE);    
    vSetProgram(pstProcess->m_szPGName);
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);
    if(RC_SUCC != lInitAllSatvm(pstSavm))
    {
        IBPerror("初始化TVM各表信息失败");
        return RC_FAIL;
    }

    for(;;usleep(50000))
    {
        if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
        {
            IBPerror("获取系统配置信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        if(SYSTEM_PLT_BING != stParam.m_lState)        //    非平台启动中
            break;
    }
    vSetSysMaster(stParam.m_lMaster); 
    vSetSysLevel(pGetCmie(), stParam.m_lLogLevel);        //    设置系统默认日志级别

    if(RC_SUCC != lGetProcess(pstSavm, pstProcess, lPid))
    {
        IBPerror("获取进程(%d)运行参数信息失败, err:%s", lPid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    vSetProgram(pstProcess->m_szPGName);
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);
    for(;;usleep(50000))
    {
        //    获取事件来源配置信息
        memset(&stGroup, 0, sizeof(stGroup));
        if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pstProcess->m_szPGName, pstProcess->m_lHost))
        {
            IBPerror("获取进程(%s)配置信息失败, err:%s", pstProcess->m_szPGName, 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        if(STATUES_ENABLE == stGroup.m_lState)
            break;
    }
    if(stGroup.m_lQid <= 0)
    {
        IBPerror("进程(%d)(%s)对应配置信息异常", pstProcess->m_lPid, pstProcess->m_szPGName);
        return RC_FAIL;
    }
    vSetInQueue(stGroup.m_lQid);

    //    获取推送事件配置信息
    memset(&stGroup, 0, sizeof(stGroup));
    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pszPGName, DEFAUT_HOST))
    {
        IBPerror("获取进程(%s)配置信息失败, err:%s", pszPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    if(stGroup.m_lQid <= 0)
    {
        IBPerror("获取进程(%s)对应的业务进程(%s)配置信息异常, err:%s", pstProcess->m_szPGName, 
            pszPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    vSetToPos(stGroup.m_lAppPos);
    vSetToNum(stGroup.m_lMaxNum);
    vSetToQueue(stGroup.m_lQid);

    //    设置推送队列的最大个数
    if(RC_FAIL >= (lQueue = lQueueMaxByte(pstSavm, stGroup.m_lQid)))
    {
        IBPerror("获取队列(%d)最大字节失败, err:%s", stGroup.m_lQid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    
    //    预留5个，给进程缓冲
    lQueue = lQueue / (sizeof(CtsEvent) - sizeof(int)) - 5;
    vSetMaxQueue(lQueue);
    vSetUserSize(stParam.m_lUserSize);    //    启动时初始化共享池大小

    //    其他参数初始化, 文件句柄检查
    vSetMaxPid(lGetCmdReturn("cat /proc/sys/kernel/pid_max"));

    if(!strcmp(CTS_CORE, pstProcess->m_szPGName) || !strcmp(CTS_MON, pstProcess->m_szPGName) ||
        !strcmp(CTS_MANAGE, pstProcess->m_szPGName) || !strcmp(CTS_ALARM, pstProcess->m_szPGName))
        return RC_SUCC;
    else if(!strcmp(CTS_MSGEX, pstProcess->m_szPGName))
        return lInitCachePack(pstSavm);
    else
        return lInitClassPack(pstSavm, pstProcess);
}

/**************************************************************************************************
    函 数 名：lEventCache(CtsProcess *pstProcess)
    功能描述：事件发起刷新
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lEventCache(SATvm *pstSavm, CtsProcess *pstProcess)
{
    vLogSetPath();
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);
    if(RC_SUCC != lInitAllSatvm(pstSavm))
        return RC_FAIL;

    if(!strcmp(CTS_CORE, pstProcess->m_szPGName) || !strcmp(CTS_MON, pstProcess->m_szPGName) ||
        !strcmp(CTS_MANAGE, pstProcess->m_szPGName) || !strcmp(CTS_ALARM, pstProcess->m_szPGName))
        return RC_SUCC;
    else if(!strcmp(CTS_MSGEX, pstProcess->m_szPGName))
        return lInitCachePack(pstSavm);
    else
        return lInitClassPack(pstSavm, pstProcess);
}

/**************************************************************************************************
    函 数 名：lInitCoreDemo(long lPid, CtsProcess *pstProcess)
    功能描述：核心进程初始化
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInitCoreDemo(SATvm *pstSavm, long lPid, CtsProcess *pstProcess)
{
    return _lInitRunDemo(pstSavm, lPid, pstProcess, CTS_MSGEX);
}

/**************************************************************************************************
    函 数 名：lInitialDemo(long lPid, CtsProcess *pstProcess)
    功能描述：业务组件进程初始化
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInitialDemo(SATvm *pstSavm, long lPid, CtsProcess *pstProcess)
{
    return _lInitRunDemo(pstSavm, lPid, pstProcess, CTS_CORE);
}

/**************************************************************************************************
    函 数 名：lGetSystem(char *pszTarget)
    功能描述：获取标签对应的参数
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetSystem(char *pszTarget)
{
    long        lRet = 0;
    CtsParam    stParam;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
    {
        IBPerror("获取系统配置信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(!strcmp(pszTarget, "SeqNo"))
        return stParam.m_lSeqNo;
    else if(!strcmp(pszTarget, "State"))
        return stParam.m_lState;
    else if(!strcmp(pszTarget, "TrCount"))
        return stParam.m_lTrCount;
    else if(!strcmp(pszTarget, "SuccCount"))
        return stParam.m_lSuccCount;
    else if(!strcmp(pszTarget, "FailCount"))
        return stParam.m_lFailCount;
    else if(!strcmp(pszTarget, "InsaCount"))
        return stParam.m_lInsaCount;
    else if(!strcmp(pszTarget, "InsaTime"))
        return stParam.m_lInsaTime;
    else if(!strcmp(pszTarget, "PoolSize"))
        return stParam.m_lUserSize;
    else if(!strcmp(pszTarget, "PoolNum"))
        return stParam.m_lPoolNum;
    else if(!strcmp(pszTarget, "ScanTime"))
        return stParam.m_lScanTime;
    else if(!strcmp(pszTarget, "QueueTime"))
        return stParam.m_lQueueTime;
    else if(!strcmp(pszTarget, "TRTime"))
        return stParam.m_lTRTime;
    else if(!strcmp(pszTarget, "MaxServers"))
        return stParam.m_lMaxServers;
    else if(!strcmp(pszTarget, "MaxProcess"))
        return stParam.m_lMaxProcess;
    else if(!strcmp(pszTarget, "MaxAsyn"))
        return stParam.m_lMaxAsyn;
    else if(!strcmp(pszTarget, "LogLevel"))
        return stParam.m_lLogLevel;
    else if(!strcmp(pszTarget, "QPRatio"))
        return stParam.m_lQPRatio;
}

/**************************************************************************************************
    函 数 名：sGetSysActDate()
    功能描述：获取系统账务日期
    返回说明：
        szDate            --日期
 **************************************************************************************************/
char*    sGetSysActDate()
{
    CtsParam        stParam;
    static    char    szDate[9];
    SATvm           *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
    {
        IBPerror("获取系统配置信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return szDate;
    }

    memset(&szDate, 0, sizeof(szDate));
    strncpy(szDate, stParam.m_szCrDate, sizeof(szDate) - 1);

    return szDate;
}

/**************************************************************************************************
    函 数 名：lGetTrCode(SATvm *pstSavm, CtsTrCode *pstCode, char *pszInCode, char *pszInSub)
    功能描述：获取交易配置信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetTrCode(SATvm *pstSavm, CtsTrCode *pstCode, char *pszInCode, char *pszInSub)
{
    CtsTrCode    stCode;

    memset(&stCode, 0, sizeof(CtsTrCode));    
    strncpy(stCode.m_szInCode, pszInCode, sizeof(stCode.m_szInCode));
    strncpy(stCode.m_szInSCode, pszInSub, sizeof(stCode.m_szInSCode));

    pstSavm->tblName = CTS_TRADE_CODE;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsTrCode);
    pstSavm->pstVoid = &stCode;
    return lSelect(pstSavm, pstCode);
}

/**************************************************************************************************
    函 数 名：lGetHostClass(SATvm *pstSavm, CtsClass *pstClass, char *pszInCode, char *pszInSub)
    功能描述：获取组件配置信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetHostClass(SATvm *pstSavm, long lHost, CtsClass **ppstClass, size_t *plOut)
{
    CtsClass    stClass;

    memset(&stClass, 0, sizeof(CtsClass));
    stClass.m_lHost = lSetEXLong(lHost);

    pstSavm->pstVoid = &stClass;
    pstSavm->tblName = CTS_TRADE_CLASS;
    pstSavm->lSize   = sizeof(CtsClass);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    return lQuery(pstSavm, plOut, (void **)ppstClass);
}

/**************************************************************************************************
    函 数 名：lGetClass(SATvm *pstSavm, CtsClass *pstClass, char *pszInCode, char *pszInSub)
    功能描述：获取组件配置信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetClass(SATvm *pstSavm, CtsClass *pstClass, char *pszClass, char *pszMethod)
{
    CtsClass    stClass;
    
    memset(&stClass, 0, sizeof(CtsClass));
    strncpy(stClass.m_szClass,  pszClass, sizeof(stClass.m_szClass));
    strncpy(stClass.m_szMethod, pszMethod, sizeof(stClass.m_szMethod));

    pstSavm->pstVoid = &stClass;
    pstSavm->tblName = CTS_TRADE_CLASS;
    pstSavm->lSize   = sizeof(CtsClass);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    return lSelect(pstSavm, pstClass);
}

/**************************************************************************************************
    函 数 名：lGetClassByIdx(SATvm *pstSavm, CtsClass *pstClass, long lBuss)
    功能描述：根据组件IDX获取配置信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetClassByIdx(SATvm *pstSavm, CtsClass *pstClass, long lBuss)
{
    CtsClass    stClass;

    memset(&stClass, 0, sizeof(stClass));
    stClass.m_lBuss = lSetEXLong(lBuss);    

    pstSavm->pstVoid = &stClass;
    pstSavm->tblName = CTS_TRADE_CLASS;
    pstSavm->lSize   = sizeof(CtsClass);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    return lSelect(pstSavm, pstClass);
}

/**************************************************************************************************
    函 数 名：lGetProcess(SATvm *pstSavm, CtsProcess *pstProcess, long lPid)
    功能描述：根据进程PID获取进程组相关信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetProcess(SATvm *pstSavm, CtsProcess *pstProcess, long lPid)
{
    CtsProcess    stProcess;

    memset(&stProcess, 0, sizeof(CtsProcess));
    stProcess.m_lPid = lSetEXLong(lPid);

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->pstVoid = &stProcess;
    pstSavm->tblName = CTS_PLAT_PROCESS;
    pstSavm->lSize   = sizeof(CtsProcess);
    return lSelect(pstSavm, pstProcess);
}

/**************************************************************************************************
    函 数 名：lGetPackList(SATvm *pstSavm, long lMid, CtsPack **ppstPack, long *plOut)
    功能描述：获取平台报文编号对应的报文配置信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetPackList(SATvm *pstSavm, long lMid, CtsPack **ppstPack, size_t *plOut)
{
    CtsPack        stPack;

    memset(&stPack, 0, sizeof(CtsPack));
    stPack.m_lMid = lSetEXLong(lMid);

    pstSavm->tblName = CTS_PACK_CONFIG;
    pstSavm->lFind   = CHK_SELECT;
    pstSavm->lSize   = sizeof(CtsPack);
    pstSavm->pstVoid = &stPack;
     return lQuery(pstSavm, plOut, (void **)ppstPack);
}

/**************************************************************************************************
    函 数 名：lGetSvrFlow(SATvm *pstSavm, long lSvrId, CtsFlow **ppstFlow, long *plOut)
    功能描述：获取平台所有交易流水信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetSvrFlow(SATvm *pstSavm, long lSvrID, CtsFlow **ppstFlow, size_t *plOut)
{
    CtsFlow    stFlow;
    
    memset(&stFlow, 0, sizeof(CtsFlow));
    stFlow.m_lSvrID = lSetEXLong(lSvrID);

    pstSavm->tblName = CTS_TRADE_FLOW;
    pstSavm->lFind   = CHK_SELECT;
    pstSavm->lSize   = sizeof(CtsFlow);
    pstSavm->pstVoid = &stFlow;
    return lQuery(pstSavm, plOut, (void **)ppstFlow);
}

/**************************************************************************************************
    函 数 名：lGetGroup(SATvm *pstSavm, CtsGroup *pstGroup, char *pszProcess, long lHost)
    功能描述：获取平台指定进程组信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetGroup(SATvm *pstSavm, CtsGroup *pstGroup, char *pszProcess, long lHost)
{
    CtsGroup    stGroup;

    memset(&stGroup, 0, sizeof(CtsGroup));
    stGroup.m_lHost = lHost;
    strncpy(stGroup.m_szPGName, pszProcess, sizeof(stGroup.m_szPGName));

    pstSavm->tblName = CTS_PLAT_GROUP;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsGroup);
    pstSavm->pstVoid = &stGroup;
    return lSelect(pstSavm, pstGroup);
}

/**************************************************************************************************
    函 数 名：lGetHost(SATvm *pstSavm, CtsHost *pstHost, long lHost)
    功能描述：根据主机编号获取主机信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetHost(SATvm *pstSavm, CtsHost *pstHost, long lHost)
{
    CtsHost        stHost;

    memset(&stHost, 0, sizeof(stHost));
    stHost.m_lHost = lSetEXLong(lHost);

    pstSavm->tblName = CTS_TRADE_HOST;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsHost);
    pstSavm->pstVoid = &stHost;
    return lSelect(pstSavm, pstHost);
}

/**************************************************************************************************
    函 数 名：lGetService(SATvm *pstSavm, CtsService *pstService, char *pszTrCode)
    功能描述：获取系统服务信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetService(SATvm *pstSavm, CtsService *pstService, char *pszTrCode)
{
    CtsService    stService;

    memset(&stService, 0, sizeof(stService));
    strncpy(stService.m_szTrCode, pszTrCode, sizeof(stService.m_szTrCode));

    pstSavm->tblName = CTS_TRADE_SERVICE;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsService);
    pstSavm->pstVoid = &stService;
    return lSelect(pstSavm, pstService);
}

/**************************************************************************************************
    函 数 名：lGetHostService(SATvm *pstSavm, long lHost, CtsService **ppstService, long *plOut)
    功能描述：获取组件配置信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetHostService(SATvm *pstSavm, long lHost, CtsService **ppstService, long *plOut)
{
    CtsService  stService;

    memset(&stService, 0, sizeof(CtsService));
    stService.m_lHost = lSetEXLong(lHost);

    pstSavm->tblName = CTS_TRADE_SERVICE;
    pstSavm->lFind   = CHK_SELECT;
    pstSavm->lSize   = sizeof(CtsService);
    pstSavm->pstVoid = &stService;
    return lQuery(pstSavm, plOut, (void **)ppstService);
}

/**************************************************************************************************
    函 数 名：lDisableService(char *pszTrCode)
    功能描述：停止指定服务
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDisableService(SATvm *pstSavm, char *pszTrCode)
{
    CtsService    stService, stUpdate;

    memset(&stUpdate, 0, sizeof(CtsService));
    memset(&stService, 0, sizeof(CtsService));
    stUpdate.m_lState = STATUES_DISABLE;
    strncpy(stService.m_szTrCode, pszTrCode, sizeof(stService.m_szTrCode));

    pstSavm->tblName = CTS_TRADE_SERVICE;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsService);
    pstSavm->pstVoid = &stService;

    return lUpdate(pstSavm, &stUpdate, false);
}

/**************************************************************************************************
    函 数 名：lEnableService(char *pszTrCode)
    功能描述：启动系统某个服务
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lEnableService(SATvm *pstSavm, char *pszTrCode)
{
    CtsService    stService, stUpdate;

    memset(&stUpdate, 0, sizeof(CtsService));
    memset(&stService, 0, sizeof(CtsService));
    stUpdate.m_lState = STATUES_ENABLE;
    strncpy(stService.m_szTrCode, pszTrCode, sizeof(stService.m_szTrCode));

    pstSavm->tblName = CTS_TRADE_SERVICE;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsService);
    pstSavm->pstVoid = &stService;
    return lUpdate(pstSavm, &stUpdate, false);
}

/**************************************************************************************************
    函 数 名：bIsEventValid(SATvm *pstSavm, CtsEvent *pstEvent)
    功能描述：检查队列指向的交易是否有效（有可能交易已被清理，但是队列未并清理)
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
BOOL    bIsEventValid(SATvm *pstSavm, HHCMIE cm)
{
    SHTruck   *pvData = NULL;
    RunTime   *pstRun = NULL;
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    //    进程本身队列直接保留
    if(IBPevexit(pstEvent) || IBPevreboot(pstEvent) || IBPevcache(pstEvent))
        return TRUE;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    pvData = (PSHTruck)pGetNode(pstRun->m_pvAddr, pstEvent->m_uData);
    if(((GCts *)pvData->m_pvData)->m_lSeqNo != pstEvent->m_lSeqNo)
    {
        IBTwarn(cm, "交易流水(%d)已消失, 可能已被清理", pstEvent->m_lSeqNo);
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Resource unavailable");
        return FALSE;
    }

    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

    return TRUE;
}

/**************************************************************************************************
    函 数 名：lSetDrivePack(SATvm *, GCts *, CtsEvent *, void *psvData, long lLen)
    功能描述：上传给核心，调度解析报文
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lSetDrivePack(SATvm *pstSavm, HHCMIE cm, char *pszData, long lLen)
{
    SHTruck   *pvData   = NULL;
    RunTime   *pstRun   = NULL;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(!IBPevoutmsg(pstEvent) && !IBPevinmsg(pstEvent))
    {
        IBTerror(cm, "非报文处理事件驱动");
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    pvData = (PSHTruck)pGetNode(pstRun->m_pvAddr, pstEvent->m_uData);
    if(((GCts *)pvData->m_pvData)->m_lSeqNo != pstEvent->m_lSeqNo)
    {
        IBTwarn(cm, "交易流水(%d)已消失, 可能已被清理", pstEvent->m_lSeqNo);
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Resource unavailable");
        return RC_FAIL;
    }

    pstCts->m_lLen = lLen;
    if(IBPevoutmsg(pstEvent))
        pstCts->m_lStatus = TRADE_POOL_OUTMSG;
    else
        pstCts->m_lStatus = TRADE_POOL_INMSG;
    memcpy(pvData->m_pvData, pstCts, sizeof(GCts));

    if(lLen > 0)
    {
        pszData[lLen ++] = 0x00;
        memcpy(pvData->m_pvData + sizeof(GCts), (void *)pszData, lLen);
    }

    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lGetDrivePack(SATvm *, GCts *, CtsEvent *pstEvent, void *pszData, long *plLen)
    功能描述：上传给核心，调度解析报文, 获取响应报文结果
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetDrivePack(SATvm *pstSavm, HHCMIE cm, char *pszData, long *plLen)
{
	long      lLen;
    SHTruck   *pvData   = NULL;
    RunTime   *pstRun   = NULL;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(!IBPevoutmsg(pstEvent) && !IBPevinmsg(pstEvent) && !IBPevfinish(pstEvent))
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        IBTerror(cm, "非报文处理事件(%d)驱动", pstEvent->m_lState);
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    pvData = (PSHTruck)pGetNode(pstRun->m_pvAddr, pstEvent->m_uData);
    if(((GCts *)pvData->m_pvData)->m_lSeqNo != pstEvent->m_lSeqNo)
    {
        IBTwarn(cm, "交易流水(%d)已消失, 可能已被清理", pstEvent->m_lSeqNo);
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Resource unavailable");
        return RC_FAIL;
    }

    memcpy(pstCts, pvData->m_pvData, sizeof(GCts));
    vLogInitial(cm, pstCts);
    if(!pszData || !plLen)
	{
    	vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
		return RC_SUCC;
	}

    if((lLen = *plLen - pstCts->m_lLen) < 0)
	{
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Buffer is too small");
    	vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        return RC_FAIL;
	}

	*plLen = pstCts->m_lLen;
    memcpy((void *)pszData, pvData->m_pvData + sizeof(GCts), pstCts->m_lLen);
	memset(pszData + pstCts->m_lLen, 0, lLen);
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：bIsMonDeposit(SATvm *pstSavm, GCts *pstCts, CtsEvent *pstEvent)
    功能描述：查看交易是否被监控托管
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
BOOL    bIsMonDeposit(SATvm *pstSavm, HHCMIE cm)
{
    SHTruck   *pvData = NULL;
    RunTime   *pstRun = NULL;
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    pvData = (PSHTruck)pGetNode(pstRun->m_pvAddr, pstEvent->m_uData);
    if(((GCts *)pvData->m_pvData)->m_lSeqNo != pstEvent->m_lSeqNo)
    {
        IBTwarn(cm, "交易流水(%d)已消失, 可能已被清理", pstEvent->m_lSeqNo);
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        //    如果交易流水已消失，返回为真，该记录可以认为也被托管并清理了
        return TRUE;
    }

    if(TRADE_POOL_OUTMSG == ((GCts *)pvData->m_pvData)->m_lStatus && 
        EVENT_EVPOOL == pstEvent->m_lState && lGetTimeout() < 0)
    {
        IBTrace(cm, "交易(%d)业务组件处理超时，已被监控托管处理", pstEvent->m_lSeqNo);
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        return TRUE;
    }

    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

    return FALSE;
}

/**************************************************************************************************
    函 数 名：lSetGlobleCts(SATvm *pstSavm, GCts *pstCts, CtsEvent *pstEvent, CtsPool *root, BOOL bType)
    功能描述：上传交易共享信息 TRUE:重新组装变量池 FALSE:不组装变量池
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lSetGlobleCts(SATvm *pstSavm, HHCMIE cm, BOOL bType)
{
    SHTruck   *pvData   = NULL;
    RunTime   *pstRun   = NULL;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    pvData = (PSHTruck)pGetNode(pstRun->m_pvAddr, pstEvent->m_uData);
    if(((GCts *)pvData->m_pvData)->m_lSeqNo != pstEvent->m_lSeqNo)
    {
        IBTwarn(cm, "交易流水(%d)已消失, 可能已被清理", pstEvent->m_lSeqNo);
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Resource unavailable");
        return RC_FAIL;
    }

    pstCts->m_lStatus = TRADE_POOL_POOL;
    if(TRUE == bType)
    {
        if(RC_SUCC != lBuildPool(cm, pvData->m_pvData + sizeof(GCts), lGetUserSize(), 
            &pstCts->m_lLen))
        {
            vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
            vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Resource unavailable");
            return RC_FAIL;
        }
    }

    memcpy(pvData->m_pvData, pstCts, sizeof(GCts));
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lGetGlobleCts(SATvm *pstSavm, GCts *pstCts, CtsEvent *pstEvent, BOOL bType)
    功能描述：获取交易共享信息 TRUE  重新组装变量池 FALSE 不组装变量池
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetGlobleCts(SATvm *pstSavm, HHCMIE cm, BOOL bType)
{
    SHTruck   *pvData   = NULL;
    RunTime   *pstRun   = NULL;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    pvData = (PSHTruck)pGetNode(pstRun->m_pvAddr, pstEvent->m_uData);
    if(((GCts *)pvData->m_pvData)->m_lSeqNo != pstEvent->m_lSeqNo)
    {
        IBTwarn(cm, "交易流水(%d)已消失, 可能已被清理", pstEvent->m_lSeqNo);
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Resource unavailable");
        return RC_FAIL;
    }

    if(TRUE == bType && (TRADE_POOL_POOL == ((GCts *)pvData->m_pvData)->m_lStatus))
    {
        if(RC_SUCC != lParsePool(cm, pvData->m_pvData + sizeof(GCts), lGetUserSize(), 
            ((GCts *)pvData->m_pvData)->m_lLen))
        {
            IBTerror(cm, "解析交易(%d)共享池(%d)失败", pstEvent->m_lSeqNo, pstEvent->m_uData);
            vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
            vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Resource unavailable");
            return RC_FAIL;
        }
    }

    memcpy(pstCts, pvData->m_pvData, sizeof(GCts));
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    vLogInitial(cm, pstCts);

    return  RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lGetTradeFlow(SATvm *pstSavm, CtsFlow *pstFlow, long lSvrId, long lFlowSeq)
    功能描述：获取流程信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetTradeFlow(SATvm *pstSavm, CtsFlow *pstFlow, long lSvrID, long lFlow)
{
    CtsFlow        stFlow;

    memset(&stFlow, 0, sizeof(CtsFlow));
    stFlow.m_lFlow  = lFlow;
    stFlow.m_lSvrID = lSvrID;

    pstSavm->tblName = CTS_TRADE_FLOW;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsFlow);
    pstSavm->pstVoid = &stFlow;
    return lSelect(pstSavm, pstFlow);
}

/**************************************************************************************************
    函 数 名：lGetIdleProcess(SATvm *pstSavm, char *pszProcess, long *plPid)
    功能描述：在保证负载均衡情况下，获取最优可用的进程号
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetIdleProcess(SATvm *pstSavm, char *pszProcess, long lPos, long lNum, long *plPid)
{
    size_t      lOffset;
    RunTime     *pstRun = NULL;
    register    long    i = 0, j = 0;
    void        *pvData = NULL, *pvBack = NULL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_PLAT_PROCESS)))
        return RC_FAIL;

    lOffset = lGetRowTruck(CTS_PLAT_PROCESS) * lPos + lGetTblData(CTS_PLAT_PROCESS);
    pvData = ((SHTruck *)pGetNode(pstRun->m_pvAddr, lOffset))->m_pvData;
    for(pvBack = pvData; i < lNum; i ++, pvData += lGetRowTruck(CTS_PLAT_PROCESS))
    {
        if(BCS_APP_BUSY != ((CtsProcess *)pvData)->m_lState && 
            BCS_APP_IDLE != ((CtsProcess *)pvData)->m_lState)
            continue;

        if(BCS_APP_IDLE == ((CtsProcess *)pvData)->m_lState)
        {
            *plPid = ((CtsProcess *)pvData)->m_lPid;
            return RC_SUCC;
        }

        ++ j;
    }

    if(0 == j)
    {
        pstSavm->m_lErrno = RESOU_DISABLE;
        return RC_FAIL;
    }

    pvBack = pvBack + lGetRowTruck(CTS_PLAT_PROCESS) * (random() % j);
    *plPid = ((CtsProcess *)pvBack)->m_lPid;

    return  RC_SUCC;
}

/**************************************************************************************************
    函 数 名：bIsExistTrade()
    功能描述：检查平台运行的交易
    返回说明：
        TRUE                --成功
        FALSE               --失败
 **************************************************************************************************/
BOOL    bIsExistTrade()
{
    size_t  lOffset;
    RunTime *pstRun = NULL;
    SHTruck *pstTruck = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return false;

    for(lOffset = lGetTblData(CTS_RUNTIME_POOL); lOffset <= lGetTableSize(CTS_RUNTIME_POOL);
        lOffset += lGetRowTruck(CTS_RUNTIME_POOL))
    {
        pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, lOffset);
        if(DATA_TRUCK_NULL != pstTruck->m_chTag)
        {
            vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
            return true;
        }
    }

    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    return false;
}

/**************************************************************************************************
    函 数 名：bIsFinish(long lSeqNo, long lOfs)
    功能描述：检查该笔交易是否结束
    返回说明：
        true                --结束
          false               --未结束
 **************************************************************************************************/
BOOL    bIsFinish(SATvm *pstSavm, llSEQ lSeqNo, size_t lOffset)
{
    SHTruck *pvData = NULL;
    RunTime *pstRun = NULL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    pvData = (PSHTruck)pGetNode(pstRun->m_pvAddr, lOffset);
    if(((GCts *)pvData->m_pvData)->m_lSeqNo != lSeqNo)
    {
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        return true;
    }

    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    return false;
}

/**************************************************************************************************
    函 数 名：lCountTrade(SATvm *pstSavm)
    功能描述：新生成一个流水号
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lCountTrade()
{
    long        lCount;
    size_t      lOffset;
    RunTime     *pstRun = NULL;
    SHTruck     *pstTruck = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    for(lOffset = lGetTblData(CTS_RUNTIME_POOL); lOffset <= lGetTableSize(CTS_RUNTIME_POOL);
        lOffset += lGetRowTruck(CTS_RUNTIME_POOL))
    {
        pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, lOffset);
        if(IS_TRUCK_NULL(pstTruck))
            continue;

        ++ lCount;
    }

    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    return lCount;
}

/**************************************************************************************************
    函 数 名：lGetSeqNo(SATvm *pstSavm)
    功能描述：新生成一个流水号
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetSystSeqNo(SATvm *pstSavm)
{
    llSEQ    lSeqNo;
    RunTime *pstRun = NULL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    lSeqNo = ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ;
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    return lSeqNo;
}

/**************************************************************************************************
    函 数 名：lCreateSeqNo(SATvm *pstSavm)
    功能描述：新生成一个流水号
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lCreateSeqNo(SATvm *pstSavm)
{
    llSEQ    lSeqNo;
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        return RC_FAIL;
    }

    lSeqNo = ++ ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ;
    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    return lSeqNo;
}

/**************************************************************************************************
    函 数 名：lInitTrans(SATvm *, GCts *, long lSeqNo, long *plOfs, void *psvData, long lLen)
    功能描述：初始化交易共享池, 从表中自动寻找位置
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInitTrans(SATvm *pstSavm, GCts *pstCts, size_t *plOffset, void *psvData, long lLen)
{
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;
    SHTruck *pstTruck = NULL;

    if(lGetUserSize() < lLen)
    {
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Request message overflow");
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        return RC_FAIL;
    }

    for(*plOffset = lGetTblData(CTS_RUNTIME_POOL); *plOffset <= lGetTableSize(CTS_RUNTIME_POOL); 
        *plOffset += lGetRowTruck(CTS_RUNTIME_POOL))
    {
        pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, *plOffset);
        if(DATA_TRUCK_NULL != pstTruck->m_chTag)
            continue;

        SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
        pthread_rwlock_unlock(prwLock);
        pstCts->m_lSeqNo = ++ ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ;

        pstCts->m_lLen    = lLen;
        pstCts->m_lSeqNo  = pstCts->m_lSeqNo;
        pstCts->m_lStatus = TRADE_POOL_INMSG;
        memcpy(pstCts->m_szCrDate, ((TblDef *)pstRun->m_pvAddr)->m_szPart, 8); 
        memcpy(pstTruck->m_pvData, pstCts, sizeof(GCts));
        memcpy(pstTruck->m_pvData + sizeof(GCts), psvData, lLen);
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        return RC_SUCC;
    }
    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    pstSavm->m_lErrno = NO_DATA_FOUND;

    return RC_NOTFOUND;
}

/**************************************************************************************************
    函 数 名：lInitCall(SATvm *pstSavm, GCts *pstCts, long lSeqNo, long *plOfs)
    功能描述：针对交易自调起的交易接口
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInitCall(SATvm *pstSavm, HHCMIE cm, GCts *pstNts, long *plOffset)
{
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;
    SHTruck *pstTruck = NULL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        return RC_FAIL;
    }

    for(*plOffset = lGetTblData(CTS_RUNTIME_POOL); *plOffset <= lGetTableSize(CTS_RUNTIME_POOL); 
        *plOffset += lGetRowTruck(CTS_RUNTIME_POOL))
    {
        pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, *plOffset);
        if(DATA_TRUCK_NULL != pstTruck->m_chTag)
            continue;

        SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
        pthread_rwlock_unlock(prwLock);
        pstNts->m_lSeqNo = ++ ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ;
        pstNts->m_lStatus = TRADE_POOL_POOL;
       	memcpy(pstNts->m_szCrDate, ((TblDef *)pstRun->m_pvAddr)->m_szPart, 8); 
		IBTputstring(cm, "TR_ClearDate", pstNts->m_szCrDate);
		IBTputlong(cm, "TR_SeqNo", pstNts->m_lSeqNo);
        if(RC_SUCC != lBuildPool(cm, pstTruck->m_pvData + sizeof(GCts), lGetUserSize(), 
            &pstNts->m_lLen))
        {
            SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
            ((GCts *)pstTruck->m_pvData)->m_lStatus = TRADE_POOL_NULL;
            vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
            return RC_FAIL;
        }
        memcpy(pstTruck->m_pvData, pstNts, sizeof(GCts));
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        return RC_SUCC;
    }
    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
    pstSavm->m_lErrno = NO_DATA_FOUND;

    return RC_NOTFOUND;
}

/**************************************************************************************************
    函 数 名：lFreeTrans(SATvm *pstSavm, llSEQ lSeqNo, size_t lOffset)
    功能描述：清理交易共享池
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lFreeTrans(SATvm *pstSavm, llSEQ lSeqNo, size_t lOffset)
{
    RunTime *pstRun = NULL;
    SHTruck *pstTruck = NULL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
        return RC_FAIL;

    pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, lOffset);
    if(lSeqNo != ((GCts *)pstTruck->m_pvData)->m_lSeqNo)
    {
        vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "transaction be cleaned");
        return RC_FAIL;
    }

    ((GCts *)pstTruck->m_pvData)->m_lSeqNo  = 0;
    ((GCts *)pstTruck->m_pvData)->m_lStatus = TRADE_POOL_NULL;
    SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
    vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lUpdateGroup(SATvm *pstSavm, CtsGroup *pstGroup)
    功能描述：快速更新进程状态
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdateGroup(SATvm *pstSavm, CtsGroup *pstGroup)
{
    CtsGroup    stGroup;

    memcpy(&stGroup, pstGroup, sizeof(CtsGroup));
    
    pstSavm->pstVoid = &stGroup;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_GROUP;
    pstSavm->lSize   = sizeof(CtsGroup);

    return lUpdate(pstSavm, (void *)pstGroup, true);
}

/**************************************************************************************************
    函 数 名：lAddProcess(SATvm *pstSavm, CtsProcess *pstProcess, BOOL eType)
    功能描述：新增进程信息
    参数说明: 
        eType                --true update  false insert
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lAddProcess(SATvm *pstSavm, CtsProcess *pstProcess, BOOL eType)
{
    long        lRet;
    void        *pvData = NULL;
    RWLock      *prwLock = NULL;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_PROCESS;
    pstSavm->lSize   = sizeof(CtsProcess);
    if(!eType)
    {
        pstSavm->pstVoid = pstProcess;
        return lInsert(pstSavm);
    }

    pstSavm->pstVoid   = NULL;
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock((void *)pGetAddr(pstSavm, pstSavm->tblName));
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    while(1)
    {
        lRet = lNextFetch(pstSavm, CTS_PLAT_PROCESS, (void **)&pvData);
        if(RC_FAIL == lRet)
        {
            pthread_rwlock_unlock(prwLock);
            vTableClose(pstSavm);
            return RC_FAIL;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        if(strcmp(((CtsProcess *)pvData)->m_szPGName, pstProcess->m_szPGName) ||
            ((CtsProcess *)pvData)->m_lHost != pstProcess->m_lHost)
            continue;    

        if(BCS_APP_BUSY == ((CtsProcess *)pvData)->m_lState || 
            BCS_APP_IDLE == ((CtsProcess *)pvData)->m_lState)
            continue;

        ((CtsProcess *)pvData)->m_lPend  = 0;
        ((CtsProcess *)pvData)->m_lDoNum = 0;
        ((CtsProcess *)pvData)->m_lPid   = pstProcess->m_lPid;
        ((CtsProcess *)pvData)->m_lState = pstProcess->m_lState;
        ((CtsProcess *)pvData)->m_lRunExt= pstProcess->m_lRunExt;
        ((CtsProcess *)pvData)->m_lType  = pstProcess->m_lType;
        break;
    }
    pthread_rwlock_unlock(prwLock);
    vTableClose(pstSavm);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lUpdateProcess(SATvm *pstSavm, long lPid, long lState, long lExt)
    功能描述：快速更新进程状态
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lUpdateProcess(SATvm *pstSavm, CtsProcess *pstProcess, long lNPid, long lState, long lExt)
{
    size_t  lOffset;
    RunTime *pstRun = NULL;
    void    *pvData = NULL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_PLAT_PROCESS)))
        return RC_FAIL;

    lOffset = lGetRowTruck(CTS_PLAT_PROCESS) * pstProcess->m_lIdx + lGetTblData(CTS_PLAT_PROCESS);
    pvData = ((PSHTruck)pGetNode(pstRun->m_pvAddr, lOffset))->m_pvData;
    if(lExt !=  0)    ((CtsProcess *)pvData)->m_lRunExt = lExt;    
    if(lNPid != 0)    ((CtsProcess *)pvData)->m_lPid    = lNPid;    
    
    ((CtsProcess *)pvData)->m_lState = lState;    
    if(BCS_APP_BUSY == lState)    ((CtsProcess *)pvData)->m_lDoNum ++;
    
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lUpdateProcess(SATvm *pstSavm, CtsProcess *pstProcess, long lState)
    功能描述：更新进程状态信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdateProcess(SATvm *pstSavm, CtsProcess *pstProcess, long lState)
{
    return _lUpdateProcess(pstSavm, pstProcess, 0, lState, 0);
}

/**************************************************************************************************
    函 数 名：lExitProcess(CtsProcess *pstProcess, BOOL bf)
    功能描述：进程退出状态更新
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lExitProcess(CtsProcess *pstProcess, BOOL bf)
{
    TABLE    t;
    SATvm    *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != _lUpdateProcess(pstSavm, pstProcess, 0, BCS_APP_STOP, 0))
    {
        IBPrace("进程(%s)(%d)修改状态, err:%s", pstProcess->m_szPGName, 
            pstProcess->m_lPid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;    
    }

	if(bf)
	{
    	vResetVTData();
    	vReleaseBuffer();
    	vTvmHoldRelease(pstSavm);
	}

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lRefreshProcess(CtsProcess *pstProcess, long lCount)
    功能描述：刷新进程状态信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lRefreshProcess(CtsProcess *pstProcess, long lPid)
{
    SATvm    *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != _lUpdateProcess(pstSavm, pstProcess, lPid, pstProcess->m_lState, 
        pstProcess->m_lRunExt))
    {
        IBPrace("进程(%s)(%d)修改状态(%d)(%d), err:%s", pstProcess->m_szPGName, 
            pstProcess->m_lPid, pstProcess->m_lState, pstProcess->m_lRunExt, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;    
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lDumpProcess(long lPid)
    功能描述：进程异常退出状态更新
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDumpProcess(long lPid)
{
    CtsProcess    stProcess;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lGetProcess(pstSavm, &stProcess, lPid))
    {
        IBPerror("获取进程(%d)运行参数失败, err:%s", lPid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != _lUpdateProcess(pstSavm, &stProcess, 0, BCS_APP_DUMP, 0))
    {
        IBPrace("进程(%s)(%d)修改状态(BCS_APP_DUMP), err:%s", stProcess.m_szPGName, 
            stProcess.m_lPid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;    
    }

    vResetVTData();
    vReleaseBuffer();
    vTvmHoldRelease(pstSavm);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInsertAsyn(SATvm *pstSavm, char *pszIdx, long lSeqNo, long lFlow, long lResv, long )
    功能描述：新增异步标识
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lRegisterAsyn(SATvm *pstSavm, CtsAsyn *pstAsyn)
{
    pstSavm->pstVoid = pstAsyn;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsAsyn);
    pstSavm->tblName = CTS_ASYN_IDENTIFY;
    return lInsert(pstSavm);
}

/**************************************************************************************************
    函 数 名：lGetAsynIdx(SATvm *, CtsAsyn *pstIdx, char *pszIdx)
    功能描述：查询异步标示记录
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetAsynIdx(SATvm *pstSavm, CtsAsyn *pstIdx, char *pszIdx)
{
    CtsAsyn    stIdx;

    memset(&stIdx, 0, sizeof(stIdx));
    strncpy(stIdx.m_szIdx, pszIdx, sizeof(stIdx.m_szIdx));

    pstSavm->tblName = CTS_ASYN_IDENTIFY;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsAsyn);
    pstSavm->pstVoid = &stIdx;
    return lSelect(pstSavm, pstIdx);
}

/**************************************************************************************************
    函 数 名：lDeleteAsyn(SATvm *pstSavm, CtsAsyn *pstIdx)
    功能描述：通过唯一标识清理异步标识记录
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDeleteAsyn(SATvm *pstSavm, CtsAsyn *pstIdx)
{
    pstSavm->tblName = CTS_ASYN_IDENTIFY;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsAsyn);
    pstSavm->pstVoid = pstIdx;
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lRecycleAsyn(SATvm *pstSavm, char *pszClearDay)
    功能描述：交易日切时候，回收上周期异步标识
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lRecycleAsyn(SATvm *pstSavm, char *pszClearDay)
{
    CtsAsyn    stIdx;

    memset(&stIdx, 0, sizeof(CtsAsyn));
    strncpy(stIdx.m_szCrDate, pszClearDay, sizeof(stIdx.m_szCrDate));

    pstSavm->pstVoid = &stIdx;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsAsyn);
    pstSavm->tblName = CTS_ASYN_IDENTIFY;
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lResetAsyn(SATvm *pstSavm, long lSeqNo, long lOfs)
    功能描述：交易超时时，通过流水号清理异步标识记录
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lResetAsyn(SATvm *pstSavm, long lSeqNo, char *pszClear)
{
    CtsAsyn    stIdx;

    memset(&stIdx, 0, sizeof(CtsAsyn));
    stIdx.m_lSeqNo = lSetEXLong(lSeqNo);
    strncpy(stIdx.m_szCrDate, pszClear, sizeof(stIdx.m_szCrDate));

    pstSavm->pstVoid = &stIdx;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsAsyn);
    pstSavm->tblName = CTS_ASYN_IDENTIFY;
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInsertAsyn(SATvm *pstSavm, char *pszIdx, long lSeqNo, long lFlow, long lResv, long )
    功能描述：新增异步标识
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInsertAsyn(SATvm *pstSavm, char *pszIdx, GCts *pstCts, long lFlow, long lResv, size_t uData)
{
    CtsAsyn    stIdx;

    memset(&stIdx, 0, sizeof(CtsAsyn));
    stIdx.m_uData   = uData;
    stIdx.m_lResv   = lResv;
    stIdx.m_lFlow   = lFlow;
    stIdx.m_lSeqNo  = pstCts->m_lSeqNo;
    stIdx.m_lIdxTime= lGetUnixTime();
    strncpy(stIdx.m_szIdx, pszIdx, sizeof(stIdx.m_szIdx));
    strncpy(stIdx.m_szCrDate, pstCts->m_szCrDate, sizeof(stIdx.m_szCrDate));
    pstCts->m_lAsyn = TRADE_ENABLE_ASYN;

    pstSavm->pstVoid = &stIdx;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsAsyn);
    pstSavm->tblName = CTS_ASYN_IDENTIFY;
    return lInsert(pstSavm);
}

/**************************************************************************************************
    函 数 名：lSaveAsynIdx(SATvm *pstSavm, char *pszIdx, CtsEvent *pstEvent)
    功能描述：保存异步标识
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lSaveAsynIdx(SATvm *pstSavm, char *pszIdx, GCts *pstCts, CtsEvent *pstEvent)
{
    if(!pstEvent)
        return lInsertAsyn(pstSavm, pszIdx, pstCts, 0, 0, 0);
    else
        return lInsertAsyn(pstSavm, pszIdx, pstCts, pstEvent->m_lFlow, pstEvent->m_lResv, 
            pstEvent->m_uData);
}

/**************************************************************************************************
    函 数 名：lCleanupAsyn()
    功能描述：清理异步标识表
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lCleanupAsyn()
{
    return lTruncate((SATvm *)pGetSATvm(), CTS_ASYN_IDENTIFY);
}

/**************************************************************************************************
        配置刷新功能函数
 **************************************************************************************************/
/**************************************************************************************************
    函 数 名：lUpdateHost(CtsHost *pstHost)
    功能描述：更新主机信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdateHost(SATvm *pstSavm, CtsHost *pstHost)
{
    CtsHost    stHost;
	CtsGroup   stGroup, stUpdate;

	memset(&stHost, 0, sizeof(CtsHost));
    stHost.m_lHost = pstHost->m_lHost;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_TRADE_HOST;
    pstSavm->lSize   = sizeof(CtsHost);
    pstSavm->pstVoid = (void *)&stHost;
    if(RC_SUCC != lUpdate(pstSavm, pstHost, true))
		return RC_FAIL;

	memset(&stGroup, 0, sizeof(CtsGroup));
	memset(&stUpdate, 0, sizeof(CtsGroup));
	stUpdate.m_lMinNum  = pstHost->m_lMinNum;
    stUpdate.m_lAutoExt = pstHost->m_lAutoExt;
    stUpdate.m_bIsBoot  = pstHost->m_bIsBoot;
    stUpdate.m_bDbRcon  = pstHost->m_bDbRcon;
    stUpdate.m_lType    = pstHost->m_lType;
    stGroup.m_lHost     = pstHost->m_lHost;
	strncpy(stUpdate.m_szTime, sGetUpdTime(), sizeof(stUpdate.m_szTime));
    strncpy(stGroup.m_szPGName, pstHost->m_szPGName, sizeof(stGroup.m_szPGName));

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_GROUP;
    pstSavm->lSize   = sizeof(CtsGroup);
    pstSavm->pstVoid = (void *)&stGroup;
    if(RC_SUCC != lUpdate(pstSavm, (void *)&stUpdate, false))
	{
		if(NO_DATA_FOUND != pstSavm->m_lErrno)
			return RC_FAIL;
	}

	return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lDeleteHost(CtsHost *pstHost)
    功能描述：删除主机主机信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDeleteHost(SATvm *pstSavm, CtsHost *pstHost)
{
    CtsHost    stHost;

	memset(&stHost, 0, sizeof(CtsHost));
    stHost.m_lHost = pstHost->m_lHost;

    pstSavm->tblName = CTS_TRADE_HOST;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsHost);
    pstSavm->pstVoid = &stHost;
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lRemoveHost(CtsHost *pstHost)
    功能描述：删除主机主机信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lRemoveHost(SATvm *pstSavm, CtsHost *pstHost)
{
    size_t        i, lOut = 0;
    CtsClass    stClass, *pstClass = NULL;
    CtsService    stService, *pstService = NULL;

    memset(&stClass, 0, sizeof(CtsClass));
    memset(&stService, 0, sizeof(CtsService));
    stClass.m_lHost   = pstHost->m_lHost;    
    stService.m_lHost = pstHost->m_lHost;

    if(RC_SUCC != lDeleteHost(pstSavm, pstHost))
        return RC_FAIL;

    pstSavm->pstVoid = &stClass;
    pstSavm->tblName = CTS_TRADE_CLASS;
    pstSavm->lSize   = sizeof(CtsClass);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    if(RC_SUCC != lQuery(pstSavm, &lOut, (void **)&pstClass))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
        {
            IBPerror("查询容器(%d)组件信息失败, err:%s", stClass.m_lHost,
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }

    for(i = 0; i < lOut; i ++)
        lRemoveClass(pstSavm,  &pstClass[i]);        ;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->pstVoid = &stService;
    pstSavm->lSize   = sizeof(CtsService);
    pstSavm->tblName = CTS_TRADE_SERVICE;
    if(RC_SUCC != lQuery(pstSavm, &lOut, (void **)&pstService))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            return RC_SUCC;

        IBPerror("查询容器(%d)组件信息失败, err:%s", stService.m_lHost, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    
    for(i = 0; i < lOut; i ++)
    {
        if(RC_SUCC != lRemoveService(pstSavm, &pstService[i]))
        {
            IBPFree(pstService);
            return RC_FAIL;
        }
    }

    IBPFree(pstService);
    
    return RC_SUCC;    
}

/**************************************************************************************************
    函 数 名：lInsertHost(CtsHost *pstHost)
    功能描述：删除主机主机信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInsertHost(SATvm *pstSavm, CtsHost *pstHost)
{
    pstSavm->tblName = CTS_TRADE_HOST;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsHost);
    pstSavm->pstVoid = pstHost;
    return lInsert(pstSavm);
}

/**************************************************************************************************
    函 数 名：lUpdateClass(CtsClass *pstClass)
    功能描述：删除主机主机信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdateClass(SATvm *pstSavm, CtsClass *pstClass)
{
    CtsClass    stClass;
    
    memset(&stClass, 0, sizeof(CtsClass));
    stClass.m_lBuss = pstClass->m_lBuss;

    pstSavm->pstVoid = &stClass;
    pstSavm->tblName = CTS_TRADE_CLASS;
    pstSavm->lSize   = sizeof(CtsClass);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    return lUpdate(pstSavm, pstClass, true);
}

/**************************************************************************************************
    函 数 名：lDeleteClass(CtsClass *pstClass)
    功能描述：删除主机主机信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDeleteClass(SATvm *pstSavm, CtsClass *pstClass)
{
    CtsClass    stClass;
    
    memset(&stClass, 0, sizeof(CtsClass));
    stClass.m_lBuss = pstClass->m_lBuss;

    pstSavm->pstVoid = &stClass;
    pstSavm->tblName = CTS_TRADE_CLASS;
    pstSavm->lSize   = sizeof(CtsClass);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInsertClass(CtsClass *pstClass)
    功能描述：新增组件配置信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInsertClass(SATvm *pstSavm, CtsClass *pstClass)
{
    pstSavm->pstVoid = pstClass;
    pstSavm->tblName = CTS_TRADE_CLASS;
    pstSavm->lSize   = sizeof(CtsClass);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    return lInsert(pstSavm);
}

/**************************************************************************************************
    函 数 名：lDeleteService(CtsService *pstSService)
    功能描述：删除服务信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDeleteService(SATvm *pstSavm, CtsService *pstService)
{
    CtsService    stService;
    
    //    避免pstService为NULL删除全部
    memset(&stService, 0, sizeof(CtsService));    
    stService.m_lSvrID = pstService->m_lSvrID;

    pstSavm->tblName = CTS_TRADE_SERVICE;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsService);
    pstSavm->pstVoid = &stService;
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lRemoveService(SATvm *pstSavm, CtsService *pstSService)
    功能描述：删除服务&依赖的服务信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lRemoveService(SATvm *pstSavm, CtsService *pstService)
{
    if(RC_SUCC != lDeleteService(pstSavm, pstService))
        return RC_FAIL;
    
    return lDeleteFlow(pstSavm, pstService->m_lSvrID);
}

/**************************************************************************************************
    函 数 名：lUpdateService(CtsService *pstSService)
    功能描述：删除服务信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdateService(SATvm *pstSavm, CtsService *pstService)
{
    CtsService    stService;
    
    memset(&stService, 0, sizeof(CtsService));    
    stService.m_lSvrID = pstService->m_lSvrID;

    pstSavm->tblName = CTS_TRADE_SERVICE;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsService);
    pstSavm->pstVoid = &stService;
    return lUpdate(pstSavm, pstService, true);
}

/**************************************************************************************************
    函 数 名：lInsertService(CtsService *pstSService)
    功能描述：删除服务信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInsertService(SATvm *pstSavm, CtsService *pstService)
{
    pstSavm->tblName = CTS_TRADE_SERVICE;
    pstSavm->lSize   = sizeof(CtsService);
    pstSavm->pstVoid = pstService;
    return lInsert(pstSavm);
}

/**************************************************************************************************
    函 数 名：lDeleteTrCode(CtsTrCode *pstSCode)
    功能描述：删除交易码信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDeleteTrCode(SATvm *pstSavm, CtsTrCode *pstCode)
{
    CtsTrCode    stCode;
    
    //    避免pstCode为NULL删除全部
    memset(&stCode, 0, sizeof(CtsTrCode));    
    stCode.m_lIdx = pstCode->m_lIdx;
    strncpy(stCode.m_szInCode, pstCode->m_szInCode, sizeof(stCode.m_szInCode));
    strncpy(stCode.m_szInSCode, pstCode->m_szInSCode, sizeof(stCode.m_szInSCode));

    pstSavm->pstVoid = &stCode;
    pstSavm->tblName = CTS_TRADE_CODE;
    pstSavm->lSize   = sizeof(CtsTrCode);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lUpdateTrCode(CtsTrCode *pstSCode)
    功能描述：更新交易码信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdateTrCode(SATvm *pstSavm, CtsTrCode *pstCode)
{
    CtsTrCode    stCode;
    
    //    避免pstCode为NULL删除全部
    memset(&stCode, 0, sizeof(CtsTrCode));    
    stCode.m_lIdx = pstCode->m_lIdx;

    pstSavm->pstVoid = &stCode;
    pstSavm->tblName = CTS_TRADE_CODE;
    pstSavm->lSize   = sizeof(CtsTrCode);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    return lUpdate(pstSavm, pstCode, true);
}

/**************************************************************************************************
    函 数 名：lInsertTrCode(CtsTrCode *pstSCode)
    功能描述：新增交易码信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInsertTrCode(SATvm *pstSavm, CtsTrCode *pstCode)
{
    pstSavm->pstVoid = pstCode;
    pstSavm->tblName = CTS_TRADE_CODE;
    pstSavm->lSize   = sizeof(CtsTrCode);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    return lInsert(pstSavm);
}

/**************************************************************************************************
    函 数 名：lDeleteMapCode(SATvm *pstSavm, CtsMapCode *pstMCode)
    功能描述：删除交易码
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDeleteMapCode(SATvm *pstSavm, CtsMapCode *pstMCode)
{
    CtsMapCode    stMCode;
    
    //    避免pstCode为NULL删除全部
    memset(&stMCode, 0, sizeof(CtsMapCode));    
    strncpy(stMCode.m_szEntNo, pstMCode->m_szEntNo, sizeof(stMCode.m_szEntNo));
    strncpy(stMCode.m_szInCode, pstMCode->m_szInCode, sizeof(stMCode.m_szInCode));

    pstSavm->pstVoid = &stMCode;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_RETCODE_MAP;
    pstSavm->lSize   = sizeof(CtsMapCode);
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lUpdateMapCode(CtsMapCode *pstSCode)
    功能描述：新增交易码信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdateMapCode(SATvm *pstSavm, CtsMapCode *pstMCode)
{
    CtsMapCode    stMCode;
    
    memset(&stMCode, 0, sizeof(CtsMapCode));    
    stMCode.m_lIdx = pstMCode->m_lIdx;

    pstSavm->pstVoid = &stMCode;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_RETCODE_MAP;
    pstSavm->lSize   = sizeof(CtsMapCode);
    return lUpdate(pstSavm, pstMCode, true);
}

/**************************************************************************************************
    函 数 名：lInsertMapCode(CtsMapCode *pstSCode)
    功能描述：新增交易码信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInsertMapCode(SATvm *pstSavm, CtsMapCode *pstMCode)
{
    pstSavm->pstVoid = pstMCode;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_RETCODE_MAP;
    pstSavm->lSize   = sizeof(CtsMapCode);
    return lInsert(pstSavm);
}

/**************************************************************************************************
    函 数 名：lGetMapTruck(SATvm *pstSavm, CtsMapCode *pstMCode)
    功能描述：新增交易码信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetMapTruck(SATvm *pstSavm, CtsMapCode *pstMCode)
{
    pstSavm->pstVoid = pstMCode;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_RETCODE_MAP;
    pstSavm->lSize   = sizeof(CtsMapCode);
    return lSelect(pstSavm, pstMCode);
}

/**************************************************************************************************
    函 数 名：lGetMapTruck(SATvm *pstSavm, CtsMapCode *pstMCode)
    功能描述：新增交易码信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGetMapCode(SATvm *pstSavm, CtsMapCode *pstMCode, char *pszEntNo, char *pszInCode)
{
    CtsMapCode    stMCode;

    memset(&stMCode, 0, sizeof(CtsMapCode));
    strncpy(stMCode.m_szEntNo, pszEntNo, sizeof(stMCode.m_szEntNo));
    strncpy(stMCode.m_szInCode, pszInCode, sizeof(stMCode.m_szInCode));

    pstSavm->pstVoid = &stMCode;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_RETCODE_MAP;
    pstSavm->lSize   = sizeof(CtsMapCode);
    return lSelect(pstSavm, pstMCode);
}

/**************************************************************************************************
    函 数 名：lDeleteFlow(CtsFlow *pstFlow, long lCount)
    功能描述：删除流程信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDeleteFlow(SATvm *pstSavm, long lSvrID)
{
    CtsFlow        stFlow;

    memset(&stFlow, 0, sizeof(CtsFlow));    
    stFlow.m_lSvrID = lSvrID;

    pstSavm->tblName = CTS_TRADE_FLOW;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsFlow);
    pstSavm->pstVoid = &stFlow;
    if(RC_SUCC != lDelete(pstSavm))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInsertFlow(CtsFlow *pstFlow, long lCount)
    功能描述：新增流程信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInsertFlow(SATvm *pstSavm, CtsFlow *pstFlow, size_t lCount)
{
    return lImportTable(CTS_TRADE_FLOW, lCount, pstFlow);
}

/**************************************************************************************************
    函 数 名：lUpdateFlow(CtsFlow *pstFlow, long lCount)
    功能描述：更新流程信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdateFlow(SATvm *pstSavm, CtsFlow *pstFlow, size_t lCount)
{
    if(!pstFlow)    return RC_FAIL;

    if(RC_SUCC != lDeleteFlow(pstSavm, pstFlow[0].m_lSvrID))
    {
        IBPerror("删除交易流程(%d)失败, err:%s", pstFlow[0].m_lSvrID, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return lInsertFlow(pstSavm, pstFlow, lCount);
}

/**************************************************************************************************
    函 数 名：lDeletePack(SATvm *pstSavm, long lMid)
    功能描述：删除流程信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lDeletePack(SATvm *pstSavm, long lMid)
{
    CtsPack        stPack;
    
    memset(&stPack, 0, sizeof(CtsPack));    
    stPack.m_lMid = lMid;

    pstSavm->pstVoid = &stPack;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsPack);
    pstSavm->tblName = CTS_PACK_CONFIG;
    if(RC_SUCC == lDelete(pstSavm))
        return RC_SUCC;
    else if(NO_DATA_FOUND == pstSavm->m_lErrno)
        return RC_SUCC;

    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：lInsertPack(SATvm *pstSavm, CtsPack *pstPack, long lCount)
    功能描述：新增流程信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInsertPack(SATvm *pstSavm, CtsPack *pstPack, size_t lCount)
{
    return lImportTable(CTS_PACK_CONFIG, lCount, pstPack);
}

/**************************************************************************************************
    函 数 名：lUpdatePack(SATvm *pstSavm, CtsPack *pstPack, long lCount)
    功能描述：更新报文配置字段
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lUpdatePack(SATvm *pstSavm, CtsPack *pstPack, long lCount)
{
    if(!pstPack)    return RC_FAIL;

    if(RC_SUCC != lDeletePack(pstSavm, pstPack[0].m_lMid))
    {
        IBPerror("删除报文配置(%d)失败, err:%s", pstPack[0].m_lMid, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return lInsertPack(pstSavm, pstPack, lCount);
}

/**************************************************************************************************
    函 数 名：lRemoveClass(SATvm *pstSavm, CtsClass *pstClass)
    功能描述：删除组件依赖配置
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lRemoveClass(SATvm *pstSavm, CtsClass *pstClass)
{
    if(RC_SUCC != lDeleteClass(pstSavm, pstClass))
        return RC_FAIL;
    
    if(RC_SUCC != lDeletePack(pstSavm, pstClass->m_lInMid))
        return RC_FAIL;
    
    if(RC_SUCC != lDeletePack(pstSavm, pstClass->m_lOutMid))
        return RC_FAIL;

    if(RC_SUCC != lDeletePack(pstSavm, pstClass->m_lErrMid))
        return RC_FAIL;

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lGenPackIdx()
    功能描述：生成pack编号
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGenPackIdx()
{
	CtsPack    stPack;
    SATvm      *pstSavm = (SATvm *)pGetSATvm();
    
    pstSavm->pstVoid  = NULL;
    pstSavm->lReMatch = MATCH_MAX;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->tblName  = CTS_PACK_CONFIG;
    pstSavm->lSize    = sizeof(CtsPack);
    pstSavm->lReField = ReField(CtsPack, m_lMid);
    if(RC_SUCC != lExtreme(pstSavm, &stPack))
	{
    	if(NO_DATA_FOUND == pstSavm->m_lErrno)
        	return 1;
		return RC_FAIL;
	}

    return ++ stPack.m_lMid;
}

/**************************************************************************************************
    函 数 名：lGenHostIdx(SATvm *pstSavm, long lIdx)
    功能描述：获取新的主机ID
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGenHostIdx()
{
	CtsHost    stHost;
    SATvm      *pstSavm = (SATvm *)pGetSATvm();
    
    pstSavm->pstVoid  = NULL;
    pstSavm->lReMatch = MATCH_MAX;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->tblName  = CTS_TRADE_HOST;
    pstSavm->lSize    = sizeof(CtsHost);
    pstSavm->lReField = ReField(CtsHost, m_lHost);
    if(RC_SUCC != lExtreme(pstSavm, &stHost))
	{
    	if(NO_DATA_FOUND == pstSavm->m_lErrno)
        	return 1;
		return RC_FAIL;
	}

    return ++ stHost.m_lHost;
}

/**************************************************************************************************
    函 数 名：lGenClassIdx()
    功能描述：获取新的组件ID
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGenClassIdx()
{
	CtsClass   stClass;
    SATvm      *pstSavm = (SATvm *)pGetSATvm();
    
    pstSavm->pstVoid  = NULL;
    pstSavm->lReMatch = MATCH_MAX;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->tblName  = CTS_TRADE_CLASS;
    pstSavm->lSize    = sizeof(CtsClass);
    pstSavm->lReField = ReField(CtsClass, m_lBuss);
    if(RC_SUCC != lExtreme(pstSavm, &stClass))
	{
    	if(NO_DATA_FOUND == pstSavm->m_lErrno)
        	return 1;
		return RC_FAIL;
	}

    return ++ stClass.m_lBuss;
}

/**************************************************************************************************
    函 数 名：lGenServiceIdx()
    功能描述：获取新的服务ID
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGenServiceIdx()
{
	CtsService stService;
    SATvm      *pstSavm = (SATvm *)pGetSATvm();
    
    pstSavm->pstVoid  = NULL;
    pstSavm->lReMatch = MATCH_MAX;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->tblName  = CTS_TRADE_SERVICE;
    pstSavm->lSize    = sizeof(CtsService);
    pstSavm->lReField = ReField(CtsService, m_lSvrID);
    if(RC_SUCC != lExtreme(pstSavm, &stService))
	{
    	if(NO_DATA_FOUND == pstSavm->m_lErrno)
        	return 1;
		return RC_FAIL;
	}

    return ++ stService.m_lSvrID;
}

/**************************************************************************************************
    函 数 名：lGenTrCodeIdx(SATvm *pstSavm, long lIdx)
    功能描述：新增交易码ID
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGenTrCodeIdx()
{
    CtsTrCode  stCode;
    SATvm      *pstSavm = (SATvm *)pGetSATvm();
    
    pstSavm->pstVoid  = NULL;
    pstSavm->lReMatch = MATCH_MAX;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->tblName  = CTS_TRADE_CODE;
    pstSavm->lSize    = sizeof(CtsTrCode);
    pstSavm->lReField = ReField(CtsTrCode, m_lIdx);
    if(RC_SUCC != lExtreme(pstSavm, &stCode))
	{
    	if(NO_DATA_FOUND == pstSavm->m_lErrno)
        	return 1;
		return RC_FAIL;
	}

    return ++ stCode.m_lIdx;
}

/**************************************************************************************************
    函 数 名：lGenMapCodeIdx()
    功能描述：新增交易码ID
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lGenMapCodeIdx()
{
    CtsMapCode  stMCode;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();
    
    pstSavm->pstVoid  = NULL;
    pstSavm->lReMatch = MATCH_MAX;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->tblName  = CTS_RETCODE_MAP;
    pstSavm->lSize    = sizeof(CtsMapCode);
    pstSavm->lReField = ReField(CtsMapCode, m_lIdx);
    if(RC_SUCC != lExtreme(pstSavm, &stMCode))
	{
    	if(NO_DATA_FOUND == pstSavm->m_lErrno)
        	return 1;
		return RC_FAIL;
	}

    return ++ stMCode.m_lIdx;
}

/*************************************************************************************************
    code end
 *************************************************************************************************/
