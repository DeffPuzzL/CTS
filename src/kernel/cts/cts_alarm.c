/************************************************************************************************** 
	文 件 名：talarm.c
	代码作者：DeffPuzzL
	编写版本：V3.0.0.0
	创建日期：2016-07-14
	功能描述：交易报警
 **************************************************************************************************/
#include	"face.h"

/**************************************************************************************************
 **************************************************************************************************/
#define ALARM_INITIAL                           0
#define ALARM_DATACHG                           1
#define ALARM_RUNNING                           2

typedef	struct	__FILE_WATCH
{
	int		m_fdty;
	long	m_lOfs;	
	char	m_szIp[20];
	char	m_szLog[512];
}FileWat;

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

/**************************************************************************************************
	函 数 名：vGetRespond(void *args)
	功能描述：提高TCP接入并发接入API
	返回说明：
		RC_SUCC				--成功
		RC_FAIL				--失败
 **************************************************************************************************/
void*	vGetRespond(void *args)
{
	CtsProcess  stProcess;
	SATvm		*pstSavm = (SATvm *)pGetSATvm();

	pthread_detach(pthread_self());
	if(RC_SUCC != lGetProcess(pstSavm, &stProcess, getpid()))
	{
		IBPerror("获取进程(%d)运行信息失败, err:%s", getpid(), sGetTError(pstSavm->m_lErrno));
		return ;
	}

	while(1)
	{
		lOutGetRequest(pstSavm, &stProcess, pGetCmie());

		if(IBPevcache(pGetEvent()))
			continue;
	}

	pthread_exit(NULL);
}

/**************************************************************************************************
	函 数 名：vConvertCts(char *pszTrade, GCts *pstCts)
	功能描述：解析到pstCts结构体中
	返回说明：
		无  
 **************************************************************************************************/
void	vConvertCts(char *pszTrade, GCts *pstCts)
{
	long	n = 0;
	char	szTime[10];

	memset(szTime, 0, sizeof(szTime));
	if(!pszTrade || !strlen(pszTrade))
		return ;

	sscanf(pszTrade, "P=%d N=%d D=%8s B=%8s %[^E] E=%[^C] C=%d T=%d >>%[^,],%[^,],%[^\n]", 
		&pstCts->m_lPid, &pstCts->m_lSeqNo, pstCts->m_szCrDate, pstCts->m_szTxDate, 
		szTime, pstCts->m_szParam, &pstCts->m_lUseTime, &pstCts->m_lSType, pstCts->m_szTrCode, 
		pstCts->m_szRetCode, pstCts->m_szRetMsg);
	sTrimAll(szTime);
	strncpy(pstCts->m_szTxTime, szTime, sizeof(pstCts->m_szTxTime));
	
	memset(pstCts->m_szParam, 0, sizeof(pstCts->m_szParam));
	n = snprintf(pstCts->m_szParam, sizeof(pstCts->m_szParam), "%s ", pstCts->m_szTxDate);
	strncpy(pstCts->m_szParam + n, szTime, 2);		n += 2;
	strcat(pstCts->m_szParam + n, ":");				n += 1;
	strncpy(pstCts->m_szParam + n, szTime + 2, 2);	n += 2;
	strcat(pstCts->m_szParam + n, ":");				n += 1;
	strncpy(pstCts->m_szParam + n, szTime + 4, 2);
}

/**************************************************************************************************
	函 数 名：lReversalTrade(GCts *pstCts)
	功能描述：冲正交易
	返回说明：
		无  
 **************************************************************************************************/
