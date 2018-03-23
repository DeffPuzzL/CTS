/**************************************************************************************************
    文 件 名：event.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：cts系统事件驱动
 **************************************************************************************************/
#include    "face.h"

/**************************************************************************************************
    cts 系统所有事件驱动函数
 **************************************************************************************************/

/**************************************************************************************************
     函 数 名：_lInPutRequest(SATvm *pstSavm, void *pvData, long lLen, long lRsv, char *, char *)
    功能描述：外部系统接入函数
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lInPutRequest(SATvm *pstSavm, HHCMIE cm, void *pvData, long lLen, long lRsv, char *pszSvrc, 
			char *pszSubc, Benum em)
{
	GCts      *pstCts   = (GCts *)pCmieGloble(cm);
	CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);
    
    if(!pvData || lLen <= 0)
    {
        SYSTrror(cm, RET_SYSTEM, "无效的交易请求数据");
        return RC_FAIL;
    }
    
    if(lGetMaxQueue() <= lGetQueueNum(pstSavm, lGetToQueue()))
    {
        SYSTrror(cm, RET_SYSTEM, "业务超限(%d)，请稍后再试", pstSavm->m_lErrno);
        return RC_FAIL;
    }

	if(!bIsSysNormal(pstSavm))
	{
        SYSTrror(cm, RET_SYSTEM, "系统已暂停服务, 请稍后再试！");
		return RC_FAIL;
	}

    memset(pstCts, 0, sizeof(GCts));
    pstCts->m_lLaunch = em;
    pstEvent->m_lResv = lRsv;
    if(RC_SUCC != lGetIdleProcess(pstSavm, CTS_CORE, lGetToPos(), lGetToNum(), &pstEvent->m_lMType))
    {
        SYSTrror(cm, RET_SYSTEM, "调度进程(%s)组失败, %s", CTS_CORE, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    vFillTime(pstCts);
    vSetSuccess(pstCts);
//  pstCts->m_lInHost = 0;
    pstCts->m_lTimeOut= 60;
    pstCts->m_lResv   = lRsv;
    pstCts->m_lInPid  = getpid();
    pstCts->m_lInQid  = lGetInQueue();
    pstCts->m_lPid    = pstEvent->m_lMType;
    strncpy(pstCts->m_szInCode, pszSvrc, sizeof(pstCts->m_szInCode));
    strncpy(pstCts->m_szInSCode, pszSubc, sizeof(pstCts->m_szInSCode));
//  vSetLogName("%s.log", pstCts->m_szInCode);

    if(RC_SUCC != lInitTrans(pstSavm, pstCts, &pstEvent->m_uData, pvData, lLen))
    {
        SYSTrror(cm, RET_SYSTEM, "%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    pstEvent->m_lFlow  = CTS_FLOW_BEGIN;
    pstEvent->m_lState = EVENT_EVINMSG;
    pstEvent->m_lSeqNo = pstCts->m_lSeqNo;
    if(RC_SUCC != lEventWrite(pstSavm, lGetToQueue(), pstEvent, sizeof(CtsEvent)))
    {
        SYSTrror(cm, RET_SYSTEM, "发送交易(%d)流水调度信息失败, err:%s", pstEvent->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno))
        lResetTrans(pstSavm, cm);
        return RC_FAIL;
    }
    
    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：lInPutRequest(SATvm *pstSavm, void *pvData, long lLen, long lRevfd, char *, char *)
    功能描述：外部系统接入函数(需要平台处理完毕后同步返回响应)
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInPutRequest(SATvm *pstSavm, void *pvData, long lLen, long lRsv, char *pszSvrc, char *pszSubc)
{
    return _lInPutRequest(pstSavm, pGetCmie(), pvData, lLen, lRsv, pszSvrc, pszSubc, LAUNCH_BY_NORMAL);
}

/**************************************************************************************************
     函 数 名：lAsPutRequest(SATvm *pstSavm, void *pvData, long lLen, long lRsv, char *, char *)
    功能描述：外部系统接入函数(异步接入，不需要平台处理完毕后同步响应)
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lAsPutRequest(SATvm *pstSavm, void *pvData, long lLen, long lRsv, char *pszSvrc, char *pszSubc)
{
    return _lInPutRequest(pstSavm, pGetCmie(), pvData, lLen, lRsv, pszSvrc, pszSubc, LAUNCH_BY_NRMLAS);
}

/**************************************************************************************************
     函 数 名：lInGetRespond(SATvm *pstSavm, GCts *pstCts, CtsProcess *pstProcess, CtsEvent *pstEvent, 
                void *pszOut, long *plLen)
    功能描述：外部系统接收平台处理响应
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInGetRespond(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, char *pszOut, long *plLen)
{
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

	IBTresetpool(cm);
    vLogSetName(cm, "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);

    //  接收该队列所有消息
    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_IDLE))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, err:%s", pstProcess->m_lPid,
            sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    //    接收该队列所有消息
    pstEvent->m_lMType = getpid();    //    获取本进程的任务
    if(RC_SUCC != lEventRead(pstSavm, lGetInQueue(), pstEvent, sizeof(CtsEvent), 
        pstEvent->m_lMType))
    {
        IBTerror(cm, "获取队列(%d)中调度消息失败, err:%s", lGetInQueue(), 
            sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    if(IBPevexit(pstEvent))
    {
        IBTrace(cm, "进程(%s)(%d)收到退出信号，直接退出", sGetProgram(), getpid());
        lExitProcess(pstProcess, true);
        exit(RC_SUCC);
    }
    else if(IBPevcache(pstEvent))
        return lEventCache(pstSavm);

    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_BUSY))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, err:%s", pstProcess->m_lPid,
            sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    *plLen = lGetUserSize();
    if(IBPevhasyn(pstEvent))
    {
        if(RC_SUCC != lGetGlobleCts(pstSavm, cm, TRUE))
        {
            IBTerror(cm, "获取交易(%d)运行交换参数失败, err:%s", pstEvent->m_lSeqNo,
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        pstEvent->m_lResv = pstCts->m_lResv;
        if(IBPstaterr(pstCts->m_lState))
            pstCts->m_lOutMid = pstCts->m_lErrMid > 0 ? pstCts->m_lErrMid : pstCts->m_lOutMid;
        IBTsysm(cm, "交易(%d)半同步，组织响应报文(%d), 数据如下:", pstCts->m_lSeqNo, 
            pstCts->m_lOutMid);

        if(RC_SUCC != lPackWriteAt(pstSavm, cm, pstCts->m_lOutMid, pszOut, plLen))
        {
            IBTerror(cm, "获取交易(%d)运行交换参数失败, err:%s", pstEvent->m_lSeqNo,
                sGetTvmMsg(pstSavm));
            return RC_FAIL;
        }

        IBTcem(cm, pszOut, *plLen);
        return RC_SUCC;
    }

    if(RC_SUCC != lGetDrivePack(pstSavm, cm, pszOut, plLen))
    {
        IBTerror(cm, "获取交易(%d)运行交换参数失败, err:%s", pstEvent->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        lResetTrans(pstSavm, cm);
        return RC_FAIL;
    }
    pstEvent->m_lResv = pstCts->m_lResv;

    return lResetTrans(pstSavm, cm);
}

/**************************************************************************************************
     函 数 名：_lOutGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, long lType, CtsEvent *pstEvent, 
            EXITEXEC pvFunc, void *argv)
    功能描述：业务组件接收业务请求
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lOutGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv)
{
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    vInitFileFd(cm);    
    IBTresetpool(cm);
    vLogSetName(cm, "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);

    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_IDLE))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, err:%s", getpid(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    pstEvent->m_lMType = getpid();
    if(RC_SUCC != lEventRead(pstSavm, lGetInQueue(), pstEvent, sizeof(CtsEvent), pstEvent->m_lMType))
    {
        IBTerror(cm, "获取队列(%d)中调度消息失败, err:%s", lGetInQueue(),
            sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    if(IBPevexit(pstEvent))
    {
        IBTrace(cm, "进程(%s)(%d)收到退出信号，直接退出", sGetProgram(), getpid());
        lExitProcess(pstProcess, true);
        if(pvFunc)     pvFunc(argv);
        exit(RC_SUCC);
    }
    else if(IBPevcache(pstEvent))
        return lEventCache(pstSavm, pstProcess);
    else if(IBPevasyny(pstEvent))
        return RC_SUCC;

    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_BUSY))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, %s", getpid(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    if(RC_SUCC != lGetGlobleCts(pstSavm, cm, TRUE))
    {
        IBTerror(cm, "获取交易(%d)运行交换参数失败, %s", pstEvent->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    IBTprintpool(cm);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lOutGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, CtsEvent *pstEvent)
    功能描述：业务组件接收业务请求
    返回说明：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 **************************************************************************************************/
