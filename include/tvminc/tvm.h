#ifndef __TVM_DEFIND_HHHH___
#define __TVM_DEFIND_HHHH___
#include    "tstr.h"

typedef pthread_rwlock_t     RWLock;
typedef pthread_rwlockattr_t RWAttr;
typedef unsigned    int      TABLE;
typedef long        long     llSEQ;
typedef long                 (*TCREATE)(TABLE t);
typedef long                 CREATE;

//#pragma pack(4)

#define TVM_VERSION                        "1.0.2.0"
/*************************************************************************************************
    客户化使用
 *************************************************************************************************/
#define FIELD_LONG                          1
#define FIELD_CHAR                          2
#define FIELD_DOUBLE                        3

#define FIRST_ROW                           64
#define ORDER_ASC                           32
#define ORDER_DESC                          16
#define GROUP_BY                            8
#define MATCH_MAX                           4
#define MATCH_MIN                           2
#define MATCH_YES                           1
#define MATCH_NO                            0

//  操作执行计划
#define EXE_PLAN_ALL                        0
#define EXE_PLAN_IDX                        1 
#define EXE_PLAN_GRP                        2  

#define CHK_SELECT                          0           // 多字段匹配
#define IDX_SELECT                          1           // 索引匹配
#define RCD_SELECT                          2           // 非匹配字段
#define NIL_IGNORE                          8           // 索引字段部分可为空

#define OPERATE_NULL                        0
#define OPERATE_SELECT                      1
#define OPERATE_UPDATE                      2
#define OPERATE_DELETE                      4
#define OPERATE_INSERT                      8
#define OPERATE_QUERY                       16
#define OPERATE_DOMPSH                      32
#define OPERATE_DOMPUL                      64
#define OPERATE_DMKEEP                      128
#define OPERATE_DMRECN                      256
#define OPERATE_DOMOFF                      512
#define OPERATE_PULTBL                      (OPERATE_DOMPUL|OPERATE_DMRECN)
#define OPERATE_REFRESH                     1024
#define OPERATE_DEFAULT                     (OPERATE_SELECT | OPERATE_UPDATE | OPERATE_DELETE | OPERATE_INSERT)

/*************************************************************************************************
      内部定义
 *************************************************************************************************/
#define SYS_TVM_INDEX                       0x01        // 系统index
#define SYS_TVM_FIELD                       0x02        // 字段信息表
#define SYS_TVM_DOMAIN                      0x03        // 域连接信息表
#define SYS_TVM_SEQUE                       0x04
#define TVM_MAX_TABLE                       0xFF        // 系统最大创建个数

#define MAX_STRIG_LEN                       256
#define MAX_INDEX_LEN                       64
#define MAX_FIELD_LEN                       32
#define MAX_FILED_NUM                       32          //  一张表最大的字段个数
#define MAX_REMOTE_IP                       20
#define MAX_TIMESTAMP                       20
#define MAX_FILED_IDX                       8           //  一张表最大的字段个数

// resource flag
#define RES_LOCAL_SID                       1
#define RES_REMOT_SID                       2
#define RESOURCE_AUTH                       4           // 单边认证的表数据
#define RESOURCE_EXCP                       3           // 可能远程不存在该表
#define RESOURCE_ABLE                       2           // 远程资源可用
#define RESOURCE_STOP                       1           // 远程资源不可用
#define RESOURCE_INIT                       0           // 远程资源初始化

#define IPC_MSG                             0x01
#define IPC_SEM                             0x02 
#define IPC_SHM                             0x03 

#define SEM_INIT                            1
#define SEM_O_V                             1           //  信号量V操作
#define SEM_O_P                             -1          //  信号量P操作
#define SEM_RD                              0
#define SEM_WD                              1

//  标识该资源未被使用
#define UNQIUE                              1
#define NORMAL                              16
#define HASHID                              32

#define TYPE_SYSTEM                         0x01
#define TYPE_INCORE                         0x02
#define TYPE_CLIENT                         0x03        //  客户化
#define TYPE_KEYVAL                         0x04 
#define NUMBER_ZEOR                         -1.0
#define TVM_NODE_INFO                       "localhost"
#define TVM_RUNCFG_TAG                      "\x01\x33\xC8\x48"

#define TVM_BOOT_CLUSTER                    2
#define TVM_BOOT_LOCAL                      1
#define TVM_BOOT_SIMPLE                     0


