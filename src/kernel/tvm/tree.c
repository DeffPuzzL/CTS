#include    "tvm.h"
#include    "tmain.h"

/*************************************************************************************************
    内部函数注册
 *************************************************************************************************/
extern long    lCountRowgrp(Rowgrp *pstRoot, void *psvData, long lLen);
extern void    vSortRowgrp(Rowgrp **pproot, TblKey *pstKey, Benum em);
extern long    lSortRowList(SATvm *, size_t lRow, void *pvData, TblKey *pstKey, long lTruck);
extern Rowgrp* pInsertRowgrp(SATvm *, Rowgrp *pstRoot, Rowgrp *pstPres, void *psvData, long lLen);

/*************************************************************************************************
    全局变量定义 
 *************************************************************************************************/
SATvm   g_stSavm = {0};
TBoot   g_stBoot = {0};
TblDef  g_stTblDef[TVM_MAX_TABLE] = {{0}};


static char    tvmerr[100][MAX_INDEX_LEN] = {
    "completed successfully",
    "sever exception",
    "index field values is null",
    "condition is null",
    "no space for insert data",
    "generate shm key failure",
    "Invalid parameter or shm has disappeared",
    "shared memory already exists",
    "shared memory has been deleted",
    "Permission denied .shm",
    "Insufficient core memory .shm",
    "data truck version mismatch",
    "size is error to creat data block",
    "unique index definition overflow",
    "unique index length overflow",
    "normal index definition overflow",
    "normal index length overflow",
    "index type not define",
    "field define overflow",
    "index data mismatch",
    "field type not define",
    "memory has not been initialized",
    "unique index repeat",
    "no space for create index",
    "no data be found",
    "more then one records be selected",
    "malloc memory error",
    "cursor invalid",
    "table not define",
    "file not exist",
    "semget condition is null",
    "Invalid parameter or sem has disappeared",
    "semaphore already exists",
    "semaphore has been deleted",
    "Permission denied .sem",
    "Insufficient core memory .sem",
    "Semaphore value out of limit",
    "SQL syntax is error",
    "SQL syntax not be supported",
    "SQL no table name be inputted",
    "SQL field is not selected",
    "SQL conditional syntax error",
    "SQL field syntax error",
    "SQL where syntax error",
    "table not found",
    "SQL fields does not match the value",
    "set the read lock failure",
    "unlock read lock failure",
    "set the write lock failure",
    "unlock write lock failure",
    "socket connect failure",
    "socket connect timeout",
    "create socket failure",
    "socket recv failure",
    "socket send failure",
    "socket bind failure",
    "socket listen failure",
    "socket send timeout",
    "socket read timeout",
    "socket reset by peer",
    "epoll add fd error",
    "create epoll fd failure",
    "delete epoll fd failure",
    "socket accept failure",
    "SQL remote does not support",
    "file not found",
    "boot parameters error",
    "parameters table related error",
    "Incompatible version",
    "domain not register",
    "domain work does not support",
    "sequence does not exist",
    "file is not set",
    "record data too long",
    "Resource unavailable",
    "message queue already exists",
    "Permission denied .msg",
    "Insufficient(msg) core memory",
    "Invalid parameter or msg has disappeared",
    "message Invalid address",
    "message queue has been deleted",
    "message text length is greater than msgsz",
    "Interrupted by signal",
    "msgsnd queue overrun",
    "initial child process failed",
    "field not exist",
    "table already exist",
    "The transaction has not been opened yet",
    "",
};

/*************************************************************************************************
    函数说明：用户错误信息自定义
    参数说明：
    返回值：
 *************************************************************************************************/
void    vRedeError(long err, char *s)
{
    if(100 <= err || !s)
        return ;
    strncpy(tvmerr[err], s, MAX_INDEX_LEN - 1);
}

/*************************************************************************************************
 *     函数定义
 *************************************************************************************************/
void    vPrintHex(char *s, long lIdx, BOOL bf)
{
    int     i = 0;
    FILE    *fp = NULL;    

    if(!bf)
    {    
        for(i = 0; i < lIdx; i ++)
        {
            if((ushar)s[i] > 0x80 && (ushar)s[i + 1] >= 0x40)
            {
                fprintf(stdout, "\033[0;35m%c%c\033[0m", s[i], s[i + 1]);
                ++ i;
                continue;
            }
            
            if(s[i] > 40 && s[i] < 126)
                fprintf(stdout, "\033[0;35m%C\033[0m", s[i]);
            else
                fprintf(stdout, "/x%0X ", (ushar)s[i]);
        }
    }
    else
    {
        fp = fopen("log.txt", "a");
        fwrite(s, 1, lIdx,  fp);
        fclose(fp);
    }

    fprintf(stdout, "\n");
    fflush(stdout);
}

/*************************************************************************************************
    函数说明：系统表创建
    参数说明：
    返回值：
 *************************************************************************************************/
CREATE  lCreateTvmIndex()
{   
    DEFINE(SYS_TVM_INDEX, "SYS_TVM_INDEX", "", TIndex)
    FIELD(TIndex,    m_table,        "m_table",        FIELD_LONG)
    FIELD(TIndex,    m_lType,        "m_lType",        FIELD_LONG)
    FIELD(TIndex,    m_szTable,      "m_szTable",      FIELD_CHAR)
    FIELD(TIndex,    m_szPart,       "m_szPart",       FIELD_CHAR)
    FIELD(TIndex,    m_szOwner,      "m_szOwner",      FIELD_CHAR)
    FIELD(TIndex,    m_yKey,         "m_yKey",         FIELD_LONG)
    FIELD(TIndex,    m_shmID,        "m_shmID",        FIELD_LONG)
    FIELD(TIndex,    m_semID,        "m_semID",        FIELD_LONG)
    FIELD(TIndex,    m_lPid,         "m_lPid",         FIELD_LONG)
    FIELD(TIndex,    m_bAttch,       "m_bAttch",       FIELD_LONG)
    FIELD(TIndex,    m_lValid,       "m_lValid",       FIELD_LONG)
    FIELD(TIndex,    m_lMaxRows,     "m_lMaxRows",     FIELD_LONG)
    FIELD(TIndex,    m_lRowSize,     "m_lRowSize",     FIELD_LONG)
    FIELD(TIndex,    m_lLocal,       "m_lLocal",       FIELD_LONG)
    FIELD(TIndex,    m_lState,       "m_lState",       FIELD_LONG)
    FIELD(TIndex,    m_lPers,        "m_lPers",        FIELD_LONG)  
    FIELD(TIndex,    m_szRemote,     "m_szRemote",     FIELD_CHAR)
    FIELR(TIndex,    m_szTime,       "m_szTime",       FIELD_CHAR)

    CREATE_IDX(NORMAL)
    IDX_FIELD(TIndex, m_szTable,     FIELD_CHAR);
    IDX_FIELD(TIndex, m_szPart,      FIELD_CHAR);

    CREATE_IDX(UNQIUE)
    IDX_FIELD(TIndex, m_table,       FIELD_LONG);
    FINISH
}

/*************************************************************************************************
    函数说明：系统字段表创建
    参数说明：
    返回值：
 *************************************************************************************************/
CREATE  lCreateTvmField()
{   
    DEFINE(SYS_TVM_FIELD, "SYS_TVM_FIELD", "", TField)
    FIELD(TField,    m_table,        "m_table",        FIELD_LONG)  
    FIELD(TField,    m_szOwner,      "m_szOwner",      FIELD_CHAR)  
    FIELD(TField,    m_szTable,      "m_szTable",      FIELD_CHAR)  
    FIELD(TField,    m_szField,      "m_szField",      FIELD_CHAR)  
    FIELD(TField,    m_lSeq,         "m_lSeq",         FIELD_LONG)  
    FIELD(TField,    m_lAttr,        "m_lAttr",        FIELD_LONG)  
    FIELD(TField,    m_lFrom,        "m_lFrom",        FIELD_LONG)  
    FIELD(TField,    m_lLen,         "m_lLen",         FIELD_LONG)  
    FIELD(TField,    m_lIsPk,        "m_lIsPk",        FIELD_LONG)  

    CREATE_IDX(NORMAL)
    IDX_FIELD(TField, m_table,       FIELD_LONG);

    CREATE_IDX(UNQIUE)
    IDX_FIELD(TField, m_table,       FIELD_LONG);
    IDX_FIELD(TField, m_lSeq,        FIELD_LONG);
    FINISH
}

/*************************************************************************************************
    函数说明：系统字段表创建
    参数说明：
    返回值：
 *************************************************************************************************/
CREATE  lCreateTvmDomain()
{   
    DEFINE(SYS_TVM_DOMAIN, "SYS_TVM_DOMAIN", "", TDomain)
    FIELD(TDomain,    m_skSock,       "m_skSock",       FIELD_LONG)  
    FIELD(TDomain,    m_lLIPort,      "m_lLIPort",      FIELD_LONG)  
    FIELD(TDomain,    m_lLDPort,      "m_lLDPort",      FIELD_LONG)  
    FIELD(TDomain,    m_table,        "m_table",        FIELD_LONG)  
    FIELD(TDomain,    m_mtable,       "m_mtable",       FIELD_LONG)  
    FIELD(TDomain,    m_lLock,        "m_lLock",        FIELD_LONG)  
    FIELD(TDomain,    m_lGroup,       "m_lGroup",       FIELD_LONG)  
    FIELD(TDomain,    m_lKeepLive,    "m_lKeepLive",    FIELD_LONG)  
    FIELD(TDomain,    m_lLastTime,    "m_lLastTime",    FIELD_LONG)  
    FIELD(TDomain,    m_lTimeOut,     "m_lTimeOut",     FIELD_LONG)  
    FIELD(TDomain,    m_lTryMax,      "m_lTryMax",      FIELD_LONG)  
    FIELD(TDomain,    m_lTryTimes,    "m_lTryTimes",    FIELD_LONG)  
    FIELD(TDomain,    m_lRowSize,     "m_lRowSize",     FIELD_LONG)  
    FIELD(TDomain,    m_lStatus,      "m_lStatus",      FIELD_LONG)  
    FIELD(TDomain,    m_lPers,        "m_lPers",        FIELD_LONG)  
    FIELD(TDomain,    m_lRIPort,      "m_lRIPort",      FIELD_LONG)  
    FIELD(TDomain,    m_lRDPort,      "m_lRDPort",      FIELD_LONG)  
    FIELD(TDomain,    m_szIp,         "m_szIp",         FIELD_CHAR)  
    FIELD(TDomain,    m_szTable,      "m_szTable",      FIELD_CHAR)  
    FIELD(TDomain,    m_szPart,       "m_szPart",       FIELD_CHAR)  
    FIELD(TDomain,    m_szOwner,      "m_szOwner",      FIELD_CHAR)  

    CREATE_IDX(NORMAL)
    IDX_FIELD(TDomain, m_szTable,     FIELD_CHAR)
    IDX_FIELD(TDomain, m_szPart,      FIELD_CHAR)
/*
    CREATE_IDX(UNQIUE)
    IDX_FIELD(TDomain, m_table,       FIELD_LONG)
    IDX_FIELD(TDomain, m_szTable,     FIELD_CHAR)
    IDX_FIELD(TDomain, m_lRDPort,     FIELD_LONG);
*/

    FINISH
}

/*************************************************************************************************
    函数说明：系统序列表
    参数说明：
    返回值：
 *************************************************************************************************/
CREATE  lCreateTvmSeque()
{   
    DEFINE(SYS_TVM_SEQUE, "SYS_TVM_SEQUE", "", TSeque)
    FIELD(TSeque,    m_szSQName,     "m_szSQName",      FIELD_CHAR) 
    FIELD(TSeque,    m_uIncrement,   "m_uIncrement",    FIELD_LONG) 

    CREATE_IDX(UNQIUE)
    IDX_FIELD(TSeque, m_szSQName,      FIELD_CHAR)

    FINISH
}

/*************************************************************************************************
 * 函数定义区
 *************************************************************************************************/
