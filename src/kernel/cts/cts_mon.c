/**************************************************************************************************
	文 件 名：cts_mon.c
	代码作者：DeffPuzzL
	编写版本：V3.0.0.0 
	创建日期：2016-07-14
	功能描述：cts系统接入、返回报文解析模块
 **************************************************************************************************/
#include	"face.h"

/**************************************************************************************************
	函 数 名：vPrintVersion(char *pszParm)
	功能描述：显示该应用的版本
	返回说明：
		无
 **************************************************************************************************/
void	vPrintVersion(char *pszParm)
{
	if(!pszParm)	return ;
	if(strcmp(sUpper(pszParm), "-V"))	return ;

	fprintf(stdout, "current version:%s\n", CTS_VERSION);

	exit(0);
}

/*************************************************************************************************
	函 数 名：bSystemUptime()
	功能描述：系统负载是否满足条件
	函数参数：
		true			--拓展时检查
		false			--回收时判断
	返回说明：
		无
 *************************************************************************************************/
BOOL	bSystemLoad(BOOL bf)
{
	long	lKer;
	FILE	*op = NULL;
	double	dLoad = 0.00f;
	char	szCmd[1024], szLine[256];
	
	memset(szLine, 0, sizeof(szLine));
	memset(szCmd, 0, sizeof(szCmd));

	//	针对单核不要超过0.7 0.8  则认为，系统负载低
	snprintf(szCmd, sizeof(szCmd), "uptime|awk -F \"average:\" '{print $2}'|awk -F ',' ");
	if(bf)	strcat(szCmd, "'{print $3}'");
	else	strcat(szCmd, "'{print $2}'");

	if(NULL == (op = popen(szCmd, "r")))
  		return false;

	fgets(szLine, sizeof(szLine), op);
	sTrimAll(szLine);
	sTrimCRLF(szLine);
	pclose(op);
	op = NULL;

	lKer = sysconf(_SC_NPROCESSORS_CONF);		//	CPU个数
	dLoad = atof(szLine) / lKer;

//	IBPdebug("当前系统负载(%.2f) 系统CUP个数%d", dLoad, lKer);
	if(bf && dLoad < 1.2)
		return true;
	else if(!bf && dLoad <= 0.4)
		return true;
	else 
  		return false;
}

/*************************************************************************************************
	函 数 名：lEventTimeOut(SATvm *pstSavm, long Qid, long lQueue)
	功能描述：检查队列中的事件超时
	返回说明：
		RC_SUCC		 --成功
		RC_FAIL		 --失败
 *************************************************************************************************/