#define HAVE_INDEX(t)                       (((TblDef *)pGetTblDef(t))->m_lIType != 0)
#define HAVE_UNIQ_IDX(t)                    (((TblDef *)pGetTblDef(t))->m_lIType & UNQIUE)
#define HAVE_NORL_IDX(t)                    (((TblDef *)pGetTblDef(t))->m_lIType & NORMAL)
#define HAVE_HASH_IDX(t)                    (((TblDef *)pGetTblDef(t))->m_lIType & HASHID)

#define IS_RED(x)                           (x->m_eColor == COLOR_RED)
#define FPOS(t, f)                          ((size_t)&((t *)0)->f)
#define FLEN(t, f)                          (sizeof(((t *)0)->f))
#define NODE_NULL                           g_lNilOfs
#define	ReField(t, f)                       (FPOS(t, f) << 16 | FLEN(t, f))
#define	REFrom(t)                           (t >> 16)
#define	REFLen(t)                           (t & 0xffff)

#define COLOR_BLK                           0 
#define COLOR_RED                           1
#define SELF_POS_UNUSE                      0 
#define DATA_TRUCK_NULL                     0x00
#define DATA_TRUCK_NRML                     0x01
#define DATA_TRUCK_LOCK                     0x02
#define TABLE_LOCK_READ                     1
#define TABLE_LOCK_WRITE                    2
#define IS_TRUCK_NULL(p)                    ((p)->m_chTag == DATA_TRUCK_NULL)
#define IS_TRUCK_NRML(p)                    ((p)->m_chTag == DATA_TRUCK_NRML)
#define IS_TRUCK_LOCK(p)                    ((p)->m_chTag == DATA_TRUCK_LOCK)
#define SET_DATA_TRUCK(p, type)             ((p)->m_chTag =  type)
#define IBPFree(p)                          if(p) { free(p); p = NULL; }
#define IBPFgrp(p)                          do{vDeleteRowgrp(p);p = NULL;}while(0);
#define IBPclose(f)                         if(f) { fclose(f); f = NULL; }
#define IDX_SMCH_FIELD(t, p, f)             (1 == lGetIdxNum(t) && p->m_lFrom == f->m_lFrom && FIELD_CHAR == p->m_lAttr)
#define GRP_SMCH_FIELD(t, p, f)             (1 == lGetGrpNum(t) && p->m_lFrom == f->m_lFrom && FIELD_CHAR == p->m_lAttr)

/*************************************************************************************************
    错误码定义区
 *************************************************************************************************/
