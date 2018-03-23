/**************************************************************************************************
    文 件 名：clear_day.h
    代码作者：DeffPuzzL、liujie
    编写版本：
    创建日期：2016-07-14
    功能描述：demo头文件
 **************************************************************************************************/
#include    "face.h"
#include    "dbsapi.h"

/*************************************************************************************************
    实现秒到系统日切功能
*************************************************************************************************/
class    CClear
{
public:
    CClear();
    ~CClear();

public:
    long    lRunObject(char *, char *);
    inline  char*   sGetVersion()   {   return g_szVersion; }

	long    lCheckResource(SATvm *pstSavm);
public:
	void    vSystemDayend();
	void    vSetSysNormal();
	void    vSetSysPause();
	void    vCreateTvmTable();
	void	vActAmtTrasfer();
	void    vGetSettleDay();
	void    vExecuteScript();

private:
    typedef struct
    {
        char    m_szName[64];
        void    (CClear::*m_pEvent)();
    }stEvent;

    static stEvent  m_stEvent[];
};

/**************************************************************************************************
    code end
 **************************************************************************************************/
