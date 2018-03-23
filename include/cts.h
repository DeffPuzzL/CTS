#ifndef    __CTS_PLAT_DEFINE__HHH___
#define    __CTS_PLAT_DEFINE__HHH___
#include    "Ibp.h"
#include    "tvm.h"
#include    "tmain.h"
#include    "cJSON.h"

/*************************************************************************************************
    CTS系统版本号管理
 *************************************************************************************************/
#define     CTS_VERSION                        "2.1.1.0"

/*************************************************************************************************
    重定义
 *************************************************************************************************/
typedef struct        inotify_event    notifyevt;

/*************************************************************************************************
 *************************************************************************************************/
#define FIELD_NUMBER                            1
#define FIELD_STRING                            2
#define FIELD_DOUBLE                            3
#define FILED_BINARY                            4
#define FILED_SENACT                            5    //  帐号类型脱敏  前6+后4
#define FILED_SENNAM                            6    //  名称脱敏      前2(GBK)
#define FILED_SENTEL                            7    //  手机号脱敏    前3+后4
#define FILED_SENMAL                            8    //  邮箱脱敏      前1+@后面

/*************************************************************************************************
    CTS表宏定义
 *************************************************************************************************/
#define CTS_PLAT_PARAM                          0x10  //  平台表
#define CTS_PLAT_GROUP                          0x11  //  平台进程组表
#define CTS_PLAT_PROCESS                        0x12  //  平台进程表
#define CTS_ASYN_IDENTIFY                       0x13  //  异步标识表
#define CTS_TRADE_HOST                          0x14  //  主机信息表
#define CTS_TRADE_CODE                          0x15  //  平台交易码转换表
#define CTS_TRADE_CLASS                         0x16  //  平台组件表
#define CTS_TRADE_SERVICE                       0x17  //  平台服务表
#define CTS_TRADE_FLOW                          0x18  //  平台流程表
#define CTS_RETCODE_MAP                         0x19  //  响应码对照表 
#define CTS_PACK_CONFIG                         0x1a  //  报文配置表
#define CTS_RUNTIME_POOL                        0x1b  //  交易缓存表
#define CTS_MANAGER_USER                        0x1c  //  用户表
#define CTS_USER_KEYCODE                        0x1d  //  用户表密码表

/*************************************************************************************************
    平台参数定义
 *************************************************************************************************/
#define BCS_APP_DUMP                            4     //  异常
#define BCS_APP_STOP                            3     //  停止
#define BCS_APP_BUSY                            2     //  忙
#define BCS_APP_IDLE                            1     //  空闲
//#define BCS_APP_NUSE                            0     //  未使用

//    进程拓展定义标识
#define BCS_EXPAND_NML                          0     //  正常启动的进程
#define BCS_EXPAND_MON                          1     //  监控拓展的进程
#define BCS_EXPAND_CHD                          2     //  父进程启动进程
#define BCS_EXPAND_EXT                          3     //  平台自拓展进程

//    系统状态定义
#define SYSTEM_PLT_START                        1     //  运行
#define SYSTEM_PLT_STOP                         2     //  停止
#define SYSTEM_PLT_PAUSE                        3     //  暂停服务
#define SYSTEM_PLT_BING                         4     //  启动中

//    服务可用定义
#define STATUES_ENABLE                          1     //  可用
#define STATUES_DISABLE                         2     //  不可用

//    系统处理标识
#define STATUS_OK                               0     // 处理正常
#define STATUS_ERR                              -1    // 处理异常

#define TRADE_POOL_NULL                         0
#define TRADE_POOL_INIT                         1
#define TRADE_POOL_INMSG                        2
#define TRADE_POOL_OUTMSG                       3
#define TRADE_POOL_POOL                         4
#define TRADE_POOL_FAIL                         5

#define TRADE_ENABLE_ASYN                       1     //  异步标识
#define TRADE_DISABLE_ASYN                      0     //  异步标识

#define FLOW_NORMAL_NULL                        0x00  //  整成流程模块
#define FLOW_ENABLE_ASYN                        0x01  //  异步标识
#define FLOW_HALF_SYNCHR                        0x02  //  半同步流程

#define LAUNCH_BY_NORMAL                        0     //  正常业务发起的交易
#define LAUNCH_BY_MONITOR                       1     //  监控发起的交易
#define LAUNCH_BY_SYDEBUG                       2     //  用户发起同步调试交易
#define LAUNCH_BY_ASDEBUG                       3     //  用户发起异步调试交易
#define LAUNCH_BY_SYCALL                        4     //  交易本身发起同步交易
#define LAUNCH_BY_ASCALL                        5     //  交易本身发起异步交易
#define LAUNCH_BY_NRMLAS                        6     //  正常业务接入的异步处理(接入完之后立刻返收到技术性应答, 不需要再次返回响应)
#define LAUNCH_BY_HASYNC                        7     //  正常交易发起的半同步交易
#define LAUNCH_BY_BOOKING                       8     //  交易预约，提供更快的接入能力

