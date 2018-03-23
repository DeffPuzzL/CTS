/**************************************************************************************************
    文 件 名：settle_algo.h
    代码作者：DeffPuzzL、liujie
    编写版本：
    创建日期：2016-07-14
    功能描述：demo头文件
 **************************************************************************************************/
#ifndef     __HHH__DEMO_TEST_HHH_XXXX
#define     __HHH__DEMO_TEST_HHH_XXXX

#include    "face.h"

/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CDemo
{
public:
    CDemo();
    ~CDemo();

public:
    inline  char   *sGetVersion() { return g_szVersion; }

    long    lRunObject(char *, char *);
public:
    void    vDemoTest();
    void    vDemoPrint();
	void	vDemoTrade();
	void	vDemoError();
	void	vDemoSleep();

private:
    typedef struct
    {
        char    m_szName[64];
        void (CDemo::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif  //  __HHH__DEMO_TEST_HHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

