#ifndef _HHH_IBP_HEAD_HHH__
#define _HHH_IBP_HEAD_HHH__
#include    <libxml2/libxml/xmlmemory.h>
#include    <libxml2/libxml/parser.h>
#include    <libxml2/libxml/xpath.h>
#include    <stdlib.h>
#include    <stdio.h>
#include    <errno.h>
#include    <string.h>
#include    <time.h>
#include    <signal.h>
#include    <stdarg.h>
#include    <getopt.h>
#include    <unistd.h>
#include    <ctype.h>
#include    <math.h>
#include    <sys/types.h>
#include    <semaphore.h>
#include    <sys/syscall.h>
#include    <pthread.h>
#include    <sys/stat.h>
#include    <sys/time.h>
#include    <sys/times.h>
#include    <sys/timeb.h>
#include    <sys/ipc.h>
#include    <sys/shm.h>
#include    <sys/sem.h>
#include    <sys/msg.h>
#include    <unistd.h>
#include    <sys/socket.h>
#include    <sys/utsname.h>
#include    <netinet/tcp.h>
#include    <sys/epoll.h>
#include    <netinet/in.h>
#include    <arpa/inet.h>
#include    <sys/inotify.h>
#include    <curl/curl.h>
#include    <strings.h>
#include    <iconv.h>
#include    <dlfcn.h>
#include    <fcntl.h>
#include    <sys/wait.h>
#include    <dlfcn.h>
#include    <libgen.h>
#include    <openssl/des.h>
#include    <openssl/rsa.h>
#include    <openssl/pem.h>
#include    <openssl/err.h>


typedef unsigned    char    uchar;
/*************************************************************************************************
    函数定义    
 *************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

//    时间处理字符串
extern  char*        sGetDate();                                    /*    获取当前日期 YYYYMMDD*/
extern  char*        _sGetDate(char *p, long l);                    /*    获取当前日期 YYYYMMDD*/
extern  char*        sGetTime();                                    /*    获取当前日期 HHMMSS*/
extern  char*        sGetLogTime();
extern  char*        sGetTimeAlgo(long lMin);                    /*    获取当前时间偏移lMin分钟的时间, 不计秒*/
extern  char*        sGetMinsOfs(char *pszTime, long lOfs);        /*    获取指定时间偏移x分钟的时间，不计秒*/
extern  long         lGetUnixTime();                                /*    获取系统自1970年来的秒数*/
extern  char*        sGetCurrentTime();                            /*    获取当前时间 YYYYMMDDHHMMSS*/
extern  double       dDecRound(double dValue, long lDot);        /*    对double类型保留位数后进行四舍五入处理*/
extern  char*        sGetChgDate(char *pszInDay, long lDay);        /*    获取当前日期偏移lDay天的日期*/
extern  long         lIsValidDate(char *pszDate);


//    文件处理函数
extern  char*        sGetFileValue(char *pszPath, const char *pszName, char *pszKey);    /*    从配置文件读取数据name=value形式*/
extern  char*        sGetDbsEnv(const char *pszName);                    /*    从配置文件去读取Name对应的值*/
extern  char*        sGetRealFile(char *p);                        /*    去掉路径下的文件名*/
extern  char*        sGetLastCwd(char *p);                        /*    获取文件路径的上层目录*/
extern  char*        sDropCharset(char *s, char k);                /*    删除字符串某一个字符*/
extern  long         lGetFileSize(const char* s);                /*    获取文件大小*/

//    字符串相关处理函数
extern  char*        sUpper(char *s);
extern  char*        sLower(char *s);
extern  char*        sTrimCRLF(char *p);
extern  char*        sTrimLeft(char *p);
extern  char*        sTrimRight(char *p);
extern  char*        sTrimAll(char *p);
extern  char*        sAlterPath(char *p, long l, char *s);

extern  long         lTrimCRLF(char *p, long l);                    /*    去掉字符串中的回车换行*/
extern  long         lGetRecordNum(char *p, char *s, long l);    /*    获取p字符串中s字符串出现总次数*/
extern  long         lGetValidNum(char *s, char *k);                /*    获取k分割串后有效个数*/
extern  char*        sGetValueByIdx(char *p, char *s, long lIdx);/*    根据分隔符和域值取数据*/
extern  char*        sEncryAcNo(char *pszActNo);                    /*    账号脱敏*/
extern  char*        sDesensTel(char *s);                        /*    手机号脱敏 */
extern  char*        sDesensAct(char *s);                        /*    账号脱敏 */
extern  char*        sDesensName(char *s);                        /*    名称脱敏 */
extern  char*        sDesensMail(char *s);                        /*    邮件名脱敏 */

extern  long         lStringReplace(char *s, char *o, char *d, char **pp);        /*    字符串替换*/
extern  char*        sCharReplace(char *s, char o, char d);    
extern  long         lStringToHex(char *pszOrg, long lLen, char *pszOut, long lSize);
extern  long         lHexToString(char *pszOrg, long lLen, char *pszOut, long lSize);
extern  void         trmcpy(char *d, char *s, int l);
extern  long         chrcpy(char *d, char *s, char c, int l);
extern  long         btol(char *s, long l);                        //    将压缩BCD码转换成长度
extern  long         ltob(char *s, long t, long l);                //    将长度压缩成压缩BCD码
extern  char*        btop(char *s, long l, char *o, int flag);    //    将bcd码字符串s，处理后为压缩bcd码
extern  char*        ptob(char *s, long j, char *o, long ol);
extern  long         hxtol(char *s, int j);
extern  long         asctol(char *s, long l);
extern  void         ltoasc(char *s, long t, long l);

extern  char*        sGetBlockValue(char *s, const char *o, const char *d);
extern  long         DES_Encrypt(const char *pszInp, const long lInpt,             //    DES CBC加解密
                        const char *pszKey, char **ppszOut, long *plOut, int enc);

extern  long         lUrldecode(char *ucode, int len, char *out, int o);
extern  long         lUrlencode(unsigned char *text, int len, unsigned char *out, int o);
extern  long         base64_decode(char *s, int n, char *d, long o);
extern  long         base64_encode(const char *s, long n, char *d, long *o);
extern  long         lInitRsa(const char *pszPrvfile, const char *pszPubfile);
extern  long         lPackRSAverify(const char *text, long n, char *sig, long len);
extern  long         lPackRSAsign(uchar *text, long len, uchar *out, long *size);
extern  double       Round(double dVal, short iPlaces);

#ifdef __cplusplus
}
#endif

#endif    //    _HHH_IBP_HEAD_HHH__
