/**************************************************************************************************
    文 件 名：settle_algo.h
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：
 **************************************************************************************************/
#ifndef     __HHH__SETTLE_ALGO_HHHH_XXXX
#define     __HHH__SETTLE_ALGO_HHHH_XXXX

#include    "face.h"
#include    "dbsapi.h"
#include    "algo.h"
#include    "tbl_algo_dtls.h"
#include    "tbl_mcht_fund.h"
#include    "tbl_settle_txn.h"

/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CClear
{
public:
    CClear();
    ~CClear();

public:
    inline  char   *sGetVersion() { return g_szVersion; }
    long    lRunObject(char *, char *);

public:
    void    vGenSettleTxn();
    void    vChkSettleTxn();

private:
	void    vPutSettleTxn(TSettleTxn *pstSettle);
	void    vFillAlgoDtl(TAlgoDtls *pstAlgo);
	long    nInsertError(TAlgoDtls *pstAlgo);
	long    lSettleMonIns(TSettleTxn *pstSettle);
    long    nCalculateFee(char *pszDiscId, TAlgoDtls *pstAlgoDtl);
    double  dGetAlgoFee(dbDiscAlgo *pstAlgo, double dTxAmt, long *plFeeType);
	long    lAlgoSettle(TAlgoDtls *pstAlgo, TMchtFund *pstFund, TSettleTxn *pstSettle);

private:
    typedef struct
    {
        char    m_szName[64];
        void    (CClear::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif  //  __HHH__SETTLE_ALGO_HHHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