char*   sGetUpdTime()
{
    time_t  current;
    struct  tm *tm = NULL;
    static  char    szTime[20];

    tzset();
    time(&current);
    tm = localtime(&current);

    memset(szTime, 0, sizeof(szTime));
    snprintf(szTime, sizeof(szTime), "%4d/%02d/%02d %02d:%02d:%02d", tm->tm_year + 1900,
        tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    return szTime;
}

/****************************************************************************************
    对整形0的转换
 ****************************************************************************************/
long    lSetEXLong(long n)
{
    if(n <= 0)
        n += NUMBER_ZEOR;
    return n;
}

/****************************************************************************************
    对整形0的转换
 ****************************************************************************************/
long    lGetEXLong(long *pn)
{
    if(*pn <= NUMBER_ZEOR)
        *pn -= NUMBER_ZEOR;
    return *pn;
}

/*************************************************************************************************
    函数说明：检查字段是否全空(0x00)
    参数说明：
    返回值：
        TRUE                    --是
        FALSE                    --否
 *************************************************************************************************/
BOOL    bIsRealZore(char *s, long l)
{
    register    int    i;
    static  char szIdx[1024] = {0};

    if(l < 1024)
    {
        if(!memcmp(szIdx, s, l))  return TRUE;
        else                      return FALSE;
    }
    else
    {
        for(i = 0; i < l; i ++)
        {
            if(0x00 != s[i])
                return FALSE;
        }
    }

    return TRUE;
}

/*************************************************************************************************
    函数说明：根据索引(组成索引的任何字段不能为\0)
    参数说明：
    返回值：
        TRUE                    --是
        FALSE                    --否
 *************************************************************************************************/
char*    pGetIndex(SATvm *pstSavm, long lIdx, TblKey *pstKey, void *psVoid, char *pszIdx)
{
    register    int        i, j;

    if(!psVoid)        return NULL;

    if(NIL_IGNORE & pstSavm->lFind)
    {
        for(i = 0, j = 0; i < lIdx; i ++)
        {
            memcpy(pszIdx + j, psVoid + pstKey[i].m_lFrom, pstKey[i].m_lLen);
            j += pstKey[i].m_lLen;
        }

        if(bIsRealZore(pszIdx, j))
            return NULL;

        return pszIdx;
    }
    else
    {
        for(i = 0, j = 0; i < lIdx; i ++)
        {
            if(bIsRealZore(psVoid + pstKey[i].m_lFrom, pstKey[i].m_lLen))
                return NULL;
        
            memcpy(pszIdx + j, psVoid + pstKey[i].m_lFrom, pstKey[i].m_lLen);
            j += pstKey[i].m_lLen;
        }
    }

    return pszIdx;
}

/*************************************************************************************************
    函数说明：获取启动参数结果信息
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
void*   pGetBoot()
{
    return &g_stBoot;
}

/*************************************************************************************************
    函数说明：获取当前配置节点
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
char*   sGetNode(char *s)
{
    if(0x00 == *s)
        strcpy(s, g_stBoot.m_szNode);
    return s;
}

/*************************************************************************************************
    函数说明：
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
long    lGetBootType()
{
    return g_stBoot.m_lBootType;
}


long    lGetDomPort()
{
    return g_stBoot.m_lDomPort;
}

/*************************************************************************************************
    函数说明：获取配置日志名称
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
char*    sGetLog()
{
    return g_stBoot.m_szLog;
}

/*************************************************************************************************
    函数说明：获取查询结构
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
void*   pGetSATvm()
{
    return &g_stSavm;
}

/*************************************************************************************************
    函数说明：获取操作错误码
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
void    vInitSATvm(SATvm *pstSavm)
{
    memset(pstSavm, 0, sizeof(SATvm));
}

/*************************************************************************************************
    函数说明：用户自定义错误信息
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
void    vSetTvmMsg(SATvm *pstSamo, char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(pstSamo->m_szMsg, sizeof(pstSamo->m_szMsg), fmt, ap);
    va_end(ap);
}

/*************************************************************************************************
    函数说明：获取用户自定义错误信息
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
char*    sGetTvmMsg(SATvm *pstSamo)
{
    return pstSamo->m_szMsg;
}

/*************************************************************************************************
    函数说明：获取操作错误码
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
long    lGetTErrno()
{
    return g_stSavm.m_lErrno; 
}

/*************************************************************************************************
    函数说明：设置错误码
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
void    vSetTErrno(long err)
{
    g_stSavm.m_lErrno = err;    
}
/*************************************************************************************************
    函数说明：获取操作错误信息
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
char*    sGetTError(long err)
{
    return tvmerr[(uint)err];
}

/*************************************************************************************************
    函数说明：获取操作tvm更新。删除记录影响记录数
    参数说明：
    返回值：
        long                    --操作记录数
 *************************************************************************************************/
long    lGetEffect()
{
    return  g_stSavm.m_lEffect;
}

/*************************************************************************************************
    函数说明：获取游标操作运行结构
    参数说明：
    返回值：
        RunTime                    --获取操作运行结构
 *************************************************************************************************/
RunTime*   pGetRunTime(SATvm *pstSavm, TABLE t)
{
    return &pstSavm->stRunTime[t];
}

/*************************************************************************************************
    函数说明：获取查询结构
    参数说明：
    返回值：
        void                    --指针
 *************************************************************************************************/
void*    pGetAddr(SATvm *pstSavm, TABLE t)
{
    return ((RunTime *)pGetRunTime(pstSavm, t))->m_pvAddr;
}

/*************************************************************************************************
    函数说明：获取查询结构
    参数说明：
    返回值：
        BOOL                    --
 *************************************************************************************************/
BOOL    bIsAttch(SATvm *pstSavm, TABLE t)
{
    return ((RunTime *)pGetRunTime(pstSavm, t))->m_bAttch;
}

/*************************************************************************************************
    函数说明：获取表结构信息 
    参数说明：
        t                        --表序号
    返回值：
        void                    --指针
 *************************************************************************************************/
TblDef* pGetTblDef(TABLE t)
{
    return &g_stTblDef[t];
}

/*************************************************************************************************
    函数说明：获取锁信息
    参数说明：
        pstRun                     --表序号
    返回值：
        void                       --指针
 *************************************************************************************************/
RWLock* pGetRWLock(char* pvAddr)
{
    return &((TblDef *)pvAddr)->m_rwlock;
}

/*************************************************************************************************
    函数说明：初始化创建结构信息
    参数说明：
        t                        --表序号
    返回值：
        无
 *************************************************************************************************/
void    vInitTblDef(TABLE t)
{
    memset(&g_stTblDef[t], 0, sizeof(TblDef));
}

/*************************************************************************************************
    函数说明：获取表组个数
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
long    lGetTblGroup(TABLE t)
{
    return g_stTblDef[t].m_lGroup;
}

/*************************************************************************************************
    函数说明：获取表数据最大支持记录个数
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
long    lGetTblRow(TABLE t)
{
    return g_stTblDef[t].m_lMaxRow;
}

/*************************************************************************************************
    函数说明：获取表当前存放的记录总个数
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
long    lGetTblValid(TABLE t)
{
    return g_stTblDef[t].m_lValid;
}

/*************************************************************************************************
    函数说明：获取表主索引长度
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
long    lGetIdxLen(TABLE t)
{
    return g_stTblDef[t].m_lIdxLen;
}

/*************************************************************************************************
    函数说明：获取表索引的位置
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetIdxPos(TABLE t)
{
    return g_stTblDef[t].m_lTreePos;
}

/*************************************************************************************************
    函数说明：获取表索引的位置Root节点
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetIdxRoot(TABLE t)
{
    return g_stTblDef[t].m_lTreeRoot;
}

/*************************************************************************************************
    函数说明：获取表索引的位置Nil节点
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
long    lGetNodeNil(TABLE t)
{
    return g_stTblDef[t].m_lNodeNil;
}

/*************************************************************************************************
    函数说明：获取表查询索引组长度
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
long    lGetGrpLen(TABLE t)
{
    return g_stTblDef[t].m_lGrpLen;
}

/*************************************************************************************************
    函数说明：获取表查询索引组的位置
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetGrpPos(TABLE t)
{
    return g_stTblDef[t].m_lGroupPos;
}

/*************************************************************************************************
    函数说明：获取表索引的位置Root节点
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetGrpRoot(TABLE t)
{
    return g_stTblDef[t].m_lGroupRoot;
}

/*************************************************************************************************
    函数说明：获取表索引组列表起始区
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetListOfs(TABLE t)
{
    return g_stTblDef[t].m_lListOfs;
}

/*************************************************************************************************
    函数说明：获取表索引组列表起始区
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetListPos(TABLE t)
{
    return g_stTblDef[t].m_lListPos;
}

/*************************************************************************************************
    函数说明：获取表数据区位置
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetTblData(TABLE t)
{
    return g_stTblDef[t].m_lData;
}

/*************************************************************************************************
    函数说明：获取唯一索引组合个数
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
uint    lGetIdxNum(TABLE t)
{
    return g_stTblDef[t].m_lIdxUp;
}

/*************************************************************************************************
    函数说明：获取唯一索引组合结构信息
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
TblKey* pGetTblIdx(TABLE t)
{
    return  g_stTblDef[t].m_stIdxUp;
}

/*************************************************************************************************
    函数说明：获取索引组合个数
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
uint    lGetGrpNum(TABLE t)
{
    return g_stTblDef[t].m_lGrpUp;
}

/*************************************************************************************************
    函数说明：获取索引组合结构信息
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
TblKey* pGetTblGrp(TABLE t)
{
    return  g_stTblDef[t].m_stGrpUp;
}

/*************************************************************************************************
    函数说明：获取整张表大小
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetTableSize(TABLE t)    
{
    return g_stTblDef[t].m_lTable;
}

/*************************************************************************************************
    函数说明：获取表定义的单条记录大小
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
long    lGetRowSize(TABLE t)
{
    return g_stTblDef[t].m_lReSize;
}

/*************************************************************************************************
    函数说明：获取表定义的单条数据块大小
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
size_t    lGetRowTruck(TABLE t)
{
    return g_stTblDef[t].m_lTruck;
}

/*************************************************************************************************
    函数说明：获取表字段信息个数
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
long    lGetFldNum(TABLE t)
{
    return g_stTblDef[t].m_lIdxNum;
}

/*************************************************************************************************
    函数说明：获取表字段信息
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
TblKey* pGetTblKey(TABLE t)
{
    return  g_stTblDef[t].m_stKey;
}

/*************************************************************************************************
    函数说明：获取表单个字段信息
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
TblKey*    pGetFldKey(TABLE t, long lField)
{
    long    i = 0;
    TblKey  *pstKey = (TblKey *)pGetTblKey(t);

    for(i = 0; i < lGetFldNum(t); i ++)
    {
        if(pstKey[i].m_lFrom == REFrom(lField) && pstKey[i].m_lLen == REFLen(lField))
            return &pstKey[i];
    }

    return NULL;
}

/*************************************************************************************************
    函数说明：检查是否的索引组字段
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
BOOL    bIsGroupKey(TABLE t, TblKey *pstIdx)
{
    long    i = 0;
    TblKey  *pstKey = (TblKey *)pGetTblGrp(t);

    for(i = 0; i < lGetGrpNum(t); i ++)
    {
        if(pstKey[i].m_lFrom == pstIdx->m_lFrom && pstKey[i].m_lLen == pstIdx->m_lLen)
            return TRUE;
    }

    return FALSE;
}

/*************************************************************************************************
    函数说明：获取表字段信息
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
char* sGetTableName(TABLE t)
{
    return  g_stTblDef[t].m_szTable;
}

/*************************************************************************************************
    函数说明：获取表分块字段
    参数说明：
        t                        --表序号
    返回值：
        long                    --偏移量
 *************************************************************************************************/
char* sGetTablePart(TABLE t, char *pszNode)
{
    if(0x00 == g_stTblDef[t].m_szPart[0])
        strncpy(g_stTblDef[t].m_szPart, pszNode, MAX_FIELD_LEN);
    return  g_stTblDef[t].m_szPart;
}

/*************************************************************************************************
    函数说明：设置当前句柄环境操作的内存表长链
    参数说明：
    返回值：
        无
 *************************************************************************************************/
void    vTvmHoldConnect(SATvm *pstSavm)
{
    pstSavm->m_bHold = TRUE;
}

/*************************************************************************************************
    函数说明：关闭当前句柄环境操作的内存表长链标识(每次操作表自动断开)
    参数说明：
    返回值：
        无
 *************************************************************************************************/
void    vTvmHoldRelease(SATvm *pstSavm)
{
    TABLE    t;
    RunTime    *pstRun = NULL;

    pstSavm->m_bHold = FALSE;
    for(t = 0; t < TVM_MAX_TABLE; t ++)
    {
        pstRun = (RunTime *)pGetRunTime(pstSavm, t);
        if(RES_REMOT_SID == pstRun->m_lLocal)
            continue;

        pstRun->m_lCurType = 0;
        pstRun->m_lCurLine = 0;
        pstRun->m_pvCurAddr = NULL;
        IBPFree(pstRun->pstVoid);

        if(pstRun->m_pvAddr)
            shmdt(pstRun->m_pvAddr);
        pstRun->m_pvAddr = NULL;
        pstRun->m_bAttch = FALSE;
    }

    return ;
}

/*************************************************************************************************
    函数说明：释放资源
    参数说明：
        t                        --表序号
    返回值：
        无    
 *************************************************************************************************/
void    _vTblRelease(SATvm *pstSavm, TABLE t, BOOL bHold)
{
    RunTime    *pstRun = (RunTime *)pGetRunTime(pstSavm, t);

    IBPFree(pstRun->pstVoid);
    pstSavm->lReField = 0;
    pstSavm->lReMatch = 0;
    if(bHold)    return ;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return ;

    pstRun->m_lCurType = 0;
    pstRun->m_lCurLine = 0;
    pstRun->m_pvCurAddr = NULL;

    if(pstRun->m_pvAddr)
        shmdt(pstRun->m_pvAddr);
    pstRun->m_pvAddr = NULL;
    pstRun->m_bAttch = FALSE;
}

/*************************************************************************************************
    函数说明：关闭表资源
    参数说明：
        t                        --表序号
    返回值：
        无    
 *************************************************************************************************/
void    vTblDisconnect(SATvm *pstSavm, TABLE t)
{
    _vTblRelease(pstSavm, t, pstSavm->m_bHold);
}

/*************************************************************************************************
    函数说明：强制关闭表资源
    参数说明：
        t                        --表序号
    返回值：
        无    
 *************************************************************************************************/
void    vForceDisconnect(SATvm *pstSavm, TABLE t)
{
    _vTblRelease(pstSavm, t, false);
}

/*************************************************************************************************
    函数说明：启动事务
    参数说明：
    返回值：
        无    
 *************************************************************************************************/
void    vBeginWork(SATvm *pstSavm)
{
    if(!pstSavm)    return ;

    pstSavm->m_bWork = true;
}

/*************************************************************************************************
    函数说明：关闭事务
    参数说明：
    返回值：
        无    
 *************************************************************************************************/
void    vEndWork(SATvm *pstSavm)
{
    if(!pstSavm)    return ;

    pstSavm->m_bWork = false;
    lCommitWork(pstSavm);
}

/*************************************************************************************************
    函数说明：回滚当前事务
    参数说明：
    返回值：
        无    
 *************************************************************************************************/
long    lRollbackWork(SATvm *pstSavm)
{
    size_t    lRow = 0;
    TWork   *pstWork = NULL;
    CMList  *pstNode = NULL;

    pstSavm->m_lEffect = 0;
    if(!pstSavm || !pstSavm->m_bWork)
    {
        pstSavm->m_lErrno = WORK_NOT_OPEN;
        return RC_FAIL;
    }

    for(pstNode = pGetCMTail(pstSavm->m_pstWork); pstNode; pstNode = pstNode->pstLast)
    {
        pstWork = (TWork *)pstNode->m_psvData;
        if(!pstWork || !pstWork->m_pvData)    continue;

        if(NULL == (pstSavm = (SATvm *)pInitSATvm(pstWork->m_table)))
            return RC_FAIL;

        pstSavm->m_bPrew = true;
        pstSavm->lFind   = pstWork->m_lFind;
        pstSavm->lSize   = pstWork->m_lRowSize;
        if(OPERATE_UPDATE == pstWork->m_lOperate)
        {
            pstSavm->pstVoid = (void *)pstWork->m_pvNew;
            if(RC_SUCC != lUpdate(pstSavm, (void *)pstWork->m_pvData, true))
                return RC_FAIL;
        }
        else if(OPERATE_DELETE == pstWork->m_lOperate)
        {
            pstSavm->pstVoid = (void *)pstWork->m_pvData;
            if(RC_SUCC != lInsert(pstSavm))
                return RC_FAIL;
        }
        else if(OPERATE_INSERT == pstWork->m_lOperate)
        {
            pstSavm->pstVoid = (void *)pstWork->m_pvData;
            if(RC_SUCC != lDelete(pstSavm))
                return RC_FAIL;
        }

        lRow += pstSavm->m_lEffect;
        IBPFree(pstWork->m_pvNew);    
        IBPFree(pstWork->m_pvData);    
    }
    pstSavm->m_lEffect = lRow;
    vDestroyList(pstSavm->m_pstWork);
    pstSavm->m_pstWork = NULL;
    pstSavm->m_bPrew = false;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：假性提交事务
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lCommitWork(SATvm *pstSavm)
{
    TWork   *pstWork = NULL;
    CMList  *pstNode = NULL, *pstList = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    for(pstNode = pstSavm->m_pstWork; pstNode; pstNode = pstNode->pstNext)
    {
        pstList = pstNode;
        pstNode = pstNode->pstNext;
        pstWork = (TWork *)pstList->m_psvData;
        if(pstWork)
        {
            IBPFree(pstWork->m_pvNew);    
            IBPFree(pstWork->m_pvData);    
        }

        IBPFree(pstList->m_psvData);
        IBPFree(pstList);
        if(!pstNode)    break;
    }
    pstSavm->m_pstWork = NULL;
    pstSavm->m_bPrew = false;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：记录操作记录
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lRecordWork(SATvm *pstSavm, void *pvNew, long lOperate)
{
    TWork    stWork;

    if(!pstSavm->m_bWork || pstSavm->m_bPrew)
        return RC_SUCC;

    memset(&stWork, 0, sizeof(TWork));
    stWork.m_lOperate = lOperate;
    stWork.m_lRowSize = pstSavm->lSize;
    stWork.m_table    = pstSavm->tblName;
    if(NULL == (stWork.m_pvData = (void *)malloc(pstSavm->lSize)))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    memcpy(stWork.m_pvData, pvNew, pstSavm->lSize);

    if(OPERATE_UPDATE == lOperate)
    {
        if(NULL == (stWork.m_pvNew = (void *)malloc(pstSavm->lSize)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_FAIL;
        }

        memcpy(stWork.m_pvNew, pstSavm->pstVoid, pstSavm->lSize);
    }

    if(NULL == (pstSavm->m_pstWork = pInsertList(pstSavm->m_pstWork, &stWork, sizeof(TWork))))
        return RC_FAIL;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：初始化二级缓存
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lInitSvCache(SATvm *pstSavm)
{
    size_t  lOut = 0, i;
    RunTime *pstRun = NULL;
    TIndex  *pstIndex = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(RC_SUCC != lExportTable(SYS_TVM_INDEX, &lOut, (void *)&pstIndex))
        return RC_FAIL;

    for(i = 0; i < lOut; i ++)
    {
        pstRun = (RunTime *)pGetRunTime(pstSavm, pstIndex[i].m_table);
        pstRun->m_lState = RESOURCE_ABLE;
        pstRun->tblName  = pstIndex[i].m_table;
        pstRun->m_shmID  = pstIndex[i].m_shmID;
        pstRun->m_semID  = pstIndex[i].m_semID;
        pstRun->m_lLocal = pstIndex[i].m_lLocal;
        strncpy(pstRun->m_szPart, pstIndex[i].m_szPart, sizeof(pstRun->m_szPart));
        strncpy(pstRun->m_szTable, pstIndex[i].m_szTable, sizeof(pstRun->m_szTable));
        strncpy(pstRun->m_szIP, sgetvalue(pstIndex[i].m_szRemote, ":", 1), sizeof(pstRun->m_szIP));
        pstRun->m_lPort = atol(sgetvalue(pstIndex[i].m_szRemote, ":", 2));
        pstRun->m_lTime = atol(sgetvalue(pstIndex[i].m_szRemote, ":", 3));
        if(pstRun->m_lPort <= 0)        pstRun->m_lPort = TVM_PORT_LISTEN;
    }
    
    pstSavm->m_bCache = TRUE;
    pstSavm->bSearch  = TYPE_CLIENT;
    IBPFree(pstIndex);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取连接创建共享内存的KEY值
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
void*   pPartSATvm(SATvm *pstSavm, char *pszTable, char *pszPart)
{
    TABLE    i;
    TIndex  stIndex;
    RunTime *pstRun = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return NULL;
    }

    for(i = 0; i < TVM_MAX_TABLE; i ++)
    {
        pstRun = (RunTime *)pGetRunTime(pstSavm, pstSavm->tblName);
        if(RESOURCE_ABLE != pstRun->m_lState)
            continue;

        if(!strcmp(pstRun->m_szPart, pszPart) && !strcmp(pstRun->m_szTable, pszTable))
        {
            pstSavm->pstVoid = NULL;
            pstSavm->bSearch = TYPE_CLIENT;
            pstSavm->tblName = pstRun->tblName;
            return pstSavm;
        }
    }
    
    memset(&stIndex, 0, sizeof(TIndex));
    strncpy(stIndex.m_szPart, pszPart, sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, pszTable, sizeof(stIndex.m_szTable));

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;
    if(RC_SUCC != lSelect(pstSavm, (void *)&stIndex))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            pstSavm->m_lErrno = TBL_NOT_FOUND;
        return NULL;
    }

    pstSavm->pstVoid   = NULL;
    pstSavm->bSearch   = TYPE_CLIENT;
    pstSavm->tblName = stIndex.m_table;
    pstRun = (RunTime *)pGetRunTime(pstSavm, pstSavm->tblName);
       pstRun->m_shmID  = stIndex.m_shmID;
       pstRun->m_semID  = stIndex.m_semID;
    pstRun->m_lLocal = stIndex.m_lLocal;
    if(RES_LOCAL_SID == pstRun->m_lLocal)
        return pstSavm;

    strncpy(pstRun->m_szIP, sgetvalue(stIndex.m_szRemote, ":", 1), sizeof(pstRun->m_szIP));
    pstRun->m_lPort = atol(sgetvalue(stIndex.m_szRemote, ":", 2));
    pstRun->m_lTime = atol(sgetvalue(stIndex.m_szRemote, ":", 3));
    if(pstRun->m_lPort <= 0)    
        pstRun->m_lPort = TVM_PORT_LISTEN;

    return pstSavm;
}

/*************************************************************************************************
    函数说明：获取连接创建共享内存的KEY值
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lInitSATvm(SATvm *pstSavm, TABLE t)
{
    TIndex  stIndex;
    RunTime *pstRun  = NULL;

    pstSavm->tblName   = t;
    pstRun = (RunTime *)pGetRunTime(pstSavm, t);
    if(pstRun->m_bAttch && pstRun->m_pvAddr)
        return RC_SUCC;

    pstRun = (RunTime *)pGetRunTime(pstSavm, pstSavm->tblName);
    if(RESOURCE_ABLE == pstRun->m_lState)
    {
           pstSavm->pstVoid = NULL;
        pstSavm->bSearch = TYPE_CLIENT;
        return RC_SUCC;
    }

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_table = t;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;
    if(RC_SUCC != lSelect(pstSavm, (void *)&stIndex))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            pstSavm->m_lErrno = TBL_NOT_FOUND;
        return RC_FAIL;
    }

    pstSavm->tblName   = t;
    pstSavm->pstVoid   = NULL;
    pstSavm->bSearch   = TYPE_CLIENT;
    strcpy(pstSavm->m_szNode, stIndex.m_szOwner);
    pstRun  = (RunTime *)pGetRunTime(pstSavm, t);
    pstRun->m_shmID    = stIndex.m_shmID;
    pstRun->m_semID    = stIndex.m_semID;
    pstRun->m_lLocal   = stIndex.m_lLocal;
    if(RES_LOCAL_SID == pstRun->m_lLocal)
        return RC_SUCC;

    strncpy(pstRun->m_szIP, sgetvalue(stIndex.m_szRemote, ":", 1), sizeof(pstRun->m_szIP));
    pstRun->m_lPort = atol(sgetvalue(stIndex.m_szRemote, ":", 2));
    pstRun->m_lTime = atol(sgetvalue(stIndex.m_szRemote, ":", 3));
    if(pstRun->m_lPort <= 0)    pstRun->m_lPort = TVM_PORT_LISTEN;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取连接创建共享内存的KEY值
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
void*   pInitSATvm(TABLE t)
{
    SATvm     *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lInitSATvm(pstSavm, t))
        return NULL;

    return pstSavm;
}

/*************************************************************************************************
    函数说明：获取连接创建共享内存的KEY值
    参数说明：
        pstSavm                    --句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
key_t   yGetIPCPath(SATvm *pstSavm, Benum em)
{
    key_t    tKey = 0;
    char    szPath[512];

    memset(szPath, 0, sizeof(szPath));
    snprintf(szPath, sizeof(szPath), "%s", getenv("TVMDBD"));

    if(-1 == (tKey = ftok(szPath, em)))
    {
        pstSavm->m_lErrno = GENER_KEY_ERR;
        return RC_FAIL;
    }

    return tKey;
}

/*************************************************************************************************
    函数说明：连接共享内存点击测试
    参数说明：
        pstSavm
        t                        --表序号
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
void*    pInitHitTest(SATvm *pstSavm, TABLE t)
{
    RunTime    *pstRun = (RunTime *)pGetRunTime(pstSavm, t);

    pstRun->tblName = t;
    pstRun->lFind = pstSavm->lFind;
    pstRun->lReMatch = pstSavm->lReMatch;

    if(pstRun->m_bAttch && pstRun->m_pvAddr)
        return pstRun;

    pstRun->m_pvAddr = (void* )shmat(pstRun->m_shmID, NULL, 0);
    if(NULL == pstRun->m_pvAddr || (void *)(-1) == (void *)pstRun->m_pvAddr)
    {
        if(EACCES == errno)    
            pstSavm->m_lErrno = SHM_ERR_ACCES;
        else if(ENOMEM == errno)
            pstSavm->m_lErrno = SHM_ERR_NOMEM;
        else 
            pstSavm->m_lErrno = SHM_ERR_INVAL;
        return NULL;
    }

    pstSavm->m_lErrno = 0;
    pstSavm->m_lEffect= 0;
    pstRun->m_bAttch  = TRUE;
    pstSavm->lSize    = lGetRowSize(t);
    memcpy((void *)pGetTblDef(t), pstRun->m_pvAddr, sizeof(TblDef));

    return pstRun;
}

/*************************************************************************************************
    函数说明：连接共享内存并初始化必要索引
    参数说明：
        t                        --表序号
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
void*    pInitMemTable(SATvm *pstSavm, TABLE t)
{
    RunTime    *pstRun = (RunTime *)pGetRunTime(pstSavm, t);

    if(RES_REMOT_SID == pstRun->m_lLocal)    //    远程资源
        return pstRun;

    pstSavm->m_lErrno  = 0;
    pstSavm->m_lEffect = 0;
    if(pstRun->m_bAttch && pstRun->m_pvAddr)        
        return pstRun;

    pstRun->tblName = t;
    pstRun->lFind = pstSavm->lFind;
    pstRun->lReMatch = pstSavm->lReMatch;

    /*    如果是系统表。从根节点获取ID*/
    if(TYPE_SYSTEM == pstSavm->bSearch) 
    {
        if(RC_FAIL == (pstSavm->m_yKey = yGetIPCPath(pstSavm, IPC_SHM)))
            return NULL;
        pstRun->m_shmID = shmget(pstSavm->m_yKey, 0, IPC_CREAT|0600);

        if(pstRun->m_shmID < 0)
        {
            switch(errno)
            {
            case   EEXIST:
                pstSavm->m_lErrno = SHM_ERR_EXIST;
                break;
            case   EIDRM:
                pstSavm->m_lErrno = SHM_ERR_EIDRM;
                break;
            case   EACCES:
                pstSavm->m_lErrno = SHM_ERR_ACCES;
                break;
            case   ENOMEM:
                pstSavm->m_lErrno = SHM_ERR_NOMEM;
                break;
            default:
                pstSavm->m_lErrno = SHM_ERR_INVAL;
                break;
            }
            return NULL;
        }
    }

    pstRun->m_pvAddr = (void* )shmat(pstRun->m_shmID, NULL, 0);
    if(NULL == pstRun->m_pvAddr || (void *)(-1) == (void *)pstRun->m_pvAddr)
    {
        if(EACCES == errno)    
            pstSavm->m_lErrno = SHM_ERR_ACCES;
        else if(ENOMEM == errno)
            pstSavm->m_lErrno = SHM_ERR_NOMEM;
        else 
            pstSavm->m_lErrno = SHM_ERR_INVAL;
        return NULL;
    }

    pstRun->m_bAttch = TRUE;

    memcpy((void *)pGetTblDef(t), pstRun->m_pvAddr, sizeof(TblDef));

    if(pstSavm->lSize != lGetRowSize(t))
    {
        vTblDisconnect(pstSavm, t);
        pstSavm->m_lErrno = VER_NOT_MATCH;
        return NULL;
    }

    return pstRun;
}

/*************************************************************************************************
    函数说明：创建内存表空间
    参数说明：
        t                        --表序号
    返回值：
        无    
 *************************************************************************************************/
void*    pCreateBlock(SATvm *pstSavm, TABLE t, size_t lSize, BOOL bCreate)
{
    RunTime    *pstRun = NULL;

    if(!pstSavm || lSize <= 0)
    {
        pstSavm->m_lErrno = BLCK_SIZE_ERR;
        return NULL;
    }

    pstRun = (RunTime *)pGetRunTime(pstSavm, t);
    memset(pstRun, 0, sizeof(RunTime));
    //  必须重建共享内存，每次的数量都是没法估计的
    if(!bCreate)
    {
        pstSavm->m_yKey = IPC_PRIVATE;
        pstSavm->m_ySey = IPC_PRIVATE;
    }
    else
    {
        if(RC_FAIL == (pstSavm->m_yKey = yGetIPCPath(pstSavm, IPC_SHM)))
            return NULL;

        if(RC_FAIL == (pstSavm->m_ySey = yGetIPCPath(pstSavm, IPC_SEM)))
            return NULL;
    }

    pstRun->m_shmID = shmget(pstSavm->m_yKey, lSize, IPC_CREAT|IPC_EXCL|0600);
    if(pstRun->m_shmID < 0)
    {
        switch(errno)
        {
        case   EEXIST:
            pstSavm->m_lErrno = SHM_ERR_EXIST;
            break;
        case   EIDRM:
            pstSavm->m_lErrno = SHM_ERR_EIDRM;
            break;
        case   EACCES:
            pstSavm->m_lErrno = SHM_ERR_ACCES;
            break;
        case   ENOMEM:
            pstSavm->m_lErrno = SHM_ERR_NOMEM;
            break;
        default:
            pstSavm->m_lErrno = SHM_ERR_INVAL;
            break;
        }
        return NULL;
    }

    pstRun->m_pvAddr = (void* )shmat(pstRun->m_shmID, NULL, 0);
    if(NULL == pstRun->m_pvAddr || (void *)(-1) == (void *)pstRun->m_pvAddr)
    {
        if(EACCES == errno)    
            pstSavm->m_lErrno = SHM_ERR_ACCES;
        else if(ENOMEM == errno)
            pstSavm->m_lErrno = SHM_ERR_NOMEM;
        else 
            pstSavm->m_lErrno = SHM_ERR_INVAL;
        return NULL;
    }

//  memset(pstRun->m_pvAddr, 0, lSize);
    pstRun->m_bAttch = TRUE;

    return pstRun;
}

/*************************************************************************************************
    函数说明：新增索引字段
    参数说明：
        t                          --表序号
        type                       --索引类型
        from                       --来源
        len                        --长度
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lAddIdxField(TABLE t, long type, long lFrom, long lLen, long lAttr)
{
    long    lIdx = 0;
    TblKey  *pstKey = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(UNQIUE == type)
    {
        for(lIdx = 0, pstKey = pGetTblIdx(t); lIdx < lGetIdxNum(t); lIdx++)
        {
            if((pstKey[lIdx].m_lFrom == lFrom) && pstKey[lIdx].m_lLen == lLen)
                return RC_SUCC;
        }

        if(MAX_FILED_IDX <= lGetIdxNum(t))
        {
            pstSavm->m_lErrno = IDX_DEF_SPILL;
            return RC_FAIL;
        }

        ((TblDef *)pGetTblDef(t))->m_lIdxLen += lLen;
        if(MAX_INDEX_LEN < (((TblDef *)pGetTblDef(t))->m_lIdxLen))
        {
            pstSavm->m_lErrno = IDX_LEN_SPILL;
            return RC_FAIL;
        }

        pstKey[lGetIdxNum(t)].m_lFrom = lFrom;
        pstKey[lGetIdxNum(t)].m_lLen  = lLen;
        pstKey[lGetIdxNum(t)].m_lAttr = lAttr;
        ((TblDef *)pGetTblDef(t))->m_lIdxUp ++;
        ((TblDef *)pGetTblDef(t))->m_lIType |= type;

        return RC_SUCC;
    }
    else if(NORMAL == type || HASHID == type)
    {
        for(lIdx = 0, pstKey = pGetTblGrp(t); lIdx < lGetGrpNum(t); lIdx++)
        {
            if((pstKey[lIdx].m_lFrom == lFrom) && pstKey[lIdx].m_lLen == lLen)
                return RC_SUCC;
        }

        if(MAX_FILED_IDX <= lGetGrpNum(t))
        {
            pstSavm->m_lErrno = GRP_DEF_SPILL;
            return RC_FAIL;
        }

        ((TblDef *)pGetTblDef(t))->m_lGrpLen += lLen;
        if(MAX_INDEX_LEN < (((TblDef *)pGetTblDef(t))->m_lGrpLen))
        {
            pstSavm->m_lErrno = GRP_LEN_SPILL;
            return RC_FAIL;
        }

        pstKey[lGetGrpNum(t)].m_lFrom = lFrom;
        pstKey[lGetGrpNum(t)].m_lLen  = lLen;
        pstKey[lGetGrpNum(t)].m_lAttr = lAttr;
        ((TblDef *)pGetTblDef(t))->m_lGrpUp ++;
        ((TblDef *)pGetTblDef(t))->m_lIType |= type;

        return RC_SUCC;
    }

    pstSavm->m_lErrno = IDX_TYP_NODEF;

    return RC_FAIL;
}

/*************************************************************************************************
    函数说明：新增表字段
    参数说明：
        t                        --表序号
        from                    --起始
        len                        --长度
        desc                    --描述
        attr                    --属性
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lSetTableIdx(TABLE t, long lFrom, long lLen, char *pszDesc, long lAttr, long lType)
{
    long    lIdx = 0;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();
    TblKey    *pstKey = (TblKey *)pGetTblKey(t);

    for(lIdx = 0; lIdx < lGetFldNum(t); lIdx ++)
    {
        if((pstKey[lIdx].m_lFrom == lFrom) && pstKey[lIdx].m_lLen == lLen)
           return RC_SUCC;
    }

    if(MAX_FILED_NUM <= lGetFldNum(t))
    {
        pstSavm->m_lErrno = FLD_DEF_SPILL;
        return RC_FAIL;
    }

    pstKey[lGetFldNum(t)].m_lFrom = lFrom;
    pstKey[lGetFldNum(t)].m_lLen  = lLen;
    pstKey[lGetFldNum(t)].m_lAttr = lAttr;
    pstKey[lGetFldNum(t)].m_lIsPk = lType;
    strncpy(pstKey[lGetFldNum(t)].m_szField, pszDesc, sizeof(pstKey[lGetFldNum(t)].m_szField));
    ((TblDef *)pGetTblDef(t))->m_lIdxNum ++;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：匹配索引数据
    参数说明：
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lRematchIdx(SATvm *pstSavm, void *s, void *p, long eType)
{
    int        i, j;
    TblKey    *pstIdx = NULL;

    if(MATCH_YES != pstSavm->lReMatch)
        return RC_SUCC;

    if(UNQIUE == eType)
    {
        j = lGetIdxNum(pstSavm->tblName);
        pstIdx = (TblKey *)pGetTblIdx(pstSavm->tblName);
    }
    else if(NORMAL == eType || HASHID == eType)
    {
        j = lGetGrpNum(pstSavm->tblName);
        pstIdx = (TblKey *)pGetTblGrp(pstSavm->tblName);    
    }
    else
        return RC_SUCC;

    for(i = 0; i < j; i ++)
    {
        if(memcmp(s + pstIdx[i].m_lFrom, p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen))
        {
            pstSavm->m_lErrno = IDX_DATA_MISM;
            return RC_FAIL;
        }
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：新增字段索引
    参数说明：
        pstSavm
        t
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lInsertField(SATvm *pstSavm, TABLE t)
{
    TField    stField;
    long       i, lIdx = lGetFldNum(t);
    TblKey    *pstIdx = pGetTblKey(t);

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_FIELD)))
        return RC_FAIL;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_FIELD;
    pstSavm->lSize   = sizeof(TField);
    pstSavm->pstVoid = (void *)&stField;

    for(i = 0; i < lIdx; i ++)  //  下面比较的都是唯一索引
    {
        memset(&stField, 0, sizeof(TField));
        stField.m_table = t;
        stField.m_lSeq  = i + 1;
        stField.m_lAttr = pstIdx[i].m_lAttr;
        stField.m_lFrom = pstIdx[i].m_lFrom;
        stField.m_lLen  = pstIdx[i].m_lLen;
        stField.m_lIsPk = pstIdx[i].m_lIsPk;
        strncpy(stField.m_szOwner, TVM_NODE_INFO, sizeof(stField.m_szOwner));
        strncpy(stField.m_szTable, sGetTableName(t), sizeof(stField.m_szTable));
        strncpy(stField.m_szField, pstIdx[i].m_szField, sizeof(stField.m_szField));

        if(RC_SUCC != lInsert(pstSavm))
            return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：合并有效数据到p，用于更新字段
    参数说明：
        pstSavm
        s                            --原始数据
        p                            --待比对数据
        eType
    返回值：
        TRUE                    --是
        FALSE                    --否
 *************************************************************************************************/
void    vMergeTruck(SATvm *pstSavm, char *s, char *p) 
{
    register    int    i = 0;
    long        lIdx = lGetFldNum(pstSavm->tblName);
    TblKey      *pstIdx = pGetTblKey(pstSavm->tblName);

    for(i = 0; i < lIdx; i ++)
    {
        if(bIsRealZore(p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen))
        {
            memcpy(p + pstIdx[i].m_lFrom, s + pstIdx[i].m_lFrom, pstIdx[i].m_lLen);
            continue;
        }

        switch(pstIdx[i].m_lAttr)
        {
        case FIELD_DOUBLE:
            switch(pstIdx[i].m_lLen)
            {
            case    4:
                if(*((float *)(p + pstIdx[i].m_lFrom)) < 0.0)
                    *((float *)(p + pstIdx[i].m_lFrom)) -= NUMBER_ZEOR;
                break;
            case    8:
                if(*((double *)(p + pstIdx[i].m_lFrom)) < 0.0)
                *((double *)(p + pstIdx[i].m_lFrom)) -= NUMBER_ZEOR;
                break;
            default:
                break;
            }
            break;
        case FIELD_LONG:
            switch(pstIdx[i].m_lLen)
            {
            case    2:
                if(*((sint *)(p + pstIdx[i].m_lFrom)) < 0)
                    *((sint *)(p + pstIdx[i].m_lFrom)) -= NUMBER_ZEOR;
                break;
            case    4:
                if(*((int *)(p + pstIdx[i].m_lFrom)) < 0)
                    *((int *)(p + pstIdx[i].m_lFrom))  -= NUMBER_ZEOR;
                break;
            case    8:
                if(*((llong *)(p + pstIdx[i].m_lFrom)) < 0)
                    *((llong *)(p + pstIdx[i].m_lFrom)) -= NUMBER_ZEOR;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

/*************************************************************************************************
    函数说明：返回最大字段所在记录
    参数说明：
        pstSavm
        s                            --原始数据
        p                            --待比对数据
        eType
    返回值：
        TRUE                         --是
        FALSE                        --否
 *************************************************************************************************/
void*    pvCompExtrem(SATvm *pstSavm, void *s, void *p, TblKey *pstKey, long eType)
{
    if(!p)    return s;

    if(eType & MATCH_MAX)
    {
        switch(pstKey->m_lAttr)
        {
        case FIELD_DOUBLE:
            switch(pstKey->m_lLen)
            {
            case    4:
                if(*((float *)(p + pstKey->m_lFrom)) < *((float *)(s + pstKey->m_lFrom)))
                    return s;
                return p;
            case    8:
                if(*((double *)(p + pstKey->m_lFrom)) < *((double *)(s + pstKey->m_lFrom)))
                    return s;
                return p;
            default:
                return p;
            }
            return p;
        case FIELD_LONG: 
            switch(pstKey->m_lLen)
            {
            case    2:
                if(*((sint *)(p + pstKey->m_lFrom)) < *((sint *)(s + pstKey->m_lFrom)))
                    return s;
                return p;
            case    4:
                if(*((int *)(p + pstKey->m_lFrom)) < *((int *)(s + pstKey->m_lFrom)))
                    return s;
                return p;
            case    8:
                if(*((llong *)(p + pstKey->m_lFrom)) < *((llong *)(s + pstKey->m_lFrom)))
                    return s;
                return p;
            default:
                return p;
            }
            return p;
        case FIELD_CHAR:
            if(0 < memcmp(s + pstKey->m_lFrom, p + pstKey->m_lFrom, pstKey->m_lLen))
                return s;
            return p;
        default:
            return p;
        }
    }
    else
    {
        switch(pstKey->m_lAttr)
        {
        case FIELD_DOUBLE:
            switch(pstKey->m_lLen)
            {
            case    4:
                if(*((float *)(p + pstKey->m_lFrom)) < *((float *)(s + pstKey->m_lFrom)))
                    return p;
                return s;
            case    8:
                if(*((double *)(p + pstKey->m_lFrom)) < *((double *)(s + pstKey->m_lFrom)))
                    return p;
                return s;
            default:
                return p;
            }
            return p;
        case FIELD_LONG: 
            switch(pstKey->m_lLen)
            {
            case    2:
                if(*((sint *)(p + pstKey->m_lFrom)) < *((sint *)(s + pstKey->m_lFrom)))
                    return p;
                return s;
            case    4:
                if(*((int *)(p + pstKey->m_lFrom)) < *((int *)(s + pstKey->m_lFrom)))
                    return p;
                return s;
            case    8:
                if(*((llong *)(p + pstKey->m_lFrom)) < *((llong *)(s + pstKey->m_lFrom)))
                    return p;
                return s;
            default:
                return p;
            }
            return p;
        case FIELD_CHAR:
            if(0 < memcmp(s + pstKey->m_lFrom, p + pstKey->m_lFrom, pstKey->m_lLen))
                return p;
            return s;
        default:
            return p;
        }
    }

    return p;
}

/*************************************************************************************************
    函数说明：匹配表字段(注意如果原始数据都为空，可能导致选中)
    参数说明：
        pstSavm
        s                            --原始数据
        p                            --待比对数据
        eType
    返回值：
        TRUE                    --是
        FALSE                    --否
 *************************************************************************************************/
BOOL    bTblIdxMatch(SATvm *pstSavm, void *s, void *p, long eType)
{
    char    v[8];
    register    int   i, lSucc;
    long    lRet, lIdx, lNumber;
    TblKey  *pstIdx = pGetTblKey(pstSavm->tblName);

    lIdx = lGetFldNum(pstSavm->tblName);
    if(0 == lIdx)    return FALSE;

    for(i = 0, lSucc = 0; i < lIdx; i ++)    //  下面比较的都是唯一索引
    {
        if(RCD_SELECT == pstIdx[i].m_lIsPk)
            continue;

        /*  无论是否查询索引还是主索引，0不处理, 如果是数字类型，设置用LONG_ZER重置*/
        if(bIsRealZore(p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen))
            continue;

        switch(pstIdx[i].m_lAttr)
        {
        case FIELD_DOUBLE:
            switch(pstIdx[i].m_lLen)
            {
            case    4:
                if(*((float *)(p + pstIdx[i].m_lFrom)) < 0.0)
                    *((float *)v) = *((float *)(p + pstIdx[i].m_lFrom)) - NUMBER_ZEOR;
                else
                    memcpy(v, p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen);
                break;
            case    8:
                if(*((double *)(p + pstIdx[i].m_lFrom)) < 0.0)
                    *((double *)v) = *((double *)(p + pstIdx[i].m_lFrom)) - NUMBER_ZEOR;
                else
                    memcpy(v, p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen);
                break;
            default:
                break;
            }

            lRet = memcmp(s + pstIdx[i].m_lFrom, v, pstIdx[i].m_lLen);
            if(RC_SUCC == lRet) 
            {
                ++ lSucc;
                if(IDX_SELECT == pstIdx[i].m_lIsPk) return TRUE;
                else                                break; // 继续匹配其他条件
            }
            return FALSE;    //    无论是什么类型索引，不匹配则整个不匹配
        case FIELD_LONG: 
            switch(pstIdx[i].m_lLen)
            {
            case    2:
                if(*((sint *)(p + pstIdx[i].m_lFrom)) < 0)
                    *((sint *)v) = *((sint *)(p + pstIdx[i].m_lFrom)) - NUMBER_ZEOR;
                else
                    memcpy(v, p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen);
                break;
            case    4:
                if(*((int *)(p + pstIdx[i].m_lFrom)) < 0)
                    *((int *)v) = *((int *)(p + pstIdx[i].m_lFrom)) - NUMBER_ZEOR;
                else
                    memcpy(v, p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen);
                break;
            case    8:
                if(*((llong *)(p + pstIdx[i].m_lFrom)) < 0)
                    *((llong *)v) = *((llong *)(p + pstIdx[i].m_lFrom)) - NUMBER_ZEOR;
                else
                    memcpy(v, p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen);
                break;
            default:
                break;
            }

            lRet = memcmp(s + pstIdx[i].m_lFrom, v, pstIdx[i].m_lLen);
            if(RC_SUCC == lRet)                   //  任意一条主索引查找直接返回
            {
                ++ lSucc;
                if(IDX_SELECT == pstIdx[i].m_lIsPk) return TRUE;
                else                                break; // 继续匹配其他条件
            }
            return FALSE;    //    无论是什么类型索引，不匹配则整个不匹配
        case FIELD_CHAR:
            lRet = memcmp(s + pstIdx[i].m_lFrom, p + pstIdx[i].m_lFrom, pstIdx[i].m_lLen);
            if(RC_SUCC == lRet) //  任意一条主索引查找直接返回
            {
                ++ lSucc;
                if(IDX_SELECT == pstIdx[i].m_lIsPk) return TRUE;
                else                                break; // 继续匹配其他条件
            }
            return FALSE;
        default:
            pstSavm->m_lErrno = FTYPE_NOT_DEF;
            return FALSE;
        }
    }

    /*  查询索引不能在for循环里面返回，如果for里面没有可匹配的字段，本条默认查询匹配*/
    return BOOL(lSucc);
}

/*************************************************************************************************
    函数说明：获取索引组空间大小
    参数说明：
        lRow                      --当前表记录个数
    返回值：
        size_t                    --所需空间大小
 *************************************************************************************************/
size_t    lGetGroupTruck(size_t lRow, size_t *plOfs)
{
    *plOfs = sizeof(SHTree) * lRow;
    return (sizeof(SHTree) + sizeof(SHList)) * lRow;
}

/*************************************************************************************************
    函数说明：获取索引组空间大小
    参数说明：
        lRow                      --当前表记录个数
    返回值：
        size_t                    --所需空间大小
 *************************************************************************************************/
size_t    lGetTreeTruck(size_t lRow)
{
    return sizeof(SHTree) * lRow;
}

/*************************************************************************************************
    函数说明：初始化表结构信息
    参数说明：
        t                        --表序号
    返回值：
        无
 *************************************************************************************************/
size_t    lInitialTable(TABLE t, size_t lRow)
{
    size_t    lSize = sizeof(TblDef), lIdxOfs = 0;
    
    g_stTblDef[t].m_lMaxRow = lRow;

    //    设置NIL节点
    g_stTblDef[t].m_lNodeNil = FPOS(TblDef, m_stNil);
    memset(&g_stTblDef[t].m_stNil, 0, sizeof(SHTree));
    g_stTblDef[t].m_stNil.m_eColor = COLOR_BLK;
    g_stTblDef[t].m_stNil.m_lSePos = g_stTblDef[t].m_lNodeNil;

    if(HAVE_UNIQ_IDX(t)) 
    {
        g_stTblDef[t].m_lTreePos  = lSize;
        g_stTblDef[t].m_lTreeRoot = lSize;
        lSize += lGetTreeTruck(lRow);
    }
    else
    {
        g_stTblDef[t].m_lTreePos  = 0;
        g_stTblDef[t].m_lTreeRoot = 0;
    }

    if(HAVE_NORL_IDX(t) || HAVE_HASH_IDX(t)) 
    {
        g_stTblDef[t].m_lGroupPos  = lSize;
        g_stTblDef[t].m_lGroupRoot = lSize;
        lSize += lGetGroupTruck(lRow, &g_stTblDef[t].m_lListPos);
        g_stTblDef[t].m_lListOfs = g_stTblDef[t].m_lGroupPos + g_stTblDef[t].m_lListPos;
    }
    else
    {
        g_stTblDef[t].m_lListPos  = 0;
        g_stTblDef[t].m_lListOfs  = 0;
        g_stTblDef[t].m_lGroupPos = 0;
        g_stTblDef[t].m_lGroupRoot= 0;
    }

    g_stTblDef[t].m_lData = lSize;
    lSize += g_stTblDef[t].m_lTruck * lRow;

    return lSize;
}

/*************************************************************************************************
    函数说明：初始化索引组参数
    参数说明：
        pvData                     --当前地址
        t                          --初始化的表序号
    返回值：
        RC_FAIL                    --初始化失败
        RC_SUCC                    --成功
 *************************************************************************************************/
long    lInitailGroup(SATvm *pstSavm, void *pvData, TABLE t)
{
    long    lRow = 0;
    void    *psData = NULL;
    SHTree  *pstTree = NULL;
    SHList  *pstList = NULL;

    if(!pvData)
    {
        pstSavm->m_lErrno = SHMT_NOT_INIT;
        return RC_FAIL;
    }

    //    未定义唯一索引内存区
    if(lGetGrpNum(t) <= 0)        return RC_SUCC;

    for(psData = pvData + lGetListPos(t); lRow < lGetTblRow(t); lRow ++)
    {
        pstTree = (SHTree *)(pvData + lRow * sizeof(SHTree));
        memset(pstTree, 0, sizeof(SHTree));
        pstTree->m_lSePos = SELF_POS_UNUSE;
        pstTree->m_lData  = SELF_POS_UNUSE;

        pstList = (SHList *)(psData + lRow * sizeof(SHList));
        pstList->m_lPos  = SELF_POS_UNUSE;
        pstList->m_lNode = SELF_POS_UNUSE;
        pstList->m_lData = lGetTblData(t) + lGetRowTruck(t) * lRow;
        pstList->m_lNext = SELF_POS_UNUSE;
        pstList->m_lLast = SELF_POS_UNUSE;
    }
    ((TblDef *)pGetTblDef(t))->m_lGroupRoot = lGetGrpPos(t);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：初始化索引组参数
    参数说明：
        pvData                     --当前地址
        t                          --初始化的表序号
    返回值：
        RC_FAIL                    --初始化失败
        RC_SUCC                    --成功
 *************************************************************************************************/
long    lInitailTree(SATvm *pstSavm, void *pvData, TABLE t)
{
    long    lRow = 0;
    SHTree  *pstTree = NULL;

    if(!pvData)
    {
        pstSavm->m_lErrno = SHMT_NOT_INIT;
        return RC_FAIL;
    }

    //    未定义查询索引内存区
    if(lGetIdxNum(t) <= 0)        return RC_SUCC;

    for(lRow = 0; lRow < lGetTblRow(t); lRow ++)
    {
        pstTree = (SHTree *)(pvData + lRow * sizeof(SHTree));
        memset(pstTree, 0, sizeof(SHTree));
        pstTree->m_lSePos = SELF_POS_UNUSE;
        pstTree->m_lData  = lGetTblData(t) + lGetRowTruck(t) * lRow;
    }
    ((TblDef *)pGetTblDef(t))->m_lTreeRoot  = lGetIdxPos(t);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：结算hash值
    参数说明：
        s
        n
    返回值：
        TRUE                     --是
        FALSE                    --否
 *************************************************************************************************/
uint uGetHash(char *s, long n)
{
    uint    hash = 0;
    static  uint    i, seed = 131;

    for(i = 0; i < n; i ++)
        hash = hash * seed + (*s++);

    return (hash & 0x7FFFFFFF);
}

/*************************************************************************************************
    函数说明：获取偏移量对应的地址
    参数说明：
        lOffset                    --偏移量
    返回值：
        void                    --指针
 *************************************************************************************************/
void*    pGetNode(void *pvData, size_t lOffset)
{
    return (void *)(pvData + lOffset);
}

/*************************************************************************************************
    函数说明：创建索引节点
    参数说明：
        pvData                    --当前地址
        t                        --初始化的表序号
    返回值：
        SHTree                    --索引结构
 *************************************************************************************************/
SHTree* pCreateNode(void *pvData, TABLE t, size_t lOffset)
{
    return (SHTree *)(pvData + lOffset);
}

/*************************************************************************************************
    函数说明：新增一般索引数据
    参数说明：
        pvData                    --当前地址
        pstGroup                --一般索引组    
        lData                    --初始化的表序号
    返回值：
        SHTree                    --索引结构
 *************************************************************************************************/
SHTree* pCreateGroup(void *pvData, TABLE t, SHTree *pstGroup, size_t lGroup, SHTruck **ppstTruck)
{
    size_t    lOffset = 0;
    SHList    *pstList = NULL, *pstNode = NULL;

    if(!pstGroup)    pstGroup = (SHTree *)(pvData + lGroup);
    lOffset = lGetListOfs(t) + sizeof(SHList) * ((TblDef *)pvData)->m_lValid;
    pstList = (SHList *)pGetNode(pvData, lOffset);
    pstList->m_lPos = lOffset;
    *ppstTruck = (PSHTruck)pGetNode(pvData, pstList->m_lData);    

    //  一个节点都没有
    if(pstGroup->m_lData == SELF_POS_UNUSE)
    {
        pstList->m_lNode = lGroup;
        pstGroup->m_lData = pstList->m_lPos;
    }
    else
    {
        pstNode = (SHList *)pGetNode(pvData, pstGroup->m_lData);

#if        0
        pstList->m_lNext = pstNode->m_lNext;
        pstList->m_lLast = pstNode->m_lPos;
        if(SELF_POS_UNUSE != pstNode->m_lNext)
            ((SHList *)pGetNode(pvData, pstNode->m_lNext))->m_lLast = pstList->m_lPos;
        pstNode->m_lNext = pstList->m_lPos;
#else
        pstGroup->m_lData = pstList->m_lPos;
        pstList->m_lNode = pstGroup->m_lSePos;
        pstList->m_lNext = pstNode->m_lPos;
        pstNode->m_lLast  = pstList->m_lPos;
#endif
    }

    return pstGroup;
}

/*************************************************************************************************
    函数说明：重置索引组节点参数
    参数说明：
        pstTree                 --待清理的索引组数据
    返回值：
        SHTree                  --根节点
 *************************************************************************************************/
void    vResetList(void *pvData, TABLE t, SHTree *pstTree, long *plOut)
{
    size_t  lOffset, lNext;
    SHList  *pstList = NULL, *pstTail = NULL;


    return ;
#if        0    
    if(pstTree->m_lData <= SELF_POS_UNUSE)
        return ;

fprintf(stderr, "vResetList error \n");
exit(-1);

    lOffset = lGetListOfs(t) + lGetTblValid(t) * sizeof(SHList);
    //  将链表中的数据向前移动，将后面空间释放出来待下次使用, 同时 g_lTreeNode 个数减一
    for(pstList = (SHList *)pGetNode(pvData, pstTree->m_lData); SELF_POS_UNUSE != pstList->m_lPos; 
        (*plOut) ++, pstList = (SHList *)pGetNode(pvData, lNext))
    {
        lNext = pstList->m_lNext;
        
        lOffset -= sizeof(SHList);
        pstTail = (SHList *)pGetNode(pvData, lOffset);
        if(pstList == pstTail)
        {
            pstTail->m_lPos  = SELF_POS_UNUSE;
            pstTail->m_lNode = SELF_POS_UNUSE;
            pstTail->m_lNext = SELF_POS_UNUSE;
            pstTail->m_lLast = SELF_POS_UNUSE;
            if(SELF_POS_UNUSE == lNext)    break;
            continue;
        }

        if(SELF_POS_UNUSE != pstTail->m_lLast)
             ((SHList *)pGetNode(pvData, pstTail->m_lLast))->m_lNext = pstList->m_lPos;
        memcpy(pstList, pstTail, sizeof(SHList));
        pstTail->m_lPos  = SELF_POS_UNUSE;
        pstTail->m_lNode = SELF_POS_UNUSE;
        pstTail->m_lNext = SELF_POS_UNUSE;
        pstTail->m_lLast = SELF_POS_UNUSE;
        if(SELF_POS_UNUSE == lNext)    break;
    }
#endif

    pstTree->m_lData = SELF_POS_UNUSE;
}

/*************************************************************************************************
    函数说明：释放删除的节点
    参数说明：
        pstSavm                    --操作句柄
        t                          --表序号
        pstRoot                    --Root节点
        pstTree                    --数据节点
        pstSon                     --子节点
    返回值：
        SHTree                     --根节点
 *************************************************************************************************/
SHTree*    pReleaseNode(void *pvData, TABLE t, SHTree *pstRoot, SHTree *pstTree, SHTree **ppstSon)
{
    size_t    lSePos = 0, lData, lVaild;
    SHTree    *pstTail = NULL, *pstNode = NULL;

    if(!pstTree || ((TblDef *)pvData)->m_lValid < 1)
        return pstRoot;

    lData  = pstTree->m_lData;    //    原始lData
    lVaild = ((TblDef *)pvData)->m_lValid - 1;
    pstTail = (SHTree *)(pvData + lGetIdxPos(t) + lVaild * sizeof(SHTree));
    if(pstTail == pstTree)    //    正好删除的是尾节点
    {
        memset(pstTail, 0, sizeof(SHTree));
        pstTail->m_lData = lData;
        return pstRoot;
    }

    if(ppstSon)
    {
        if(*ppstSon == pstTail)        //    移动Son(tail)节点
            *ppstSon = pstTree;
        else if(pstTree->m_lParent == pstTail->m_lSePos)    //    相邻节点, 移动Tail
            ((SHTree *)*ppstSon)->m_lParent = pstTree->m_lSePos;
    }

    lSePos = pstTail->m_lSePos;    //    原始POS
    pstTail->m_lSePos = pstTree->m_lSePos;
    memcpy((void *)pstTree, (void *)pstTail, sizeof(SHTree));
    memset(pstTail, 0, sizeof(SHTree));
    pstTail->m_lData = lData;

    //    修改父、子节点
    if(NODE_NULL != pstTree->m_lParent)
    {
        pstNode = (SHTree *)pGetNode(pvData, pstTree->m_lParent);
        if(lSePos == pstNode->m_lLeft)
            pstNode->m_lLeft = pstTree->m_lSePos;
        else
            pstNode->m_lRight = pstTree->m_lSePos;
    }
    else    //    可能将尾部的root移动了
    {
        pstRoot = pstTree;
    }

    if(NODE_NULL != pstTree->m_lLeft)
    {
        pstNode = (SHTree *)pGetNode(pvData, pstTree->m_lLeft);
        pstNode->m_lParent = pstTree->m_lSePos;
    }

    if(NODE_NULL != pstTree->m_lRight)
    {
        pstNode = (SHTree *)pGetNode(pvData, pstTree->m_lRight);
        pstNode->m_lParent = pstTree->m_lSePos;    
    }

    return pstRoot;
}

/*************************************************************************************************
    函数说明：释放删除的数据块空间
    参数说明：
        pstSavm                    --操作句柄
        t                          --表序号
        pstTruck                   --数据库
        bErase                     --清理方式
    返回值：
		RC_SUCC                    --成功
		RC_FAIL                    --失败
 *************************************************************************************************/
long    lReleaseTruck(void *pvAddr, TABLE t, SHTruck *pstTruck, BOOL bErase)
{
    size_t  lOffset = 0;
    SHTruck *pstTail = NULL;

    if(bErase)
    {
        memset(pstTruck, 0, lGetRowTruck(t));
        return RC_SUCC;
    }

    if(((TblDef *)pvAddr)->m_lValid < 1) 
        return RC_SUCC;

    lOffset = lGetTblData(t) + lGetRowTruck(t) * (((TblDef *)pvAddr)->m_lValid - 1);
    pstTail = (PSHTruck)pGetNode(pvAddr, lOffset);
    if(pstTruck == pstTail)
    {
        memset(pstTail, 0, lGetRowTruck(t));
        return RC_SUCC;
    }

    memcpy(pstTruck, pstTail, lGetRowTruck(t));
    memset(pstTail, 0, lGetRowTruck(t));

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：释放查询索引节点
    参数说明：
        pvData                     --数据地址
        t                          --标序号
        pstRoot                    --Root节点
        pstTree                    --索引节点
        pstSon                     --子节点
        plOut                      --偏移量
    返回值：
        SHTree                     --根节点
 *************************************************************************************************/
SHTree*    pReleaseGroup(void *pvData, TABLE t, SHTree *pstRoot, SHTree *pstTree, SHTree **ppstSon, 
            long *plOut)
{
    size_t    lSePos = 0, lVaild;
    SHTree    *pstTail = NULL, *pstNode = NULL;

    if(!pstTree || ((TblDef *)pvData)->m_lGroup < 1)    //    异常情况, 0个节点无法释放
        return pstRoot;

    vResetList(pvData, t, pstTree, plOut);

    lVaild = -- ((TblDef *)pvData)->m_lGroup;
    pstTail = (SHTree *)(pvData + lGetGrpPos(t) + lVaild * sizeof(SHTree));
    if(pstTail == pstTree)    //    正好删除的是尾节点
    {
        memset(pstTail, 0, sizeof(SHTree));
        return pstRoot;
    }

    if(ppstSon)
    {
        if(*ppstSon == pstTail)     //  移动Son(tail)节点
            *ppstSon = pstTree;
        else if(pstTree->m_lParent == pstTail->m_lSePos)    //  相邻节点, 移动Tail
            ((SHTree *)*ppstSon)->m_lParent = pstTree->m_lSePos;
    }

    lSePos = pstTail->m_lSePos; //  原始POS
    pstTail->m_lSePos = pstTree->m_lSePos;
    memcpy((void *)pstTree, (void *)pstTail, sizeof(SHTree));
    memset(pstTail, 0, sizeof(SHTree));
    ((SHList *)pGetNode(pvData, pstTree->m_lData))->m_lNode = pstTree->m_lSePos;

    //    修改父、子节点
    if(NODE_NULL != pstTree->m_lParent)
    {
        pstNode = (SHTree *)pGetNode(pvData, pstTree->m_lParent);
        if(lSePos == pstNode->m_lLeft)
            pstNode->m_lLeft = pstTree->m_lSePos;
        else
            pstNode->m_lRight = pstTree->m_lSePos;
    }
    else    //    可能将尾部的root移动了
    {
        pstRoot = pstTree;
    }

    if(NODE_NULL != pstTree->m_lLeft)
    {
        pstNode = (SHTree *)pGetNode(pvData, pstTree->m_lLeft);
        pstNode->m_lParent = pstTree->m_lSePos;
    }

    if(NODE_NULL != pstTree->m_lRight)
    {
        pstNode = (SHTree *)pGetNode(pvData, pstTree->m_lRight);
        pstNode->m_lParent = pstTree->m_lSePos;    
    }

    return pstRoot;
}

/*************************************************************************************************
    函数说明：释放list节点
    参数说明：
        pvData                     --数据地址
        t                          --表序号
        pstTree                    --索引节点
        pstList                    --数据列表
        lOffset                    --偏移量
        plNext                     --下级节点
    返回值：
		RC_SUCC                    --成功
		RC_FAIL                    --失败
 *************************************************************************************************/
long    lReleaseList(void *pvData, TABLE t, SHTree *pstTree, SHList *pstList, size_t lOffset, 
            size_t *plNext)
{
    size_t   lData;
    SHList  *pstTail = NULL;

    pstTail = (SHList *)pGetNode(pvData, lOffset);
    if(SELF_POS_UNUSE == pstTail->m_lPos)
        return RC_FAIL;

    if(pstTree->m_lData == pstList->m_lPos)
    {
        pstTree->m_lData = pstList->m_lNext;
        if(SELF_POS_UNUSE != pstList->m_lNext)
        {
            ((SHList *)pGetNode(pvData, pstList->m_lNext))->m_lLast = SELF_POS_UNUSE;
            ((SHList *)pGetNode(pvData, pstList->m_lNext))->m_lNode = pstTree->m_lSePos;
        }
    }
    else
    {
        ((SHList *)pGetNode(pvData, pstList->m_lLast))->m_lNext = pstList->m_lNext;
        if(SELF_POS_UNUSE != pstList->m_lNext)
            ((SHList *)pGetNode(pvData, pstList->m_lNext))->m_lLast = pstList->m_lLast;
    }

    if(pstTail == pstList)
    {
        //    保留lData
        pstList->m_lPos  = SELF_POS_UNUSE;
        pstList->m_lNode = SELF_POS_UNUSE;
        pstList->m_lNext = SELF_POS_UNUSE;
        pstList->m_lLast = SELF_POS_UNUSE;
        return RC_SUCC;
    }

    //    如果tail节点正是下次扫描的节点
    if(pstTail->m_lPos == pstList->m_lNext)    
        *plNext = pstList->m_lPos;

    lData = pstList->m_lData;
    if(pstTail->m_lPos == pstTree->m_lData)
        pstTree->m_lData = pstList->m_lPos;
    else
    {
        if(SELF_POS_UNUSE == pstTail->m_lLast)
            ((SHTree *)pGetNode(pvData, pstTail->m_lNode))->m_lData = pstList->m_lPos;
    }

    if(SELF_POS_UNUSE != pstTail->m_lLast)
        ((SHList *)pGetNode(pvData, pstTail->m_lLast))->m_lNext = pstList->m_lPos;
    if(SELF_POS_UNUSE != pstTail->m_lNext)
        ((SHList *)pGetNode(pvData, pstTail->m_lNext))->m_lLast = pstList->m_lPos;

    pstList->m_lNode = pstTail->m_lNode;
    pstList->m_lNext = pstTail->m_lNext;
    pstList->m_lData = pstTail->m_lData;
    pstList->m_lLast = pstTail->m_lLast;
    
    pstTail->m_lData = lData;
    pstTail->m_lPos  = SELF_POS_UNUSE;
    pstTail->m_lNode = SELF_POS_UNUSE;
    pstTail->m_lNext = SELF_POS_UNUSE;
    pstTail->m_lLast = SELF_POS_UNUSE;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：对红黑树的节点(pstTree)进行左旋转
    参数说明：
        pstRoot                    --树根节点
        pstTree                    --待左旋的节点
    返回值：
        SHTree                    --根节点

    左旋示意图:
       root                           root
       /                               /
     tree                            node                
     /  \      --(左旋)-->           /   \                                                       #
     lA  node                       tree  rC    
        /  \                       /  \
       lB   rC                    lA  lB  
 *************************************************************************************************/
SHTree*    _pLeftRotation(void *pvData, SHTree *pstRoot, SHTree *pstTree)
{
    SHTree    *pstCur = NULL, *pstNode = (SHTree *)pGetNode(pvData, pstTree->m_lRight);

    // 将pstNode的左孩子 重置为 pstTree的右孩子
    // 如果pstNode的左孩子非空，将 pstTree 设为 pstNode的左孩子的父亲
    pstTree->m_lRight = pstNode->m_lLeft;
    if(NODE_NULL != pstNode->m_lLeft)
    {
        pstCur = (SHTree *)pGetNode(pvData, pstNode->m_lLeft);
        pstCur->m_lParent = pstTree->m_lSePos;
    }

    // 将 “pstTree的父亲” 设为 “pstNode的父亲”
    pstNode->m_lParent = pstTree->m_lParent;

    //    如果pstTree的父节点为NIL
    if(NODE_NULL == pstTree->m_lParent)
        pstRoot = pstNode;            //    新的Root产生了
    else
    {
        pstCur = (SHTree *)pGetNode(pvData, pstTree->m_lParent);
        if(pstCur->m_lLeft == pstTree->m_lSePos)
            pstCur->m_lLeft = pstNode->m_lSePos;
        else
            pstCur->m_lRight = pstNode->m_lSePos;
    }
    
    // 将pstTree设为pstNode的左孩子
    pstNode->m_lLeft = pstTree->m_lSePos;
    // 将pstTree的父节点设为pstNode”
    pstTree->m_lParent = pstNode->m_lSePos;

    return pstRoot;
}

/*************************************************************************************************
    函数说明：对红黑树的节点(pstTree)进行右旋转
    参数说明：
        pstRoot                    --树根节点
        pstTree                    --待右旋的节点
    返回值：
        SHTree                     --根节点

    右旋示意图:
        root                           root
        /                               /
       tree                           node                  
      /   \      --(右旋)-->          /  \                                                       #
     node  rC                        lA  tree  
     /  \                                /  \                                                    #
    lA  rB                              rB   rC
 *************************************************************************************************/
SHTree*   _pRightRotation(void *pvData, SHTree *pstRoot, SHTree *pstTree)
{
    SHTree    *pstCur = NULL, *pstNode = (SHTree *)pGetNode(pvData, pstTree->m_lLeft);

    // 将 “pstNode的右孩子” 设为 “pstTree的左孩子”；
    // 如果"pstNode的右孩子"不为空的话，将 “pstTree” 设为 “pstNode的右孩子的父亲”
    pstTree->m_lLeft = pstNode->m_lRight;
    if(NODE_NULL != pstNode->m_lRight)
    {
        pstCur = (SHTree *)pGetNode(pvData, pstNode->m_lRight);
        pstCur->m_lParent = pstTree->m_lSePos;
    }

    // 将 “pstTree的父亲” 设为 “pstNode的父亲”
    pstNode->m_lParent = pstTree->m_lParent;
    if(NODE_NULL == pstTree->m_lParent)
        pstRoot = pstNode;          //  新的Root产生了
    else
    {
        pstCur = (SHTree *)pGetNode(pvData, pstTree->m_lParent);
        if(pstCur->m_lLeft == pstTree->m_lSePos)
            pstCur->m_lLeft = pstNode->m_lSePos;
        else
            pstCur->m_lRight = pstNode->m_lSePos;
    }

    // 将 “pstTree” 设为 “pstNode的右孩子”
    pstNode->m_lRight = pstTree->m_lSePos;
    // 将 “pstTree的父节点” 设为 “pstNode”
    pstTree->m_lParent = pstNode->m_lSePos;

    return pstRoot;
}

/*************************************************************************************************
    函数说明：在RBTree遍历获取极值
    参数说明：
        pstSavm                    --操作句柄
        pvData                     --数据地址
        pstTree                    --待查询节点
    返回值：
        SHTree                     --返回极值节点
 *************************************************************************************************/
SHTree* pExtremeTree(SATvm *pstSavm, void *pvData, SHTree *pstTree)
{
    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
        return NULL;

    if(pstSavm->lReMatch & MATCH_MAX)
    {
        while(SELF_POS_UNUSE != pstTree->m_lRight && NODE_NULL != pstTree->m_lRight)
            pstTree = (SHTree *)pGetNode(pvData, pstTree->m_lRight);
    }
    else
    {
        while(SELF_POS_UNUSE != pstTree->m_lLeft && NODE_NULL != pstTree->m_lLeft)
            pstTree = (SHTree *)pGetNode(pvData, pstTree->m_lLeft);
    }

    if(SELF_POS_UNUSE == pstTree->m_lSePos && NODE_NULL == pstTree->m_lSePos)
        pstTree = NULL;

    return pstTree;
}

/*************************************************************************************************
    函数说明：在RBTree遍历指定KEY值
    参数说明：
        pvData                    --数据地址
        pstTree                   --查询节点
        psvIdx                    --索引值
        lIdx                      --索引长度
    返回值：
        SHTree                    --查询结果
 *************************************************************************************************/
SHTree* pSearchTree(void *pvData, SHTree *pstTree, void *psvIdx, long lIdx)
{
    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
        return NULL;

    if(!memcmp(pstTree->m_szIdx, psvIdx, lIdx))
        return pstTree;

    if(0 < memcmp(pstTree->m_szIdx, psvIdx, lIdx))
        return pSearchTree(pvData, (SHTree *)pGetNode(pvData, pstTree->m_lLeft), psvIdx, lIdx);
    else
        return pSearchTree(pvData, (SHTree *)pGetNode(pvData, pstTree->m_lRight), psvIdx, lIdx);
}

/*************************************************************************************************
    函数说明：获取数据组数据列表
    参数说明：
        pvData                    --数据地址
        pstTree                   --查询节点
        psvIdx                    --索引值
        lIdx                      --索引长度
    返回值：
        SHList                    --数据链表
 *************************************************************************************************/
SHList    *pSearchGroup(void *pvData, SHTree *pstTree, void *psvIdx, long lIdx)
{
    if(NULL == (pstTree = pSearchTree(pvData, pstTree, psvIdx, lIdx)))
        return NULL;

    //    异常情况下如果group信息存在，但是无数据，那么这个group就应该被删除的
    if(SELF_POS_UNUSE == pstTree->m_lData)
        return NULL;
    
    return (SHList *)pGetNode(pvData, pstTree->m_lData);
}

/*************************************************************************************************
    函数说明：修复新增导致RBTree不平衡问题
    参数说明：
        pvData                    --数据地址
        pstRoot                   --Root节点
        pstCur                    --已新增的节点
    返回值：
        SHTree                    --根节点
 *************************************************************************************************/
SHTree*    _pFixupInsert(void *pvData, SHTree *pstRoot, SHTree *pstCur)
{
    SHTree *pstParent = NULL, *pstUncle = NULL, *pstGrand = NULL;

    while((pstParent = (SHTree *)pGetNode(pvData, pstCur->m_lParent)) && IS_RED(pstParent))
    {
        pstGrand = (SHTree *)pGetNode(pvData, pstParent->m_lParent);
        if(pstParent->m_lSePos == pstGrand->m_lLeft)
        {
            pstUncle = (SHTree *)pGetNode(pvData, pstGrand->m_lRight);
            if((NODE_NULL != pstUncle->m_lSePos) && IS_RED(pstUncle))
            {
                pstUncle->m_eColor = COLOR_BLK;
                pstParent->m_eColor = COLOR_BLK;
                pstGrand->m_eColor = COLOR_RED;
                pstCur = pstGrand;
                continue;
            }

            if(pstCur->m_lSePos == pstParent->m_lRight)
            {
                pstRoot = _pLeftRotation(pvData, pstRoot, pstParent); //左旋 [结点z，与父母结点]  
                pstParent = pstCur;
            }

            pstParent->m_eColor = COLOR_BLK;
            pstGrand->m_eColor = COLOR_RED; 
            pstRoot = _pRightRotation(pvData, pstRoot, pstGrand);
        }
        else
        {
            pstUncle = (SHTree *)pGetNode(pvData, pstGrand->m_lLeft);
            if((NODE_NULL != pstUncle->m_lSePos) && IS_RED(pstUncle))
            {
                pstUncle->m_eColor = COLOR_BLK;
                pstParent->m_eColor = COLOR_BLK;
                pstGrand->m_eColor = COLOR_RED;
                pstCur = pstGrand;
                continue;
            }

            if(pstCur->m_lSePos == pstParent->m_lLeft)
            {
                pstRoot = _pRightRotation(pvData, pstRoot, pstParent); //左旋[结点z，与父母结点]  
                pstParent = pstCur; 
            }

            pstParent->m_eColor = COLOR_BLK;
            pstGrand->m_eColor = COLOR_RED;
            pstRoot = _pLeftRotation(pvData, pstRoot, pstGrand);
        }
    }

    pstRoot->m_eColor = COLOR_BLK;    //    根结点，黑色

    return pstRoot;
}

/*************************************************************************************************
    将结点插入到AVL树中，并返回根节点
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --Root节点
        psvIdx                     --索引值
        lIdx                       --索引长度
        ppstTruck                  --数据库
    返回值：
        SHTree                    --根节点
 *************************************************************************************************/
SHTree*    pInsertTree(SATvm *pstSavm, SHTree *pstRoot, void *psvIdx, long lIdx, SHTruck **ppstTruck)
{
    int     nRet = 0;
    size_t  lOffset;
    SHTree  *pstNode = NULL, *pstCur = NULL, *pstTree = pstRoot;
    void    *pvData = NULL, *pvAddr = pGetAddr(pstSavm, pstSavm->tblName);

    while(SELF_POS_UNUSE != pstTree->m_lSePos && NODE_NULL != pstTree->m_lSePos)
    {
        pstNode = pstTree;
        nRet = memcmp(pstTree->m_szIdx, psvIdx, lIdx);
        if(0 < nRet)
        {
            if(NODE_NULL == pstTree->m_lLeft)
                break;
            pstTree = (SHTree *)pGetNode(pvAddr, pstTree->m_lLeft);
        }
        else if(0 > nRet)
        {
            if(NODE_NULL == pstTree->m_lRight)
                break;
            pstTree = (SHTree *)pGetNode(pvAddr, pstTree->m_lRight);
        }
        else    //    唯一索引重复
        {
            pstSavm->m_lErrno = UNIQ_IDX_REPT;
            return NULL;
        }
    }
        
    lOffset = sizeof(SHTree) * ((TblDef *)pvAddr)->m_lValid + lGetIdxPos(pstSavm->tblName);
    if(NULL == (pstCur = pCreateNode(pvAddr, pstSavm->tblName, lOffset)))
    {       
        pstSavm->m_lErrno = IDX_SPC_SPILL;
        return NULL;
    }
    
    if(*ppstTruck && (*ppstTruck != (PSHTruck)pGetNode(pvAddr, pstCur->m_lData)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return NULL;
    }
    else
    {
        *ppstTruck = (PSHTruck)pGetNode(pvAddr, pstCur->m_lData);
        if(!IS_TRUCK_NULL(*ppstTruck))
        {
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return NULL;
        }
    }

    pstCur->m_lIdx = lIdx;
    pstCur->m_lSePos = lOffset;
    pstCur->m_lParent = NODE_NULL;
    memcpy(pstCur->m_szIdx, psvIdx, pstCur->m_lIdx);
    //    设置父节点，和左右节点全部指向NIL节点
    pstCur->m_lLeft = pstCur->m_lRight = NODE_NULL;

    if(pstNode != NULL)
    {
        pstCur->m_eColor = COLOR_RED;
        pstCur->m_lParent = pstNode->m_lSePos;
        if(0 < memcmp(pstNode->m_szIdx, pstCur->m_szIdx, pstCur->m_lIdx))
            pstNode->m_lLeft = pstCur->m_lSePos;
        else
            pstNode->m_lRight = pstCur->m_lSePos;
    }
    else
    {
        pstRoot = pstCur;    
        pstCur->m_eColor = COLOR_BLK;
        return pstRoot;    //    开始为空节点
    }

    //    由于本身节点Cur是红色的，如果父节点是黑色，就不存在修复
    return _pFixupInsert(pvAddr, pstRoot, pstCur);
}

/**********************************************************************************************
    函数功能：根据hash表指示地方插入值并处理碰撞
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        pstTree                    --索引节点
        lGroup                     --偏移量
        ppstTruck                  --数据块
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    __lInsertHash(SATvm *pstSavm, void *pvAddr, TABLE t, SHTree *pstTree, size_t lGroup, 
            SHTruck **ppstTruck)
{
    size_t  lOffset;
    SHTruck *pstTruck = NULL;
    SHList  *pstList = NULL, *pstNode = NULL;

    lOffset = lGetListOfs(t) + sizeof(SHList) * ((TblDef *)pvAddr)->m_lValid;
    pstList = (SHList *)pGetNode(pvAddr, lOffset);
    pstList->m_lPos = lOffset;
    pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);

    //    如果在前面唯一索引创建时，已找出数据空间，重新校验一次
    if(!IS_TRUCK_NULL(pstTruck) || ((*ppstTruck) && (*ppstTruck != pstTruck)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    *ppstTruck = pstTruck;  /*  当无唯一索引，这里必须赋值 */
    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
    {
        pstTree->m_lSePos= lGroup;
        pstTree->m_lData = pstList->m_lPos;
        pstList->m_lNode = pstTree->m_lSePos;
    }
    else
    {
        pstNode = (SHList *)pGetNode(pvAddr, pstTree->m_lData);

        pstList->m_lNext = pstNode->m_lNext;
        pstList->m_lLast = pstNode->m_lPos;
        if(SELF_POS_UNUSE != pstNode->m_lNext)
            ((SHList *)pGetNode(pvAddr, pstNode->m_lNext))->m_lLast = pstList->m_lPos;
        pstNode->m_lNext = pstList->m_lPos;
    }

    return RC_SUCC;
}    

/**********************************************************************************************
    函数说明：将结点插入到AVL树中，并返回根节点
    参数说明：
        pstSavm                     --操作句柄
        pstRoot                     --Root节点
        psvIdx                      --索引值
        lIdx                        --索引长度
        ppstTruck                   --索引长度
    返回值：
        SHTree                       --根节点
 *************************************************************************************************/
SHTree* pInsertGroup(SATvm *pstSavm, SHTree *pstRoot, void *psvIdx, long lIdx, SHTruck **ppstTruck)
{
    int     nRet = 0;
    size_t  lOffset;
    SHTruck *pstTruck = NULL;
    void    *pvData = pGetAddr(pstSavm, pstSavm->tblName);
    SHTree  *pstNode = NULL, *pstCur = NULL, *pstTree = pstRoot;

    while(SELF_POS_UNUSE != pstTree->m_lSePos && NODE_NULL != pstTree->m_lSePos)
    {
        pstNode = pstTree;
        nRet = memcmp(pstTree->m_szIdx, psvIdx, lIdx);
        if(0 < nRet)
        {
            if(NODE_NULL == pstTree->m_lLeft)
                break;
            pstTree = (SHTree *)pGetNode(pvData, pstTree->m_lLeft);
        }
        else if(0 > nRet)
        {
            if(NODE_NULL == pstTree->m_lRight)
                break;
            pstTree = (SHTree *)pGetNode(pvData, pstTree->m_lRight);
        }
        else    //  唯一索引重复
        {
            pCreateGroup(pvData, pstSavm->tblName, pstTree, 0, &pstTruck);
            if(!IS_TRUCK_NULL(pstTruck) || ((*ppstTruck) && (*ppstTruck != pstTruck)))
            {
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return NULL;
            }

            return pstRoot;
        }
    }

    lOffset = sizeof(SHTree) * ((TblDef *)pvData)->m_lGroup + lGetGrpPos(pstSavm->tblName);
    if(NULL == (pstCur = pCreateGroup(pvData, pstSavm->tblName, NULL, lOffset, &pstTruck)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return pstRoot;
    }

    //    如果在前面唯一索引创建时，已找出数据空间，需要跟这查询索引匹配一次
    if(!IS_TRUCK_NULL(pstTruck) || ((*ppstTruck) && (*ppstTruck != pstTruck)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return NULL;
    }

    ((TblDef *)pvData)->m_lGroup ++;
    //    如果都匹配，在无唯一索引下，必须赋值
    *ppstTruck = pstTruck;

    pstCur->m_lIdx = lIdx;
    pstCur->m_lSePos = lOffset;
    pstCur->m_lParent = NODE_NULL;
    memcpy(pstCur->m_szIdx, psvIdx, pstCur->m_lIdx);
    //  设置父节点，和左右节点全部指向NIL节点
    pstCur->m_lLeft = pstCur->m_lRight = NODE_NULL;
    if(pstNode != NULL)
    {
        pstCur->m_eColor = COLOR_RED;
        pstCur->m_lParent = pstNode->m_lSePos;
        if(0 < memcmp(pstNode->m_szIdx, pstCur->m_szIdx, pstCur->m_lIdx))
            pstNode->m_lLeft = pstCur->m_lSePos;
        else
            pstNode->m_lRight = pstCur->m_lSePos;
    }
    else
    {
        pstRoot = pstCur;
        pstCur->m_eColor = COLOR_BLK;
        return pstRoot; //  开始为空节点
    }

    //  由于本身节点Cur是红色的，如果父节点是黑色，就不存在修复
    return _pFixupInsert(pvData, pstRoot, pstCur);
}

/*************************************************************************************************
    函数说明：修复删除导致RBTree不平衡问题
    参数说明：
        pvData                     --数据地址
        pstRoot                    --Root节点
        pstTree                    --导致不平衡的节点
    返回值：
        SHTree                     --根节点
 *************************************************************************************************/
SHTree*    _pFixupDelete(void *pvData, SHTree *pstRoot, SHTree *pstTree)
{
    SHTree    *pstParent = NULL, *pstBrother = NULL, *pstLeft = NULL, *pstRight = NULL;

    while(COLOR_BLK == pstTree->m_eColor && (pstRoot != pstTree))
    {   
        pstParent = (SHTree *)pGetNode(pvData, pstTree->m_lParent);
        
        if(pstTree->m_lSePos == pstParent->m_lLeft) 
        {
            pstBrother = (SHTree *)pGetNode(pvData, pstParent->m_lRight);

            if(COLOR_RED == pstBrother->m_eColor) 
            {
                pstParent->m_eColor = COLOR_RED;
                pstBrother->m_eColor = COLOR_BLK;
                pstRoot = _pLeftRotation(pvData, pstRoot, pstParent);

                // 新兄弟节点。其实就是左旋后的效果。 */
                pstBrother = (SHTree *)pGetNode(pvData, pstParent->m_lRight);
            }
            if(NODE_NULL == pstBrother->m_lSePos)
                return pstRoot;

            pstLeft = (SHTree *)pGetNode(pvData, pstBrother->m_lLeft);
            pstRight = (SHTree *)pGetNode(pvData, pstBrother->m_lRight);
            if(COLOR_BLK == pstLeft->m_eColor && COLOR_BLK == pstRight->m_eColor)
            {
                pstBrother->m_eColor = COLOR_RED;
                pstTree = pstParent;
            }
            else 
            {
                /*Case 3: 兄弟结点为黑色(默认), 兄弟节点的左子结点为红色, 
                    右子结点为黑色:  以brother为支点, 右旋处理 */
                if(COLOR_BLK == pstRight->m_eColor)
                {
                    pstLeft->m_eColor = COLOR_BLK;
                    pstBrother->m_eColor = COLOR_RED;
                    pstRoot = _pRightRotation(pvData, pstRoot, pstBrother);

                    /*  变化后的，父结点的右孩子，作为新的兄弟结点 */
                    pstBrother = (SHTree *)pGetNode(pvData, pstParent->m_lRight);
                    pstRight = (SHTree *)pGetNode(pvData, pstBrother->m_lRight);
                }
                
                /* Case 4: 兄弟结点为黑色(默认), 兄弟结点右孩子结点为红色:  以parent为支点, 左旋处理 */
                pstBrother->m_eColor = pstParent->m_eColor;
                pstParent->m_eColor = COLOR_BLK;
                pstRight->m_eColor  = COLOR_BLK;
                pstRoot = _pLeftRotation(pvData, pstRoot, pstParent);
                pstTree = pstRoot;
            }
        }
        else    /*    pstTree 为pstParent的右孩子 */
        {
            pstBrother = (SHTree *)pGetNode(pvData, pstParent->m_lLeft);
            if(COLOR_RED == pstBrother->m_eColor)
            {
                pstParent->m_eColor = COLOR_RED;
                pstBrother->m_eColor = COLOR_BLK;
                pstRoot = _pRightRotation(pvData, pstRoot, pstParent);

                // 新兄弟节点。其实就是左旋后的效果。 */
                pstBrother = (SHTree *)pGetNode(pvData, pstParent->m_lLeft);
            }
            if(NODE_NULL == pstBrother->m_lSePos)
                return pstRoot;

            /* 兄弟结点为黑色(默认), 且兄弟结点的2个子结点都为黑色 */
            pstLeft = (SHTree *)pGetNode(pvData, pstBrother->m_lLeft);
            pstRight = (SHTree *)pGetNode(pvData, pstBrother->m_lRight);
            if(COLOR_BLK == pstLeft->m_eColor && COLOR_BLK == pstRight->m_eColor)
            {
                pstBrother->m_eColor = COLOR_RED;
                pstTree = pstParent;
            }
            else
            {
                /* Case 7: 兄弟结点为黑色(默认), 兄弟节点的右子结点为红色, 
                   左子结点为黑色:以brother为支点, 左旋处理 */
                if(COLOR_BLK == pstLeft->m_eColor)
                {
                    pstRight->m_eColor = COLOR_BLK;
                    pstBrother->m_eColor = COLOR_RED;
                    pstRoot = _pLeftRotation(pvData, pstRoot, pstBrother); // 右旋

                    /*  变化后的，父结点的右孩子，作为新的兄弟结点 */
                    pstBrother = (SHTree *)pGetNode(pvData, pstParent->m_lLeft);
                    pstLeft = (SHTree *)pGetNode(pvData, pstBrother->m_lLeft);
                }
            
                /* Case 8: 兄弟结点为黑色(默认), 兄弟结点左孩子结点为红色: 以parent为支点, 右旋处理 */
                pstBrother->m_eColor = pstParent->m_eColor;
                pstParent->m_eColor = COLOR_BLK;
                pstLeft->m_eColor  = COLOR_BLK;
                pstRoot = _pRightRotation(pvData, pstRoot, pstParent);
                pstTree = pstRoot;
            }
        }
    }

    pstTree->m_eColor  = COLOR_BLK;
    
    return pstRoot;
}

/*************************************************************************************************
    函数说明：从RBTree中删除执行索引
    参数说明：
        pvData                     --数据地址
        t                          --表序号
        pstRoot                    --root节点
        pstTree                    --索引节点
    返回值：
        SHTree                     --根节点
 *************************************************************************************************/
SHTree*    _pDeleteTree(void *pvData, TABLE t, SHTree *pstRoot, SHTree *pstTree)
{
    size_t  lData;
    SHTree  *pstChild = NULL, *pstParent = NULL, *pstNext = NULL;

    /*    存在左节点，或者右节点的情况，注意无节点默认NIL为右节点处理*/
    if(NODE_NULL == pstTree->m_lLeft || NODE_NULL == pstTree->m_lRight)
    {
        if(NODE_NULL == pstTree->m_lLeft)
            pstNext = (SHTree *)pGetNode(pvData, pstTree->m_lRight);
        else
            pstNext = (SHTree *)pGetNode(pvData, pstTree->m_lLeft);

        //    子节点指向待删除的节点的父节点
        pstNext->m_lParent = pstTree->m_lParent;
        if(NODE_NULL == pstTree->m_lParent)    //    待删除无父节点
            pstRoot = pstNext;
        else
        {
            pstParent = (SHTree *)pGetNode(pvData, pstTree->m_lParent);
            if(pstParent->m_lLeft == pstTree->m_lSePos)
                pstParent->m_lLeft = pstNext->m_lSePos;
            else
                pstParent->m_lRight = pstNext->m_lSePos;
        }  

        /* 如果删除的红色节点，不影响树结构    */
        if(pstTree->m_eColor == COLOR_RED)
            return pReleaseNode(pvData, t, pstRoot, pstTree, NULL);

        pstRoot = pReleaseNode(pvData, t, pstRoot, pstTree, &pstNext);

        return _pFixupDelete(pvData, pstRoot, pstNext);
    }

    pstNext = (SHTree *)pGetNode(pvData, pstTree->m_lRight);
   /* 被删结点D的左右孩子均不为叶子节点 */
    while(NODE_NULL != pstNext->m_lLeft)
        pstNext = (SHTree *)pGetNode(pvData, pstNext->m_lLeft);

    pstChild  = (SHTree *)pGetNode(pvData, pstNext->m_lRight);
    pstParent = (SHTree *)pGetNode(pvData, pstNext->m_lParent);

    //    与子节点互指
    pstChild->m_lParent = pstParent->m_lSePos;    
    if(pstNext->m_lSePos == pstParent->m_lLeft)
        pstParent->m_lLeft = pstChild->m_lSePos;
    else
        pstParent->m_lRight = pstChild->m_lSePos;

    lData = pstTree->m_lData;
    pstTree->m_lData = pstNext->m_lData;
    pstNext->m_lData = lData;
    memcpy(pstTree->m_szIdx, pstNext->m_szIdx, pstNext->m_lIdx);

    if(pstNext->m_eColor == COLOR_RED)
        return pReleaseNode(pvData, t, pstRoot, pstNext, NULL);

    pstRoot = pReleaseNode(pvData, t, pstRoot, pstNext, &pstChild);

    return _pFixupDelete(pvData, pstRoot, pstChild);
}

/*************************************************************************************************
    函数说明：从RBTree中删除查询索引
    参数说明：
        pvData                      --数据地址
        t                           --表序号
        pstRoot                     --Root节点
        pstTree                     --索引节点
        plOut                       --处理记录个数
    返回值：
        SHTree                    --根节点
 *************************************************************************************************/
SHTree*    _pDeleteGroup(void *pvData, TABLE t, SHTree *pstRoot, SHTree *pstTree, long *plOut)
{
    size_t  lData = 0;
    SHTree  *pstChild = NULL, *pstParent = NULL, *pstNext = NULL;

    /*  存在左节点，或者右节点的情况，注意无节点默认NIL为右节点处理*/
    if(NODE_NULL == pstTree->m_lLeft || NODE_NULL == pstTree->m_lRight)
    {
        if(NODE_NULL == pstTree->m_lLeft)
            pstNext = (SHTree *)pGetNode(pvData, pstTree->m_lRight);
        else
            pstNext = (SHTree *)pGetNode(pvData, pstTree->m_lLeft);

        //  子节点指向待删除的节点的父节点
        pstNext->m_lParent = pstTree->m_lParent;
        if(NODE_NULL == pstTree->m_lParent) //  待删除无父节点
            pstRoot = pstNext;
        else
        {
            pstParent = (SHTree *)pGetNode(pvData, pstTree->m_lParent);
            if(pstParent->m_lLeft == pstTree->m_lSePos)
                pstParent->m_lLeft = pstNext->m_lSePos;
            else
                pstParent->m_lRight = pstNext->m_lSePos;
        }

        /* 如果删除的红色节点，不影响树结构 */
        if(pstTree->m_eColor == COLOR_RED)
            return pReleaseGroup(pvData, t, pstRoot, pstTree, NULL, plOut);

        pstRoot = pReleaseGroup(pvData, t, pstRoot, pstTree, &pstNext, plOut);
        return _pFixupDelete(pvData, pstRoot, pstNext);
    }

    /* 被删结点D的左右孩子均不为叶子节点 */
    pstNext = (SHTree *)pGetNode(pvData, pstTree->m_lRight);
    while(NODE_NULL != pstNext->m_lLeft)
        pstNext = (SHTree *)pGetNode(pvData, pstNext->m_lLeft);

    pstChild  = (SHTree *)pGetNode(pvData, pstNext->m_lRight);
    pstParent = (SHTree *)pGetNode(pvData, pstNext->m_lParent);

    //  与子节点互指
    pstChild->m_lParent = pstParent->m_lSePos;
    if(pstNext->m_lSePos == pstParent->m_lLeft)
        pstParent->m_lLeft = pstChild->m_lSePos;
    else
        pstParent->m_lRight = pstChild->m_lSePos;

    lData = pstTree->m_lData;
    pstTree->m_lData = pstNext->m_lData;
    pstNext->m_lData = lData;    //lData 列表区复制过来，将指向为释放的列表释放
    memcpy(pstTree->m_szIdx, pstNext->m_szIdx, pstNext->m_lIdx);
    ((SHList *)pGetNode(pvData, pstTree->m_lData))->m_lNode = pstTree->m_lSePos;

    if(pstNext->m_eColor == COLOR_RED)
        return pReleaseGroup(pvData, t, pstRoot, pstNext, NULL, plOut);

    pstRoot = pReleaseGroup(pvData, t, pstRoot, pstNext, &pstChild, plOut);

    return _pFixupDelete(pvData, pstRoot, pstChild);
}

/*************************************************************************************************
    函数说明：从RBTree中删除执行索引
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --Root节点
        psvIdx                     --索引值
        lIdx                       --索引长度
        *plData                    --数据偏移量
    返回值：
        SHTree                     --根节点
 *************************************************************************************************/
SHTree*    pDeleteTree(SATvm *pstSavm, SHTree *pstRoot, void *psvIdx, long lIdx, size_t *plData)
{
    SHTree  *pstTree = NULL;
    void    *pvData = pGetAddr(pstSavm, pstSavm->tblName);

    if(NULL == (pstTree = pSearchTree(pvData, pstRoot, psvIdx, lIdx)))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return NULL;
    }

    *plData = pstTree->m_lData;

    return _pDeleteTree(pvData, pstSavm->tblName, pstRoot, pstTree);
}
   
/*************************************************************************************************
    函数说明：从Hash表中删除指示索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        pstTree                    --索引节点
        lData                      --数据偏移量
        pvVoid                     --数据
    返回值：
        SHTree                     --根节点
 *************************************************************************************************/
long    _lPurgedHash(SATvm *pstSavm, void *pvAddr, SHTree *pstTree, size_t lData, void *pvVoid)
{
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    size_t  lNext = 0, lOffset;

    pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData);
    lOffset = lGetListOfs(pstSavm->tblName) + ((TblDef *)pvAddr)->m_lValid * sizeof(SHList);
    for(;SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, lNext))
    {    
        lNext = pstList->m_lNext;
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(lData != SELF_POS_UNUSE)
        {
            if(lData != pstList->m_lData)
            {
                if(SELF_POS_UNUSE == lNext)    break;
                continue;
            }

            if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pvVoid, pstSavm->lFind))
            {    
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }    
        }
        else
        {
            if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pvVoid, pstSavm->lFind)) 
            {
                if(SELF_POS_UNUSE == lNext)    break;
                continue;
            }
        }

        lOffset -= sizeof(SHList);
        if(RC_SUCC != lReleaseList(pvAddr, pstSavm->tblName, pstTree, pstList, lOffset, &lNext))
        {    
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }    

        if(SELF_POS_UNUSE == pstTree->m_lData)
            pstTree->m_lSePos = SELF_POS_UNUSE;

        return RC_SUCC;
    }    

    return RC_FAIL;
}

/*************************************************************************************************
    函数说明：从RBTree中删除执行索引
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --Root节点
        psvIdx                     --索引值
        lIdx                       --索引长度
        pvVoid                     --用户设定的条件
        plOut                      --影响的记录行
        eType                      --0-按照偏移匹配，1-按照记录条件匹配
    返回值：
        SHTree                     --根节点
 *************************************************************************************************/
SHTree*    _pRemoveGroup(SATvm *pstSavm, SHTree *pstRoot, void *psvIdx, long lIdx, void *pvVoid, 
            long *plOut, long eType)
{
    SHList  *pstList = NULL;
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    size_t  lNext = 0, lOffset;
    void    *pvAddr = pGetAddr(pstSavm, pstSavm->tblName);

    if(NULL == (pstTree = pSearchTree(pvAddr, pstRoot, psvIdx, lIdx)))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return NULL;
    }

    //    异常情况下如果group信息存在，但是无数据，那么这个group就应该被删除的
    if(SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return _pDeleteGroup(pvAddr, pstSavm->tblName, pstRoot, pstTree, plOut);
    }

    pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData);

    lOffset = lGetListOfs(pstSavm->tblName) + ((TblDef *)pvAddr)->m_lValid * sizeof(SHList);
    for(*plOut = 0; SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(0 == eType)
        {
            if(memcmp(pvVoid, (void *)&pstList->m_lData, sizeof(pstList->m_lData)))
            {
                if(SELF_POS_UNUSE == lNext)    break;
                continue;
            }
        }
        else
        {
            if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pvVoid, pstSavm->lFind))
            {
                if(SELF_POS_UNUSE == lNext)    break;
                continue;
            }
        }

        (*plOut) ++;
        lOffset -= sizeof(SHList);
        //    lNext的值可能是tail，正好被移动了， 这里需要更新lNext
        if(RC_SUCC != lReleaseList(pvAddr, pstSavm->tblName, pstTree, pstList, lOffset, &lNext))
        {
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return pstRoot;
        }

        if(SELF_POS_UNUSE == lNext || 0 == eType)    break;
    } 

    if(SELF_POS_UNUSE != pstTree->m_lData)
        return pstRoot;
    else
        return _pDeleteGroup(pvAddr, pstSavm->tblName, pstRoot, pstTree, plOut);
}

/*************************************************************************************************
    函数说明：从RBTree中删除执行索引(按照偏移量来删除对应数据)
    参数说明：
        pstSavm                   --操作句柄
        pstRoot                   --Root节点
        psvIdx                    --索引值
        lIdx                      --索引长度
        pvVoid                    --用户设定的条件
        plOut                     --影响的记录行
        eType                     --0-按照偏移匹配，1-按照记录条件匹配
    返回值：
        SHTree                    --根节点
 *************************************************************************************************/
SHTree*    pDeleteGroup(SATvm *pstSavm, SHTree *pstRoot, void *psvIdx, long lIdx, void *pvVoid, 
            long *plOut)
{
    return _pRemoveGroup(pstSavm, pstRoot, psvIdx, lIdx, pvVoid, plOut, 0);
}

/*************************************************************************************************
    函数说明：从RBTree中删除执行索引(按照用户数据条件来删除对应数据)
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --Root节点
        psvIdx                     --索引值
        lIdx                       --索引长度
        pvVoid                     --用户设定的条件
        plOut                      --影响的记录行
        eType                      --0-按照偏移匹配，1-按照记录条件匹配
    返回值：
        SHTree                    --根节点
 *************************************************************************************************/
SHTree*    pRemoveGroup(SATvm *pstSavm, SHTree *pstRoot, void *psvIdx, long lIdx, void *pvVoid, 
            long *plOut)
{
    return _pRemoveGroup(pstSavm, pstRoot, psvIdx, lIdx, pvVoid, plOut, 1);
}

/*************************************************************************************************
    函数说明：从Hash中删除对应数据
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --Root节点
        psvIdx                    --索引值
        lIdx                    --索引长度
        pvVoid                    --用户设定的条件
        plOut                    --影响的记录行
        eType                    --0-按照偏移匹配，1-按照记录条件匹配
    返回值：
        SHTree                    --根节点
 *************************************************************************************************/
long    __lDeleteHash(SATvm *pstSavm, void *pvAddr, SHTree *pstTree, TABLE t, void *psVoid, 
            ulong *puTimes)
{
    SHTree  *pstRoot = NULL;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    size_t  lNext = 0, lOffset, lData = 0;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);

    pstSavm->m_lEType = EXE_PLAN_GRP;
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        return RC_FAIL;
    }

    pstList  = (SHList *)pGetNode(pvAddr, pstTree->m_lData);
    lOffset  = lGetListOfs(t) + ((TblDef *)pvAddr)->m_lValid * sizeof(SHList);
    for(pstSavm->m_lEffect = 0; SELF_POS_UNUSE != pstList->m_lPos;
        pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, psVoid, CHK_SELECT))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        if(HAVE_UNIQ_IDX(t))    //    如果存在唯一索引，那么一定存在数据删除
        {
            memset(szIdx, 0, sizeof(szIdx));
            if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstTruck->m_pvData, szIdx))
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            pstRoot = pDeleteTree(pstSavm, pstRoot, szIdx, lGetIdxLen(t), &lData);
            if(!pstRoot || pstList->m_lData != lData)
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            ((TblDef *)pvAddr)->m_lTreeRoot = pstRoot->m_lSePos;
        }

        pstSavm->m_lEffect ++;
        lOffset -= sizeof(SHList);
        if(RC_SUCC != lReleaseList(pvAddr, t, pstTree, pstList, lOffset, &lNext))
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }

        if(SELF_POS_UNUSE == pstTree->m_lData)
            pstTree->m_lSePos = SELF_POS_UNUSE;

        if(puTimes)    *puTimes = pstTruck->m_lTimes;

        if(RC_SUCC != lRecordWork(pstSavm, pstTruck->m_pvData, OPERATE_DELETE))
        {
            pthread_rwlock_unlock(prwLock);
            return RC_FAIL;
        }

        //    需要删除查询索引和数据, 最后删除真正的数据
        lReleaseTruck(pvAddr, t, pstTruck, TRUE);
        ((TblDef *)pvAddr)->m_lValid --;

        if(SELF_POS_UNUSE == lNext)          break;
        if(FIRST_ROW & pstSavm->lReMatch)    break;
    }

    pthread_rwlock_unlock(prwLock);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：删除Hash表指向数据
    参数说明：
        pstSavm                    --操作句柄
        t                          --表名
        psVoid                     --待插入的数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lDeleteHash(SATvm *pstSavm, void *pvAddr, TABLE t, void *pvVoid)
{
    size_t  lIdx;
    SHTree  *pstTree = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pvVoid, szIdx))
        return RC_CONTINUE;

    lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    pstTree = pvAddr + ((TblDef *)pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
    if(NULL == pstTree || SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return __lDeleteHash(pstSavm, pvAddr, pstTree, t, pvVoid, NULL);
}

/*************************************************************************************************
    函数说明：针对唯一索引遍历出来的数据进行删除
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        psVoid                     --待删除的数据
        psvIdx                     --数据索引值
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    __lDeleteIndex(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid, void *psvIdx, ulong *puTimes)
{
    long    lRow = 0;
    size_t  lData, lIdx;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    SHTree  *pstRoot = NULL, *pstTree = NULL;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);

    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        return RC_FAIL;
    }

    pstSavm->m_lEType = EXE_PLAN_IDX;
    if(NULL == (pstRoot = pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == (pstTree = pSearchTree(pvAddr, pstRoot, psvIdx, lGetIdxLen(t))))
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    lData = pstTree->m_lData;
    pstTruck = (PSHTruck)pGetNode(pvAddr, pstTree->m_lData);
	if(IDX_SELECT != pstSavm->lFind)
	{
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, psVoid, CHK_SELECT))
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = NO_DATA_FOUND;
            return RC_FAIL;
        }
	}

    if(puTimes)    *puTimes = pstTruck->m_lTimes ++;

    if(NULL == (pstRoot = _pDeleteTree(pvAddr, pstSavm->tblName, pstRoot, pstTree)))
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    ((TblDef *)pvAddr)->m_lTreeRoot = pstRoot->m_lSePos;
    
    if(HAVE_NORL_IDX(t))                    //    是否存在查询索引, 如果存在那么肯定有数据需要删除
    {
        memset(szIdx, 0, sizeof(szIdx));
        if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstTruck->m_pvData, 
            szIdx))
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }

        if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
        {   
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }    

        if(NULL == (pstRoot = pDeleteGroup(pstSavm, pstRoot, szIdx, lGetGrpLen(t), &lData, &lRow)))
        {    
            pthread_rwlock_unlock(prwLock);
            return RC_FAIL;
        }

        if(1 != lRow)
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }

        ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;    
    }
    else if(HAVE_HASH_IDX(t))
    {
        memset(szIdx, 0, sizeof(szIdx));
        if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstTruck->m_pvData, szIdx))
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }

        lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
        pstRoot = pvAddr + ((TblDef *)pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
        if(NULL == pstRoot || SELF_POS_UNUSE == pstRoot->m_lData)
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }

        if(RC_SUCC != _lPurgedHash(pstSavm, pvAddr, pstRoot, SELF_POS_UNUSE, 
            pstTruck->m_pvData))
        {
            pthread_rwlock_unlock(prwLock);
            return RC_FAIL;
        }
    }

    if(RC_SUCC != lRecordWork(pstSavm, pstTruck->m_pvData, OPERATE_DELETE))
    {
        pthread_rwlock_unlock(prwLock);
        return RC_FAIL;
    }

    lReleaseTruck(pvAddr, t, pstTruck, TRUE);
    pthread_rwlock_unlock(prwLock);

    pstSavm->m_lEffect = 1;
    ((TblDef *)pvAddr)->m_lValid --;

    //    如果通过索引方式删除的数据，肯定有且只有一条数据
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：针对唯一索引遍历出来的数据进行删除
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        psVoid                     --待插入的数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lDeleteIndex(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid)
{
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), psVoid, szIdx))
        return RC_CONTINUE;

    return __lDeleteIndex(pstSavm, pvAddr, t, psVoid, (void *)szIdx, NULL);
}

/*************************************************************************************************
    函数说明：针对查询索引遍历出来的数据进行删除
    参数说明：
        pstSavm                    --操作句柄
        t                        --表名
        psVoid                    --待插入的数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    __lDeleteGroup(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid, void *psvIdx, 
            ulong *puTimes)
{
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    size_t  lNext = 0, lOffset, lData = 0;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);
    SHTree  *pstRoot = NULL, *pstTree = NULL, *pstIRoot = NULL;

    pstSavm->m_lEType = EXE_PLAN_GRP;
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        return RC_FAIL;
    }

    if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == (pstTree = pSearchTree(pvAddr, pstRoot, psvIdx, lGetGrpLen(t))))
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    //  异常情况下如果group信息存在，但是无数据，那么这个group就应该被删除的
    if(SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstRoot = _pDeleteGroup(pvAddr, t, pstRoot, pstTree, &pstSavm->m_lEffect);
        ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;    
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_SUCC;
    }
    
    pstList  = (SHList *)pGetNode(pvAddr, pstTree->m_lData);
    lOffset  = lGetListOfs(t) + ((TblDef *)pvAddr)->m_lValid * sizeof(SHList);
    for(pstSavm->m_lEffect = 0; SELF_POS_UNUSE != pstList->m_lPos; 
        pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, psVoid, CHK_SELECT))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext) break;
            continue;
        }

        if(HAVE_UNIQ_IDX(t))    //    如果存在唯一索引，那么一定存在数据删除
        {
            memset(szIdx, 0, sizeof(szIdx));
            if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstTruck->m_pvData, 
                szIdx))
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            if(NULL == (pstIRoot = pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
            {   
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            pstIRoot = pDeleteTree(pstSavm, pstIRoot, szIdx, lGetIdxLen(t), &lData);
            if(!pstIRoot || pstList->m_lData != lData)
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            ((TblDef *)pvAddr)->m_lTreeRoot = pstIRoot->m_lSePos;
        }

        pstSavm->m_lEffect ++;
        lOffset -= sizeof(SHList);
        if(RC_SUCC != lReleaseList(pvAddr, t, pstTree, pstList, lOffset, &lNext))
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }

        if(puTimes)        *puTimes = pstTruck->m_lTimes;

        if(RC_SUCC != lRecordWork(pstSavm, pstTruck->m_pvData, OPERATE_DELETE))
        {
            pthread_rwlock_unlock(prwLock);
            return RC_FAIL;
        }

        //    需要删除查询索引和数据, 最后删除真正的数据
        lReleaseTruck(pvAddr, t, pstTruck, TRUE);
        ((TblDef *)pvAddr)->m_lValid --;

        if(SELF_POS_UNUSE == lNext)         break;
        if(FIRST_ROW & pstSavm->lReMatch)   break;
    }

    if(SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstRoot = _pDeleteGroup(pvAddr, pstSavm->tblName, pstRoot, pstTree, &pstSavm->m_lEffect);
        ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;    
    }

    pthread_rwlock_unlock(prwLock);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：针对查询索引遍历出来的数据进行删除
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        psVoid                     --待插入的数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lDeleteGroup(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid)
{
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), psVoid, szIdx))
        return RC_CONTINUE;

    return __lDeleteGroup(pstSavm, pvAddr, t, psVoid, (void *)szIdx, NULL);
}

/*************************************************************************************************
    函数说明：针对一般数据遍历出来的数据进行删除
    参数说明：
        pstSavm                    --操作句柄
        t                          --表名
        psVoid                     --待插入的数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lDeleteTruck(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid)
{
    BOOL    bIsIdx = FALSE;
    SHTree  *pstRoot = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);
    size_t  lData = 0, lOffset = lGetTblData(t), lIdx;
    long    lRow, lValid = ((TblDef *)pvAddr)->m_lValid;

    if(HAVE_INDEX(t))    bIsIdx = TRUE;

    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        return RC_FAIL;
    }

    pstSavm->m_lEType = EXE_PLAN_ALL;
    for(lRow = 0, pstSavm->m_lEffect = 0, pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset); 
        (lRow < lValid) && (lOffset < lGetTableSize(t)); pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {
        if(IS_TRUCK_NULL(pstTruck))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        ++ lRow;        //    有效记录+1
        if(psVoid && !bTblIdxMatch(pstSavm, pstTruck->m_pvData, psVoid, pstSavm->lFind))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        pstSavm->m_lEffect ++;
        if(HAVE_UNIQ_IDX(t))    //    如果存在唯一索引，那么一定存在数据删除
        {
            memset(szIdx, 0, sizeof(szIdx));
            if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstTruck->m_pvData, 
                szIdx))
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            if(NULL == (pstRoot = pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
            {   
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            pstRoot = pDeleteTree(pstSavm, pstRoot, szIdx, lGetIdxLen(t), &lData);
            if(!pstRoot || lOffset != lData)
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            ((TblDef *)pvAddr)->m_lTreeRoot = pstRoot->m_lSePos;
        }

        if(HAVE_NORL_IDX(t))
        {
            memset(szIdx, 0, sizeof(szIdx));
            if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstTruck->m_pvData, 
                szIdx))
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }
        
            if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
            {   
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            pstRoot = pDeleteGroup(pstSavm, pstRoot, szIdx, lGetGrpLen(t), &lOffset, (long *)&lData);
            if(!pstRoot)    return RC_FAIL;
            if(1 != lData)
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }
        
            ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;    
        }
        else if(HAVE_HASH_IDX(t))
        {
            memset(szIdx, 0, sizeof(szIdx));
            if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstTruck->m_pvData, 
                szIdx))
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
            pstRoot = pvAddr + ((TblDef *)pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
            if(NULL == pstRoot || SELF_POS_UNUSE == pstRoot->m_lData)
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            if(RC_SUCC != _lPurgedHash(pstSavm, pvAddr, pstRoot, SELF_POS_UNUSE, 
                pstTruck->m_pvData))
            {
                pthread_rwlock_unlock(prwLock);
                return RC_FAIL;
            }
        }

        //    存在索引 需要删除查询索引和数据, 如果无，将无需修改偏移
        if(bIsIdx)    lOffset += lGetRowTruck(t);

        if(RC_SUCC != lRecordWork(pstSavm, pstTruck->m_pvData, OPERATE_DELETE))
        {
            pthread_rwlock_unlock(prwLock);
            return RC_FAIL;
        }

        lReleaseTruck(pvAddr, t, pstTruck, bIsIdx);
        ((TblDef *)pvAddr)->m_lValid --;
        if(FIRST_ROW & pstSavm->lReMatch)    break;
    }
    pthread_rwlock_unlock(prwLock);

    if(0 == pstSavm->m_lEffect)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：向指定内存表中删除数据
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lDelete(SATvm *pstSavm)
{
    long    lRet;
    RunTime *pstRun = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return lDeleteByRt(pstSavm, pstRun, pstSavm->tblName);

    if(((TblDef *)pstRun->m_pvAddr)->m_lValid <= 0)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    if(!pstSavm->pstVoid)
    {
        lRet = _lDeleteTruck(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, NULL);
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return lRet;
    }

    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        lRet = _lDeleteIndex(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pstSavm->pstVoid);
        if(RC_CONTINUE != lRet)
        {
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }

    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        lRet = _lDeleteGroup(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pstSavm->pstVoid);
        if(RC_CONTINUE != lRet)
        {
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }
    else if(HAVE_HASH_IDX(pstSavm->tblName))
    {
        lRet = _lDeleteHash(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pstSavm->pstVoid);
        if(RC_CONTINUE != lRet)
        {
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }

    lRet = _lDeleteTruck(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pstSavm->pstVoid);
    vTblDisconnect(pstSavm, pstSavm->tblName);
    return lRet;
}

/*************************************************************************************************
    函数说明：统计符合条件记录
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        psVoid                     --待插入的数据
        plCount                    --统计记录条数
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lCountIndex(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid, size_t *plCount)
{
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), psVoid, szIdx))
        return RC_CONTINUE;

    *plCount = 1;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：统计符合条件记录
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        psVoid                     --待插入的数据
        psVoid                     --待插入的数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lCountGroup(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid, size_t *plCount)
{
    size_t  lNext;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    SHTree  *pstRoot = NULL, *pstTree = NULL;

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), psVoid, szIdx))
        return RC_CONTINUE;

    if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == (pstTree = pSearchTree(pvAddr, pstRoot, szIdx, lGetGrpLen(t))))
    {
        *plCount = 0;
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_SUCC;
    }

    for(*plCount = 0, pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData); 
        SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, psVoid, pstSavm->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext) break;
            continue;
        }

        (*plCount) ++;
        if(SELF_POS_UNUSE == lNext)     break;
    }

    if(0 == (pstSavm->m_lEffect = *plCount))
        pstSavm->m_lErrno = NO_DATA_FOUND;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：统计内存符合条件记录
    参数说明：
        pstSavm                    --操作句柄
        t                          --表名
        psVoid                     --数据条件
        plCount                    --统计记录条数
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lCountHash(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid, size_t *plCount)
{
    size_t  lIdx, lNext;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    SHTree  *pstTree = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), psVoid, szIdx))
        return RC_CONTINUE;

    lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    pstTree = pvAddr + ((TblDef *)pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
    if(NULL == pstTree || SELF_POS_UNUSE == pstTree->m_lData)
    {
        *plCount = 0;
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_SUCC;
    }

    for(*plCount = 0, pstList  = (SHList *)pGetNode(pvAddr, pstTree->m_lData);
        SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, psVoid, pstSavm->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)
                break;
            continue;
        }

        (*plCount) ++;
        if(SELF_POS_UNUSE == lNext)     break;
    }

    if(0 == (pstSavm->m_lEffect = *plCount))
        pstSavm->m_lErrno = NO_DATA_FOUND;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：统计内存符合条件记录
    参数说明：
        pstSavm                    --操作句柄
        t                          --表名
        psVoid                     --数据条件
        plCount                    --统计记录条数
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lCountTruck(SATvm *pstSavm, void *pvAddr, TABLE t, void *psVoid, size_t *plCount)
{
    SHTruck *pstTruck = NULL;
    size_t  lRow = 0, lOffset = lGetTblData(t);

    for(*plCount = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t)); 
        lOffset += lGetRowTruck(t))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
        if(IS_TRUCK_NULL(pstTruck))
            continue;

        ++ lRow; 
        if(psVoid && !bTblIdxMatch(pstSavm, pstTruck->m_pvData, psVoid, pstSavm->lFind))
            continue;

        (*plCount) ++;
    }

    if(0 == (pstSavm->m_lEffect = *plCount))
        pstSavm->m_lErrno = NO_DATA_FOUND;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：统计内存符合条件记录
    参数说明：
        pstSavm                    --操作句柄
        plCount                    --统计记录条数
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lCount(SATvm *pstSavm, size_t *plCount)
{
    long    lRet;
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;

    if(!pstSavm || !plCount)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return RC_FAIL;
//      return lCountByRt(pstSavm, pstRun, pstSavm->tblName);

    if(((TblDef *)pstRun->m_pvAddr)->m_lValid <= 0)
    {
        *plCount = 0;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_SUCC;
    }

    if(!pstSavm->pstVoid)
    {
        *plCount = ((TblDef *)pstRun->m_pvAddr)->m_lValid;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_SUCC;
    }

    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        lRet = _lCountIndex(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pstSavm->pstVoid, plCount);
        if(RC_CONTINUE != lRet)
        {
            pstSavm->m_lEType = EXE_PLAN_IDX;
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }

    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        lRet = _lCountGroup(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pstSavm->pstVoid, plCount);
        if(RC_CONTINUE != lRet)
        {
            pstSavm->m_lEType = EXE_PLAN_GRP;
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }
    else if(HAVE_HASH_IDX(pstSavm->tblName))
    {
        lRet = _lCountHash(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pstSavm->pstVoid, plCount);
        if(RC_CONTINUE != lRet)
        {
            pstSavm->m_lEType = EXE_PLAN_GRP;
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }

    pstSavm->m_lEType = EXE_PLAN_ALL;
    lRet = _lCountTruck(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pstSavm->pstVoid, plCount);
    vTblDisconnect(pstSavm, pstSavm->tblName);
    return lRet;
}

/*************************************************************************************************
    函数说明：重建唯一索引
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --根节点
        psvIdx                     --索引
        lIdx                       --索引长度
        lRow                       --当前记录行
        lData                      --当前有效数据位置
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
SHTree*    pRebuildTree(SATvm *pstSavm, SHTree *pstRoot, void *psvIdx, long lIdx, long lRow, size_t lData)
{
    int     nRet = 0;
    size_t  lOffset, lPos = 0, lNext = 0;
    void    *pvData = NULL, *pvAddr = pGetAddr(pstSavm, pstSavm->tblName);
    SHTree  *pstNode = NULL, *pstCur = NULL, *pstNext = NULL, *pstTree = pstRoot;

    while(SELF_POS_UNUSE != pstTree->m_lSePos && NODE_NULL != pstTree->m_lSePos)
    {
        pstNode = pstTree;
        nRet = memcmp(pstTree->m_szIdx, psvIdx, lIdx);
        if(0 < nRet)
        {
            if(NODE_NULL == pstTree->m_lLeft)
                break;
            pstTree = (SHTree *)pGetNode(pvAddr, pstTree->m_lLeft);
        }
        else if(0 > nRet)
        {
            if(NODE_NULL == pstTree->m_lRight)
                break;
            pstTree = (SHTree *)pGetNode(pvAddr, pstTree->m_lRight);
        }
        else    //  唯一索引重复
        {
            pstSavm->m_lErrno = UNIQ_IDX_REPT;
            return NULL;
        }
    }

    lOffset = sizeof(SHTree) * lRow + lGetIdxPos(pstSavm->tblName);
    if(NULL == (pstCur = pCreateNode(pvAddr, pstSavm->tblName, lOffset)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return NULL;
    }

    lPos = pstCur->m_lData;
    pstCur->m_lIdx    = lIdx;
    pstCur->m_lSePos  = lOffset;
    pstCur->m_lData   = lData;
    pstCur->m_lParent = NODE_NULL;
    memcpy(pstCur->m_szIdx, psvIdx, pstCur->m_lIdx);
    //  设置父节点，和左右节点全部指向NIL节点
    pstCur->m_lLeft = pstCur->m_lRight = NODE_NULL;

    if(lData != lPos)    //    需要替换位置
    {
        lNext = (lData - lGetTblData(pstSavm->tblName)) / lGetRowTruck(pstSavm->tblName);
        pstNext = (SHTree *)pGetNode(pvAddr, lNext * sizeof(SHTree) + lGetIdxPos(pstSavm->tblName));
        pstNext->m_lData = lPos;
    }

    if(pstNode != NULL)
    {
        pstCur->m_eColor = COLOR_RED;
        pstCur->m_lParent = pstNode->m_lSePos;
        if(0 < memcmp(pstNode->m_szIdx, pstCur->m_szIdx, pstCur->m_lIdx))
            pstNode->m_lLeft = pstCur->m_lSePos;
        else
            pstNode->m_lRight = pstCur->m_lSePos;
    }
    else
    {
        pstRoot = pstCur;
        pstCur->m_eColor = COLOR_BLK;
        return pstRoot; //  开始为空节点
    }

    //  由于本身节点Cur是红色的，如果父节点是黑色，就不存在修复
    return _pFixupInsert(pvAddr, pstRoot, pstCur);
}

/*************************************************************************************************
    函数说明：重建查询索引子节点
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --根节点
        psvIdx                     --索引
        lIdx                       --索引长度
        lRow                       --当前记录行
        lData                      --当前有效数据位置
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
SHTree*    _pRebuildGroup(void *pvAddr, TABLE t, SHTree *pstGroup, long lGroup, long lRow, size_t lData)
{
    size_t  lOffset = 0, lPos = 0, lNext = 0;
    SHList  *pstList = NULL, *pstNode = NULL, *pstNext = NULL;

    if(!pstGroup)   pstGroup = (SHTree *)(pvAddr + lGroup);
    lOffset = lGetListOfs(t) + sizeof(SHList) * lRow;
    pstList = (SHList *)pGetNode(pvAddr, lOffset);
    pstList->m_lPos = lOffset;
    lPos = pstList->m_lData;
    pstList->m_lData= lData;

    if(lData != lPos)   //  需要替换位置
    {
        lNext = (lData - lGetTblData(t)) / lGetRowTruck(t);
        pstNext = (SHList *)pGetNode(pvAddr, lNext * sizeof(SHList) + lGetListOfs(t));
        pstNext->m_lData = lPos;
    }  

    if(pstGroup->m_lData == SELF_POS_UNUSE)
    {
        pstList->m_lNode = lGroup;
        pstGroup->m_lData = pstList->m_lPos;
    }
    else
    {
        pstNode = (SHList *)pGetNode(pvAddr, pstGroup->m_lData);

        pstList->m_lNext = pstNode->m_lNext;
        pstList->m_lLast = pstNode->m_lPos;
        if(SELF_POS_UNUSE != pstNode->m_lNext)
            ((SHList *)pGetNode(pvAddr, pstNode->m_lNext))->m_lLast = pstList->m_lPos;
        pstNode->m_lNext = pstList->m_lPos;
    }

    return pstGroup;
}

/*************************************************************************************************
    函数说明：重建索引
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --根节点
        psvIdx                     --索引
        lIdx                       --索引长度
        lRow                       --当前记录行
        lData                      --当前有效数据位置
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lRebuildHash(SATvm *pstSavm, TABLE t, void *pvAddr, SHTree *pstTree, size_t lRow, size_t lData)
{
    size_t  lOffset = 0, lPos, lNext;
    SHList  *pstList = NULL, *pstNode = NULL, *pstNext = NULL;

    lOffset = lGetListOfs(t) + sizeof(SHList) * lRow;
    pstList = (SHList *)pGetNode(pvAddr, lOffset);
    pstList->m_lPos = lOffset;
    lPos = pstList->m_lData;
    pstList->m_lData= lData;

    if(lData != lPos)   //  需要替换位置
    {
        lNext = (lData - lGetTblData(t)) / lGetRowTruck(t);
        pstNext = (SHList *)pGetNode(pvAddr, lNext * sizeof(SHList) + lGetListOfs(t));
        pstNext->m_lData = lPos;
    }

    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
    {
        pstTree->m_lSePos= (void *)pstTree - pvAddr;
        pstList->m_lNode = pstTree->m_lSePos;
        pstTree->m_lData = pstList->m_lPos;
    }
    else
    {
        pstNode = (SHList *)pGetNode(pvAddr, pstTree->m_lData);

        pstList->m_lNext = pstNode->m_lNext;
        pstList->m_lLast = pstNode->m_lPos;
        if(SELF_POS_UNUSE != pstNode->m_lNext)
            ((SHList *)pGetNode(pvAddr, pstNode->m_lNext))->m_lLast = pstList->m_lPos;
        pstNode->m_lNext = pstList->m_lPos;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：重建查询索引
    参数说明：
        pstSavm                    --操作句柄
        pstRoot                    --根节点
        psvIdx                     --索引
        lIdx                       --索引长度
        lRow                       --当前记录行
        lData                      --当前有效数据位置
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
SHTree*    pRebuildGroup(SATvm *pstSavm, SHTree *pstRoot, char *psvIdx, long lIdx, long lRow, size_t lData)
{
    int     nRet;
    size_t  lOffset;
    SHTruck *pstTruck = NULL;
    void    *pvAddr = pGetAddr(pstSavm, pstSavm->tblName);
    SHTree  *pstNode = NULL, *pstCur = NULL, *pstTree = pstRoot;

    while(SELF_POS_UNUSE != pstTree->m_lSePos && NODE_NULL != pstTree->m_lSePos)
    {
        pstNode = pstTree;
        nRet = memcmp(pstTree->m_szIdx, psvIdx, lIdx);
        if(0 < nRet)
        {
            if(NODE_NULL == pstTree->m_lLeft)
                break;
            pstTree = (SHTree *)pGetNode(pvAddr, pstTree->m_lLeft);
        }
        else if(0 > nRet)
        {
            if(NODE_NULL == pstTree->m_lRight)
                break;
            pstTree = (SHTree *)pGetNode(pvAddr, pstTree->m_lRight);
        }
        else    //  唯一索引重复
        {
            _pRebuildGroup(pvAddr, pstSavm->tblName, pstTree, 0, lRow, lData);
            return pstRoot;
        }
    }

    lOffset = sizeof(SHTree) * ((TblDef *)pvAddr)->m_lGroup + lGetGrpPos(pstSavm->tblName);
    if(NULL == (pstCur = _pRebuildGroup(pvAddr, pstSavm->tblName, NULL, lOffset, lRow, lData)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return pstRoot;
    }

    pstCur->m_lIdx = lIdx;
    pstCur->m_lSePos = lOffset;
    pstCur->m_lParent = NODE_NULL;
    memcpy(pstCur->m_szIdx, psvIdx, pstCur->m_lIdx);
    //  设置父节点，和左右节点全部指向NIL节点
    pstCur->m_lLeft = pstCur->m_lRight = NODE_NULL;
    ((TblDef *)pvAddr)->m_lGroup ++;

    if(pstNode != NULL)
    {
        pstCur->m_eColor = COLOR_RED;
        pstCur->m_lParent = pstNode->m_lSePos;
        if(0 < memcmp(pstNode->m_szIdx, pstCur->m_szIdx, pstCur->m_lIdx))
            pstNode->m_lLeft = pstCur->m_lSePos;
        else
            pstNode->m_lRight = pstCur->m_lSePos;
    }
    else
    {
        pstRoot = pstCur;
        pstCur->m_eColor = COLOR_BLK;
        return pstRoot; //  开始为空节点
    }

    return _pFixupInsert(pvAddr, pstRoot, pstCur);
}

/*************************************************************************************************
    函数说明：清理表数据(不带事务)
    参数说明：
        pstSavm                    --操作句柄
        t                          --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lTruncate(SATvm *pstSavm, TABLE t)
{
    void    *pvData = NULL;
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, t)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return RC_SUCC;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    //    内存创建完毕，将初始化索引信息
    if(RC_SUCC != lInitailTree(pstSavm, (void *)pGetNode(pstRun->m_pvAddr, lGetIdxPos(t)), t))
    {
        pthread_rwlock_unlock(prwLock);
        vTblDisconnect(pstSavm, t);
        return RC_FAIL;
    }

    if(RC_SUCC != lInitailGroup(pstSavm, (void *)pGetNode(pstRun->m_pvAddr, lGetGrpPos(t)), t))
    {
        pthread_rwlock_unlock(prwLock);
        vTblDisconnect(pstSavm, t);
        return RC_FAIL;
    }

    pstSavm->m_lEffect = ((TblDef *)pstRun->m_pvAddr)->m_lValid;
    pvData = (void *)pGetNode(pstRun->m_pvAddr, lGetTblData(t));
    memset(pvData, 0, lGetTableSize(t) - lGetTblData(t));
    ((TblDef *)pstRun->m_pvAddr)->m_lGroup = 0;
    ((TblDef *)pstRun->m_pvAddr)->m_lValid = 0;

    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, t);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：根据数据区数据进行索引重建
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lRebuildIndex(SATvm *pstSavm, void *pvAddr)
{
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    TABLE   t = pstSavm->tblName;
    SHTree  *pstIRoot = NULL, *pstGRoot = NULL;
    size_t  lRow = 0, lOffset = lGetTblData(t), lIdx;

    //    内存创建完毕，将初始化索引信息
    if(RC_SUCC != lInitailTree(pstSavm, (void *)pGetNode(pvAddr, lGetIdxPos(t)), t))
        return RC_FAIL;

    if(RC_SUCC != lInitailGroup(pstSavm, (void *)pGetNode(pvAddr, lGetGrpPos(t)), t))
        return RC_FAIL;

    //    查询索引组重置
    ((TblDef *)pvAddr)->m_lGroup = 0;
    ((TblDef *)pvAddr)->m_lGroupRoot = ((TblDef *)pvAddr)->m_lGroupPos;
    pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
    for(lRow = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t));
        lOffset += lGetRowTruck(t), pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {
        if(IS_TRUCK_NULL(pstTruck))
            continue;

        if(HAVE_UNIQ_IDX(t))
        {
            memset(szIdx, 0, sizeof(szIdx));
            if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstTruck->m_pvData, szIdx))
            {
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            pstIRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot);
            pstIRoot = pRebuildTree(pstSavm, pstIRoot, szIdx, lGetIdxLen(t), lRow, lOffset);
            if(!pstIRoot)    return RC_FAIL;

            ((TblDef *)pvAddr)->m_lTreeRoot = pstIRoot->m_lSePos;
        }

        if(HAVE_NORL_IDX(t))
        {
            memset(szIdx, 0, sizeof(szIdx));
            if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstTruck->m_pvData, szIdx))
            {
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            pstGRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot);
            pstGRoot = pRebuildGroup(pstSavm, pstGRoot, szIdx, lGetGrpLen(t), lRow, lOffset);
            if(!pstGRoot)    return RC_FAIL;

            ((TblDef *)pvAddr)->m_lGroupRoot = pstGRoot->m_lSePos;
        }
        else if(HAVE_HASH_IDX(t))
        {
            memset(szIdx, 0, sizeof(szIdx));
            if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstTruck->m_pvData, szIdx))
            {
                pstSavm->m_lErrno = SVR_EXCEPTION;
                return RC_FAIL;
            }

            lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
            pstGRoot = pvAddr + ((TblDef *)pvAddr)->m_lGroupPos + lIdx * sizeof(SHTree);
            if(NULL == pstGRoot)    return RC_FAIL;

            if(RC_SUCC != lRebuildHash(pstSavm, t, pvAddr, pstGRoot, lRow, lOffset))
                return RC_FAIL;
        }

        lRow ++;    //    有记录数+1
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：按照表中数据进行索引重新
    参数说明：
        pstSavm                    --操作句柄
        t                          --表序号
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lRebuildIndex(SATvm *pstSavm, TABLE t)
{
    long    lRet;
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(t)))
        return RC_FAIL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(!HAVE_INDEX(pstSavm->tblName))
    {
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_SUCC;
    }

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    pstSavm->lSize = lGetRowSize(t);
    lRet = _lRebuildIndex(pstSavm, pstRun->m_pvAddr);
    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, pstSavm->tblName);

    return lRet;
}

/*************************************************************************************************
    函数说明：按照hash算法查询表数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        psvOut                     --查询结果
        plData                     --查询偏移量
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lSelectHash(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut, size_t *plData)
{
    size_t  lData, lIdx;
    SHTree  *pstTree = NULL;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN], *pvData = NULL;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    pstTree = pvAddr + ((TblDef *)pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
    if(NULL == pstTree || SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    if(RC_SUCC != pthread_rwlock_rdlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 0;
    pstSavm->m_lEType = EXE_PLAN_GRP;
    pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData);
    for(; SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
        {
            if(1 < (++ pstSavm->m_lEffect))
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = MORE_ROWS_SEL;
                return RC_FAIL;
            }

            *plData = pstList->m_lData;
            pvData = pstTruck->m_pvData;
            if(FIRST_ROW & pstSavm->lReMatch)    break;
        }

        if(SELF_POS_UNUSE == pstList->m_lNext)   break;
    }

    if(0 == pstSavm->m_lEffect)
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    memcpy(psvOut, pvData, lGetRowSize(pstSavm->tblName));
    pthread_rwlock_unlock(prwLock);
    pstTruck->m_lTimes ++;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：按照查询索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        psvOut                     --查询结果
        plData                     --查询偏移量
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lSelectGroup(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut, size_t *plData)
{
    void    *pvData = NULL;
    SHList  *pstList = NULL;
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    if(RC_SUCC != pthread_rwlock_rdlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {   
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == (pstList = pSearchGroup(pvAddr, pstTree, szIdx, lGetGrpLen(t))))
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_CONTINUE;
    }

    pstSavm->m_lEType = EXE_PLAN_GRP;
    for(; SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
        {
            if(1 < (++ pstSavm->m_lEffect))
            {
                pthread_rwlock_unlock(prwLock);
                pstSavm->m_lErrno = MORE_ROWS_SEL;
                return RC_FAIL;
            }

            *plData = pstList->m_lData;
            pvData = pstTruck->m_pvData;
            if(FIRST_ROW & pstSavm->lReMatch)    break;
        }

        if(SELF_POS_UNUSE == pstList->m_lNext)   break;
    }

    if(0 == pstSavm->m_lEffect)
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    memcpy(psvOut, pvData, lGetRowSize(pstSavm->tblName));
    pthread_rwlock_unlock(prwLock);
    pstTruck->m_lTimes ++;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(全表遍历方式)
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        psvOut                     --查询结果
        plData                     --查询偏移量
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lSelectTruck(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut, size_t *plData)
{
    void    *pvData = NULL;
    SHTruck *pstTruck = NULL;
    size_t  lRow = 0, lOffset = lGetTblData(t);
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);

    if(RC_SUCC != pthread_rwlock_rdlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 0;
    pstSavm->m_lEType = EXE_PLAN_ALL;
    pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
    for(lRow = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t)); 
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {        
        if(IS_TRUCK_NULL(pstTruck))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        lRow ++;
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }
        
        if(1 < (++ pstSavm->m_lEffect))
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = MORE_ROWS_SEL;
            return RC_FAIL;
        }

        *plData = lOffset;
        lOffset+= lGetRowTruck(t);
        pvData  = pstTruck->m_pvData;
        if(FIRST_ROW & pstSavm->lReMatch)   break;
    }

    if(0 == pstSavm->m_lEffect)
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    memcpy(psvOut, pvData, lGetRowSize(pstSavm->tblName));
    pthread_rwlock_unlock(prwLock);
    pstTruck->m_lTimes ++;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(全表遍历方式)
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        psvOut                     --查询结果
        plData                     --查询偏移量
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lSelectIndex(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut, size_t *plData)
{
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    if(RC_SUCC != pthread_rwlock_rdlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        return RC_FAIL;
    }

    pstSavm->m_lEType = EXE_PLAN_IDX;
    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {   
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    pstTree = (SHTree *)pSearchTree(pvAddr, pstTree, szIdx, lGetIdxLen(pstSavm->tblName));
    if(!pstTree)
    {
        pthread_rwlock_unlock(prwLock);
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstTruck = (PSHTruck)pGetNode(pvAddr, pstTree->m_lData);
    if(CHK_SELECT == pstSavm->lFind)
    {
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, CHK_SELECT))
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lErrno = NO_DATA_FOUND;
            return RC_FAIL;
        }
    }

    memcpy(psvOut, pstTruck->m_pvData, lGetRowSize(pstSavm->tblName));
    pthread_rwlock_unlock(prwLock);
    pstTruck->m_lTimes ++;
    pstSavm->m_lEffect = 1;
    *plData = pstTree->m_lData;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(唯一索引查询)
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        pstIdx                     --字段
        t                          --表序号
        ppstRoot                   --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lGroupIndex(SATvm *pstSavm, void *pvAddr, TblKey *pstIdx, TABLE t, Rowgrp **ppstRoot)
{
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {   
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    pstTree = (SHTree *)pSearchTree(pvAddr, pstTree, szIdx, lGetIdxLen(pstSavm->tblName));
    if(!pstTree)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstTruck = (PSHTruck)pGetNode(pvAddr, pstTree->m_lData);
    if(CHK_SELECT == pstSavm->lFind)
    {
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, CHK_SELECT))
        {
            pstSavm->m_lErrno = NO_DATA_FOUND;
            return RC_FAIL;
        }
    }

    *ppstRoot = pInsertRowgrp(pstSavm, *ppstRoot, NULL, pstTruck->m_pvData + pstIdx->m_lFrom, 
        pstIdx->m_lLen);
    if(NULL == *ppstRoot)
        return RC_FAIL;

    pstSavm->m_lEffect = 1;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：导出group索引组数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        pstIdx                     --字段
        t                          --表序号
        ppstRoot                   --查询的结果链表
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lGroupTree(SATvm *pstSavm, void *pvAddr, SHTree *pstTree, TblKey *pstIdx, Rowgrp **ppstRoot)
{
    SHTruck *pstTruck = NULL;

    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
        return RC_SUCC;

    pstTruck = (PSHTruck)pGetNode(pvAddr, ((SHList *)pGetNode(pvAddr, pstTree->m_lData))->m_lData);
    if(0 == lCountRowgrp(*ppstRoot, pstTruck->m_pvData + pstIdx->m_lFrom, pstIdx->m_lLen))
    {
        *ppstRoot = pInsertRowgrp(pstSavm, *ppstRoot, NULL, pstTruck->m_pvData + pstIdx->m_lFrom,
            pstIdx->m_lLen);
        if(NULL == *ppstRoot)
            return RC_FAIL;

        pstSavm->m_lEffect ++;
    }

    if(RC_SUCC != _lGroupTree(pstSavm, pvAddr, (SHTree *)pGetNode(pvAddr, pstTree->m_lLeft), 
        pstIdx, ppstRoot))
        return RC_FAIL;

    return _lGroupTree(pstSavm, pvAddr, (SHTree *)pGetNode(pvAddr, pstTree->m_lRight), pstIdx, 
        ppstRoot);
}

/*************************************************************************************************
    函数说明：按照查询索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        pstIdx                     --字段
        t                          --表序号
        ppstRoot                   --查询的结果链表
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lGroupGroup(SATvm *pstSavm, void *pvAddr, TblKey *pstIdx, TABLE t, Rowgrp **ppstRoot)
{
    BOOL    bRet;
    SHList  *pstList = NULL;
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    *pszIdx, szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    bRet = bIsGroupKey(t, pstIdx);
    pszIdx = pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx);
    if(!pszIdx && !bRet)    return RC_CONTINUE;

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {   
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(pszIdx && bRet)
    {
        if(NULL == (pstList = pSearchGroup(pvAddr, pstTree, szIdx, lGetGrpLen(t))))
        {
            pstSavm->m_lErrno = NO_DATA_FOUND;
            return RC_FAIL;
        }

        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        *ppstRoot = pInsertRowgrp(pstSavm, *ppstRoot, NULL, pstTruck->m_pvData + pstIdx->m_lFrom, 
            pstIdx->m_lLen);
        if(NULL == *ppstRoot)
            return RC_FAIL;

        pstSavm->m_lEffect = 1;
        return RC_SUCC;
    }

    pstSavm->m_lEffect  = 0;
    if(bRet)    return _lGroupTree(pstSavm, pvAddr, pstTree, pstIdx, ppstRoot);

    if(NULL == (pstList = pSearchGroup(pvAddr, pstTree, szIdx, lGetGrpLen(t))))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_CONTINUE;
    }

    for(; SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        if(0 != lCountRowgrp(*ppstRoot, pstTruck->m_pvData + pstIdx->m_lFrom, pstIdx->m_lLen))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        *ppstRoot = pInsertRowgrp(pstSavm, *ppstRoot, NULL, pstTruck->m_pvData + pstIdx->m_lFrom,
            pstIdx->m_lLen);
        if(NULL == *ppstRoot)
            return RC_FAIL;

        pstSavm->m_lEffect ++;
        if(SELF_POS_UNUSE == pstList->m_lNext)    break;
    }

    if(0 == pstSavm->m_lEffect)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：按照查询索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        pstIdx                     --字段
        t                          --表序号
        ppstRoot                   --查询的结果链表
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lGroupHash(SATvm *pstSavm, void *pvAddr, TblKey *pstIdx, TABLE t, Rowgrp **ppstRoot)
{
    size_t  lIdx;
    SHTree  *pstTree = NULL;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    pstTree = pvAddr + ((TblDef *)pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
    if(NULL == pstTree || SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    for(pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData); SELF_POS_UNUSE != pstList->m_lPos; 
        pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)
                break;
            continue;
        }

        if(0 != lCountRowgrp(*ppstRoot, pstTruck->m_pvData + pstIdx->m_lFrom, pstIdx->m_lLen))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        *ppstRoot = pInsertRowgrp(pstSavm, *ppstRoot, NULL, pstTruck->m_pvData + pstIdx->m_lFrom,
            pstIdx->m_lLen);
        if(NULL == *ppstRoot)
            return RC_FAIL;

        pstSavm->m_lEffect ++;
        if(SELF_POS_UNUSE == pstList->m_lNext)    break;
    }

    if(0 == pstSavm->m_lEffect)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：按照查询索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        pstIdx                     --字段
        t                          --表序号
        ppstRoot                   --查询的结果链表
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lGroupTruck(SATvm *pstSavm, void *pvAddr, TblKey *pstIdx, TABLE t, Rowgrp **ppstRoot)
{
    SHTruck *pstTruck = NULL;
    size_t  lRow = 0, lOffset = lGetTblData(t);

    pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
    for(lRow = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t)); 
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {        
        if(IS_TRUCK_NULL(pstTruck))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        lRow ++;
        if(pstSavm->pstVoid && 
            (!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind)))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }
        
        if(0 != lCountRowgrp(*ppstRoot, pstTruck->m_pvData + pstIdx->m_lFrom, pstIdx->m_lLen))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        *ppstRoot = pInsertRowgrp(pstSavm, *ppstRoot, NULL, pstTruck->m_pvData + pstIdx->m_lFrom,
            pstIdx->m_lLen);
        if(NULL == *ppstRoot)
            return RC_FAIL;

        pstSavm->m_lEffect ++;
        lOffset += lGetRowTruck(t);
    }

    if(0 == pstSavm->m_lEffect)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(多条记录导出)
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表句柄
        ppstRoot                   --查询的结果链表
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lGroup(SATvm *pstSavm, RunTime *pstRun, Rowgrp **ppstRoot)
{
    long    lRet;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    TblKey  *pstIdx = pGetFldKey(pstSavm->tblName, pstSavm->lReField);

    if(!pstIdx)
    {
        pstSavm->m_lErrno = FLD_NOT_EXIST;
        return RC_FAIL;
    }

    if(RC_SUCC != pthread_rwlock_rdlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        return RC_FAIL;
    }

    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        lRet = _lGroupIndex(pstSavm, pstRun->m_pvAddr, pstIdx, pstSavm->tblName, ppstRoot);
        if(RC_CONTINUE != lRet) 
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lEType = EXE_PLAN_IDX;
            return lRet;
        }
    }

    //    对于查询索引，可能某些字段本身存在为空，这里就无法判断，必须有人工来选择 pstSavm->lFind
    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        lRet = _lGroupGroup(pstSavm, pstRun->m_pvAddr, pstIdx, pstSavm->tblName, ppstRoot);
        if(RC_CONTINUE != lRet)
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lEType = EXE_PLAN_GRP;
            return lRet;
        }
    }
    else if(HAVE_HASH_IDX(pstSavm->tblName))
    {
        lRet = _lGroupHash(pstSavm, pstRun->m_pvAddr, pstIdx, pstSavm->tblName, ppstRoot);
        if(RC_CONTINUE != lRet)
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lEType = EXE_PLAN_GRP;
            return lRet;
        }
    }

    pstSavm->m_lEType = EXE_PLAN_ALL;
    lRet = _lGroupTruck(pstSavm, pstRun->m_pvAddr, pstIdx, pstSavm->tblName, ppstRoot);
    pthread_rwlock_unlock(prwLock);

    if(pstSavm->lReMatch & ORDER_DESC || pstSavm->lReMatch & ORDER_ASC)
        vSortRowgrp(ppstRoot, pstIdx, pstSavm->lReMatch); 

    return lRet;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(多条记录导出)
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lGroup(SATvm *pstSavm, Rowgrp **ppsvOut)
{
    long    lRet;
    RunTime *pstRun = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    pstSavm->lReMatch |= GROUP_BY;
    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return lQueryByRt(pstSavm, pstRun, pstSavm->tblName, 0, ppsvOut);
    else
    {
        lRet = _lGroup(pstSavm, pstRun, ppsvOut);
        vTblDisconnect(pstSavm, pstSavm->tblName);
    }

    return lRet;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        plOut                      --查询的结果条数
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lQueryIndex(SATvm *pstSavm, void *pvAddr, TABLE t, size_t *plOut, void **ppsvOut)
{
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {   
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    pstTree = (SHTree *)pSearchTree(pvAddr, pstTree, szIdx, lGetIdxLen(pstSavm->tblName));
    if(!pstTree)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstTruck = (PSHTruck)pGetNode(pvAddr, pstTree->m_lData);
    if(CHK_SELECT == pstSavm->lFind)
    {
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, CHK_SELECT))
        {
            pstSavm->m_lErrno = NO_DATA_FOUND;
            return RC_FAIL;
        }
    }

    if(NULL == (*ppsvOut = (char *)realloc(*ppsvOut, lGetRowSize(pstSavm->tblName))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    pstTruck->m_lTimes ++;
    *plOut = pstSavm->m_lEffect = 1;
    memcpy(*ppsvOut, pstTruck->m_pvData, lGetRowSize(pstSavm->tblName));

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(hash索引查询)
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        plOut                      --查询的结果条数
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lQueryHash(SATvm *pstSavm, void *pvAddr, TABLE t, size_t *plOut, void **ppsvOut)
{
    SHTree  *pstTree = NULL;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    size_t  lData, lIdx, lPos;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    pstTree = pvAddr + ((TblDef *)pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
    if(NULL == pstTree || SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData);
    for(*plOut = 0, lPos = 0; SELF_POS_UNUSE != pstList->m_lPos; 
        pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        pstTruck->m_lTimes ++;
        lPos = (++ (*plOut)) * lGetRowSize(t);
        if(NULL == (*ppsvOut = (char *)realloc(*ppsvOut, lPos)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_FAIL;
        }

        memcpy(*ppsvOut + (lPos - lGetRowSize(t)), pstTruck->m_pvData, lGetRowSize(t));

        if(SELF_POS_UNUSE == pstList->m_lNext)    break;
    }

    if(0 == (pstSavm->m_lEffect = *plOut))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：按照查询索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        plOut                      --查询的结果条数
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lQueryGroup(SATvm *pstSavm, void *pvAddr, TABLE t, size_t *plOut, void **ppsvOut)
{
    size_t  lPos;
    TblKey  *pstIdx = NULL;
    SHList  *pstList = NULL;
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {   
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == (pstList = pSearchGroup(pvAddr, pstTree, szIdx, lGetGrpLen(t))))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_CONTINUE;
    }

    for(*plOut = 0, lPos = 0; SELF_POS_UNUSE != pstList->m_lPos; 
        pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }    
    
        pstTruck->m_lTimes ++;
        lPos = (++ (*plOut)) * lGetRowSize(t);
        if(NULL == (*ppsvOut = (char *)realloc(*ppsvOut, lPos)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_FAIL;
        }
    
        memcpy(*ppsvOut + (lPos - lGetRowSize(t)), pstTruck->m_pvData, lGetRowSize(t));
        if(SELF_POS_UNUSE == pstList->m_lNext)    break;
    }

    if(0 == (pstSavm->m_lEffect = *plOut))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        plOut                      --查询的结果条数
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lQueryTruck(SATvm *pstSavm, void *pvAddr, TABLE t, size_t *plOut, void **ppsvOut)
{
    SHTruck *pstTruck = NULL;
    size_t  lRow = 0, lOffset = lGetTblData(t), lPos = 0;

    pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
    for(lRow = 0, *plOut = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t)); 
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {        
        if(IS_TRUCK_NULL(pstTruck))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        lRow ++;
        if(pstSavm->pstVoid && 
            (!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind)))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }
        
        lPos = (++ (*plOut)) * lGetRowSize(t);
        if(NULL == (*ppsvOut = (char *)realloc(*ppsvOut, lPos)))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_FAIL;
        }

        memcpy(*ppsvOut + (lPos - lGetRowSize(t)), pstTruck->m_pvData, lGetRowSize(t));
        lOffset += lGetRowTruck(t);
    }

    if(0 == (pstSavm->m_lEffect = *plOut))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(多条记录导出)
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表数据地址
        plOut                      --查询的结果条数
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lQuery(SATvm *pstSavm, RunTime *pstRun, size_t *plOut, void **ppsvOut)
{
    long    lRet;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);

    if(RC_SUCC != pthread_rwlock_rdlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        return RC_FAIL;
    }

    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        lRet = _lQueryIndex(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, plOut, ppsvOut);
        if(RC_CONTINUE != lRet) 
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lEType = EXE_PLAN_IDX;
            return lRet;
        }
    }

    //    对于查询索引，可能某些字段本身存在为空，这里就无法判断，必须有人工来选择 pstSavm->lFind
    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        lRet = _lQueryGroup(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, plOut, ppsvOut);
        if(RC_CONTINUE != lRet)
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lEType = EXE_PLAN_GRP;
            return lRet;
        }
    }
    else if(HAVE_HASH_IDX(pstSavm->tblName))
    {
        lRet = _lQueryHash(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, plOut, ppsvOut);
        if(RC_CONTINUE != lRet)
        {
            pthread_rwlock_unlock(prwLock);
            pstSavm->m_lEType = EXE_PLAN_GRP;
            return lRet;
        }
    }

    pstSavm->m_lEType = EXE_PLAN_ALL;
    lRet = _lQueryTruck(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, plOut, ppsvOut);
    pthread_rwlock_unlock(prwLock);

    return lRet;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(多条记录导出)
    参数说明：
        pstSavm                    --操作句柄
        plOut                      --查询的结果条数
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lQuery(SATvm *pstSavm, size_t *plOut, void **ppsvOut)
{
    long    lRet;
    RunTime *pstRun = NULL;
    TblKey  *pstIdx = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return lQueryByRt(pstSavm, pstRun, pstSavm->tblName, plOut, ppsvOut);

    if(pstSavm->lReMatch & GROUP_BY)
    {
        lRet =  _lGroup(pstSavm, pstRun, (Rowgrp **)ppsvOut);
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return lRet;
    }

    if(RC_SUCC != (lRet = _lQuery(pstSavm, pstRun, plOut, ppsvOut)))
    {
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    if(pstSavm->lReMatch & ORDER_DESC || pstSavm->lReMatch & ORDER_ASC)
    {
        if(NULL == (pstIdx = pGetFldKey(pstSavm->tblName, pstSavm->lReField)))
         {
            pstSavm->m_lErrno = FLD_NOT_EXIST;
            return RC_FAIL;
        }

        lRet = lSortRowList(pstSavm, *plOut, *ppsvOut, pstIdx, lGetRowSize(pstSavm->tblName));
    }

    vTblDisconnect(pstSavm, pstSavm->tblName);
    return lRet;
}

/*************************************************************************************************
    函数说明：查询内存表单条记录返回结果和位置
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表数据地址
        t                          --表序号
        plOut                      --查询的结果条数
        psvOut                     --查询的结果数据
        plData                     --偏移量
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lSelect(SATvm *pstSavm, RunTime *pstRun, TABLE t, void *psvOut, size_t *plData)
{
    long    lRet;

    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        lRet = _lSelectIndex(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, psvOut, plData);
        if(RC_CONTINUE != lRet)    return lRet;
    }

    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        lRet = _lSelectGroup(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, psvOut, plData);
        if(RC_CONTINUE != lRet)    return lRet;
    }
    else if(HAVE_HASH_IDX(t))
    {
        lRet = _lSelectHash(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, psvOut, plData);
        if(RC_CONTINUE != lRet)    return lRet;
    }

    return _lSelectTruck(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, psvOut, plData);
}

/*************************************************************************************************
    函数说明：查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lSelect(SATvm *pstSavm, void *psvOut)
{
    long    lRet;
    size_t  lData = 0;
    RunTime *pstRun  = NULL;

    if(!pstSavm || !pstSavm->pstVoid)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return lSelectByRt(pstSavm, pstRun, pstSavm->tblName, psvOut);

    lRet = _lSelect(pstSavm, pstRun, pstSavm->tblName, psvOut, &lData);
    vTblDisconnect(pstSavm, pstSavm->tblName);
    return lRet;
}

/*************************************************************************************************
    函数说明：新增唯一索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        ppstTruck                  --数据块
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lInsertIndex(SATvm *pstSavm, void *pvAddr, TABLE t, SHTruck **ppstTruck)
{
    SHTree  *pstRoot = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == (pstRoot = pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {   
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstSavm->pstVoid, szIdx))
    {
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        return RC_FAIL;
    }

    pstRoot = pInsertTree(pstSavm, pstRoot, szIdx, lGetIdxLen(t), ppstTruck);
    if(!pstRoot)    return RC_FAIL;

    ((TblDef *)pvAddr)->m_lTreeRoot = pstRoot->m_lSePos;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：新增hash表指示数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        ppstTruck                  --数据块
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lInsertHash(SATvm *pstSavm, void *pvAddr, TABLE t, SHTruck **ppstTruck)
{
    size_t  lIdx, lOffset;
    SHTree  *pstTree = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
    {
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        return RC_FAIL;
    }

    lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    lOffset = ((TblDef *)pvAddr)->m_lGroupPos + lIdx * sizeof(SHTree);
    if(NULL == (pstTree = (SHTree *)(pvAddr + lOffset)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    return __lInsertHash(pstSavm, pvAddr, t, pstTree, lOffset, ppstTruck);
}

/*************************************************************************************************
    函数说明：新增查询索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        ppstTruck                  --数据块
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lInsertGroup(SATvm *pstSavm, void *pvAddr, TABLE t, SHTruck **ppstTruck)
{
    SHTree  *pstRoot = NULL;
    char    szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == (pstRoot = pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {   
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
    {
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        return RC_FAIL;
    }

    pstRoot = pInsertGroup(pstSavm, pstRoot, szIdx, lGetGrpLen(t), ppstTruck);
    if(!pstRoot)    return RC_FAIL;

    ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;    

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：新增数据块
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
        ppstTruck                  --数据块
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lInsertTruck(SATvm *pstSavm, void *pvAddr, TABLE t, SHTruck *pstTruck, ulong uTimes)
{
    SHTruck    *pvTruck = pstTruck;

    if(!pvTruck)    //    说明无索引
        pvTruck = pGetNode(pvAddr, lGetTblData(t) + lGetRowTruck(t) * ((TblDef *)pvAddr)->m_lValid);

    SET_DATA_TRUCK(pvTruck, DATA_TRUCK_NRML);
    pvTruck->m_lTimes = uTimes;
    memcpy(pvTruck->m_pvData, pstSavm->pstVoid, lGetRowSize(t));

    ((TblDef *)pvAddr)->m_lValid ++;
    pstSavm->m_lEffect = 1;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：向指定内存表中插入数据
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表序号
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    __lInsert(SATvm *pstSavm, void *pvAddr, TABLE t, ulong uTimes)
{
    SHTruck *pstTruck = NULL;

    //    要有回滚，新增一个索引有问题不能导致其他数据出问题
    if(HAVE_UNIQ_IDX(t))
    {
        if(RC_SUCC != _lInsertIndex(pstSavm, pvAddr, t, &pstTruck))
            return RC_FAIL;
    }

    if(HAVE_NORL_IDX(t))
    {
        if(RC_SUCC != _lInsertGroup(pstSavm, pvAddr, t, &pstTruck))
            return RC_FAIL;
    }
    else if(HAVE_HASH_IDX(t))
    {
        if(RC_SUCC != _lInsertHash(pstSavm, pvAddr, t, &pstTruck))
            return RC_FAIL;
    }

    return _lInsertTruck(pstSavm, pvAddr, t, pstTruck, uTimes);
}

/*************************************************************************************************
    函数说明：异常情况对指定表进行解锁
    参数说明：
        pstSavm                    --操作句柄
        bf                         
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lResetLock(TABLE table, BOOL bf)
{
    RWAttr  attr;
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;
    SATvm   *pstSavm = NULL;

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(table)))
        return RC_FAIL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, table)))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(bf)
    {
        pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_rwlock_init(prwLock, &attr);
    }
    else
    {
        pthread_rwlock_unlock(prwLock);
    }

    vTblDisconnect(pstSavm, table);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：向指定内存表中插入数据
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lInsert(SATvm *pstSavm)
{
    long    lRet;
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;

    if(!pstSavm || !pstSavm->pstVoid)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return lInsertByRt(pstSavm, pstRun, pstSavm->tblName);

    if(lGetTblRow(pstSavm->tblName) <= ((TblDef *)pstRun->m_pvAddr)->m_lValid)
    {
        pstSavm->m_lErrno = DATA_SPC_FULL;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    lRet = __lInsert(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, 0);
    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, pstSavm->tblName);

    if(RC_SUCC != lRet)
        return RC_FAIL;

    return lRecordWork(pstSavm, pstSavm->pstVoid, OPERATE_INSERT);
}

/*************************************************************************************************
    函数说明：交易流水式新增记录
    参数说明：lInsertTrans()
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lInsertTrans(SATvm *pstSavm, size_t *plOffset, llSEQ *pllSeq)
{
    size_t    lOffset;
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;
    SHTruck *pstTruck = NULL;

    if(!pstSavm || !pstSavm->pstVoid)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(lGetTblRow(pstSavm->tblName) <= ((TblDef *)pstRun->m_pvAddr)->m_lValid)
    {
        pstSavm->m_lErrno = DATA_SPC_FULL;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    //    要有回滚，新增一个索引有问题不能导致其他数据出问题
    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        if(RC_SUCC != _lInsertIndex(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, &pstTruck))
            goto ErrInsert;
    }

    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        if(RC_SUCC != _lInsertGroup(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, &pstTruck))
            goto ErrInsert;
    }
    else if(HAVE_HASH_IDX(pstSavm->tblName))
    {
        if(RC_SUCC != _lInsertHash(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, &pstTruck))
            goto ErrInsert;
    }

    if(!pstTruck)    //    说明无索引
    {
        lOffset = lGetTblData(pstSavm->tblName) + 
            lGetRowTruck(pstSavm->tblName) * ((TblDef *)pstRun->m_pvAddr)->m_lValid;
        pstTruck = pGetNode(pstRun->m_pvAddr, lOffset);
    }

    lOffset = (void *)pstTruck->m_pvData - (void *)pstRun->m_pvAddr;
    SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
    pstTruck->m_lTimes = 0;
    memcpy(pstTruck->m_pvData, pstSavm->pstVoid, lGetRowSize(pstSavm->tblName));
    ((TblDef *)pstRun->m_pvAddr)->m_lValid ++;
    ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ ++;
    if(pllSeq)    *pllSeq = ((TblDef *)pstRun->m_pvAddr)->m_lExSeQ;

    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, pstSavm->tblName);
    pstSavm->m_lEffect = 1;
    if(plOffset)    *plOffset = lOffset;

    return RC_SUCC;

ErrInsert:
    pthread_rwlock_unlock(prwLock);
      vTblDisconnect(pstSavm, pstSavm->tblName);
    return RC_FAIL;
}

/*************************************************************************************************
    函数说明：用唯一索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表句柄
        t                          --表序号
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _llFetchIndex(SATvm *pstSavm, RunTime *pstRun, TABLE t, void *psvOut)
{
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    void    *pvAddr = pGetAddr(pstSavm, t);

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstRun->pstVoid, szIdx))
    {
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {   
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    pstTree = (SHTree *)pSearchTree(pvAddr, pstTree, szIdx, lGetIdxLen(t));
    if(!pstTree)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_NOTFOUND;
    }

    pstTruck = (PSHTruck)pGetNode(pvAddr, pstTree->m_lData);
    if(CHK_SELECT == pstRun->lFind)
    {
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstRun->pstVoid, pstRun->lFind))
        {
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }
    }

    if(IS_TRUCK_NULL(pstTruck))
        return RC_NOTFOUND;

    pstRun->m_lCurLine ++;
    pstRun->m_pvCurAddr = NULL;

    memcpy(psvOut, pstTruck->m_pvData, lGetRowSize(t));

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：用匹配索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表句柄
        t                          --表序号
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _llFetchGroup(SATvm *pstSavm, RunTime *pstRun, TABLE t, void *psvOut)
{
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    void    *pvAddr = pGetAddr(pstSavm, t);

    for(pstList = (SHList *)pstRun->m_pvCurAddr; pstList && (SELF_POS_UNUSE != pstList->m_lPos); 
        pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(IS_TRUCK_NULL(pstTruck))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstRun->pstVoid, pstRun->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        pstRun->m_lCurLine ++;
        memcpy(psvOut, pstTruck->m_pvData, lGetRowSize(t));
        if(SELF_POS_UNUSE == pstList->m_lNext)
            pstRun->m_pvCurAddr = NULL;
        else
            pstRun->m_pvCurAddr = (void *)pGetNode(pvAddr, pstList->m_lNext);

        return RC_SUCC;
    }

    return RC_NOTFOUND;
}

/*************************************************************************************************
    函数说明：遍历查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表句柄
        t                          --表序号
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _llFetchTruck(SATvm *pstSavm, RunTime *pstRun, TABLE t, void *psvOut)
{
    SHTruck *pstTruck = NULL;
    long    lRow = 0, lOffset;
    void    *pvAddr = pGetAddr(pstSavm, t);

    if(1 == pstRun->m_lCurLine)
    {
        lOffset = lGetTblData(t);
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
    }
    else
    {
        lOffset = pstRun->m_pvCurAddr - pvAddr;
        pstTruck = (PSHTruck)pstRun->m_pvCurAddr;
    }

    for(lRow = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t));
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {
        if(IS_TRUCK_NULL(pstTruck))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        lRow ++;
        if(pstRun->pstVoid && !bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstRun->pstVoid, 
            pstRun->lFind))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        pstRun->m_lCurLine ++;
        lOffset += lGetRowTruck(t);
        pstRun->m_pvCurAddr = pGetNode(pvAddr, lOffset);
        memcpy(psvOut, pstTruck->m_pvData, lGetRowSize(t));

        return RC_SUCC;
    }

    return RC_NOTFOUND;
}

/*************************************************************************************************
    函数说明：定义游标查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        t                          --表序号
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lTableDeclare(SATvm *pstSavm, TABLE t)
{
    long    lIdx;
    SHTree  *pstTree = NULL;
    RunTime *pstRun = NULL;
    char    szIdx[MAX_INDEX_LEN];

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, t)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
    {
        pstSavm->m_lErrno = RMT_NOT_SUPPT;
        return RC_FAIL;
    }

    pstSavm->tblName   = t;
    pstRun->m_lCurLine = 1;
    pstRun->m_lCurType = EXE_PLAN_ALL;
    pstRun->m_lRowSize = pstSavm->lSize;
    pstRun->m_pvCurAddr = pstRun->m_pvAddr;

    if(!pstSavm->pstVoid)    return RC_SUCC;

    pstRun->pstVoid = (char *)malloc(pstSavm->lSize);
    memcpy(pstRun->pstVoid, pstSavm->pstVoid, pstSavm->lSize);

    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        memset(szIdx, 0, sizeof(szIdx));
        if(NULL != pGetIndex(pstSavm, lGetIdxNum(pstSavm->tblName), pGetTblIdx(pstSavm->tblName), 
            pstSavm->pstVoid, szIdx))
        {
            pstRun->m_lCurType = EXE_PLAN_IDX;
            return RC_SUCC;
        }
    }

    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        memset(szIdx, 0, sizeof(szIdx));
        if(NULL == pGetIndex(pstSavm, lGetGrpNum(pstSavm->tblName), pGetTblGrp(pstSavm->tblName), 
            pstSavm->pstVoid, szIdx))
            return RC_SUCC;
    
        pstRun->m_lCurType = EXE_PLAN_GRP;
        pstRun->m_pvCurAddr = pSearchGroup(pstRun->m_pvAddr, (SHTree *)pGetNode(pstRun->m_pvAddr,
            ((TblDef *)pstRun->m_pvAddr)->m_lGroupRoot), szIdx, lGetGrpLen(t));

        return RC_SUCC;
    }
    else if(HAVE_HASH_IDX(pstSavm->tblName))
    {
        memset(szIdx, 0, sizeof(szIdx));
        if(NULL == pGetIndex(pstSavm, lGetGrpNum(pstSavm->tblName), pGetTblGrp(pstSavm->tblName), 
            pstSavm->pstVoid, szIdx))
            return RC_SUCC;

        pstRun->m_lCurType = EXE_PLAN_GRP;
        lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pstRun->m_pvAddr)->m_lMaxRow;
        pstTree = pstRun->m_pvAddr + ((TblDef *)pstRun->m_pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
        if(NULL == pstTree || SELF_POS_UNUSE == pstTree->m_lData)
        {
            pstRun->m_pvCurAddr = NULL;
            return RC_SUCC;
        }

        pstRun->m_pvCurAddr = (SHList *)pGetNode(pstRun->m_pvAddr, pstTree->m_lData);
        return RC_SUCC;
    }    

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：使用游标获取数据
    参数说明：
        pstSavm                    --操作句柄
        t                          --表序号
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lTableFetch(SATvm *pstSavm, TABLE t, void *psvOut)
{
    long    lRet;
    RunTime    *pstRun = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CURS_IS_INVAL;
        return RC_FAIL;
    }

    pstRun = (RunTime *)pGetRunTime(pstSavm, t);
    if(NULL == pstRun->m_pvCurAddr)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_NOTFOUND;
    }

    if(RES_REMOT_SID == pstRun->m_lLocal)
    {
        pstSavm->m_lErrno = RMT_NOT_SUPPT;
        return RC_FAIL;
    }

    if(EXE_PLAN_IDX == pstRun->m_lCurType)
        lRet = _llFetchIndex(pstSavm, pstRun, t, psvOut);
    else if(EXE_PLAN_GRP == pstRun->m_lCurType)
        lRet = _llFetchGroup(pstSavm, pstRun, t, psvOut);
    else
        lRet = _llFetchTruck(pstSavm, pstRun, t, psvOut);
    if(RC_NOTFOUND == lRet)
    {
        pstRun->m_pvCurAddr = NULL;
        pstSavm->m_lErrno = NO_DATA_FOUND;
    }

    return lRet;    
}


/*************************************************************************************************
    函数说明：遍历查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表句柄
        t                          --表序号
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lNextTruck(SATvm *pstSavm, RunTime *pstRun, TABLE t, void **ppvOAddr)
{
    SHTruck *pstTruck = NULL;
    long    lRow = 0, lOffset;
    void    *pvAddr = pGetAddr(pstSavm, t);

    if(1 == pstRun->m_lCurLine)
    {
        lOffset = lGetTblData(t);
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
    }
    else
    {
        lOffset = pstRun->m_pvCurAddr - pvAddr;
        pstTruck = (PSHTruck)pstRun->m_pvCurAddr;
    }

    for(lRow = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t));
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {
        if(IS_TRUCK_NULL(pstTruck))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        lRow ++;
        if(pstRun->pstVoid && !bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstRun->pstVoid, 
            pstRun->lFind))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        pstRun->m_lCurLine ++;
        *ppvOAddr = pstTruck->m_pvData;
        lOffset += lGetRowTruck(t);
        pstRun->m_pvCurAddr = pGetNode(pvAddr, lOffset);
        return RC_SUCC;
    }

    return RC_NOTFOUND;
}

/*************************************************************************************************
    函数说明：用查询索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表句柄
        t                          --表序号
        ppvOAddr                   --结果数据地址
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lNextGroup(SATvm *pstSavm, RunTime *pstRun, TABLE t, void **ppvOAddr)
{
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    void    *pvAddr = pGetAddr(pstSavm, t);

    for(pstList = (SHList *)pstRun->m_pvCurAddr; pstList && (SELF_POS_UNUSE != pstList->m_lPos); 
        pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(IS_TRUCK_NULL(pstTruck))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstRun->pstVoid, pstRun->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        pstRun->m_lCurLine ++;
        *ppvOAddr = pstTruck->m_pvData;
        if(SELF_POS_UNUSE == pstList->m_lNext)
            pstRun->m_pvCurAddr = NULL;
        else
            pstRun->m_pvCurAddr = (void *)pGetNode(pvAddr, pstList->m_lNext);

        return RC_SUCC;
    }

    return RC_NOTFOUND;
}

/*************************************************************************************************
    函数说明：用唯一索引查询内存表中的数据
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表句柄
        t                          --表序号
        ppvOAddr                   --结果数据地址
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lNextIndex(SATvm *pstSavm, RunTime *pstRun, TABLE t, void **ppvOAddr)
{
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    void    *pvAddr = pGetAddr(pstSavm, t);

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstRun->pstVoid, szIdx))
    {
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {   
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    pstTree = (SHTree *)pSearchTree(pvAddr, pstTree, szIdx, lGetIdxLen(t));
    if(!pstTree)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_NOTFOUND;
    }

    pstTruck = (PSHTruck)pGetNode(pvAddr, pstTree->m_lData);
    if(CHK_SELECT == pstRun->lFind)
    {
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstRun->pstVoid, pstRun->lFind))
        {
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }
    }

    if(IS_TRUCK_NULL(pstTruck))
        return RC_NOTFOUND;

    pstRun->m_lCurLine ++;
    pstRun->m_pvCurAddr = NULL;
    *ppvOAddr = pstTruck->m_pvData;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：lNextFetch(SATvm *pstSavm)
    参数说明：
        pstSavm                    --操作句柄
        t                          --表序号
        ppvOAddr                   --结果数据地址
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lNextFetch(SATvm *pstSavm, TABLE t, void **ppvOAddr)
{
    long       lRet;
    RunTime    *pstRun = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CURS_IS_INVAL;
        return RC_FAIL;
    }

    pstRun = (RunTime *)pGetRunTime(pstSavm, t);
    if(NULL == pstRun->m_pvCurAddr)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_NOTFOUND;
    }

    if(RES_REMOT_SID == pstRun->m_lLocal)
    {
        pstSavm->m_lErrno = RMT_NOT_SUPPT;
        return RC_FAIL;
    }

    if(EXE_PLAN_IDX == pstRun->m_lCurType)
        lRet = _lNextIndex(pstSavm, pstRun, t, ppvOAddr);
    else if(EXE_PLAN_GRP == pstRun->m_lCurType)
        lRet = _lNextGroup(pstSavm, pstRun, t, ppvOAddr);
    else
        lRet = _lNextTruck(pstSavm, pstRun, t, ppvOAddr);
    if(RC_NOTFOUND == lRet)
    {
        pstRun->m_pvCurAddr = NULL;
        pstSavm->m_lErrno = NO_DATA_FOUND;
    }

    return lRet;    
}

/*************************************************************************************************
    函数说明：关闭游标
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
void    vTableClose(SATvm *pstSavm)
{
    _vTblRelease(pstSavm, pstSavm->tblName, pstSavm->m_bHold);
}

/*************************************************************************************************
    函数说明：更新记录hash索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        pstTruck                   --数据块
        lData                      --偏移量
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    __lUpdateHash(SATvm *pstSavm, void *pvAddr, TABLE t, SHTruck *pstTruck, size_t lData)
{
    SHList  *pstList = NULL;
    SHTree  *pstTree = NULL;
    size_t  lIdx, lOld, lOffset, lNext;
    char    szOld[MAX_INDEX_LEN], szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
    {
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        return RC_FAIL;
    }

    memset(szOld, 0, sizeof(szOld));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstTruck->m_pvData, szOld))
    {
        pstSavm->m_lErrno = IDX_DATA_MISM;
        return RC_FAIL;
    }

    lOld = uGetHash(szOld, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    if(lOld == lIdx)    return RC_SUCC;

    lOffset = ((TblDef *)pvAddr)->m_lGroupPos + lOld * sizeof(SHTree);
    if(NULL == (pstTree = (SHTree *)(pvAddr + lOffset)))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    lOffset = lGetListOfs(t) + ((TblDef *)pvAddr)->m_lValid * sizeof(SHList);
    for(pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData); SELF_POS_UNUSE != pstList->m_lPos;
        pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        if(lData != pstList->m_lData)
        {
            if(SELF_POS_UNUSE == pstList->m_lNext) break;
            continue;
        }

        lOffset -= sizeof(SHList);
        if(RC_SUCC != lReleaseList(pvAddr, t, pstTree, pstList, lOffset, &lNext))
        {
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }

        ((TblDef *)pvAddr)->m_lValid --;
        lOffset = RC_CLOSE;
        break;
    }

    if(SELF_POS_UNUSE == pstTree->m_lData)
        pstTree->m_lSePos = SELF_POS_UNUSE;

    if(RC_CLOSE != lOffset)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    lOffset = ((TblDef *)pvAddr)->m_lGroupPos + lIdx * sizeof(SHTree);
    if(NULL == (pstTree = (SHTree *)(pvAddr + lOffset)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(RC_SUCC !=  __lInsertHash(pstSavm, pvAddr, t, pstTree, lOffset, &pstTruck))
        return RC_FAIL;

    ((TblDef *)pvAddr)->m_lValid ++;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：更新记录查询索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        pstTruck                   --数据块
        lData                      --偏移量
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    __lUpdateGroup(SATvm *pstSavm, void *pvAddr, TABLE t, SHTruck *pstTruck, size_t lData)
{
    SHList  *pstList = NULL;
    size_t  lNext = 0, lOffset;
    SHTree  *pstRoot, *pstTree = NULL;
    char    szOld[MAX_INDEX_LEN], szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
    {
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        return RC_FAIL;
    }

    memset(szOld, 0, sizeof(szOld));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstTruck->m_pvData, szOld))
    {
        pstSavm->m_lErrno = IDX_DATA_MISM;
        return RC_FAIL;
    }

    if(!memcmp(szIdx, szOld, MAX_INDEX_LEN)) // 索引未变动
        return RC_SUCC;

    if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == (pstTree = pSearchTree(pvAddr, pstRoot, szOld, lGetGrpLen(t))))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    lOffset  = lGetListOfs(t) + ((TblDef *)pvAddr)->m_lValid * sizeof(SHList);
    for(pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData); SELF_POS_UNUSE != pstList->m_lPos;
        pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        if(lData != pstList->m_lData)
        {
            if(SELF_POS_UNUSE == pstList->m_lNext) break;
            continue;
        }

        lOffset -= sizeof(SHList);
        if(RC_SUCC != lReleaseList(pvAddr, t, pstTree, pstList, lOffset, &lNext))
        {
            pstSavm->m_lErrno = SVR_EXCEPTION;
            return RC_FAIL;
        }

        ((TblDef *)pvAddr)->m_lValid --;
        lOffset = RC_CLOSE;
        break;
    }

    if(SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstRoot = _pDeleteGroup(pvAddr, pstSavm->tblName, pstRoot, pstTree, &pstSavm->m_lEffect);
        ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;
    }

    if(RC_CLOSE != lOffset)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    if(NULL == (pstRoot = pInsertGroup(pstSavm, pstRoot, szIdx, lGetGrpLen(t), &pstTruck)))
        return RC_FAIL;

    ((TblDef *)pvAddr)->m_lValid ++;
    ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：更新记录唯一索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        pstTruck                   --数据块
        lData                      --偏移量
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long   __lIndexUpdate(SATvm *pstSavm, void *pvAddr, TABLE t, SHTruck *pstTruck, size_t lData)
{
    SHTree  *pstRoot, *pstTree = NULL;
    char    szOld[MAX_INDEX_LEN], szIdx[MAX_INDEX_LEN];

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstSavm->pstVoid, szIdx))
    {
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        return RC_FAIL;
    }

    memset(szOld, 0, sizeof(szOld));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstTruck->m_pvData, szOld))
    {
        pstSavm->m_lErrno = IDX_DATA_MISM;
        return RC_FAIL;
    }

    if(!memcmp(szIdx, szOld, MAX_INDEX_LEN)) // 索引未变动
        return RC_SUCC;

    if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    //    唯一索引采用先查询后新增，避免删除后报唯一性错误
    if(NULL != (SHTree *)pSearchTree(pvAddr, pstRoot, szIdx, lGetIdxLen(t)))
    {
        pstSavm->m_lErrno = UNIQ_IDX_REPT;
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pSearchTree(pvAddr, pstRoot, szOld, lGetIdxLen(t))))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    if(lData != pstTree->m_lData)
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(NULL == (pstRoot = _pDeleteTree(pvAddr, pstSavm->tblName, pstRoot, pstTree)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }
    ((TblDef *)pvAddr)->m_lValid --;

    if(NULL == (pstRoot = pInsertTree(pstSavm, pstRoot, szIdx, lGetIdxLen(t), &pstTruck)))
        return RC_FAIL;
    ((TblDef *)pvAddr)->m_lValid ++;
    ((TblDef *)pvAddr)->m_lTreeRoot = pstRoot->m_lSePos;
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查找数据并更新唯一索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        pstTruck                   --数据块
        bIsNil                     --是否全字段覆盖
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lUpdateIndex(SATvm *pstSavm, void *pvAddr, TABLE t, void *pvUpdate, const BOOL bIsNil)
{
    size_t  lData;
    long    lRet = RC_SUCC;
    void    *pvData = NULL;
    SHTruck *pstTruck = NULL;
    SHTree  *pstRoot, *pstTree = NULL;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);
    char    szOld[MAX_INDEX_LEN], szIdx[MAX_INDEX_LEN];

    memset(szOld, 0, sizeof(szOld));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstSavm->pstVoid, szOld))
        return RC_CONTINUE;
    
    if(NULL == (pvData = (void *)malloc(lGetRowSize(pstSavm->tblName))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(RC_SUCC != pthread_rwlock_wrlock(prwLock)) // 防止查询删除将数据修改
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        IBPFree(pvData);
        return RC_FAIL;
    }

    pstSavm->m_lEType = EXE_PLAN_IDX;
    if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {
        pthread_rwlock_unlock(prwLock);    
        pstSavm->m_lErrno = SVR_EXCEPTION;
        IBPFree(pvData);
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pSearchTree(pvAddr, pstRoot, szOld, lGetIdxLen(t))))
    {
        pthread_rwlock_unlock(prwLock);    
        pstSavm->m_lErrno = NO_DATA_FOUND;
        IBPFree(pvData);
        return RC_FAIL;
    }

    lData = pstTree->m_lData;
    pstTruck = (PSHTruck)pGetNode(pvAddr, pstTree->m_lData);
    if(CHK_SELECT == pstSavm->lFind)
    {
        if(!bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
        {
            pthread_rwlock_unlock(prwLock);    
            pstSavm->m_lErrno = NO_DATA_FOUND;
            IBPFree(pvData);
            return RC_FAIL;
        }
    }

    memcpy(pvData, pstTruck->m_pvData, lGetRowSize(pstSavm->tblName));
    if(!bIsNil)    vMergeTruck(pstSavm, pvData, pvUpdate);
    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pvUpdate, szIdx))
    {
        pthread_rwlock_unlock(prwLock);    
        pstSavm->m_lErrno = IDX_FIELD_NIL;
        IBPFree(pvData);
           return RC_FAIL;
    }

    pstSavm->pstVoid = pvUpdate;    // 设置新的数据
    SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
    if(!memcmp(szIdx, szOld, MAX_INDEX_LEN))
    {
        if(HAVE_NORL_IDX(t))
               lRet = __lUpdateGroup(pstSavm, pvAddr, t, pstTruck, lData);
        else if(HAVE_HASH_IDX(t))            
               lRet = __lUpdateHash(pstSavm, pvAddr, t, pstTruck, lData);
    }
    else
    {
        if(NULL != (SHTree *)pSearchTree(pvAddr, pstRoot, szIdx, lGetIdxLen(t)))
        {
            SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
            pthread_rwlock_unlock(prwLock);    
            pstSavm->m_lErrno = UNIQ_IDX_REPT;
            IBPFree(pvData);
            return RC_FAIL;
        }

        if(NULL == (pstRoot = _pDeleteTree(pvAddr, pstSavm->tblName, pstRoot, pstTree)))
        {
            SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
            pthread_rwlock_unlock(prwLock);    
            pstSavm->m_lErrno = SVR_EXCEPTION;
            IBPFree(pvData);
            return RC_FAIL;
        }
        
        ((TblDef *)pvAddr)->m_lValid --;
        if(NULL == (pstRoot = pInsertTree(pstSavm, pstRoot, szIdx, lGetIdxLen(t), &pstTruck)))
        {
            SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
            pthread_rwlock_unlock(prwLock);    
            IBPFree(pvData);
            return RC_FAIL;
        }

        ((TblDef *)pvAddr)->m_lValid ++;
        ((TblDef *)pvAddr)->m_lTreeRoot = pstRoot->m_lSePos;
        if(HAVE_NORL_IDX(t))
               lRet = __lUpdateGroup(pstSavm, pvAddr, t, pstTruck, lData);
        else if(HAVE_HASH_IDX(t))
               lRet = __lUpdateHash(pstSavm, pvAddr, t, pstTruck, lData);
    }

    SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
    if(RC_SUCC != lRet)
    {
        pthread_rwlock_unlock(prwLock);    
        IBPFree(pvData);
        return RC_FAIL;
    }

    memcpy(pstTruck->m_pvData, pvUpdate, lGetRowSize(t));
    pthread_rwlock_unlock(prwLock);    

    pstSavm->m_lEffect = 1;
    if(RC_SUCC != lRecordWork(pstSavm, pvData, OPERATE_UPDATE))    // 记录备份点
    {
        IBPFree(pvData);
        return RC_FAIL;
    }

    IBPFree(pvData);
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查找数据并更新查询索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        pvUpdate                   --新的数据
        bIsNil                     --是否全字段覆盖
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lUpdateGroup(SATvm *pstSavm, void *pvAddr, TABLE t, void *pvUpdate, const BOOL bIsNil)
{
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    size_t  lOffset, lNext;
    SHTree  *pstTree, *pstRoot;
    void    *pvData, *pvBack, *pvVoid = NULL;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);
    char    szOld[MAX_INDEX_LEN], szIdx[MAX_INDEX_LEN];

    memset(szOld, 0, sizeof(szOld));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szOld))
        return RC_CONTINUE;

    if(NULL == (pvData = (void *)malloc(lGetRowSize(t))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(NULL == (pvBack = (void *)malloc(lGetRowSize(t))))
    {
        IBPFree(pvData);
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        IBPFree(pvData);
        IBPFree(pvBack);
        return RC_FAIL;
    }

    if(NULL == (pstRoot = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        goto GROUP_ERROR;
    }

    if(NULL == (pstTree = pSearchTree(pvAddr, pstRoot, szOld, lGetGrpLen(t))))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        goto GROUP_ERROR;
    }

    pstSavm->m_lEffect = 0;
    pvVoid = pstSavm->pstVoid;
    pstSavm->m_lEType = EXE_PLAN_GRP;
    for(pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData); SELF_POS_UNUSE != pstList->m_lPos;
        pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        memcpy(pvData, pstTruck->m_pvData, lGetRowSize(t));
        if(!bTblIdxMatch(pstSavm, pvData, pvVoid, pstSavm->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        memcpy(pvBack, pvUpdate, lGetRowSize(t));
        if(!bIsNil)    vMergeTruck(pstSavm, pvData, pvBack);

        memset(szIdx, 0, sizeof(szIdx));
        if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pvBack, szIdx))
        {
            pstSavm->m_lErrno = IDX_FIELD_NIL;
            goto GROUP_ERROR;
        }

        pstSavm->m_lEffect ++;
        pstSavm->pstVoid = pvBack;    // 设置新的数据
        SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
        if(HAVE_UNIQ_IDX(t))
        {
            if(RC_SUCC != __lIndexUpdate(pstSavm, pvAddr, t, pstTruck, pstList->m_lData))
            {
                SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
                goto GROUP_ERROR;
            }
        }

        if(memcmp(szIdx, szOld, MAX_INDEX_LEN)) // 索引变动
        {
            -- ((TblDef *)pvAddr)->m_lValid;
            lOffset = lGetListOfs(t) + ((TblDef *)pvAddr)->m_lValid * sizeof(SHList);
            if(RC_SUCC != lReleaseList(pvAddr, t, pstTree, pstList, lOffset, &lNext))
            {
                ((TblDef *)pvAddr)->m_lValid ++;
                pstSavm->m_lErrno = SVR_EXCEPTION;
                SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
                goto GROUP_ERROR;
            }

            if(SELF_POS_UNUSE == pstTree->m_lData)
            {
                pstRoot = _pDeleteGroup(pvAddr, t, pstRoot, pstTree, &pstSavm->m_lEffect);
                ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;
            }

            if(NULL == (pstRoot = pInsertGroup(pstSavm, pstRoot, szIdx, lGetGrpLen(t), &pstTruck)))
            {
                SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
                goto GROUP_ERROR;
            }

            ((TblDef *)pvAddr)->m_lValid ++;
            ((TblDef *)pvAddr)->m_lGroupRoot = pstRoot->m_lSePos;
        }
        SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
        memcpy(pstTruck->m_pvData, pvBack, lGetRowSize(t));
        if(RC_SUCC != lRecordWork(pstSavm, pvData, OPERATE_UPDATE)) // 记录备份点
            goto GROUP_ERROR;

        if(FIRST_ROW & pstSavm->lReMatch)    break;
    }

    pthread_rwlock_unlock(prwLock);
    IBPFree(pvData);
    IBPFree(pvBack);
    if(0 == pstSavm->m_lEffect)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;

GROUP_ERROR:
    pthread_rwlock_unlock(prwLock);
    IBPFree(pvData);
       IBPFree(pvBack);
    return RC_FAIL;
}

/*************************************************************************************************
    函数说明：查找数据并更新hash索引
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        pvUpdate                   --新的数据
        bIsNil                     --是否全字段覆盖
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lUpdateHash(SATvm *pstSavm, void *pvAddr, TABLE t, void *pvUpdate, const BOOL bIsNil)
{
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    SHTree  *pstTree, *pstHash;
    size_t  lOld, lIdx, lOffset, lNext;
    void    *pvData = NULL, *pvBack, *pvVoid = NULL;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);
    char    szOld[MAX_INDEX_LEN], szIdx[MAX_INDEX_LEN];

    memset(szOld, 0, sizeof(szOld));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szOld))
        return RC_CONTINUE;

    if(NULL == (pvData = (void *)malloc(lGetRowSize(t))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(NULL == (pvBack = (void *)malloc(lGetRowSize(t))))
    {
        IBPFree(pvData);
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    lOld = uGetHash(szOld, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    lOffset = ((TblDef *)pvAddr)->m_lGroupPos + lOld * sizeof(SHTree);
    if(NULL == (pstHash = (SHTree *)(pvAddr + lOffset)))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        IBPFree(pvData);
           IBPFree(pvBack);
        return RC_FAIL;
    }

    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        IBPFree(pvData);
           IBPFree(pvBack);
           return RC_FAIL;
    }

    pstSavm->m_lEffect = 0;
    pvVoid = pstSavm->pstVoid;
    pstSavm->m_lEType = EXE_PLAN_GRP;
    for(pstList = (SHList *)pGetNode(pvAddr, pstHash->m_lData); SELF_POS_UNUSE != pstList->m_lPos; 
        pstList = (SHList *)pGetNode(pvAddr, lNext))
    {
        lNext = pstList->m_lNext;
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        memcpy(pvData, pstTruck->m_pvData, lGetRowSize(t));
        if(!bTblIdxMatch(pstSavm, pvData, pvVoid, pstSavm->lFind))
        {
            if(SELF_POS_UNUSE == pstList->m_lNext)    break;
            continue;
        }

        memcpy(pvBack, pvUpdate, lGetRowSize(t));
        if(!bIsNil)    vMergeTruck(pstSavm, pvData, pvBack);

        memset(szIdx, 0, sizeof(szIdx));
        if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pvBack, szIdx))
        {
            pstSavm->m_lErrno = IDX_FIELD_NIL;
            goto HASH_ERROR;
        }

        pstSavm->m_lEffect ++;
        pstSavm->pstVoid = pvBack;    // 设置新的数据
        lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
        SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
        if(HAVE_UNIQ_IDX(t))
        {
            if(RC_SUCC != __lIndexUpdate(pstSavm, pvAddr, t, pstTruck, pstList->m_lData))
            {
                SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
                goto HASH_ERROR;
            }
        }

        //    索引不匹配, 重建索引
        if(lOld != lIdx)
        {
            -- ((TblDef *)pvAddr)->m_lValid;
            lOffset = lGetListOfs(t) + ((TblDef *)pvAddr)->m_lValid * sizeof(SHList);
            if(RC_SUCC != lReleaseList(pvAddr, t, pstHash, pstList, lOffset, &lNext))
            {
                ((TblDef *)pvAddr)->m_lValid ++;
                pstSavm->m_lErrno = SVR_EXCEPTION;
                SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
                goto HASH_ERROR;
            }

            lOffset = ((TblDef *)pvAddr)->m_lGroupPos + lIdx * sizeof(SHTree);
            if(NULL == (pstTree = (SHTree *)(pvAddr + lOffset)))
            {
                pstSavm->m_lErrno = SVR_EXCEPTION;
                SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
                goto HASH_ERROR;
            }

            if(RC_SUCC != __lInsertHash(pstSavm, pvAddr, t, pstTree, lOffset, &pstTruck))
            {
                pstSavm->m_lErrno = SVR_EXCEPTION;
                SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
                goto HASH_ERROR;
            }

            ((TblDef *)pvAddr)->m_lValid ++;
        }
        SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
        memcpy(pstTruck->m_pvData, pvBack, lGetRowSize(t));
        if(RC_SUCC != lRecordWork(pstSavm, pvData, OPERATE_UPDATE)) // 记录备份点
            goto HASH_ERROR;

        if(FIRST_ROW & pstSavm->lReMatch)    break;
    }

    pthread_rwlock_unlock(prwLock);
    IBPFree(pvData);
    IBPFree(pvBack);
    if(0 == pstSavm->m_lEffect)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;

HASH_ERROR:
    pthread_rwlock_unlock(prwLock);
    IBPFree(pvData);
       IBPFree(pvBack);
       return RC_FAIL;
}

/*************************************************************************************************
    函数说明：查找数据并索引字段
    参数说明：
        pstSavm                    --操作句柄
        pvAddr                     --数据地址
        t                          --表名
        pvUpdate                   --新的数据
        bIsNil                     --是否全字段覆盖
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lTruckUpdate(SATvm *pstSavm, void *pvAddr, TABLE t, void *pvUpdate, const BOOL bIsNil)
{
    long    lRet = RC_SUCC;
    SHTruck *pstTruck = NULL;
    size_t  lRow, lOffset = lGetTblData(t);
    void    *pvData = NULL, *pvBack, *pvVoid = NULL;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pvAddr);

    if(NULL == (pvData = (void *)malloc(lGetRowSize(t))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(NULL == (pvBack = (void *)malloc(lGetRowSize(t))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(RC_SUCC != pthread_rwlock_wrlock(prwLock)) // 防止查询删除将数据修改
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        IBPFree(pvData);
        IBPFree(pvBack);
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 0;
    pvVoid = pstSavm->pstVoid;
    pstSavm->m_lEType = EXE_PLAN_ALL;
    pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
    for(lRow = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t));
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {
        if(IS_TRUCK_NULL(pstTruck))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        lRow ++;
        memcpy(pvData, pstTruck->m_pvData, lGetRowSize(t));
        if(pvVoid && !bTblIdxMatch(pstSavm, pvData, pvVoid, pstSavm->lFind))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        memcpy(pvBack, pvUpdate, lGetRowSize(t));
        if(!bIsNil)    vMergeTruck(pstSavm, pvData, pvBack);

        pstSavm->m_lEffect ++;
        pstSavm->pstVoid = pvBack;    // 设置新的数据
        SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NULL);
        if(HAVE_UNIQ_IDX(t))
        {
            if(RC_SUCC != __lIndexUpdate(pstSavm, pvAddr, t, pstTruck, lOffset))
            {
                SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
                goto TRUCK_ERROR;
            }
        }
 
        if(HAVE_NORL_IDX(t))
            lRet = __lUpdateGroup(pstSavm, pvAddr, t, pstTruck, lOffset);
        else if(HAVE_HASH_IDX(t))
            lRet = __lUpdateHash(pstSavm, pvAddr, t, pstTruck, lOffset);
        SET_DATA_TRUCK(pstTruck, DATA_TRUCK_NRML);
        if(RC_SUCC != lRet)
            goto TRUCK_ERROR;

        memcpy(pstTruck->m_pvData, pvBack, lGetRowSize(t));
        if(RC_SUCC != lRecordWork(pstSavm, pvData, OPERATE_UPDATE))    // 记录备份点
            goto TRUCK_ERROR;

        if(FIRST_ROW & pstSavm->lReMatch)    break;
        lOffset += lGetRowTruck(t);
    }

    pthread_rwlock_unlock(prwLock);
    IBPFree(pvData);
    IBPFree(pvBack);
    if(0 == pstSavm->m_lEffect)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;

TRUCK_ERROR:
    pthread_rwlock_unlock(prwLock);
    IBPFree(pvData);
    IBPFree(pvBack);
    return RC_FAIL;
}

/*************************************************************************************************
    函数说明：更新内存表数据
    参数说明：
        pstSavm                    --操作句柄
        pvUpdate                   --新的数据
        bIsNil                     --是否全字段覆盖
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lUpdate(SATvm *pstSavm, void *pvUpdate, const BOOL bIsNil)
{
    long    lRet;
    RunTime *pstRun = NULL;

    if(!pstSavm || !pvUpdate)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        return lUpdateByRt(pstSavm, pstRun, pstSavm->tblName, pvUpdate);

    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        lRet = _lUpdateIndex(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pvUpdate, bIsNil);
        if(RC_CONTINUE != lRet)
        {
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }

    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        lRet = _lUpdateGroup(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pvUpdate, bIsNil);
        if(RC_CONTINUE != lRet)
        {
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }
    else if(HAVE_HASH_IDX(pstSavm->tblName))
    {
        lRet = _lUpdateHash(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pvUpdate, bIsNil);
        if(RC_CONTINUE != lRet)
        {
            vTblDisconnect(pstSavm, pstSavm->tblName);
            return lRet;
        }
    }

    lRet = _lTruckUpdate(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, pvUpdate, bIsNil);
    vTblDisconnect(pstSavm, pstSavm->tblName);
    return lRet;
}

/*************************************************************************************************
    函数说明：注册表和表字段
    参数说明：
        pstSavm                    --操作句柄
        pstRun                     --表句柄
        t                          --表名
        lType                      --注册表类型
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lRegisterTable(SATvm *pstSavm, RunTime *pstRun, TABLE t, long lType)
{
    TIndex  stIndex;
    TBoot   *pstBoot = (TBoot *)pBootInitial();

    if(TYPE_CLIENT != lType)    return RC_SUCC;

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_INDEX)))
        return RC_FAIL;

    if(RC_FAIL == (pstSavm->m_ySey = yGetIPCPath(pstSavm, IPC_SEM)))
          return RC_FAIL;

    pstRun->m_semID = semget(pstSavm->m_ySey, pstBoot->m_lMaxTable, IPC_CREAT|0660);

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_lValid   = 0;
    stIndex.m_yKey     = 0;
    stIndex.m_table    = t;
    stIndex.m_lType    = lType;
    stIndex.m_lMaxRows = lGetTblRow(t);
    stIndex.m_lRowSize = lGetRowSize(t);
    stIndex.m_shmID    = pstRun->m_shmID;
    stIndex.m_semID    = pstRun->m_semID;
    stIndex.m_lLocal   = RES_LOCAL_SID;
    stIndex.m_lPers    = lGetPermit(t);

    strncpy(stIndex.m_szOwner, pstSavm->m_szNode, sizeof(stIndex.m_szOwner));
    strncpy(stIndex.m_szTime, sGetUpdTime(), sizeof(stIndex.m_szTime));
    strncpy(stIndex.m_szTable, sGetTableName(t), sizeof(stIndex.m_szTable));
    strncpy(stIndex.m_szPart, sGetTablePart(t, pstSavm->m_szNode), sizeof(stIndex.m_szPart));

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;
    if(RC_SUCC != lInsert(pstSavm))
    {
        if(UNIQ_IDX_REPT == pstSavm->m_lErrno)
            pstSavm->m_lErrno = TBL_ARD_EXIST;
        return RC_FAIL;
    }

    if(TVM_BOOT_SIMPLE != pstBoot->m_lBootType)
        lRefreshNotify(pstSavm, pstBoot->m_lDomPort);

    return lInsertField(pstSavm, t);
}

/*************************************************************************************************
    函数说明：定义客户表
    参数说明：
        pstSavm                    --操作句柄
        t                          --表序号
        lRow                       --表空间大小
        bCreate                    --创建方式
        lType                      --表类型
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lCustomTable(SATvm *pstSavm, TABLE t, size_t lRow, BOOL bCreate, long lType)
{
    RWAttr    attr;
    RunTime *pstRun = NULL;
    RWLock    *prwLock = NULL;

    //    表记录大小
    pstSavm->tblName = t;
    ((TblDef *)pGetTblDef(t))->m_lTable = lInitialTable(t, lRow);
    if(NULL == (pstRun = (RunTime *)pCreateBlock(pstSavm, t, ((TblDef *)pGetTblDef(t))->m_lTable, 
        bCreate)))
        return RC_FAIL;

    //    内存创建完毕，将初始化索引信息
    if(RC_SUCC != lInitailTree(pstSavm, (void *)pGetNode(pstRun->m_pvAddr, lGetIdxPos(t)), t))
        return RC_FAIL;

    if(RC_SUCC != lInitailGroup(pstSavm, (void *)pGetNode(pstRun->m_pvAddr, lGetGrpPos(t)), t))
        return RC_FAIL;

    memcpy(pstRun->m_pvAddr, (void *)pGetTblDef(t), sizeof(TblDef));
    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_rwlock_init(prwLock, &attr);

    memset(pstRun->m_pvAddr + lGetTblData(t), 0, lGetTableSize(t) - lGetTblData(t));
    vTblDisconnect(pstSavm, t);

    if(RC_SUCC != lRegisterTable(pstSavm, pstRun, t, lType))
    {
        shmctl(pstRun->m_shmID, IPC_RMID, NULL);
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：创建表
    参数说明：
        pstSavm                    --操作句柄
        t                          --表序号
        lRow                       --表空间大小
        bCreate                    --创建方式
        lType                      --表类型
        pfCreateFunc               --创建表函数
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lCreateTable(SATvm *pstSavm, TABLE t, size_t lRow, BOOL bCreate, long lType, 
            TCREATE pfCreateFunc)
{
    long    lRet;

    if(!pstSavm || lRow <= 0)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    vInitTblDef(t);
    pstSavm->tblName = t;
    switch(t)
    {
    case SYS_TVM_INDEX:
        lRet = lCreateTvmIndex();
        break;
    case SYS_TVM_FIELD:
        lRet = lCreateTvmField();
        break;
    case SYS_TVM_DOMAIN:
        lRet = lCreateTvmDomain();
        break;
    case SYS_TVM_SEQUE:
        lRet = lCreateTvmSeque();
        break;
    default:
        if(!pfCreateFunc)
            return RC_SUCC;

        lRet = pfCreateFunc(t);
        break;
    }    
    if(RC_SUCC != lRet)        return lRet;

    return _lCustomTable(pstSavm, t, lRow, bCreate, lType);
}

/*************************************************************************************************
    函数说明：创建序列
    参数说明：
        pstSavm                    --操作句柄
        pszSQName                  --序列名称
        uIncre                     --每次增长
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lCreateSeque(SATvm *pstSavm, char *pszSQName, uint uIncre)
{
    TSeque    stSeque;

    memset(&stSeque, 0, sizeof(stSeque));
    strncpy(stSeque.m_szSQName, pszSQName, sizeof(stSeque.m_szSQName));
    stSeque.m_uIncrement = uIncre > 0 ? uIncre : 1;

    if(RC_SUCC != lInitSATvm(pstSavm, SYS_TVM_SEQUE))
        return RC_FAIL;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(TSeque);
    pstSavm->pstVoid = (void *)&stSeque;

    return lInsert(pstSavm);
}

/*************************************************************************************************
    函数说明：设置序列的起始值
    参数说明：
        pstSavm                    --操作句柄
        pszSQName                  --序列名称
        uStart                     --增长起始值
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lSetSequence(SATvm *pstSavm, char *pszSQName, uint uStart)
{
    TSeque  stSeque;
    SHTree  *pstTree = NULL;
    RunTime *pstRun  = NULL;
    RWLock  *prwLock = NULL;
    SHTruck *pstTruck = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(&stSeque, 0, sizeof(stSeque));
    strncpy(stSeque.m_szSQName, pszSQName, sizeof(stSeque.m_szSQName));
    if(RC_SUCC != lInitSATvm(pstSavm, SYS_TVM_SEQUE))
        return RC_FAIL;

    pstSavm->lSize   = sizeof(TSeque);
    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, SYS_TVM_SEQUE)))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        vTblDisconnect(pstSavm, SYS_TVM_SEQUE);
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pGetNode(pstRun->m_pvAddr, 
        ((TblDef *)pstRun->m_pvAddr)->m_lTreeRoot)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        pthread_rwlock_unlock(prwLock);
        vTblDisconnect(pstSavm, SYS_TVM_SEQUE);
        return RC_FAIL;
    }

    pstTree = (SHTree *)pSearchTree(pstRun->m_pvAddr, pstTree, stSeque.m_szSQName, 
        lGetIdxLen(SYS_TVM_SEQUE));
    if(!pstTree)
    {
        pthread_rwlock_unlock(prwLock);
        vTblDisconnect(pstSavm, SYS_TVM_SEQUE);
        pstSavm->m_lErrno = SEQ_NOT_FOUND;
        return RC_FAIL;
    }

    pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, pstTree->m_lData);
    pstTruck->m_lTimes = uStart;
       pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, SYS_TVM_SEQUE);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取序列数据
    参数说明：
        pstSavm                    --操作句柄
        pszSQName                  --序列名称
        pulNumber                  --返回序列值
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lSelectSeque(SATvm *pstSavm, char *pszSQName, ulong *pulNumber)
{
    TSeque  stSeque;
    SHTree  *pstTree = NULL;
    RunTime *pstRun  = NULL;
    RWLock  *prwLock = NULL;
    SHTruck *pstTruck = NULL;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(&stSeque, 0, sizeof(stSeque));
    strncpy(stSeque.m_szSQName, pszSQName, sizeof(stSeque.m_szSQName));
    if(RC_SUCC != lInitSATvm(pstSavm, SYS_TVM_SEQUE))
        return RC_FAIL;

    pstSavm->lSize   = sizeof(TSeque);
    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, SYS_TVM_SEQUE)))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        vTblDisconnect(pstSavm, SYS_TVM_SEQUE);
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pGetNode(pstRun->m_pvAddr, 
        ((TblDef *)pstRun->m_pvAddr)->m_lTreeRoot)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        pthread_rwlock_unlock(prwLock);
        vTblDisconnect(pstSavm, SYS_TVM_SEQUE);
        return RC_FAIL;
    }

    pstTree = (SHTree *)pSearchTree(pstRun->m_pvAddr, pstTree, stSeque.m_szSQName, 
        lGetIdxLen(SYS_TVM_SEQUE));
    if(!pstTree)
    {
        pthread_rwlock_unlock(prwLock);
        vTblDisconnect(pstSavm, SYS_TVM_SEQUE);
        pstSavm->m_lErrno = SEQ_NOT_FOUND;
        return RC_FAIL;
    }

    pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, pstTree->m_lData);
    pstTruck->m_lTimes += ((TSeque *)pstTruck->m_pvData)->m_uIncrement;
    *pulNumber = pstTruck->m_lTimes;
       pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, SYS_TVM_SEQUE);
    pstSavm->m_lEffect = 1;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：停止系统
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lShutdownTvm()
{
    size_t  lOut = 0;
    TIndex  *pstIndex = NULL;
    RunTime *pstRun = NULL;
    RWLock  *prwLock = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lExportTable(SYS_TVM_INDEX, &lOut, (void *)&pstIndex))
        return RC_FAIL;

    if(0 == lOut)    return RC_SUCC;

    // 从后面向前删除，根索引表最后删除
    for(lOut; 0 < lOut; -- lOut)
    {
        if(RES_REMOT_SID == pstIndex[lOut - 1].m_lLocal)
            continue;

        vForceDisconnect(pstSavm, pstIndex[lOut - 1].m_table);
        pstRun = (RunTime *)pGetRunTime(pstSavm, pstIndex[lOut - 1].m_table);
        pstRun->m_shmID = pstIndex[lOut - 1].m_shmID;
        if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, pstIndex[lOut - 1].m_table)))
        {
            IBPFree(pstIndex);
            return RC_FAIL;
        }

        prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
        pthread_rwlock_destroy(prwLock);
        vForceDisconnect(pstSavm, pstIndex[lOut - 1].m_table);

        /* 无论成功与否，该表都删除 */
        shmctl(pstIndex[lOut - 1].m_shmID, IPC_RMID, NULL);
    }
    semctl(pstIndex[0].m_semID, 0, IPC_RMID, NULL);
    IBPFree(pstIndex);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：从配置文件获取参数
    参数说明：
    返回值：
        PORT
 *************************************************************************************************/
