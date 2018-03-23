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
class    CBatch
{
public:
    CBatch();
    ~CBatch();

public:
    long    lRunObject(char *, char *);
    inline  char*   sGetVersion()   {   return g_szVersion; }

    long    lUnloadMchtNo(char *pszDate, char *pszSettle);
    long    lLoadMchtNo(char *pszFile, char *pszDate, char *pszSettle);
    long    lCheckResource(SATvm *pstSamo);

    long    lIsNMchtInit(char *pszDate);
    long    lSetInitStatus(char *pszDate, char chStatus);
public:
    void    vDayendMain();
    void    vCreateTvmTable();

    void    vSetBatRun();
    void    vGetBatList();
    void    vInitBatTask();
    void    vSetBatFinish();

private:
    typedef struct
    {
        char    m_szName[64];
        void    (CBatch::*m_pEvent)();
    }stEvent;

    static stEvent  m_stEvent[];
};

/**************************************************************************************************
    code end
 **************************************************************************************************/