long    lOutGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm)
{
    return _lOutGetRequest(pstSavm, pstProcess, cm, NULL, NULL);
}

/**************************************************************************************************
    函 数 名：lOutGetRequestEx(SATvm *pstSavm, CtsProcess *pstProcess, CtsEvent *pstEvent
                EXITEXEC pvFunc, void *argv)
    功能描述：业务组件接收业务请求
    返回说明：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 **************************************************************************************************/
long    lOutGetRequestEx(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv)
{
    return _lOutGetRequest(pstSavm, pstProcess, cm, pvFunc, argv);
}

/**************************************************************************************************
     函 数 名：_lAsynGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, CtsEvent *pstEvent, 
            EXITEXEC pvFunc, void *argv)
    功能描述：业务组件接收业务请求
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lAsynGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv)
{
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    vInitFileFd(cm);    
    IBTresetpool(cm);
    vLogSetName(cm, "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);
    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_IDLE))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, err:%s", getpid(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    pstEvent->m_lMType = getpid();
    if(RC_SUCC != lEventRead(pstSavm, lGetInQueue(), pstEvent, sizeof(CtsEvent), pstEvent->m_lMType))
    {
        IBPerror("获取队列(%d)中调度消息失败, err:%s", lGetInQueue(),
            sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

//    IBPdebug("收到交易(%d)响应, 共享池(%d), 流程序号(%d), 备注字段(%d), 事件标识(%d)", pstEvent->m_lSeqNo, 
//        pstEvent->m_uData, pstEvent->m_lFlow, pstEvent->m_lResv, pstEvent->m_lState);

    if(IBPevexit(pstEvent))
    {
        IBTrace(cm, "进程(%s)(%d)收到退出信号，直接退出", sGetProgram(), getpid());
        lExitProcess(pstProcess, true);
        if(pvFunc)     pvFunc(argv);        //    调用退出函数
        exit(RC_SUCC);
    }
    else if(IBPevcache(pstEvent))
        return lEventCache(pstSavm, pstProcess);

    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_BUSY))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, %s", getpid(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    return RC_SUCC;
}
/**************************************************************************************************
    函 数 名：lAsyGetRequestEx(SATvm *pstSavm, CtsProcess *pstProcess, CtsEvent *pstEvent, 
                EXITEXEC pvFunc, void *argv)
    功能描述：业务组件接收业务请求
    返回说明：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 **************************************************************************************************/
long    lAsyGetRequestEx(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv)
{
    return _lAsynGetRequest(pstSavm, pstProcess, cm, pvFunc, argv);
}

/**************************************************************************************************
    函 数 名：lAsyGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, CtsEvent *pstEvent)
    功能描述：业务组件接收业务请求
    返回说明：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 **************************************************************************************************/
long    lAsyGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm)
{
    return _lAsynGetRequest(pstSavm, pstProcess, cm, NULL, NULL);
}