long    lGetLDPort()
{
    TBoot    stBoot;
    FILE    *fp = NULL;
    char    szPath[512], szVersion[10];

    memset(szPath, 0, sizeof(szPath));
    memset(szVersion, 0, sizeof(szVersion));
       snprintf(szPath, sizeof(szPath), "%s", getenv("TVMCFG"));
    if(NULL == (fp = fopen(szPath, "rb")))
        return TVM_PORT_DOMAIN;

    fread(szVersion, 4, 1, fp);
    if(memcmp(szVersion, TVM_RUNCFG_TAG, 4))
    {
        fclose(fp);
        return TVM_PORT_DOMAIN;
    }

    fread(&stBoot, sizeof(TBoot), 1, fp);
    fclose(fp);

    return stBoot.m_lDomPort;
}

/*************************************************************************************************
    函数说明：从配置文件中加载启动参数
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lGetBootConfig(SATvm *pstSavm, TBoot *pstBoot)       
{
    FILE    *fp = NULL;
    char    szPath[512], szVersion[10];

    if(!pstSavm || !pstBoot)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(szPath, 0, sizeof(szPath));
    memset(szVersion, 0, sizeof(szVersion));
    snprintf(szPath, sizeof(szPath), "%s", getenv("TVMCFG"));
    if(NULL == (fp = fopen(szPath, "rb")))
    {
        pstSavm->m_lErrno = FILE_NOTFOUND;
        return RC_FAIL;
    }

    fread(szVersion, 4, 1, fp);
    if(memcmp(szVersion, TVM_RUNCFG_TAG, 4))
    {
        fclose(fp);
        pstSavm->m_lErrno = BOOT_VER_ICMP;
        return RC_FAIL;
    }

    fread(pstBoot, sizeof(TBoot), 1, fp);
    fclose(fp);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：启动初始化
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
void*    pBootInitial()
{
    static    int        i = 0;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(1 == i ++)    return &g_stBoot;

    memset(&g_stBoot, 0, sizeof(TBoot));
    pstSavm->m_lErrno = TVM_DONE_SUCC;
    lGetBootConfig(pstSavm, &g_stBoot);
    return &g_stBoot;
}

/*************************************************************************************************
    函数说明：从配置文件中加载启动参数INDEX
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lGetLocalIndex(SATvm *pstSavm, long *plOut, TIndex **ppstIndex)
{
    FILE    *fp = NULL;
    char    szPath[512], szVersion[10];

    if(!pstSavm || !ppstIndex || !plOut)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(szPath, 0, sizeof(szPath));
    memset(szVersion, 0, sizeof(szVersion));
    snprintf(szPath, sizeof(szPath), "%s", getenv("TVMCFG"));
    if(NULL == (fp = fopen(szPath, "rb")))
    {
        pstSavm->m_lErrno = FILE_NOTFOUND;
        return RC_FAIL;
    }

    fread(szVersion, 4, 1, fp);
    if(memcmp(szVersion, TVM_RUNCFG_TAG, 4))
    {
        fclose(fp);
        pstSavm->m_lErrno = BOOT_VER_ICMP;
        return RC_FAIL;
    }

    fseek(fp, sizeof(TBoot), SEEK_CUR);
    fread((void *)plOut, sizeof(long), 1, fp);
    if(*plOut <= 0)
    {
        fclose(fp);
        return RC_SUCC;
    }

    if(NULL == (*ppstIndex = (void *)calloc(*plOut, sizeof(TIndex))))
    {
        fclose(fp);
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    fread(*ppstIndex, (*plOut) * sizeof(TIndex), 1, fp);
    fclose(fp);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：从配置文件中加载启动参数DOMAIN
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lGetDomainIndex(SATvm *pstSavm, long *plOut, TIndex **ppstIndex)
{
    FILE    *fp = NULL;
    char    szPath[512], szVersion[10];

    if(!pstSavm || !ppstIndex || !plOut)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(szPath, 0, sizeof(szPath));
    memset(szVersion, 0, sizeof(szVersion));
    snprintf(szPath, sizeof(szPath), "%s", getenv("TVMCFG"));
    if(NULL == (fp = fopen(szPath, "rb")))
    {
        pstSavm->m_lErrno = FILE_NOTFOUND;
        return RC_FAIL;
    }

    fread(szVersion, 4, 1, fp);
    if(memcmp(szVersion, TVM_RUNCFG_TAG, 4))
    {
        fclose(fp);
        pstSavm->m_lErrno = BOOT_VER_ICMP;
        return RC_FAIL;
    }

    fseek(fp, sizeof(TBoot), SEEK_CUR);
    fread((void *)plOut, sizeof(long), 1, fp);
    if(*plOut <= 0)
    {
        fclose(fp);
        return RC_SUCC;
    }

    fseek(fp, (*plOut) * sizeof(TIndex), SEEK_CUR);
    fread((void *)plOut, sizeof(long), 1, fp);
    if(NULL == (*ppstIndex = (void *)calloc(*plOut, sizeof(TIndex))))
    {
        fclose(fp);
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    fread(*ppstIndex, (*plOut) * sizeof(TIndex), 1, fp);
    fclose(fp);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：从配置文件中加载启动参数DOMAIN
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lGetDomainTable(SATvm *pstSavm, long *plOut, TDomain **ppstDomain)
{
    FILE    *fp = NULL;
    char    szPath[512], szVersion[10];

    if(!pstSavm || !ppstDomain || !plOut)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(szPath, 0, sizeof(szPath));
    memset(szVersion, 0, sizeof(szVersion));
    snprintf(szPath, sizeof(szPath), "%s", getenv("TVMCFG"));
    if(NULL == (fp = fopen(szPath, "rb")))
    {
        pstSavm->m_lErrno = FILE_NOTFOUND;
        return RC_FAIL;
    }

    fread(szVersion, 4, 1, fp);
    if(memcmp(szVersion, TVM_RUNCFG_TAG, 4))
    {
        fclose(fp);
        pstSavm->m_lErrno = BOOT_VER_ICMP;
        return RC_FAIL;
    }

    fseek(fp, sizeof(TBoot), SEEK_CUR);
    fread((void *)plOut, sizeof(long), 1, fp);
    fseek(fp, (*plOut) * sizeof(TIndex), SEEK_CUR);
    fread((void *)plOut, sizeof(long), 1, fp);
    fseek(fp, (*plOut) * sizeof(TIndex), SEEK_CUR);
    fread((void *)plOut, sizeof(long), 1, fp);
    if(*plOut <= 0)
    {
        fclose(fp);
        return RC_SUCC;
    }

    if(NULL == (*ppstDomain = (void *)calloc(*plOut, sizeof(TDomain))))
    {
        fclose(fp);
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    fread(*ppstDomain, (*plOut) * sizeof(TDomain), 1, fp);
    fclose(fp);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：初始化远程表本地映射
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lInitDomain(SATvm *pstSavm)
{
    long    i, lCount = 0;
    TDomain *pstDomain = NULL;
    TIndex  stIndex, *pstIndex = NULL;

/*
    memset(&stIndex, 0, sizeof(TIndex));
    if(RC_SUCC != lGetLocalIndex(pstSavm, &lCount, (void *)&pstIndex))
        return RC_FAIL;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
       pstSavm->pstVoid = (void *)&stIndex;

    for(i = 0; i < lCount; i ++)
    {
        stIndex.m_table = pstIndex[i].m_table;
        if(RC_SUCC != lUpdate(pstSavm, (void *)&pstIndex[i]))
        {
            IBPFree(pstIndex);
            return RC_FAIL;
        }
    }
    lCount = 0;
    IBPFree(pstIndex);
*/

    if(RC_SUCC != lGetDomainIndex(pstSavm, &lCount, &pstIndex))
        return RC_FAIL;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    for(i = 0; i < lCount; i ++)
    {
        pstIndex[i].m_lValid   = 0;
        pstIndex[i].m_lType    = TYPE_CLIENT;
        pstIndex[i].m_lLocal   = RES_REMOT_SID;
        pstIndex[i].m_lState   = RESOURCE_STOP;     // 不允许外部系统访问
        pstIndex[i].m_lPers    = OPERATE_NULL;      // 对外操作权限NULL
        strncpy(pstIndex[i].m_szTime, sGetUpdTime(), sizeof(pstIndex[i].m_szTime));

        pstSavm->pstVoid = (void *)&pstIndex[i];
        if(RC_SUCC != lInsert(pstSavm))
        {
            IBPFree(pstIndex);
            return RC_FAIL;
        }
    }
    lCount = 0;
    IBPFree(pstIndex);

    if(RC_SUCC != lGetDomainTable(pstSavm, &lCount, &pstDomain))
        return RC_FAIL;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_DOMAIN;
    for(i = 0; i < lCount; i ++)
    {
        pstSavm->lSize   = sizeof(TDomain);
        pstSavm->pstVoid = (void *)&pstDomain[i];
        if(RC_SUCC != lInsert(pstSavm))
        {
            IBPFree(pstDomain);
            return RC_FAIL;
        }
    }
    lCount = 0;
    IBPFree(pstDomain);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取表权限
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lGetPermit(TABLE t)
{
    TIndex  *pstIndex = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();
    long    i, lCount = 0, lPers = OPERATE_DEFAULT;

    if(RC_SUCC != lGetLocalIndex(pstSavm, &lCount, &pstIndex))
        return lPers;

    for(i = 0; i < lCount; i ++)
    {
        if(t == pstIndex[i].m_table)
        {
            lPers = pstIndex[i].m_lPers;
            break;    
        }
    }
    IBPFree(pstIndex);

    return lPers;
}

