/**************************************************************************************************
    文 件 名：table.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：cts业务内存表访问API
 **************************************************************************************************/
#include    "tvm.h"
#include    "dbsapi.h"
#include    "shmdb.h"

/**************************************************************************************************
       表定义
 **************************************************************************************************/
/**************************************************************************************************
 *     函 数 名：定义表TBL_MCHT_PROD表结构
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
CREATE  C_TBL_MCHT_PROD(TABLE T)
{
    DEFINE(T, "TBL_MCHT_PROD", DBS_TVM, dbMchtProd)
    FIELD(dbMchtProd, prod_code,           "prod_code",              FIELD_CHAR)
    FIELD(dbMchtProd, buss_type,           "buss_type",              FIELD_CHAR)
    FIELD(dbMchtProd, sub_buss_type,       "sub_buss_type",          FIELD_CHAR)
    FIELD(dbMchtProd, status,              "status",                 FIELD_CHAR)
    FIELD(dbMchtProd, disc_num,            "disc_num",               FIELD_LONG)
    FIELD(dbMchtProd, dis_groups,          "dis_groups",             FIELD_CHAR)
    FIELD(dbMchtProd, pay_kind,            "pay_kind",               FIELD_CHAR)
    FIELD(dbMchtProd, disc_cycle,          "disc_cycle",             FIELD_LONG)
    FIELD(dbMchtProd, settle_model,        "settle_model",           FIELD_LONG)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(dbMchtProd, prod_code,       FIELD_CHAR);

    FINISH
}

/**************************************************************************************************
 *     函 数 名：定义表TBL_DISC_ALGO表结构
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
CREATE  C_TBL_DISC_ALGO(TABLE T)
{
    DEFINE(T, "TBL_DISC_ALGO", DBS_TVM, dbDiscAlgo)
    FIELD(dbDiscAlgo, id,                  "id",                     FIELD_LONG)
    FIELD(dbDiscAlgo, disc_id,             "disc_id",                FIELD_CHAR)
    FIELD(dbDiscAlgo, index_num,           "index_num",              FIELD_LONG)
    FIELD(dbDiscAlgo, min_fee,             "min_fee",                FIELD_DOUBLE)
    FIELD(dbDiscAlgo, max_fee,             "max_fee",                FIELD_DOUBLE)
    FIELD(dbDiscAlgo, floor_amount,        "floor_amount",           FIELD_DOUBLE)
    FIELD(dbDiscAlgo, upper_amount,        "upper_amount",           FIELD_DOUBLE)
    FIELD(dbDiscAlgo, flag,                "flag",                   FIELD_LONG)
    FIELD(dbDiscAlgo, fee_value,           "fee_value",              FIELD_DOUBLE)
    FIELD(dbDiscAlgo, card_type,           "card_type",              FIELD_LONG)

    CREATE_IDX(NORMAL)
    IDX_FIELD(dbDiscAlgo, disc_id,         FIELD_CHAR);

    FINISH
}

/**************************************************************************************************
 *     函 数 名：定义表TBL_MCHT_BASE表结构
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
CREATE  C_TBL_MCHT_BASE(TABLE T)
{
    DEFINE(T, "TBL_MCHT_BASE", DBS_TVM, dbMchtBase)
    FIELD(dbMchtBase, mcht_no,             "mcht_no",                FIELD_CHAR)
    FIELD(dbMchtBase, mcht_nm,             "mcht_nm",                FIELD_CHAR)
    FIELD(dbMchtBase, mcht_status,         "mcht_status",            FIELD_CHAR)
    FIELD(dbMchtBase, mcht_kind,           "mcht_kind",              FIELD_CHAR)
    FIELD(dbMchtBase, mcht_addr,           "mcht_addr",              FIELD_CHAR)
    FIELD(dbMchtBase, mcht_brh,            "mcht_brh",               FIELD_CHAR)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(dbMchtBase, mcht_no,         FIELD_CHAR);

    FINISH
}

/**************************************************************************************************
 *     函 数 名：定义表TBL_MCHT_STLM表结构
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
CREATE  C_TBL_MCHT_STLM(TABLE T)
{
    DEFINE(T, "TBL_MCHT_STLM", DBS_TVM, dbMchtAcct)
    FIELD(dbMchtAcct, acct_id,             "acct_id",                FIELD_LONG)
    FIELD(dbMchtAcct, mcht_code,           "mcht_code",              FIELD_CHAR)
    FIELD(dbMchtAcct, acct_default,        "acct_default",           FIELD_LONG)
    FIELD(dbMchtAcct, acct_type,           "acct_type",              FIELD_CHAR)
    FIELD(dbMchtAcct, acct_no,             "acct_no",                FIELD_CHAR)
    FIELD(dbMchtAcct, acct_nm,             "acct_nm",                FIELD_CHAR)
    FIELD(dbMchtAcct, acct_bank_no,        "acct_bank_no",           FIELD_CHAR)
    FIELD(dbMchtAcct, acct_bank_nm,        "acct_bank_nm",           FIELD_CHAR)
    FIELD(dbMchtAcct, acct_zbank_no,       "acct_zbank_no",          FIELD_CHAR)
    FIELD(dbMchtAcct, acct_zbank_nm,       "acct_zbank_nm",          FIELD_CHAR)
    FIELD(dbMchtAcct, acct_zbank_addr,     "acct_zbank_addr",        FIELD_CHAR)
    FIELD(dbMchtAcct, acct_zbank_code,     "acct_zbank_code",        FIELD_CHAR)
    FIELD(dbMchtAcct, acct_net_no,         "acct_net_no",            FIELD_CHAR)
    FIELD(dbMchtAcct, acct_union_no,       "acct_union_no",          FIELD_CHAR)
    FIELD(dbMchtAcct, cert_type,           "cert_type",              FIELD_CHAR)
    FIELD(dbMchtAcct, cert_no,             "cert_no",                FIELD_CHAR)
    FIELD(dbMchtAcct, resv_phone,          "resv_phone",             FIELD_CHAR)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(dbMchtAcct, mcht_code,       FIELD_CHAR);
    IDX_FIELD(dbMchtAcct, acct_type,       FIELD_CHAR);

    FINISH
}

/**************************************************************************************************
    加载定义
 **************************************************************************************************/