#define FLOW_SYNC_REQUEST                       0     //  同步流程调用
#define FLOW_ASYN_REQUEST                       1     //  异步流程调用

#define CTS_FLOW_END                            -1    //  流程结束
#define CTS_FLOW_BEGIN                          1     //  流程开始

#define HOST_TYPE_BUSS                          1     //  流程结束
#define HOST_TYPE_SERVER                        2     //  流程结束
#define HOST_TYPE_SERVICE                       3     //  流程结束
#define HOST_TYPE_CLIENT                        4     //  流程结束

//#define __KEEP_RUN__                            
#define EXP_CONST_KEPRUN                        1     //  __KEEP_RUN__ 无条件继续执行
#define EXP_CONST_STRING                        2     //  string  自定义格式
#define EXP_CONST_CONDIT                        3     //  if
#define EXP_CONST_STRCMP                        4     //  strcmp
#define EXP_CONST_STRCPY                        5     //  strcpy
#define EXP_CONST_STRCAT                        6     //  strcat
#define EXP_CONST_STRTOL                        7     //  atol
#define EXP_CONST_SUBSTR                        8     //  substr
#define EXP_CONST_STRSET                        9     //  =
#define EXP_CONST_DECODE                        10    //  decode

#define ITEM_LEN_FIXED                          0     //  指定长度为固定
#define ITEM_BCD_FIXED                          1     //  指定长度为BCD
#define ITEM_BCDP_FIXED                         2     //  根据压缩BCD来读取固定字节
#define ITEM_BCDP_BCDPL                         3     //  根据压缩BCD来读取取整字节，左对齐
#define ITEM_BCDP_BCDPR                         4     //  根据压缩BCD来读取取整字节，右对齐
#define ITEM_FIXED_BCDPL                        5     //  根据固定来读取取整字节，左对齐
#define ITEM_FIXED_BCDPR                        6     //  根据固定来读取取整字节，右对齐
#define ITEM_ASCII_BCDPL                        7     //  根据字符ASCII读取取整字节，左对齐
#define ITEM_ASCII_BCDPR                        8     //  根据字符ASCII读取取整字节，右对齐
#define ITEM_ASCII_FIXED                        9     //  根据字符ASCII读取固定字节
#define ITEM_ALL_REMAIN                         10    //  剩下全部报文

/*************************************************************************************************
    系统响应码&响应信息定义
 *************************************************************************************************/
#define RET_SUCCESS                             "000"        //  交易成功
#define RET_SYSTEM                              "010"        //  系统错误(系统本身配置等问题)
#define RET_OVERFULL                            "020"        //  系统繁忙
#define RET_TIMEOUT                             "030"        //  交易超时
#define RET_FLOWCTRL                            "040"        //  流量控制
#define RET_FAULTISO                            "050"        //  故障隔离
#define RET_DATABASE                            "080"        //  数据库错误
#define MSG_SUCCESS                             "交易成功"   //  成功描述
#define MSG_FAILURE                             "交易失败"   //  失败缺省描述
#define MSG_TIMEOUT                             "交易超时"   //  失败缺省描述

/*************************************************************************************************
    返回值定义
 *************************************************************************************************/
#define RC_EXIT                                 -2
#define RC_EXIST                                -3
#define RC_RESET                                -4
#define RC_SIZEZORE                             0
#define RC_CLOSE                                -9  
#define RC_IGNORE                               -10         /*  渠道装载忽略行定义*/
#define RC_NOTFOUND                             1403

#ifndef TRUE
#define TRUE                                    1
#endif

#ifndef FALSE
#define FALSE                                   0
#endif

#define    true                                 1
#define    false                                0

/*************************************************************************************************
    平台事件定义
 *************************************************************************************************/
#define EVENT_EVQUIT                            1
#define EVENT_EVREBOOT                          2
#define EVENT_INVALID                           3
#define EVENT_REVERSE                           4
#define EVENT_SERVICE                           5
#define EVENT_EVPOOL                            6
#define EVENT_EVINMSG                           7
#define EVENT_EVOUTMSG                          8
#define EVENT_EVFINISH                          9
#define EVENT_EVCACHE                           10
#define EVENT_EVDEBUG                           11
#define EVENT_EVHASYN                           12
#define EVENT_EVASYNY                           13

/*************************************************************************************************
    平台交易分类
 *************************************************************************************************/