long	lEventTimeOut(SATvm *pstSavm, long lQid, long lQueue)
{
    GCts       *pstCts   = (GCts *)pGetGloble();
    CtsEvent   *pstEvent = (CtsEvent *)pGetEvent();

	while(lQueue -- > 0)
	{
		vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), CTS_MON);
		memset(pstEvent, 0, sizeof(CtsEvent));
		if(RC_SUCC != lReadNoWait(pstSavm, lQid, pstEvent, sizeof(CtsEvent), pstEvent->m_lMType))
		{
			IBPerror("获取队列(%d)中事件消息失败, err:%s", lQid, sGetTError(pstSavm->m_lErrno));
			return RC_FAIL;
		}

		if(EAGAIN == errno || ENOMSG == errno)		//	无消息
			break;
		IBPrace("%d:检测到队列(%d)消息类型(%d)事件标识(%d)", lQueue, lQid, pstEvent->m_lMType, 
			pstEvent->m_lState);

		//	如果为退出信号，首先该进程不存在了，该退出信号无意义了
		if(IBPevexit(pstEvent) || IBPevreboot(pstEvent) || IBPevcache(pstEvent))
		{
			//	如果该进程存在，就扔回队列事件中去
			if(!bExistProcess(pstEvent->m_lMType))
			{
				IBPrace("忽略已消失进程(%d)功能事件(%d)", pstEvent->m_lMType, pstEvent->m_lState);
				continue;
			}	

			if(RC_SUCC != lEventWrite(pstSavm, lQid, pstEvent, sizeof(CtsEvent)))
			{
				IBPerror("发送队列(%d)进程(%d)退出信号失败, err:%s", lQid,
					pstEvent->m_lMType, sGetTError(pstSavm->m_lErrno));
				return RC_FAIL;
			}
			continue;
		}

		if(RC_SUCC != lGetGlobleCts(pstSavm, pGetCmie(), FALSE))
		{
			if(IBPevfinish(pstEvent))		//	队列虽然超时，但交易已消失
			{
				IBPrace("事件(%d)已超时，但交易运行参数(%d)已消失, 继续处理", pstEvent->m_lMType, 
					pstEvent->m_lSeqNo);
				continue;
			}

			//	如果资源已清理，队列并未清理，这里直接清理队列
			IBPerror("获取交易(%d)共享池失败, err:%s", pstEvent->m_lSeqNo, sGetTError(pstSavm->m_lErrno));
			continue;
		}

		vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), CTS_MON);
		
		//	还未超时处理, 丢给进程重新处理
		if(lGetTimeout() > 1)
		{
			IBPrace("该交易(%d)事件(%d)还未超时，继续处理(%d)>(%d)", pstEvent->m_lSeqNo, pstEvent->m_lMType,
				pstCts->m_lTimeOut, lGetUnixTime() - pstCts->m_lInTime);
			if(RC_SUCC != lEventWrite(pstSavm, lQid, pstEvent, sizeof(CtsEvent)))
			{
				IBPerror("发送队列(%d)进程(%d)事件失败, err:%s", lQid,
					pstEvent->m_lMType, sGetTError(pstSavm->m_lErrno));
				return RC_FAIL;
			}
			continue;
		}

		vSetStatus(STATUS_ERR);
		if(IBPevfinish(pstEvent))		//	已经处理完毕了
		{
			IBPrace("该交易(%d)虽处理超时(%d)，但已处理完毕, 等待接入进程(%d)处理", 
				pstEvent->m_lSeqNo, pstCts->m_lTimeOut, pstEvent->m_lMType);
			if(RC_SUCC != lEventWrite(pstSavm, lQid, pstEvent, sizeof(CtsEvent)))
			{
				IBPerror("发送队列(%d)进程(%d)事件失败, err:%s", lQid,
					pstEvent->m_lMType, sGetTError(pstSavm->m_lErrno));
				return RC_FAIL;
			}
		}
		else if(IBPevoutmsg(pstEvent) || IBPevinmsg(pstEvent))	//	如果是响应处理的问题
		{	
			//	错误码，错误信息设置	
			SYSError(RET_TIMEOUT, "交易(%d)解析/组织报文超时(%d)耗时(%d)", pstCts->m_lSeqNo,
				pstCts->m_lTimeOut, lGetUnixTime() - pstCts->m_lInTime);
			IBPerror("在解析/组织报文时超时，事件(%d)", pstEvent->m_lState);	
			if(RC_SUCC != lCorePutResponse(pstSavm, pGetCmie()))
				IBPcrit("发送交易(%d)最终响应失败", pstEvent->m_lSeqNo);
		}
		else
		{
			SYSError(RET_TIMEOUT, "交易(%d)系统返回时请求进程已超时(%d)耗时(%d)", 
				pstCts->m_lSeqNo, pstCts->m_lTimeOut, lGetUnixTime() - pstCts->m_lInTime);
			if(!IBPevfinish(pstEvent))
				pstEvent->m_lState = EVENT_EVOUTMSG;
			
			if(RC_SUCC != lCallPackExch(pstSavm, pGetCmie(), FALSE))
				IBPcrit("发送交易(%d)最终响应失败", pstEvent->m_lSeqNo);
		}	
	}

	return RC_SUCC;
}

/*************************************************************************************************
	函 数 名：lRecycleExt(SATvm *pstSavm, CtsGroup *pstGroup, CtsProcess *pstProcess, long lQPRatio)
	功能描述：对拓展的进程进行回收
	返回说明：
		RC_SUCC		 --成功
		RC_FAIL		 --失败
 *************************************************************************************************/