/**************************************************************************************************
 *     函 数 名：pGetTblCreate(TABLE t)
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
TblCreat*   pGetTblCreate(TABLE t)
{
    return &g_stCreate[t];
}

/**************************************************************************************************
 *     函 数 名：lGetCreateNum()
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
long    lGetCreateNum()
{
    TABLE   t = 0;

    for(t = 0; t < MAX_TABLE_USE; t ++)
    {
        if(!pGetTblCreate(t)->pfCreate || 0 == pGetTblCreate(t)->lDbsFrom)
            break;
    }

    return t;
}

/**************************************************************************************************
 *     函 数 名：定义表TBL_MCHT_STLM结构
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
long    lInitMchtAcct(SATvm *pstSavm, void *arg)
{
    dbMchtAcct    stAct;
    long          lRet;

    vTvmHoldConnect(pstSavm);
    lDropTable(pstSavm, TBL_MCHT_STLM);  // 先删除表
    lRet = lCreateTable(pstSavm, TBL_MCHT_STLM, 10000, C_TBL_MCHT_STLM);
    vTvmHoldRelease(pstSavm);

    return lRet;
}

/**************************************************************************************************
 *     函 数 名：定义表TBL_MCHT_BASE结构
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
long    lInitMchtBase(SATvm *pstSavm, void *arg)
{
    dbMchtBase    stBase;
    long          lRet;

    vTvmHoldConnect(pstSavm);
    lDropTable(pstSavm, TBL_MCHT_BASE);  // 先删除表
    lRet = lCreateTable(pstSavm, TBL_MCHT_BASE, 10000, C_TBL_MCHT_BASE);
    vTvmHoldRelease(pstSavm);

    return lRet;
}

/**************************************************************************************************
 *     函 数 名：定义表TBL_MCHT_PROD结构
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
long    lInitMchtProd(SATvm *pstSavm, void *arg)
{
    dbMchtProd    stProd;
    long          lRet;

    vTvmHoldConnect(pstSavm);
    lDropTable(pstSavm, TBL_MCHT_PROD);  // 先删除表
    lRet = lCreateTable(pstSavm, TBL_MCHT_PROD, 10000, C_TBL_MCHT_PROD);
    vTvmHoldRelease(pstSavm);

    return lRet;
}

/**************************************************************************************************
 *     函 数 名：定义表TBL_DISC_ALGO结构
 *     功能描述：
 *     返回说明：
 **************************************************************************************************/
long    lInitDiscAlgo(SATvm *pstSavm, void *arg)
{
    dbDiscAlgo    stDisc;
    long          lRet;

    vTvmHoldConnect(pstSavm);
    lDropTable(pstSavm, TBL_DISC_ALGO);  // 先删除表
    lRet = lCreateTable(pstSavm, TBL_DISC_ALGO, 10000, C_TBL_DISC_ALGO);
    vTvmHoldRelease(pstSavm);

    return lRet;
}

/**************************************************************************************************
    code end
 **************************************************************************************************/
