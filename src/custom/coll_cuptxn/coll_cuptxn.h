/**************************************************************************************************
    文 件 名：settle_algo.h
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：demo头文件
 **************************************************************************************************/
#ifndef     __HHH__CLASS_ALGO_HHHH_XXXX
#define     __HHH__CLASS_ALGO_HHHH_XXXX

#include    "face.h"
#include    "tbl_algo_dtls.h"
#include    "tbl_cups_succ.h"
#include    "tbl_order_err.h"
#include    "tbl_order_succ.h"

/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CColl
{
public:
    CColl();
    ~CColl();

public:
    inline  char   *sGetVersion() { return g_szVersion; }
    long    lRunObject(char *, char *);

private:
	int     nGetLocalError(TCupsSucc *pstCups, TOrderErr *pstErr);
	int     nIsNAlgoTxn(char *pszId, TAlgoDtls *pstAlgoDtl, BOOL *pbAlgo, BOOL *pbCom);
	int     nMatchError(BOOL *pbFind, TOrderErr *pstError, char *pszId);
	int     nSetLocalTxn(char *pszId, char *pszDate, char cState);
	int     nLocalErrorTxn(TOrderSucc *pstOrder, char *pszStlmDate, char *pszSettleNum);
	int     nFromTxnSucc(char *pszStlmDate, char *pszCupsNo, char *pszSettleNum);
	int     nDisposeTxn(TOrderErr *pstErr, TAlgoDtls *pstAlgoDtl, BOOL bAlgo, BOOL bCmt);
	int     nManageError(TOrderErr *pstErr, TOrderErr *pstCom);
	int     nUpdateError(TOrderErr *pstError, TOrderSucc *pstTxn);
	int     nUpdateSucc(TOrderErr *pstError, TOrderSucc *pstTxn);
	int     nCheckTxnList(TCupsSucc *pstCups, TOrderSucc *pstTxn, char *pszStlmDate, char *pszSettleNum);
	int     nCupsErrorTxn(TCupsSucc *pstCups, TOrderSucc *pstTxn, char *pszStlmDate, char *pszSettleNum);
	int     nCheckCupsTxn(TCupsSucc *pstCups, char *pszStlmDate, char *pstSettleNum);
	int     nFromCupsTxn(char *pszStlmDate, char *pszCupsNo, char *pszSettleNum);
	void    vFillStlmErr(TOrderErr *pstErr, TOrderSucc *pstOrder);
	void    vSaveAlgoDtl(TAlgoDtls *pstAlgoDtl);

public:
	void	vCollateDetail();
	void	vCollateSettle();


private:
    typedef struct
    {
        char    m_szName[64];
        void (CColl::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];

	char	m_szBak[512];
};

#endif  //  __HHH__CLASS_ALGO_HHHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

