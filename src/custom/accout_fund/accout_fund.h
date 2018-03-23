/**************************************************************************************************
    文 件 名：accout_fund.h
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：
 **************************************************************************************************/
#ifndef     __HHH__ACCOUT_FUND_HHHH_XXXX
#define     __HHH__ACCOUT_FUND_HHHH_XXXX

#include    "face.h"
#include    "dbsapi.h"
#include    "algo.h"
#include    "tbl_algo_dtls.h"
#include    "tbl_settle_txn.h"

/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CAccount
{
public:
    CAccount();
    ~CAccount();

public:
    inline  char   *sGetVersion() { return g_szVersion; }
    long    lRunObject(char *, char *);

public:
    void    vCheckAlgoTxn();
    void    vAlgoAccount();
	void    vPaySettleTxn();
	void    vUpdateResult();
	void    vPaySettleChk();
	void	vSynSettleTxn();
	void	vQueryBalance();
	void	vPopAsynIndex();
	void	vReverseSettle();
	void    vMainActBalace();
	void    vAlgoErrorTxn();

private:
	long    lAlgoDtlTxn(TAlgoDtls *pstAlgo);
	void    vInsertError(TAlgoDtls *pstAlgo);
	void    vFillAlgoDtl(TAlgoDtls *pstAlgo);
	void    vChargeInAmt(char cApp, char cType);
	long    lProductPayFee(TSettleTxn *pstSettle);
	void    vAccountReverse(TSettleTxn *pstSettle);
	void    vAccountChange(char chDc, char chApp, char cType);
	long    nCalculateFee(long lNum, char *pszDisGrp, TAlgoDtls *pstAlgoDtl);
	double  dGetAlgoFee(dbDiscAlgo *pstAlgo, double dTxAmt, long *plFeeType);
	void    vErrorAlgoAct(TAlgoDtls *pstAlgoDtl, char *pszIllust);
    void    vPayActChange(TSettleTxn *pstSettle, char cDc, char *pszIllust);
    long    lAlgoActChange(TAlgoDtls *pstAlgoDtl, char cType, char *pszIllust);

private:
    typedef struct
    {
        char    m_szName[64];
        void    (CAccount::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif  //  __HHH__ACCOUT_FUND_HHHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

