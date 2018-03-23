/**************************************************************************************************
    文 件 名：print.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：打印cts核心参数
 **************************************************************************************************/
#include    "face.h"

/**************************************************************************************************
 * 函数
 **************************************************************************************************/
/**************************************************************************************************
    函 数 名：lReadMemCfg(char *pszBuffer, long lFileLen)
    功能描述：从内存中获取配置
    返回说明：
        无
 **************************************************************************************************/
long    lDumpToFile(char *pszFile)
{
    size_t    lLen = 0;
    FILE      *fp = NULL;
    void      *pvData = NULL;
    SATvm     *pstSavm = (SATvm *)pGetSATvm();

    if(!bIsTvmBoot())
    {
        fprintf(stderr, "\n平台未启动!\n\n");
        return ;
    }

    if(RC_FAIL == (lLen = lExportEngine(&pvData, 0)))
    {
        fprintf(stderr, "导出运行配置失败, %s\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(NULL == (fp = fopen(pszFile, "w")))
    {
        IBPFree(pvData);
        IBPerror("打开配置文件(%s)失败, err:%s", pszFile, strerror(errno));
        return RC_FAIL;
    }

    fwrite(pvData, lLen, 1, fp);
    fclose(fp);
    IBPFree(pvData);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vUnloadConfig()
    功能描述：从当前运行系统中紧急回复配置
    返回说明：
        无
 **************************************************************************************************/
void    vUnloadConfig()
{
    char    szPath[1024];

    memset(szPath, 0, sizeof(szPath));
    snprintf(szPath, sizeof(szPath), "%s.rec", getenv("SETUPCFG"));
    if(RC_SUCC != lDumpToFile(szPath))
        return ;

    fprintf(stdout, "\n导出运行配置成功, completed successfully!!!\n\n");
    return ;
}

/**************************************************************************************************
    函 数 名：vPrintPack()
    功能描述：打印报文包
    返回说明：
        无
 **************************************************************************************************/
void    vPrintPack()
{
    size_t        i, lCount = 0;
    CtsPack        *pstPack = NULL;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lExportTable(CTS_PACK_CONFIG, &lCount, (void **)&pstPack))
    {
        fprintf(stderr, "获取平台报文配置信息失败, err:(%s)\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "   ------------------------------------------------------------------------"
        "--------------------------------------------------------------------------------------"
        "--------------\n");
    fprintf(stdout, "   line  编号 引用 格式 节点 父级 序号 起始 结束 常值 定义 PSET PIDX"
        "  属性      取值标签            标签描述      CONST     变量池名称       字段处理函数     "    
        " 定义函数       |\n");
    fprintf(stdout, "   ------------------------------------------------------------------------"
        "--------------------------------------------------------------------------------------"
        "--------------\n");
    for(i = 0; i < lCount; i ++)
    {
        fprintf(stdout, " [%3d]:  %4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%6s|%16s|%20s"
            "|%5s|%16s|%18s|%18s|\n", i, pstPack[i].m_lMid, pstPack[i].m_lQid, pstPack[i].m_lType, 
            pstPack[i].m_lChild, pstPack[i].m_lIsParant, pstPack[i].m_lSeq, pstPack[i].m_lFrom, 
            pstPack[i].m_lLen, pstPack[i].m_lIsConst, pstPack[i].m_lUser, pstPack[i].m_lPlSet, 
            pstPack[i].m_lPlIdx, pstPack[i].m_szAttr, pstPack[i].m_szLabel, pstPack[i].m_szMark, 
            pstPack[i].m_szConst, pstPack[i].m_szPlName, pstPack[i].m_szExFunc, 
            pstPack[i].m_szUserFunc);
    }
    IBPFree(pstPack);

    fprintf(stdout, "   ------------------------------------------------------------------------"
        "--------------------------------------------------------------------------------------"
        "--------------\n");

    return ;
}

/**************************************************************************************************
    函 数 名：vPrintSystem()
    功能描述：打印系统核心配置
    返回说明：
        无
 **************************************************************************************************/
void    vPrintSystem()
{
	CtsParam    stParam;
    long        lLen = 0;
    char        szPrint[512];
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    memset(szPrint, 0, sizeof(szPrint));
    if(RC_SUCC != lSyncParam(pstSavm, &stParam))
    {
        fprintf(stderr, "获取系统配置失败, 可能还未初始化!\n");
        return ;
    }

    lLen = snprintf(szPrint, sizeof(szPrint), "%-8s|%s|%s|%8d|", stParam.m_szSysName, 
        stParam.m_szCrDate, stParam.m_szTxDate, stParam.m_lSeqNo);

    fprintf(stdout, "--------------------------------------------------当前系统核心参数如下---"
            "----------------------------------------------\n");
    fprintf(stdout, "系统名   清算日期 交易日期 流水序号 平台状态 最大容量 监控间隔 队列监控 "
        "超时监控 日志级别 列程比例     启动时间\n");
    fprintf(stdout, "--------------------------------------------------------------------------"
            "---------------------------------------------\n");
    if(SYSTEM_PLT_START == stParam.m_lState)
        lLen += snprintf(szPrint + lLen , sizeof(szPrint) - lLen, "  正常  |");
    else if(SYSTEM_PLT_STOP == stParam.m_lState)
        lLen += snprintf(szPrint + lLen , sizeof(szPrint) - lLen, "  停止  |");
    else if(SYSTEM_PLT_PAUSE == stParam.m_lState)
        lLen += snprintf(szPrint + lLen , sizeof(szPrint) - lLen, "  暂停  |");
    else if(SYSTEM_PLT_BING == stParam.m_lState)
        lLen += snprintf(szPrint + lLen , sizeof(szPrint) - lLen, "  BOOT  |");
    else
        lLen += snprintf(szPrint + lLen , sizeof(szPrint) - lLen, "%8d|", stParam.m_lState);

    snprintf(szPrint + lLen , sizeof(szPrint) - lLen, "%8d|%8d|%8d|%8d|%8d|%8d|%s|", 
        stParam.m_lPoolNum, stParam.m_lScanTime, stParam.m_lQueueTime, stParam.m_lTRTime,
        stParam.m_lLogLevel, stParam.m_lQPRatio, stParam.m_szUpTime); 

    fprintf(stdout, "%s\n", szPrint);
    fprintf(stdout, "--------------------------------------------------------------------------"
            "---------------------------------------------\n\n");

    return ;
}

/**************************************************************************************************
    函 数 名：vShowProcess()
    功能描述：打印当前启动进程信息
    返回说明：
        无
 **************************************************************************************************/
void    vShowProcess()
{
    char        szPrint[1024];
    CtsProcess    *pstProcess = NULL;
    size_t        i = 0, lCount = 0, lReal;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lExportTable(CTS_PLAT_PROCESS, &lCount, (void **)&pstProcess))
    {
        fprintf(stderr, "获取平台进程运行信息失败, err:(%s)\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "   ---------------------------------------------------------"
        "---------------------------------------\n");
    fprintf(stdout, "   line    OFST 执行码名称    组编号   主机  类型  组序号  进程PID"
        "  状态   被调用数  拓展类型 重连|\n");
    fprintf(stdout, "   ---------------------------------------------------------"
        "---------------------------------------\n");
    for(lReal = 0, i = 0; i < lCount; i ++)
    {
        if(BCS_APP_STOP == pstProcess[i].m_lState && BCS_EXPAND_EXT == pstProcess[i].m_lRunExt)
            continue;

        memset(szPrint, 0, sizeof(szPrint));
        snprintf(szPrint, sizeof(szPrint), "   [%3d]:  %03d| %-13s|%7d|%5d|%5d|%7d|%8d|", 
            ++ lReal, pstProcess[i].m_lIdx, pstProcess[i].m_szPGName, pstProcess[i].m_lGrpIdx, 
            pstProcess[i].m_lHost, pstProcess[i].m_lType, pstProcess[i].m_lSeq, pstProcess[i].m_lPid);

        if(BCS_APP_IDLE == pstProcess[i].m_lState)    
            strcat(szPrint, " IDLE |");
        else if(BCS_APP_BUSY == pstProcess[i].m_lState)
            strcat(szPrint, " BUSY |");
        else if(BCS_APP_STOP == pstProcess[i].m_lState)
            strcat(szPrint, " STOP |");
        else if(BCS_APP_DUMP == pstProcess[i].m_lState)
            strcat(szPrint, " DUMP |");
        else
            strcat(szPrint, " NAN? |");

        sprintf(szPrint + strlen(szPrint), "%10d|", pstProcess[i].m_lDoNum);

        if(BCS_EXPAND_NML == pstProcess[i].m_lRunExt)    
            strcat(szPrint, " NORMAL |");
        else if(BCS_EXPAND_MON == pstProcess[i].m_lRunExt)
            strcat(szPrint, " MONITOR|");
        else if(BCS_EXPAND_CHD == pstProcess[i].m_lRunExt)
            strcat(szPrint, " CHILD  |");
        else if(BCS_EXPAND_EXT == pstProcess[i].m_lRunExt)
            strcat(szPrint, " EXTEND |");
        else
            strcat(szPrint, " ?NAN?  |");

        if(pstProcess[i].m_bDbRcon > 0)    
            strcat(szPrint, "  Y |");
        else 
            strcat(szPrint, "  N |");

        fprintf(stdout, "%s\n", szPrint);
    }
    IBPFree(pstProcess);

    fprintf(stdout, "   ---------------------------------------------------------"
        "---------------------------------------\n");

    return ;
}

/**************************************************************************************************
    函 数 名：vShowAsynIdx()
    功能描述：打印当前启动进程信息
    返回说明：
        无
 **************************************************************************************************/
void    vShowAsynIdx()
{
    CtsAsyn    stAsyn;
    long    lIdx = 0, lRet;
    SATvm    *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_ASYN_IDENTIFY;
    pstSavm->lSize   = sizeof(CtsAsyn);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
    {
        IBPerror("定义表共享池游标失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "   ------------------------------------------------------------------"
        "------------------------\n");
    fprintf(stdout, "   line      L                索引            登记日期  登记流水号 位置"
        " 流程  备注  登记时间\n");
    fprintf(stdout, "   ------------------------------------------------------------------"
        "------------------------\n");

    while(1)
    {
        memset(&stAsyn, 0, sizeof(CtsAsyn));
        lRet = lTableFetch(pstSavm, CTS_ASYN_IDENTIFY, (void *)&stAsyn);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "获取异步标识表数据失败, err:(%d)(%s)\n", pstSavm->m_lErrno,
                sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        fprintf(stdout, "   [%04d]:  %30s|%8s|%10d|%4d|%4d|%5d|%10d|\n", ++ lIdx, 
            stAsyn.m_szIdx, stAsyn.m_szCrDate, stAsyn.m_lSeqNo, stAsyn.m_uData, 
            stAsyn.m_lFlow, stAsyn.m_lResv, stAsyn.m_lIdxTime);
    }
    vTableClose(pstSavm);
    fprintf(stdout, "   ------------------------------------------------------------------"
        "------------------------\n");
    return ;
}

/*************************************************************************************************
    调试信息
 *************************************************************************************************/
/*************************************************************************************************
    打印平台登记进程组
 *************************************************************************************************/
void    vPrintGroup()
{
    CtsGroup    stGroup;
    long        lRet, lIdx = 0;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_GROUP;
    pstSavm->lSize   = sizeof(CtsGroup);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
    {
        fprintf(stderr, "打开进程组配置表失败, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "------------------------------------------------------------------"
        "-------------------------------------------------------------------\n");
    fprintf(stdout, "line\t  执行码         进程组  主机  类型  当前个数 最少启动"
        " 最大启动 自动拓展  消息队列  状态  位置 重连 重启 启动时间\n");
    fprintf(stdout, "------------------------------------------------------------------"
        "-------------------------------------------------------------------\n");
    while(1)
    {
        memset(&stGroup, 0, sizeof(CtsGroup));
        lRet = lTableFetch(pstSavm, CTS_PLAT_GROUP, (void *)&stGroup);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "查询进程组配置表错误, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        fprintf(stdout, "[%d]:\t%-15s|%7d|%5d|%5d|%9d|%8d|%8d|%8d|%10d|%5d|%5d|", 
            lIdx ++, stGroup.m_szPGName, stGroup.m_lGrpIdx, stGroup.m_lHost, stGroup.m_lType, 
            stGroup.m_lCurNum, stGroup.m_lMinNum, stGroup.m_lMaxNum, stGroup.m_lAutoExt, 
            stGroup.m_lQid, stGroup.m_lState, stGroup.m_lAppPos);
        if(stGroup.m_bIsBoot > 0)    fprintf(stdout, " Y  |");
        else                        fprintf(stdout, " N  |");

        if(stGroup.m_bDbRcon > 0)
            fprintf(stdout, " Y |%14s|\n", stGroup.m_szTime);
        else
            fprintf(stdout, " N |%14s|\n", stGroup.m_szTime);
        fflush(stdout);
    }

    vTableClose(pstSavm);
    fprintf(stdout, "------------------------------------------------------------------"
        "-------------------------------------------------------------------\n");

    return ;
}

/*************************************************************************************************
    打印当前系统配置的主机参数
 *************************************************************************************************/
void    vPrintHost()
{
    CtsHost        stHost;
    long        lRet, lIdx = 0;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_TRADE_HOST;
    pstSavm->lSize   = sizeof(CtsHost);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
    {
        fprintf(stderr, "打开容器配置表失败, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "------------------------------------------------------------------"
        "----------------------------------------------------------------------------\n");
    fprintf(stdout, "line\t主机 T        名称       执行码              ip        监听  "
        "超时  客户端FTP port   FTPuser   FTPpasswd 长链 时间  签到名  签到密码\n");
    fprintf(stdout, "------------------------------------------------------------------"
        "----------------------------------------------------------------------------\n");
    while(1)
    {
        memset(&stHost, 0, sizeof(CtsHost));
        lRet = lTableFetch(pstSavm, CTS_TRADE_HOST, (void *)&stHost);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "查询容器配置表错误, 原因:%s", sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        fprintf(stdout, "[%d]:\t%4d|%d|%25s|%12s|%10s|%5d|%4d|%16s|%4d|%10s|%10s|%4d|"
            "%4d|%8s|%8s|\n", lIdx ++, stHost.m_lHost, stHost.m_lType, stHost.m_szName, 
            stHost.m_szPGName, stHost.m_szHostIp, stHost.m_lHostPort, stHost.m_lTimeOut, 
            stHost.m_szFtpIp, stHost.m_lFtpPort, stHost.m_szFtpUser, stHost.m_szFtpPwd, 
            stHost.m_bLTLink, stHost.m_lRCTime, stHost.m_szAuthUser, stHost.m_szAuthPwd);
    }
    vTableClose(pstSavm);
    fprintf(stdout, "------------------------------------------------------------------"
        "----------------------------------------------------------------------------\n");

    return ;
}

/*************************************************************************************************
    打印当前系统组件配置信息
 *************************************************************************************************/
void    vPrintClass()
{
    CtsClass    stClass;
    long        lRet, lIdx = 0;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_TRADE_CLASS;
    pstSavm->lSize   = sizeof(CtsClass);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
    {
        fprintf(stderr, "打开组件配置表失败, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "----------------------------------------------------------------"
        "---------------------------------\n");
    fprintf(stdout, "line\t属主机  组件ID 请求报文 响应报文 错误报文     类名"
        "    子方法名         方法描述\n");
    fprintf(stdout, "----------------------------------------------------------------"
        "---------------------------------\n");
    while(1)
    {
        memset(&stClass, 0, sizeof(CtsClass));
        lRet = lTableFetch(pstSavm, CTS_TRADE_CLASS, (void *)&stClass);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "查询组件配置表错误, 原因:%s", sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        fprintf(stdout, "[%d]:\t%6d|%7d|%8d|%8d|%8d|%10s|%14s|%21s|\n", lIdx ++, stClass.m_lHost, 
            stClass.m_lBuss, stClass.m_lInMid, stClass.m_lOutMid, stClass.m_lErrMid, 
            stClass.m_szClass, stClass.m_szMethod, stClass.m_szMark); 
    }

    vTableClose(pstSavm);
    fprintf(stdout, "----------------------------------------------------------------"
        "---------------------------------\n");

    return ;
}

/*************************************************************************************************
    打印当前系统提供服务配置信息
 *************************************************************************************************/
void    vPrintService()
{
    CtsService    stService;
    char        szPrint[1024];
    long        lRet, lIdx = 0;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_TRADE_SERVICE;
    pstSavm->lSize   = sizeof(CtsService);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
    {
        fprintf(stderr, "打开服务配置表失败, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "----------------------------------------------------------------------"
        "----------------------------\n");
    fprintf(stdout, "line\t属主机 服务ID   服务代码         服务名称     "
            "状态   超时  日志级别   类型   日志名\n");
    fprintf(stdout, "----------------------------------------------------------------------"
        "----------------------------\n");
    while(1)
    {
        memset(&stService, 0, sizeof(CtsService));
        lRet = lTableFetch(pstSavm, CTS_TRADE_SERVICE, (void *)&stService);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "查询服务配置表错误, err:%s\n", sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        memset(szPrint, 0, sizeof(szPrint));
        snprintf(szPrint, sizeof(szPrint), "[%d]:\t%6d|%6d|%10s|%-24s|", lIdx ++, stService.m_lHost, 
            stService.m_lSvrID, stService.m_szTrCode, stService.m_szSvrName);
        if(STATUES_ENABLE == stService.m_lState)
            strcat(szPrint, " 启用 |");
        else
            strcat(szPrint, " 禁用 |");
        sprintf(szPrint + strlen(szPrint), "%4d|", stService.m_lTimeOut);    
        switch(stService.m_lLogLevel)
        {
        case 1:
            strcat(szPrint, "LOG_DETAIL|");
            break;
        case 2:
            strcat(szPrint, "LOG_DEBUG |");
            break;
        case 3:
            strcat(szPrint, "LOG_INFO  |");
            break;
        case 4:
            strcat(szPrint, "LOG_TRACE |");
            break;
        case 5:
            strcat(szPrint, "LOG_MOST  |");
            break;
        default:
            strcat(szPrint, "  --NAN-- |");
            break;
        }    

        sprintf(szPrint + strlen(szPrint), "%5d|%s|", stService.m_lSvrType, 
            stService.m_szLogName); 
        fprintf(stdout, "%s\n", szPrint);
    }

    vTableClose(pstSavm);
    fprintf(stdout, "----------------------------------------------------------------------"
        "----------------------------\n");

    return ;
}

/*************************************************************************************************
    打印当前系统服务对应的流程
 *************************************************************************************************/
void    vPrintFlow()
{
    CtsFlow     stFlow;
    long        lRet, lIdx = 0, i = 0;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_TRADE_FLOW;
    pstSavm->lSize   = sizeof(CtsFlow);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
    {
        fprintf(stderr, "打开交易流程表失败, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "---------------------------------------------------------------------"
        "---------------------------------------------------\n");
    fprintf(stdout, "line\t服务 流程 类型 主机 组件 InMid OutMid ErrMid 个数 异步 下级流程 "
        "    类名        子方法名    主机执行码   表达式\n");
    fprintf(stdout, "---------------------------------------------------------------------"
        "---------------------------------------------------\n");
    while(1)
    {
        memset(&stFlow, 0, sizeof(CtsFlow));
        lRet = lTableFetch(pstSavm, CTS_TRADE_FLOW, (void *)&stFlow);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "查询交易流程表错误, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        fprintf(stdout, "[%d]:\t%4d|%4d|%4d|%4d|%4d|%5d|%6d|%6d|%4d|%4d|%10s|%10s|%14s|%12s|%6d|\n", 
            lIdx ++, stFlow.m_lSvrID, stFlow.m_lFlow, stFlow.m_lType, stFlow.m_lHost, 
            stFlow.m_lBuss, stFlow.m_lInMid, stFlow.m_lOutMid, stFlow.m_lErrMid, stFlow.m_lNext, 
            stFlow.m_lAsyn, sGetFlowStr(stFlow.m_lNext, stFlow.m_lNextFlow), stFlow.m_szClass, 
            stFlow.m_szMethod, stFlow.m_szPGName, stFlow.m_lExp);
    }
    vTableClose(pstSavm);
    fprintf(stdout, "---------------------------------------------------------------------"
        "---------------------------------------------------\n");

    return ;

}

/*************************************************************************************************
    打印当前系统内外服务码定义
 *************************************************************************************************/
void    vPrintTrCode()
{
    CtsTrCode    stCode;
    long        lRet, lIdx = 0;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_TRADE_CODE;
    pstSavm->lSize   = sizeof(CtsTrCode);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))    
    {
        fprintf(stderr, "打开交易表失败, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "--------------------------------------------------------------------------\n");
    fprintf(stdout, "line\t序号  标识            接入码             接入子码      平台交易码\n");
    fprintf(stdout, "--------------------------------------------------------------------------\n");
    while(1)
    {
        memset(&stCode, 0, sizeof(CtsTrCode));
        lRet = lTableFetch(pstSavm, CTS_TRADE_CODE, (void *)&stCode);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "查询交易码表错误, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        fprintf(stdout, "[%d]:\t%4d|%5s|%-25s|%15s|%12s|\n", lIdx ++, stCode.m_lIdx, 
            stCode.m_szUse, stCode.m_szInCode, stCode.m_szInSCode, stCode.m_szTrCode); 
    }
    vTableClose(pstSavm);
    fprintf(stdout, "--------------------------------------------------------------------------\n");

    return ;
}

/*************************************************************************************************
    打印当前启动进程信息
 *************************************************************************************************/
void    vPrintProcess()
{
    CtsProcess    stProcess;
    long        lRet, lIdx = 0;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_PROCESS;
    pstSavm->lSize   = sizeof(CtsProcess);
    if(RC_SUCC != lTableDeclare(pstSavm, pstSavm->tblName))
    {
        fprintf(stderr, "打开进程运行表失败, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "---------------------------------------------------------"
        "----------------------\n");
    fprintf(stdout, "line   OFST     执行码名称  组号 容器 类型 组号  PID  "
        " 状态 处理  调用数  拓展\n");
    fprintf(stdout, "---------------------------------------------------------"
        "----------------------\n");
    while(1)
    {
        memset(&stProcess, 0, sizeof(CtsProcess));
        lRet = lTableFetch(pstSavm, CTS_PLAT_PROCESS, (void *)&stProcess);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "查询进程运行表错误, 原因:%s\n", sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        fprintf(stdout, "[%d]:\t%3d|%-15s|%4d|%4d|%4d|%4d|%6d|%4d|%4d|%8d|%4d|\n", ++ lIdx,
            stProcess.m_lIdx, stProcess.m_szPGName, stProcess.m_lGrpIdx, stProcess.m_lHost, 
            stProcess.m_lType, stProcess.m_lSeq, stProcess.m_lPid, stProcess.m_lState, 
            stProcess.m_lPend, stProcess.m_lDoNum, stProcess.m_lRunExt);
    }
    vTableClose(pstSavm);
    fprintf(stdout, "---------------------------------------------------------"
        "----------------------\n");

    return ;
}

/**************************************************************************************************
    函 数 名：vPrintParam(char *pszOpt)
    功能描述：显示系统各参数信息
    返回说明：
        无
 **************************************************************************************************/
void    vPrintParam(char *pszOpt)
{
    long    lLen = 0, i = 0;
    SATvm    *pstSavm = (SATvm *)pGetSATvm();    

    if(!pszOpt)    return ;

    if(!bIsTvmBoot())
    {
        fprintf(stderr, "\n平台未启动!\n\n");
        return ;
    }

    for(i = 0, lLen = strlen(pszOpt); i < lLen; i ++)
    {
        switch(pszOpt[i])
        {
        case 'g':
        case 'G':
            vPrintGroup();
            break;
        case 'p':
        case 'P':
            vPrintProcess();
            break;
        case 'h':
        case 'H':
            vPrintHost();
            break;
        case 'c':
        case 'C':
            vPrintClass();
            break;
        case 's':
        case 'S':
            vPrintService();
            break;
        case 'f':
        case 'F':
            vPrintFlow();
            break;
        case 'r':
        case 'R':
            vPrintTrCode();
            break;
        case 't':
        case 'T':
            vShowProcess();
            break;
        case 'i':
        case 'I':
            vShowAsynIdx();
            break;
        case 'y':
        case 'Y':
            vPrintSystem();
            break;
        case 'k':
        case 'K':
            vPrintPack();
            break;
        default:
            break;
        }
    }

    return ;
}

/*************************************************************************************************
    code end
 *************************************************************************************************/