/**************************************************************************************************
     函 数 名：_lOutPutResponse(SATvm *pstSavm, CtsEvent *pstEvent, GCts *pstCts)
    功能描述：业务组件发送业务处理响应
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lOutPutResponse(SATvm *pstSavm, HHCMIE cm)
{
    GCts        *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent    *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(IBPevdebug(pstEvent))
    {
        if(LAUNCH_BY_ASDEBUG == pstCts->m_lLaunch)
            return lResetTrans(pstSavm, cm);

        pstCts->m_lPid = pstCts->m_lInPid;
        if(RC_SUCC != lSetGlobleCts(pstSavm, cm, TRUE))
        {
            SYSTrror(cm, RET_SYSTEM, "更新交易(%d)共享运行参数失败, err:%s", pstEvent->m_lSeqNo, 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
        
        //    发往接入进程处理
        pstEvent->m_lState = EVENT_EVFINISH;
        pstEvent->m_lMType = pstCts->m_lInPid;    
        if(RC_SUCC != lEventWrite(pstSavm, pstCts->m_lInQid, pstEvent, sizeof(CtsEvent)))
        {
            IBTerror(cm, "获取队列(%d)中调度消息失败, err:%s", pstCts->m_lInQid,
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
        
        vLogSetName(cm, "%s/%s.log", sLogGetPath(), sGetProgram());
        return RC_SUCC;
    }
    else if(IBPevasyny(pstEvent))
    {
        vCheckFileFd(cm);    
        return RC_SUCC;
    }

    if(RC_SUCC != lGetIdleProcess(pstSavm, CTS_CORE, lGetToPos(), lGetToNum(), &pstEvent->m_lMType))
    {
        lDbsRollback();
        SYSTrror(cm, RET_SYSTEM, "调度进程(%s)组失败, %s", CTS_CORE, sGetTError(pstSavm->m_lErrno));
        IBPwarn("强制回滚当前事务");
        return RC_FAIL;
    } 

    pstCts->m_lPid = pstEvent->m_lMType;
    if(RC_SUCC != lSetGlobleCts(pstSavm, cm, TRUE))
    {
        lDbsRollback();
        SYSTrror(cm, RET_SYSTEM, "更新交易(%d)共享运行参数失败，err:%s", pstCts->m_lSeqNo, 
			sGetTError(pstSavm->m_lErrno));
        IBTwarn(cm, "强制回滚当前事务");
        return RC_FAIL;
    }

    vCheckFileFd(cm);    
    IBTrace(cm, "业务组件(%s)(%s)处理结束", pstCts->m_szCallCode, pstCts->m_szCallSub);
    if(lGetTimeout() < 0)
    {
        lDbsRollback();
        SYSTrror(cm, RET_SYSTEM, "交易(%d)在提交组件处理结果超时，回滚当前事务", pstCts->m_lSeqNo);

        //    判断流水是否已被监控托管，如果已被托管，那么这里直接返回    
        if(bIsMonDeposit(pstSavm, cm))    return RC_SUCC;
    }
    else if(IBPIserr())
    {
        IBTerror(cm, "交易(%d)处理出错，回滚当前事务", pstCts->m_lSeqNo);
        lDbsRollback();
    }

    if(RC_SUCC != lEventWrite(pstSavm, lGetToQueue(), pstEvent, sizeof(CtsEvent)))
    {
        lDbsRollback();
        SYSTrror(cm, RET_SYSTEM, "发送交易(%d)流水调度信息失败, err:%s", pstCts->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        IBTwarn(cm, "强制回滚当前事务");
        return RC_FAIL;
    }

    lDbsCommit();

    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：lCorePutResponse(SATvm *pstSavm, GCts *pstCts, CtsEvent *pstEvent)
    功能描述：core_in发送响应信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lCorePutResponse(SATvm *pstSavm, HHCMIE cm)
{
    GCts        *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent    *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(IBPIsok())
        IBTsysm(cm, "交易(%d)已正常处理结束", pstCts->m_lSeqNo);
    else
        IBTsysm(cm, "交易(%d)因错误而提前中止", pstCts->m_lSeqNo);

    //    异步调用直接清理资源
    if(LAUNCH_BY_ASCALL == pstCts->m_lLaunch || LAUNCH_BY_NRMLAS == pstCts->m_lLaunch ||
        LAUNCH_BY_HASYNC == pstCts->m_lLaunch)
        return lResetTrans(pstSavm, cm);

    pstCts->m_lPid = pstCts->m_lInPid;
    if(RC_SUCC != lSetGlobleCts(pstSavm, cm, FALSE))
    {
        SYSTrror(cm, RET_SYSTEM, "更新交易(%d)共享运行参数失败, err:%s", pstEvent->m_lSeqNo, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    //    发往接入进程处理
    pstEvent->m_lMType = pstCts->m_lInPid;    
    if(RC_SUCC != lEventWrite(pstSavm, pstCts->m_lInQid, pstEvent, sizeof(CtsEvent)))
    {
        IBTerror(cm, "获取队列(%d)中调度消息失败, err:%s", pstCts->m_lInQid,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：lCoreGetRequest(CstProcess *pstProcess, CtsEvent *pstEvent)
    功能描述：core in/out 核心所有接收请求消息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lCoreGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm)
{
    long      lRet = 0;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    IBTresetpool(cm);
    vLogSetName(cm, "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);
    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_IDLE))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, err:%s", getpid(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    pstEvent->m_lMType = getpid();
    if(RC_SUCC != lEventRead(pstSavm, lGetInQueue(), pstEvent, sizeof(CtsEvent), pstEvent->m_lMType))
    {
        IBTerror(cm, "获取队列(%d)中调度消息失败, err:%s", lGetInQueue(),
            sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    if(IBPevexit(pstEvent))
    {
        IBTrace(cm, "进程(%s)(%d)收到退出信号，直接退出", sGetProgram(), getpid());
        lExitProcess(pstProcess, true);
        exit(RC_SUCC);
    }
    else if(IBPevcache(pstEvent))
        return lEventCache(pstSavm, pstProcess);

    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_BUSY))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, %s", getpid(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    } 

    if(IBPevoutmsg(pstEvent) || IBPevinmsg(pstEvent) || IBPevfinish(pstEvent))
        lRet = lGetGlobleCts(pstSavm, cm, FALSE);
    else
        lRet = lGetGlobleCts(pstSavm, cm, TRUE);
    if(RC_SUCC != lRet)
    {
        IBTerror(cm, "获取交易(%d)运行交换参数失败, %s", pstEvent->m_lSeqNo, 
			sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(!IBPevinmsg(pstEvent))
        IBTdebug(cm, "收到交易(%d)请求, 共享池(%d), 流程序号(%d), 超时(%d)", pstEvent->m_lSeqNo, 
            pstEvent->m_uData, pstEvent->m_lFlow, pstCts->m_lTimeOut);

    if(lGetTimeout() < 0)   //  超时处理    
    {
        if(LAUNCH_BY_SYCALL == pstCts->m_lLaunch || LAUNCH_BY_ASCALL == pstCts->m_lLaunch ||
            LAUNCH_BY_HASYNC == pstCts->m_lLaunch)
            pstEvent->m_lState = EVENT_EVFINISH;
        else if(!IBPevfinish(pstEvent))
            pstEvent->m_lState = EVENT_EVOUTMSG;
        SYSTrror(cm, RET_TIMEOUT, "交易(%d)系统返回时请求进程已超时", pstCts->m_lSeqNo);
        //    虽然交易失败，但是运行调度为成功，按正常调度处理
    }
/*
    else if(IBPIserr())    //  该组件运行失败
    {
        if(LAUNCH_BY_SYCALL == pstCts->m_lLaunch || LAUNCH_BY_ASCALL == pstCts->m_lLaunch ||
            LAUNCH_BY_HASYNC == pstCts->m_lLaunch)
            pstEvent->m_lState = EVENT_EVFINISH;
        else if(!IBPevfinish(pstEvent))
            pstEvent->m_lState = EVENT_EVOUTMSG;
         IBPsysm("交易(%d)已运行失败, 处理标识(%d)响应码(%s)响应信息(%s)", pstCts->m_lSeqNo,
            pstCts->m_lState, pstCts->m_szRetCode, pstCts->m_szRetMsg);
        //    虽然交易失败，但是运行调度为成功，按正常调度处理
    }
*/

    return RC_SUCC;
}

/**************************************************************************************************
    事件驱动end
 **************************************************************************************************/
