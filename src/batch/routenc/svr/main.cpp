/**************************************************************************************************
    文 件 名：
    代码作者：
    编写版本：
    创建日期：
    功能描述：
 **************************************************************************************************/
#include "NCardRoute.h"
#include "model.h"

/**************************************************************************************************
    函 数 名：vPrintVersion(char *pszParm)
    功能描述：显示该应用的版本
    返回说明：
        无
 **************************************************************************************************/
void    vPrintVersion(char *pszParm)
{
    if (!pszParm)
    {
        return ;
    }

    if (strcmp(sUpper(pszParm), "-V"))
    {
        return ;
    }

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
    NCardRoute  objExp;
    CtsEvent    stEvent;
    CtsProcess  stProcess;
    GCts        *pstCts = (GCts *)pGetGloble();
    SATvm       *pstSamo = (SATvm *)pGetSATvm();
    int          iRet = -1;

    vPrintVersion(argv[1]);
    memset(&stProcess, 0, sizeof(stProcess));
    strcpy(stProcess.m_szPGName, basename(argv[0]));

    if (RC_SUCC != lInitialDemo(getpid(), &stProcess))
    {
        IBPerror("进程%s启动初始化失败!", stProcess.m_szPGName);
        return RC_SUCC;
    }

    IBPrace("进程%s启动，当前版本：%s ...", stProcess.m_szPGName, objExp.sGetVersion());

    if (RC_SUCC != lDbsConnect(DBS_ROUTENC))
    {
        return RC_FAIL;
    }

    if (0 != model_init())
    {
        return RC_FAIL;
    }

    while (1)
    {
        memset(&stEvent, 0, sizeof(CtsEvent));

        if (RC_SUCC == lOutGetRequest(pstSamo, &stProcess, &stEvent))
        {
#if 1
            /* 数据库重连探测 */
            IBPinfo("测试数据库连接, 开始...");
            iRet = dbs_isconn();

            if (iRet)
            {
                IBPwarn("测试数据库连接, 失败: iRet = [%d]!", iRet);
                IBPwarn("准备重连数据库...");
                iRet = dbs_reconnect();

                if (iRet)
                {
                    IBPwarn("重连数据库失败, 丢弃此笔报文...");
                    continue;
                }
            }

            IBPinfo("测试数据库连接, 成功.");
#endif

            if (IBPevcache(stEvent.m_lState))
            {
                if (0 != model_init())
                {
                    IBPerror("model_init 条件规则缓存失败");
                }

                continue;
            }

            if (RC_SUCC != objExp.lRunObject(pstCts->m_szCallCode, pstCts->m_szCallSub))
            {
                SYSError(RET_SYSTEM, "执行组件(%s)的事件(%s)错误", pstCts->m_szCallCode,
                         pstCts->m_szCallSub);
            }
        }

        lOutPutResponse(pstSamo, &stEvent);
    }


    return RC_SUCC;
}

/**************************************************************************************************
 *  code end
 **************************************************************************************************/
