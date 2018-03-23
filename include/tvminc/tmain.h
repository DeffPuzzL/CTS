#ifndef _HHH_TVM_DOMAIN_HHH__
#define _HHH_TVM_DOMAIN_HHH__
#include "tvm.h"


typedef struct  epoll_event     epollevt;
typedef long    (*FUNCEXEC)(SATvm *pstSavm, void *arg);

/*************************************************************************************************
  
 *************************************************************************************************/
#define SET_BLOCK                           0
#define SET_UNBLOCK                         1
#define READ_BUFFER                         4096            
#define MAX_EVENTS                          5000
#define READ_MAX_LEN                        1024 * 10
#define DATA_MAX_LEN                        1048576    // 1024*1024    tvm自我保护最大单记录1M
#define MAX_CON_EVENTS                      65535
#define TVM_PORT_LISTEN                     1801
#define TVM_PORT_DOMAIN                     1800
#define TVM_LOCAL_SERV                      "LIS.tvm"
#define TVM_REMOTE_DOM                      "RDS.tvm"
#define LOCAL_HOST_IP                       "127.0.0.1"


/*************************************************************************************************
    表结构&索引定义区
 *************************************************************************************************/
typedef struct __SOCK_CONNECT
{
    char    m_szCltIp[16];
    int     m_lCltPort;
    BSock   m_skSock;
    int     m_isListen;
}SKCon;

typedef struct  __TVM_REMOTE_SYNC
{
    Benum   m_enum;
    int     m_lFind;                          //  match rule
    TABLE   m_table;
    int     m_lRows;                          //  rows
    int     m_lDLen;                          //  condition data
    int     m_lErrno;                         //  errno 
    int     m_lPort;
    int     m_lNLen;                          //  new data
    int     m_lCLine;                         //  for cursor
    int     m_lCType;                         //  cursor fetch data type
    size_t  m_lOffset;
}TRSync;

typedef struct  __TVM_INTERFACE
{
    Benum   m_enum;
    uint    m_lFind;
    TABLE   m_table;
    ulong   m_lRows;
    uint    m_lDLen;
    uint    m_lErrno;
}TFace;

/*************************************************************************************************
 *     宏
 *************************************************************************************************/

/*************************************************************************************************
    函数定义    
 *************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern    void    lInitTitle(int argc, char **argv, char **envp);
extern    void    vSetTitile(const char *name);

extern    long    lBootLocal(SATvm *pstSavm, TBoot *pstBoot);
extern    long    lBootDomain(SATvm *pstSavm, TBoot *pstBoot, long lMode);
extern    long    lMultListen(SATvm *pstSavm, long lPort, long , FUNCEXEC , void *arg, FUNCEXEC );

#ifdef __cplusplus
}
#endif

#endif    //    _HHH_TVM_DOMAIN_HHH__