/**************************************************************************************************
     函 数 名：lGetPackRequest(SATvm *pstSavm, CstProcess *pstProcess, CtsEvent *pstEvent, 
                EXITEXEC pvFunc, void *argv)
    功能描述：获取报文处理事件
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lGetPackRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv)
{
    GCts        *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent    *pstEvent = (CtsEvent *)pCmieEvent(cm);

    IBTresetpool(cm);
    vLogSetName(cm, "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);
    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_IDLE))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, err:%s", getpid(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    pstEvent->m_lMType = getpid();    
    if(RC_SUCC != lEventRead(pstSavm, lGetInQueue(), pstEvent, sizeof(CtsEvent), pstEvent->m_lMType)) 
    {
        IBTerror(cm, "获取队列(%d)中调度消息失败, %s", lGetInQueue(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    if(IBPevexit(pstEvent))
    {
        IBPrace("进程(%s)(%d)收到退出信号，直接退出", sGetProgram(), getpid());
        lExitProcess(pstProcess, true);
        if(pvFunc)    pvFunc(argv);      //  调用退出函数
        exit(RC_SUCC);
    }
    else if(IBPevcache(pstEvent))
        return lEventCache(pstSavm, pstProcess);

    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_BUSY))
    {
        IBTerror(cm, "设置进程(%d)运行状态失败, %s", getpid(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    } 

    if(!IBPevinmsg(pstEvent) && !IBPevoutmsg(pstEvent))
    {    
        IBTerror(cm, "无效解析/组装报文标识(%d)", pstEvent->m_lState);
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lGetPackRequest(SATvm *pstSavm, CtsEvent *pstEvent)
    功能描述：发送报文处理完毕事件
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lGetPackRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm)
{
	return _lGetPackRequest(pstSavm, pstProcess, cm, NULL, NULL);
}

/**************************************************************************************************
    函 数 名：lGetPackRequestEx(SATvm *pstSavm, CtsEvent *pstEvent)
    功能描述：发送报文处理完毕事件
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lGetPackRequestEx(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv)
{
	return _lGetPackRequest(pstSavm, pstProcess, cm, pvFunc, argv);
}

/**************************************************************************************************
     函 数 名：lPutPackResponse(SATvm *pstSavm, HHCMIE cm)
    功能描述：发送报文处理完毕事件
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lPutPackResponse(SATvm *pstSavm, HHCMIE cm)
{
    long      lRet;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(IBPevinmsg(pstEvent))
    {
        if(IBPstatok(pstCts->m_lState))    //    只有解析到变量池正常才设置变量池标识
            pstEvent->m_lState = EVENT_EVPOOL;
        IBTinfo(cm, "解析交易(%d)请求报文处理结束", pstCts->m_lSeqNo);

        lRet = lEventWrite(pstSavm, lGetToQueue(), pstEvent, sizeof(CtsEvent));
    }
    else if(IBPevoutmsg(pstEvent))
    {
        pstEvent->m_lState = EVENT_EVFINISH;
        IBTinfo(cm, "组织交易(%d)响应报文处理结束", pstCts->m_lSeqNo);

        pstEvent->m_lMType = pstCts->m_lInPid;
        lRet = lEventWrite(pstSavm, pstCts->m_lInQid, pstEvent, sizeof(CtsEvent));
    }

    if(RC_SUCC != lRet)
    {
        IBTerror(cm, "发送交易(%d)流水调度信息失败, err:%s", pstCts->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：lCallPackExch(SATvm *pstSavm, CtsEvent *pstEvent, BOOL bBuild)
    功能描述：通知报文组件开始处理
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lCallPackExch(SATvm *pstSavm, HHCMIE cm, BOOL bBuild)
{
    long      lQueue = 0;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(!IBPevinmsg(pstEvent) && !IBPevoutmsg(pstEvent))
        return RC_SUCC;

    if(IBPevoutmsg(pstEvent))
    {
        //  LAUNCH_BY_NRMLAS 通常在接入完之后立刻返收到技术性应答，然后不需要再次返回响应
        //  直接在这里回收资源
        if(LAUNCH_BY_ASCALL == pstCts->m_lLaunch || LAUNCH_BY_NRMLAS == pstCts->m_lLaunch)      
        {
            IBTsysm(cm, "交易(%d)设置为正常发起异步, 已正常处理结束", pstCts->m_lSeqNo);
            return lResetTrans(pstSavm, cm);
        }
        else if(LAUNCH_BY_HASYNC == pstCts->m_lLaunch)
        {
            IBTsysm(cm, "交易(%d)设置为半同步交易, 正常处理结束", pstCts->m_lSeqNo);
            return lResetTrans(pstSavm, cm);
        }

        IBTsysm(cm, "组织主机(%d)进程(%d)服务(%s)(%s)响应", pstCts->m_lInHost, pstCts->m_lInPid, 
            pstCts->m_szTrCode, pstCts->m_szSvrName);
    }

    if(RC_SUCC != lGetIdleProcess(pstSavm, CTS_MSGEX, lGetToPos(), lGetToNum(), &pstEvent->m_lMType))
    {
        SYSTrror(cm, RET_SYSTEM, "调度进程(%s)组失败, %s", CTS_MSGEX, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    pstCts->m_lPid = pstEvent->m_lMType;
    if(RC_SUCC != lSetGlobleCts(pstSavm, cm, bBuild))
    {
        SYSTrror(cm, RET_SYSTEM, "更新交易(%d)共享运行参数失败，err:%s", pstCts->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    //    核心流程中，无需判断，只针对业务进程判断
    if(RC_FAIL >= (lQueue = lGetQueueNum(pstSavm, lGetToQueue())))
    {
        SYSTrror(cm, RET_SYSTEM, "获取交易(%d)队列(%d)参数失败, err:%s", pstCts->m_lSeqNo,
            lGetToQueue(), sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(lGetMaxQueue() <= lQueue)
    {
        IBTerror(cm, "流水(%d)发往进程(%s)(%d)队列(%d)超限，请稍后再试(%d)(%d)", pstCts->m_lSeqNo, 
			CTS_MSGEX, pstEvent->m_lMType, lGetToQueue(), lGetMaxQueue(), lQueue);
        SYSTrror(cm, RET_SYSTEM, "业务超限，请稍后再试");
        return RC_FAIL;
    }

//    IBPrace("发送进程(%s)(%d)队列数据", stGroup.m_szPGName, stGroup.m_lQid);
    if(RC_SUCC != lEventWrite(pstSavm, lGetToQueue(), pstEvent, sizeof(CtsEvent)))
    {
        SYSTrror(cm, RET_SYSTEM, "发送交易(%d)流水调度信息失败, err:%s", pstCts->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：_lLaunchEvent(SATvm *pstSavm, GCts *pstNcs, long lSeqNo, long lOfs, Benum eType)
    功能描述：同步调用交易并获取交易结果
        流水号            --提前
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lLaunchEvent(SATvm *pstSavm, HHCMIE cm, HHCMIE ce, Benum eType, BOOL bPool)
{
    long      lQueue, i;
    GCts      *pstCts   = (GCts *)pCmieGloble(ce);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(ce);

    if(RC_FAIL >= (lQueue = lGetQueueNum(pstSavm, lGetToQueue())))
    {
        IBTerror(cm, "获取交易(%d)队列(%d)参数失败, %s", pstEvent->m_lSeqNo, lGetToQueue(), 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(lGetMaxQueue() <= lQueue)
    {
        IBTerror(cm, "流水(%d)发往进程(%s)(%d)队列(%d)超限，请稍后再试(%d)(%d)", pstEvent->m_lSeqNo, 
			CTS_CORE, pstEvent->m_lMType, lGetToQueue(), lGetMaxQueue(), lQueue);
        return RC_FAIL;
    }

    if(RC_SUCC != lEventWrite(pstSavm, lGetToQueue(), pstEvent, sizeof(CtsEvent)))
    {
        IBTerror(cm, "发送交易(%d)调度失败, %s", pstEvent->m_lSeqNo, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    
    if(LAUNCH_BY_ASCALL == eType)    //    异步直接返回
    {
        IBTsysm(cm, "交易(%s)异步发起系统交易(%s)流水号(%d)调用成功", pstCts->m_szInCode,
              pstCts->m_szTrCode, pstCts->m_lSeqNo);
        return RC_SUCC;
    }

    IBTsysm(cm, "交易(%s)同步发起系统交易(%s)流水号(%d)调用成功", pstCts->m_szInCode,
        pstCts->m_szTrCode, pstCts->m_lSeqNo);

    pstEvent->m_lMType   = pstCts->m_lInPid;
    //    多等待2s， 超过2秒，肯定是超时了
    for(i = 0, IBTresetpool(cm); i < (2 + pstCts->m_lTimeOut) * 20; i ++, usleep(50000))
    {
        if(RC_SUCC != lReadNoWait(pstSavm, lGetInQueue(), pstEvent, sizeof(CtsEvent), 
            pstEvent->m_lMType))
        {
            IBTerror(cm, "获取队列(%d)中事件消息失败, err:%s", lGetInQueue(), 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        if(EAGAIN == errno || ENOMSG == errno)      //  无消息
            continue;
        break;
    }

    if(EAGAIN == errno || ENOMSG == errno)      //  无消息
    {
        lGetGlobleCts(pstSavm, ce, FALSE);
        SYSTrror(ce, RET_SYSTEM, "交易(%s)流水号(%d)超过超时(%d)未响应", 
			pstCts->m_szTrCode, pstCts->m_lSeqNo, pstCts->m_lTimeOut);
        return lResetTrans(pstSavm, ce);
    }

    if(bPool) 
    {
        if(RC_SUCC != lGetGlobleCts(pstSavm, ce, TRUE))
        {
            IBTerror(cm, "获取交易(%d)运行交换参数失败, err:%s", pstEvent->m_lSeqNo, 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        vCombinePool(cm, &ce->m_Roop);
    }
    else
    {
        if(RC_SUCC != lGetGlobleCts(pstSavm, ce, FALSE))
        {
            IBTerror(cm, "获取交易(%d)运行交换参数失败, err:%s", pstEvent->m_lSeqNo, 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }

    //    如果是半同步，由流程结束自行清理
    if(LAUNCH_BY_HASYNC == pstCts->m_lLaunch)
        return RC_SUCC;

    return lResetTrans(pstSavm, ce);
}

/**************************************************************************************************
     函 数 名：lAsynDispatch(SATvm *pstSavm, CtsEvent *pstEvent, CtsFlow *pstNext)
    功能描述：异步调用指定单个模块
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lAsynDispatch(SATvm *pstSavm, HHCMIE cm, CtsFlow *pstNext)
{
    long      lQueue;
    CtsGroup  stGroup;
    CtsEvent  stEvent;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    memcpy(&stEvent, pstEvent, sizeof(CtsEvent));
    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pstNext->m_szPGName, pstNext->m_lHost))
    {
        SYSTrror(cm, RET_SYSTEM, "获取进程(%s)配置信息失败, err:%s", pstNext->m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lGetIdleProcess(pstSavm, pstNext->m_szPGName, stGroup.m_lAppPos, 
        stGroup.m_lMaxNum, &stEvent.m_lMType))
    {
        SYSTrror(cm, RET_SYSTEM, "调度进程(%s)组失败, %s", pstNext->m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    IBTsysm(cm, "异步调用主机(%3d)服务(%s)(%s)\t\t进程：%s->%d", pstCts->m_lHost, 
        pstCts->m_szTrCode, pstCts->m_szSvrName, pstNext->m_szPGName, stEvent.m_lMType);

    stEvent.m_lState = EVENT_EVASYNY;    //    仅仅调起, 不保证正确性
    if(RC_SUCC != lEventWrite(pstSavm, stGroup.m_lQid, &stEvent, sizeof(CtsEvent)))
    {
        SYSTrror(cm, RET_SYSTEM, "发送交易(%d)流水调度信息失败, %s", pstCts->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：lDispatchMethod(SATvm *pstSavm, CtsEvent *pstEvent, CtsFlow *pstFlow)
    功能描述：通知组件开始运行
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lDispatchMethod(SATvm *pstSavm, HHCMIE cm)
{
    CtsFlow   stNext;
    CtsGroup  stGroup;
    CtsEvent  stEvent;
    long      lRet = 0, lQueue;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(!IBPevpool(pstEvent))
        return RC_SUCC;    

    if(RC_SUCC != lGetNextFlow(pstSavm, cm, &stNext, &lRet))
        return RC_FAIL;

    if(CTS_FLOW_END == stNext.m_lFlow)
    {
        pstCts->m_lUseTime = lGetUnixTime() - pstCts->m_lInTime;
        pstEvent->m_lFlow  = stNext.m_lFlow;
        if(IBPIsok())
        {
            lRet = RC_SUCC;
            IBTsysm(cm, "主机(%d)发起的交易(%s)，流程正常结束, 耗时(%d)", pstCts->m_lInHost, 
                pstCts->m_szTrCode, pstCts->m_lUseTime);
        }
        else
        {
            lRet = RC_FAIL;
            IBTsysm(cm, "主机(%d)发起的交易(%s)，因错误而提前中止", pstCts->m_lInHost, 
                pstCts->m_szTrCode);
        }

        // 平台交易自身发起的，不需要响应报文, 直接返回
        if(LAUNCH_BY_SYCALL == pstCts->m_lLaunch || LAUNCH_BY_ASCALL == pstCts->m_lLaunch ||
            LAUNCH_BY_HASYNC == pstCts->m_lLaunch)
	    {
            pstEvent->m_lState = EVENT_EVFINISH;
            return RC_SUCC;
	    }
        else
        {
            pstEvent->m_lState = EVENT_EVOUTMSG;
            return lRet;
        }
    }
    else if(EXP_CONST_KEPRUN != lRet && IBPIserr())
    {
        if(LAUNCH_BY_SYCALL == pstCts->m_lLaunch || LAUNCH_BY_ASCALL == pstCts->m_lLaunch ||
            LAUNCH_BY_HASYNC == pstCts->m_lLaunch)
            pstEvent->m_lState = EVENT_EVFINISH;
        else if(!IBPevfinish(pstEvent))
            pstEvent->m_lState = EVENT_EVOUTMSG;

        IBTsysm(cm, "交易(%d)已运行失败, 处理标识(%d)响应码(%s)响应信息(%s)", pstCts->m_lSeqNo,
            pstCts->m_lState, pstCts->m_szRetCode, pstCts->m_szRetMsg);
        return RC_SUCC;
    }

    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, stNext.m_szPGName, stNext.m_lHost))
    {
        SYSTrror(cm, RET_SYSTEM, "获取进程(%s)配置信息失败, err:%s", stNext.m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lGetIdleProcess(pstSavm, stNext.m_szPGName, stGroup.m_lAppPos, 
        stGroup.m_lMaxNum, &pstEvent->m_lMType))
    {
        SYSTrror(cm, RET_SYSTEM, "调度进程(%s)组失败, %s", stNext.m_szPGName, 
			sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    //    继续调用进程来处理
    pstCts->m_lPid = pstEvent->m_lMType;
    pstCts->m_lLastState = pstCts->m_lState;
    strcpy(pstCts->m_szLastRet, pstCts->m_szRetCode);
    strcpy(pstCts->m_szLastMsg, pstCts->m_szRetMsg);
    pstCts->m_lState = STATUS_OK;
    strcpy(pstCts->m_szRetCode, RET_SUCCESS);
    strcpy(pstCts->m_szRetMsg, MSG_SUCCESS);

    pstCts->m_lToMid    = stNext.m_lInMid;
    pstCts->m_lToInMid  = stNext.m_lOutMid;
    pstCts->m_lToErrMid = stNext.m_lErrMid;
    pstCts->m_lHost     = stNext.m_lHost;
    pstEvent->m_lFlow   = stNext.m_lFlow;
    strncpy(pstCts->m_szCallCode, stNext.m_szClass, sizeof(pstCts->m_szCallCode));
    strncpy(pstCts->m_szCallSub, stNext.m_szMethod, sizeof(pstCts->m_szCallSub));
    IBTsysm(cm, "继续调用主机(%3d)服务(%s)(%s)\t\t方法：%s->%s", pstCts->m_lHost, 
        pstCts->m_szTrCode, pstCts->m_szSvrName, stNext.m_szClass, stNext.m_szMethod);

    if(FLOW_HALF_SYNCHR == stNext.m_lAsyn)
    {
        pstCts->m_lLaunch = LAUNCH_BY_HASYNC;    
        if(stNext.m_lExp == 0)
            lRet = lSetGlobleCts(pstSavm, cm, FALSE);
        else
            lRet = lSetGlobleCts(pstSavm, cm, TRUE);
        if(RC_SUCC != lRet)
        {
            SYSTrror(cm, RET_SYSTEM, "更新交易(%d)共享运行参数失败, err:%s", pstEvent->m_lSeqNo,
                  sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        memcpy(&stEvent, pstEvent, sizeof(stEvent));
        stEvent.m_lState = EVENT_EVHASYN;        //    半同步
        stEvent.m_lMType = pstCts->m_lInPid;
        if(RC_SUCC != lEventWrite(pstSavm, pstCts->m_lInQid, &stEvent, sizeof(CtsEvent)))
        {
            IBTerror(cm, "获取队列(%d)中调度消息失败, %s", pstCts->m_lInQid,
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }
    else
    {
        if(stNext.m_lExp == 0)
            lRet = lSetGlobleCts(pstSavm, cm, FALSE);
        else
            lRet = lSetGlobleCts(pstSavm, cm, TRUE);
        if(RC_SUCC != lRet)
        {
            SYSTrror(cm, RET_SYSTEM, "更新交易(%d)共享运行参数失败, %s", pstEvent->m_lSeqNo,
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }

    //  核心流程中，无需判断，只针对业务进程判断
    if(RC_FAIL >= (lQueue = lGetQueueNum(pstSavm, stGroup.m_lQid)))
    {                                                                          
        SYSTrror(cm, RET_SYSTEM, "获取交易(%d)队列(%d)参数失败, %s", pstCts->m_lSeqNo,
            stGroup.m_lQid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(lGetMaxQueue() <= lQueue)
    {
        IBPerror("流水(%d)发往业务进程(%s)(%d)队列(%d)超限，请稍后再试(%d)(%d)", pstCts->m_lSeqNo, 
            stNext.m_szPGName, pstEvent->m_lMType, stGroup.m_lQid, lGetMaxQueue(), lQueue);
        SYSTrror(cm, RET_SYSTEM, "业务超限，请稍后再试");
        return RC_FAIL;
    }

    if(RC_SUCC != lEventWrite(pstSavm, stGroup.m_lQid, pstEvent, sizeof(CtsEvent)))
    {
        SYSTrror(cm, RET_SYSTEM, "发送交易(%d)流水调度信息失败, err:%s", pstCts->m_lSeqNo,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：_lTraceMethod(SATvm *pstSavm, GCts *pstCts, char *pszPGName, char *pszClass, 
                char *pszMethod)
    功能描述：单步测试组件
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lTraceMethod(SATvm *pstSavm, HHCMIE cm, char *pszPGName, char *pszClass, char *pszMethod, 
			Benum eType)
{
    CtsClass   stClass;
    CtsGroup   stGroup;
    long       lQueue = 0;
    GCts       *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent   *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(!pszPGName || !pszClass || !pszMethod) 
    {
        IBTerror(cm, "调试参数不能为空");
        return RC_FAIL;
    }

    vFillTime(pstCts);    
    vSetSuccess(pstCts);
    pstCts->m_lInPid  = lGetMaxPid() + getpid();
    pstCts->m_lInQid  = lGetInQueue();
    pstCts->m_lTimeOut= 60;
    pstCts->m_lResv   = pstEvent->m_lResv;      //  备注字段
    pstCts->m_lLaunch = eType;
    strncpy(pstCts->m_szCallCode, pszClass, sizeof(pstCts->m_szCallCode));
    strncpy(pstCts->m_szCallSub, pszMethod, sizeof(pstCts->m_szCallSub));
    if(RC_SUCC != lGetClass(pstSavm, &stClass, pszClass, pszMethod))
    {
        IBPerror("获取组件(%s)(%s)失败, %s", pszClass, pszMethod, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

	if(0x00 == cm->m_szLog[0])
	{
    	snprintf(pstCts->m_szLogName, sizeof(pstCts->m_szLogName), "%s.log", pszPGName);
    	vLogSetName(cm, "%s/%s.log", sLogGetPath(), pstCts->m_szLogName);
	}
	else
		strncpy(pstCts->m_szLogName, cm->m_szLog, sizeof(pstCts->m_szLogName));

    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pszPGName, stClass.m_lHost))
    {
        IBPerror("获取进程(%s)配置信息失败, %s", pszPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lGetIdleProcess(pstSavm, pszPGName, stGroup.m_lAppPos, stGroup.m_lMaxNum, 
		&pstEvent->m_lMType))
    {
        IBPerror("调度进程(%s)组失败, %s", pszPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    pstCts->m_lPid = pstEvent->m_lMType;
    if(RC_SUCC != lInitTrans(pstSavm, pstCts, &pstEvent->m_uData, "", 0))
        return RC_FAIL;

    pstEvent->m_lState = EVENT_EVDEBUG;
    pstEvent->m_lMType = pstCts->m_lInPid;
    pstEvent->m_lSeqNo = pstCts->m_lSeqNo;
    pstEvent->m_lFlow  = CTS_FLOW_BEGIN;
    if(RC_SUCC != lSetGlobleCts(pstSavm, cm, TRUE))
    {
        SYSTrror(cm, RET_SYSTEM, "更新交易(%d)共享运行参数失败，%s", pstCts->m_lSeqNo, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    //  核心流程中，无需判断，只针对业务进程判断
    if(RC_FAIL >= (lQueue = lGetQueueNum(pstSavm, stGroup.m_lQid)))
    {                                                                          
        IBPerror("获取交易(%d)队列(%d)参数失败, err:%s", pstCts->m_lSeqNo,
            stGroup.m_lQid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(lGetMaxQueue() <= lQueue)
    {
        IBPerror("流水(%d)发往业务进程(%s)(%d)队列(%d)超限，请稍后再试(%d)(%d)", pstCts->m_lSeqNo, 
            pszPGName, pstEvent->m_lMType, stGroup.m_lQid, lGetMaxQueue(), lQueue);
        return RC_FAIL;
    }

    if(RC_SUCC != lEventWrite(pstSavm, stGroup.m_lQid, pstEvent, sizeof(CtsEvent)))
    {
        IBPerror("发送交易(%d)流水调度信息失败, err:%s", pstCts->m_lSeqNo, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(LAUNCH_BY_ASDEBUG == eType)    return RC_SUCC;
/*
    for(lTime; (!bIsFinish(stEvent.m_lSeqNo, stEvent.m_uData)) && lTime < 120; lTime ++)
        usleep(50000);
*/
    pstEvent->m_lMType = pstCts->m_lInPid;
    if(RC_SUCC != lEventRead(pstSavm, lGetInQueue(), pstEvent, sizeof(CtsEvent), pstEvent->m_lMType))
    {
        lResetTrans(pstSavm, cm);
        IBPerror("获取队列(%d)中调度消息失败, err:%s", lGetInQueue(), sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    
    if(RC_SUCC != lGetGlobleCts(pstSavm, cm, TRUE))
    {
        lResetTrans(pstSavm, cm);
        IBPerror("获取交易(%d)交换参数失败, %s", pstEvent->m_lSeqNo, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return lResetTrade(pstSavm, cm);
}

/**************************************************************************************************
     函 数 名：lTraceMethod(SATvm *pstSavm, GCts *pstCts, char *pszPGName, char *pszClass, 
                char *pszMethod)
    功能描述：单步测试组件
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lTraceMethod(SATvm *pstSavm, HHCMIE cm, char *pszPGName, char *pszClass, char *pszMethod)
{
    return _lTraceMethod(pstSavm, cm, pszPGName, pszClass, pszMethod, LAUNCH_BY_SYDEBUG);
}

/**************************************************************************************************
     函 数 名：lAyCallMethod(SATvm *pstSavm, GCts *pstCts, char *pszPGName, char *pszClass, 
                char *pszMethod)
    功能描述：单步测试组件
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lAyCallMethod(SATvm *pstSavm, HHCMIE cm, char *pszPGName, char *pszClass, char *pszMethod)
{
    return _lTraceMethod(pstSavm, cm, pszPGName, pszClass, pszMethod, LAUNCH_BY_ASDEBUG);
}

/**************************************************************************************************
     函 数 名：_lMonitorEvent(SATvm *pstSavm, CtsProcess *pstProcess)
    功能描述：监控进程事件驱动
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lMonitorEvent(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv)
{
	CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    IBTresetpool(cm);
    memset(pstEvent, 0, sizeof(CtsEvent));
    vLogSetName(cm, "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);

    errno = 0;
    pstEvent->m_lMType = getpid();
    if(RC_SUCC != lReadNoWait(pstSavm, lGetInQueue(), pstEvent, sizeof(CtsEvent), 
		pstEvent->m_lMType))
    {
        IBTerror(cm, "获取队列(%d)消息失败, %s", lGetInQueue(), sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    //    IPC_NOWAIT 但是返回EAGAIN, 无消息
    if(EAGAIN == errno || ENOMSG == errno)
        return RC_SUCC;

    if(IBPevexit(pstEvent))
    {
        IBTrace(cm, "进程(%s)(%d)收到退出信号，直接退出", sGetProgram(), getpid());
        lExitProcess(pstProcess, true);
		if(pvFunc)     pvFunc(argv);
        exit(RC_SUCC);
    }
    else if(IBPevcache(pstEvent))
        return lEventCache(pstSavm, pstProcess);

    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：_lMonitorEvent(SATvm *pstSavm, CtsProcess *pstProcess)
    功能描述：监控进程事件驱动
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lMonitorEvent(SATvm *pstSavm, CtsProcess *pstProcess)
{
	return _lMonitorEvent(pstSavm, pstProcess, pGetCmie(), NULL, NULL);
}

/**************************************************************************************************
    函 数 名：lMonitorEventEx(SATvm *pstSavm, CtsProcess *pstProcess, EXITEXEC pvFunc, void *argv)
    功能描述：监控进程事件驱动
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lMonitorEventEx(SATvm *pstSavm, CtsProcess *pstProcess, EXITEXEC pvFunc, void *argv)
{
	return _lMonitorEvent(pstSavm, pstProcess, pGetCmie(), pvFunc, argv);
}

/**************************************************************************************************
    函 数 名：vExpressParse(GCts *pstCts, long lExp, Express *pstExp, long *plRet)
    功能描述：表达式处理
    返回说明：
        TRUE            --满足
        FALSE            --不满足
 **************************************************************************************************/
void	vExpressParse(HHCMIE cm, long lExpress, Express *p)
{
	char      *q;
    register  long   i, n;

    for(i = 0; i < lExpress; i ++)
    {    
        switch(p[i].m_lConst)
        {
        case EXP_CONST_STRING:
            strncpy(pCmieGloble(cm)->m_szParam, p[i].m_szResult, TARGET_VALUE_LEN);
            break;    
        case EXP_CONST_STRSET:
            if(2 == p[i].m_lField)
                IBTgetdefstring(cm, p[i].m_szField[1] + 2, p[i].m_szResult, "");
			IBTputstring(cm, p[i].m_szField[0] + 2, p[i].m_szResult);
            break;
        case EXP_CONST_STRTOL:
            IBPgetdefstring(p[i].m_szField[0] + 2, p[i].m_szResult, "");
			IBTputlong(cm, p[i].m_szField[0] + 2, atol(p[i].m_szResult));
            break;
        case EXP_CONST_STRCPY:
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szResult, "");
            else
                strcpy(p[i].m_szResult, p[i].m_szField[1]);
			IBTputstring(cm, p[i].m_szField[0] + 2, p[i].m_szResult);
            break;
        case EXP_CONST_STRCAT:
            IBPgetdefstring(p[i].m_szField[0] + 2, p[i].m_szResult, "");
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szField[1], "");
            strcat(p[i].m_szResult, p[i].m_szField[1]);
			IBTputstring(cm, p[i].m_szField[0] + 2, p[i].m_szResult);
            break;
        case EXP_CONST_SUBSTR:
            IBPgetdefstring(p[i].m_szField[0] + 2, p[i].m_szResult, "");
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szField[1], "");
			if(0x00 == p[i].m_szField[1][0])
				break;
            p[i].m_lField = strlen(p[i].m_szResult);
			n = atol(p[i].m_szNotMatch);
			if((TARGET_VALUE_LEN - p[i].m_lField) < n)
				n = TARGET_VALUE_LEN - p[i].m_lField; 
			strncpy(p[i].m_szResult + p[i].m_lField, p[i].m_szField[1] + atol(p[i].m_szIsMatch), n);
			IBTputstring(cm, p[i].m_szField[0] + 2, p[i].m_szResult);
            break;
		case EXP_CONST_DECODE:
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szField[1], "");
			if(0x00 == p[i].m_szField[1][0])
				break;
			if(NULL == (q = strstr(p[i].m_szIsMatch, p[i].m_szField[1])))
				break;

            p[i].m_lField = lGetRecordNum(p[i].m_szIsMatch, ":", q - p[i].m_szIsMatch) + 1;
			strncpy(p[i].m_szResult, sGetValueByIdx(p[i].m_szNotMatch, ":", p[i].m_lField), 
                TARGET_VALUE_LEN);
			IBTputstring(cm, p[i].m_szField[0] + 2, p[i].m_szResult);
            break;
        default:
            break ;
        }
    }

	return ;
}