long	lRecycleExt(SATvm *pstSavm, CtsGroup *pstGroup, CtsProcess *pstProcess, long lQPRatio)
{
	long	  lQueue;
	CtsGroup  stGroup;

	if(BCS_EXPAND_EXT != pstProcess->m_lRunExt)
		return RC_SUCC;

//  拓展进程异常退出, 这里需要回收
	memcpy(&stGroup, pstGroup, sizeof(CtsGroup));
	if(!bExistProcess(pstProcess->m_lPid))
	{
		if(RC_SUCC != lExitProcess(pstProcess, false))
			return RC_FAIL;

		if(-- pstGroup->m_lCurNum < 0)
			pstGroup->m_lCurNum = 0;

		pstSavm->lFind   = IDX_SELECT;
		pstSavm->tblName = CTS_PLAT_GROUP;
		pstSavm->lSize   = sizeof(CtsGroup);
		pstSavm->pstVoid = (void *)&stGroup;
		if(RC_SUCC != lUpdate(pstSavm, pstGroup, true))
		{
		    IBPerror("更新进程(%s)当前个数失败, err:%s", pstGroup->m_szPGName,
		        sGetTError(pstSavm->m_lErrno));
		    return RC_FAIL;
		}

		return RC_SUCC;
	}

	if(RC_FAIL >= (lQueue = lGetQueueNum(pstSavm, pstGroup->m_lQid)))
	{
		IBPerror("获取进程(%s)队列(%d)参数失败, err:%s", pstGroup->m_szPGName,
			pstGroup->m_lQid, sGetTError(pstSavm->m_lErrno));
		return RC_FAIL;
	}

	//	如果当前任务个数只有当前进程个数的3倍的时候，任务可以释放资源了
	if(lQueue > pstGroup->m_lMinNum || !bSystemLoad(false))	//	进程还处理繁忙
		return RC_SUCC;

	if(RC_SUCC != lShrinkProcess(pstSavm, pstProcess, pstGroup))
	{
		IBPerror("进程(%d)退出失败, err:%s", pstProcess->m_lPid, sGetTError(pstSavm->m_lErrno));
		return RC_FAIL;
	}
	IBPrace("自动回收拓展进程(%s)(%d)成功", pstProcess->m_szPGName, pstProcess->m_lPid);

	pstSavm->lFind   = IDX_SELECT;
	pstSavm->tblName = CTS_PLAT_GROUP;
	pstSavm->lSize   = sizeof(CtsGroup);
	pstSavm->pstVoid = (void *)&stGroup;
	if(RC_SUCC != lUpdate(pstSavm, pstGroup, true))
	{
		IBPerror("更新进程(%s)当前个数失败, err:%s", pstGroup->m_szPGName, 
	        sGetTError(pstSavm->m_lErrno));
	    return RC_FAIL;
	}

	return RC_SUCC;
}

/*************************************************************************************************
	函 数 名：vFixedGroup(CtsGroup *pstGroup)
	功能描述：修正进程组信息
	返回说明：
		无
 *************************************************************************************************/
void	vFixedGroup(CtsGroup *pstGroup)
{
	CtsGroup    stGroup;
	size_t		lCount, i = 0;
	CtsProcess	stProcess, *pstProcess = NULL;
	SATvm		*pstSavm = (SATvm *)pGetSATvm();

	if(!pstGroup)	return ;

	memset(&stProcess, 0, sizeof(CtsProcess));
	strcpy(stProcess.m_szPGName, pstGroup->m_szPGName);

	pstSavm->lFind   = IDX_SELECT;
	pstSavm->tblName = CTS_PLAT_PROCESS;
	pstSavm->lSize   = sizeof(stProcess);
	pstSavm->pstVoid = &stProcess;
	if(RC_SUCC != lQuery(pstSavm, &lCount, (void **)&pstProcess))
	{
		IBPerror("获取平台进程(%s)信息失败, err:%s", pstGroup->m_szPGName,
			sGetTError(pstSavm->m_lErrno));
		return ;
	}

	memcpy(&stGroup, pstGroup, sizeof(CtsGroup));
	for(i = 0, pstGroup->m_lCurNum = 0; i < lCount; i ++)
	{
		if(pstProcess[i].m_lPid <= 0)
			continue;

 		if(!bExistProcess(pstProcess[i].m_lPid)) //  进程异常
  			continue;

		//  对于这类进程，如果都异常，进程状态可能在处理正常状态，必须重置stGroup.m_lCurNum,
		//  不然无法重启
		if(BCS_APP_BUSY != pstProcess[i].m_lState && BCS_APP_IDLE != pstProcess[i].m_lState)
			continue;
	
		pstGroup->m_lCurNum ++;		
	}
	IBPFree(pstProcess);

	pstSavm->lFind   = IDX_SELECT;
	pstSavm->tblName = CTS_PLAT_GROUP;
	pstSavm->lSize   = sizeof(CtsGroup);
	pstSavm->pstVoid = (void *)&stGroup;
	if(RC_SUCC != lUpdate(pstSavm, pstGroup, true))
	{
		IBPerror("更新进程(%s)当前个数(%d)失败, err:%s", pstGroup->m_szPGName,
	        sGetTError(pstSavm->m_lErrno));
	    return ;
	}

	return ;
}

/*************************************************************************************************
	函 数 名：vQueueMonitor(SATvm *pstSavm, CtsGroup *pstGroup, long lQueue)
	功能描述：对队列进行监控，对于在队列中有消息，并且长时间未处理，可能已经堵塞了
	返回说明：
		无
 *************************************************************************************************/