/*************************************************************************************************
    函数说明：解析配置文件
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lParseConfig(SATvm *pstSavm, CMList **ppstRoot, const char *pszTarget)
{
    FILE    *fp = NULL;
    BOOL    bFlag = FALSE;
    CMList  *pstList = NULL;
    char    szLine[4098], szPath[512];

    memset(szPath, 0, sizeof(szPath));
    snprintf(szPath, sizeof(szPath), "%s", getenv("TVMCFG"));
    if(NULL == (fp = fopen(szPath, "rb")))
    {
        pstSavm->m_lErrno = FILE_NOTFOUND;
        return RC_FAIL;
    }

    memset(szLine, 0, sizeof(szLine));
    while(fgets(szLine, sizeof(szLine), fp))
    {
        strimcrlf(szLine);
        sltrim(szLine);
        srtrim(szLine);
        if(!strlen(szLine))
            continue;

        if('#' == szLine[0] || !memcmp("//", szLine, 2) || !memcmp("/*", szLine, 2) ||
            !memcmp("＃", szLine, 2) || !memcmp("--", szLine, 2))
            continue;

        if(!strcmp(pszTarget, szLine) && !bFlag)
        {
            bFlag = TRUE;
            memset(szLine, 0, sizeof(szLine));
            continue;
        }
        else if(szLine[0] == '*' && bFlag)
            break;

        if(!bFlag)
        {
            memset(szLine, 0, sizeof(szLine));
            continue;
        }

        if(NULL == (pstList = pInsertList(pstList, (void *)szLine, sizeof(szLine))))
        {
            fclose(fp);
            vDestroyList(pstList);
            return RC_FAIL;
        }
    }

    fclose(fp);
    *ppstRoot = pstList;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：启动系统根节点
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lStartupTvm(TBoot *pstBoot)
{
    long      semID;
    TIndex    stIndex;
    RunTime   *pstRun = NULL;
    SATvm     *pstSavm = (SATvm *)pGetSATvm();

    memset(&stIndex, 0, sizeof(TIndex));
    if(RC_SUCC != _lCreateTable(pstSavm, SYS_TVM_INDEX, pstBoot->m_lMaxTable, TRUE, 
        TYPE_SYSTEM, NULL))
        return RC_FAIL;

    pstRun = (RunTime *)pGetRunTime(pstSavm, SYS_TVM_INDEX);
    if(RC_SUCC != lCreateSems(pstSavm, pstRun, pstBoot->m_lMaxTable, SEM_INIT))
        return RC_FAIL;

    semID = pstRun->m_semID;
    stIndex.m_lValid   = 0;
    stIndex.m_semID    = semID;
    stIndex.m_lType    = TYPE_SYSTEM;
    stIndex.m_table    = SYS_TVM_INDEX;
    stIndex.m_yKey     = pstSavm->m_yKey;
    stIndex.m_shmID    = pstRun->m_shmID;
    stIndex.m_lMaxRows = pstBoot->m_lMaxTable;
    stIndex.m_lLocal   = RES_LOCAL_SID;
    stIndex.m_lState   = RESOURCE_STOP;        // 不允许外部系统访问
    stIndex.m_lPers    = OPERATE_NULL;        // 对外操作权限NULL
    stIndex.m_lRowSize = lGetRowSize(SYS_TVM_INDEX);
    strncpy(stIndex.m_szTime, sGetUpdTime(), sizeof(stIndex.m_szTime));
    strncpy(stIndex.m_szOwner, pstBoot->m_szNode, sizeof(stIndex.m_szOwner));
    strncpy(stIndex.m_szPart, pstBoot->m_szNode, sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sGetTableName(SYS_TVM_INDEX), sizeof(stIndex.m_szTable));
    
    /*    该表在创建的时候已初始化
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_INDEX)))
        return RC_FAIL;
    */

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;
    if(RC_SUCC != lInsert(pstSavm))    return RC_FAIL;

    memset(&stIndex, 0, sizeof(TIndex));
    if(RC_SUCC != _lCreateTable(pstSavm, SYS_TVM_FIELD, pstBoot->m_lMaxField, FALSE, 
        TYPE_INCORE, NULL))
        return RC_FAIL;

    pstRun = (RunTime *)pGetRunTime(pstSavm, SYS_TVM_FIELD);
    stIndex.m_lValid   = 0;
    stIndex.m_semID    = semID;
    stIndex.m_lType    = TYPE_INCORE;
    stIndex.m_lLocal   = RES_LOCAL_SID;
    stIndex.m_table    = SYS_TVM_FIELD;
    stIndex.m_yKey     = pstSavm->m_yKey;
    stIndex.m_shmID    = pstRun->m_shmID;
    stIndex.m_lState   = RESOURCE_STOP;        // 不允许外部系统访问
    stIndex.m_lPers    = OPERATE_NULL;        // 对外操作权限NULL
    stIndex.m_lRowSize = lGetRowSize(SYS_TVM_FIELD);
    strncpy(stIndex.m_szTime, sGetUpdTime(), sizeof(stIndex.m_szTime));
    strncpy(stIndex.m_szOwner, pstBoot->m_szNode, sizeof(stIndex.m_szOwner));
    strncpy(stIndex.m_szPart, pstBoot->m_szNode, sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sGetTableName(SYS_TVM_FIELD), sizeof(stIndex.m_szTable));
    
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;

    if(RC_SUCC != lInsert(pstSavm))    return RC_FAIL;

    if(RC_SUCC != lInsertField(pstSavm, SYS_TVM_FIELD))
        return RC_FAIL;

    memset(&stIndex, 0, sizeof(TIndex));
    if(RC_SUCC != _lCreateTable(pstSavm, SYS_TVM_DOMAIN, pstBoot->m_lMaxDomain, FALSE, 
        TYPE_INCORE, NULL))
        return RC_FAIL;

    pstRun = (RunTime *)pGetRunTime(pstSavm, SYS_TVM_DOMAIN);
    stIndex.m_lValid   = 0;
    stIndex.m_semID    = semID;
    stIndex.m_lType    = TYPE_INCORE;
    stIndex.m_lLocal   = RES_LOCAL_SID;
    stIndex.m_table    = SYS_TVM_DOMAIN;
    stIndex.m_yKey     = pstSavm->m_yKey;
    stIndex.m_shmID    = pstRun->m_shmID;
    stIndex.m_lState   = RESOURCE_STOP;        // 不允许外部系统访问
    stIndex.m_lPers    = OPERATE_NULL;        // 对外操作权限NULL
    stIndex.m_lRowSize = lGetRowSize(SYS_TVM_DOMAIN);
    strncpy(stIndex.m_szTime, sGetUpdTime(), sizeof(stIndex.m_szTime));
    strncpy(stIndex.m_szOwner, pstBoot->m_szNode, sizeof(stIndex.m_szOwner));
    strncpy(stIndex.m_szPart, pstBoot->m_szNode, sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sGetTableName(SYS_TVM_DOMAIN), sizeof(stIndex.m_szTable));
    
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;

    if(RC_SUCC != lInsert(pstSavm))    return RC_FAIL;

    if(RC_SUCC != lInsertField(pstSavm, SYS_TVM_DOMAIN))
        return RC_FAIL;

    memset(&stIndex, 0, sizeof(TIndex));
    if(RC_SUCC != _lCreateTable(pstSavm, SYS_TVM_SEQUE, pstBoot->m_lMaxSeque, FALSE, 
        TYPE_INCORE, NULL))
        return RC_FAIL;

    pstRun = (RunTime *)pGetRunTime(pstSavm, SYS_TVM_SEQUE);
    stIndex.m_lValid   = 0;
    stIndex.m_semID    = semID;
    stIndex.m_lType    = TYPE_INCORE;
    stIndex.m_lLocal   = RES_LOCAL_SID;
    stIndex.m_table    = SYS_TVM_SEQUE;
    stIndex.m_yKey     = pstSavm->m_yKey;
    stIndex.m_shmID    = pstRun->m_shmID;
    stIndex.m_lState   = RESOURCE_STOP;        // 不允许外部系统访问
    stIndex.m_lPers    = OPERATE_NULL;        // 对外操作权限NULL
    stIndex.m_lRowSize = lGetRowSize(SYS_TVM_SEQUE);
    strncpy(stIndex.m_szTime, sGetUpdTime(), sizeof(stIndex.m_szTime));
    strncpy(stIndex.m_szOwner, pstBoot->m_szNode, sizeof(stIndex.m_szOwner));
    strncpy(stIndex.m_szPart, pstBoot->m_szNode, sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sGetTableName(SYS_TVM_SEQUE), sizeof(stIndex.m_szTable));
    
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;

    if(RC_SUCC != lInsert(pstSavm))    return RC_FAIL;

    if(RC_SUCC != lInsertField(pstSavm, SYS_TVM_SEQUE))
        return RC_FAIL;

    return lInitDomain(pstSavm);
}

