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

/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CQuery
{
public:
    CQuery();
    ~CQuery();

public:
    inline  char   *sGetVersion() { return g_szVersion; }
    long    lRunObject(char *, char *);

public:
	void    vAutoCheckTxn(SATvm *pstSavm, long lSleep, CtsProcess *pstProcess);

private:
	long    lGetTimeDiff(char *pszTime, long lTime);

private:
    typedef struct
    {
        char    m_szName[64];
        void    (CQuery::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif  //  __HHH__SETTLE_ALGO_HHHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