#define SVC_TRADE_NMCHK                         0        /*  一般查询 */
#define SVC_TRADE_FOCHK                         1        /*  重要查询 */
#define SVC_TRADE_COMSU                         2        /*  消费交易 */
#define SVC_TRADE_REVER                         3        /*  冲正交易 */
#define SVC_TRADE_MONIT                         4        /*  监控交易 */
#define SVC_TRADE_BATCH                         5        /*  批量交易 */
#define SVC_TRADE_SYSTM                         6        /*  系统交易 */

/*************************************************************************************************
    属性宏定义
 *************************************************************************************************/
#define IBPstatok(x)                            (STATUS_OK == x)
#define IBPstaterr(x)                           (STATUS_ERR== x)
#define IBPIsable(x)                            (x == STATUES_ENABLE)
#define IBPIsok()                               (STATUS_OK == lGetStatus())
#define IBPIserr()                              (STATUS_ERR == lGetStatus())
#define IBPasyn(x)                              (x == TRADE_ENABLE_ASYN)

//    事件定义
#define IBPevexit(x)                            (x->m_lState == EVENT_EVQUIT)
#define IBPevcache(x)                           (x->m_lState == EVENT_EVCACHE)
#define IBPevreboot(x)                          (x->m_lState == EVENT_EVREBOOT)
#define IBPevpool(x)                            (x->m_lState == EVENT_EVPOOL)
#define IBPevinmsg(x)                           (x->m_lState == EVENT_EVINMSG)
#define IBPevoutmsg(x)                          (x->m_lState == EVENT_EVOUTMSG)
#define IBPevfinish(x)                          (x->m_lState == EVENT_EVFINISH)
#define IBPevdebug(x)                           (x->m_lState == EVENT_EVDEBUG)
#define IBPevhasyn(x)                           (x->m_lState == EVENT_EVHASYN)
#define IBPevasyny(x)                           (x->m_lState == EVENT_EVASYNY)
#define IBPinvalid(x)                           (x->m_lState == EVENT_INVALID)
#define IBPreverse(x)                           (x->m_lState == EVENT_REVERSE)
#define IBPservice(x)                           (x->m_lState == EVENT_SERVICE)

/*************************************************************************************************
    变量池标签定义 
 *************************************************************************************************/
#define VAR_POOL_FLG                            "$$"
#define CTS_POOL_TAR                            "TR_"
#define CTS_POOL_BAT                            "BAT_"
#define MAP_POOL_ENT                            "TR_EntNo"
#define MAP_POOL_COD                            "TR_MapCode"
#define MAP_POOL_MSG                            "TR_MapMsg"

/*************************************************************************************************
     平台核心进程注册 
 *************************************************************************************************/
#define SYSTEM_NAME                             "cts"
#define CTS_MAIN                                "cts"
#define CTS_CORE                                "cts_core"
#define CTS_MSGEX                               "cts_msgex"
#define CTS_MANAGE                              "cts_manage"
#define CTS_ALARM                               "cts_alarm"
#define CTS_MON                                 "cts_mon"

#define DEFAUT_HOST                             -1    //    平台进程所属主机号
#define DEFAULT_LOG                             "cts.log"
#define CRITICAL_LOG                            "cts_crit.log"
#define CTS_BUSS_LOG                            "cts_buss.log"

#define LOG_DEFAULT_SIZE                        20480000    //  日志文件大小设置为20M
#define IBPGetParam(name)                       lGetSystem(name)

/*************************************************************************************************
    字符长度定义
 *************************************************************************************************/
#define PROCESS_NAME_LEN                        25
#define KEYWORD_CODE_LEN                        5
#define TARGET_VALUE_LEN                        256
#define PATH_LOGNMAE_LEN                        256
#define MAX_IPADRESS_LEN                        16
#define MARK_NORMAL_LEN                         128    
#define POOL_VALUES_LEN                         512
#define FILED_INDEX_LEN                         32
#define TARGET_NAME_LEN                         30
#define FILED_NORML_LEN                         64
#define LOG_NAMEUSE_LEN                         64
#define EXFUNC_NAME_LEN                         64
#define DATE_STRING_LEN                         9
#define TIME_STRING_LEN                         7
#define TIME_STAMPS_LEN                         20
#define PACKET_ATTR_LEN                         7
#define MAX_LOGBUFF_LEN                         10240
#define MAX_KEYCODE_LEN                         2048
#define EXPRESS_FLD                             5
#define MAX_EXPRESS                             9 
#define SETUP_MAX_SIZE                          5242880		// 5M

/*************************************************************************************************
    报文格式定义    
 *************************************************************************************************/
