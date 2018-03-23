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
#include    "dbsapi.h"
#include    "tbl_mcht_fund.h"
#include    "tbl_settle_txn.h"


/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CSettle
{
public:
    CSettle();
    ~CSettle();

public:
    inline  char   *sGetVersion() { return g_szVersion; }
    long    lRunObject(char *, char *);

    long    lUpdateError(TSettleTxn *pstSettle);
    long    lUpdateSettle(TSettleTxn *pstSettle);
    long    lCallSettlePay(TSettleTxn *pstSettle);
    long    lInsertSettle(TSettleTxn *pstSettle, TMchtFund *pstFund, long lSeqNo);

public:
    void    vGenSettleTxn();
    void    vUpdSettleTxn();

private:
    typedef struct
    {
        char    m_szName[64];
        void (CSettle::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif  //  __HHH__CLASS_ALGO_HHHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