/**************************************************************************************************
     函 数 名：bExpressCheck(HHCMIE cm, long lExp, Express *pstExp, long *plRet)
    功能描述：表达式处理
    返回说明：
        TRUE            --满足
        FALSE            --不满足
 **************************************************************************************************/
BOOL    bExpressCheck(HHCMIE cm, long lExpress, Express *p, long *plRet)
{
	char		*q;
    register    long    i, n;
    BOOL        bRet = true;

    for(i = 0; i < lExpress; i ++)
    {    
        switch(p[i].m_lConst)
        {
        case EXP_CONST_CONDIT:
        case EXP_CONST_STRCMP:
            if(!memcmp(p[i].m_szField[0], "$$", 2))
                IBPgetdefstring(p[i].m_szField[0] + 2, p[i].m_szField[0], "");
            if(!memcmp(p[i].m_szField[1], "$$", 2))
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szField[1], "");
            if(!strcmp(p[i].m_szField[0], p[i].m_szField[1]))
                bRet = (BOOL)atol(p[i].m_szIsMatch);
            else
                bRet = (BOOL)atol(p[i].m_szNotMatch);
            break;
        case EXP_CONST_KEPRUN:
            *plRet = EXP_CONST_KEPRUN;
            break;    
        case EXP_CONST_STRING:
            strncpy(pCmieGloble(cm)->m_szParam, p[i].m_szResult, TARGET_VALUE_LEN);
            break;    
        case EXP_CONST_STRSET:
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szResult, "");
            break;
        case EXP_CONST_STRTOL:
            IBPgetdefstring(p[i].m_szField[0] + 2, p[i].m_szResult, "");
            break;
        case EXP_CONST_STRCPY:
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szResult, "");
            else
                strcpy(p[i].m_szResult, p[i].m_szField[1]);
            break;
        case EXP_CONST_STRCAT:
            IBPgetdefstring(p[i].m_szField[0] + 2, p[i].m_szResult, "");
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szField[1], "");
            strcat(p[i].m_szResult, p[i].m_szField[1]);
            break;
        case EXP_CONST_SUBSTR:
            IBPgetdefstring(p[i].m_szField[0] + 2, p[i].m_szResult, "");
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szField[1], "");
			if(0x00 == p[i].m_szField[1][0])
				break;
			
            p[i].m_lField = strlen(p[i].m_szResult);
			n = atol(p[i].m_szNotMatch);
			if((TARGET_VALUE_LEN - p[i].m_lField) < n)
				n = TARGET_VALUE_LEN - p[i].m_lField; 
			strncpy(p[i].m_szResult + p[i].m_lField, p[i].m_szField[1] + atol(p[i].m_szIsMatch), n);
            break;
		case EXP_CONST_DECODE:
            if(2 == p[i].m_lField)
                IBPgetdefstring(p[i].m_szField[1] + 2, p[i].m_szField[1], "");
			if(0x00 == p[i].m_szField[1][0])
				break;
			if(NULL == (q = strstr(p[i].m_szIsMatch, p[i].m_szField[1])))
				break;

            p[i].m_lField = lGetRecordNum(p[i].m_szIsMatch, ":", q - p[i].m_szIsMatch) + 1;
			strncpy(p[i].m_szResult, sGetValueByIdx(p[i].m_szNotMatch, ":", p[i].m_lField), 
                TARGET_VALUE_LEN);
            break;
        default:
            return false;
        }
    }

	// 当条件满足才对表达式其他赋值, 避免A组件的表达式进入B流程里面
    if(!bRet)    return false;

    for(i = 0; i < lExpress; i ++)
    {    
        switch(p[i].m_lConst)
        {
        case EXP_CONST_STRSET:
        case EXP_CONST_STRTOL:
        case EXP_CONST_STRCPY:
        case EXP_CONST_STRCAT:
        case EXP_CONST_SUBSTR:    
		case EXP_CONST_DECODE:
            IBPputstring(p[i].m_szField[0] + 2, p[i].m_szResult);
            break;
        default:
            break;
        }
    }

    return true;
}