void	vQueueMonitor(SATvm *pstSavm, CtsGroup *pstGroup, long lQueue)
{
	long	lTime = 0, lCurrent = lGetUnixTime();
	long	lQueTime = IBPGetParam("QueueTime") - 2;

	if(lQueTime <= 0)
	{
		IBPwarn("监控队列时间设置为(%d), 不处理", lQueTime);
		return ;
	}

	if(RC_SUCC > (lTime = lQueueRcvTime(pstSavm, pstGroup->m_lQid)))
	{
		IBPerror("获取队列(%d)接收数据时间(%d)失败, err:%s", pstGroup->m_lQid, lTime, 
			sGetTError(pstSavm->m_lErrno));
		return ;
	}

	if(lCurrent == lTime)	return ;

	//	该队列还没有进程获取过该队列的消息， 不处理, 可能是废弃的队列
	IBPrace("当前(%s)队列个数(%d)时间(%d), 队列中最后处理时间(%d), 差值(%d)s", 
		pstGroup->m_szPGName, lQueue, lCurrent, lTime, lCurrent - lTime);
	if(0 == lTime)	return;	

	//	如果队列中的数据 超过lQueTime还没处理，就可能存在超时的事件，将剔除掉
	if(lCurrent - lTime < lQueTime)
		return ;

	if(RC_SUCC != lEventTimeOut(pstSavm, pstGroup->m_lQid, lQueue))
	{
		IBPerror("检查队列中(%d)中超时交易失败", pstGroup->m_lQid);	
		return ;
	}

	return ;
}

/*************************************************************************************************
	函 数 名：vAutoExtMon(SATvm *pstSavm, CtsProcess *pstProcess, long lQPRatio)
	功能描述：自动拓展监控
	返回说明：
		无
 *************************************************************************************************/
void	vAutoExtMon(SATvm *pstSavm, CtsProcess *pstProcess, long lQPRatio)
{
	CtsProcess	stProcess;
	long		lQueue, lPid; 
	size_t		lCount = 0, i;
	CtsGroup	stGroup, *pstGroup = NULL;

	memset(&stProcess, 0, sizeof(CtsProcess));
	if(RC_SUCC != lExportTable(CTS_PLAT_GROUP, &lCount, (void *)&pstGroup))
	{
		IBPerror("获取平台所有进程组失败, err:%s", sGetTError(pstSavm->m_lErrno));
		return ;
	}

	for(i = 0, lQueue = 0; i < lCount; i ++)
	{
		if(pstGroup[i].m_lQid <= 0)	continue;
		if(!strcmp(pstGroup[i].m_szPGName, pstProcess->m_szPGName))   continue;

		if(RC_FAIL >= (lQueue = lGetQueueNum(pstSavm, pstGroup[i].m_lQid)))
		{
			IBPerror("获取进程(%s)队列(%d)参数失败, err:%s", pstGroup[i].m_szPGName,
				pstGroup[i].m_lQid, sGetTError(pstSavm->m_lErrno));
			IBPFree(pstGroup);
			return ;
		}

		vFixedGroup(pstGroup);
		if(lQueue <= 0)		continue;

		//	对进程队列监控
		vQueueMonitor(pstSavm, &pstGroup[i], lQueue);

		if(pstGroup[i].m_lMaxNum <= 0)	continue;
		if(pstGroup[i].m_lAutoExt <=0)  continue;
		if(pstGroup[i].m_lCurNum >= pstGroup[i].m_lMaxNum)		//	无法拓展
			continue;

		//	对于队列个数较少，不需要处理（拓展和监控)
		if((lQueue <= pstGroup[i].m_lCurNum * lQPRatio))
			continue ;

		//	如果队列超过总数的 1 / 2 并且队列中事件个数大于进程启动个数的3倍，则开始监控
		//	if(lQueue > 0 && lGetMaxQueue() * (pstGroup->m_lCurNum / lQueue) > lQueue * 2)
		if(lGetMaxQueue() * (pstGroup[i].m_lCurNum / lQueue) > lQueue * pstGroup[i].m_lMaxNum)
			continue;

		//	如果系统负载满足，这可以拓展
		if(!bSystemLoad(true))
			continue;
		
		memset(&stProcess, 0, sizeof(CtsProcess));
		stProcess.m_lState	= BCS_APP_IDLE;
		stProcess.m_lRunExt = BCS_EXPAND_EXT;
		stProcess.m_lHost	= pstGroup[i].m_lHost;
		stProcess.m_lType	= pstGroup[i].m_lType;
		stProcess.m_lSeq	= pstGroup[i].m_lCurNum;
		stProcess.m_lGrpIdx	= pstGroup[i].m_lGrpIdx;
		memcpy(stProcess.m_szPGName, pstGroup[i].m_szPGName, sizeof(stProcess.m_szPGName) - 1);

		//	检查uptime 查看系统复杂如何
		if(RC_SUCC >= (lPid = lForkProcess(pstGroup[i].m_szPGName, &stProcess, TRUE)))
		{
			IBPerror("启动进程(%s)失败, %s\n", pstGroup[i].m_szPGName, strerror(errno));
			IBPFree(pstGroup);
			return ;
		}
		IBPrace("自动启动拓展进程(%s)(%d)成功", stProcess.m_szPGName, lPid);

		memcpy(&stGroup, &pstGroup[i], sizeof(CtsGroup));
		pstGroup[i].m_lCurNum ++;				//	每次只能拓展一个

		pstSavm->lFind   = IDX_SELECT;
		pstSavm->tblName = CTS_PLAT_GROUP;
		pstSavm->lSize   = sizeof(CtsGroup);
		pstSavm->pstVoid = (void *)&stGroup;
		if(RC_SUCC != lUpdate(pstSavm, &pstGroup[i], true))
		{
			IBPerror("更新进程(%s)当前个数失败, err:%s", pstGroup[i].m_szPGName, 
		        sGetTError(pstSavm->m_lErrno));
		    return ;
		}
	}
	IBPFree(pstGroup);

	return ;
}

