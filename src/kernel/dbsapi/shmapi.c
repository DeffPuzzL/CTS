/**************************************************************************************************
    文 件 名：shmapi.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：cts业务内存表访问API
 **************************************************************************************************/
#include    "tvm.h"
#include    "dbsapi.h"

/****************************************************************************************
 *    应用层
 ****************************************************************************************/
/****************************************************************************************
 *    获取DISC_ID所有分段的费率    
 ****************************************************************************************/
long    lActTypeCmp(long a, long u)
{
    long    m = 1;

    if(u < 1) return RC_FAIL;

    m = m << (u - 1);
    if(m != (a & m))
        return RC_FAIL;

    return RC_SUCC;
}

/****************************************************************************************
 *    根据商户号索引获取商户信息
 ****************************************************************************************/
long    lGetMchtPord(dbMchtProd *pstProd, char *pszPord)
{
    dbMchtProd  stProd;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    memset(&stProd, 0, sizeof(dbMchtProd));
    strncpy(stProd.prod_code, pszPord, sizeof(stProd.prod_code) - 1);

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->pstVoid = (void *)&stProd;
    pstSavm->tblName = TBL_MCHT_PROD;
    pstSavm->lSize   = sizeof(dbMchtProd);
    if(RC_SUCC == lSelect(pstSavm, pstProd))
        return RC_SUCC;

    if(RC_SUCC != lGetMchtProdByDb(pstProd, pszPord))
    {
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, sDbsError());
        return RC_FAIL;
    }

    return RC_SUCC;
}

/****************************************************************************************
 *    根据商户号获取商户信息
 ****************************************************************************************/
long    lGetMchtBase(dbMchtBase *pstMcht, char *pszMchtNo)
{
    dbMchtBase   stBase;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    memset(&stBase, 0, sizeof(stBase));
    strncpy(stBase.mcht_no, pszMchtNo, sizeof(stBase.mcht_no));

    pstSavm->pstVoid = &stBase;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = TBL_MCHT_BASE;
    pstSavm->lSize   = sizeof(dbMchtBase);
    if(RC_SUCC == lSelect(pstSavm, pstMcht))
        return RC_SUCC;

    if(RC_SUCC != lGetMchtBaseByDb(pstMcht, pszMchtNo))
    {
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, sDbsError());
        return RC_FAIL;
    }

    return RC_SUCC;
}

/****************************************************************************************
 *    获取账号信息
 ****************************************************************************************/
long    lGetMchtAcct(dbMchtAcct *pstAct, char *pszMcht)
{
    dbMchtAcct  stAcct;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    memset(&stAcct, 0, sizeof(stAcct));
    strncpy(stAcct.mcht_code, pszMcht, sizeof(stAcct.mcht_code) - 1);

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = TBL_MCHT_STLM;
    pstSavm->pstVoid = (void *)&stAcct;
    pstSavm->lSize   = sizeof(dbMchtAcct);
    if(RC_SUCC == lSelect(pstSavm, pstAct))
        return RC_SUCC;

    if(RC_SUCC != lGetMchtAcctByDb(pstAct, pszMcht))
    {
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, sDbsError());
        return RC_FAIL;
    }

    return RC_SUCC;
}

/****************************************************************************************
 *    获取DISC_ID所有分段的费率    
 ****************************************************************************************/
long    lGetDiscAlgo(char *pszDisc, double dTxAmt, long lAcType, dbDiscAlgo *pstAlgo)
{
    size_t      i, lOut = 0;
    dbDiscAlgo  stDisc, *pstDisc = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    memset(&stDisc, 0, sizeof(dbDiscAlgo));
    strncpy(stDisc.disc_id, pszDisc, sizeof(stDisc.disc_id) - 1);

    pstSavm->pstVoid = &stDisc;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = TBL_DISC_ALGO;
    pstSavm->lSize   = sizeof(dbDiscAlgo);
    if(RC_SUCC != lQuery(pstSavm, &lOut, (void **)&pstDisc))
        return RC_FAIL;
   
    for(i = 0; i < lOut; i ++)
    {
        if(RC_SUCC != lActTypeCmp(pstDisc[i].card_type, lAcType))
            continue;

        if(dTxAmt <= pstDisc[i].upper_amount || 1 == lOut)
        {
            memcpy(pstAlgo, &pstDisc[i], sizeof(dbDiscAlgo));
            IBPFree(pstDisc);
	        return RC_SUCC;
        }
    }

    IBPFree(pstDisc);
    pstSavm->m_lErrno = NO_DATA_FOUND; 
    return RC_FAIL;
}

/****************************************************************************************
 *    code end
 ****************************************************************************************/
