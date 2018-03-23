#include    "cts.h"
#include    "dbsapi.h"


/****************************************************************************************
    获取操作数据库错误信息
 ****************************************************************************************/
char*   sDbsError()
{
    static char szMsg[256];

    return szMsg;
}

/****************************************************************************************
    连接数据数据库
 ****************************************************************************************/
long    lDbsConnect(long lType)
{
    char  szSID[40], szUser[40], szPwd[40];

    memset(szSID, 0, sizeof(szSID));
    memset(szUser, 0, sizeof(szUser));
    memset(szPwd, 0, sizeof(szPwd));

    if(lType == DBS_BUSS)
    {
        strcpy(szUser, sGetDbsEnv("CB_USER"));
        strcpy(szPwd, sGetDbsEnv("CB_PASSWD"));
        strcpy(szSID, sGetDbsEnv("CORACLE_SID"));
    }
    else if(lType == DBS_TXN)
    {
        strcpy(szUser, sGetDbsEnv("TB_USER"));
        strcpy(szPwd, sGetDbsEnv("TB_PASSWD"));
        strcpy(szSID, sGetDbsEnv("TORACLE_SID"));
    }
    else if(lType == DBS_BATCH)
    {
        strcpy(szUser, sGetDbsEnv("DB_USER"));
        strcpy(szPwd, sGetDbsEnv("DB_PASSWD"));
        strcpy(szSID, sGetDbsEnv("DORACLE_SID"));
    }
    else if(lType == DBS_ROUTE)
    {
        strcpy(szUser, sGetDbsEnv("RB_USER"));
        strcpy(szPwd, sGetDbsEnv("RB_PASSWD"));
        strcpy(szSID, sGetDbsEnv("RORACLE_SID"));
    }
    else if(lType == DBS_RISK)
    {
        strcpy(szUser, sGetDbsEnv("RI_USER"));
        strcpy(szPwd, sGetDbsEnv("RI_PASSWD"));
        strcpy(szSID, sGetDbsEnv("RIORACLE_SID"));
    }
    else if(lType == DBS_ROUTENC)
    {
        strcpy(szUser, sGetDbsEnv("RTNC_USER"));
        strcpy(szPwd, sGetDbsEnv("RTNC_PASSWD"));
        strcpy(szSID, sGetDbsEnv("RTNCORACLE_SID"));
    }
    else if(lType == DBS_ROUTEDK)
    {
        strcpy(szUser, sGetDbsEnv("RTDK_USER"));
        strcpy(szPwd, sGetDbsEnv("RTDK_PASSWD"));
        strcpy(szSID, sGetDbsEnv("RTDKORACLE_SID"));
    }
    else
    {
        return RC_FAIL;
    }

    return 0;
}

/****************************************************************************************
    断开当前数据库
 ****************************************************************************************/
void    vDbsDisconnect()
{
}

/****************************************************************************************
    启动数据库事务
 ****************************************************************************************/
long    lDbsBegin()
{
    return RC_SUCC;
}

/****************************************************************************************
    提交事务
 ****************************************************************************************/
long    lDbsCommit()
{
    return 0;
}

/****************************************************************************************
    回滚事务    
 ****************************************************************************************/
long    lDbsRollback()
{
    return 0;
}

/****************************************************************************************
    影响有效记录数    
 ****************************************************************************************/
long    lDbsEffectCnt()
{
    return  0;
}

long    lDbsConnectByDataSource(long lType)
{
    char  szSID[40], szUser[40], szPwd[40], szDbConnectFlag[20];

    memset(szSID, 0, sizeof(szSID));
    memset(szUser, 0, sizeof(szUser));
    memset(szPwd, 0, sizeof(szPwd));
    memset(szDbConnectFlag, 0, sizeof(szDbConnectFlag));

    if(lType == DBS_BUSS)
    {
        strcpy(szUser, sGetDbsEnv("CB_USER"));
        strcpy(szPwd, sGetDbsEnv("CB_PASSWD"));
        strcpy(szSID, sGetDbsEnv("CORACLE_SID"));
        strcpy(szDbConnectFlag, "dbs_buss");
    }
    else if(lType == DBS_TXN)
    {
        strcpy(szUser, sGetDbsEnv("TB_USER"));
        strcpy(szPwd, sGetDbsEnv("TB_PASSWD"));
        strcpy(szSID, sGetDbsEnv("TORACLE_SID"));
        strcpy(szDbConnectFlag, "dbs_txn");
    }
    else if(lType == DBS_BATCH)
    {
        strcpy(szUser, sGetDbsEnv("DB_USER"));
        strcpy(szPwd, sGetDbsEnv("DB_PASSWD"));
        strcpy(szSID, sGetDbsEnv("DORACLE_SID"));
        strcpy(szDbConnectFlag, "dbs_batch");
    }
    else if(lType == DBS_ROUTE)
    {
        strcpy(szUser, sGetDbsEnv("RB_USER"));
        strcpy(szPwd, sGetDbsEnv("RB_PASSWD"));
        strcpy(szSID, sGetDbsEnv("RORACLE_SID"));
        strcpy(szDbConnectFlag, "dbs_route");
    }
    else if(lType == DBS_RISK)
    {
        strcpy(szUser, sGetDbsEnv("RI_USER"));
        strcpy(szPwd, sGetDbsEnv("RI_PASSWD"));
        strcpy(szSID, sGetDbsEnv("RIORACLE_SID"));
        strcpy(szDbConnectFlag, "dbs_risk");
    }
    else if(lType == DBS_ROUTENC)
    {
        strcpy(szUser, sGetDbsEnv("RTNC_USER"));
        strcpy(szPwd, sGetDbsEnv("RTNC_PASSWD"));
        strcpy(szSID, sGetDbsEnv("RTNCORACLE_SID"));
        strcpy(szDbConnectFlag, "dbs_routenc");
    }
    else if(lType == DBS_ROUTEDK)
    {
        strcpy(szUser, sGetDbsEnv("RTDK_USER"));
        strcpy(szPwd, sGetDbsEnv("RTDK_PASSWD"));
        strcpy(szSID, sGetDbsEnv("RTDKORACLE_SID"));
        strcpy(szDbConnectFlag, "dbs_routedk");
    }
    else
        ;

    return SQLCODE;
}

void    vDbsDisconnectByDataSource(long lType)
{
    char  szDbConnectFlag[20];

    memset(szDbConnectFlag, 0, sizeof(szDbConnectFlag));

    if(lType == DBS_BUSS)
    {
        strcpy(szDbConnectFlag, "dbs_buss");
    }
    else if(lType == DBS_TXN)
    {
        strcpy(szDbConnectFlag, "dbs_txn");
    }   
    else if(lType == DBS_BATCH)
    {
        strcpy(szDbConnectFlag, "dbs_batch");
    }   
    else if(lType == DBS_ROUTE)
    {
        strcpy(szDbConnectFlag, "dbs_route");
    }   
    else if(lType == DBS_RISK)
    {
        strcpy(szDbConnectFlag, "dbs_risk");
    }   
    else if(lType == DBS_ROUTENC)
    {
        strcpy(szDbConnectFlag, "dbs_routenc");
    }   
    else if(lType == DBS_ROUTEDK)
    {
        strcpy(szDbConnectFlag, "dbs_routedk");
    }

}