/*************************************************************************************************
    函数说明：创建表
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lCreateTable(SATvm *pstSavm, TABLE t, size_t lRow, TCREATE pfCreateFunc)
{
    return _lCreateTable(pstSavm, t, lRow, FALSE, TYPE_CLIENT, pfCreateFunc);
}

/*************************************************************************************************
    函数说明：自定义模板创建
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lCustomTable(SATvm *pstSavm, TABLE t, size_t lRow, TblDef *pstDef)
{

    memcpy((void *)pGetTblDef(t), (void *)pstDef, sizeof(TblDef));    

    return _lCustomTable(pstSavm, t, lRow, FALSE, TYPE_CLIENT);
}

/*************************************************************************************************
    函数说明：删除表
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lDropTable(SATvm *pstSavm, TABLE t)
{
    TIndex    stIndex;
    TField    stField;
    RunTime   *pstRun = NULL;
    RWLock    *prwLock = NULL;

    memset(&stField, 0, sizeof(TField));
    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_table = t;
    stIndex.m_lType = TYPE_CLIENT;

    pstSavm->pstVoid = &stIndex;
    pstSavm->lFind   = CHK_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);

    if(RC_SUCC != lSelect(pstSavm, (void *)&stIndex))    return RC_FAIL;

    vForceDisconnect(pstSavm, t);
    pstRun = (RunTime *)pGetRunTime(pstSavm, t);
    pstRun->m_shmID = stIndex.m_shmID;
    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, t)))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    pthread_rwlock_destroy(prwLock);
    vForceDisconnect(pstSavm, t);

    //    无论成功与否，该表都删除
    shmctl(stIndex.m_shmID, IPC_RMID, NULL);
    semctl(stIndex.m_semID, 0, IPC_RMID, 0);

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_table = t;
    stIndex.m_lType = TYPE_CLIENT;
    pstSavm->pstVoid = &stIndex;
    pstSavm->lSize   = sizeof(TIndex);

    if(RC_SUCC != lDelete(pstSavm))    return RC_FAIL;

    //    删除字段表数据
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_FIELD)))
        return RC_FAIL;

    stField.m_table  = t;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_FIELD;
    pstSavm->lSize   = sizeof(TField);
    pstSavm->pstVoid = (void *)&stField;
    if(RC_SUCC != lDelete(pstSavm))    return RC_FAIL;

    pstSavm->bSearch = TYPE_CLIENT;
    memset(pstRun, 0, sizeof(RunTime));
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：将数据内容格式化导出
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lExportContext(FILE *fp, char *s, long lIdx, TblKey *pstIdx, char *f)
{
    long    i = 0;

    if(!fp || !s || !pstIdx)    return RC_FAIL;

    for(i = 0; i < lIdx; i ++)    //  下面比较的都是唯一索引
    {
        switch(pstIdx[i].m_lAttr)
        {
        case FIELD_DOUBLE:
            switch(pstIdx[i].m_lLen)
            {
            case    4:
                fprintf(fp, "%.6f", *((float *)(s + pstIdx[i].m_lFrom)));
                break;
            case    8:
                fprintf(fp, "%.6f", *((double *)(s + pstIdx[i].m_lFrom)));
                break;
            default:
                break;
            }
            break;
        case FIELD_LONG:
            switch(pstIdx[i].m_lLen)
            {
            case    2:
                fprintf(fp, "%d", *((sint *)(s + pstIdx[i].m_lFrom)));
                break;
            case    4:
                fprintf(fp, "%d", *((int *)(s + pstIdx[i].m_lFrom)));
                break;
            case    8:
                fprintf(fp, "%ld", *((llong *)(s + pstIdx[i].m_lFrom)));
                break;
            default:
                break;
            }
            break;
        case FIELD_CHAR:
//            fwrite(s + pstIdx[i].m_lFrom, 1, pstIdx[i].m_lLen, fp);
            fprintf(fp, "%s", s + pstIdx[i].m_lFrom);
            break;
        default:
            fprintf(stderr, "导出字段格式异常\n");
            break;
        }
        fprintf(fp, "%s", f);
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：将数据内容格式化导入
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lImportContext(char *s, long lIdx, TblKey *pstIdx, char *pvOut, char *f)
{
    long    i = 0;
    void    *p = NULL;

    for(i = 0; i < lIdx; i ++)    //  下面比较的都是唯一索引
    {
        p = sgetvalue(s, f, i + 1);

        switch(pstIdx[i].m_lAttr)
        {
        case FIELD_DOUBLE:
            switch(pstIdx[i].m_lLen)
            {
            case    4:
                *((float *)(pvOut + pstIdx[i].m_lFrom)) = atof(p);
                break;
            case    8:
                *((double *)(pvOut + pstIdx[i].m_lFrom)) = atof(p);
                break;
            default:
                break;
            }
            break;
        case FIELD_LONG:
            switch(pstIdx[i].m_lLen)
            {
            case    2:
                *((sint *)(pvOut + pstIdx[i].m_lFrom)) = atoi(p);
                break;
            case    4:
                *((int *)(pvOut + pstIdx[i].m_lFrom)) = atoi(p);
                break;
            case    8:
                *((llong *)(pvOut + pstIdx[i].m_lFrom)) = atol(p);
                break;
            default:
                break;
            }
            break;
        case FIELD_CHAR:
            memcpy(pvOut + pstIdx[i].m_lFrom, p, pstIdx[i].m_lLen);
            break;
        default:
            break;
        }
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：从文件中导入数据到内存表中
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lImportFile(TABLE t, char *pszFile, char *pszFlag)
{
    FILE    *fp = NULL;
    char    szLine[1024];
    RunTime *pstRun = NULL;    
    void    *pvData = NULL;
    RWLock  *prwLock = NULL;
    long    lEffect = 0, lRet = RC_SUCC;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(!pszFile || !pszFlag || !strlen(pszFlag))
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(szLine, 0, sizeof(szLine));
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(t)))
        return RC_FAIL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(NULL == (fp = fopen(pszFile, "rb")))
    {
        pstSavm->m_lErrno = FIL_NOT_EXIST;
        return RC_FAIL;
    }

    if(NULL == (pvData = (void *)malloc(lGetRowSize(t))))
    {
        fclose(fp);
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    pstSavm->pstVoid = pvData;
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    pstSavm->lSize   = lGetRowSize(t);
    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        fclose(fp);
        IBPFree(pvData);
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        return RC_FAIL;
    }

    while(fgets(szLine, sizeof(szLine), fp))
    {
        strimcrlf(szLine);
        if(!strlen(szLine))
            continue;

        if(lGetTblRow(t) <= ((TblDef *)pstRun->m_pvAddr)->m_lValid)
        {
            lRet = RC_FAIL;
            pstSavm->m_lErrno = DATA_SPC_FULL;
            break;
        }

        memset(pvData, 0, lGetRowSize(t));
        _lImportContext(szLine, lGetFldNum(t), pGetTblKey(t), pvData, pszFlag);
        if(RC_SUCC != (lRet = __lInsert(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, 0)))
            break;

        lEffect ++;
        memset(szLine, 0, sizeof(szLine));
        if(RC_SUCC != lRecordWork(pstSavm, pstSavm->pstVoid, OPERATE_INSERT))
            break;
    }

    pstSavm->m_lEffect = lEffect;
    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, pstSavm->tblName);

    fclose(fp);
    IBPFree(pvData);
    return lRet;
}

/*************************************************************************************************
    函数说明：从内存数据库中将数据导出
    参数说明：
        t                          --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lExportTable(TABLE t, size_t *plOut, void **ppsvOut)
{
    long    lRet;
    RunTime *pstRun = NULL;
    SATvm   *pstSavm = NULL;

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(t)))
        return RC_FAIL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, t)))
        return RC_FAIL;

    pstSavm->pstVoid = NULL;
    lRet = _lQueryTruck(pstSavm, pstRun->m_pvAddr, t, plOut, ppsvOut);
    vTblDisconnect(pstSavm, pstSavm->tblName);
    if(NO_DATA_FOUND == pstSavm->m_lErrno)
        return RC_SUCC;

    return lRet;
}

/*************************************************************************************************
    函数说明：从内存数据库中将数据导出
    参数说明：
        t                          --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lImportTable(TABLE t, size_t lCount, void *psvData)
{
    RunTime *pstRun = NULL;
    SATvm   *pstSavm = NULL;
    RWLock  *prwLock = NULL;
    long    i, lRet = RC_SUCC;

    if(!psvData || lCount < 0)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(t)))
        return RC_FAIL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, t)))
        return RC_FAIL;

    prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);
    if(RC_SUCC != pthread_rwlock_wrlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DOWR_ERR;
        return RC_FAIL;
    }

    pstSavm->lFind = IDX_SELECT | NIL_IGNORE;
    for(i = 0; i < lCount; i ++)
    {
        pstSavm->pstVoid = psvData + lGetRowSize(t) * i;
        if(RC_SUCC != (lRet = __lInsert(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, 0)))
            break;
    }

    pthread_rwlock_unlock(prwLock);
    vTblDisconnect(pstSavm, pstSavm->tblName);

    return lRet;
}

/*************************************************************************************************
    函数说明：从内存数据库中将数据导出到文件
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lExportFile(TABLE t, char *pszFile, char *pszFlag)
{
    FILE    *fp = NULL;
    char    szLine[1024];
    void    *psvOut = NULL;
    RunTime *pstRun = NULL;
    SATvm   *pstSavm = NULL;

    if(!pszFile || !pszFlag || !strlen(pszFlag))
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(szLine, 0, sizeof(szLine));
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(t)))
        return RC_FAIL;

    if(NULL == (fp = fopen(pszFile, "wb")))
    {
        pstSavm->m_lErrno = FIL_NOT_EXIST;
        return RC_FAIL;
    }

    pstSavm->pstVoid   = NULL;
    pstSavm->lFind     = IDX_SELECT;
    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, t)))
    {
        fclose(fp);
        return RC_FAIL;
    }

    if(NULL == (psvOut = (char *)malloc(lGetRowSize(t))))
    {
        fclose(fp);
        vTblDisconnect(pstSavm, pstSavm->tblName);
        return RC_FAIL;
    }

    pstRun->m_lCurLine = 1;
    pstRun->m_lCurType = EXE_PLAN_ALL;
    pstRun->m_pvCurAddr = pstRun->m_pvAddr;    
    while(RC_NOTFOUND != _llFetchTruck(pstSavm, pstRun, t, psvOut))
    {
        _lExportContext(fp, psvOut, lGetFldNum(t), pGetTblKey(t), pszFlag);
        fprintf(fp, "\n");
    }

    fclose(fp);
    IBPFree(psvOut);
    vTblDisconnect(pstSavm, pstSavm->tblName);
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取表字段
    参数说明：
        t                           --操作句柄
        plOut                        --结果集个数
        ppstField                    --结果集组
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lGetTblField(TABLE t, size_t *plOut, TField **ppstField)
{   
    TField  stField; 
    SATvm   *pstSavm = NULL; 
    
    memset(&stField, 0, sizeof(TField));
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_FIELD)))
        return RC_FAIL;
    
    stField.m_table = t;
    
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(TField);
    pstSavm->pstVoid = (void *)&stField;
    return lQuery(pstSavm, plOut, (void **)ppstField);
}

/*************************************************************************************************
    函数说明：根据表名获取表属性
    参数说明：
        pszTable                    --表名称
        pstIndex                    --表属性
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lGetTblIndex(SATvm *pstSavm, char *pszTable, char *pszPart, TIndex *pstIndex)
{
    TIndex    stIndex;

    if(!pszTable || !pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(&stIndex, 0, sizeof(TIndex));
    strncpy(stIndex.m_szPart, pszPart, sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, pszTable, sizeof(stIndex.m_szTable));
    stIndex.m_lLocal = RES_LOCAL_SID;

    pstSavm->pstVoid = &stIndex;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    if(RC_SUCC != lSelect(pstSavm, (void *)pstIndex))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            pstSavm->m_lErrno = TBL_NOT_FOUND;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：根据表名获取表属性
    参数说明：
        pszTable                    --表名称
        pstIndex                    --表属性
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lUpdIndexPart(SATvm *pstSavm, TABLE t, char *pszPart)
{
    TIndex    stIndex, stUpdate;

    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_table = t;
    stIndex.m_lLocal = RES_LOCAL_SID;

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    pstSavm->pstVoid = (void *)&stIndex;
    if(RC_SUCC != lSelect(pstSavm, (void *)&stIndex))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            pstSavm->m_lErrno = TBL_NOT_FOUND;
        return RC_FAIL;
    }

    pstSavm->bSearch = TYPE_CLIENT;
    memcpy(&stUpdate, &stIndex, sizeof(TIndex));
    strncpy(stUpdate.m_szPart, pszPart, sizeof(stUpdate.m_szPart));

    pstSavm->pstVoid = (void *)&stIndex;
    return lUpdate(pstSavm, &stUpdate, true);
}

/*************************************************************************************************
    函数说明：判断表是否存在
    参数说明：
        无
    返回值：
        TRUE                       --成功
        FALSE                      --失败
 *************************************************************************************************/