#define PACK_ADAPTER_TLV                        7   //  TLV报文
#define PACK_ADAPTER_8583                       6   //  8583报文
#define PACK_ADAPTER_JSON                       5   //  json报文
#define PACK_ADAPTER_XML                        4   //  xml报文
#define PACK_ADAPTER_FROM                       3   //  html-from报文
#define PACK_ADAPTER_FIXED                      2   //  定长报文
#define PACK_ADAPTER_SEPAR                      1   //  分隔符报文

#define PACK_CHILD_NODE                         0
#define PACK_PARENT_NODE                        1
#define PACK_PARENT_LOOP                        2


/*************************************************************************************************
    函数宏
 *************************************************************************************************/
#define sGetError()                             sGetTError(lGetTErrno())
#define IBPFree(p)                              if(p) { free(p); p = NULL; }
#define IBPFreeList(p)                          if(p) {    vDestroyList(p);    p = NULL; }
#define IBPIsvariable(x)                        (FIELD_LONG <= x && x <= FILED_SENMAL)
#define MAX(a, b)                               ((a) > (b) ? (a) : (b))
#define MIN(a, b)                               ((a) > (b) ? (b) : (a))

#define IBPenvcheck(x)                          if(!getenv(x))  \
                                                {   \
                                                    fprintf(stderr, "环境变量%s未配置\n", x);   \
                                                    exit(RC_FAIL);  \
                                                }

/*************************************************************************************************
    退出函数调用
*************************************************************************************************/
typedef    void        (*EXITEXEC)(void *argv);

/*************************************************************************************************
    系统内部表结构
 *************************************************************************************************/
typedef struct __CTS_PLAT_PARAM                     /* 系统参数表 */
{
    char    m_szSysName[FILED_NORML_LEN];           //  系统名
    char    m_szCrDate[DATE_STRING_LEN];            //  清算日期
    char    m_szTxDate[DATE_STRING_LEN];            //  交易日期
    llSEQ   m_lSeqNo;                               //  系统流水号
    long    m_lState;                               //  平台状态    1 - 正在运行 2 - 停止    3 -- 进制接入
    long    m_lTrCount;                             //  本周期交易总笔数
    long    m_lSuccCount;                           //  本周期交易成功总笔数
    long    m_lFailCount;                           //  本周期交易失败总笔数
    long    m_lInsaCount;                           //  连续失败次数（后续做故障隔离)
    long    m_lInsaTime;                            //  时间窗口        
    long    m_lUserSize;                            //  变量池大小
    long    m_lPoolNum;                             //  变量池最大并发个数
    long    m_lScanTime;                            //  进程监控时间间隔
    long    m_lQueueTime;                           //  队列监控时间间隔minute
    long    m_lTRTime;                              //  交易超时资源回收最少时间间隔
    long    m_lMaxServers;                          //  最大类型数
    long    m_lMaxProcess;                          //  最大进程数
    long    m_lMaxAsyn;                             //  异步标识最大个数
    long    m_lMaxKey;                              //  用户密码表最大个数
    long    m_lRevsTry;                             //  冲正最大重试次数
    long    m_lQPRatio;                             //  队列进程比(进程自动拓展)
    long    m_lLogLevel;                            //  日志级别
	long	m_lMaster;
    char    m_szUpTime[TIME_STAMPS_LEN];            //  系统启动时间
}CtsParam;

typedef struct __CTS_PLAT_GROUP                     /* 平台进程组信息表 */
{
    char    m_szPGName[PROCESS_NAME_LEN];           //  执行码名称
    long    m_lGrpIdx;                              //  进程组编号
    long    m_lHost;                                //  进程所属主机
    long    m_lType;                                //  进程类型
    long    m_lCurNum;                              //  当前进程个数
    long    m_lMinNum;                              //  最少启动个数
    long    m_lMaxNum;                              //  最大可启动个数
    long    m_lAutoExt;                             //  自动拓展，当系统进程资源不足，可拓展。 
    long    m_lQid;                                 //  消息队列ID
    long    m_lState;                               //  可用状态， 可用/不可用
    long    m_lAppPos;                              //  备用，优化遍历进程索引
    BOOL    m_bIsBoot;                              //  是否接受重启
    BOOL    m_bDbRcon;                              //  进程自动重连
    char    m_szTime[TIME_STAMPS_LEN];              //  启动时间
}CtsGroup;