/*************************************************************************************************
	函 数 名：vProcessMon(SATvm *pstSavm, CtsProcess *pstProc, long lQPRatio)
	功能描述：进程监控
	返回说明：
		无
 *************************************************************************************************/
void	vProcessMon(SATvm *pstSavm, CtsProcess *pstProc, long lQPRatio)
{
	long		lRet, lPid;
	size_t		lCount = 0, i;
	CtsGroup	stGroup, stCondt;
	CtsProcess	*pstProcess = NULL, stProcess;

	if(RC_SUCC != lUpdateProcess(pstSavm, pstProc, BCS_APP_BUSY))
	{
		IBPerror("设置进程(%d)运行状态失败, err:%s", getpid(), sGetTError(pstSavm->m_lErrno));
		return ;
	}

	if(RC_SUCC != lExportTable(CTS_PLAT_PROCESS, &lCount, (void *)&pstProcess))
	{
		IBPerror("获取平台所有进程信息失败");
		return ;
	}

	for(i = 0; i < lCount; i ++)
	{
		if(pstProcess[i].m_lPid == getpid())			//	本身进程无需检查
			continue;

		if(BCS_APP_STOP == pstProcess[i].m_lState)		//	对于指定停止的进程无需处理
			continue;

		memset(&stGroup, 0, sizeof(CtsGroup));
		if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pstProcess[i].m_szPGName, 
			pstProcess[i].m_lHost))
		{
			IBPerror("获取进程(%s)配置信息失败, err:%s", pstProcess[i].m_szPGName,
				sGetTError(pstSavm->m_lErrno));
			IBPFree(pstProcess);
			return ;
		}

		//	如果进程设置不重启且当前进程个数大于0，则不重启
		//	这样做法，即使不重启的情况下，保证进程最少也要启动一个
		if(stGroup.m_bIsBoot == 0 && stGroup.m_lCurNum > 1)
		{
			//	如果是这类进程异常退出的话，不修改进程状态那么很可能继续收到交易事件
			if(!bExistProcess(pstProcess[i].m_lPid))
			{
				if(RC_FAIL == lExitProcess(&pstProcess[i], false))
					continue;

				memcpy(&stCondt, &stGroup, sizeof(CtsGroup));
				if(-- stGroup.m_lCurNum < 0)
					stGroup.m_lCurNum = 0;
			
				pstSavm->lFind   = IDX_SELECT;
				pstSavm->tblName = CTS_PLAT_GROUP;
				pstSavm->lSize   = sizeof(CtsGroup);
				pstSavm->pstVoid = (void *)&stCondt;
				if(RC_SUCC != lUpdate(pstSavm, &stGroup, true))
				{
					IBPerror("更新进程(%s)当前个数失败, err:%s", stGroup.m_szPGName,
				        sGetTError(pstSavm->m_lErrno));
				    return ;
				}
			}

			continue;
		}

		if(BCS_EXPAND_EXT == pstProcess[i].m_lRunExt)
		{
			if(RC_SUCC != lRecycleExt(pstSavm, &stGroup, &pstProcess[i], lQPRatio))
			{
				IBPrace("回收进程(%s)(%d)失败, err:%s", pstProcess[i].m_szPGName, 
					pstProcess[i].m_lPid, sGetTError(pstSavm->m_lErrno));
				IBPFree(pstProcess);
				return ;
			}

			continue;
		}

		//	进程已存在
		if(bExistProcess(pstProcess[i].m_lPid))
			continue;

		if(BCS_APP_DUMP == pstProcess[i].m_lState)
		{
			pstProcess[i].m_lState  = BCS_APP_IDLE;
			pstProcess[i].m_lRunExt = pstProcess[i].m_lRunExt;
			if(RC_SUCC != lRefreshProcess(&pstProcess[i], pstProcess[i].m_lPid))
			{
				IBPrace("刷新进程(%s)(%d)失败, 待处理个数(%d)", pstProcess[i].m_szPGName, 
					pstProcess[i].m_lPid, lCount);
				IBPFree(pstProcess);
				return ;
			}

			//	发送短信, 无论是否成功，都不影响主流程
			IBPwarn("进程%s异常退出coredump", pstProcess[i].m_szPGName);
//			continue;
		}
	
		//	进程不存在，
		//	在再次判断进程状态，间隙可能存在用户cts -k 操作，导致进程不存在
		memset(&stProcess, 0, sizeof(stProcess));
		lRet = lGetProcess(pstSavm, &stProcess, pstProcess[i].m_lPid);
		if(RC_NOTFOUND == lRet)
			continue;
		else if(RC_SUCC != lRet)
		{
			IBPerror("获取进程(%d)运行参数信息失败, err:%s", pstProcess[i].m_lPid, 
				sGetTError(pstSavm->m_lErrno));
			IBPFree(pstProcess);
			return ;
		}
		if(BCS_APP_STOP == stProcess.m_lState)	//	这个进程已经停止了
			continue;

		lPid = lRaiseProcess(&pstProcess[i], BCS_EXPAND_MON);
		if(lPid <= RC_SUCC)	
		{
			IBPerror("启动进程失败, %s\n", strerror(errno));
			IBPFree(pstProcess);
			return ;	
		}
		IBPrace("进程(%s)(%d)已消失, 重启进程(%d)", pstProcess[i].m_szPGName, 
			pstProcess[i].m_lPid, lPid);

		//	如果存在老的进程有未处理完毕的事件，必须重置事件MType
		if(RC_SUCC != lResetEvent(pstSavm, &pstProcess[i], pGetCmie(), lPid))
		{
			IBPerror("重置进程(%s)(%d)事件失败, err:%s", pstProcess[i].m_szPGName, 
				pstProcess[i].m_lPid, sGetTError(pstSavm->m_lErrno));
			IBPFree(pstProcess);
			return ;
		}

		lCallSmsModule("系统警告：监控到进程%s已消失, 请检查代码，本次自动重启, 异常时间:%s", 
			pstProcess[i].m_szPGName, sFormatDate((time_t)lGetUnixTime()));
	}
	IBPFree(pstProcess);

	return ;
}

