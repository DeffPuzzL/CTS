/**************************************************************************************************
    文 件 名：cts_msgex.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：cts系统接入、返回报文解析模块
 **************************************************************************************************/
#include    "face.h"

/**************************************************************************************************
    报文的最大空间、报文解析二级缓存
 **************************************************************************************************/
char        *g_pszBuffer = NULL;

/**************************************************************************************************
    函 数 名：vPrintVersion(char *pszParm)
    功能描述：显示该应用的版本
    返回说明：
        无
 **************************************************************************************************/
void    vPrintVersion(char *pszParm)
{
    if(!pszParm)    return ;
    if(strcmp(sUpper(pszParm), "-V"))    return ;

    fprintf(stdout, "current version:%s\n", CTS_VERSION);

    exit(0);
}

/**************************************************************************************************
    函 数 名：lPacketParse(SATvm *pstSavm, CtsEvent *pstEvent)
    功能描述：报文解析模块 
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lPacketParse(SATvm *pstSavm, HHCMIE cm, char *pszOut, long *plLen)
{
    struct stat st;
    CtsTrCode   stCode;
    CtsClass    stClass;
    CtsService  stService;
    char        szBak[PATH_LOGNMAE_LEN];
    GCts        *pstCts = (GCts *)pCmieGloble(cm);

    if(RC_SUCC != lGetDrivePack(pstSavm, cm, pszOut, plLen))
    {
        SYSTrror(cm, RET_SYSTEM, "%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lGetClass(pstSavm, &stClass, pstCts->m_szInCode, pstCts->m_szInSCode))
    {
        SYSTrror(cm, RET_SYSTEM, "获取组件(%s)(%s)失败, %s", pstCts->m_szInCode, 
			pstCts->m_szInSCode, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    
    if(RC_SUCC != lGetTrCode(pstSavm, &stCode, pstCts->m_szInCode, pstCts->m_szInSCode))
    {
        SYSTrror(cm, RET_SYSTEM, "根据接入(%s)(%s)定位系统交易失败, err:%s", pstCts->m_szInCode, 
            pstCts->m_szInSCode, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    pstCts->m_lInHost = stClass.m_lHost;
    pstCts->m_lHost   = stClass.m_lHost;
    pstCts->m_lInMid  = stClass.m_lInMid;
    pstCts->m_lOutMid = stClass.m_lOutMid;
    pstCts->m_lErrMid = stClass.m_lErrMid;
    strncpy(pstCts->m_szSvrName, stClass.m_szMark, sizeof(pstCts->m_szSvrName) - 1);
    strncpy(pstCts->m_szTrCode, stCode.m_szTrCode, sizeof(pstCts->m_szTrCode) - 1);

    if(RC_SUCC != lGetService(pstSavm, &stService, pstCts->m_szTrCode))
    {
        SYSTrror(cm, RET_SYSTEM, "获取系统服务(%s)失败, %s", pstCts->m_szTrCode, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    vSetSysLevel(cm, stService.m_lLogLevel);
    pstCts->m_lLogLev  = stService.m_lLogLevel;
    pstCts->m_lHost    = stService.m_lHost;
    pstCts->m_lSvrID   = stService.m_lSvrID;
    pstCts->m_lSType   = stService.m_lSvrType;
    pstCts->m_lTimeOut = stService.m_lTimeOut > 0 ? stService.m_lTimeOut : 60;
    strncpy(pstCts->m_szSvrName, stService.m_szSvrName, sizeof(pstCts->m_szSvrName) - 1);
    strncpy(pstCts->m_szLogName, stService.m_szLogName, sizeof(pstCts->m_szLogName) - 1);
    sAlterPath(cm->m_szLog, PATH_LOGNMAE_LEN, pstCts->m_szLogName);

    if(0 == stat(cm->m_szLog, &st) && st.st_size >= lLogGetSize())
    {
        snprintf(szBak, sizeof(szBak), "%s.%s", cm->m_szLog, pstCts->m_szTxTime);
        rename(cm->m_szLog, szBak);
    }

    //  判断该服务是否启用
    if(!IBPIsable(stService.m_lState))
    {
        SYSTrror(cm, RET_SYSTEM, "服务(%s)已停用", pstCts->m_szTrCode);
        return RC_FAIL;
    }
    IBTinfo(cm, "根据交易(%s)(%s)定位平台交易(%s)成功", pstCts->m_szInCode, pstCts->m_szInSCode,
        stCode.m_szTrCode);

    IBTsysm(cm, "开始解读报文(%d)数据如下(%d):", pstCts->m_lInMid, *plLen);
    IBTcem(cm, pszOut, *plLen);

    vSetTradePool(cm);
    IBTsysm(cm, "开始解析接入主机(%d)(%s)进程(%d)交易(%s)(%s)请求", pstCts->m_lInHost,
        pstCts->m_szSvrName, pstCts->m_lInPid, pstCts->m_szInCode, pstCts->m_szInSCode);

    if(RC_SUCC != lPackReadAt(pstSavm, cm, pstCts->m_lInMid, pszOut, plLen))
    {
        SYSError(RET_SYSTEM, "解读报文(%d)失败, %s", pstCts->m_lInMid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lBuildPacket(SATvm *pstSavm, CtsEvent *pstEvent)
    功能描述：报文组装模块 
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lBuildPacket(SATvm *pstSavm, HHCMIE cm, void *psvOut, long *plOut)
{
    long      lRet;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(!psvOut || !plOut)
    {
        SYSTrror(cm, RET_SYSTEM, "组织报文参数设置错误(%X)(%X)", psvOut, plOut);
        return RC_FAIL;
    }

    //    获取变量池数据
    if(RC_SUCC != lGetGlobleCts(pstSavm, cm, TRUE))
    {
        SYSTrror(cm, RET_SYSTEM, "获取交易(%d)运行交换参数失败, err:%s", pstEvent->m_lSeqNo, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(IBPIsok())
    {
        lRet = lPackWriteAt(pstSavm, cm, pstCts->m_lOutMid, psvOut, plOut);
        IBTsysm(cm, "组织正常响应报文(%d)，数据如下(%d):", pstCts->m_lOutMid, *plOut);
    }
    else 
    {
        if(pstCts->m_lErrMid <= 0)
            lRet = lPackWriteAt(pstSavm, cm, pstCts->m_lOutMid, psvOut, plOut);
        else
            lRet = lPackWriteAt(pstSavm, cm, pstCts->m_lErrMid, psvOut, plOut);
        IBTsysm(cm, "组织错误响应报文(%d)，数据如下(%d):", pstCts->m_lErrMid, *plOut);
    }
    if(RC_SUCC != lRet)
    {
		*plOut = 0;
        SYSTrror(cm, RET_SYSTEM, "组织报文失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    IBTcem(cm, psvOut, *plOut);

    return RC_SUCC;
}

/*************************************************************************************************
    报文解析/组织接入处理
 *************************************************************************************************/ 
