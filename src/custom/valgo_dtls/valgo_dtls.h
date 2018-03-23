/**************************************************************************************************
    文 件 名：settle_algo.h
    代码作者：DeffPuzzL、liujie
    编写版本：
    创建日期：2016-07-14
    功能描述：demo头文件
 **************************************************************************************************/
#ifndef     __HHH__CLASS_ALGO_HHHH_XXXX
#define     __HHH__CLASS_ALGO_HHHH_XXXX

#include    "face.h"
#include    "tbl_order_succ.h"
#include    "tbl_algo_dtls.h"

/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CAlgo
{
public:
    CAlgo();
    ~CAlgo();

public:
    inline  char   *sGetVersion() { return g_szVersion; }
    long    lRunObject(char *, char *);

public:
	void	vAlgoTxnDtl();
	void    vErrorToAlgo();

private:
	long	nCalculateFee(char *pszDiscId, TAlgoDtls *pstAlgoDtl);
	long    lAlgoDtlIns(TOrderSucc *pstOrder, char *pszDate, char *pszNum);	
	double  dGetAlgoFee(dbDiscAlgo *pstAlgo, double dTxAmt, long *plFeeType);

private:
    typedef struct
    {
        char    m_szName[64];
        void (CAlgo::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif  //  __HHH__CLASS_ALGO_HHHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

