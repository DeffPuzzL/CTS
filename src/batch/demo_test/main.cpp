/**************************************************************************************************
    文 件 名：main.pc
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：业务测试程序Demo
 **************************************************************************************************/
#include    "demo_test.h"

/**************************************************************************************************
    函 数 名：vPrintVersion(char *pszParm)
    功能描述：显示该应用的版本
    返回说明：
        无
 **************************************************************************************************/
void    vPrintVersion(char *pszParm)
{
	if(!pszParm)	return ;
    if(strcasecmp(sUpper(pszParm), "-V"))
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
    CDemo       objExp;
    CtsEvent    stEvent;
    CtsProcess  stProcess;
    GCts        *pstCts = (GCts *)pGetGloble();
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    vPrintVersion(argv[1]);
    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));
    if(RC_SUCC != lInitialDemo(pstSavm, getpid(), &stProcess))
    {
        IBPerror("进程%s初始化失败, %s", stProcess.m_szPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, objExp.sGetVersion());

    while (1)
    {
        memset(&stEvent, 0, sizeof(CtsEvent));
        if (RC_SUCC == lOutGetRequest(pstSavm, &stProcess, &stEvent))
        {
            if (IBPevcache(stEvent.m_lState))
                continue;

            if (RC_SUCC != objExp.lRunObject(pstCts->m_szCallCode, pstCts->m_szCallSub))
            {
                SYSError(RET_SYSTEM, "执行组件(%s)的事件(%s)错误", pstCts->m_szCallCode,
                         pstCts->m_szCallSub);
            }
        }
        lOutPutResponse(pstSavm, &stEvent);
    }

    return RC_SUCC;
}

/**************************************************************************************************
 *  code end
 **************************************************************************************************/