long    lPacketCore(SATvm *pstSavm, HHCMIE cm)
{
    long      lLen = lGetUserSize();
    GCts      *pstCts = (GCts *)pGetGloble();
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(IBPevinmsg(pstEvent))
    {
//		memset(g_pszBuffer, 0, lGetUserSize());
        if(RC_SUCC != lPacketParse(pstSavm, cm, g_pszBuffer, &lLen))
        {
            pstEvent->m_lState = EVENT_EVOUTMSG;    //    解包失败，直接返回
            lSetDrivePack(pstSavm, cm, NULL, 0);
            return lPacketCore(pstSavm, cm);
        }

        //  报文解析完毕需要丢送给核心继续下面的处理
        if(RC_SUCC != lGetIdleProcess(pstSavm, CTS_CORE, lGetToPos(), lGetToNum(),
            &pstEvent->m_lMType))
        {
            pstEvent->m_lState = EVENT_EVOUTMSG;    //    解包失败，直接返回
            SYSTrror(cm, RET_SYSTEM, "调度进程(%s)组失败, %s", CTS_CORE, 
                sGetTError(pstSavm->m_lErrno));
            lSetDrivePack(pstSavm, cm, NULL, 0);
            return lPacketCore(pstSavm, cm);
        }

        //    将数据填充到变量池中
        pstCts->m_lPid = pstEvent->m_lMType;
        if(RC_SUCC != lSetGlobleCts(pstSavm, cm, TRUE)) 
        {
            pstEvent->m_lState = EVENT_EVOUTMSG;    //    解包失败，直接返回
            SYSTrror(cm, RET_SYSTEM, "上传交易(%d)共享池失败, %s", pstEvent->m_lSeqNo,
                sGetTError(pstSavm->m_lErrno));
            IBPcrit("交易(%d), 严重错误，联系开发人员!!!!!!!!!", pstEvent->m_lSeqNo);
            return RC_FAIL;
        }
    }
    else if(IBPevoutmsg(pstEvent))    //  core-out 请求的组织报文
    {
        if(RC_SUCC != lBuildPacket(pstSavm, cm, g_pszBuffer, &lLen))
            IBTerror(cm, "组织交易(%d)响应报文错误", pstCts->m_lSeqNo);

        pstCts->m_lPid = pstCts->m_lInPid;
        return lSetDrivePack(pstSavm, cm, g_pszBuffer, lLen);
    }
    else
        ; 

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：main
    功能描述：报文组装/解析处理入口
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
int        main(int argc, char *argv[])
{
    CtsProcess   stProcess;
	HHCMIE       cm = (HHCMIE)pGetCmie();
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    vPrintVersion(argv[1]);
    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));
    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", basename(argv[0]));
        return RC_SUCC;
    }

	if(NULL == (g_pszBuffer = (char *)pGetBuffer()))
    {
        IBPerror("分配进程启动内存失败");
        return ;
    }

    IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, g_szVersion);

    while(1)
    {
        if(RC_SUCC == lGetPackRequest(pstSavm, &stProcess, cm))
        {
            if(IBPevcache(pCmieEvent(cm)))
                continue;

            lPacketCore(pstSavm, cm);
        }

        lPutPackResponse(pstSavm, cm);
    }

    return RC_SUCC;
}

/**************************************************************************************************
    code end
 **************************************************************************************************/