#define TVM_DONE_SUCC                       0           // completed successfully
#define SVR_EXCEPTION                       1           // sever exception
#define IDX_FIELD_NIL                       2           // index field values is null
#define CONDIT_IS_NIL                       3           // condition is null
#define DATA_SPC_FULL                       4           // no space for create data
#define GENER_KEY_ERR                       5           // generate shm key failure
#define SHM_ERR_INVAL                       6           // Invalid parameter or shm has disappeared
#define SHM_ERR_EXIST                       7           // shared memory already exists
#define SHM_ERR_EIDRM                       8           // shared memory has been deleted
#define SHM_ERR_ACCES                       9           // Permission denied
#define SHM_ERR_NOMEM                       10          // Insufficient(shm) core memory
#define VER_NOT_MATCH                       11          // data truck version mismatch
#define BLCK_SIZE_ERR                       12          // size is error to creat data block
#define IDX_DEF_SPILL                       13          // unique Index definition overflow
#define IDX_LEN_SPILL                       14          // unique Index length overflow
#define GRP_DEF_SPILL                       15          // normal Index definition overflow
#define GRP_LEN_SPILL                       16          // normal Index length overflow
#define IDX_TYP_NODEF                       17          // index type not define
#define FLD_DEF_SPILL                       18          // field define overflow
#define IDX_DATA_MISM                       19          // index data mismatch
#define FTYPE_NOT_DEF                       20          // field type not define
#define SHMT_NOT_INIT                       21          // memory has not been initialized
#define UNIQ_IDX_REPT                       22          // unique index repeat
#define IDX_SPC_SPILL                       23          // no space for create index
#define NO_DATA_FOUND                       24          // no data be found
#define MORE_ROWS_SEL                       25          // more then one records be selected
#define MALLC_MEM_ERR                       26          // malloc memory error
#define CURS_IS_INVAL                       27          // cursor invalid
#define TABLE_NOT_DEF                       28          // table not define
#define FIL_NOT_EXIST                       29          // file not exist
#define SEM_CDIT_NULL                       30          // semget condition is null
#define SEM_ERR_INVAL                       31          // Invalid parameter or sem has disappeared
#define SEM_ERR_EXIST                       32          // semaphore already exists
#define SEM_ERR_EIDRM                       33          // semaphore has been deleted
#define SEM_ERR_ACCES                       34          // Permission denied(sem)
#define SEM_ERR_NOMEM                       35          // Insufficient(sem) core memory
#define SEM_ERR_LIMIT                       36          // Semaphore value out of limit
#define SQL_SYNTX_ERR                       37          // SQL syntax is error
#define SQL_NOT_SUPPT                       38          // SQL operation not be supported
#define SQL_TABLE_NIL                       39          // SQL no table name be inputted
#define SQL_FIELD_NIL                       40          // SQL field is not selected
#define SQL_WHERE_NIL                       41          // SQL conditional syntax error
#define SQL_ERR_FIELD                       42          // SQL field syntax error
#define SQL_ERR_WHERE                       43          // SQL where syntax error
#define TBL_NOT_FOUND                       44          // table not found
#define SQL_FAV_MATCH                       45          // SQL fields does not match the value 
#define LOCK_DORD_ERR                       46          // set the read lock failure
#define LOCK_UNRD_ERR                       47          // unlock read lock failure
#define LOCK_DOWR_ERR                       48          // set the write lock failure
#define LOCK_UNWR_ERR                       49          // unlock write lock failure
#define SOCK_CONN_ERR                       50          // socket connect failure
#define SOCK_CONN_TMO                       51          // socket connect timeout
#define SOCK_ERR_CRTE                       52          // create socket failure
#define SOCK_READ_ERR                       53          // socket recv failure
#define SOCK_SEND_ERR                       54          // socket send failure
#define SOCK_BIND_ERR                       55          // socket bind failure
#define SOCK_LSEN_ERR                       56          // socket listen failure
#define SOCK_SEND_TMO                       57          // socket send timeout
#define SOCK_READ_TMO                       58          // socket read timeout
#define SOCK_IO_RESET                       59          // socket reset
#define EPOLL_ADD_ERR                       60          // epoll add fd error
#define EPOLL_CRT_ERR                       61          // create epoll fd failure
#define EPOLL_DEL_ERR                       62          // delete epoll fd failure
#define SOCK_ACPT_ERR                       63          // socket accept failure
#define RMT_NOT_SUPPT                       64          // SQL remote does not support
#define FILE_NOTFOUND                       65          // file not found
#define BOOT_PARM_ERR                       66          // boot parameters error
#define BOOT_RELA_ERR                       67          // parameters table related error
#define BOOT_VER_ICMP                       68          // Incompatible version
#define DOM_NOT_REGST                       69          // domain not register
#define DMWORK_NOTSUP                       70          // domain work does not support
#define SEQ_NOT_FOUND                       71          // sequence does not exist
#define FILE_NOT_RSET                       72          // file is not set
#define RECD_TOO_LONG                       73          // record data too long 
#define RESOU_DISABLE                       74          // Resource unavailable 
#define MSG_ERR_EXIST                       75          // message queue already exists
#define MSG_ERR_ACCES                       76          // Permission denied .msg
#define MSG_ERR_NOMEM                       77          // Insufficient(msg) core memory
#define MSG_ERR_INVAL                       78          // Invalid parameter or msg has disappeared
#define MSG_ERR_FAULT                       79          // msg Invalid address
#define MSG_ERR_EIDRM                       80          // message queue has been deleted
#define MSG_ERR_E2BIG                       81          // message text length is greater than msgsz
#define MSG_ERR_EINTR                       82          // Interrupted by signal
#define MSG_ERR_SNDEG                       83          // msgsnd queue overrun
#define INI_ERR_CHLDP                       84          // initial child process failed
#define FLD_NOT_EXIST                       85          // field not exist
#define TBL_ARD_EXIST                       86          // table already exist
#define WORK_NOT_OPEN                       87          // The transaction has not been opened yet

/*************************************************************************************************
     创建表宏函数
 *************************************************************************************************/