typedef struct __CTS_PLAT_PROCESS                   /* 进程信息表 */
{
    long    m_lIdx;                                 //  执行码序号
    char    m_szPGName[PROCESS_NAME_LEN];           //  执行码名称
    long    m_lGrpIdx;                              //  进程组编号
    long    m_lHost;                                //  所属主机
    long    m_lType;                                //  进程类型
    long    m_lSeq;                                 //  同类进程组的序号
    long    m_lPid;                                 //  进程PID
    long    m_lState;                               //  进程状态 
    long    m_lPend;                                //  待处理个数
    long    m_lDoNum;                               //  被调用次数
    long    m_lRunExt;                              //  拓展类型
    BOOL    m_bDbRcon;                              //  进程自动重连
}CtsProcess;

typedef struct __CTS_ASYN_IDENTIFY                  /* 异步标识表 */
{
    char    m_szIdx[MAX_INDEX_LEN];                 // 异步标识码
    char    m_szCrDate[DATE_STRING_LEN];            // 账务日期
    llSEQ   m_lSeqNo;                               // 交易流水号
    size_t  m_uData;                                // 共享池位置
    long    m_lFlow;                                // 流程编号
    long    m_lResv;                                // 备注字段
    long    m_lIdxTime;                             // 登记时间
	char	m_szMark[TARGET_VALUE_LEN];
}CtsAsyn;

typedef struct __CTS_TRADE_HOST                     /* 交易运行参数表（主机配置信息）*/
{
    long    m_lHost;                                // 主机编号(从1自增)
    long    m_lEnum;                                // 主机类型
    long    m_lType;                                // 通讯类型
    char    m_szName[MARK_NORMAL_LEN];              // 主机名称
    char    m_szPGName[PROCESS_NAME_LEN];           // 主机执行码
    BOOL    m_bIsBoot;                              // 是否接受被重启
    BOOL    m_bDbRcon;                              // 数据库重连
    long    m_lMinNum;                              // 最少启动个数
    long    m_lMaxNum;                              // 最大可启动个数
    long    m_lAutoExt;                             // 最大可启动个数
    char    m_szDomain[FILED_NORML_LEN];            // 域名
    char    m_szHostIp[MAX_IPADRESS_LEN];           // 主机监听ip
    long    m_lHostPort;                            // 主机监听Port
    long    m_lTimeOut;                             // Host全局超时时间(通讯层)
    char    m_szFtpIp[MAX_IPADRESS_LEN];            // 客户端ftp ip
    long    m_lFtpPort;                             // 客户端ftp port
    char    m_szFtpUser[FILED_NORML_LEN];           // 客户端ftp user
    char    m_szFtpPwd[FILED_NORML_LEN];            // 客户端ftp passwd
    BOOL    m_bLTLink;                              // 连接类型 0:短 1:长
    long    m_lRCTime;                              // 断线重连时间间隔
    char    m_szAuthUser[FILED_NORML_LEN];          // 报文用户名
    char    m_szAuthPwd[FILED_NORML_LEN];           // 报文密码
    char    m_szService[FILED_NORML_LEN];
    long    m_bAsyn;                                // 是否异步通讯
    long    m_lResv;
    long    m_lPreRead;                             // 预读几位
}CtsHost;

typedef struct __CTS_TRADE_CLASS                    /* 功能组件表 */
{
    long    m_lHost;                                // 主机编号(从1自增)
    long    m_lBuss;                                // 组件ID
    long    m_lInMid;                               // 请求报文编号
    long    m_lOutMid;                              // 响应报文编号
    long    m_lErrMid;                              // 处理错误响应报文编号
    long    m_lTimeOut;                             // 模块超时时间
    char    m_szClass[FILED_INDEX_LEN];             // 类名/交易码
    char    m_szMethod[FILED_INDEX_LEN];            // 子方法名/交易字码
    char    m_szMark[MARK_NORMAL_LEN];              // 方法描述
}CtsClass;

typedef struct __CTS_TRADE_CODE                     /* 接入内存交易码对照表*/
{
    long    m_lIdx;                                 // 序号
    char    m_szUse[2];                             // 交易标识，0为管理类交易，1为交易类
    char    m_szInCode[FILED_INDEX_LEN];            // 交易类型(fd0)
    char    m_szInSCode[FILED_INDEX_LEN];           // 交易处理码(fd3)
    char    m_szTrCode[MAX_INDEX_LEN];              // 交易代码
}CtsTrCode;

typedef struct __CTS_TRADE_SERVICE                  /* 系统服务表(记录系统能提供所有服务) */
{
    long    m_lHost;                                // 主机编号(从1自增)
    long    m_lSvrID;                               // 服务编号(从1开始自增)
    char    m_szTrCode[MAX_INDEX_LEN];              // 服务代码
    char    m_szSvrName[MARK_NORMAL_LEN];           // 服务名称
    long    m_lState;                               // 可用状态(0:不可用 1:可用)
    long    m_lTimeOut;                             // 服务超时时间
    long    m_lLogLevel;                            // 服务日志级别
    char    m_szLogName[LOG_NAMEUSE_LEN];           // 服务日志名(内部规则 cts_$SrvCode.log)
    long    m_lSvrType;                             // 服务类型(针对这种交易做特别关注)
    char    m_szRevCode[MAX_INDEX_LEN];             // 冲正服务编号
}CtsService;

