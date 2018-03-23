/**************************************************************************************************
    文 件 名：
    代码作者：
    编写版本：
    创建日期：
    功能描述：
 **************************************************************************************************/
#ifndef     _NCARD_ROUTE_H_
#define     _NCARD_ROUTE_H_

#include    "face.h"


#ifdef  __cplusplus  
extern "C" {  
#endif


extern int dbs_isconn(void);
extern int dbs_reconnect(void);
extern int iNCardRouteFindCmerPro(void);
extern int iNCardRouteStatPro(void);
extern int iNCardRouteSynMchtPro(void);

#ifdef  __cplusplus  
}  
#endif  /* end of __cplusplus */  


/*************************************************************************************************
    无卡路由
 *************************************************************************************************/
class   NCardRoute
{
public:
    NCardRoute();
    ~NCardRoute();

public:
    inline  char   *sGetVersion()
    {
        return g_szVersion;
    }

    long    lRunObject(char *, char *);
public:
    void    vNCardRouteTest();
    void    vNCardRouteWork();
	void    vNCardRouteSynMcht();
	void    vNCardRouteStat();

private:
    typedef struct
    {
        char    m_szName[64];
        void (NCardRoute::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif
/**************************************************************************************************
    code end
 **************************************************************************************************/

