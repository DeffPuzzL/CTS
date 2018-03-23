#include    "cts.h"
#include    "dbsapi.h"
#include    "shmdb.h"

/**************************************************************************************************
       表定义
 **************************************************************************************************/
/**************************************************************************************************
    函 数 名：lGetMchtProdByDb(dbMchtProd *pstProd, char *pszMcht, char *pszPord)
    功能描述：从数据库中查找产品信息表
    返回说明：
 **************************************************************************************************/
long    lGetMchtProdByDb(dbMchtProd *pstProd, char *pszPord)
{
	return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lGetMchtAcctByDb(dbMchtAcct *pstAcctInfo, char *pszMcht)
    功能描述：从数据库中查找商户清算卡号信息
    返回说明：
 **************************************************************************************************/
long    lGetMchtAcctByDb(dbMchtAcct *pstAcctInfo, char *pszMcht)
{
	return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lGetMchtBaseByDb(dbMchtBase *pstMcht, char *pszMchtNo)
    功能描述：从数据库中查找商户基础信息
    返回说明：
 **************************************************************************************************/
long    lGetMchtBaseByDb(dbMchtBase *pstMcht, char *pszMchtNo)
{
    return RC_SUCC;
}

/**************************************************************************************************
    code end
 **************************************************************************************************/