typedef struct __EXPRESS
{
    long    m_lConst;                                // 是否表达式类型 0、常数 
    long    m_lField;                                // 条件字段个数
    char    m_szField[EXPRESS_FLD][TARGET_NAME_LEN]; // 表达式变量
    char    m_szResult[TARGET_VALUE_LEN];            // 
    char    m_szIsMatch[TARGET_VALUE_LEN];           // 匹配结果
    char    m_szNotMatch[TARGET_VALUE_LEN];          // 不匹配结果
}Express;

typedef struct __CTS_TRADE_FLOW                      /* 交易流程表 */
{
    long    m_lSvrID;                                // 服务编号
    long    m_lFlow;                                 // 流程编号
    long    m_lType;                                 // 流程类型
    long    m_lHost;                                 // 主机编号(从1自增)    
    long    m_lBuss;                                 // 当前流程的组件ID
    long    m_lInMid;                                // 请求报文编号
    long    m_lOutMid;                               // 响应报文编号
    long    m_lErrMid;                               // 处理错误响应报文编号
    long    m_lNext;                                 // 下级流程编号个数
    long    m_lNextFlow[FILED_NORML_LEN];            // 下级流程编号
    char    m_szClass[FILED_INDEX_LEN];              // 类名
    char    m_szMethod[FILED_INDEX_LEN];             // 子方法名
    char    m_szPGName[PROCESS_NAME_LEN];            // 主机执行码
    long    m_lExp;                                  // 表达式个数
    long    m_lCondit;
    long    m_lAsyn;                                 // 组件异步标识
    char    m_szExp[TARGET_VALUE_LEN];
    Express m_stExp[MAX_EXPRESS];                    // 表达式解析结果
}CtsFlow;

typedef struct __CTS_RETCODE_MAP                     /* 响应码对照表 */
{
	long	m_lIdx;
    char    m_szEntNo[FILED_INDEX_LEN];
    char    m_szInCode[FILED_INDEX_LEN];            // 内部系响应吗
    char    m_szEntCode[FILED_INDEX_LEN];           // 外部系统响应吗
    char    m_szEntMsg[MARK_NORMAL_LEN];            // 外部系统响应信息
    char    m_szLcode[TARGET_NAME_LEN];             // 响应码变量池
    char    m_szLabel[TARGET_NAME_LEN];             // 响应信息变量池
	long    m_lState;                               // 处理状态
}CtsMapCode;

typedef struct __CTS_PACK_CONFIG                    /* 报文配置信息表 */
{
    long    m_lMid;                                 //    配置序号
    long    m_lQid;                                 //    引用子报文ID
    long    m_lPQid;                                //    父节点ID
    long    m_lType;                                //    报文格式类型(8583报文/xml报文/定长报文/分割报文/json)
    long    m_lChild;                               //    子节点个数
    long    m_lIsParant;                            //    为父节点
    long    m_lSeq;                                 //    报文序号 从1开始, 必须使连续的
    long    m_lAttr;                                //    属性(DOUBLE/CHAR/LONG)
    long    m_lPos;                                 //    取值偏移(用于分隔符报文)
    long    m_lFrom;                                //    备用
    long    m_lLen;                                 //    长度
    long    m_lIsConst;                             //    是否为常数
    long    m_lUser;                                //    用户定义处理
    long    m_lPlSet;                               //    设置了变量池    
    long    m_lPlIdx;                               //    下标记
    long    m_lLoop;                                //    循环次数
    long    m_lSLen;                                //    指示长度类型
	long	m_lNilOn;								//    剔除非空字段
    char    m_szAttr[PACKET_ATTR_LEN];              //    用于打印的时候类型输出
    char    m_szLabel[TARGET_NAME_LEN];             //    标签
    char    m_szMark[MARK_NORMAL_LEN];              //    取值中文描述
    char    m_szConst[MARK_NORMAL_LEN];             //    设置常值
    char    m_szPlName[TARGET_NAME_LEN];            //    变量池名称
    char    m_szLoop[TARGET_NAME_LEN];              //    循环次数标签
    char    m_szExFunc[EXFUNC_NAME_LEN];            //    特殊处理函数名称
    char    m_szUserFunc[EXFUNC_NAME_LEN];          //    用户处理函数
}CtsPack;

