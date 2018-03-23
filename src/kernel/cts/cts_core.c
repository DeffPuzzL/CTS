/*************************************************************************************************
    文 件 名：cts_core.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0
    创建日期：2016-07-14
    功能描述：cts系统核心调度
 *************************************************************************************************/
#include    "face.h"

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
    函 数 名：vCoreInEvent(CtsProcess *pstProcess)
    功能描述：核心调度主程序
    返回说明：
        无
 **************************************************************************************************/
void    vCoreInEvent(SATvm *pstSavm, CtsProcess *pstProcess)
{
    long        lRet = 0;
	HHCMIE      cm = (HHCMIE)pGetCmie();
    GCts        *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent    *pstEvent = (CtsEvent *)pCmieEvent(cm);

    vSetSysLevel(cm, LOG_TRACE);
    while(1)
    {
        if(RC_SUCC != lCoreGetRequest(pstSavm, pstProcess, cm))
            goto ERROR_OVER;

        if(IBPevcache(pstEvent)) 
            continue;

        if(RC_SUCC != lDispatchMethod(pstSavm, cm))
            goto ERROR_OVER;

        if(IBPevfinish(pstEvent))
            goto TRADE_OVER;                

        if(RC_SUCC != lCallPackExch(pstSavm, cm, FALSE))
            goto ERROR_OVER;

        continue;

ERROR_OVER:
		pstCts->m_lState = STATUS_ERR;
        pstEvent->m_lState = EVENT_EVOUTMSG;
        if(RC_SUCC != lCallPackExch(pstSavm, cm, FALSE))
            SYSError(RET_SYSTEM, "调用解析报文解析流水(%d)失败", pstEvent->m_lSeqNo);
        continue;

TRADE_OVER:
        if(RC_SUCC != lCorePutResponse(pstSavm, cm))
            IBPcrit("发送交易(%d)最终响应失败", pstEvent->m_lSeqNo);
        continue;
    }

    return ;
}

/**************************************************************************************************
    函 数 名：main
    功能描述：核心调度主程序
    返回说明：
        无
 **************************************************************************************************/
int        main(int argc, char *argv[])
{
    CtsProcess    stProcess;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    vPrintVersion(argv[1]);
    memset(&stProcess, 0, sizeof(stProcess));    
    if(RC_SUCC != lInitCoreDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", stProcess.m_szPGName);
        return RC_SUCC;
    }

    IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, g_szVersion);

    vCoreInEvent(pstSavm, &stProcess);

    return RC_SUCC;
}

/*************************************************************************************************
    code end
 *************************************************************************************************/