BOOL    bTableIsExist(TABLE t)
{
    TIndex  stIndex;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_table = t;

    pstSavm->pstVoid = &stIndex;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    if(RC_SUCC != lSelect(pstSavm, (void *)&stIndex))
        return false;

    return true;
}

/*************************************************************************************************
    函数说明：表序号重新命名
    参数说明：
        无
    返回值：
        TRUE                       --成功
        FALSE                      --失败
 *************************************************************************************************/
long    lRenameTable(TABLE to, TABLE tn)
{
    RunTime *pstRun = NULL;
    TIndex  stIndex, stNIdx;
    TField  stField, stNFld;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    memset(&stNFld, 0, sizeof(TField));
    memset(&stField, 0, sizeof(TField));
    memset(&stNIdx, 0, sizeof(TIndex));
    memset(&stIndex, 0, sizeof(TIndex));
    stField.m_table = to;
    stNFld.m_table  = tn;
    stIndex.m_table = to;
    stNIdx.m_table  = tn;

    pstSavm->pstVoid = &stIndex;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);

    if(RC_SUCC != lUpdate(pstSavm, &stNIdx, false))
        return RC_FAIL;

    memcpy((void *)pGetRunTime(pstSavm, tn), (void *)pGetRunTime(pstSavm, to), sizeof(RunTime));
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(tn)))
        return RC_FAIL;

    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    ((TblDef *)pstRun->m_pvAddr)->m_table = tn;
    vTblDisconnect(pstSavm, pstSavm->tblName);
    memset((void *)pGetRunTime(pstSavm, to), 0, sizeof(RunTime)); 

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_FIELD)))
        return RC_FAIL;

    pstSavm->pstVoid = &stField;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = SYS_TVM_FIELD;
    pstSavm->lSize   = sizeof(TField);

    return lUpdate(pstSavm, &stNFld, false);
}