#define DEFINE(t, n, p, s)      TABLE tbl = t; long type = 0; \
                                ((TblDef *)pGetTblDef(t))->m_table = t; \
                                ((TblDef *)pGetTblDef(t))->m_lReSize = sizeof(s); \
                                strncpy(((TblDef *)pGetTblDef(t))->m_szPart, p, MAX_FIELD_LEN); \
                                strncpy(((TblDef *)pGetTblDef(t))->m_szTable, n, MAX_FIELD_LEN); \
                                ((TblDef *)pGetTblDef(t))->m_lTruck = sizeof(s) + sizeof(SHTruck);

#define CREATE_IDX(t)           type = t;
#define IDX_FIELD(t, f, a)      if(RC_SUCC != lAddIdxField(tbl, type, FPOS(t, f), FLEN(t, f), a)) \
                                    return RC_FAIL;
#define FIELD(t, f, d, a)       if(RC_SUCC != lSetTableIdx(tbl, FPOS(t, f), FLEN(t, f), d, a, CHK_SELECT)) \
                                    return RC_FAIL;
#define FIELU(t, f, d, a)       if(RC_SUCC != lSetTableIdx(tbl, FPOS(t, f), FLEN(t, f), d, a, IDX_SELECT)) \
                                    return RC_FAIL;
#define FIELR(t, f, d, a)       if(RC_SUCC != lSetTableIdx(tbl, FPOS(t, f), FLEN(t, f), d, a, RCD_SELECT)) \
                                    return RC_FAIL;
#define FINISH                  return RC_SUCC;

/*************************************************************************************************
    表结构&索引定义区
 *************************************************************************************************/
typedef struct  __SH_DATA_TRUCK
{
    ulong   m_lTimes;
    char    m_chTag;
    char    m_pvData[0];
}SHTruck, *PSHTruck;

typedef struct  __SH_RBTREE
{
    size_t  m_lSePos;
    char    m_szIdx[MAX_INDEX_LEN];
    long    m_lIdx;
    size_t  m_lData;
    long    m_eColor;
    size_t  m_lParent;
    size_t  m_lLeft;
    size_t  m_lRight;
}SHTree;

typedef struct  __SH_LIST
{
    size_t    m_lPos; 
    size_t    m_lNode;
    size_t    m_lData;
    size_t    m_lNext;
    size_t    m_lLast;
}SHList;

typedef struct __TBL_COM_KEY
{
    long    m_lFrom;
    long    m_lLen; 
    long    m_lAttr;
    long    m_lIsPk;
    char    m_szField[MAX_FIELD_LEN]; 
}TblKey;

typedef struct  __TBL_HEAD_DEF
{
    RWLock  m_rwlock;                         //  读写锁
    long    m_lGroup;                         //  组记录个数(索引)
    size_t  m_lMaxRow;                        //  该表最大数据个数
    size_t  m_lValid;                         //  有效条数
    long    m_lIdxLen;                        //  索引长度
    size_t  m_lNodeNil;                       //  NIL节点
    size_t  m_lTreePos;                       //  唯一索引数据区位置    
    size_t  m_lTreeRoot;                      //  索引区Root节点
    long    m_lGrpLen;                        //  索引长度
    size_t  m_lGroupPos;                      //  查询索引区组位置
    size_t  m_lGroupRoot;                     //  索引组区Root节点
    size_t  m_lListPos;                       //  链表区位置
    size_t  m_lListOfs;                       //  链表区相对唯一量
    size_t  m_lData;                          //  数据区位置
    uint    m_lIType;                         //  索引类型
    uint    m_lIdxUp;                         //  是否定义唯一索引
    TblKey  m_stIdxUp[MAX_FILED_IDX];         //  唯一索引组成
    uint    m_lGrpUp;                         //  是否定义唯一索引
    TblKey  m_stGrpUp[MAX_FILED_IDX];         //  查询组合索引组成
    size_t  m_lTable;                         //  表大小
    long    m_lReSize;                        //  记录大小
    size_t  m_lTruck;                         //  数据块大小
    llSEQ   m_lExSeQ;                         //  拓展流水号
    long    m_lExtern;                        //  拓展备用字段    
    long    m_lIdxNum;                        //  每一个表的索引个数
    TblKey  m_stKey[MAX_FILED_NUM];           //  每一个表的索引定义
    TABLE   m_table;                          //  表序号
    char    m_szTable[MAX_FIELD_LEN];         //  表名
    char    m_szPart[MAX_FIELD_LEN];          //    
    SHTree  m_stNil;
}TblDef;