long	lReversalTrade(GCts *pstCts)
{
	llSEQ       lSeqNo;
    CtsService  stService;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

	if(SVC_TRADE_COMSU != pstCts->m_lSType)
		return RC_SUCC;

    if(RC_SUCC != lGetService(pstSavm, &stService, pstCts->m_szTrCode))
    {
        SYSError(RET_SYSTEM, "获取系统服务(%s)失败, %s", pstCts->m_szTrCode,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

	if(!strlen(stService.m_szRevCode))
        return RC_SUCC;

	IBPresetpool();
	IBPputlong("TV_SeqNo", pstCts->m_lSeqNo);         // 设置冲正流水号
	IBPputstring("TV_ClearDate", pstCts->m_szCrDate); // 设置冲正日期
	if(RC_FAIL == lCallTrade(pstSavm, pGetCmie(), stService.m_szRevCode))
	{
		SYSError(RET_SYSTEM, "调用冲正交易(%s)失败, %s", stService.m_szRevCode, sGetError());
		return RC_FAIL;
	}

	return RC_SUCC;
}

/**************************************************************************************************
	函 数 名：vAnalyAlarm(FileWat *pstWat)
	功能描述：监控交易流水日志
	返回说明：
		无  
 **************************************************************************************************/
void	vAnalyAlarm(FileWat *pstWat)
{
	FILE	*fp = NULL;
	char	szLine[1024];
	GCts	*pstCts = (GCts *)pGetGloble();

	if(!(fp = fopen(pstWat->m_szLog, "rb")))
		return ;

	fseek(fp, pstWat->m_lOfs, SEEK_SET);
	
	memset(szLine, 0, sizeof(szLine));
	while(fgets(szLine, sizeof(szLine), fp))
	{
		vInitialGloble();
		vSetRetSucc(pstCts);
		vConvertCts(szLine, pstCts);
		if(!strcmp(pstCts->m_szRetCode, RET_SUCCESS))	//	交易失败
			continue;	

		pstCts->m_lState = STATUS_ERR;
		if(SVC_TRADE_NMCHK == pstCts->m_lSType || SVC_TRADE_FOCHK == pstCts->m_lSType)
			continue;

    	lReversalTrade(pstCts);
		lCallSmsModule("报警(%s)级别(%d)：交易(%s)流水(%d)接入时间(%s)耗时(%d)处理失败, 响应码"
			"(%s)(%s)", pstWat->m_szIp, pstCts->m_lSType, pstCts->m_szTrCode, pstCts->m_lSeqNo,
			pstCts->m_szParam, pstCts->m_lUseTime, pstCts->m_szRetCode, pstCts->m_szRetMsg);

		memset(szLine, 0, sizeof(szLine));
	}

	pstWat->m_lOfs = ftell(fp);
	fclose(fp);
}
  
/**************************************************************************************************
	函 数 名：vBussMonitor(char *pszLog, char *pszDate)
	功能描述：监控交易流水日志
	返回说明：
		无  
 **************************************************************************************************/
void	vBussMonitor(FileWat *pstWatch, char *pszDate, int *pnMode) 
{
	int			epoll;	
	FILE		*fp = NULL;
	FileWat		*pstWa = NULL;
	epollevt	stEvents[10], ev;
	notifyevt	*pstNotify = NULL;
	int			i, nEvents, lOfs, lLen;
	char		szBuffer[1024], *p = NULL;  

	if((epoll = epoll_create(MAX_EVENTS)) < 0)  
	{  
 		IBPerror("Create epoll failed, err:(%s)", strerror(errno));  
		return ;
	}  

	//	这里需要判断，如果监控进程异常，应该从启动时刻监控，之前没必要监控，
	//	监控之前的交易有可能存在重复日志报警
	if(ALARM_INITIAL == *pnMode)
	{
		if(NULL == (fp = fopen(pstWatch->m_szLog, "rb")))
			return ;

		fseek(fp, 0, SEEK_END);
		pstWatch->m_lOfs = ftell(fp);
		fclose(fp);
		*pnMode = ALARM_RUNNING;
	}
	else if(ALARM_DATACHG == *pnMode)
	{
		// 日志修改发生改变，新的日志里面可能已经存在记录，需要重新监控
		pstWatch->m_lOfs = 0;
		vAnalyAlarm(pstWatch);
		*pnMode = ALARM_RUNNING;
	}

//	ev.data.fd = fd;
	ev.data.ptr = (void *)pstWatch;
	ev.events = EPOLLIN|EPOLLET;  
	epoll_ctl(epoll, EPOLL_CTL_ADD, pstWatch->m_fdty, &ev);
  
	while(1)
	{
		if((nEvents = epoll_wait(epoll, stEvents, MAX_EVENTS, 1000)) < 0)  
			break;
	
		if(strcmp(pszDate, sGetDate()))		//	说明日期跳转了
		{
			*pnMode = ALARM_DATACHG;		//	文件发生改变
			return ;
		}

		for(i = 0; i < nEvents; i ++)
		{
			if(stEvents[i].events & EPOLLIN)
			{  
				pstWa = (FileWat *)stEvents[i].data.ptr;
				memset(szBuffer, 0, sizeof(szBuffer));  
				lLen = read(pstWa->m_fdty, szBuffer, sizeof(szBuffer));  
				
				for(p = szBuffer, lOfs = 0; (p - szBuffer) < lLen; p += lOfs)
				{  
					pstNotify = (notifyevt *)p;  
				
					if(pstNotify->mask & IN_CLOSE_WRITE)
						vAnalyAlarm(pstWa);
					else if(pstNotify->mask & IN_MOVE_SELF || pstNotify->mask & IN_DELETE_SELF)
					{
						close(epoll);
						return ;		//	文件删除了
					}

					lOfs = sizeof(notifyevt) + pstNotify->len;  
				}  
 			}
		}
	}
  
	close(epoll);
  
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
	pthread_t	tPid = 0;
	FileWat		stWatch;
	CtsProcess  stProcess;
	char		szDate[9];
	int			fdty, nMode = ALARM_INITIAL;
	SATvm		*pstSavm = (SATvm *)pGetSATvm();

	vPrintVersion(argv[1]);
	memset(szDate, 0, sizeof(szDate));
	memset(&stWatch, 0, sizeof(FileWat));
	memset(&stProcess, 0, sizeof(stProcess));
	strcpy(stProcess.m_szPGName, basename(argv[0]));
	if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
	{
		IBPerror("进程%s启动初始化失败!", stProcess.m_szPGName);
		return RC_FAIL;
	}

	lGetCmdString("ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d \"addr:\"", 
		stWatch.m_szIp, sizeof(stWatch.m_szIp));

	if(0 != pthread_create(&tPid, NULL, vGetRespond, NULL))
	{
		fprintf(stderr, "pthread_create error, err:%s.\n", strerror(errno));
		return RC_FAIL;
	}

	IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, CTS_VERSION);

	while(1)
	{
		sleep(1);
		vLogSetPath();
		_sGetDate(szDate, sizeof(szDate));
		snprintf(stWatch.m_szLog, sizeof(stWatch.m_szLog), "%s/%s", sLogGetPath(),
			CTS_BUSS_LOG);

		stWatch.m_fdty = inotify_init();			//	初始化
		if(-1 == (fdty = inotify_add_watch(stWatch.m_fdty, stWatch.m_szLog, IN_ALL_EVENTS)))
		{
			nMode = ALARM_DATACHG;        //  文件发生改变, 从头开始监控
			close(stWatch.m_fdty);
			continue;
		}

		vBussMonitor(&stWatch, szDate, &nMode);
		inotify_rm_watch(stWatch.m_fdty, fdty); 
		close(stWatch.m_fdty);
	}

	return RC_SUCC;
}