/*************************************************************************************************
    函数说明：判断表是否存在
    参数说明：
        无
    返回值：
        TRUE                       --成功
        FALSE                      --失败
 *************************************************************************************************/
BOOL    bPartIsExist(char *pszTable, char *pszPart)
{
    TIndex  stIndex;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_lLocal = RES_LOCAL_SID;
    strncpy(stIndex.m_szPart, pszPart, sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, pszTable, sizeof(stIndex.m_szTable));

    pstSavm->pstVoid = &stIndex;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    if(RC_SUCC != lSelect(pstSavm, (void *)&stIndex))
    {
        if(MORE_ROWS_SEL == pstSavm->m_lErrno)
            return true;

        return false;
    }

    return true;
}

/*************************************************************************************************
    函数说明：获取表空间大小
    参数说明：
        无
    返回值：
        TRUE                       --成功
        FALSE                      --失败
 *************************************************************************************************/
long    lTableMaxRow(SATvm *pstSavm, TABLE t)
{
    TIndex  stIndex;
    
    if(!pstSavm)
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_table = t;

    pstSavm->pstVoid = &stIndex;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    if(RC_SUCC != lSelect(pstSavm, (void *)&stIndex))
        return RC_FAIL;

    return stIndex.m_lMaxRows;
}

/*************************************************************************************************
    函数说明：获取极值
    参数说明：
    返回值：
        TRUE                       --成功
        FALSE                      --失败
 *************************************************************************************************/
