/************************************************************************************************** 
    文 件 名：cts.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0
    创建日期：2016-07-14
    功能描述：cts系统操作入口
 **************************************************************************************************/
#include    "cts.h"
#include    "face.h"

/**************************************************************************************************
    函 数 名：sGetCoreVers()
    功能描述：返回cts系统版本
    返回说明：
        szVersion            --当前版本
 **************************************************************************************************/
static    char*    sGetCoreVers()
{
    int       lOfs = 0;
    struct    utsname    sinf;
    static    char    szVersion[512];

    memset(szVersion, 0, sizeof(szVersion));
    uname(&sinf);
    lOfs += sprintf(szVersion, "Compile %s %s on %s %s\n", __DATE__, __TIME__, sinf.sysname, 
        sinf.machine);    
    lOfs += sprintf(szVersion + lOfs, "Release TVM C/C++ Library Version V%s on %s %s\n", 
        TVM_VERSION, sinf.sysname, sinf.machine);
    sprintf(szVersion + lOfs, "Release CTS Version V%s For online transaction System\n", 
        CTS_VERSION);
    strcat(szVersion, "Author：DeffPuzzL\n");
    strcat(szVersion, "Mail：liuzhenhao@iboxpay.com\n");

    return szVersion;
}

/**************************************************************************************************
    函 数 名：lWakeupSystem()
    功能描述：启动cts系统
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
long    lWakeupSystem()
{
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    //    如果系统配置已启动，只需启动进程
    if(bIsTvmBoot())
    {
        if(RC_SUCC != lRebootSystem(pstSavm))
        {
            fprintf(stderr, "系统启动失败, %s\n", sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        return RC_SUCC;
    }

    //    加载系统参数配置
    if(RC_SUCC != lStartupParam(pstSavm))
        return RC_FAIL;

    //    启动平台进程
    if(RC_SUCC != lBootProcess())
    {
        fprintf(stderr, "启动配置进程失败!\n");
        return RC_FAIL;
    }

    if(RC_SUCC != lSetSysState(pstSavm, SYSTEM_PLT_START))
    {
        fprintf(stderr, "设置平台[正常]状态失败, err:%s\n\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    fprintf(stdout, "平台启动成功, completed successfully!!!\n\n");

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lShutSystem(long eType)
    功能描述：全局系统进程信息变量
    参数说明：
        eType           --停止参数标识
    返回说明：
        RC_SUCC      --成功
        RC_FAIL      --失败
 **************************************************************************************************/