typedef struct __CTS_ACCESS_CTRL                    /* 接入流控配置表 */
{
	long	m_lHost;
    char    m_szInCode[FILED_INDEX_LEN];            // 类名/交易码
    char    m_szSuCode[FILED_INDEX_LEN];            // 子方法名/交易字码
    char    m_szMark[MARK_NORMAL_LEN];              // 方法描述
	long    m_eCtrl;                                // 流控类型
	char	m_szIp[MAX_IPADRESS_LEN];
	long	m_lTime;                                // 上次接入时间
	long	m_lSecond;                              // 颗粒度
	long	m_lCurCnt;                              // 当前接入笔数
	long	m_lMaxCnt;                              // 最大接入笔数
}CtsCtrl;

typedef struct __CTS_USER_KEYCODE                   /* 用户定义密码表 */
{
    long    m_lIdx;                                 // 序号
    char    m_szUser[MAX_INDEX_LEN];                // 用户名
    char    m_szCode[TARGET_VALUE_LEN];             // 密码备注
    char    m_szKeyP[MAX_KEYCODE_LEN];              // 密钥1
    char    m_szKeyV[MAX_KEYCODE_LEN];              // 密钥2
}CtsUkey;

typedef struct __CTS_MANAGER_USER                   /* 用户配置信息表 */
{
    long    m_lUserGrp;                             //    用户组号
    long    m_lUserID;                              //    用户组序号
    char    m_szUser[FILED_NORML_LEN];              //    用户名
    char    m_szPasswd[FILED_NORML_LEN];            //    密码
    char    m_szLTime[TIME_STAMPS_LEN];             //    上次登录时间
    char    m_szCTime[TIME_STAMPS_LEN];             //    当前登录时间
}CtsUser;


/*************************************************************************************************
     共享池头结点
 *************************************************************************************************/
typedef    struct __CTS_GLOGBL_PARAM                /*    对于交易接入系统，将生成一条跟踪流水 */
{
    long    m_lLen;
    long    m_lStatus;
    llSEQ   m_lSeqNo;                               // 交易流水号
    long    m_lInHost;                              //    接入主机号       
    long    m_lInPid;                               //    接入方进程号
    long    m_lInQid;                               //    接入队列编号
    long    m_lInMid;                               //    服务接入正常请求报文格式
    long    m_lOutMid;                              //    服务接入正常响应报文格式
    long    m_lErrMid;                              //    服务接入错误响应报文格式
    long    m_lInTime;                              //    接入时间
    long    m_lTimeOut;                             //    该服务的超时时间
    long    m_lUseTime;                             //    已用时间
    long    m_lAsyn;                                //    是否存在异步标识
    long    m_lLaunch;                              //    发起的交易来源
    char    m_szInCode[FILED_INDEX_LEN];            //    接入交易码
    char    m_szInSCode[FILED_INDEX_LEN];           //    接入交易字码
    char    m_szTrCode[MAX_INDEX_LEN];              //    服务交易码
    char    m_szCallCode[FILED_INDEX_LEN];          //    调用服务码
    char    m_szCallSub[FILED_INDEX_LEN];           //    调用服务字码
    char    m_szSvrName[MARK_NORMAL_LEN];           //    服务名称
    long    m_lLogLev;                              //    日志级别
    char    m_szLogName[LOG_NAMEUSE_LEN];           //    日志名称(内部规则 cts_SrvCode.log)
    
    long    m_lHost;                                //    当前处理主机号
    long    m_lSvrID;                               //    系统服务编号
    long    m_lResv;                                //    备注字段
    long    m_lPid;                                 //    处理进程号
    long    m_lSType;                               //    服务类型
    long    m_lToMid;                               //    服务的接出正常响应报文格式
    long    m_lToInMid;                             //    服务的接出正常请求报文格式
    long    m_lToErrMid;                            //    服务的接出错误响应报文格式

    char    m_szTxDate[DATE_STRING_LEN];            //    交易日期
    char    m_szCrDate[DATE_STRING_LEN];            //    账务日期
    char    m_szTxTime[TIME_STRING_LEN];            //    交易时间
    long    m_lState;                               //    处理状态
    char    m_szRetCode[KEYWORD_CODE_LEN];
    char    m_szRetMsg[MARK_NORMAL_LEN];
    long    m_lLastState;
    char    m_szLastRet[KEYWORD_CODE_LEN];
    char    m_szLastMsg[MARK_NORMAL_LEN];
    char    m_szParam[TARGET_VALUE_LEN];
}GCts;