static    long    g_lNilOfs = FPOS(TblDef, m_stNil);

typedef struct __SQL_FIELD
{
     TblKey  m_stKey;   
    struct  __SQL_FIELD   *pstNext;
}SQLFld;

/*************************************************************************************************
    TVM引擎启动所需表(勿动)
 *************************************************************************************************/
typedef struct  __SYS_TVM_INDEX
{
    TABLE   m_table;                          //  表序号
    long    m_lType;                          //  表类型
    char    m_szTable[MAX_FIELD_LEN];         //  表名
    char    m_szPart[MAX_FIELD_LEN];          //    
    char    m_szOwner[MAX_FIELD_LEN];         //  所属
    key_t   m_yKey;                           //  表Key值
    long    m_shmID;                          //  内存表shmID
    long    m_semID;                          //  信号量ID
    long    m_lPid;                           //  进程PID
    BOOL    m_bAttch;                         //  连接
    long    m_lValid;                         //  表有效记录
    long    m_lMaxRows;                       //  表最大支持记录数
    long    m_lRowSize;                       //  truck行大小
    long    m_lLocal;
    long    m_lState;                         //  是否可用(对外可用, 默认对外都可用)
    long    m_lPers;                          //  权限
    char    m_szRemote[30];
    char    m_szTime[MAX_TIMESTAMP];          //  最后更新时间
}TIndex;

typedef struct  __SYS_TVM_FIELD
{
    TABLE   m_table;                          //  表序号
    long    m_lSeq;                           //  对表字段个数
    char    m_szOwner[MAX_FIELD_LEN];         //  表名
    char    m_szTable[MAX_FIELD_LEN];         //  表名
    char    m_szField[MAX_FIELD_LEN];         //  字段名称
    long    m_lAttr;                          //  索引字段的属性
    long    m_lFrom;                          //  索引起始位置
    long    m_lLen;                           //  索引长度
    long    m_lIsPk;                          //  是否是唯一索引
}TField;

typedef struct  __SYS_TVM_DOMAIN
{
    BSock   m_skSock;
    long    m_lLIPort;
    long    m_lLDPort;
    TABLE   m_table;
    TABLE   m_mtable;
    long    m_lLock;
    long    m_lGroup;
    long    m_lKeepLive;                       //  心跳包时间
    long    m_lLastTime;
    long    m_lTimeOut;
    long    m_lTryMax;
    long    m_lTryTimes;
    long    m_lRowSize;                        //  truck行大小
    long    m_lStatus;                         //  正常 无用 待重试
    long    m_lPers;                           //  权限
    long    m_lRIPort;
    long    m_lRDPort;
    char    m_szIp[MAX_REMOTE_IP];
    char    m_szTable[MAX_FIELD_LEN];
    char    m_szPart[MAX_FIELD_LEN];
    char    m_szOwner[MAX_FIELD_LEN];          //  所属
}TDomain;

typedef struct  __SYS_TVM_SEQUE
{
    char    m_szSQName[MAX_INDEX_LEN];         //  序列名称
    uint    m_uIncrement;
}TSeque;

/*************************************************************************************************
    操作句柄
 *************************************************************************************************/
typedef struct  __TVM_WORK
{
    TABLE   m_table;
    long    m_lRowSize;                       //  truck行大小
    long    m_lFind;
    long    m_lOperate;
    void    *m_pvData;
    void    *m_pvNew;
}TWork;

typedef struct  __TVM_RUNTIME
{
    long    lFind;                            //  查询规则
    BOOL    bSearch;
    TABLE   tblName;
    long    lReMatch;                         //  对快速查询，再次匹配标识
    llong   lReField; 
    void    *pstVoid;                         //  查询条件
    BOOL    m_bAttch;                         //  是否初始化内存

    long    m_lState;
    long    m_lLocal;
    long    m_lPort;
    int     m_lTime;            
    char    m_szIP[MAX_REMOTE_IP];
    char    m_szPart[MAX_FIELD_LEN];          //    
    char    m_szTable[MAX_FIELD_LEN];
    long    m_shmID;                          //  内存Key
    long    m_semID;                          //  信号灯key
    long    m_lRowSize;                       //  记录数据块大小
    void    *m_pvAddr;
    long    m_lCurLine;                       //  取值记录行
    long    m_lCurType;                       //  遍历类型
    void    *m_pvCurAddr;                     //  游标地址
}RunTime;

