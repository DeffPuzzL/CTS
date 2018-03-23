#ifndef     _HH_DBS_API_DEFIND_HHHH_XXX
#define     _HH_DBS_API_DEFIND_HHHH_XXX
#include    "shmdb.h"

/*************************************************************************************************
    数据库源定义
 *************************************************************************************************/
#define DBS_BUSS                              1
#define DBS_TXN                               2
#define DBS_BATCH                             3
#define DBS_ROUTE                             4
#define DBS_RISK                              5
#define DBS_ROUTENC                           6
#define DBS_ROUTEDK                           7

#define TBL_MCHT_STLM                         0x1e
#define TBL_MCHT_BASE                         0x1f
#define TBL_MCHT_PROD                         0x20
#define TBL_DISC_ALGO                         0x21
#define MAX_TABLE_USE                         0x32

#define DBS_TVM                               "dbstvm"
/*************************************************************************************************
    SQLCODE定义    
 *************************************************************************************************/
#ifndef _SQL_CODE_DEFINE__

#define DBS_NOTFOUND                          1403
#define SQLCODE                               0
#define SQLOK                                 (0 == SQLCODE)
#define SQLERR                                ((0 != SQLCODE) && (DBS_NOTFOUND != SQLCODE))
#define SQLNOTFOUND                           (DBS_NOTFOUND == SQLCODE)
#define SQL_UD_ERR                            (SQLCODE && !(DBS_NOTFOUND == SQLCODE && 0 == 0))
#define EFFECT_NUM                            0

#endif        //    _SQL_CODE_DEFINE__

/*************************************************************************************************
    用户业务表装载自定义方法结构
 *************************************************************************************************/
typedef struct __TABLE_CREATE
{
    TABLE   table;
    long    lDbsFrom;
    long    (*pfCreate)(SATvm *pstSamo, void *arg);
}TblCreat;

/*************************************************************************************************
    数据库操作函数
 *************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
extern char*        sDbsError();
extern long         lDbsConnect(long lType);
extern void         vDbsDisconnect();
extern long         lDbsBegin();
extern long         lDbsCommit();
extern long         lDbsRollback();
extern long         lDbsEffectCnt();
extern long         lDbsConnectByDataSource(long lType);
extern void         vDbsDisconnectByDataSource(long lType);

extern long         lGetCreateNum();
extern TblCreat*    pGetDefCreate(long idx);
extern TblCreat*    pGetTblCreate(TABLE t);

extern long         lInitMchtAcct(SATvm *pstSavm, void *arg);
extern long         lInitMchtBase(SATvm *pstSavm, void *arg);
extern long         lInitMchtProd(SATvm *pstSavm, void *arg);
extern long         lInitDiscAlgo(SATvm *pstSavm, void *arg);


extern long         lGetMchtAcct(dbMchtAcct *pstAct, char *pszMcht);
extern long         lGetMchtBase(dbMchtBase *pstMcht, char *pszMchtNo);
extern long         lGetMchtPord(dbMchtProd *pstProd, char *pszPord);
extern long         lGetDiscAlgo(char *pszDisc, double dTxAmt, long lAcType, dbDiscAlgo *pstAlgo);

#ifdef __cplusplus
}
#endif

/*************************************************************************************************
    用户自定义加载
 *************************************************************************************************/
static  TblCreat    g_stCreate[MAX_TABLE_USE] = {
    {TBL_MCHT_STLM,         DBS_BUSS,   lInitMchtAcct},
    {TBL_MCHT_BASE,         DBS_BUSS,   lInitMchtBase},
    {TBL_DISC_ALGO,         DBS_BUSS,   lInitDiscAlgo},
    {TBL_MCHT_PROD,         DBS_BUSS,   lInitMchtProd}
};

#endif    //    _HH_DBS_API_DEFIND_HHHH_XXX