typedef struct __CTS_RUN_EVENT                       /*    进程交换信息 */
{
    long    m_lMType;
    llSEQ   m_lSeqNo;
    size_t  m_uData;
    long    m_lFlow;
    long    m_lResv;                                 //  备注字段
    long    m_lState;                                //  事件标识
}CtsEvent;

/*************************************************************************************************
 *     二级缓存    为了提高速度，在应用启动时，参数先初始化
 *************************************************************************************************/
typedef struct  __RUN_INITIAL
{
    long    m_fdChk;                                 //  全局文件句柄检查
    long    m_lUserSize;                             //  全局共享池大小
    long    m_lLogSize;                              //  全局日志最大空间
    char    m_szPath[PATH_LOGNMAE_LEN];              //  全局针对日志路径缓存
    char    m_szMsg[MAX_LOGBUFF_LEN];                //  日志内容最大10K
    char    m_szDate[DATE_STRING_LEN];
    char    m_szBuss[PATH_LOGNMAE_LEN];              //  buss日志

    long    m_lMaxQueue;                             //  队列中最大支持消息个数(超过直接返回)
    long    m_lInQueue;                              //  全局事件输入队列ID
    long    m_lToQueue;                              //  全局事件输出队列ID
    long    m_lTrQueue;                              //  全局交易队列ID
    long    m_lToPos;                                //  全局事件输出队列ID可用进程位置
    long    m_lToNum;                                //  全局事件输出队列ID可用进程个数
    long    m_lMaxPid;                               //  系统最大支持进程编号
	long	m_lMaster;
    BSock   m_skSock;                                //  全局针对socket套接字的保存(主要针对长连接)
    char    m_szPGName[PROCESS_NAME_LEN];            //  全局当前运行执行码名称
    void    *m_pvtData;                              //  全局内存表数据块地址
    char    m_szCrDate[DATE_STRING_LEN];             //  全局系统清算日期
    char    *m_pstBuffer;                            //  全局报文最大缓存空间
}SATInit;

typedef struct  __RUN_PACK_CACHE
{
    long        m_lMid;
    long        m_lMax;
    CtsPack     *m_pstRoot;
}PKCache;

/*************************************************************************************************
    共享池定义
 *************************************************************************************************/
typedef struct  __CTS_ATTR
{
    long    lIdx;                                    // 变量编号(从1自增)
    long    lLen;                                    // 变量值长度
    long    lAttr;                                   // 变量类型
    char    szTarget[TARGET_NAME_LEN];               // 变量名
}CtsAttr;

typedef struct  __CTS_POOL_LIST
{
    CtsAttr stVar;
    char    szValue[TARGET_VALUE_LEN]; 
    struct  __CTS_POOL_LIST *pstNext;
    struct  __CTS_POOL_LIST *pstLast;
}CtsPool;

//handle of cts Module information exchange 
typedef struct __CTS_HWND_TRADE
{
	CtsPool     *m_Roop;
    GCts        m_stCts;
	GCts        m_stBks;
    CtsEvent    m_stEvent;
    char        m_szLog[PATH_LOGNMAE_LEN];               //  日志名
    char        m_szBak[PATH_LOGNMAE_LEN];               //  日志备份名
    long        m_lSysLevel;                             //  全局交易日志检查
    long        m_lSysLevBak;                            //  系统日志级别备份
}HCMIE, *HHCMIE;

#define PONLY_AUTO                             -1
#define PONLY_NRML                             0

/*************************************************************************************************
    日志结构定义
 *************************************************************************************************/
#define LOG_DETAIL                             0x01
#define LOG_DEBUG                              0x02
#define LOG_INFO                               0x03
#define LOG_TRACE                              0x04
#define LOG_MOST                               0x05
#define LOG_ERROR                              0x06
#define LOG_WARM                               0x07
#define LOG_SYSTEM                             0x08
#define LOG_DATABASE                           0x09

#define LOG_DEFAULT_SIZE                       20480000    //  日志文件大小设置为20M

/**************************************************************************************************
    XML宏定义
 **************************************************************************************************/
#define XMLlong(x, t, n, v)          snprintf(t, sizeof(t), "%d", v); xmlNewTextChild(x, NULL, (xmlChar *)n, t);
#define XMLlongattr(x, t, n, v)      snprintf(t, sizeof(t), "%d", v); xmlNewProp(x, (xmlChar *)n, t);
#define XMLstring(x, t, n, v)        xmlNewTextChild(x, NULL, (xmlChar *)n, v);

/*************************************************************************************************
    内部函数定义
 *************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
extern  void        vPrintSystem();
extern  char*       sGetFlowStr(long lNum, long *plCGroup);

#ifdef __cplusplus
}
#endif

#endif    //    __CTS_PLAT_DEFINE__HHH___
