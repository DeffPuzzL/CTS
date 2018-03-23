#ifndef _HHH_TVM_BASE_STR_HHH__
#define _HHH_TVM_BASE_STR_HHH__
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/timeb.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/inotify.h>
#include <strings.h>
#include <iconv.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <libgen.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xmlmemory.h>

typedef short       int     sint;
typedef unsigned    int     uint;
typedef unsigned    long    ulong;
typedef long        long    llong;
typedef unsigned    char    ushar;
typedef unsigned    short   ushort;
typedef unsigned    char    Byte;
typedef long        int     Benum;
typedef int                 BSock;

#ifndef BOOL
typedef unsigned    int  BOOL;
#endif

#ifndef TRUE
#define TRUE				1
#endif

#ifndef FALSE
#define FALSE				0
#endif

#ifndef __cplusplus
#ifndef bool
typedef unsigned    int  bool;
#endif

#ifndef true
#define true				1
#endif

#ifndef false
#define false				0
#endif

#endif	// __cplusplus


#ifndef	RC_SUCC
#define RC_SUCC                             0
#endif

#ifndef	RC_FAIL
#define RC_FAIL                             -1
#endif

#ifndef	RC_CONTINUE
#define RC_CONTINUE                         -5
#endif

#ifndef	RC_CLOSE
#define RC_CLOSE                            -9
#endif

#ifndef	RC_NOTFOUND
#define RC_NOTFOUND                         1403
#endif

#define LOG_DEFU_SIZE                       20480000
#define LOG_DEFU_NAME                       "tvm.log"

/*************************************************************************************************
	函数定义	
 *************************************************************************************************/
typedef struct  __COMLIST
{
    long    m_lSize;
    void    *m_psvData;
    struct  __COMLIST   *pstNext;
    struct  __COMLIST   *pstLast;
}CMList;

typedef struct  __ROWGROUP
{
    long    lLen;
    long    lCount;
    void    *psvData;
	struct  __ROWGROUP   *pstNode;
	struct  __ROWGROUP   *pstPres;
    struct  __ROWGROUP   *pstNext;
    struct  __ROWGROUP   *pstLast;
}Rowgrp;

#ifdef __cplusplus
extern "C" {
#endif

#define MAX(a, b)                           ((a) > (b) ? (a) : (b))
#define MIN(a, b)                           ((a) > (b) ? (b) : (a))
#define BOOL(x)                             (x == 0 ? false : true)
#define TVMrace(...)                        vTraceLog(sGetLog(), __FILE__, __LINE__, 0, __VA_ARGS__)
#define TVMuser(...)                        vTraceLog(sGetLog(), __FILE__, __LINE__, 1, __VA_ARGS__)

extern CMList*     pGetCMTail(CMList *pstRoot);
extern CMList*     pSearchNode(CMList *pstRoot, void *psvData, long lSize);
extern CMList*     pInsertList(CMList *pstRoot, void *pszData, long lSize);
extern CMList*     pDeleteNode(CMList *pstRoot, void *psvData, long lSize);
extern void        vDestroyList(CMList *pstRoot);
extern void        vDeleteRowgrp(Rowgrp *pstNode);

extern char*       supper(char *s);
extern char*       slower(char *s);
extern char*       strimcrlf(char *p);
extern char*       sltrim(char *p);
extern char*       srtrim(char *p);
extern char*       strimall(char *p);
extern char*       strimfield(char *s);
extern char*       strimcpy(char *d, char *s, int l);
extern char*       strimabout(char *s, char *o, char *d);
extern long        lfieldnum(char *p, char *k);
extern char*       sfieldvalue(char *p, char *k, int id);
extern char*       sgetvalue(char *p, char *s, int id);
extern char*       sfieldreplace(char *p, char o, char d);
extern void        vTraceLog(char *pszLog, const char *pszFile, long lLine, long lState, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif	//	_HHH_TVM_BASE_STR_HHH__