typedef struct  __TVM_OPERATE
{
    long    lFind;                            //  查询规则
    BOOL    bSearch;                          //  从客户表中查询
    long    lSize;                            //  待查询结构大小
    TABLE   tblName;                          //  表明
    long    lReMatch;                         //  对快速查询，查询后再次匹配标识
    llong   lReField; 
    void    *pstVoid;                         //  结构信息

    BOOL    m_bWork;                          //  事务
    CMList  *m_pstWork;                       //  操作事务记录    
    BOOL    m_bPrew;                          //  事务
    BOOL    m_bHold;                          //  长链标识
    uint    m_lTimes;
    BSock   m_skSock;
    long    m_lEType;
    BOOL    m_bCreat;                         //  true自定义Key值，false系统默认key值
    key_t   m_yKey;
    key_t   m_ySey;
    key_t   m_yMey;
    long    m_lErrno;
    char    m_szMsg[256];                     //  操作过程中用户自定义的错误描述
    long    m_lEffect;                        //  执行更新影响记录数
    BOOL    m_bCache;
    char    m_szNode[MAX_FIELD_LEN];          //  所属
    RunTime stRunTime[TVM_MAX_TABLE];         //  用于游标查询
}SATvm;

typedef    struct    __TVM_BOOT_PARAM
{
    long    m_lMaxTable;
    long    m_lMaxField;
    long    m_lMaxDomain;
    long    m_lMaxSeque;
    long    m_lSvrBoot;
    long    m_lSvrPort;
    long    m_lDomPort;
    long    m_lExtern;
    long    m_lBootType;
    long    m_lBootKey;
    char    m_szNode[MAX_FIELD_LEN];
    char    m_szLog[MAX_STRIG_LEN];
}TBoot;

/*************************************************************************************************
    内部函数
 *************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
extern    long     lAddIdxField(TABLE, long, long, long, long);
extern    long     lSetTableIdx(TABLE, long, long, char*, long, long);
extern    void*    pInitMemTable(SATvm *pstSavm, TABLE t);
extern    void*    pInitHitTest(SATvm *pstSavm, TABLE t);
extern    TblDef*  pGetTblDef(TABLE t);
extern    RWLock*  pGetRWLock(char* pvAddr);
extern    void*    pGetNode(void *pvData, size_t lOfs);
extern    TblKey*  pGetTblIdx(TABLE t);
extern    TblKey*  pGetTblGrp(TABLE t);
extern    TblKey*  pGetTblKey(TABLE t);
extern    RunTime* pGetRunTime(SATvm *pstSavm, TABLE t);
extern    void*    pGetBoot();
extern    void*    pBootInitial();
extern    void*    pGetSATvm();
extern    long     lGetLDPort();
extern    char*    sGetLog();
extern    char*    sGetNode(char *s);
extern    long     lGetPermit(TABLE t);
extern    void     vRedeError(long err, char *s);
extern    void*    pGetAddr(SATvm *pstSavm, TABLE t);
extern    long     lTableMaxRow(SATvm *pstSavm, TABLE t);
extern    long     lGetBootConfig(SATvm *pstSavm, TBoot *pstBoot);
extern    long     lUpdIndexPart(SATvm *pstSavm, TABLE t, char *pszPart);
extern    long     lGetTblField(TABLE t, size_t *plOut, TField **ppstField);
extern    long     lGetTblIndex(SATvm *pstSavm, char *pszTable, char *pszPart, TIndex *pstIndex);

/*************************************************************************************************
    IPC 消息队列和信号量 
 *************************************************************************************************/
extern    long     lOperateSems(SATvm *pstSavm, long semID, long lSems, Benum evp);
extern    long     lCreateSems(SATvm *pstSavm, RunTime *pstRun, long lSems, long lValue);
extern    long     lCreateQueue(SATvm *pstSavm, BOOL bCreate);
extern    long     lGetQueueNum(SATvm *pstSavm, long lQid);
extern    long     lQueueMaxByte(SATvm *pstSavm, long lQid);
extern    long     lQueueRcvTime(SATvm *pstSavm, long lQid);
extern    long     lEventRead(SATvm *pstSavm, long lQid, void *pstVoid, long lSize, long lMType);
extern    long     lReadNoWait(SATvm *pstSavm, long lQid, void *psvVoid, long lSize, long lMType);
extern    long     lEventWrite(SATvm *pstSavm, long lQid, void *psvData, long lSize);