/**************************************************************************************************
     函 数 名：lGetNextFlow(SATvm *pstSavm, CtsEvent *pstEvent, CtsFlow *pstFlow)
    功能描述：获取下级流程结构信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lGetNextFlow(SATvm *pstSavm, HHCMIE cm, CtsFlow *pstFlow, long *plRet)
{
    register long   i;
    CtsFlow   stFlow, stNext;
    GCts      *pstCts   = (GCts *)pCmieGloble(cm);
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(CTS_FLOW_BEGIN == pstEvent->m_lFlow)
    {
        IBTsysm(cm, "开始处理主机(%3d)发起的交易(%s)(%s)流水号(%d)日志(%d)超时(%d)标识(%d)",
            pstCts->m_lInHost, pstCts->m_szTrCode, pstCts->m_szSvrName, pstCts->m_lSeqNo, 
            pstCts->m_lLogLev, pstCts->m_lTimeOut, pstCts->m_lState);
    }
    else
    {
        IBTsysm(cm, "业务组件返回：处理标志(%d)响应码(%s)响应信息(%s)", pstCts->m_lState,
            pstCts->m_szRetCode, pstCts->m_szRetMsg);
    }

    if(RC_SUCC != lGetTradeFlow(pstSavm, &stFlow, pstCts->m_lSvrID, pstEvent->m_lFlow))
    {
        SYSTrror(cm, RET_SYSTEM, "获取交易(%d)流程(%d)(%d)配置信息失败, %s", pstEvent->m_lSeqNo,
            pstCts->m_lSvrID, pstEvent->m_lFlow, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(i = 0, *plRet = 0; i < stFlow.m_lNext; i ++, *plRet = 0)
    {
        if(RC_SUCC != lGetTradeFlow(pstSavm, &stNext, pstCts->m_lSvrID, stFlow.m_lNextFlow[i]))
        {
            SYSTrror(cm, RET_SYSTEM, "获取交易(%d)流程(%d)(%d)配置信息失败, %s", pstEvent->m_lSeqNo,
                pstCts->m_lSvrID, pstFlow->m_lFlow, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    
        if(FLOW_ENABLE_ASYN == stNext.m_lAsyn)
        {
            if(RC_SUCC != lAsynDispatch(pstSavm, cm, &stNext))
            {
                vSetRetSucc(pstCts);
                IBTwarn(cm, "异步交易(%s)服务(%d)流程(%d)失败, 继续下个流程", pstCts->m_szTrCode,
                    pstCts->m_lSvrID, stNext.m_lFlow);
            }

            continue;
        }

		if(stNext.m_lCondit)
		{
            if(bExpressCheck(cm, stNext.m_lExp, stNext.m_stExp, plRet))
            {
                memcpy(pstFlow, &stNext, sizeof(CtsFlow));
                return RC_SUCC;
            }
		}
		else
		{
            vExpressParse(cm, stNext.m_lExp, stNext.m_stExp);
            memcpy(pstFlow, &stNext, sizeof(CtsFlow));
            return RC_SUCC;
        }

        if((i + 1) == stFlow.m_lNext) 
        {
            IBTsysm(cm, "流程(%d)不满足条件，默认调用流程", stNext.m_lFlow);
            memcpy(pstFlow, &stNext, sizeof(CtsFlow));
            return RC_SUCC;
        }
    }

    SYSTrror(cm, RET_SYSTEM, "定位后续交易出错，主机(%d)发起的交易(%s)(%s)非正常结束",
        pstCts->m_lInHost, pstCts->m_szTrCode, pstCts->m_szSvrName);
    pstEvent->m_lFlow = CTS_FLOW_END;        

    return RC_FAIL;
}

/**************************************************************************************************
     函 数 名：lResetEvent(SATvm *pstSavm, CtsProcess *pstProcess, long lPid)
    功能描述：将老进程中事件替换成新的进程PID
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lResetEvent(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, long lPid)
{
    CtsGroup  stGroup;
    long      lQueue = 0;
    CtsEvent  *pstEvent = (CtsEvent *)pCmieEvent(cm);

    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pstProcess->m_szPGName, pstProcess->m_lHost))
    {
        IBTerror(cm, "获取进程(%s)配置信息失败, err:%s", pstProcess->m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_FAIL >= (lQueue = lGetQueueNum(pstSavm, stGroup.m_lQid)))
    {
        IBTerror(cm, "获取进程(%s)(%d)队列(%d)参数失败, err:%s", pstProcess->m_szPGName, 
            pstProcess->m_lPid, stGroup.m_lQid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
     }

    if(lQueue == 0)        return RC_SUCC;

    while(lQueue --)
    {
        pstEvent->m_lMType = pstProcess->m_lPid;
        errno = 0;
        if(RC_SUCC != lReadNoWait(pstSavm, stGroup.m_lQid, pstEvent, sizeof(CtsEvent), 
            pstEvent->m_lMType))
        {
            IBPcts("获取进程(%s)(%d)队列中事件(%d)失败, err:%s", pstProcess->m_szPGName, 
                pstProcess->m_lPid, stGroup.m_lQid, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        if(EAGAIN == errno || ENOMSG == errno)      //  无消息
            break;

        //    检查队列指向的交易是否有效
        if(!bIsEventValid(pstSavm, cm))
            continue;
        
        pstEvent->m_lMType = lPid;
        if(RC_SUCC != lEventWrite(pstSavm, stGroup.m_lQid, pstEvent, sizeof(CtsEvent)))
        {
            IBPcts("发送进程(%s)(%d)队列中事件(%d)失败, err:%s", pstProcess->m_szPGName, 
                lPid, stGroup.m_lQid, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }

    return RC_SUCC;
}

/**************************************************************************************************
     函 数 名：lResetQueue(SATvm *pstSavm, long lQueue)
    功能描述：清理指定队列数据
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lResetQueue(SATvm *pstSavm, long lQid)
{
    long       lQueue = 0, lCount = 0;
	CtsEvent   *pstEvent = (CtsEvent *)pGetEvent();

    if(!pstSavm)    return RC_FAIL;

    if(RC_FAIL >= (lQueue = lGetQueueNum(pstSavm, lQid)))
    {
        IBPerror("获取队列(%d)参数失败, err:%s", lQid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    while(lQueue -- > 0)
    {
        memset(pstEvent, 0, sizeof(CtsEvent));
        if(RC_SUCC != lReadNoWait(pstSavm, lQid, pstEvent, sizeof(CtsEvent), pstEvent->m_lMType))
        {
            IBPerror("获取队列(%d)中事件消息失败, err:%s", lQid, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        if(EAGAIN == errno || ENOMSG == errno)      //  无消息
            break;

        ++ lCount;
        IBPcout("event.log", "Type:%d, Seq:%d, Ofs:%d, Flow:%d, Resv:%d, State:%d", 
			pstEvent->m_lMType, pstEvent->m_lSeqNo, pstEvent->m_uData, pstEvent->m_lFlow, 
            pstEvent->m_lResv, pstEvent->m_lState);
    }

    return lCount;
}

/**************************************************************************************************
     code end
 **************************************************************************************************/