/**************************************************************************************************
	函 数 名：vTradeMonitor(SATvm *pstSavm, CtsProcess *pstProcess)
	功能描述：流水监控主程序
	返回说明：
		无	
 **************************************************************************************************/
void	vTradeMonitor(SATvm *pstSavm, CtsProcess *pstProcess)
{
	size_t		lOffset;
    RunTime     *pstRun = NULL;
    SHTruck     *pstTruck = NULL;
	long		lTRTime = 0, lTime;
	GCts		*pstCts = (GCts *)pGetGloble();
    CtsEvent    *pstEvent = (CtsEvent *)pGetEvent();

	lTRTime = IBPGetParam("TRTime");		//	回收超时流水默认时间
	if(lTRTime <= 0)	lTRTime = 30;		//	默认30秒

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, CTS_RUNTIME_POOL)))
	{
		IBPerror("定义表共享池游标失败, err:%s", sGetTError(pstSavm->m_lErrno));
		return ;
	}

    for(lOffset = lGetTblData(CTS_RUNTIME_POOL); lOffset <= lGetTableSize(CTS_RUNTIME_POOL);
        lOffset += lGetRowTruck(CTS_RUNTIME_POOL))
    {
        pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, lOffset);
        if(IS_TRUCK_NULL(pstTruck))
            continue;

		memcpy(pstCts, pstTruck->m_pvData, sizeof(GCts));
		if(lGetTimeout() + lTRTime > 0)			//	还未超时达到指定回收时间
			continue;

		lTime = lGetUnixTime();

		//  监控回收的交易，将使用标识设置1
		if(LAUNCH_BY_SYDEBUG == pstCts->m_lLaunch || LAUNCH_BY_ASDEBUG == pstCts->m_lLaunch)
		{
			SYSError(RET_SYSTEM, "交易处理异常");
			lResetAsyn(pstSavm, pstEvent->m_lSeqNo, pstCts->m_szCrDate);
			vLogWriteList(pstCts);

			IBPrace("用户发起的异常交易(%d)，回收处理", ((GCts *)pstTruck->m_pvData)->m_lSeqNo);
			((GCts *)pstTruck->m_pvData)->m_lLen    = 0;
			((GCts *)pstTruck->m_pvData)->m_lSeqNo  = 0;
			((GCts *)pstTruck->m_pvData)->m_lStatus = TRADE_POOL_NULL;
			SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
			continue;
		}
		else if((LAUNCH_BY_SYCALL == pstCts->m_lLaunch || LAUNCH_BY_ASCALL == pstCts->m_lLaunch ||
			LAUNCH_BY_NRMLAS == pstCts->m_lLaunch) && IBPevfinish(pstEvent) &&
			TRADE_POOL_POOL == ((GCts *)pstTruck->m_pvData)->m_lStatus)
		{
			SYSError(RET_SYSTEM, "交易处理异常");
			lResetAsyn(pstSavm, pstEvent->m_lSeqNo, pstCts->m_szCrDate);
			vLogWriteList(pstCts);

			((GCts *)pstTruck->m_pvData)->m_lLen    = 0;
			((GCts *)pstTruck->m_pvData)->m_lSeqNo  = 0;
			((GCts *)pstTruck->m_pvData)->m_lStatus = TRADE_POOL_NULL;
			SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
			IBPrace("交易本身发起的同步/异步交易(%d)缓存重置完毕", pstEvent->m_lSeqNo);
			continue;
		}
		else if(LAUNCH_BY_MONITOR == pstCts->m_lLaunch)
		{
			SYSError(RET_SYSTEM, "交易处理异常");
			lResetAsyn(pstSavm, pstEvent->m_lSeqNo, pstCts->m_szCrDate);
			vLogWriteList(pstCts);
			
			((GCts *)pstTruck->m_pvData)->m_lLen    = 0;
			((GCts *)pstTruck->m_pvData)->m_lSeqNo  = 0;
			((GCts *)pstTruck->m_pvData)->m_lStatus = TRADE_POOL_NULL;
			SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
			IBPrace("由监控发流水(%d)交易仍然失败, 重置缓存完毕", pstEvent->m_lSeqNo);
			continue;
		}

