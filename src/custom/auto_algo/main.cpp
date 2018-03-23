/**************************************************************************************************
    文 件 名：main.pc
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2017-10-24
    功能描述：
 **************************************************************************************************/
#include    "auto_algo.h"

/**************************************************************************************************
    函 数 名：vPrintVersion(char *pszParm)
    功能描述：显示该应用的版本
    返回说明：
        无
 **************************************************************************************************/
void    vPrintVersion(char *pszParm)
{
    if(!pszParm)    return ;
    if (strcmp(sUpper(pszParm), "-V"))
        return ;

    fprintf(stdout, "current version:%s\n", g_szVersion);
    exit(0);
}

/**************************************************************************************************
    函 数 名：main
    功能描述：进程监控主模块
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
int     main(int argc, char *argv[])
{
    CAutoAlgo   objExp;
	long		i, lMin;
    CtsProcess  stProcess;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    vPrintVersion(argv[1]);
    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));
    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", stProcess.m_szPGName);
        return RC_SUCC;
    }

    IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, objExp.sGetVersion());
    while (1)
    {
        lMin = IBPGetParam("ScanTime");
        if(lMin <= 30)   lMin = 30 * 4;      //  默认30秒

		vLogSetPath();
		IBPlogsetname("%s/%s.log", sLogGetPath(), stProcess.m_szPGName);

        lDbsConnect(DBS_BATCH);
        objExp.vAutoCheckErr(pstSavm, &stProcess);
        vDbsDisconnect();

        for(i = 0; i < lMin * 20; i ++, usleep(50000))
            lMonitorEvent(pstSavm, &stProcess);
    }
    vDbsDisconnect();

    return RC_SUCC;
}

/**************************************************************************************************
 *  code end
 **************************************************************************************************/