BOOL    bIsTvmBoot()
{
    RunTime *pstRun = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);

    pstRun = (RunTime *)pGetRunTime(pstSavm, SYS_TVM_INDEX);
    pstRun->m_lLocal = RES_LOCAL_SID;

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return FALSE;

    vTblDisconnect(pstSavm, pstSavm->tblName);
    pstSavm->bSearch = TYPE_CLIENT;

    return TRUE;
}

/*************************************************************************************************
    函数说明：_lExtremeGroup(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut)
    参数说明：返回指定字段最大值所在行记录
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lExtremeGroup(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut)
{
    void    *pvData = NULL;
    SHList  *pstList = NULL;
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    TblKey  *pstIdx = pGetFldKey(pstSavm->tblName, pstSavm->lReField);

    if(!pstIdx)
    {
        pstSavm->m_lErrno = FLD_NOT_EXIST;
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lGroupRoot)))
    {
         pstSavm->m_lErrno = SVR_EXCEPTION;
         return RC_FAIL;
    }

    if(GRP_SMCH_FIELD(t, pGetTblGrp(t), pstIdx))
    {
        pstSavm->m_lEType = EXE_PLAN_GRP;
        if(NULL == (pstTree = (SHTree *)pExtremeTree(pstSavm, pvAddr, pstTree)))
        {
            pstSavm->m_lErrno = NO_DATA_FOUND;
            return RC_FAIL;
        }

        if(NULL == (pstList = (SHList *)pGetNode(pvData, pstTree->m_lData)))
        {
            pstSavm->m_lErrno = NO_DATA_FOUND;
            return RC_FAIL;
        }

        pstSavm->m_lEffect = 1;
        memcpy(psvOut + pstIdx->m_lFrom, pstTree->m_szIdx, pstIdx->m_lLen);

        return RC_SUCC;
    }

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;
    
    if(NULL == (pstList = pSearchGroup(pvAddr, pstTree, szIdx, lGetGrpLen(t))))
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_CONTINUE;
    }

    pstSavm->m_lEType = EXE_PLAN_GRP;
    for(; SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
            pvData = pvCompExtrem(pstSavm, pstTruck->m_pvData, pvData, pstIdx, pstSavm->lReMatch);
    
        if(SELF_POS_UNUSE == pstList->m_lNext)    break;
    }

    if(!pvData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 1;
    memcpy(psvOut, pvData, lGetRowSize(pstSavm->tblName));

    return RC_SUCC;
}   

/*************************************************************************************************
    函数说明：按照hash算法查询表数据
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lExtremeHash(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut)
{
    size_t  lData, lIdx;
    SHTree  *pstTree = NULL;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN], *pvData = NULL;
    TblKey  *pstIdx = pGetFldKey(pstSavm->tblName, pstSavm->lReField);

    if(!pstIdx)
    {
        pstSavm->m_lErrno = FLD_NOT_EXIST;
        return RC_FAIL;
    }

    memset(szIdx, 0, sizeof(szIdx));
    if(NULL == pGetIndex(pstSavm, lGetGrpNum(t), pGetTblGrp(t), pstSavm->pstVoid, szIdx))
        return RC_CONTINUE;

    lIdx = uGetHash(szIdx, lGetGrpLen(t)) % ((TblDef *)pvAddr)->m_lMaxRow;
    pstTree = pvAddr + ((TblDef *)pvAddr)->m_lGroupRoot + lIdx * sizeof(SHTree);
    if(NULL == pstTree || SELF_POS_UNUSE == pstTree->m_lData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 0;
    pstSavm->m_lEType = EXE_PLAN_GRP;
    pstList = (SHList *)pGetNode(pvAddr, pstTree->m_lData);
    for(; SELF_POS_UNUSE != pstList->m_lPos; pstList = (SHList *)pGetNode(pvAddr, pstList->m_lNext))
    {
        pstTruck = (PSHTruck)pGetNode(pvAddr, pstList->m_lData);
        if(bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid, pstSavm->lFind))
            pvData = pvCompExtrem(pstSavm, pstTruck->m_pvData, pvData, pstIdx, pstSavm->lReMatch);

        if(SELF_POS_UNUSE == pstList->m_lNext)    break;
    }

    if(!pvData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 1;
    memcpy(psvOut, pvData, lGetRowSize(pstSavm->tblName));

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(全表遍历方式)
    参数说明：
        pstSavm                    --操作句柄
           psvOut                    --查询的结果数据
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lExtremeTruck(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut)
{
    void    *pvData = NULL;
    SHTruck *pstTruck = NULL;
    size_t  lRow = 0, lOffset = lGetTblData(t);
    TblKey  *pstIdx = pGetFldKey(pstSavm->tblName, pstSavm->lReField);

    if(!pstIdx)
    {
        pstSavm->m_lErrno = FLD_NOT_EXIST;
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 0;
    pstSavm->m_lEType = EXE_PLAN_ALL;
    pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset);
    for(lRow = 0; (lRow < ((TblDef *)pvAddr)->m_lValid) && (lOffset < lGetTableSize(t)); 
        pstTruck = (PSHTruck)pGetNode(pvAddr, lOffset))
    {        
        if(IS_TRUCK_NULL(pstTruck))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }

        lRow ++;
        if(pstSavm->pstVoid && !bTblIdxMatch(pstSavm, pstTruck->m_pvData, pstSavm->pstVoid,
            pstSavm->lFind))
        {
            lOffset += lGetRowTruck(t);
            continue;
        }
        
        lOffset+= lGetRowTruck(t);
        pvData = pvCompExtrem(pstSavm, pstTruck->m_pvData, pvData, pstIdx, pstSavm->lReMatch);
    }

    if(!pvData)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 1;
    memcpy(psvOut, pvData, lGetRowSize(pstSavm->tblName));

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：查询内存表中的数据(全表遍历方式)
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        RC_SUCC                    --成功
           RC_FAIL                    --失败
 *************************************************************************************************/
long    _lExtremeIndex(SATvm *pstSavm, void *pvAddr, TABLE t, void *psvOut)
{
    SHTree  *pstTree = NULL;
    SHTruck *pstTruck = NULL;
    char    szIdx[MAX_INDEX_LEN];
    TblKey  *pstIdx = pGetFldKey(pstSavm->tblName, pstSavm->lReField);

    if(!pstIdx)
    {
        pstSavm->m_lErrno = FLD_NOT_EXIST;
        return RC_FAIL;
    }

    if(NULL == (pstTree = (SHTree *)pGetNode(pvAddr, ((TblDef *)pvAddr)->m_lTreeRoot)))
    {
        pstSavm->m_lErrno = SVR_EXCEPTION;
        return RC_FAIL;
    }

    if(IDX_SMCH_FIELD(t, pGetTblIdx(t), pstIdx))
    {
        pstSavm->m_lEType = EXE_PLAN_IDX;
        pstTree = (SHTree *)pExtremeTree(pstSavm, pvAddr, pstTree);
    }
    else
    {
        memset(szIdx, 0, sizeof(szIdx));
        if(NULL == pGetIndex(pstSavm, lGetIdxNum(t), pGetTblIdx(t), pstSavm->pstVoid, szIdx))
            return RC_CONTINUE;
        
        pstSavm->m_lEType = EXE_PLAN_IDX;
        pstTree = (SHTree *)pSearchTree(pvAddr, pstTree, szIdx, lGetIdxLen(pstSavm->tblName));
    }
    if(!pstTree)
    {
        pstSavm->m_lErrno = NO_DATA_FOUND;
        return RC_FAIL;
    }

    pstSavm->m_lEffect = 1;
    pstTruck = (PSHTruck)pGetNode(pvAddr, pstTree->m_lData);
    memcpy(psvOut, pstTruck->m_pvData, lGetRowSize(pstSavm->tblName));

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取极值
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    _lExtremeSelect(SATvm *pstSavm, RunTime *pstRun, TABLE t, void *psvOut)
{
    long    lRet;
    RWLock  *prwLock = (RWLock *)pGetRWLock(pstRun->m_pvAddr);

    if(RC_SUCC != pthread_rwlock_rdlock(prwLock))
    {
        pstSavm->m_lErrno = LOCK_DORD_ERR;
        return RC_FAIL;
    }

    if(HAVE_UNIQ_IDX(pstSavm->tblName))
    {
        lRet = _lExtremeIndex(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, psvOut);
        if(RC_CONTINUE != lRet)
        {
            pthread_rwlock_unlock(prwLock);
            return lRet;
        }
    }

    if(HAVE_NORL_IDX(pstSavm->tblName))
    {
        lRet = _lExtremeGroup(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, psvOut);
        if(RC_CONTINUE != lRet)
        {
            pthread_rwlock_unlock(prwLock);
            return lRet;
        }
    }
    else if(HAVE_HASH_IDX(t))
    {
        lRet = _lExtremeHash(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, psvOut);
        if(RC_CONTINUE != lRet)
        {
            pthread_rwlock_unlock(prwLock);
            return lRet;
        }
    }

    lRet = _lExtremeTruck(pstSavm, pstRun->m_pvAddr, pstSavm->tblName, psvOut);
    pthread_rwlock_unlock(prwLock);
    return lRet;
}

/*************************************************************************************************
    函数说明：获取极值
    参数说明：
        pstSavm                    --操作句柄
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
 *************************************************************************************************/
long    lExtreme(SATvm *pstSavm, void *psvOut)
{
    long    lRet;
    RunTime *pstRun = NULL;

    if(!pstSavm || (MATCH_MAX != pstSavm->lReMatch && MATCH_MIN != pstSavm->lReMatch))
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    if(NULL == (pstRun = (RunTime *)pInitMemTable(pstSavm, pstSavm->tblName)))
        return RC_FAIL;

    if(RES_REMOT_SID == pstRun->m_lLocal)
        lRet = lSelectByRt(pstSavm, pstRun, pstSavm->tblName, psvOut);
    else
    {
        lRet = _lExtremeSelect(pstSavm, pstRun, pstSavm->tblName, psvOut);
        vTblDisconnect(pstSavm, pstSavm->tblName);
    }

    return RC_SUCC;
}

/*************************************************************************************************
 * debug
 *************************************************************************************************/