/*************************************************************************************************
    对外API
 *************************************************************************************************/
extern    long     lShutdownTvm();
extern    long     lStartupTvm(TBoot *pstBoot);

extern    char*    sGetUpdTime();
extern    long     lGetTErrno();
extern    void     vSetTvmMsg(SATvm *pstSavm, char *fmt, ...);
extern    char*    sGetTvmMsg(SATvm *pstSavm);
extern    void     vSetTErrno(long err);
extern    char*    sGetTError(long err);
extern    BOOL     bIsTvmBoot();
extern    void*    pInitSATvm(TABLE t);
extern    long     lInitSvCache(SATvm *pstSavm);
extern    void     vInitSATvm(SATvm *pstSavm);
extern    BOOL     bTableIsExist(TABLE t);
extern    BOOL     bPartIsExist(char *pszTable, char *pszPart);
extern    long     lInitSATvm(SATvm *pstSavm, TABLE t);
extern    void*    pPartSatvm(SATvm *pstSavm, char *pszTable, char *pszPart);
extern    long     lRebuildIndex(SATvm *pstSavm, TABLE t);

extern    void     vTvmHoldConnect(SATvm *pstSavm);
extern    void     vTvmHoldRelease(SATvm *pstSavm);
extern    long     lTvmConnect(SATvm *pstSavm, char *pszIp, long lPort, int times);
extern    void     vTvmDisconnect(SATvm *pstSavm);
extern    void     vTblDisconnect(SATvm *pstSamo, TABLE t);
extern    void     vForceDisconnect(SATvm *pstSamo, TABLE t);
extern    void     vBeginWork(SATvm *pstSavm);
extern    void     vEndWork(SATvm *pstSavm);
extern    long     lCommitWork(SATvm *pstSavm);
extern    long     lRollbackWork(SATvm *pstSavm);
extern    long     lDropTable(SATvm *pstSavm, TABLE t);
extern    long     lImportFile(TABLE t, char *pszFile, char *pszFlag);
extern    long     lExportFile(TABLE t, char *pszFile, char *pszFlag);
extern    long     lImportTable(TABLE t, size_t lCount, void *psvOut);
extern    long     lExportTable(TABLE t, size_t *plOut, void **ppsvOut);

extern    long     lRenameTable(TABLE to, TABLE tn);
extern    long     lCreateSeque(SATvm *pstSavm, char *pszSQName, uint uIncre);
extern    long     lSelectSeque(SATvm *pstSavm, char *pszSQName, ulong *pulNumber);
extern    long     lSetSequence(SATvm *pstSavm, char *pszSQName, uint uStart);
extern    long     lCustomTable(SATvm *pstSavm, TABLE t, size_t lRow, TblDef *pstDef);
extern    long     lCreateTable(SATvm *pstSavm, TABLE t, size_t lRow, TCREATE pfCreateFunc);
extern    long     lInsertTrans(SATvm *pstSavm, size_t *plOffset, llSEQ *pllSeq);


extern    long     lDelete(SATvm *pstSavm);
extern    long     lInsert(SATvm *pstSavm);
extern    long     lTruncate(SATvm *pstSavm, TABLE t);
extern    long     lSelect(SATvm *pstSavm, void *psvOut);
extern    long     lUpdate(SATvm *pstSavm, void *psVoid, const BOOL bIsNil);
extern    long     lTableDeclare(SATvm *pstSavm, TABLE t);
extern    long     lTableFetch(SATvm *pstSavm, TABLE t, void *psvOut);
extern    long     lNextFetch(SATvm *pstSavm, TABLE t, void **ppvOAddr);
extern    void     vTableClose(SATvm *pstSavm);
extern    long     lCount(SATvm *pstSavm, size_t *plCount);
extern    long     lGroup(SATvm *pstSavm, Rowgrp **ppsvOut);
extern    long     lQuery(SATvm *pstSavm, size_t *plOut, void **ppsvOut);
extern    long     lExtreme(SATvm *pstSavm, void *psvOut);
#ifdef __cplusplus
}
#endif

/*************************************************************************************************
    code end
 *************************************************************************************************/
#endif    //    __TVM_DEFIND_HHHH___