/*
		if(LAUNCH_BY_NORMAL == pstCts->m_lLaunch)
			((GCts *)pstTruck->m_pvData)->m_lLaunch = LAUNCH_BY_MONITOR;
*/

		memset(pstEvent, 0, sizeof(CtsEvent));
		pstEvent->m_lSeqNo = ((GCts *)pstTruck->m_pvData)->m_lSeqNo;
		IBPwarn("发现流水(%d)异常, 已经超时超过(%ds), 接入时间(%d)当前(%d)超时(%d), 准备处理", 
			pstEvent->m_lSeqNo, lTRTime, pstCts->m_lInTime, lTime, pstCts->m_lTimeOut);

		lCallSmsModule("交易警告：交易(%s)流水(%d)已超时, 交易发起方式(%d)超时(%d), "
			"接入渠道(%s)(%s)，接入时间:%s", pstCts->m_szTrCode, pstCts->m_lSeqNo, 
			pstCts->m_lLaunch, pstCts->m_lTimeOut, pstCts->m_szInCode, pstCts->m_szInSCode, 
			sFormatDate((time_t)pstCts->m_lInTime));
		//	超时的需要处理, 主要针对在非事件中，程序coredump引起的超时
		//	如果接入进来就超时，非事件
		//	如果是响应报文超时, 非事件
		//	如果是变量池状态超时, 非事件
		if(TRADE_POOL_INMSG == ((GCts *)pstTruck->m_pvData)->m_lStatus ||
			TRADE_POOL_OUTMSG == ((GCts *)pstTruck->m_pvData)->m_lStatus)
		{
			//	清理异步标识 
			SYSError(RET_SYSTEM, "交易处理异常");
			lResetAsyn(pstSavm, pstEvent->m_lSeqNo, pstCts->m_szCrDate);
			vLogWriteList(pstCts);
			
			((GCts *)pstTruck->m_pvData)->m_lLen    = 0;
			((GCts *)pstTruck->m_pvData)->m_lSeqNo  = 0;
			((GCts *)pstTruck->m_pvData)->m_lStatus = TRADE_POOL_NULL;
			SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
			IBPrace("流水(%d)交易缓存重置完毕", pstEvent->m_lSeqNo);
		}
		else if(TRADE_POOL_POOL == ((GCts *)pstTruck->m_pvData)->m_lStatus)
		{
			//	清理异步标识 
			lResetAsyn(pstSavm, pstEvent->m_lSeqNo, pstCts->m_szCrDate);
			//	调用组织错误响应报文, 或者直接返回错误报文
			SYSError(RET_TIMEOUT, "交易(%d)系统返回时请求进程已超时", pstCts->m_lSeqNo);

			if(LAUNCH_BY_SYCALL == pstCts->m_lLaunch)
			{
				//	本次设置结束，待进程回收，下次如果还是未完成，那么可能是启动进程coredump
				//	直接在上面判断回收, 不然buss文件可能存在2笔
				pstEvent->m_lState = EVENT_EVFINISH;
				((GCts *)pstTruck->m_pvData)->m_lLaunch = LAUNCH_BY_MONITOR;	//	该笔交易已被托管

				if(RC_SUCC != lCorePutResponse(pstSavm, pGetCmie()))
					IBPerror("发送交易(%d)最终响应失败", pstEvent->m_lSeqNo);
				continue;
			}
			else if(LAUNCH_BY_ASCALL == pstCts->m_lLaunch)
			{
				pstEvent->m_lState = EVENT_EVFINISH;
				((GCts *)pstTruck->m_pvData)->m_lLaunch = LAUNCH_BY_MONITOR;	//	该笔交易已被托管
				vLogWriteList(pstCts);

				if(IBPIsok())
					IBTsysm(pGetCmie(), "交易(%d)已正常处理结束", pstCts->m_lSeqNo);
				else
					IBTsysm(pGetCmie(), "交易(%d)因错误而提前中止", pstCts->m_lSeqNo);
				
				lResetTrade(pstSavm, pGetCmie());
				continue;
			}
			else
			{
				pstEvent->m_lState = EVENT_EVOUTMSG;
				pstEvent->m_lResv  = pstCts->m_lResv;
				((GCts *)pstTruck->m_pvData)->m_lStatus = TRADE_POOL_OUTMSG; // 重置状态,监控托管
				memcpy(pstCts, pstTruck->m_pvData, sizeof(GCts));

				//	如果组织报文模块异常，那么buss可能就存在2笔流水，暂不考虑平台进程异常
				lCallPackExch(pstSavm, pGetCmie(), FALSE);
				continue;
			}
		}
		else
			;
	}
	vTblDisconnect(pstSavm, CTS_RUNTIME_POOL);

	if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_IDLE))
	{
		IBPerror("设置进程(%d)运行状态失败, err:%s", getpid(), sGetTError(pstSavm->m_lErrno));
		return ;
	}

	return ;
}