long    lShutSystem(long eType)
{
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    //  设置平台状态 --禁用
    if(RC_SUCC != lSetSysState(pstSavm, SYSTEM_PLT_STOP))
    {
        fprintf(stdout, "设置平台状态失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    //  设置平台状态 --禁用
    if(RC_SUCC != lSetSysState(pstSavm, SYSTEM_PLT_PAUSE))
    {
        fprintf(stdout, "设置平台状态失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(true == bIsExistTrade())
    {
        fprintf(stdout, "平台还在正在处理的流水, 请稍等...\n");
        return RC_FAIL;
    }

    //  停止平台进程
    if(RC_SUCC != lHaltProcess())
        return RC_FAIL;

    if(!eType)  return RC_SUCC;

    //  保存流水号和账务日期
    if(RC_SUCC != lSaveSysInfo(pstSavm))
    {
        fprintf(stderr, "保存系统相关信息失败!\n");
        return RC_FAIL;
    }

    fprintf(stdout, "开始删除系统配置引擎!\n");
    if(RC_SUCC != lShutdownTvm())
    {
        fprintf(stderr, "showdown node failed, err:(%d)(%s)\n", pstSavm->m_lErrno, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    fprintf(stdout, "平台进程包括运行配置停止完毕, completed successfully!!!\n\n");

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lTradeOperate(char *pszArgv)
    功能描述：交易操作(慎用)
    返回说明：
        无
 **************************************************************************************************/
long    lTradeOperate(char *pszArgv)
{
    long        lChoices = 0;
    GCts        *pstCts = (GCts *)pGetGloble();
    SATvm       *pstSamo = (SATvm *)pGetSATvm();
	CtsEvent    *pstEvent = (CtsEvent *)pGetEvent();

    memset(pstEvent, 0, sizeof(CtsEvent));
    if(!pszArgv || !strlen(pszArgv))    //  显示流水列表
        return lExportGCts(NULL, pstCts);
    else
    {
        pstEvent->m_lSeqNo = atol(pszArgv);
        pstEvent->m_uData  = -1;
        pstCts->m_lLaunch = LAUNCH_BY_MONITOR;  //  人工监控删除
        lExportGCts(pstEvent, pstCts);
    }

    if(pstEvent->m_uData < 0)
    {
        fprintf(stdout, "清理流水(%d)已被清理\n", pstEvent->m_lSeqNo);
        return RC_SUCC;
    }

    fprintf(stdout, "\n\t警告:确认清理该笔流水(%d)?(Y/N):", pstEvent->m_lSeqNo);
    lChoices = getchar();
    if(89 != lChoices && 121 != lChoices)
        return RC_SUCC;

    if(RC_SUCC != lResetTrade(pstSamo, pGetCmie()))
        fprintf(stderr, "清理流水(%d)交易失败\n", pstEvent->m_lSeqNo);
    else
        fprintf(stdout, "\n清理流水(%d)成功, completed successfully!!!\n\n", pstEvent->m_lSeqNo);

    return lExportGCts(NULL, pstCts);
}

/**************************************************************************************************
    函 数 名：vPrintFunc(char *s)
    功能描述：cts操作功能说明
    返回说明：
        无
 **************************************************************************************************/
void    vPrintFunc(char *s)
{
    fprintf(stdout, "\nUsage:\t%s -[SWUERFTkcstwpoth][aeswd]\n", s);
    fprintf(stdout, "\t-S(A|a)\t\t--停止平台(包括系统配置参数)\n");
    fprintf(stdout, "\t-W\t\t--启动平台\n");
    fprintf(stdout, "\t-U(eswdt)\t--设置平台参数\n");
    fprintf(stdout, "\t-R\t\t--刷新系统核心参数\n");
    fprintf(stdout, "\t-F(icQ)\t\t--重置交易运行参数\n");
    fprintf(stdout, "\t-E\t\t--紧急恢复运行配置\n");
    fprintf(stdout, "\t-T(t)\t\t--手工清理异常流水(慎用)\n");
    fprintf(stdout, "\t-c(sw)\t\t--上/下线执行码\n");
    fprintf(stdout, "\t-k\t\t--重启执行码\n");
    fprintf(stdout, "\t-s\t\t--停止指定服务\n");
    fprintf(stdout, "\t-w\t\t--启动指定服务\n");
    fprintf(stdout, "\t-p(gphcsfrtiy)\t--打印系统运行信息\n");
    fprintf(stdout, "\t-o\t\t--导出系统当前运行信息\n");
    fprintf(stdout, "\t-v\t\t--打印系统版本信息\n");
    fprintf(stdout, "\t-h\t\t--打印帮助信息\n\n");
    fprintf(stdout, "\033[4m\033[45;33mDESCRIPTION\033[0m\n");
    fprintf(stdout, "\t\033[0m\033[33;40m-Fi\t\t--清理异步标识表\033[0m\n");
    fprintf(stdout, "\t\033[0m\033[33;40m-Fc\t\t--重新刷新二级缓存\033[0m\n");
    fprintf(stdout, "\t\033[0m\033[33;40m-F$Qid\t\t--清理队列现有数据\033[0m\n");
    fprintf(stdout, "\t\033[0m\033[33;40m-T -t $seqno\t--手工清理指定流水(慎用)\033[0m\n");
    fprintf(stdout, "\t\033[0m\033[33;40m-c[x] -s exe\t--对执行码下线停止操作\033[0m\n");
    fprintf(stdout, "\t\033[0m\033[33;40m-c[x] -w exe\t--对执行码上线启动操作\033[0m\n");
    fprintf(stdout, "\n");
}

/**************************************************************************************************
    函 数 名：vRunAction(long lAction, char *pszPGName, char *pszArgv, long lHost)
    功能描述：cts操作函数
    返回说明：
        无
 **************************************************************************************************/
void    vRunAction(long lAction, char *pszPGName, char *pszArgv, long lHost)
{
    long    lRet = 0, lNo = 0;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(!bIsTvmBoot())
    {
        fprintf(stderr, "\n平台未启动!\n\n");
        return ;
    }

    if(RC_SUCC != lInitRunTest(pszPGName))
    {
        fprintf(stderr, "\n\t初始化系统参数失败!\n\n");
        return ;
    }

    if(1 == lAction)
        lRet = lShutSystem(false);
    else if(2 == lAction)
        lRet = lShutSystem(true);
    else if(4 == lAction)
    {
        lRet = lDisableService(pstSavm, pszArgv);
        if(RC_FAIL == lRet)
            fprintf(stderr, "关闭服务(%s)失败, err:%s\n", pszArgv, sGetTError(pstSavm->m_lErrno));
        else if(RC_EXIST == lRet)
            fprintf(stderr, "%s\n", sGetTError(pstSavm->m_lErrno));
        else
            fprintf(stdout, "关闭服务(%s)成功!\n", pszArgv);
    }
    else if(8 == lAction)
    {
        lRet = lEnableService(pstSavm, pszArgv);
        if(RC_FAIL == lRet)
            fprintf(stderr, "开启服务(%s)失败, err:%s\n", pszArgv, sGetTError(pstSavm->m_lErrno));
        else if(RC_EXIST == lRet)
            fprintf(stderr, "%s\n", sGetTError(pstSavm->m_lErrno));
        else
            fprintf(stdout, "开启服务(%s)成功!\n", pszArgv);
    }
    else if(20 == lAction)
    {
        if(RC_SUCC != lSetSysState(pstSavm, SYSTEM_PLT_PAUSE))
            fprintf(stderr, "\n设置平台[暂停]状态失败, err:%s\n\n", sGetTError(pstSavm->m_lErrno));
        else
            fprintf(stdout, "\n设置系统状态成功, 当前系统状态为:[暂停]!\n\n");
        vPrintSystem();
    }
    else if(24 == lAction)
    {
        if(RC_SUCC != lSetSysState(pstSavm, SYSTEM_PLT_START))
            fprintf(stderr, "\n设置平台[正常]状态失败, err:%s\n\n", sGetTError(pstSavm->m_lErrno));
        else
            fprintf(stdout, "\n设置系统状态成功, 当前系统状态为:[正常]!\n\n");
        vPrintSystem();
    }
    else if(48 == lAction)
    {
        if((lNo = atol(pszArgv)) <= 0)
        {
            fprintf(stderr, "流水号格式错误:%s\n\n", pszArgv);
            return ;
        }

        if(RC_SUCC != lSetSysSeqNo(pstSavm, lNo))
            fprintf(stdout, "\n设置平台流水号(%d)失败, err:%s\n\n", lNo, sGetTError(pstSavm->m_lErrno));
        else
            fprintf(stdout, "\n设置平台流水号(%d)成功!\n\n", lNo);
        vPrintSystem();
    }
    else if(80 == lAction)
    {
        if(RC_SUCC != lIsValidDate(pszArgv))
        {
            fprintf(stderr, "清算日期格式错误:%s\n\n", pszArgv);
            return ;
        }

        if(RC_SUCC != lSetSysCrDate(pstSavm, pszArgv))
            fprintf(stdout, "\n设置平台清算日期(%s)失败, err:%s\n\n", pszArgv, sGetTError(pstSavm->m_lErrno));
        else
            fprintf(stdout, "\n设置平台清算日期(%s)成功!\n\n", pszArgv);
        vPrintSystem();
    }
    else if(144 == lAction)     //  测试时候内存加载关联系统日期
    {
        if(RC_SUCC != lIsValidDate(pszArgv))
        {
            fprintf(stderr, "测试交易日期格式错误:%s\n\n", pszArgv);
            return ;
        }

        if(RC_SUCC != lSetSysTxDate(pstSavm, pszArgv))
            fprintf(stdout, "\n设置平台交易日期(%s)失败, err:%s\n\n", pszArgv, sGetTError(pstSavm->m_lErrno));
        else
            fprintf(stdout, "\n设置平台交易日期(%s)成功, 仅供测试内存加载使用!\n\n", pszArgv);
        vPrintSystem();
    }
    else if(260 == lAction)
        lRet = lOffLineGroup(pszArgv, lHost);
    else if(264 == lAction)
        lRet = lOnLineGroup(pszArgv, lHost);
    else if(512 == lAction || 640 == lAction)
        lRet = lTradeOperate(pszArgv);
    else
        vPrintFunc(pszPGName);
    if(lRet != RC_SUCC)
    {
//      fprintf(stderr, "\n停止系统失败\n");
        return ;
    }
    return ;
}

/**************************************************************************************************
    函 数 名：cts主函数
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
int     main(int argc, char *argv[])
{
    char    szCom[100];
    int     iChoose = 0;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();
    long    lAction = 0, lRet = 0, lHost = 0;

    vLogSetPath();
    vLogSetSize(0);
    vSetSysLevel(pGetCmie(), LOG_TRACE);
    vSetProgram(basename(argv[0]));
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), argv[0]);
    if(RC_SUCC != lInitAllSatvm(pstSavm))
    {
        fprintf(stderr, "初始化平台参数失败, err:%s\n", sGetTError(lGetTErrno()));
        return RC_FAIL;
    }
	vTvmHoldRelease(pstSavm);
    IBPenvcheck("TVMDBD");
    IBPenvcheck("TVMCFG");
    IBPenvcheck("SETUPCFG");

/*
    IBPenvcheck("FILEDIR");
    IBPenvcheck("LOG_PATH_DIR");
    IBPenvcheck("FILE_PATH_DIR");
*/

    memset(szCom, 0, sizeof(szCom));
    while(-1 != (iChoose = getopt(argc, argv, "S::W::U::E::T::c::R::h::F:k:o:s:e:d:t:w:b:p:v?::")))
    {
        switch(iChoose)
        {
        case    'S':
            lAction = 1;
            if(!optarg)  break;
            else if(!strcasecmp(optarg, "a"))
                lAction = 2;
            break;
        case    'W':
            return lWakeupSystem();
        case    'T':
            lAction |= 512;
            break;
        case    'k':
            if(bIsExtraBoot(basename(argv[0])))
            {
                fprintf(stderr, "存在%s进程正在操作\n", basename(argv[0]));
                return RC_SUCC;
            }
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            return lRebootProcess(pstSavm, szCom);
        case    'c':
            lAction |= 256;
			if(optarg)	lHost = atol(optarg);
            break;
        case    's':
            lAction |= 4;
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            break;
        case    'w':
            lAction |= 8;
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            break;
        case    'R':        //  回收
            if(RC_SUCC != lRefreshParam(pstSavm))
                fprintf(stderr, "\n刷新核心配置失败, %s\n\n", sGetTError(pstSavm->m_lErrno));
            else
                fprintf(stderr, "\n刷新核心配置成功, completed successfully!!!\n\n");
            return RC_SUCC;
        case    'F':        //  回收
            vResetObject(optarg);
            return RC_SUCC;
        case    'U':
            lAction |= 16;
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            if(optarg)
            {
                if(!strcasecmp(optarg, "s"))
                    lAction |= 4;
                if(!strcasecmp(optarg, "w"))
                    lAction |= 8;
            }
            break;
        case    'e':
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            lAction |= 32;
            break;
        case    'd':
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            lAction |= 64;
            break;
        case    't':
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            lAction |= 128;
            break;
        case    'E':
            vUnloadConfig();
            return RC_SUCC;
        case    'o':
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            vExpRunTime(pstSavm, szCom);
            return RC_SUCC;
        case    'b':
            lAction = lAction | 16;
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            return RC_SUCC;
        case    'p':
            vPrintParam(optarg);
            return RC_SUCC;
        case    'h':
            vPrintFunc(basename(argv[0]));
            return RC_SUCC;
        case    'v':
            fprintf(stdout, "\nCopyright (C) 2016-2020 盒子支付(iBOXPAY.com) Technology Co., "
                "Ltd. All Rights Reversed\n");
            fprintf(stdout, "%s\n", sGetCoreVers());
            fflush(stdout);
            return RC_SUCC;
        default:
            break;
        }
    }

    if(0 == lAction)
    {
        vPrintFunc(basename(argv[0]));
        return RC_SUCC;
    }

    if(bIsExtraBoot(basename(argv[0])))
    {
        fprintf(stderr, "存在%s进程正在操作\n", argv[0]);
        return RC_FAIL;
    }

    vRunAction(lAction, argv[0], szCom, lHost);
    return RC_SUCC;
}

/**************************************************************************************************
     code end
 **************************************************************************************************/
