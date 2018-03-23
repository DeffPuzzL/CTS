/**************************************************************************************************
    文 件 名：auto_algo.h
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：
 **************************************************************************************************/
#ifndef     __HHH_AUTO_QUERY_HHHH_XXXX
#define     __HHH_AUTO_QUERY_HHHH_XXXX

#include    "face.h"
#include    "dbsapi.h"
#include    "algo.h"
#include    "tbl_order_err.h"

/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CAutoAlgo
{
public:
    CAutoAlgo();
    ~CAutoAlgo();

public:
    inline  char   *sGetVersion() { return g_szVersion; }
    long    lRunObject(char *, char *);

	void    vPutErrorPool(TOrderErr *pstErr);
public:
	void    vAutoCheckErr(SATvm *pstSavm, CtsProcess *pstProcess);

private:
    typedef struct
    {
        char    m_szName[64];
        void    (CAutoAlgo::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif  //  __HHH_AUTO_QUERY_HHHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

