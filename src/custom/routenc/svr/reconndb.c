#include "top_dbs/top_dbs.h"
#include "cts.h"
#include "face.h"

/**
 * 数据库重连
 */
int dbs_isconn(void)
{
    int  iRet = -1;
    char rlt[43 + 1] = {0};
    dbsSetDbType(VAR_TOP_DBS_DBTYPE_ORA);
    iRet = dbsExecuteV("select 1 from dual where rownum = :rid", "1", rlt);
    return iRet;
}

/* 数据库重新连接 */
int dbs_reconnect(void)
{
    int i;
    long rt = -1L;

    for (i = 0; i < 5; i++)
    {
        rt = lDbsConnect(DBS_ROUTENC);

        if (rt)
        {
            IBPwarn("第[%d]次重新连接数据库, 错误[%s]...", i + 1, dbsErrStr());
            sleep(1);
        }
        else
        {
            break;
        }
    }

    if (5 == i)
    {
        return -1;
    }

    return 0;
}
