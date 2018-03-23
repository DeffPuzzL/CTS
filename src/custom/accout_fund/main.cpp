/**************************************************************************************************
    文 件 名：main.pc
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：账户实现模块
 **************************************************************************************************/
#include    "accout_fund.h"

/**************************************************************************************************
    函 数 名：vPrintVersion(char *pszParm)
    功能描述：显示该应用的版本
    返回说明：
        无
 **************************************************************************************************/
void    vPrintVersion(char *pszParm)
{
    if(!pszParm)    return ;
    if(strcasecmp(pszParm, "-V"))
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
    CAccount    objExp;
    CtsProcess  stProcess;
    GCts        *pstCts = (GCts *)pGetGloble();
    SATvm       *pstSavm = (SATvm *)pGetSATvm();
    CtsEvent    *pstEvent = (CtsEvent *)pGetEvent();

    vPrintVersion(argv[1]);
    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));
    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", stProcess.m_szPGName);
        return RC_SUCC;
    }

    IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, objExp.sGetVersion());

    if(RC_SUCC != lDbsConnect(DBS_BATCH))
    {
        SYSError(RET_DATABASE, "连接代扣数据库失败, err:(%s)", sDbsError())
        return RC_FAIL;
    }
	
    while (1)
    {
        if(RC_SUCC == lOutGetRequest(pstSavm, &stProcess, pGetCmie()))
        {
            if(IBPevcache(pstEvent))
                continue;

            if (RC_SUCC != objExp.lRunObject(pstCts->m_szCallCode, pstCts->m_szCallSub))
            {
                SYSError(RET_SYSTEM, "执行组件(%s)的事件(%s)错误", pstCts->m_szCallCode,
                    pstCts->m_szCallSub);
            }
        }

        lOutPutResponse(pstSavm, pGetCmie());
    }

    return RC_SUCC;
}

/**************************************************************************************************
 *  code end
 **************************************************************************************************/