/**************************************************************************************************
	函 数 名：main
	功能描述：进程监控主模块
	返回说明：
		RC_SUCC			--成功
		RC_FAIL			--失败
 **************************************************************************************************/
int		main(int argc, char *argv[])
{
	CtsProcess  stProcess;
	long		i, lMonTime = 0, lQPRatio = 0;
	GCts		*pstCts = (GCts *)pGetGloble();
	SATvm		*pstSavm = (SATvm *)pGetSATvm();

	vPrintVersion(argv[1]);
	memset(&stProcess, 0, sizeof(stProcess));
	if(RC_SUCC != lInitCoreDemo(pstSavm, getpid(), &stProcess))
	{
		IBPerror("进程%s启动初始化失败", stProcess.m_szPGName);
		return RC_SUCC;
	}

	if(bIsExtraBoot(stProcess.m_szPGName))
	{
		IBPerror("进程(%s)只允许启动一个", stProcess.m_szPGName);
		return RC_FAIL;
	}

	IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, g_szVersion);
	while(1)
	{
		lMonTime = IBPGetParam("ScanTime");
		if(lMonTime <= 0)	lMonTime = 30 * 2;		//	默认30秒

		for(i = 0; i < lMonTime * 20; i ++, usleep(50000))
			lMonitorEvent(pstSavm, &stProcess);

		lQPRatio = IBPGetParam("QPRatio");
		if(lQPRatio <= 0)	lQPRatio = 3;

		vLogSetPath();
		vSetSysLevel(pGetCmie(), LOG_DETAIL);
		
		vProcessMon(pstSavm, &stProcess, lQPRatio);	//	进程监控
		vAutoExtMon(pstSavm, &stProcess, lQPRatio);	//	进程拓展,队列监控
		vTradeMonitor(pstSavm, &stProcess);			//	当进程dump后，导致流水超时
	}

	return RC_SUCC;
}

/**************************************************************************************************
	code end
 **************************************************************************************************/
