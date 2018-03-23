/************************************************************************************************** 
    文 件 名：boot.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0
    创建日期：2016-07-14
     功能描述：cts系统启停操作
 **************************************************************************************************/
#include    "face.h"

/**************************************************************************************************
    配置解析结构
 **************************************************************************************************/
typedef struct __CONFIG_ATTR
{
    char    m_szAttr[TARGET_NAME_LEN];
    char    m_szValue[TARGET_VALUE_LEN];
    struct __CONFIG_ATTR    *pstNext;
}CfgAttr;

typedef struct __CONFIG_HEAD
{
    char    m_szName[TARGET_VALUE_LEN];
    CfgAttr *m_pstAttr;
    struct __CONFIG_HEAD    *pstNext;
}CfgHead;

/**************************************************************************************************
    表结构定义
 **************************************************************************************************/
/**************************************************************************************************
    函 数 名：定义表CTS_PLAT_PARAM表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_PLAT_PARAM(TABLE T)
{
    DEFINE(T, "CTS_PLAT_PARAM", SYSTEM_NAME, CtsParam)
    FIELD(CtsParam, m_szSysName,       "m_szSysName",                FIELD_CHAR)
    FIELD(CtsParam, m_szCrDate,        "m_szCrDate",                 FIELD_CHAR)
    FIELD(CtsParam, m_szTxDate,        "m_szTxDate",                 FIELD_CHAR)
    FIELD(CtsParam, m_lSeqNo,          "m_lSeqNo",                   FIELD_LONG)
    FIELD(CtsParam, m_lState,          "m_lState",                   FIELD_LONG)
    FIELD(CtsParam, m_lTrCount,        "m_lTrCount",                 FIELD_LONG)
    FIELD(CtsParam, m_lSuccCount,      "m_lSuccCount",               FIELD_LONG)
    FIELD(CtsParam, m_lFailCount,      "m_lFailCount",               FIELD_LONG)
    FIELD(CtsParam, m_lInsaCount,      "m_lInsaCount",               FIELD_LONG)
    FIELD(CtsParam, m_lInsaTime,       "m_lInsaTime",                FIELD_LONG)
    FIELD(CtsParam, m_lUserSize,       "m_lUserSize",                FIELD_LONG)
    FIELD(CtsParam, m_lPoolNum,        "m_lPoolNum",                 FIELD_LONG)
    FIELD(CtsParam, m_lScanTime,       "m_lScanTime",                FIELD_LONG)
    FIELD(CtsParam, m_lQueueTime,      "m_lQueueTime",               FIELD_LONG)
    FIELD(CtsParam, m_lTRTime,         "m_lTRTime",                  FIELD_LONG)
    FIELD(CtsParam, m_lMaxServers,     "m_lMaxServers",              FIELD_LONG)
    FIELD(CtsParam, m_lMaxProcess,     "m_lMaxProcess",              FIELD_LONG)
    FIELD(CtsParam, m_lMaxAsyn,        "m_lMaxAsyn",                 FIELD_LONG)
    FIELD(CtsParam, m_lQPRatio,        "m_lQPRatio",                 FIELD_LONG)
    FIELD(CtsParam, m_lLogLevel,       "m_lLogLevel",                FIELD_LONG)
    FIELD(CtsParam, m_lMaster,         "m_lMaster",                  FIELD_LONG)
    FIELD(CtsParam, m_szUpTime,        "m_szUpTime",                 FIELD_CHAR)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsParam, m_szSysName,     FIELD_CHAR);

    FINISH
}

/**************************************************************************************************
    函 数 名：定义表CTS_PLAT_GROUP表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE C_CTS_PLAT_GROUP(T)
{
    DEFINE(T, "CTS_PLAT_GROUP", SYSTEM_NAME, CtsGroup)
    FIELD(CtsGroup, m_szPGName,        "m_szPGName",                 FIELD_CHAR)
    FIELD(CtsGroup, m_lGrpIdx,         "m_lGrpIdx",                  FIELD_LONG)
    FIELD(CtsGroup, m_lHost,           "m_lHost",                    FIELD_LONG)
    FIELD(CtsGroup, m_lType,           "m_lType",                    FIELD_LONG)
    FIELD(CtsGroup, m_lCurNum,         "m_lCurNum",                  FIELD_LONG)
    FIELD(CtsGroup, m_lMinNum,         "m_lMinNum",                  FIELD_LONG)
    FIELD(CtsGroup, m_lMaxNum,         "m_lMaxNum",                  FIELD_LONG)
    FIELD(CtsGroup, m_lAutoExt,        "m_lAutoExt",                 FIELD_LONG)
    FIELD(CtsGroup, m_lQid,            "m_lQid",                     FIELD_LONG)
    FIELD(CtsGroup, m_lState,          "m_lState",                   FIELD_LONG)
    FIELD(CtsGroup, m_lAppPos,         "m_lAppPos",                  FIELD_LONG)
    FIELD(CtsGroup, m_bIsBoot,         "m_bIsBoot",                  FIELD_LONG)
    FIELD(CtsGroup, m_bDbRcon,         "m_bDbRcon",                  FIELD_LONG)
    FIELD(CtsGroup, m_szTime,          "m_szTime",                   FIELD_CHAR)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsGroup, m_szPGName,     FIELD_CHAR);
    IDX_FIELD(CtsGroup, m_lHost,        FIELD_LONG);

    FINISH
}

/**************************************************************************************************
    函 数 名：定义表CTS_PLAT_PROCESS表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_PLAT_PROCESS(T)
{
    DEFINE(T, "CTS_PLAT_PROCESS", SYSTEM_NAME, CtsProcess)
    FIELD(CtsProcess, m_lIdx,          "m_lIdx",                     FIELD_LONG)
    FIELD(CtsProcess, m_szPGName,      "m_szPGName",                 FIELD_CHAR)
    FIELD(CtsProcess, m_lGrpIdx,       "m_lGrpIdx",                  FIELD_LONG)
    FIELD(CtsProcess, m_lHost,         "m_lHost",                    FIELD_LONG)
    FIELD(CtsProcess, m_lType,         "m_lType",                    FIELD_LONG)
    FIELD(CtsProcess, m_lSeq,          "m_lSeq",                     FIELD_LONG)
    FIELD(CtsProcess, m_lPid,          "m_lPid",                     FIELD_LONG)
    FIELD(CtsProcess, m_lState,        "m_lState",                   FIELD_LONG)
    FIELD(CtsProcess, m_lPend,         "m_lPend",                    FIELD_LONG)
    FIELD(CtsProcess, m_lDoNum,        "m_lDoNum",                   FIELD_LONG)
    FIELD(CtsProcess, m_lRunExt,       "m_lRunExt",                  FIELD_LONG)
    FIELD(CtsProcess, m_bDbRcon,       "m_bDbRcon",                  FIELD_LONG)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsProcess, m_szPGName,   FIELD_CHAR)
    IDX_FIELD(CtsProcess, m_lSeq,       FIELD_LONG)
    IDX_FIELD(CtsProcess, m_lHost,      FIELD_LONG)

    CREATE_IDX(NORMAL)
    IDX_FIELD(CtsProcess, m_szPGName,   FIELD_CHAR)

    FINISH
}

/**************************************************************************************************
    函 数 名：定义表CTS_ASYN_IDENTIFY表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_ASYN_IDENTIFY(T)
{
    DEFINE(T, "CTS_ASYN_IDENTIFY", SYSTEM_NAME, CtsAsyn)
    FIELD(CtsAsyn, m_szIdx,            "m_szIdx",                    FIELD_CHAR)    
    FIELD(CtsAsyn, m_szCrDate,         "m_szCrDate",                 FIELD_CHAR)    
    FIELD(CtsAsyn, m_lSeqNo,           "m_lSeqNo",                   FIELD_LONG)    
    FIELD(CtsAsyn, m_uData,            "m_uData",                    FIELD_LONG)    
    FIELD(CtsAsyn, m_lFlow,            "m_lFlow",                    FIELD_LONG)    
    FIELD(CtsAsyn, m_lResv,            "m_lResv",                    FIELD_LONG)    
    FIELD(CtsAsyn, m_lIdxTime,         "m_lChkTime",                 FIELD_LONG)    
    FIELD(CtsAsyn, m_lIdxTime,         "m_lChkTime",                 FIELD_LONG)    
    FIELD(CtsAsyn, m_szMark,           "m_szMark",                   FIELD_CHAR)    

    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsAsyn, m_szIdx,      FIELD_CHAR)

    CREATE_IDX(NORMAL)
    IDX_FIELD(CtsAsyn, m_szCrDate,   FIELD_CHAR)

    FINISH
}

/**************************************************************************************************
    函 数 名：定义表CTS_TRADE_HOST表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_TRADE_HOST(T)
{
    DEFINE(T, "CTS_TRADE_HOST", SYSTEM_NAME, CtsHost)
    FIELD(CtsHost, m_lHost,            "m_lHost",                    FIELD_LONG)
    FIELD(CtsHost, m_lEnum,            "m_lEnum",                    FIELD_LONG)
    FIELD(CtsHost, m_lType,            "m_lComType",                 FIELD_LONG)
    FIELD(CtsHost, m_szName,           "m_szName",                   FIELD_CHAR)
    FIELD(CtsHost, m_szPGName,         "m_szPGName",                 FIELD_CHAR)
    FIELD(CtsHost, m_bIsBoot,          "m_bIsBoot",                  FIELD_LONG)
    FIELD(CtsHost, m_bDbRcon,          "m_bDbRcon",                  FIELD_LONG)
    FIELD(CtsHost, m_lMinNum,          "m_lMinNum",                  FIELD_LONG)
    FIELD(CtsHost, m_lMaxNum,          "m_lMaxNum",                  FIELD_LONG)
    FIELD(CtsHost, m_lAutoExt,         "m_lAutoExt",                 FIELD_LONG)
    FIELD(CtsHost, m_szDomain,         "m_szDomain",                 FIELD_CHAR)
    FIELD(CtsHost, m_szHostIp,         "m_szHostIp",                 FIELD_CHAR)
    FIELD(CtsHost, m_lHostPort,        "m_lHostPort",                FIELD_LONG)
    FIELD(CtsHost, m_lTimeOut,         "m_lTimeOut",                 FIELD_LONG)
    FIELD(CtsHost, m_szFtpIp,          "m_szFtpIp",                  FIELD_CHAR)
    FIELD(CtsHost, m_lFtpPort,         "m_lFtpPort",                 FIELD_LONG)
    FIELD(CtsHost, m_szFtpUser,        "m_szFtpUser",                FIELD_CHAR)
    FIELD(CtsHost, m_szFtpPwd,         "m_szFtpPwd",                 FIELD_CHAR)
    FIELD(CtsHost, m_bLTLink,          "m_bLTLink",                  FIELD_LONG)
    FIELD(CtsHost, m_lRCTime,          "m_lRCTime",                  FIELD_LONG)
    FIELD(CtsHost, m_szAuthUser,       "m_szAuthUser",               FIELD_CHAR)
    FIELD(CtsHost, m_szAuthPwd,        "m_szAuthPwd",                FIELD_CHAR)
    FIELD(CtsHost, m_szService,        "m_szService",                FIELD_CHAR)
    FIELD(CtsHost, m_bAsyn,            "m_bAsyn",                    FIELD_LONG)
    FIELD(CtsHost, m_lResv,            "m_lResv",                    FIELD_LONG)
    FIELD(CtsHost, m_lPreRead,         "m_lPreRead",                 FIELD_LONG)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsHost, m_lHost,   FIELD_LONG)

    FINISH
}

/**************************************************************************************************
    函 数 名：定义表CTS_TRADE_CLASS表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_TRADE_CLASS(T)
{
    DEFINE(T, "CTS_TRADE_CLASS", SYSTEM_NAME, CtsClass) 
    FIELD(CtsClass, m_lHost,           "m_lHost",                    FIELD_LONG)
    FIELD(CtsClass, m_lBuss,           "m_lBuss",                    FIELD_LONG)
    FIELD(CtsClass, m_lInMid,          "m_lInMid",                   FIELD_LONG)
    FIELD(CtsClass, m_lOutMid,         "m_lOutMid",                  FIELD_LONG)
    FIELD(CtsClass, m_lErrMid,         "m_lErrMid",                  FIELD_LONG)
    FIELD(CtsClass, m_lTimeOut,        "m_lTimeOut",                 FIELD_LONG)
    FIELD(CtsClass, m_szClass,         "m_szClass",                  FIELD_CHAR)
    FIELD(CtsClass, m_szMethod,        "m_szMethod",                 FIELD_CHAR)
    FIELD(CtsClass, m_szMark,          "m_szMark",                   FIELD_CHAR)
    
    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsClass, m_szClass,   FIELD_CHAR)
    IDX_FIELD(CtsClass, m_szMethod,  FIELD_CHAR)

    CREATE_IDX(HASHID)
    IDX_FIELD(CtsClass, m_lBuss,     FIELD_LONG)

    FINISH   
}

/**************************************************************************************************
    函 数 名：定义表CTS_TRADE_CLASS表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_TRADE_CODE(T) 
{
    DEFINE(T, "CTS_TRADE_CODE", SYSTEM_NAME, CtsTrCode) 
    FIELD(CtsTrCode, m_lIdx,           "m_lIdx",                     FIELD_LONG)         
    FIELD(CtsTrCode, m_szUse,          "m_szUse",                    FIELD_CHAR)
    FIELD(CtsTrCode, m_szInCode,       "m_szInCode",                 FIELD_CHAR)
    FIELD(CtsTrCode, m_szInSCode,      "m_szInSCode",                FIELD_CHAR)
    FIELD(CtsTrCode, m_szTrCode,       "m_szTrCode",                 FIELD_CHAR)
                                                                              
    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsTrCode, m_szInCode,   FIELD_CHAR)
    IDX_FIELD(CtsTrCode, m_szInSCode,  FIELD_CHAR)

    CREATE_IDX(HASHID)
    IDX_FIELD(CtsTrCode, m_lIdx,       FIELD_LONG)

    FINISH                                                                    
}

/**************************************************************************************************
    函 数 名：定义表CTS_TRADE_SERVICE表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_TRADE_SERVICE(T)
{                                                                             
    DEFINE(T, "CTS_TRADE_SERVICE", SYSTEM_NAME, CtsService)
    FIELD(CtsService, m_lHost,         "m_lHost",                    FIELD_LONG)                            
    FIELD(CtsService, m_lSvrID,        "m_lSvrID",                   FIELD_LONG)
    FIELD(CtsService, m_szTrCode,      "m_szTrCode",                 FIELD_CHAR)
    FIELD(CtsService, m_szSvrName,     "m_szSvrName",                FIELD_CHAR)
    FIELD(CtsService, m_lState,        "m_lState",                   FIELD_LONG)
    FIELD(CtsService, m_lTimeOut,      "m_lTimeOut",                 FIELD_LONG)
    FIELD(CtsService, m_lLogLevel,     "m_lLogLevel",                FIELD_LONG)
    FIELD(CtsService, m_szLogName,     "m_szLogName",                FIELD_CHAR)
    FIELD(CtsService, m_lSvrType,      "m_lSvrType",                 FIELD_LONG)
    FIELD(CtsService, m_szRevCode,     "m_szRevCode",                FIELD_CHAR)
                                                                              
    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsService, m_szTrCode,   FIELD_CHAR)

    CREATE_IDX(NORMAL)
    IDX_FIELD(CtsService, m_lHost,      FIELD_LONG)

    FINISH                                                                    
}

/**************************************************************************************************
    函 数 名：定义表CTS_TRADE_FLOW表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_TRADE_FLOW(T)                                                   
{                                                                             
    DEFINE(T, "CTS_TRADE_FLOW", SYSTEM_NAME, CtsFlow)                                  
    FIELD(CtsFlow, m_lSvrID,           "m_lSvrID",                   FIELD_LONG)
    FIELD(CtsFlow, m_lFlow,            "m_lFlow",                    FIELD_LONG)
    FIELD(CtsFlow, m_lType,            "m_lType",                    FIELD_LONG)
    FIELD(CtsFlow, m_lHost,            "m_lHost",                    FIELD_LONG)
    FIELD(CtsFlow, m_lBuss,            "m_lBuss",                    FIELD_LONG)
    FIELD(CtsFlow, m_lInMid,           "m_lInMid",                   FIELD_LONG)
    FIELD(CtsFlow, m_lOutMid,          "m_lOutMid",                  FIELD_LONG)
    FIELD(CtsFlow, m_lErrMid,          "m_lErrMid",                  FIELD_LONG)
    FIELD(CtsFlow, m_lNext,            "m_lNext",                    FIELD_LONG)
    FIELD(CtsFlow, m_lNextFlow,        "m_lNextFlow",                FIELD_LONG)
    FIELD(CtsFlow, m_szClass,          "m_szClass",                  FIELD_CHAR)
    FIELD(CtsFlow, m_szMethod,         "m_szMethod",                 FIELD_CHAR)
    FIELD(CtsFlow, m_szPGName,         "m_szPGName",                 FIELD_CHAR)
    FIELD(CtsFlow, m_lExp,             "m_lExp",                     FIELD_LONG)
    FIELD(CtsFlow, m_lCondit,          "m_lCondit",                  FIELD_LONG)
    FIELD(CtsFlow, m_lAsyn,            "m_lAsyn",                    FIELD_LONG)
    FIELD(CtsFlow, m_szExp,            "m_szExp",                    FIELD_CHAR)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsFlow, m_lSvrID,       FIELD_LONG)
    IDX_FIELD(CtsFlow, m_lFlow,        FIELD_LONG)

    CREATE_IDX(NORMAL)
    IDX_FIELD(CtsFlow, m_lSvrID,       FIELD_LONG)

    FINISH
}

/**************************************************************************************************
    函 数 名：定义表CTS_RETCODE_MAP表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_RETCODE_MAP(T)
{
    DEFINE(T, "CTS_RETCODE_MAP", SYSTEM_NAME, CtsMapCode) 
    FIELD(CtsMapCode, m_lIdx,          "m_lIdx",                      FIELD_LONG)
    FIELD(CtsMapCode, m_szEntNo,       "m_szEntNo",                   FIELD_CHAR)
    FIELD(CtsMapCode, m_szInCode,      "m_szInCode",                  FIELD_CHAR)
    FIELD(CtsMapCode, m_szEntCode,     "m_szEntCode",                 FIELD_CHAR)
    FIELD(CtsMapCode, m_szEntMsg,      "m_szEntMsg",                  FIELD_CHAR)
    FIELD(CtsMapCode, m_szLcode,       "m_szLcode",                   FIELD_CHAR)
    FIELD(CtsMapCode, m_szLabel,       "m_szLabel",                   FIELD_CHAR)
    FIELD(CtsMapCode, m_lState,        "m_lState",                    FIELD_LONG)
    
    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsMapCode, m_szEntNo,     FIELD_CHAR)
    IDX_FIELD(CtsMapCode, m_szInCode,    FIELD_CHAR)

    CREATE_IDX(HASHID)
    IDX_FIELD(CtsMapCode, m_lIdx,        FIELD_LONG)

    FINISH
}  

/**************************************************************************************************
    函 数 名：定义表CTS_PACK_CONFIG表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_PACK_CONFIG(T)
{
    DEFINE(T, "CTS_PACK_CONFIG", SYSTEM_NAME, CtsPack)   
    FIELD(CtsPack, m_lMid,             "m_lMid",                     FIELD_LONG)    
    FIELD(CtsPack, m_lQid,             "m_lQid",                     FIELD_LONG)
    FIELD(CtsPack, m_lPQid,            "m_lPQid",                    FIELD_LONG)
    FIELD(CtsPack, m_lType,            "m_lType",                    FIELD_LONG)
    FIELD(CtsPack, m_lChild,           "m_lChild",                   FIELD_LONG)
    FIELD(CtsPack, m_lIsParant,        "m_lIsParant",                FIELD_LONG)
    FIELD(CtsPack, m_lSeq,             "m_lSeq",                     FIELD_LONG)
    FIELD(CtsPack, m_lAttr,            "m_lAttr",                    FIELD_LONG)
    FIELD(CtsPack, m_lPos,             "m_lPos",                     FIELD_LONG)
    FIELD(CtsPack, m_lFrom,            "m_lFrom",                    FIELD_LONG)
    FIELD(CtsPack, m_lLen,             "m_lLen",                     FIELD_LONG)
    FIELD(CtsPack, m_lIsConst,         "m_lIsConst",                 FIELD_LONG)
    FIELD(CtsPack, m_lUser,            "m_lUser",                    FIELD_LONG)
    FIELD(CtsPack, m_lPlSet,           "m_lPlSet",                   FIELD_LONG)
    FIELD(CtsPack, m_lPlIdx,           "m_lPlIdx",                   FIELD_LONG)
    FIELD(CtsPack, m_lLoop,            "m_lLoop",                    FIELD_LONG)
    FIELD(CtsPack, m_lSLen,            "m_lSLen",                    FIELD_LONG)                                                      
    FIELD(CtsPack, m_lNilOn,           "m_lNilOn",                   FIELD_LONG)                                                      
    FIELD(CtsPack, m_szAttr,           "m_szAttr",                   FIELD_CHAR)
    FIELD(CtsPack, m_szLabel,          "m_szLabel",                  FIELD_CHAR)
    FIELD(CtsPack, m_szMark,           "m_szMark",                   FIELD_CHAR)
    FIELD(CtsPack, m_szConst,          "m_szConst",                  FIELD_CHAR)
    FIELD(CtsPack, m_szPlName,         "m_szPlName",                 FIELD_CHAR)
    FIELD(CtsPack, m_szLoop,           "m_szLoop",                   FIELD_CHAR)
    FIELD(CtsPack, m_szExFunc,         "m_szExFunc",                 FIELD_CHAR)
    FIELD(CtsPack, m_szUserFunc,       "m_szUserFunc",               FIELD_CHAR)
           
    CREATE_IDX(NORMAL)
    IDX_FIELD(CtsPack, m_lMid,       FIELD_LONG)
    IDX_FIELD(CtsPack, m_lSeq,       FIELD_LONG)

    FINISH
}

/**************************************************************************************************
    函 数 名：定义表CTS_MANAGER_USER表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_MANAGER_USER(T)
{
    DEFINE(T, "CTS_MANAGER_USER", SYSTEM_NAME, CtsUser)
    FIELD(CtsUser, m_lUserGrp,         "m_lUserGrp",                 FIELD_LONG)                                   
    FIELD(CtsUser, m_lUserID,          "m_lUserID",                  FIELD_LONG)  
    FIELD(CtsUser, m_szUser,           "m_szUser",                   FIELD_CHAR)  
    FIELD(CtsUser, m_szPasswd,         "m_szPasswd",                 FIELD_CHAR)  
    FIELD(CtsUser, m_szLTime,          "m_szLTime",                  FIELD_CHAR)  
    FIELD(CtsUser, m_szCTime,          "m_szCTime",                  FIELD_CHAR)  
    
    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsUser, m_szUser,       FIELD_CHAR)

    FINISH
}  

/**************************************************************************************************
    函 数 名：定义表CTS_USER_KEYCODE表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE  C_CTS_USER_KEYCODE(T)                                                   
{                                                                             
    DEFINE(T, "CTS_USER_KEYCODE", SYSTEM_NAME, CtsUkey)                                  
    FIELD(CtsUkey, m_lIdx,           "m_lIdx",                 FIELD_LONG)
    FIELD(CtsUkey, m_szUser,         "m_szUser",               FIELD_CHAR)
    FIELD(CtsUkey, m_szCode,         "m_szCode",               FIELD_CHAR)
    FIELD(CtsUkey, m_szKeyP,         "m_szKeyP",               FIELD_CHAR)
    FIELD(CtsUkey, m_szKeyV,         "m_szKeyV",               FIELD_CHAR)

    CREATE_IDX(UNQIUE)
    IDX_FIELD(CtsUkey, m_szUser,      FIELD_CHAR)

    CREATE_IDX(HASHID)
    IDX_FIELD(CtsUkey, m_lIdx,        FIELD_LONG)

    FINISH
}

/**************************************************************************************************
    函 数 名：定义表CTS_RUNTIME_POOL表结构
    功能描述：
    返回说明：
 **************************************************************************************************/
CREATE    C_CTS_RUNTIME_POOL(T)
{
    DEFINE(T, "CTS_RUNTIME_POOL", sGetDate(), GCts)
    FIELD(GCts, m_lStatus,                 "m_lStatus",        FIELD_LONG)
    FIELD(GCts, m_lSeqNo,                  "m_lSeqNo",         FIELD_LONG)
    FIELD(GCts, m_lInHost,                 "m_lInHost",        FIELD_LONG)
    FIELD(GCts, m_szInCode,                "m_szInCode",       FIELD_CHAR)
    FIELD(GCts, m_szInSCode,               "m_szInSCode",      FIELD_CHAR)
    FIELD(GCts, m_szTrCode,                "m_szTrCode",       FIELD_CHAR)

    ((TblDef *)pGetTblDef(T))->m_lReSize = sizeof(GCts) + lGetUserSize();
    ((TblDef *)pGetTblDef(T))->m_lExtern = ((TblDef *)pGetTblDef(T))->m_lData;
    ((TblDef *)pGetTblDef(T))->m_lTruck  = ((TblDef *)pGetTblDef(T))->m_lReSize + sizeof(SHTruck);

    FINISH
}

/**************************************************************************************************
 * 表结构定义完毕
 **************************************************************************************************/
/**************************************************************************************************
    函 数 名：sExpressLegal(char *p)
    功能描述：检查表达式规则
    返回说明：
        s                --检查完毕后的字符串
 **************************************************************************************************/
char*    sExpressLegal(char *p)
{
    BOOL    bf = false;
    int     i, n, j, k, m;

    if(!p || 0 == (n = strlen(p)))
        return NULL;

    for(i = 0, j = 0, k = 0; i < n; i ++)
    {
        if(p[i] == '\"')
        {
            bf = !bf;
            continue;
        }

        if(bf)    continue;

        if(p[i] == '(')
            j ++;
        if(p[i] == ')')
            k ++;
    }

    if(j != k) return NULL;

    return strimfield(p);
}

/**************************************************************************************************
    函 数 名：sExpreplace(char *r, char *s, char *d)
    功能描述：表达式中函数替换
    返回说明：
        s                --替换完毕后的字符串
 **************************************************************************************************/
char*    sExpreplace(char *r, char *s, char *d)
{
    long    l, i, n;
    char    *p = strstr(r, s);

    if(!p || 0 == (l = strlen(s)) || 0 == (n = strlen(d)))
        return r;

    for(i = n, memcpy(p, d, n); i < l; i ++)
        p[i] = 0x20;

    if(n == strlen(strimfield(r)))
        memset(r, 0, n);

    return r;
}

/**************************************************************************************************
    函 数 名：sGetExpField(char *s, char *o)
    功能描述：获取表达式中的变量
    返回说明：
        s                --替换完毕后的字符串
 **************************************************************************************************/
char*    sGetExpField(char *s, char *o)
{
    char    *p;
    long    i, n, m;
    BOOL    bf = false;

    if(!s || NULL == (p = strstr(s, "(")))
        return o;

    for(i = 0, m = 0, p += 1, n = strlen(p); i < n; i ++)
    {
        if(p[i] == '\"')
            bf = !bf;
        if(')' == p[i])
            break;

        if(bf)  o[m ++] = p[i];
        else if(p[i] != ' ' && p[i] != '\t')
            o[m ++] = p[i];
    }

    o[m] = 0x00;

    return s;
}

/**************************************************************************************************
    函 数 名：_lInitTruck(CMList **pstRoot, const char *pszTarget, char *pszFile)
    功能描述：初始化启动配置
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lInitTruck(CMList **pstRoot, const char *pszTarget, char *pszFile)
{
    CMList  *pstList = NULL;
    char    szLine[512], szTarg[30];
    FILE    *fp = NULL;
    BOOL    bFlag = FALSE;

    memset(szTarg, 0, sizeof(szTarg));
    snprintf(szTarg, sizeof(szTarg), "%s>", pszTarget);

    if(NULL == (fp = fopen(pszFile, "rb")))
    {
        fprintf(stderr, "打开配置文件(%s)失败, err:%s", pszFile, strerror(errno));
        return RC_FAIL;
    }
    
    memset(szLine, 0, sizeof(szLine));
    while(fgets(szLine, sizeof(szLine), fp))
    {
        strimcrlf(szLine);    
        strimall(szLine);    
        if(!strlen(szLine))        continue;

        if('#' == szLine[0] || !memcmp("//", szLine, 2) || !memcmp("/*", szLine, 2) || 
            !memcmp("＃", szLine, 2) || !memcmp("--", szLine, 2))
            continue;

        if('<' == szLine[0] && !strcmp(szTarg, szLine + 1))
        {
            bFlag = TRUE;
            memset(szLine, 0, sizeof(szLine));
            continue;
        }
        else if(!strncmp("</", szLine, 2) && !strcmp(szTarg, szLine + 2))
            break;

        if(!bFlag)
        {
            memset(szLine, 0, sizeof(szLine));
            continue;
        }
        
        if(!(pstList = pInsertList(pstList, szLine, sizeof(szLine))))
        {
            fclose(fp);
            vDestroyList(pstList);
            IBPerror("加载配置文件(%s)域(%s)失败", pszFile, pszTarget);
            return RC_FAIL;
        }
    }
    fclose(fp);

    *pstRoot = pstList;

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_pParseConfig(SATvm *pstSavm, CfgAttr *pstRoot, char *pszAttr)
    功能描述：解析属性
    返回说明：
        CfgAttr        返回CfgAttr根结点
 **************************************************************************************************/
CfgAttr    *_pParseConfig(SATvm *pstSavm, CfgAttr *pstRoot, char *pszAttr)
{
    char    *p = NULL;
    CfgAttr    *pstAttr = NULL, *pstTail = pstRoot;

    if((p = strstr(pszAttr, "--")))
        pszAttr[p - pszAttr] = 0x00;

    while(pstTail)
    {
        if(!pstTail->pstNext)
            break;
        pstTail = pstTail->pstNext;
    }    

    if(NULL == (pstAttr = (CfgAttr *)malloc(sizeof(CfgAttr))))
    {
        IBPerror("创建配置内存空间(%d)失败, err:%s", sizeof(CfgAttr), strerror(errno));
        return pstRoot;
    }

    sDropCharset(pszAttr, '\"');
    memset(pstAttr, 0, sizeof(CfgAttr));
    strncpy(pstAttr->m_szAttr, sGetValueByIdx(pszAttr, "=", 1), sizeof(pstAttr->m_szAttr));
    strncpy(pstAttr->m_szValue, sGetValueByIdx(pszAttr, "=", 2), sizeof(pstAttr->m_szValue));
    strimall(pstAttr->m_szAttr);
    strimall(pstAttr->m_szValue);

    if(!pstRoot)
        pstRoot = pstAttr;
    else
        pstTail->pstNext = pstAttr;

    return pstRoot;
}

/**************************************************************************************************
    函 数 名：lParseConfig(SATvm *pstSavm, CfgHead **pstRoot, const char *pszTarget)
    功能描述：读取指定区域配置
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lParseConfig(SATvm *pstSavm, CfgHead **pstRoot, const char *pszTarget)
{
    long    i = 0;
    BOOL    bFlag = FALSE;
    char    szLine[512], szPath[512], szAttr[256];
    char    *p = NULL, *q = NULL;
    FILE    *fp = NULL;
    CfgHead    *pstNode = NULL;

    memset(szPath, 0, sizeof(szPath));
    snprintf(szPath, sizeof(szPath), "%s", getenv("CTSCFG"));
    if(NULL == (fp = fopen(szPath, "rb")))
    {
        fprintf(stderr, "打开配置文件(%s)失败, err:%s", szPath, strerror(errno));
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

        if('*' == szLine[0] || !bFlag)
        {
            if(bFlag)        break;
            else if(!strcmp(pszTarget, szLine) && !bFlag)
            {
                bFlag = TRUE;
                if(!(pstNode = (CfgHead *)calloc(1, sizeof(CfgHead))))
                {
                    fclose(fp);
                    return RC_FAIL;
                }

                strncpy(pstNode->m_szName, pszTarget, sizeof(pstNode->m_szName) - 1);
                *pstRoot = pstNode;
            }
            else
                ;

            memset(szLine, 0, sizeof(szLine));
            continue;
        }
        
        p = sCharReplace(szLine, '\t', ' ');    
        if('\"' == szLine[0] || '\'' == szLine[0])    //    子选项
        {
            if(!(pstNode->pstNext = (CfgHead *)calloc(1, sizeof(CfgHead))))
            {
                fclose(fp);
                return RC_FAIL;
            }

            pstNode = pstNode->pstNext;
            strncpy(pstNode->m_szName, sGetValueByIdx(szLine, " ", 1), sizeof(pstNode->m_szName));
            p = szLine + strlen(pstNode->m_szName);
            sDropCharset(pstNode->m_szName, '\"');
            p = sltrim(p);
        }
        else
        {
            if(!(pstNode->m_pstAttr = _pParseConfig(pstSavm, pstNode->m_pstAttr, szLine)))
            {
                fclose(fp);
                return RC_FAIL;
            }

            continue;
        }

        for(i = lGetValidNum(p, " "); i > 0; i --)
        {
            memset(szAttr, 0, sizeof(szAttr));
            strncpy(szAttr, sGetValueByIdx(p, " ", i), sizeof(szAttr));
            if(!strlen(szAttr))    continue;
            if(!(q = strstr(szAttr, "=")))
            {
                fprintf(stderr, "解析(%s)失败\n", p);
                fclose(fp);
                return RC_FAIL;
            }

            if(!(pstNode->m_pstAttr = _pParseConfig(pstSavm, pstNode->m_pstAttr, szAttr)))
            {
                fclose(fp);
                return RC_FAIL;
            }
        }
        memset(szLine, 0, sizeof(szLine));
    }
    fclose(fp);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lGetConfig(CfgHead *pstRoot, const char *, const char *, long eType, char *pszOut)
    功能描述：获取配置信息
    返回说明：
        RC_SUCC            --成功
        RC_NOTFOUND        --无记录
 **************************************************************************************************/
long    lGetConfig(CfgAttr *pstARoot, const char *pszAttr, long eType, void *pvData)
{
    CfgAttr    *pstAttr = pstARoot;
    
    while(pstAttr)
    {
        if(!strcmp(pstAttr->m_szAttr, pszAttr))
            break;
        pstAttr = pstAttr->pstNext;
    }
    if(!pstAttr)    return RC_NOTFOUND;

    if(FIELD_NUMBER == eType)
        *((long *)pvData) = atol(pstAttr->m_szValue);
    else if(FIELD_DOUBLE == eType)
        *((double *)pvData) = atof(pstAttr->m_szValue);
    else if(FIELD_STRING == eType)
        strcpy((char *)pvData, pstAttr->m_szValue);
    
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vCleanConfig(CfgHead *pstRoot)
    功能描述：清理加载的缓存
    返回说明：
        无
 **************************************************************************************************/
void    vCleanConfig(CfgHead *pstRoot)
{
    CfgAttr    *pstAttr = NULL;
    CfgHead    *pstList = NULL;

    while(pstRoot)
    {
        pstList = pstRoot;
        pstRoot = pstRoot->pstNext;
        while(pstList->m_pstAttr)
        {
            pstAttr = pstList->m_pstAttr;
            pstList->m_pstAttr = pstList->m_pstAttr->pstNext;

            pstAttr->pstNext = NULL;
            IBPFree(pstAttr);
        }

        pstList->pstNext = NULL;
        IBPFree(pstList);
    }

    pstRoot = NULL;
}

/**************************************************************************************************
    函 数 名：lGetAttrCount(CfgAttr *pstRoot)
    功能描述：获取配置中子配置个数
    返回说明：
        lCount        返回个数
 **************************************************************************************************/
long    lGetAttrCount(CfgHead *pstRoot)
{
    long    lCount;
    CfgHead    *pstList = NULL;

    for(pstList = pstRoot, lCount = 0; pstList; pstList = pstList->pstNext)
    {
        if('*' == pstList->m_szName[0] && !pstList->m_pstAttr)
            continue;

        ++ lCount;
    }

    return lCount;
}

/**************************************************************************************************
    函 数 名：lParseGroup(SATvm *pstSavm, size_t *plOut, CtsGroup **ppstGroup)
    功能描述：获取系统服务运行参数
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lParseGroup(SATvm *pstSavm, size_t *plOut, CtsGroup **ppstGroup)
{
    long        i;
    char        szTemp[10];
    CtsGroup    *pstGroup = NULL;
    CfgHead        *pstList = NULL, *pstRoot = NULL;

    memset(szTemp, 0, sizeof(szTemp));
    if(RC_SUCC != lParseConfig(pstSavm, &pstRoot, "*SERVERS"))
        return RC_FAIL;

    if(RC_SUCC >= (*plOut = lGetAttrCount(pstRoot)))
        return RC_SUCC;
    
    if(!(pstGroup = (CtsGroup *)calloc(*plOut, sizeof(CtsGroup))))
    {
        vCleanConfig(pstRoot);
        fprintf(stderr, "分配内存(%d)失败, %s", *plOut, strerror(errno));
        return RC_FAIL;
    }

    for(i = 0, pstList = pstRoot; pstList; pstList = pstList->pstNext)
    {
        if(!strcmp(pstList->m_szName, "*SERVERS") && !pstList->m_pstAttr)
            continue;

        pstGroup[i].m_lQid     = 0;
        pstGroup[i].m_lCurNum  = 0;
        pstGroup[i].m_lAutoExt = 0;
        pstGroup[i].m_lHost    = DEFAUT_HOST;
        pstGroup[i].m_lState   = STATUES_ENABLE;
        strcpy(pstGroup[i].m_szTime, sGetUpdTime());
        strncpy(pstGroup[i].m_szPGName, pstList->m_szName, sizeof(pstGroup[i].m_szPGName));
        
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "MIN", FIELD_NUMBER, &pstGroup[i].m_lMinNum))
            pstGroup[i].m_lMinNum = 1;

        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "GROUP", FIELD_NUMBER, &pstGroup[i].m_lGrpIdx))
            pstGroup[i].m_lGrpIdx = i + 1;

        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "TYPE", FIELD_NUMBER, &pstGroup[i].m_lType))
            pstGroup[i].m_lType = 0;

        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "MAX", FIELD_NUMBER, &pstGroup[i].m_lMaxNum))
            pstGroup[i].m_lMaxNum = 1;    
    
        if(pstGroup[i].m_lMaxNum < pstGroup[i].m_lMinNum)
            pstGroup[i].m_lMinNum = pstGroup[i].m_lMaxNum;

        memset(szTemp, 0, sizeof(szTemp));
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "RECON", FIELD_STRING, szTemp))
            szTemp[0] = 'N';

        if('Y' == szTemp[0] || 'y' == szTemp[0])
            pstGroup[i].m_bDbRcon = 1;
        else
            pstGroup[i].m_bDbRcon = 0;

        memset(szTemp, 0, sizeof(szTemp));
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "REBOOT", FIELD_STRING, szTemp))
            szTemp[0] = 'N';

        if('Y' == szTemp[0] || 'y' == szTemp[0])
            pstGroup[i].m_bIsBoot = 1;
        else
            pstGroup[i].m_bIsBoot = 0;

        memset(szTemp, 0, sizeof(szTemp));
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "EXTEND", FIELD_STRING, szTemp))
            szTemp[0] = 'N';

        if('Y' == szTemp[0] || 'y' == szTemp[0])
            pstGroup[i ++].m_lAutoExt = 1;
        else
            pstGroup[i ++].m_lAutoExt = 0;
    }

    *ppstGroup = pstGroup;
    vCleanConfig(pstRoot);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInitTruck(CMList **pstRoot, const char *pszTarget)
    功能描述：初始化启动配置
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitTruck(CMList **pstRoot, const char *pszTarget)
{
    char    szPath[512];

    memset(szPath, 0, sizeof(szPath));
    snprintf(szPath, sizeof(szPath), "%s", getenv("SETUPCFG"));

    return  _lInitTruck(pstRoot, pszTarget, szPath);
}

/**************************************************************************************************
    函 数 名：lConvUserCfg(char *pszOut, long lSize, CtsUser *pstUser)
    功能描述：获取流程配置信息
    返回说明：
        无
 **************************************************************************************************/
long    lConvUserCfg(char *pszOut, long lSize, CtsUser *pstUser)
{
    if(!pstUser || !pszOut) return 0;

    return snprintf(pszOut, lSize, "%ld|%ld|%s|%s|%s|%s|\n", pstUser->m_lUserGrp, 
        pstUser->m_lUserID, pstUser->m_szUser, pstUser->m_szPasswd, pstUser->m_szLTime, 
        pstUser->m_szCTime);
}

/**************************************************************************************************
    函 数 名：vGetUserCfg(char *pszCfg, CtsHost *pstUser)
    功能描述：获取配置主机结构信息
    返回说明：
        无
 **************************************************************************************************/
void    vGetUserCfg(char *pszCfg, CtsUser *pstUser)
{
    if(!pstUser)    return ;

    pstUser->m_lUserGrp = atol(sGetValueByIdx(pszCfg, "|", 1));
    pstUser->m_lUserID = atol(sGetValueByIdx(pszCfg, "|", 2));
    strcpy(pstUser->m_szUser, sGetValueByIdx(pszCfg, "|", 3));
    strcpy(pstUser->m_szPasswd, sGetValueByIdx(pszCfg, "|", 4));
    strcpy(pstUser->m_szLTime, sGetValueByIdx(pszCfg, "|", 5));
    strcpy(pstUser->m_szCTime, sGetValueByIdx(pszCfg, "|", 6));
}

/**************************************************************************************************
    函 数 名：sGetFlowStr(long lNum, long *plCGroup)
    功能描述：打印时，将流程组织字符串反显串出来
    返回说明：
        szCGroup            --字符串    
 **************************************************************************************************/
char*    sGetFlowStr(long lNum, long *plCGroup)
{
    long            lLen = 0, i;
    static    char  szCGroup[256];

    memset(szCGroup, 0, sizeof(szCGroup));
    if(lNum == 0 || !plCGroup)    return szCGroup;
            
    lLen = sprintf(szCGroup, "%d", plCGroup[0]);
    for(i = 1; i < lNum; i ++)
        lLen += snprintf(szCGroup + lLen, sizeof(szCGroup) - lLen, ",%d", plCGroup[i]);

    return szCGroup;
}

/**************************************************************************************************
    函 数 名：vDebugExpress(CtsFlow *pstFlow)
    功能描述：debug表达式
    返回说明：
        无
 **************************************************************************************************/
void    vDebugExpress(CtsFlow *pstFlow)
{
    long    i = 0;
    Express    *pstExpress = NULL;

    if(!pstFlow)    return ;

    
    fprintf(stdout, "total:%d\n", pstFlow->m_lExp);
    for(i = 0; i < pstFlow->m_lExp; i ++)
    {
        pstExpress = &pstFlow->m_stExp[i];
        fprintf(stdout, "lConst:%d, lField:%d, fd1:%s, fd2:%s, res:%s, yes:%s, no:%s\n", 
            pstExpress->m_lConst, pstExpress->m_lField, pstExpress->m_szField[0], 
            pstExpress->m_szField[1], pstExpress->m_szResult, pstExpress->m_szIsMatch, 
            pstExpress->m_szNotMatch);
    }
}

/**************************************************************************************************
    函 数 名：lInitExpress(CtsFlow *pstFlow, char *r, char *s)
    功能描述：初始化表达式参数
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lInitExpress(CtsFlow *pstFlow, char *r, char *s)
{
    Express *pstExpress = NULL;
    char    szField[TARGET_VALUE_LEN];

    if(MAX_EXPRESS <= pstFlow->m_lExp)
	{
		IBPwarn("表达式个数(%d)超限", pstFlow->m_lExp);
        return RC_FAIL;
	}

    sGetExpField(s, szField);
    pstExpress = &pstFlow->m_stExp[pstFlow->m_lExp ++];
	pstFlow->m_lCondit = 0;
    if(!strncmp(s, "if($$", 5))
    {
		// if($$a=2, 1, 0);
		pstFlow->m_lCondit = 1;
        pstExpress->m_lField = 2;
        pstExpress->m_lConst = EXP_CONST_CONDIT;
        strcpy(pstExpress->m_szField[0], sfieldvalue(szField, "=", 1));
        strcpy(szField, sfieldvalue(szField, "=", 2));
        strcpy(pstExpress->m_szField[1], sfieldvalue(szField, ",", 1));
        if(!strncmp(pstExpress->m_szField[1], VAR_POOL_FLG, 2))
            pstExpress->m_lField = 2;
        else
            pstExpress->m_lField = 1;

        sprintf(pstExpress->m_szIsMatch, "%d", atol(sfieldvalue(szField, ",", 2)));
        sprintf(pstExpress->m_szNotMatch, "%d", atol(sfieldvalue(szField, ",", 3)));
        strcpy(pstExpress->m_szResult, "$$CONDRES");
        sDropCharset(pstExpress->m_szField[1], '\"');
        sExpreplace(r, s, pstExpress->m_szResult);
    }
    else if(!strncmp(s, "strcmp($$", 9))
    {
		// strcmp($$a, $$b, 1, 2);
		pstFlow->m_lCondit = 1;
        pstExpress->m_lConst = EXP_CONST_STRCMP;
        strcpy(pstExpress->m_szField[0], sfieldvalue(szField, ",", 1));
        strcpy(pstExpress->m_szField[1], sfieldvalue(szField, ",", 2));
        if(!strncmp(pstExpress->m_szField[1], VAR_POOL_FLG, 2))
            pstExpress->m_lField = 2;
        else
            pstExpress->m_lField = 1;
        sprintf(pstExpress->m_szIsMatch, "%d", atol(sfieldvalue(szField, ",", 3)));
        sprintf(pstExpress->m_szNotMatch, "%d", atol(sfieldvalue(szField, ",", 4)));
        strcpy(pstExpress->m_szResult, "$$CONDRES");
        sDropCharset(pstExpress->m_szField[1], '\"');
        sExpreplace(r, s, pstExpress->m_szResult);
    }
    else if(!strncmp(s, "atol($$", 7))
    {
		// atol($$a)
        pstExpress->m_lField = 1;
        pstExpress->m_lConst = EXP_CONST_STRTOL;
		strimall(szField);
        strncpy(pstExpress->m_szField[0], szField, sizeof(pstExpress->m_szField[0]));
        strcpy(pstExpress->m_szResult, pstExpress->m_szField[0]);
        sExpreplace(r, s, pstExpress->m_szField[0]);
    }
    else if(!strncmp(s, "strcpy($$", 9))
    {
		// strcpy($$a, $$b)
        pstExpress->m_lConst = EXP_CONST_STRCPY;
        strcpy(pstExpress->m_szField[0], sfieldvalue(szField, ",", 1));
        strcpy(pstExpress->m_szField[1], sfieldvalue(szField, ",", 2));
        if(!strncmp(pstExpress->m_szField[1], VAR_POOL_FLG, 2))
            pstExpress->m_lField = 2;
        else
            pstExpress->m_lField = 1;
        sDropCharset(pstExpress->m_szField[1], '\"');
        sExpreplace(r, s, pstExpress->m_szField[0]);
    }
    else if(!strncmp(s, "strcat($$", 9))
    {
		// strcat($$a, $$b)
        pstExpress->m_lConst = EXP_CONST_STRCAT;
        strcpy(pstExpress->m_szField[0], sfieldvalue(szField, ",", 1));
        strcpy(pstExpress->m_szField[1], sfieldvalue(szField, ",", 2));
        if(!strncmp(pstExpress->m_szField[1], VAR_POOL_FLG, 2))
            pstExpress->m_lField = 2;
        else
            pstExpress->m_lField = 1;
        sExpreplace(r, s, pstExpress->m_szField[0]);
        sDropCharset(pstExpress->m_szField[1], '\"');
    }
    else if(!strncmp(s, "substr($$", 9))
    {
		// substr($$a, $$b, 2, 100)
        pstExpress->m_lConst = EXP_CONST_SUBSTR;
        strcpy(pstExpress->m_szField[0], sfieldvalue(szField, ",", 1));
        strcpy(pstExpress->m_szField[1], sfieldvalue(szField, ",", 2));
        if(!strncmp(pstExpress->m_szField[1], VAR_POOL_FLG, 2))
            pstExpress->m_lField = 2;
        else
            pstExpress->m_lField = 1;

        sprintf(pstExpress->m_szIsMatch, "%d", atol(sfieldvalue(szField, ",", 3)));
        sprintf(pstExpress->m_szNotMatch, "%d", atol(sfieldvalue(szField, ",", 4)));
        sExpreplace(r, s, pstExpress->m_szField[0]);
        sDropCharset(pstExpress->m_szField[1], '\"');
    }
    else if(!strncmp(s, "decode($$", 9))
    {
		// decode($$a, $$b, "1231;12;2", "100;12;123")
        pstExpress->m_lConst = EXP_CONST_DECODE;
        strcpy(pstExpress->m_szField[0], sfieldvalue(szField, ",", 1));
        strcpy(pstExpress->m_szField[1], sfieldvalue(szField, ",", 2));
        if(!strncmp(pstExpress->m_szField[1], VAR_POOL_FLG, 2))
            pstExpress->m_lField = 2;
        else
            pstExpress->m_lField = 1;

		strncpy(pstExpress->m_szIsMatch, sfieldvalue(szField, ",", 3), TARGET_VALUE_LEN);
		strncpy(pstExpress->m_szNotMatch, sfieldvalue(szField, ",", 4), TARGET_VALUE_LEN);
        sExpreplace(r, s, pstExpress->m_szField[0]);
        sDropCharset(pstExpress->m_szField[1], '\"');
        sDropCharset(pstExpress->m_szIsMatch, '\"');
        sDropCharset(pstExpress->m_szNotMatch, '\"');
    }
    else
    {
        IBPwarn("非法表达式:%s", s);
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lPickupFunc(CtsFlow *pstFlow, char *r, char *s)
    功能描述：递归遍历函数
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lPickupFunc(CtsFlow *pstFlow, char *r, char *s)
{
    long    i, j, skip;
    char    *p = NULL, *q = NULL, szCon[TARGET_VALUE_LEN];

    memset(szCon, 0, sizeof(szCon));
    if(!s || 0 == (j = strlen(s)))    return RC_SUCC;

    if(strstr(s, "if("))
        q = strstr(s, "if(");
    else if(strstr(s, "strcmp("))
        q = strstr(s, "strcmp(");
    else if(strstr(s, "strcat("))
        q = strstr(s, "strcat(");
    else if(strstr(s, "strcpy("))
        q = strstr(s, "strcpy(");
    else if(strstr(s, "atol("))
        q = strstr(s, "atol(");
    else if(strstr(s, "substr("))
        q = strstr(s, "substr(");
    else if(strstr(s, "decode("))
        q = strstr(s, "decode(");
    else
    {
        IBPwarn("无效的表达式:%s", s);
        return RC_FAIL;
    }

    for(i = 0, skip = 0, p = strstr(s, "(") + 1; i < (j - (p - s)); i ++)
    {
        if(p[i] == '(')
            skip ++;
        else if(p[i] == ')')
        {
            if(0 == skip)
                return lInitExpress(pstFlow, r, q);
            else if(0 == -- skip)
                break;
        }
    }

    memcpy(szCon, p, ++ i);
    return lPickupFunc(pstFlow, r, szCon);
}

/**************************************************************************************************
    函 数 名：lPraseExpFunc(CtsFlow *pstFlow, char *s)
    功能描述：解析表达式函数
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lPraseExpFunc(CtsFlow *pstFlow, char *s)
{
    long     i;
    Express  *pstExpress = NULL;
    
    if(!strcmp(s, "__KEEP_RUN__"))
    {
        if(MAX_EXPRESS < pstFlow->m_lExp)
            return RC_FAIL;

        pstExpress = &pstFlow->m_stExp[pstFlow->m_lExp ++];
        pstExpress->m_lConst = EXP_CONST_KEPRUN;
        pstExpress->m_szResult[0] = 0x30;
        return RC_SUCC;
    }
    else if(strncmp(s, "if(", 3) && strncmp(s, "strcmp(", 7) && strncmp(s, "strcpy(", 7) 
        && strncmp(s, "strcat(", 7) && strncmp(s, "atol(", 5) && strncmp(s, "substr(", 7) 
        && strncmp(s, "decode(", 7))
    {
        if(MAX_EXPRESS < pstFlow->m_lExp)
            return RC_FAIL;

        pstExpress = &pstFlow->m_stExp[pstFlow->m_lExp ++];
        if(strncmp(s, VAR_POOL_FLG, 2) || !strstr(s, "="))
        {
            pstExpress->m_lConst = EXP_CONST_STRING;
            pstExpress->m_lField = 0;
            strncpy(pstExpress->m_szResult, s, sizeof(pstExpress->m_szResult));
        }
        else
        {
            pstExpress->m_lField = 2;
            pstExpress->m_lConst = EXP_CONST_STRSET;
            strcpy(pstExpress->m_szField[0], sfieldvalue(s, "=", 1));
            strcpy(pstExpress->m_szField[1], sfieldvalue(s, "=", 2));
            if(strncmp(pstExpress->m_szField[1], VAR_POOL_FLG, 2))
            {
                pstExpress->m_lField = 1;
                strcpy(pstExpress->m_szResult, pstExpress->m_szField[1]);
                sDropCharset(pstExpress->m_szResult, '\"');
            }
        }
        
        return RC_SUCC;
    }

    for(i = 0; 0 < strlen(strimfield(s)); i ++)
    {
        if(RC_SUCC != lPickupFunc(pstFlow, s, s))
        {
            IBPwarn("解析表达式(%s)失败", s);
            return RC_FAIL;
        }
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vGetHostCfg(char *pszCfg, CtsHost *pstHost)
    功能描述：获取配置主机结构信息
    返回说明：
        无
 **************************************************************************************************/
void    vGetHostCfg(char *pszCfg, CtsHost *pstHost)
{
    if(!pstHost)    return ;

#if        0
    pstHost->m_lHost = atol(sGetValueByIdx(pszCfg, "|", 1));
    pstHost->m_lEnum = atol(sGetValueByIdx(pszCfg, "|", 2));
    strcpy(pstHost->m_szMark, sGetValueByIdx(pszCfg, "|", 3));
    strcpy(pstHost->m_szPGName, sGetValueByIdx(pszCfg, "|", 4));
    pstHost->m_lMinNum = atol(sGetValueByIdx(pszCfg, "|", 5));

    strcpy(pstHost->m_szDomain, sGetValueByIdx(pszCfg, "|", 6));
    strcpy(pstHost->m_szHostIp, sGetValueByIdx(pszCfg, "|", 7));
    pstHost->m_lHostPort = atol(sGetValueByIdx(pszCfg, "|", 8));
    pstHost->m_lTimeOut = atol(sGetValueByIdx(pszCfg, "|", 9));
    strcpy(pstHost->m_szFtpIp, sGetValueByIdx(pszCfg, "|", 10));
    pstHost->m_lFtpPort = atol(sGetValueByIdx(pszCfg, "|", 11));
    strcpy(pstHost->m_szFtpUser, sGetValueByIdx(pszCfg, "|", 12));
    strcpy(pstHost->m_szFtpPwd, sGetValueByIdx(pszCfg, "|", 13));
    pstHost->m_bLTLink = atol(sGetValueByIdx(pszCfg, "|", 14));
    pstHost->m_lRCTime = atol(sGetValueByIdx(pszCfg, "|", 15));
    strcpy(pstHost->m_szAuthUser, sGetValueByIdx(pszCfg, "|", 16));
    strcpy(pstHost->m_szAuthPwd, sGetValueByIdx(pszCfg, "|", 17));
    strcpy(pstHost->m_szService, sGetValueByIdx(pszCfg, "|", 18));
    pstHost->m_bAsyn = atol(sGetValueByIdx(pszCfg, "|", 19));
    pstHost->m_lResv = atol(sGetValueByIdx(pszCfg, "|", 20));
    pstHost->m_lPreRead = atol(sGetValueByIdx(pszCfg, "|", 21));
#else

    pstHost->m_lHost = atol(sGetValueByIdx(pszCfg, "|", 1));
    pstHost->m_lEnum = atol(sGetValueByIdx(pszCfg, "|", 2));
    pstHost->m_lType = atol(sGetValueByIdx(pszCfg, "|", 3));
    strcpy(pstHost->m_szName, sGetValueByIdx(pszCfg, "|", 4));
    strcpy(pstHost->m_szPGName, sGetValueByIdx(pszCfg, "|", 5));
    pstHost->m_bIsBoot = atol(sGetValueByIdx(pszCfg, "|", 6));
    pstHost->m_bDbRcon = atol(sGetValueByIdx(pszCfg, "|", 7));
    pstHost->m_lMinNum = atol(sGetValueByIdx(pszCfg, "|", 8));
    pstHost->m_lMaxNum = atol(sGetValueByIdx(pszCfg, "|", 9));
    pstHost->m_lAutoExt = atol(sGetValueByIdx(pszCfg, "|", 10));
    strcpy(pstHost->m_szDomain, sGetValueByIdx(pszCfg, "|", 11));
    strcpy(pstHost->m_szHostIp, sGetValueByIdx(pszCfg, "|", 12));
    pstHost->m_lHostPort = atol(sGetValueByIdx(pszCfg, "|", 13));
    pstHost->m_lTimeOut = atol(sGetValueByIdx(pszCfg, "|", 14));
    strcpy(pstHost->m_szFtpIp, sGetValueByIdx(pszCfg, "|", 15));
    pstHost->m_lFtpPort = atol(sGetValueByIdx(pszCfg, "|", 16));
    strcpy(pstHost->m_szFtpUser, sGetValueByIdx(pszCfg, "|", 17));
    strcpy(pstHost->m_szFtpPwd, sGetValueByIdx(pszCfg, "|", 18));
    pstHost->m_bLTLink = atol(sGetValueByIdx(pszCfg, "|", 19));
    pstHost->m_lRCTime = atol(sGetValueByIdx(pszCfg, "|", 20));
    strcpy(pstHost->m_szAuthUser, sGetValueByIdx(pszCfg, "|", 21));
    strcpy(pstHost->m_szAuthPwd, sGetValueByIdx(pszCfg, "|", 22));
    strcpy(pstHost->m_szService, sGetValueByIdx(pszCfg, "|", 23));
    pstHost->m_bAsyn = atol(sGetValueByIdx(pszCfg, "|", 24));
    pstHost->m_lResv = atol(sGetValueByIdx(pszCfg, "|", 25));
    pstHost->m_lPreRead = atol(sGetValueByIdx(pszCfg, "|", 26));
#endif
}

/**************************************************************************************************
    函 数 名：lConvHostCfg(char *pszCfg, CtsHost *pstHost)
    功能描述：获取配置主机结构信息
    返回说明：
        无
 **************************************************************************************************/
long    lConvHostCfg(char *pszOut, long lSize, CtsHost *pstHost)
{
    if(!pstHost || !pszOut)    return 0;

    return snprintf(pszOut, lSize, "%d|%d|%d|%s|%s|%d|%d|%d|%d|%d|%s|%s|%d|%d|%s|%d|%s|%s|%d"
        "|%d|%s|%s|%s|%d|%d|%d|\n", pstHost->m_lHost, pstHost->m_lEnum, pstHost->m_lType, 
        pstHost->m_szName, pstHost->m_szPGName, pstHost->m_bIsBoot, pstHost->m_bDbRcon, 
        pstHost->m_lMinNum, pstHost->m_lMaxNum, pstHost->m_lAutoExt, pstHost->m_szDomain, 
        pstHost->m_szHostIp, pstHost->m_lHostPort, pstHost->m_lTimeOut, pstHost->m_szFtpIp, 
        pstHost->m_lFtpPort, pstHost->m_szFtpUser, pstHost->m_szFtpPwd, pstHost->m_bLTLink, 
        pstHost->m_lRCTime, pstHost->m_szAuthUser, pstHost->m_szAuthPwd, pstHost->m_szService, 
        pstHost->m_bAsyn, pstHost->m_lResv, pstHost->m_lPreRead); 
}

/**************************************************************************************************
    函 数 名：lInitialUser(SATvm *pstSavm)
    功能描述：初始化用户表
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialUser(SATvm *pstSavm)
{
    long        lRet;
    size_t        lCount = 0, i = 0;
    CtsUser        *pstUser = NULL;
    CMList        *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lInitTruck(&pstRoot, "cts"))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(RC_SUCC != lCreateTable(pstSavm, CTS_MANAGER_USER, lCount + 10, C_CTS_MANAGER_USER))
        return RC_FAIL;

    if(lCount <= 0)        return RC_SUCC;

    if(!(pstUser = (CtsUser *)malloc(sizeof(CtsUser) * lCount)))
    {
        vDestroyList(pstRoot);
        IBPerror("分配大小内存(%d)失败, err:%s", sizeof(CtsUser) * lCount, strerror(errno));
        return RC_FAIL;
    }

    memset(pstUser, 0, sizeof(CtsUser) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetUserCfg((char *)pstNode->m_psvData, &pstUser[i]);
    vDestroyList(pstRoot);

    lRet = lImportTable(CTS_MANAGER_USER, lCount, (void *)pstUser);
    IBPFree(pstUser);
    return lRet;
}


/**************************************************************************************************
    函 数 名：nHostSort(const void *pvFrom, const void *pvNext)
    功能描述：比较排序回调函数
    返回说明：
        true
        false
 **************************************************************************************************/
int    nHostSort(const void *pvFrom, const void *pvNext)
{
    return ((CtsHost *)pvFrom)->m_lHost > ((CtsHost *)pvNext)->m_lHost;
}

/**************************************************************************************************
    函 数 名：lInitialHost(CMList *pstRoot)
    功能描述：初始化主机配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialHost(SATvm *pstSavm)
{
    long        lRet;
    size_t        lCount = 0, i = 0;
    CtsHost        *pstHost = NULL;
    CMList        *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lInitTruck(&pstRoot, "Host"))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(RC_SUCC != lCreateTable(pstSavm, CTS_TRADE_HOST, lCount + 1000, C_CTS_TRADE_HOST))
        return RC_FAIL;

    if(lCount <= 0)        return RC_SUCC;

    if(!(pstHost = (CtsHost *)malloc(sizeof(CtsHost) * lCount)))
    {
        vDestroyList(pstRoot);
        IBPerror("分配大小内存(%d)失败, err:%s", sizeof(CtsHost) * lCount, strerror(errno));
        return RC_FAIL;
    }

    memset(pstHost, 0, sizeof(CtsHost) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetHostCfg((char *)pstNode->m_psvData, &pstHost[i]);
    vDestroyList(pstRoot);

	qsort((void *)pstHost, lCount, sizeof(CtsHost), nHostSort);
    lRet = lImportTable(CTS_TRADE_HOST, lCount, (void *)pstHost);
    IBPFree(pstHost);
    return lRet;
}

/**************************************************************************************************
    函 数 名：lConvPackCfg(char *pszOut, long lSize, CtsPack *pstPack)
    功能描述：获取流程配置信息
    返回说明：
        无
 **************************************************************************************************/
long    lConvPackCfg(char *pszOut, long lSize, CtsPack *pstPack)
{
    char    szBCD[TARGET_NAME_LEN * 2];

    if(!pstPack || !pszOut) return 0;

    memset(szBCD, 0, sizeof(szBCD));
    lStringToHex(pstPack->m_szLabel, strlen(pstPack->m_szLabel), szBCD, sizeof(szBCD));

    return snprintf(pszOut, lSize, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s"
        "|%s|%s|%s|%s|%s|%s|\n", pstPack->m_lMid, pstPack->m_lQid, pstPack->m_lPQid, 
        pstPack->m_lType, pstPack->m_lChild, pstPack->m_lIsParant, pstPack->m_lSeq, 
        pstPack->m_lAttr, pstPack->m_lPos, pstPack->m_lFrom, pstPack->m_lLen, 
        pstPack->m_lIsConst, pstPack->m_lUser, pstPack->m_lPlSet, pstPack->m_lPlIdx, 
        pstPack->m_lLoop, pstPack->m_lSLen, pstPack->m_lNilOn, szBCD, pstPack->m_szMark, 
		pstPack->m_szConst, pstPack->m_szPlName, pstPack->m_szLoop, pstPack->m_szExFunc, 
		pstPack->m_szUserFunc);
}

/**************************************************************************************************
    函 数 名：vGetPackCfg(char *pszCfg, CtsPack *pstPack)
    功能描述：获取配置主机结构信息
    返回说明：
        无
 **************************************************************************************************/
void    vGetPackCfg(char *pszCfg, CtsPack *pstPack)
{
    char    szBCD[TARGET_NAME_LEN * 2] =  {0};

    if(!pstPack)    return ;

    pstPack->m_lMid = atol(sGetValueByIdx(pszCfg, "|", 1));
    pstPack->m_lQid = atol(sGetValueByIdx(pszCfg, "|", 2));
    pstPack->m_lPQid = atol(sGetValueByIdx(pszCfg, "|", 3));
    pstPack->m_lType = atol(sGetValueByIdx(pszCfg, "|", 4));
    pstPack->m_lChild = atol(sGetValueByIdx(pszCfg, "|", 5));
    pstPack->m_lIsParant = atol(sGetValueByIdx(pszCfg, "|", 6));
    pstPack->m_lSeq = atol(sGetValueByIdx(pszCfg, "|", 7));
    pstPack->m_lAttr = atol(sGetValueByIdx(pszCfg, "|", 8));
    pstPack->m_lPos = atol(sGetValueByIdx(pszCfg, "|", 9));
    pstPack->m_lFrom = atol(sGetValueByIdx(pszCfg, "|", 10));
    pstPack->m_lLen = atol(sGetValueByIdx(pszCfg, "|", 11));
    pstPack->m_lIsConst = atol(sGetValueByIdx(pszCfg, "|", 12));
    pstPack->m_lUser = atol(sGetValueByIdx(pszCfg, "|", 13));
    pstPack->m_lPlSet = atol(sGetValueByIdx(pszCfg, "|", 14));
    pstPack->m_lPlIdx = atol(sGetValueByIdx(pszCfg, "|", 15));
    pstPack->m_lLoop = atol(sGetValueByIdx(pszCfg, "|", 16));
    pstPack->m_lSLen = atol(sGetValueByIdx(pszCfg, "|", 17));
    pstPack->m_lNilOn = atol(sGetValueByIdx(pszCfg, "|", 18));

    if(pstPack->m_lPos < 0)        pstPack->m_lPos = 0;
    if(pstPack->m_lFrom < 0)       pstPack->m_lFrom = 0;
    if(pstPack->m_lLen < 0)        pstPack->m_lLen = 0;
    if(pstPack->m_lPlIdx < 0)      pstPack->m_lPlIdx = 0;
    if(pstPack->m_lSLen < 0)       pstPack->m_lSLen = 0;

    if(FIELD_NUMBER == pstPack->m_lAttr)
        strcpy(pstPack->m_szAttr, "NUMBER");
    else if(FIELD_DOUBLE == pstPack->m_lAttr)
        strcpy(pstPack->m_szAttr, "DOUBLE");
    else if(FIELD_STRING == pstPack->m_lAttr)
        strcpy(pstPack->m_szAttr, "STRING");
    else if(FILED_BINARY == pstPack->m_lAttr)
        strcpy(pstPack->m_szAttr, "BINARY");
    else if(FILED_SENACT == pstPack->m_lAttr)
        strcpy(pstPack->m_szAttr, "SENACT");
    else if(FILED_SENNAM == pstPack->m_lAttr)
        strcpy(pstPack->m_szAttr, "SENNAM");
    else if(FILED_SENTEL == pstPack->m_lAttr)
        strcpy(pstPack->m_szAttr, "SENTEL");
    else if(FILED_SENMAL == pstPack->m_lAttr)
        strcpy(pstPack->m_szAttr, "SENMAL");
    else
        strcpy(pstPack->m_szAttr, "#PNODE");

    strncpy(szBCD, sGetValueByIdx(pszCfg, "|", 19), sizeof(szBCD));
    lHexToString(szBCD, strlen(szBCD), pstPack->m_szLabel, sizeof(pstPack->m_szLabel));

    strncpy(pstPack->m_szMark, sGetValueByIdx(pszCfg, "|", 20), sizeof(pstPack->m_szMark));
    strncpy(pstPack->m_szConst, sGetValueByIdx(pszCfg, "|", 21), sizeof(pstPack->m_szConst));
    strncpy(pstPack->m_szPlName, sGetValueByIdx(pszCfg, "|", 22), sizeof(pstPack->m_szPlName));
    strncpy(pstPack->m_szLoop, sGetValueByIdx(pszCfg, "|", 23), sizeof(pstPack->m_szLoop));
    strncpy(pstPack->m_szExFunc, sGetValueByIdx(pszCfg, "|", 24), sizeof(pstPack->m_szExFunc));
    strncpy(pstPack->m_szUserFunc, sGetValueByIdx(pszCfg, "|", 25), sizeof(pstPack->m_szUserFunc));

    return ;
}

/**************************************************************************************************
    函 数 名：lInitialPack(CMList *pstRoot)
    功能描述：初始化主机配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialPack(SATvm *pstSavm)
{
    CtsPack    *pstPack = NULL;
    long       lCount = 0, i = 0, lRet;
    CMList     *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lInitTruck(&pstRoot, "Pack"))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(RC_SUCC != lCreateTable(pstSavm, CTS_PACK_CONFIG, lCount + 1000, C_CTS_PACK_CONFIG))
        return RC_FAIL;

    if(lCount <= 0)        return RC_SUCC;

    if(!(pstPack = (CtsPack *)malloc(sizeof(CtsPack) * lCount)))
    {
        vDestroyList(pstRoot);
        IBPerror("分配大小内存(%d)失败, err:%s", sizeof(CtsPack) * lCount, strerror(errno));
        return RC_FAIL;
    }

    memset(pstPack, 0, sizeof(CtsPack) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetPackCfg((char *)pstNode->m_psvData, &pstPack[i]);
    vDestroyList(pstRoot);

    qsort((void *)pstPack, lCount, sizeof(CtsPack), nCallSort);
    lRet = lImportTable(CTS_PACK_CONFIG, lCount, (void *)pstPack);
    IBPFree(pstPack);
    return lRet;
}

/**************************************************************************************************
    函 数 名：lInitialGroup(SATvm *pstSavm)
    功能描述：获取配置中的进程组信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialGroup(SATvm *pstSavm)
{
    CtsParam    stParam;
    CtsHost     *pstHost = NULL;
    size_t      lCount, lRow, i, j, k;
    CtsGroup    stGroup, *pstGroup = NULL;

    if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
        return RC_FAIL;

    if(RC_SUCC != lParseGroup(pstSavm, &lRow, &pstGroup))
        return RC_FAIL;

    if(RC_SUCC != lExportTable(CTS_TRADE_HOST, &lCount, (void **)&pstHost))
        return RC_FAIL;

    for(i = 0, k = 0; i < lRow; i ++)
        k += pstGroup[i].m_lMaxNum;    

    for(i = 0, j = lRow; i < lCount; i ++)
    {
        if(!strlen(pstHost[i].m_szPGName))
            continue;
        j ++;
        k += pstHost[i].m_lMaxNum;
    }

    if(stParam.m_lMaxServers < j || stParam.m_lMaxProcess < k)
    {
        fprintf(stderr, "服务进程配置参数配置超限\n");
        goto ERROR_GROUP;
    }

    if(RC_SUCC != lCreateTable(pstSavm, CTS_PLAT_GROUP, stParam.m_lMaxServers, C_CTS_PLAT_GROUP))
    {
        fprintf(stderr, "创建进程组信息表失败, (%s)\n", sGetTError(pstSavm->m_lErrno));
        goto ERROR_GROUP;
    }

    if(RC_SUCC != lImportTable(CTS_PLAT_GROUP, lRow, (void *)pstGroup))
    {
        fprintf(stderr, "导入进程组配置表失败, %s\n", sGetTError(pstSavm->m_lErrno));
        goto ERROR_GROUP;
    }
    IBPFree(pstGroup);

    if(RC_SUCC != lCreateTable(pstSavm, CTS_PLAT_PROCESS, stParam.m_lMaxProcess,
        C_CTS_PLAT_PROCESS))
    {
        fprintf(stderr, "创建进程运行表失败, (%s)\n", sGetTError(pstSavm->m_lErrno));
        goto ERROR_GROUP;
    }

    for(i = 0, k = 0; i < lCount; i ++)
    {
        if(!strlen(pstHost[i].m_szPGName))
            continue;

        memset(&stGroup, 0, sizeof(CtsGroup));
        stGroup.m_lGrpIdx  = ++ lRow;
        stGroup.m_lState   = STATUES_ENABLE; 
        stGroup.m_lHost    = pstHost[i].m_lHost; 
        stGroup.m_lType    = pstHost[i].m_lType; 
        stGroup.m_lMinNum  = pstHost[i].m_lMinNum;
        stGroup.m_lMaxNum  = pstHost[i].m_lMaxNum;
        stGroup.m_lAutoExt = pstHost[i].m_lAutoExt; 
        stGroup.m_bIsBoot  = pstHost[i].m_bIsBoot;
        stGroup.m_bDbRcon  = pstHost[i].m_bDbRcon;
        strncpy(stGroup.m_szTime, sGetUpdTime(), sizeof(stGroup.m_szTime));
        strncpy(stGroup.m_szPGName, pstHost[i].m_szPGName, sizeof(stGroup.m_szPGName));

        pstSavm->tblName = CTS_PLAT_GROUP;
        pstSavm->lFind   = IDX_SELECT;
        pstSavm->lSize   = sizeof(CtsGroup);
        pstSavm->pstVoid = &stGroup;
        if(RC_SUCC != lInsert(pstSavm))
            goto ERROR_GROUP;
    }

    IBPFree(pstHost);
    return RC_SUCC;

ERROR_GROUP:
    IBPFree(pstHost);
    IBPFree(pstGroup);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：lConvClassCfg(char *pszOut, long lSize, CtsClass *pstClass)
    功能描述：获取流程配置信息
    返回说明：
        无
 **************************************************************************************************/
long    lConvClassCfg(char *pszOut, long lSize, CtsClass *pstClass)
{
    if(!pstClass || !pszOut) return 0;

    return snprintf(pszOut, lSize, "%d|%d|%d|%d|%d|%d|%s|%s|%s|\n", pstClass->m_lHost, 
            pstClass->m_lBuss, pstClass->m_lInMid, pstClass->m_lOutMid, pstClass->m_lErrMid, 
            pstClass->m_lTimeOut, pstClass->m_szClass, pstClass->m_szMethod, pstClass->m_szMark);
}

/**************************************************************************************************
    函 数 名：vGetClassCfg(char *pszCfg, CtsHost *pstClass)
    功能描述：获取配置组件结构信息
    返回说明：
        无
 **************************************************************************************************/
void    vGetClassCfg(char *pszCfg, CtsClass *pstClass)
{
    if(!pstClass)    return ;

    pstClass->m_lHost = atol(sGetValueByIdx(pszCfg, "|", 1));
    pstClass->m_lBuss = atol(sGetValueByIdx(pszCfg, "|", 2));
    pstClass->m_lInMid = atol(sGetValueByIdx(pszCfg, "|", 3));
    pstClass->m_lOutMid = atol(sGetValueByIdx(pszCfg, "|", 4));
    pstClass->m_lErrMid = atol(sGetValueByIdx(pszCfg, "|", 5));
    pstClass->m_lTimeOut = atol(sGetValueByIdx(pszCfg, "|", 6));
    strcpy(pstClass->m_szClass, sGetValueByIdx(pszCfg, "|", 7));
    strcpy(pstClass->m_szMethod, sGetValueByIdx(pszCfg, "|", 8));
    strcpy(pstClass->m_szMark, sGetValueByIdx(pszCfg, "|", 9));
}

/**************************************************************************************************
    函 数 名：nCallSort(const void *pvFrom, const void *pvNext)
    功能描述：比较排序回调函数
    返回说明：
        true
        false
 **************************************************************************************************/
int    nClassSort(const void *pvFrom, const void *pvNext)
{
    if (((CtsClass *)pvFrom)->m_lHost == ((CtsClass *)pvNext)->m_lHost)
        return ((CtsClass *)pvFrom)->m_lBuss > ((CtsClass *)pvNext)->m_lBuss;
    else
        return ((CtsClass *)pvFrom)->m_lHost > ((CtsClass *)pvNext)->m_lHost;
}

/**************************************************************************************************
    函 数 名：lInitialClass(CMList *pstRoot)
    功能描述：初始化组件配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialClass(SATvm *pstSavm)
{
    long        lRet;
    CtsClass    *pstClass = NULL;
    size_t        lCount = 0, i = 0;
    CMList        *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lInitTruck(&pstRoot, "Class"))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(RC_SUCC != lCreateTable(pstSavm, CTS_TRADE_CLASS, lCount + 1000, C_CTS_TRADE_CLASS))
        return RC_FAIL;

    if(lCount <= 0)        return RC_SUCC;

    if(!(pstClass = (CtsClass *)malloc(sizeof(CtsClass) * lCount)))
    {
        vDestroyList(pstRoot);
        IBPerror("分配大小内存(%d)失败, err:%s", sizeof(CtsClass) * lCount, strerror(errno));
        return RC_FAIL;
    }

    memset(pstClass, 0, sizeof(CtsClass) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetClassCfg((char *)pstNode->m_psvData, &pstClass[i]);
    vDestroyList(pstRoot);

	qsort((void *)pstClass, lCount, sizeof(CtsClass), nClassSort);
    lRet = lImportTable(CTS_TRADE_CLASS, lCount, (void *)pstClass);
    IBPFree(pstClass);
    return lRet;
}

/**************************************************************************************************
    函 数 名：lConvServiceCfg(char *pszOut, long lSize, CtsService *pstService)
    功能描述：获取流程配置信息
    返回说明：
        无
 **************************************************************************************************/
long    lConvServiceCfg(char *pszOut, long lSize, CtsService *pstService)
{
    if(!pstService || !pszOut) return 0;

    return snprintf(pszOut, lSize, "%d|%d|%s|%s|%d|%d|%d|%s|%d|%s|\n", pstService->m_lHost, 
        pstService->m_lSvrID, pstService->m_szTrCode, pstService->m_szSvrName, 
        pstService->m_lState, pstService->m_lTimeOut, pstService->m_lLogLevel, 
        pstService->m_szLogName, pstService->m_lSvrType, pstService->m_szRevCode);
}

/**************************************************************************************************
    函 数 名：vGetServiceCfg(char *pszCfg, CtsService *pstService)
    功能描述：获取配置组件结构信息
    返回说明：
        无
 **************************************************************************************************/
void    vGetServiceCfg(char *pszCfg, CtsService *pstService)
{
    if(!pstService)    return ;

    pstService->m_lHost = atol(sGetValueByIdx(pszCfg, "|", 1));
    pstService->m_lSvrID = atol(sGetValueByIdx(pszCfg, "|", 2));
    strcpy(pstService->m_szTrCode, sGetValueByIdx(pszCfg, "|", 3));
    strcpy(pstService->m_szSvrName, sGetValueByIdx(pszCfg, "|", 4));
    
    pstService->m_lState = atol(sGetValueByIdx(pszCfg, "|", 5));
    pstService->m_lTimeOut = atol(sGetValueByIdx(pszCfg, "|", 6));
    pstService->m_lLogLevel = atol(sGetValueByIdx(pszCfg, "|", 7));
    strcpy(pstService->m_szLogName, sGetValueByIdx(pszCfg, "|", 8));
    pstService->m_lSvrType = atol(sGetValueByIdx(pszCfg, "|", 9));
    strcpy(pstService->m_szRevCode, sGetValueByIdx(pszCfg, "|", 10));
}

/**************************************************************************************************
    函 数 名：nServiceSort(const void *pvFrom, const void *pvNext)
    功能描述：比较排序回调函数
    返回说明：
        true
        false
 **************************************************************************************************/
int    nServiceSort(const void *pvFrom, const void *pvNext)
{
    if(((CtsService *)pvFrom)->m_lHost == ((CtsService *)pvNext)->m_lHost)
        return memcmp(((CtsService *)pvFrom)->m_szTrCode, ((CtsService *)pvNext)->m_szTrCode, 
			sizeof(CtsService));
    else
        return memcmp(((CtsService *)pvFrom)->m_szTrCode, ((CtsService *)pvNext)->m_szTrCode, 
			sizeof(CtsService));
}

/**************************************************************************************************
    函 数 名：lInitialService()
    功能描述：初始化服务配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialService(SATvm *pstSavm)
{
    long        lRet;
    CtsService  *pstService = NULL;
    size_t      lCount = 0, i = 0;
    CMList      *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lInitTruck(&pstRoot, "Service"))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(RC_SUCC != lCreateTable(pstSavm, CTS_TRADE_SERVICE, lCount + 1000, C_CTS_TRADE_SERVICE))
        return RC_FAIL;

    if(lCount <= 0)        return RC_SUCC;

    if(!(pstService = (CtsService *)malloc(sizeof(CtsService) * lCount)))
    {
        vDestroyList(pstRoot);
        IBPerror("分配大小内存(%d)失败, err:%s", sizeof(CtsService) * lCount, strerror(errno));
        return RC_FAIL;
    }

    memset(pstService, 0, sizeof(CtsService) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetServiceCfg((char *)pstNode->m_psvData, &pstService[i]);
    vDestroyList(pstRoot);

	qsort((void *)pstService, lCount, sizeof(CtsService), nServiceSort);
    lRet = lImportTable(CTS_TRADE_SERVICE, lCount, (void *)pstService);
    IBPFree(pstService);
    return lRet;
}

/**************************************************************************************************
    函 数 名：vGetFlowCfg(char *pszCfg, CtsFlow *pstFlow)
    功能描述：获取流程配置信息
    返回说明：
        无
 **************************************************************************************************/
void    vGetFlowCfg(char *pszCfg, CtsFlow *pstFlow)
{
    long    j = 0, n = 0;
    char    szTemp[512], szExpress[256];

    if(!pstFlow)    return ;

    memset(szTemp, 0, sizeof(szTemp));
    pstFlow->m_lSvrID = atol(sGetValueByIdx(pszCfg, "|", 1));
    pstFlow->m_lFlow = atol(sGetValueByIdx(pszCfg, "|", 2));
    pstFlow->m_lType = atol(sGetValueByIdx(pszCfg, "|", 3));
    pstFlow->m_lHost = atol(sGetValueByIdx(pszCfg, "|", 4));
    pstFlow->m_lBuss = atol(sGetValueByIdx(pszCfg, "|", 5));

    pstFlow->m_lInMid = atol(sGetValueByIdx(pszCfg, "|", 6));
    pstFlow->m_lOutMid = atol(sGetValueByIdx(pszCfg, "|", 7));
    pstFlow->m_lErrMid = atol(sGetValueByIdx(pszCfg, "|", 8));
    pstFlow->m_lNext = atol(sGetValueByIdx(pszCfg, "|", 9));
    strcpy(szTemp, sGetValueByIdx(pszCfg, "|", 10));

    for(j = 0; j < pstFlow->m_lNext; j ++)
        pstFlow->m_lNextFlow[j] = atol(sGetValueByIdx(szTemp, ",", j + 1));

    strcpy(pstFlow->m_szClass, sGetValueByIdx(pszCfg, "|", 11));
    strcpy(pstFlow->m_szMethod, sGetValueByIdx(pszCfg, "|", 12));
    strcpy(pstFlow->m_szPGName, sGetValueByIdx(pszCfg, "|", 13));
    pstFlow->m_lAsyn = atol(sGetValueByIdx(pszCfg, "|", 15));
    strcpy(pstFlow->m_szExp, sGetValueByIdx(pszCfg, "|", 14));
    strimcrlf(pstFlow->m_szExp);
    sltrim(pstFlow->m_szExp);
    srtrim(pstFlow->m_szExp);

    for(j = 0, pstFlow->m_lExp = 0, n = lGetValidNum(pstFlow->m_szExp, ";"); j < n; j ++)
    {
        memset(szTemp, 0, sizeof(szTemp));
        strcpy(szTemp, sGetValueByIdx(pstFlow->m_szExp, ";", j + 1));
        if(!sExpressLegal(szTemp))
        {
            fprintf(stderr, "函数不合法:%s, 继续解析其他流程\n", szTemp);
            continue;
        }

        lPraseExpFunc(pstFlow, szTemp);
    }

//	vDebugExpress(pstFlow);
}

/**************************************************************************************************
    函 数 名：lConvFlowCfg(char *pszOut, long lSize, CtsFlow *pstFlow)
    功能描述：获取流程配置信息
    返回说明：
        无
 **************************************************************************************************/
long    lConvFlowCfg(char *pszOut, long lSize, CtsFlow *pstFlow)
{
    if(!pstFlow || !pszOut) return 0;

    return  snprintf(pszOut, lSize, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s|%s|%s|%d|\n", 
        pstFlow->m_lSvrID, pstFlow->m_lFlow, pstFlow->m_lType, pstFlow->m_lHost, 
        pstFlow->m_lBuss, pstFlow->m_lInMid, pstFlow->m_lOutMid, pstFlow->m_lErrMid, 
        pstFlow->m_lNext, sGetFlowStr(pstFlow->m_lNext, pstFlow->m_lNextFlow), 
        pstFlow->m_szClass, pstFlow->m_szMethod, pstFlow->m_szPGName, pstFlow->m_szExp, 
        pstFlow->m_lAsyn);
}

/**************************************************************************************************
    函 数 名：lInitialFlow()
    功能描述：初始化服务配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialFlow(SATvm *pstSavm)
{
    CtsFlow        *pstFlow = NULL;
    size_t        lCount = 0, i = 0, lRet;
    CMList        *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lInitTruck(&pstRoot, "Flow"))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(RC_SUCC != lCreateTable(pstSavm, CTS_TRADE_FLOW, lCount + 1000, C_CTS_TRADE_FLOW))
        return RC_FAIL;

    if(lCount <= 0)        return RC_SUCC;

    if(!(pstFlow = (CtsFlow *)malloc(sizeof(CtsFlow) * lCount)))
    {
        vDestroyList(pstRoot);
        IBPerror("分配大小内存(%d)失败, err:%s", sizeof(CtsFlow) * lCount, strerror(errno));
        return RC_FAIL;
    }

    memset(pstFlow, 0, sizeof(CtsFlow) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetFlowCfg((char *)pstNode->m_psvData, &pstFlow[i]);
    vDestroyList(pstRoot);

    lRet = lImportTable(CTS_TRADE_FLOW, lCount, (void *)pstFlow);
    IBPFree(pstFlow);
    return lRet;
}

/**************************************************************************************************
    函 数 名：lConvTrCodeCfg(char *pszOut, long lSize, CtsTrCode *pstTrCode)
    功能描述：获取流程配置信息
    返回说明：
        无
 **************************************************************************************************/
long    lConvTrCodeCfg(char *pszOut, long lSize, CtsTrCode *pstTrCode)
{
    if(!pstTrCode || !pszOut) return 0;

    return snprintf(pszOut, lSize, "%d|%s|%s|%s|%s|\n", pstTrCode->m_lIdx, pstTrCode->m_szUse,
        pstTrCode->m_szInCode, pstTrCode->m_szInSCode, pstTrCode->m_szTrCode);
}

/**************************************************************************************************
    函 数 名：vGetTrCodeCfg(char *pszCfg, CtsService *pstService)
    功能描述：获取配置组件结构信息
    返回说明：
        无
 **************************************************************************************************/
void    vGetTrCodeCfg(char *pszCfg, CtsTrCode *pstCode)
{
    if(!pstCode || !pszCfg)    return ;

    pstCode->m_lIdx = atol(sGetValueByIdx(pszCfg, "|", 1));
    strncpy(pstCode->m_szUse, sGetValueByIdx(pszCfg, "|", 2), sizeof(pstCode->m_szUse));
    strncpy(pstCode->m_szInCode, sGetValueByIdx(pszCfg, "|", 3), sizeof(pstCode->m_szInCode));
    strncpy(pstCode->m_szInSCode, sGetValueByIdx(pszCfg, "|", 4), sizeof(pstCode->m_szInSCode));
    strncpy(pstCode->m_szTrCode, sGetValueByIdx(pszCfg, "|", 5), sizeof(pstCode->m_szTrCode));
}

/**************************************************************************************************
    函 数 名：lInitialTrCode()
    功能描述：初始化服务配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialTrCode(SATvm *pstSavm)
{
    CtsTrCode    *pstCode = NULL;
    size_t        lCount = 0, i = 0, lRet;
    CMList        *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lInitTruck(&pstRoot, "TrCode"))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(RC_SUCC != lCreateTable(pstSavm, CTS_TRADE_CODE, lCount + 1000, C_CTS_TRADE_CODE))
        return RC_FAIL;

    if(lCount <= 0)        return RC_SUCC;

    if(!(pstCode = (CtsTrCode *)malloc(sizeof(CtsTrCode) * lCount)))
    {
        vDestroyList(pstRoot);
        IBPerror("分配大小内存(%d)失败, err:%s", sizeof(CtsTrCode) * lCount, strerror(errno));
        return RC_FAIL;
    }

    memset(pstCode, 0, sizeof(CtsTrCode) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetTrCodeCfg((char *)pstNode->m_psvData, &pstCode[i]);
    vDestroyList(pstRoot);

    lRet = lImportTable(CTS_TRADE_CODE, lCount, (void *)pstCode);
    IBPFree(pstCode);
    return lRet;
}

/**************************************************************************************************
    函 数 名：lConvTrCodeCfg(char *pszOut, long lSize, CtsMapCode *pstMCode)
    功能描述：获取流程配置信息
    返回说明：
        无
 **************************************************************************************************/
long    lConvMapCodeCfg(char *pszOut, long lSize, CtsMapCode *pstMCode)
{
    if(!pstMCode || !pszOut) return 0;

    return snprintf(pszOut, lSize, "%d|%s|%s|%s|%s|%s|%s|%d|\n", pstMCode->m_lIdx, pstMCode->m_szEntNo, 
		pstMCode->m_szInCode, pstMCode->m_szEntCode, pstMCode->m_szEntMsg, pstMCode->m_szLcode,
		pstMCode->m_szLabel, pstMCode->m_lState);
}

/**************************************************************************************************
    函 数 名：vGetMapCodeCfg(char *pszCfg, CtsMapCode *pstMCode)
    功能描述：获取配置组件结构信息
    返回说明：
        无
 **************************************************************************************************/
void    vGetMapCodeCfg(char *pszCfg, CtsMapCode *pstMCode)
{
    if(!pstMCode || !pszCfg)    return ;

    pstMCode->m_lIdx = atol(sGetValueByIdx(pszCfg, "|", 1));
    strncpy(pstMCode->m_szEntNo, sGetValueByIdx(pszCfg, "|", 2), sizeof(pstMCode->m_szEntNo));
    strncpy(pstMCode->m_szInCode, sGetValueByIdx(pszCfg, "|", 3), sizeof(pstMCode->m_szInCode));
    strncpy(pstMCode->m_szEntCode, sGetValueByIdx(pszCfg, "|", 4), sizeof(pstMCode->m_szEntCode));
    strncpy(pstMCode->m_szEntMsg, sGetValueByIdx(pszCfg, "|", 5), sizeof(pstMCode->m_szEntMsg));
//	strncpy(pstMCode->m_szLcode, sGetValueByIdx(pszCfg, "|", 6), sizeof(pstMCode->m_szLcode));
//	strncpy(pstMCode->m_szLabel, sGetValueByIdx(pszCfg, "|", 7), sizeof(pstMCode->m_szLabel));
	strncpy(pstMCode->m_szLcode, MAP_POOL_COD, sizeof(pstMCode->m_szLcode));
	strncpy(pstMCode->m_szLabel, MAP_POOL_MSG, sizeof(pstMCode->m_szLabel));
    pstMCode->m_lState = atol(sGetValueByIdx(pszCfg, "|", 8));
}

/**************************************************************************************************
    函 数 名：lInitialMapCode(SATvm *pstSavm)
    功能描述：初始化服务配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitialMapCode(SATvm *pstSavm)
{
    CtsMapCode    *pstMCode = NULL;
    size_t        lCount = 0, i = 0, lRet;
    CMList        *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lInitTruck(&pstRoot, "MapCode"))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(RC_SUCC != lCreateTable(pstSavm, CTS_RETCODE_MAP, 5000, C_CTS_RETCODE_MAP))
        return RC_FAIL;

    if(lCount <= 0)        return RC_SUCC;

    if(!(pstMCode = (CtsMapCode *)malloc(sizeof(CtsMapCode) * lCount)))
    {
        vDestroyList(pstRoot);
        IBPerror("分配大小内存(%d)失败, err:%s", sizeof(CtsMapCode) * lCount, strerror(errno));
        return RC_FAIL;
    }

    memset(pstMCode, 0, sizeof(CtsMapCode) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetMapCodeCfg((char *)pstNode->m_psvData, &pstMCode[i]);
    vDestroyList(pstRoot);

    lRet = lImportTable(CTS_RETCODE_MAP, lCount, (void *)pstMCode);
    IBPFree(pstMCode);
    return lRet;
}

/**************************************************************************************************
    函 数 名：lInitSetup()
    功能描述：初始化启动配置
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lInitSetup(SATvm *pstSavm)
{
    //    初始化主机信息
    if(RC_SUCC != lInitialUser(pstSavm))
        return RC_FAIL;

    //    初始化主机信息
    if(RC_SUCC != lInitialHost(pstSavm))
        return RC_FAIL;

    //    初始化组件信息
    if(RC_SUCC != lInitialClass(pstSavm))
        return RC_FAIL;

    //    初始化服务配置信息
    if(RC_SUCC != lInitialService(pstSavm))
        return RC_FAIL;
    
    //    初始化流程配置信息
    if(RC_SUCC != lInitialFlow(pstSavm))
        return RC_FAIL;
    
    //    初始化交易码
    if(RC_SUCC != lInitialTrCode(pstSavm))
        return RC_FAIL;

    //    初始化交易码
    if(RC_SUCC != lInitialMapCode(pstSavm))
        return RC_FAIL;

    //    初始化报文配置
    if(RC_SUCC != lInitialPack(pstSavm))
        return RC_FAIL;

    return lInitialGroup(pstSavm);
}

/**************************************************************************************************
    函 数 名：lExportEngine(char *pszBuffer)
    功能描述：从内存中获取配置
    参数说明:
        lMax                --最大空间大小
        ppszOut                --字符串首地址指针
    返回说明：
        lOfs                --数据长度
 **************************************************************************************************/
long    lExportEngine(char **ppszOut, long lMax)
{
    size_t      lCount = 0, i, lTotal, lOfs = 0;
    void        *pvData = NULL, *pszBuffer = NULL;

    if(NULL == (*ppszOut))
    {    
        lTotal = SETUP_MAX_SIZE;
        if(NULL == (pszBuffer = (char *)calloc(lTotal, sizeof(char))))
        {
            SYSError(RET_SYSTEM, "创建卸载内存(%d)失败, %s", lTotal, strerror(errno));
            return RC_FAIL;
        }
        *ppszOut = pszBuffer;
    }
    else
    {
        lTotal    = lMax;    
        pszBuffer = *ppszOut;
    }

    if(RC_SUCC != lExportTable(CTS_MANAGER_USER, &lCount, (void **)&pvData))
        goto ExportError;

    lOfs = snprintf(pszBuffer + lOfs, lTotal - lOfs, "<cts>\n");
    for(i = 0; i < lCount; i ++)
        lOfs += lConvUserCfg(pszBuffer + lOfs, lTotal - lOfs, &((CtsUser *)pvData)[i]);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "</cts>\n");
    IBPFree(pvData);

    if(RC_SUCC != lExportTable(CTS_TRADE_HOST, &lCount, (void **)&pvData))
        goto ExportError;

    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "<Host>\n");
    for(i = 0; i < lCount; i ++)
        lOfs += lConvHostCfg(pszBuffer + lOfs, lTotal - lOfs, &((CtsHost *)pvData)[i]);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "</Host>\n");
    IBPFree(pvData);

    if(RC_SUCC != lExportTable(CTS_TRADE_CLASS, &lCount, (void **)&pvData))
        goto ExportError;

    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "<Class>\n");
    for(i = 0; i < lCount; i ++)
        lOfs += lConvClassCfg(pszBuffer + lOfs, lTotal - lOfs, &((CtsClass *)pvData)[i]);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "</Class>\n");
    IBPFree(pvData);

    if(RC_SUCC != lExportTable(CTS_TRADE_SERVICE, &lCount, (void **)&pvData))
        goto ExportError;

    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "<Service>\n");
    for(i = 0; i < lCount; i ++)
        lOfs += lConvServiceCfg(pszBuffer + lOfs, lTotal - lOfs, &((CtsService *)pvData)[i]);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "</Service>\n");
    IBPFree(pvData);

    if(RC_SUCC != lExportTable(CTS_TRADE_FLOW, &lCount, (void **)&pvData))
        goto ExportError;

    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "<Flow>\n");
    for(i = 0; i < lCount; i ++)
        lOfs += lConvFlowCfg(pszBuffer + lOfs, lTotal - lOfs, &((CtsFlow *)pvData)[i]);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "</Flow>\n");
    IBPFree(pvData);

    if(RC_SUCC != lExportTable(CTS_TRADE_CODE, &lCount, (void **)&pvData))
        goto ExportError;

    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "<TrCode>\n");
    for(i = 0; i < lCount; i ++)
        lOfs += lConvTrCodeCfg(pszBuffer + lOfs, lTotal - lOfs, &((CtsTrCode *)pvData)[i]);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "</TrCode>\n");
    IBPFree(pvData);

    if(RC_SUCC != lExportTable(CTS_RETCODE_MAP, &lCount, (void **)&pvData))
        goto ExportError;

    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "<MapCode>\n");
    for(i = 0; i < lCount; i ++)
        lOfs += lConvMapCodeCfg(pszBuffer + lOfs, lTotal - lOfs, &((CtsMapCode *)pvData)[i]);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "</MapCode>\n");
    IBPFree(pvData);

    if(RC_SUCC != lExportTable(CTS_PACK_CONFIG, &lCount, (void **)&pvData))
        goto ExportError;

    qsort((void *)pvData, lCount, sizeof(CtsPack), nCallSort);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "<Pack>\n");
    for(i = 0; i < lCount; i ++)
        lOfs += lConvPackCfg(pszBuffer + lOfs, lTotal - lOfs, &((CtsPack *)pvData)[i]);
    lOfs += snprintf(pszBuffer + lOfs, lTotal - lOfs, "</Pack>\n");
    IBPFree(pvData);

    return lOfs;

ExportError:
    SYSError(RET_SYSTEM, "从运行引擎中导出数据异常");
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：vPrintConfig(CfgHead *pstRoot)
    功能描述：调试加载数据
    返回说明：
        无
 **************************************************************************************************/
void    vPrintConfig(CfgHead *pstRoot)
{
    CfgAttr    *pstAttr = NULL;
    CfgHead    *pstList = pstRoot;

    while(pstList)
    {
        fprintf(stdout, "头结构:%s\n", pstList->m_szName);    
        pstAttr = pstList->m_pstAttr;
        while(pstAttr)
        {
            fprintf(stdout, "NAME[%s]=[%s]\n", pstAttr->m_szAttr, pstAttr->m_szValue);    
            pstAttr = pstAttr->pstNext;
        }

        pstList = pstList->pstNext;
    }
}

/**************************************************************************************************
    函 数 名：vPrintTest(CtsParam *pstParam, CtsGroup *pstGroup, long lOut)
    功能描述：调试
    返回说明：
        无
 **************************************************************************************************/
void    vPrintTest(CtsParam *pstParam, CtsGroup *pstGroup, long lOut)
{
    long    i = 0;
    
    fprintf(stdout, "系统名:%s\n", pstParam->m_szSysName);
    fprintf(stdout, "清算日期:%s\n", pstParam->m_szCrDate);
    fprintf(stdout, "交易日期:%s\n", pstParam->m_szTxDate);
    fprintf(stdout, "流水号:%d\n", pstParam->m_lSeqNo);
    fprintf(stdout, "平台状态:%d\n", pstParam->m_lState);
    fprintf(stdout, "交易总笔数:%d\n", pstParam->m_lTrCount);
    fprintf(stdout, "成功总笔数:%d\n", pstParam->m_lSuccCount);
    fprintf(stdout, "失败总笔数:%d\n", pstParam->m_lFailCount);
    fprintf(stdout, "连续失败次数:%d\n", pstParam->m_lInsaCount);
    fprintf(stdout, "失败时间窗口:%d\n", pstParam->m_lInsaCount);
    fprintf(stdout, "变量池大小:%d\n", pstParam->m_lUserSize);
    fprintf(stdout, "最大并发数:%d\n", pstParam->m_lPoolNum);
    fprintf(stdout, "监控时间间隔:%d\n", pstParam->m_lScanTime);
    fprintf(stdout, "最大进程组数:%d\n", pstParam->m_lMaxServers);
    fprintf(stdout, "最大进程数:%d\n", pstParam->m_lMaxProcess);
    fprintf(stdout, "异步标识数:%d\n", pstParam->m_lMaxAsyn);
    fprintf(stdout, "日志级别:%d\n", pstParam->m_lLogLevel);
    fprintf(stdout, "管理节点:%d\n", pstParam->m_lMaster);
    fprintf(stdout, "启动时间:%s\n", pstParam->m_szUpTime);
    
    fprintf(stdout, "line\t执行码名称           进程组编号  进程类型  当前个数 最少启动数"
        "  最大启动数 自动拓展数 消息队列Q  状态  位置  启动时间\n");
    fprintf(stdout, "------------------------------------------------------------------"
        "-----------------------------------------------------------------\n");
    for(i = 0; i < lOut; i ++)
    {
        fprintf(stdout, "[%d]:\t%-20s|%10d|%10d|%8d|%10d|%10d|%10d|%10d|%5d|%5d|%14s|\n",
            i, pstGroup[i].m_szPGName, pstGroup[i].m_lGrpIdx, pstGroup[i].m_lType, pstGroup[i].m_lCurNum,
            pstGroup[i].m_lMinNum, pstGroup[i].m_lMaxNum, pstGroup[i].m_lAutoExt, pstGroup[i].m_lQid,
            pstGroup[i].m_lState, pstGroup[i].m_lAppPos, pstGroup[i].m_szTime);
    }
}

/**************************************************************************************************
    函 数 名：lParseParam(SATvm *pstSavm, CtsParam *pstParam)
    功能描述：获取系统运行参数列表
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lParseParam(SATvm *pstSavm, CtsParam *pstParam)
{
    CfgHead    *pstList = NULL, *pstRoot = NULL;

    if(RC_SUCC != lParseConfig(pstSavm, &pstRoot, "*RESOURCE"))
        return RC_FAIL;

    if(1 != lGetAttrCount(pstRoot))
    {
        vCleanConfig(pstRoot);
        fprintf(stdout, "*RESOURCE区配置错误\n");
        return RC_FAIL; 
    }

    pstParam->m_lTrCount   = 0;
    pstParam->m_lSuccCount = 0;
    pstParam->m_lFailCount = 0;
    pstParam->m_lSeqNo       = 100;
    pstParam->m_lState       = SYSTEM_PLT_START;
    strcpy(pstParam->m_szTxDate, sGetDate());
    strcpy(pstParam->m_szSysName, SYSTEM_NAME);
    strcpy(pstParam->m_szCrDate, sGetDate());
    strcpy(pstParam->m_szUpTime, sGetUpdTime());

    for(pstList = pstRoot; pstList; pstList = pstList->pstNext)
    {
        if(!strcmp(pstList->m_szName, "*RESOURCE") && !pstList->m_pstAttr)
            continue;

        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "POOLSIZE", FIELD_NUMBER, 
            &pstParam->m_lUserSize))
            pstParam->m_lUserSize = 32768;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "POOLNUM", FIELD_NUMBER, 
            &pstParam->m_lPoolNum))
            pstParam->m_lPoolNum = 1000;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "SANITYSCAN", FIELD_NUMBER, 
            &pstParam->m_lScanTime))
            pstParam->m_lScanTime = 30;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "QUEUETIME", FIELD_NUMBER, 
            &pstParam->m_lQueueTime))
            pstParam->m_lQueueTime = 30;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "RECYLETIME", FIELD_NUMBER, 
            &pstParam->m_lTRTime))
            pstParam->m_lTRTime = 100;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "MAXSERVERS", FIELD_NUMBER, 
            &pstParam->m_lMaxServers))
            pstParam->m_lMaxServers = 100;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "MAXPROCESS", FIELD_NUMBER, 
            &pstParam->m_lMaxProcess))
            pstParam->m_lMaxProcess = 300;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "MAXASYNNUM", FIELD_NUMBER, 
            &pstParam->m_lMaxAsyn))
            pstParam->m_lMaxAsyn = 1000;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "LOGLEVEL", FIELD_NUMBER, 
            &pstParam->m_lLogLevel))
            pstParam->m_lLogLevel = 5;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "MASTER", FIELD_NUMBER, 
            &pstParam->m_lMaster))
            pstParam->m_lMaster = 0;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "QPRATIO", FIELD_NUMBER, 
            &pstParam->m_lQPRatio))
            pstParam->m_lQPRatio = 3;
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "INSULATENUM", FIELD_NUMBER, 
            &pstParam->m_lInsaCount))
            pstParam->m_lInsaCount = 10;    
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "INSULATETIME", FIELD_NUMBER, 
            &pstParam->m_lInsaCount))
            pstParam->m_lInsaCount = 60;    
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "USERKEY", FIELD_NUMBER, &pstParam->m_lMaxKey))
            pstParam->m_lMaxKey = 1000;    
        if(RC_SUCC != lGetConfig(pstList->m_pstAttr, "REVSTRY", FIELD_NUMBER, &pstParam->m_lRevsTry))
            pstParam->m_lRevsTry = 3;
    }
    vCleanConfig(pstRoot);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lExportParam(xmlNodePtr pxRoot, CtsParam *pstParam)
    功能描述：导出系统运行参数信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lExportParam(xmlNodePtr pxRoot, CtsParam *pstParam)
{
    char        szTemp[1024];
    xmlNodePtr    pxNode = NULL, pxChild = NULL;
    
    memset(szTemp, 0, sizeof(szTemp));
    pxNode = xmlNewNode(NULL, (xmlChar *)"globle");
    if(!pxNode)
    {
        fprintf(stdout, "创建xml节点globle失败!\n");
        return RC_FAIL;
    }

    xmlAddChild(pxRoot, pxNode);

    XMLstring(pxNode, szTemp, "sysname", pstParam->m_szSysName);
    XMLstring(pxNode, szTemp, "txdate", pstParam->m_szTxDate);
    XMLstring(pxNode, szTemp, "cleardate", pstParam->m_szCrDate);
    XMLlong(pxNode, szTemp, "seqno", pstParam->m_lSeqNo);
    XMLlong(pxNode, szTemp, "state", pstParam->m_lState);
    XMLlong(pxNode, szTemp, "trcount", pstParam->m_lTrCount);
    XMLlong(pxNode, szTemp, "succcount", pstParam->m_lSuccCount);
    XMLlong(pxNode, szTemp, "failcount", pstParam->m_lFailCount);
    XMLlong(pxNode, szTemp, "insulatenum", pstParam->m_lInsaCount);
    XMLlong(pxNode, szTemp, "insulatetime", pstParam->m_lInsaCount);
    XMLlong(pxNode, szTemp, "poolsize", pstParam->m_lUserSize);
    XMLlong(pxNode, szTemp, "poolnum", pstParam->m_lPoolNum);
    XMLlong(pxNode, szTemp, "scantime", pstParam->m_lScanTime);
    XMLlong(pxNode, szTemp, "queuetime", pstParam->m_lQueueTime);
    XMLlong(pxNode, szTemp, "trctime", pstParam->m_lTRTime);
    XMLlong(pxNode, szTemp, "maxservers", pstParam->m_lMaxServers);
    XMLlong(pxNode, szTemp, "maxprocess", pstParam->m_lMaxProcess);
    XMLlong(pxNode, szTemp, "maxasyn", pstParam->m_lMaxAsyn);
    XMLlong(pxNode, szTemp, "userkey", pstParam->m_lMaxKey);
    XMLlong(pxNode, szTemp, "revstry", pstParam->m_lRevsTry);
    XMLlong(pxNode, szTemp, "qpratio", pstParam->m_lQPRatio);
    XMLlong(pxNode, szTemp, "loglevel", pstParam->m_lLogLevel);
    XMLlong(pxNode, szTemp, "master", pstParam->m_lMaster);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lExportGloble(xmlNodePtr pxRoot, GCts *pstGloble, long lIdx)
    功能描述：导出系统交易共享信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lExportGloble(xmlNodePtr pxRoot, GCts *pstCts, long lIdx)
{
    char        szTemp[1024], *p = NULL;
    xmlNodePtr    pxNode = NULL, pxChild = NULL;
    
    memset(szTemp, 0, sizeof(szTemp));
    pxNode = xmlNewNode(NULL, (xmlChar *)"runtime");
    if(!pxNode)
    {
        fprintf(stdout, "创建xml节点globle失败!\n");
        return RC_FAIL;
    }

    xmlAddChild(pxRoot, pxNode);
    
    XMLlongattr(pxNode, szTemp, "etime", lGetUnixTime());
    XMLlongattr(pxNode, szTemp, "len", pstCts->m_lLen);
    XMLlongattr(pxNode, szTemp, "state", pstCts->m_lStatus);
    XMLlongattr(pxNode, szTemp, "idx", lIdx);

    XMLlong(pxNode, szTemp, "seqno", pstCts->m_lSeqNo);
    XMLlong(pxNode, szTemp, "inhost", pstCts->m_lInHost);
    XMLlong(pxNode, szTemp, "inpid", pstCts->m_lInPid);
    XMLlong(pxNode, szTemp, "inqid", pstCts->m_lInQid);
    XMLlong(pxNode, szTemp, "inmid", pstCts->m_lInMid);
    XMLlong(pxNode, szTemp, "outmid", pstCts->m_lOutMid);
    XMLlong(pxNode, szTemp, "errmid", pstCts->m_lErrMid);
    XMLlong(pxNode, szTemp, "intime", pstCts->m_lInTime);
    XMLlong(pxNode, szTemp, "timeout", pstCts->m_lTimeOut);
    XMLlong(pxNode, szTemp, "usetime", pstCts->m_lUseTime);
    XMLlong(pxNode, szTemp, "asyn", pstCts->m_lAsyn);
    XMLlong(pxNode, szTemp, "launch", pstCts->m_lLaunch);

    XMLlong(pxNode, szTemp, "loglev", pstCts->m_lLogLev);
    XMLlong(pxNode, szTemp, "host", pstCts->m_lHost);
    XMLlong(pxNode, szTemp, "srvid", pstCts->m_lSvrID);
    XMLlong(pxNode, szTemp, "pid", pstCts->m_lPid);
    XMLlong(pxNode, szTemp, "resv", pstCts->m_lResv);
    XMLlong(pxNode, szTemp, "tomid", pstCts->m_lToMid);
    XMLlong(pxNode, szTemp, "toinmid", pstCts->m_lToInMid);
    XMLlong(pxNode, szTemp, "toerrmid", pstCts->m_lToErrMid);

    XMLlong(pxNode, szTemp, "state", pstCts->m_lState);
    XMLlong(pxNode, szTemp, "laststate", pstCts->m_lLastState);

    XMLstring(pxNode, szTemp, "incode", pstCts->m_szInCode);
    XMLstring(pxNode, szTemp, "inscode", pstCts->m_szInSCode);
    XMLstring(pxNode, szTemp, "trcode", pstCts->m_szTrCode);
    XMLstring(pxNode, szTemp, "callcode", pstCts->m_szCallCode);
    XMLstring(pxNode, szTemp, "callsub", pstCts->m_szCallSub);

    XMLstring(pxNode, szTemp, "svrname", pstCts->m_szSvrName);
    XMLstring(pxNode, szTemp, "logname", pstCts->m_szLogName);
    XMLstring(pxNode, szTemp, "txdate", pstCts->m_szTxDate);
    XMLstring(pxNode, szTemp, "txtime", pstCts->m_szTxTime);
    XMLstring(pxNode, szTemp, "cleardate", pstCts->m_szCrDate);
    XMLstring(pxNode, szTemp, "retcode", pstCts->m_szRetCode);
    XMLstring(pxNode, szTemp, "retmsg", pstCts->m_szRetMsg);
    XMLstring(pxNode, szTemp, "lastret", pstCts->m_szLastRet);
    XMLstring(pxNode, szTemp, "lastmsg", pstCts->m_szLastMsg);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：bEnsure(char *pszPGName)
    功能描述：对于cts仅仅只能一个进程在运行
    返回说明：
        TRUE            --成功
        FALSE            --失败
 **************************************************************************************************/
BOOL    bIsExtraBoot(char *pszPGName)
{
    FILE    *op = NULL;
    char    szCmd[1024], szLine[256];

    memset(szLine, 0, sizeof(szLine));
    memset(szCmd, 0, sizeof(szCmd));
    snprintf(szCmd, sizeof(szCmd), "ps -u %s|grep -w %s|wc -l", getenv("LOGNAME"), pszPGName);
    if(NULL == (op = popen(szCmd, "r")))
        return false;

    fgets(szLine, sizeof(szLine), op);
    strimcrlf(szLine);
    pclose(op);
    op = NULL;

    if(atoi(szLine) > 1)    return true;
    else                    return false;
}

/**************************************************************************************************
    函 数 名：lGetCmdReturn(char *pszCmd)
    功能描述：获取命令返回的值
    返回说明：
        无
 **************************************************************************************************/
long    lGetCmdReturn(char *pszCmd)
{
    FILE    *op = NULL;
    char    szLine[256];
    
    if(!pszCmd)        return RC_FAIL;

    memset(szLine, 0, sizeof(szLine));
    if(NULL == (op = popen(pszCmd, "r")))
        return RC_FAIL;

    fgets(szLine, sizeof(szLine), op);
    strimcrlf(szLine);
    strimall(szLine);
    pclose(op);
    op = NULL;

    return atol(szLine);
}

/**************************************************************************************************
    函 数 名：lGetCmdString(char *pszCmd, char *pszOut, long lOut)
    功能描述：获取命令返回的值
    返回说明：
        无
 **************************************************************************************************/
long    lGetCmdString(char *pszCmd, char *pszOut, long lOut)
{
    FILE    *op = NULL;
    
    if(NULL == (op = popen(pszCmd, "r")))
        return RC_FAIL;

    fgets(pszOut, lOut, op);
    strimcrlf(pszOut);
    pclose(op);
    op = NULL;

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vExitAbnormal(int sig)
    功能描述：捕获指定信号后的处理函数
    返回说明：
        无
 **************************************************************************************************/
void    vExitAbnormal(int sig)
{
    IBPrace("进程(%d)捕捉到异常信号(%d), 异常退出", getpid(), sig);
    switch(sig)
    {
        case SIGTERM:
        case SIGUSR1:
            lDumpProcess(getpid());
            exit(RC_SUCC);
        case SIGUSR2:
            break;
        default:
            ;
    }
}

/**************************************************************************************************
    函 数 名：lRaiseProcess(char *pszPGName, long lExt)
    功能描述：Fork进程拓展进程
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lRaiseProcess(CtsProcess *pstProcess, long lExt)
{
    long        lRet = 0, lPid;
    char        szProcess[512];

    memset(szProcess, 0, sizeof(szProcess));
    snprintf(szProcess, sizeof(szProcess), "%s/bin/%s", getenv("HOME"), pstProcess->m_szPGName);
    if(RC_SUCC != access(szProcess, X_OK | F_OK))
    {
        IBPerror("进程文件(%s)异常, err:%s\n", pstProcess->m_szPGName, strerror(errno));
        return RC_FAIL;
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    if((lPid = fork()) < 0)
    {
         IBPerror("fork %s failed, %s\n", strerror(errno));
        return RC_FAIL;
    }
    else if(lPid > 0)
        return lPid;

    vLogSetPath();
    vSetSysLevel(pGetCmie(), LOG_TRACE);
    vLogSetSize(LOG_DEFAULT_SIZE);
    vSetProgram(pstProcess->m_szPGName);
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);
    IBPrace("重启执行码(%s)", pstProcess->m_szPGName);

    pstProcess->m_lState  = BCS_APP_IDLE;
    pstProcess->m_lRunExt = lExt;
    if(RC_SUCC != lRefreshProcess(pstProcess, getpid()))
    {
        IBPerror("刷新进程(%d)(%s)信息失败", pstProcess->m_lPid, pstProcess->m_szPGName);
        return RC_FAIL;
    }

    /* 执行 process execlp()会依PATH 变量中的/bin找, execlp执行后后面的代码将不会被执行*/
    lRet = execlp(pstProcess->m_szPGName, pstProcess->m_szPGName, (char*)0);
    if(0 > lRet)    return RC_FAIL;

    //  启动完之后进行登记
    exit(RC_SUCC);
}

/**************************************************************************************************
    函 数 名：lForkProcess(char *pszAppName, CtsProcess *pstProcess)
    功能描述：Fork进程并注册
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lForkProcess(char *pszAppName, CtsProcess *pstProcess, BOOL eType)
{
    long    lRet = 0, i = 0, lPid;
    SATvm    *pstSavm = (SATvm *)pGetSATvm();
  
    signal(SIGCHLD, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    if((lPid = fork()) < 0)
    {
        fprintf(stderr, "exec .. failed, %s\n", strerror(errno));
        return RC_FAIL;
    }
    else if(lPid > 0)
        return lPid;

    pstProcess->m_lPid = getpid();
    if(RC_SUCC != lAddProcess(pstSavm, pstProcess, eType))
    {
        fprintf(stderr, "登记任务(%s)的进程失败, %s", pszAppName, sGetTError(pstSavm->m_lErrno));
        exit(RC_FAIL);
    }

    /* 执行 process execlp()会依PATH 变量中的/bin找, execlp执行后后面的代码将不会被执行*/
    lRet = execlp(pszAppName, pszAppName, (char*)0);
    if(0 > lRet)
        return RC_FAIL;

    //  启动完之后进行登记
    exit(RC_SUCC);
}

/**************************************************************************************************
    函 数 名：bExistProcess(long lPid)
    功能描述：检查子进程是否启动
    返回说明：
        TURE            --找到该进程
        FAILE            --进程不存在
 **************************************************************************************************/
BOOL    bExistProcess(long lPid)
{
    errno = 0;
    if(getpgid(lPid) == -1)
    {
        if(ESRCH == errno)    return FALSE;
        else                return TRUE;
    }

    return TRUE;
}

/**************************************************************************************************
    函 数 名：bCheckProcess(long lPid)
    功能描述：检查进程是否在运行
    返回说明：
        TURE            --成功
        FAILE            --失败
 **************************************************************************************************/
BOOL    bCheckProcess(long lPid)
{
    FILE    *op = NULL;
    char    szCmd[1024], szLine[256];

    memset(szLine, 0, sizeof(szLine));
    memset(szCmd, 0, sizeof(szCmd));
    snprintf(szCmd, sizeof(szCmd), "ps -u %s|grep %d|wc -l", getenv("LOGNAME"), lPid);
    if(NULL == (op = popen(szCmd, "r")))
        return FALSE;

    fgets(szLine, sizeof(szLine), op);
    strimcrlf(szLine);
    pclose(op);
    op = NULL;

    if(atoi(szLine) > 0)    return TRUE;
    else                    return FALSE;
}

/**************************************************************************************************
    函 数 名：lCheckSystem()
    功能描述：检查系统运行状况
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lCheckSystem()
{
    SATvm        *pstSavm = (SATvm *)pGetSATvm();
    CtsParam    stParam;

    if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
    {
        IBPerror("获取系统配置信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }






    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lShrinkProcess(SATvm *pstSavm, CtsProcess *pstProcess, CtsGroup *pstGroup, BOOL bf)
    功能描述：指定退出一个进程
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    _lShrinkProcess(SATvm *pstSavm, CtsProcess *pstProcess, CtsGroup *pstGroup, BOOL bf)
{
    long        i = 0;
    CtsEvent    stEvent;

    memset(&stEvent, 0, sizeof(CtsEvent));
    vLogSetPath();
    vLogStore(pGetCmie());    
    vLogSetName(pGetCmie(), "%s/%s.log", sLogGetPath(), pstProcess->m_szPGName);
    if(-- pstGroup->m_lCurNum < 0)
        pstGroup->m_lCurNum = 0;    

    //    优先修改进程状态，避免在退出后仍然收到任务
    if(RC_SUCC != lExitProcess(pstProcess, false))
        return RC_FAIL;

    vLogRestore(pGetCmie());    

    if(bExistProcess(pstProcess->m_lPid))    //    进程还未退出
    {
        usleep(50000);    //    等待0.5s等待任务调度完毕
        stEvent.m_lState = EVENT_EVQUIT;
        stEvent.m_lMType = pstProcess->m_lPid;
        stEvent.m_lResv  = getpid();
        if(RC_SUCC != lEventWrite(pstSavm, pstGroup->m_lQid, &stEvent, sizeof(CtsEvent)))
        {
            IBPerror("发送队列(%d)进程(%s)(%d)退出信号失败, err:%s", pstGroup->m_lQid,
                pstProcess->m_szPGName, pstProcess->m_lPid, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        if(bf)        //    为真，标识必须等待退出
        {
            for( ;bExistProcess(pstProcess->m_lPid); )
                usleep(50000);
        }
        else        //    为假，表示只需等待分钟观察
        {
            for(i = 0; bExistProcess(pstProcess->m_lPid) && i < 20; ++ i)
                usleep(50000);
        }
    }
    
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lShrinkProcess(SATvm *pstSavm, CtsProcess *pstProcess, CtsGroup *pstGroup)
    功能描述：指定指定某一个进程, 不需要等待该进程退出
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lShrinkProcess(SATvm *pstSavm, CtsProcess *pstProcess, CtsGroup *pstGroup)
{
    return _lShrinkProcess(pstSavm, pstProcess, pstGroup, false);
}

/**************************************************************************************************
    函 数 名：lShrinkProcess(SATvm *pstSavm, CtsProcess *pstProcess, CtsGroup *pstGroup)
    功能描述：指定指定某一个进程, 需要等待该进程退出
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lStallProcess(SATvm *pstSavm, CtsProcess *pstProcess, CtsGroup *pstGroup)
{
    return _lShrinkProcess(pstSavm, pstProcess, pstGroup, true);
}

/**************************************************************************************************
    函 数 名：lRegisterProcess(char *pszProcess, long lHost)
    功能描述：针对自启动程序的注册使用(在子进程中注册)
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lRegisterProcess(char *pszProcess, long lHost)
{
    CtsGroup    stGroup;
    CtsProcess  stProcess;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    memset(&stGroup, 0, sizeof(CtsGroup));
    memset(&stProcess, 0, sizeof(CtsProcess));
    if(!pszProcess)
    {
        IBPerror("注册进程(%s)参数非法", pszProcess);
        return RC_FAIL;
    }

    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pszProcess, lHost))
     {
        IBPerror("获取进程组(%s)信息失败, err:%s", pszProcess, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(stGroup.m_lMaxNum <= 0)
    {
        IBPerror("进程(%s)设置最大个数为0，无法注册", pszProcess);
        return RC_FAIL;
    }

    if(stGroup.m_lMaxNum - stGroup.m_lCurNum <= 0)
    {
        IBPerror("进程(%s)当前已启动该进程最大个数%d，无法注册", pszProcess, stGroup.m_lMaxNum);
        return RC_FAIL;
    }

    stProcess.m_lPend   = 0;
    stProcess.m_lDoNum  = 0;
    stProcess.m_lHost   = lHost;
    stProcess.m_lPid    = getpid();
    stProcess.m_lState  = BCS_APP_IDLE;
    stProcess.m_lRunExt = BCS_EXPAND_CHD;
    stProcess.m_lType   = stGroup.m_lType;
    stProcess.m_lGrpIdx = stGroup.m_lGrpIdx;
    strncpy(stProcess.m_szPGName, pszProcess, sizeof(stProcess.m_szPGName));

    if(RC_SUCC != lAddProcess(pstSavm, &stProcess, TRUE))
    {
        IBPerror("登记任务(%s)的进程失败, err:%s", pszProcess, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    ++ stGroup.m_lCurNum;
    if(RC_SUCC != lUpdateGroup(pstSavm, &stGroup))
    {
        IBPerror("更新进程(%s)当前个数(%d)失败, err:%s", stGroup.m_szPGName, stGroup.m_lCurNum,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lUnregisterProcess(char *pszProcess)
    功能描述：进程退出状态更新
    返回说明：
        RC_SUCC             --成功
        RC_FAIL             --失败
 **************************************************************************************************/
long    lUnregisterProcess(CtsProcess *pstProcess)
{
    CtsGroup    stGroup;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pstProcess->m_szPGName, pstProcess->m_lHost))
    {
        IBPerror("获取进程(%s)配置信息失败, err:%s", pstProcess->m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(stGroup.m_lCurNum <= 0)
    {
        IBPerror("进程当前个数异常，无法注销进程(%s)(%d)", pstProcess->m_szPGName);    
        return RC_FAIL;
    }

    if(RC_SUCC != lUpdateProcess(pstSavm, pstProcess, BCS_APP_STOP))
    {
        IBPrace("进程(%s)(%d)修改状态, err:%s", pstProcess->m_szPGName,
            pstProcess->m_lPid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    -- stGroup.m_lCurNum;

    if(RC_SUCC != lUpdateGroup(pstSavm, &stGroup))
    {
        IBPerror("更新进程(%s)当前个数(%d)失败, err:%s", stGroup.m_szPGName, stGroup.m_lCurNum,
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

	vResetVTData();
	vReleaseBuffer();
    vTvmHoldRelease(pstSavm);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lExecProcess(CtsGroup *pstGroup, long *plExec, BOOL eType)
    功能描述：启动指定的进程组 eType为真，新增记录，反之覆盖记录
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lExecProcess(SATvm *pstSavm, CtsGroup *pstGroup, BOOL bType)
{
    CtsProcess  stProcess;
    char        szProcess[512];
    BOOL        bRet = FALSE, bExist = TRUE;
    long        lExec = 0, lPid, j, lBegin= 0;

    memset(szProcess, 0, sizeof(szProcess));
    memset(&stProcess, 0, sizeof(stProcess));
    fprintf(stdout, "Boot process(%s), current %d total %d process  %d\n", pstGroup->m_szPGName,
        pstGroup->m_lCurNum, pstGroup->m_lMinNum, pstGroup->m_lMaxNum);
    fflush(stdout);

    snprintf(szProcess, sizeof(szProcess), "%s/bin/%s", getenv("HOME"), pstGroup->m_szPGName);
    if(RC_SUCC != access(szProcess, X_OK | F_OK))
    {
/*
        if(!bType)
        {
            pstGroup->m_lMinNum = 0;
            pstGroup->m_lAppPos = 0;
            pstGroup->m_lMaxNum = 0;
        }
*/

		bExist = false;
        fprintf(stderr, "   exec %s ... failed. (%s)\n", pstGroup->m_szPGName, strerror(errno));
        fflush(stderr);
//        return RC_FAIL;
    }

    stProcess.m_lPend   = 0;
    stProcess.m_lDoNum  = 0;
    stProcess.m_lRunExt = 0;
    stProcess.m_lHost   = pstGroup->m_lHost;
    stProcess.m_lType   = pstGroup->m_lType;
    stProcess.m_lGrpIdx = pstGroup->m_lGrpIdx;
    stProcess.m_bDbRcon = pstGroup->m_bDbRcon;
    memcpy(stProcess.m_szPGName, pstGroup->m_szPGName, sizeof(stProcess.m_szPGName) - 1);

    lBegin = pstGroup->m_lCurNum;
    for(lExec = 0, stProcess.m_lSeq = lBegin; lExec < pstGroup->m_lMaxNum - lBegin; lExec ++)
    {
        stProcess.m_lIdx = pstGroup->m_lAppPos + stProcess.m_lSeq;
        ++ stProcess.m_lSeq;
        if(pstGroup->m_lMinNum <= pstGroup->m_lCurNum || !bExist)
        {
            //    不在拓展进程空间, 启动false才拓展
            if(bType)    return lExec;

            stProcess.m_lPid = 0;
            stProcess.m_lState  = BCS_APP_STOP;
			if(!bExist)
			{
        		if(stProcess.m_lSeq <= pstGroup->m_lMinNum)
            		stProcess.m_lRunExt = BCS_EXPAND_NML;
        		else
            		stProcess.m_lRunExt = BCS_EXPAND_EXT;
			}
			else
            	stProcess.m_lRunExt = BCS_EXPAND_EXT;

            if(RC_SUCC != lAddProcess(pstSavm, &stProcess, bType))
                fprintf(stderr, "登记任务(%s)的进程失败!\n", pstGroup->m_szPGName);

            continue;
        }

        ++ pstGroup->m_lCurNum;
        stProcess.m_lState    = BCS_APP_IDLE;
        fprintf(stdout, "   exec %s ", pstGroup->m_szPGName);
        if((lPid = lForkProcess(pstGroup->m_szPGName, &stProcess, bType)) <= RC_SUCC)
        {
            fprintf(stderr, "启动进程失败, %s\n", strerror(errno));
            continue;
        }

        for(j = 0; j < 200; j ++)
        {
            usleep(50000);
            if((bRet = bExistProcess(lPid)))    break;
        }

        if(!bRet)
        {
            lUpdateProcess(pstSavm, &stProcess, BCS_APP_STOP);
            fprintf(stdout, "... failed\n");
        }
        else
        {
            fprintf(stdout, "process id=%d ... success\n", lPid);
        }
        fflush(stdout);
    }

	if(!bExist)    return RC_FAIL;

    return lExec;
}

/**************************************************************************************************
    函 数 名：lBootProcess()
    功能描述：启动平台所有进程
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lBootProcess()
{
    CtsGroup    *pstGroup = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();
    long        i = 0, lCount = 0, lFaid = 0, lPos = 0, lExec = 0;
    
    if(RC_SUCC != lExportTable(CTS_PLAT_GROUP, &lCount, (void *)&pstGroup))
        return RC_FAIL;

    for(i = 0; i < lCount; i ++)
    {
        if(pstGroup[i].m_lMinNum <= 0 || pstGroup[i].m_lMinNum - pstGroup[i].m_lCurNum <= 0)
            continue;

        if(pstGroup[i].m_lQid > 0)    continue;

        //    先启动队列，在启动进程， 进程在启动的时候先访问队列
        if(RC_SUCC > (pstGroup[i].m_lQid = lCreateQueue(pstSavm, false)))
        {
            IBPerror("创建消息队列失败, %s\n", sGetTError(pstSavm->m_lErrno));
            IBPFree(pstGroup);
            return RC_FAIL;
        }

        if(RC_SUCC != lUpdateGroup(pstSavm, (void *)&pstGroup[i]))
        {
            IBPerror("更新进程(%s)组配置信息失败, err:(%s)", pstGroup[i].m_szPGName, 
                sGetTError(pstSavm->m_lErrno));
            IBPFree(pstGroup);
            return RC_FAIL;
        }
    }

    for(i = 0, lFaid = 0, lPos = 0; i < lCount; i ++)
    {
        if(pstGroup[i].m_lMaxNum <= 0 || (pstGroup[i].m_lMaxNum - pstGroup[i].m_lCurNum) <= 0)
            continue;

        pstGroup[i].m_lAppPos = lPos;
        if(RC_SUCC > (lExec = lExecProcess(pstSavm, &pstGroup[i], FALSE)))
            lFaid ++;

        if(RC_SUCC != lUpdateGroup(pstSavm, (void *)&pstGroup[i]))
        {
            IBPFree(pstGroup);
            IBPerror("更新进程(%s)组配置信息失败, err:(%s)", pstGroup[i].m_szPGName, 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        lPos += pstGroup[i].m_lMaxNum;
    }
    IBPFree(pstGroup);

    fprintf(stdout, "Boot platform all process is completed, success (%d) failed (%d)\n", 
        lCount - lFaid, lFaid);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lStopProcess(CtsGroup *pstGroup)
    功能描述：停止平台进程
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lStopProcess(CtsGroup *pstGroup)
{
    size_t        i = 0, lCount = 0;
    CtsProcess    stProcess, *pstProcess = NULL;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    memset(&stProcess, 0, sizeof(stProcess));
    stProcess.m_lHost = lSetEXLong(pstGroup->m_lHost);
    strncpy(stProcess.m_szPGName, pstGroup->m_szPGName, sizeof(stProcess.m_szPGName));
    
    pstSavm->lFind   = CHK_SELECT;
    pstSavm->tblName = CTS_PLAT_PROCESS;
    pstSavm->lSize   = sizeof(CtsProcess);
    pstSavm->pstVoid = &stProcess;
    if(RC_SUCC != lQuery(pstSavm, &lCount, (void **)&pstProcess))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            return RC_SUCC;

        IBPerror("获取平台进程(%s)(%d)信息失败, err:%s", pstGroup->m_szPGName,
            pstGroup->m_lHost, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(i = 0; i < lCount; i ++)
    {
        if(BCS_APP_DUMP == pstProcess[i].m_lState || BCS_APP_STOP == pstProcess[i].m_lState)
            continue;

        if(RC_SUCC != lStallProcess(pstSavm, &pstProcess[i], pstGroup))
        {
            IBPerror("进程(%d)退出失败, err:%s", pstProcess[i].m_lPid, sGetTError(pstSavm->m_lErrno));
            IBPFree(pstProcess);
            return RC_FAIL;
        }

        fprintf(stdout, "   stop %s process id=%d .... success\n", pstGroup->m_szPGName, 
            pstProcess[i].m_lPid);
    }
    IBPFree(pstProcess);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lDropQueue()
    功能描述：删除队列
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lDropQueue()
{
    CtsGroup    stGroup, *pstGroup = NULL;
    size_t        i = 0, lCount = 0, lFail = 0;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();
    
    if(RC_SUCC != lExportTable(CTS_PLAT_GROUP, &lCount, (void *)&pstGroup))
    {
        IBPerror("获取平台进程队列组失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(i = 0, lFail = 0; i < lCount; i ++)
    {
        if(pstGroup[i].m_lQid <= 0)
            continue;        

        //    清理队列
        msgctl(pstGroup[i].m_lQid, IPC_RMID, NULL);
        memcpy(&stGroup, &pstGroup[i], sizeof(CtsGroup));    
        pstGroup[i].m_lQid = 0;

        if(RC_SUCC != lUpdateGroup(pstSavm, &pstGroup[i]))
        {
            IBPFree(pstGroup);
            return RC_FAIL;
        }
    }
    IBPFree(pstGroup);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lRestartProcess(long lPid)
    功能描述：重启指定进程
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lRestartProcess(long lPid)
{
    long        lNPid;
    CtsGroup    stGroup;
    CtsProcess    stProcess;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    memset(&stGroup, 0, sizeof(stGroup));
    memset(&stProcess, 0, sizeof(stProcess));
    if(RC_SUCC != lGetProcess(pstSavm, &stProcess, lPid))
    {
        IBPerror("获取进程(%d)运行参数信息失败, err:%s", lPid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, stProcess.m_szPGName, stProcess.m_lHost))
    {
        IBPerror("获取进程(%s)配置信息失败, err:%s", stProcess.m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    //    停止父进程
    if(RC_SUCC != lShrinkProcess(pstSavm, &stProcess, &stGroup))
    {
        IBPerror("进程(%d)退出失败, err:%s", stProcess.m_lPid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC >= (lNPid = lRaiseProcess(&stProcess, stProcess.m_lRunExt)))
    {
        IBPerror("启动进程(%s)失败, err:%s\n", stProcess.m_szPGName, strerror(errno));
        return RC_FAIL;
    }

    IBPdebug("重启进程(%s)(%d)->(%d)成功!", stProcess.m_szPGName, lPid, lNPid);

    return lNPid;
}

/**************************************************************************************************
    函 数 名：lOffLineExec(char *pszPGName)
    功能描述：对指定的进程进行下线处理
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lOffLineGroup(char *pszPGName, long lHost)
{
    size_t      i, lCount = 0;
    CtsGroup    stGroup, *pstGroup = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    if(!pszPGName || !strlen(pszPGName))
        return RC_SUCC;

    if(!bIsTvmBoot())    //    平台未启动
    {
        fprintf(stderr, "\n平台未启动!\n\n");
        return RC_SUCC;
    }

    memset(&stGroup, 0, sizeof(CtsGroup));
	stGroup.m_lHost = lHost;
    strncpy(stGroup.m_szPGName, pszPGName, sizeof(stGroup.m_szPGName));

    pstSavm->pstVoid = &stGroup;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_GROUP;
    pstSavm->lSize   = sizeof(CtsGroup);
    if(RC_SUCC != lQuery(pstSavm, &lCount, (void **)&pstGroup))
    {
        fprintf(stderr, "获取进程(%s)组信息失败, %s", stGroup.m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(i = 0; i < lCount; i ++)
    {
        fprintf(stdout, "Shutdown host %d process(%s), current %d total %d process\n", 
            pstGroup[i].m_lHost, pstGroup[i].m_szPGName, pstGroup[i].m_lCurNum, 
            pstGroup[i].m_lMinNum);
        fflush(stdout);

        //    停止指定的执行码
        if(RC_SUCC != lStopProcess(&pstGroup[i]))
            return RC_FAIL;

        pstGroup[i].m_lState = STATUES_DISABLE;
        if(RC_SUCC != lUpdateGroup(pstSavm, &pstGroup[i]))
        {
            IBPerror("更新进程(%s)信息失败, %s", pstGroup[i].m_szPGName,
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lOnLineExec(char *pszPGName, long lHost)
    功能描述：对指定的进程进行上线处理
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lOnLineGroup(char *pszPGName, long lHost)
{
    long        i, lCount = 0;
    CtsGroup    stGroup, *pstGroup = NULL;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    if(!pszPGName || !strlen(pszPGName))
        return RC_SUCC;

    if(!bIsTvmBoot())    //  平台未启动
    {
        fprintf(stderr, "\n平台未启动!\n\n");
        return RC_SUCC;
    }

    memset(&stGroup, 0, sizeof(CtsGroup));
	stGroup.m_lHost = lHost;
    strncpy(stGroup.m_szPGName, pszPGName, sizeof(stGroup.m_szPGName));

    pstSavm->pstVoid = &stGroup;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_GROUP;
    pstSavm->lSize   = sizeof(CtsGroup);
    if(RC_SUCC != lQuery(pstSavm, &lCount, (void **)&pstGroup))
    {
        fprintf(stderr, "获取进程(%s)组信息失败, %s", stGroup.m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(i = 0; i < lCount; i ++)
    {
        if(pstGroup[i].m_lMinNum <= pstGroup[i].m_lCurNum || pstGroup[i].m_lMaxNum <= 0)
            continue;

        if(RC_SUCC > lExecProcess(pstSavm, &pstGroup[i], TRUE))
        {
            IBPerror("启动执行码(%s)出错, %s\n", pszPGName, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        pstGroup[i].m_lState = STATUES_ENABLE;   // 启动应用完毕，启动进程
        if(RC_SUCC != lUpdateGroup(pstSavm, &pstGroup[i]))
        {
            IBPerror("更新进程(%s)信息失败, %s", pstGroup[i].m_szPGName,
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lRebootProcess(SATvm *pstSavm, char *pszPGName)
    功能描述：启动系统指定进程组
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lRebootProcess(SATvm *pstSavm, char *pszPGName)
{
    size_t        i, lCount = 0;
    char        szProcess[512];
    CtsGroup    stGroup, *pstGroup = NULL;

    if(!pszPGName || !strlen(pszPGName))
        return RC_SUCC;

    if(!bIsTvmBoot())    //    平台未启动
    {
        fprintf(stderr, "\n平台未启动!\n\n");
        return RC_SUCC;
    }

    getcwd(szProcess, sizeof(szProcess));
    strcat(szProcess, "/");
    strcat(szProcess, stGroup.m_szPGName);
    if(RC_SUCC != access(szProcess, X_OK | F_OK))
    {
        IBPerror("当前目录下不存在执行文件(%s)", pstGroup[i].m_szPGName);
        return RC_FAIL;
    }

    memset(&stGroup, 0, sizeof(CtsGroup));
    strncpy(stGroup.m_szPGName, pszPGName, sizeof(stGroup.m_szPGName));

    pstSavm->tblName = CTS_PLAT_GROUP;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(CtsGroup);
    pstSavm->pstVoid = &stGroup;
    if(RC_SUCC != lQuery(pstSavm, &lCount, (void **)&pstGroup))
        return RC_FAIL;

    for(i = 0; i < lCount; i ++)
    {
        fprintf(stdout, "Shutdown host %d process(%s), current %d total %d process\n", 
            pstGroup[i].m_lHost, pstGroup[i].m_szPGName, pstGroup[i].m_lCurNum, 
            pstGroup[i].m_lMinNum);
        fflush(stdout);
        
        //    停止指定的执行码
        if(RC_SUCC != lStopProcess(&pstGroup[i]))
            return RC_FAIL;
        
        pstGroup[i].m_lState = STATUES_DISABLE;        //    首先该执行码禁用
        if(RC_SUCC != lUpdateGroup(pstSavm, &pstGroup[i]))
        {
            IBPerror("更新进程(%s)信息失败, %s", pstGroup[i].m_szPGName, 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }

    snprintf(szProcess, sizeof(szProcess), "%s/bin/%s", getenv("HOME"), stGroup.m_szPGName);
    if(RC_SUCC != rename(pszPGName, szProcess))
    {
        IBPerror("替换执行码(%s)出错, %s\n", pszPGName, strerror(errno));
        return RC_FAIL;
    }

    for(i = 0; i < lCount; i ++)
    {
        if(RC_SUCC > lExecProcess(pstSavm, &pstGroup[i], TRUE))
        {
            IBPerror("启动执行码(%s)出错, %s\n", pszPGName, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
        
        pstGroup[i].m_lState = STATUES_ENABLE;     //    启动应用完毕，启动进程
        if(RC_SUCC != lUpdateGroup(pstSavm, &pstGroup[i]))
        {
            IBPerror("更新进程(%s)信息失败, %s", pstGroup[i].m_szPGName, 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    }
}

/**************************************************************************************************
    函 数 名：lHaltProcess()
    功能描述：启动系统相关配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lHaltProcess()
{
    CtsGroup    *pstGroup = NULL;
    size_t      i = 0, lCount = 0, lFail = 0;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lExportTable(CTS_PLAT_GROUP, &lCount, (void *)&pstGroup))
    {
        IBPerror("获取平台进程组失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(i = 0, lFail = 0; i < lCount; i ++)
    {
        fprintf(stdout, "shutdown process(%s), current %d process\n", pstGroup[i].m_szPGName,
            pstGroup[i].m_lCurNum);
        fflush(stdout);

        if(RC_SUCC != lStopProcess(&pstGroup[i]))
            lFail ++;
        
        if(pstGroup[i].m_lQid > 0)    //    清理消息队列
        {
            msgctl(pstGroup[i].m_lQid, IPC_RMID, NULL);
            pstGroup[i].m_lQid = 0;
        }

        if(RC_SUCC != lUpdateGroup(pstSavm, &pstGroup[i]))
        {
            IBPFree(pstGroup);
            return RC_FAIL;
        }
    }
    IBPFree(pstGroup);
    fprintf(stdout, "shutdown platform all process is completed, success (%d) failed (%d)\n", 
        lCount - lFail, lFail);

    //    删除内存表
    lDropTable(pstSavm, CTS_PLAT_GROUP);
    lDropTable(pstSavm, CTS_PLAT_PROCESS);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lRecvSysInfo()
    功能描述：系统相关配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lRecvSysInfo(SATvm *pstSavm)
{
    CtsParam    stParam;
    FILE        *fp = NULL;
    char        szPath[1024];

    memset(szPath, 0, sizeof(szPath));
    snprintf(szPath, sizeof(szPath), "%s/etc/.%s", getenv("HOME"), SYSTEM_NAME);    

    if(RC_SUCC > (sizeof(CtsParam) != lGetFileSize(szPath)))
    {
        fprintf(stderr, "系统配置文件异常(%s)，正常启动!", strerror(errno));
        return RC_SUCC;
    }

    if(NULL == (fp = fopen(szPath, "rb")))
        return RC_SUCC;
    fread(&stParam, sizeof(CtsParam), 1, fp);
    fclose(fp);

    lSetSysCrDate(pstSavm, stParam.m_szCrDate);
    lSetSysSeqNo(pstSavm, stParam.m_lSeqNo);
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：pBootDefault()
    功能描述：TVM启动缺省参数
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
TBoot    *pBootDefault()
{
    FILE    *fp = NULL;
    long    lNum = 0;
    TBoot    *pstBoot = (TBoot *)pGetBoot();

    pstBoot->m_lExtern = 0;
    pstBoot->m_lBootType = TVM_BOOT_SIMPLE;
    pstBoot->m_lMaxTable = TVM_MAX_TABLE;
    pstBoot->m_lMaxField = 5120;
    pstBoot->m_lMaxDomain = 255;
    pstBoot->m_lMaxSeque = 100;
    pstBoot->m_lSvrBoot = get_nprocs();
    pstBoot->m_lSvrPort = 1801;
    pstBoot->m_lDomPort = 1802;
    strncpy(pstBoot->m_szNode, SYSTEM_NAME, sizeof(pstBoot->m_szNode));
    strncpy(pstBoot->m_szLog, DEFAULT_LOG, sizeof(pstBoot->m_szLog));

    if(RC_SUCC == (fp = fopen(getenv("TVMCFG"), "wb")))
    {
        fprintf(stderr, "创建缺省配置文件失败, err:%s\n", strerror(errno));
        return NULL;
    }

    fwrite(TVM_RUNCFG_TAG, 4, 1, fp);
    fwrite(pstBoot, sizeof(TBoot), 1, fp);
    fwrite(&lNum, sizeof(long), 1, fp);
    fwrite(&lNum, sizeof(long), 1, fp);
    fwrite(&lNum, sizeof(long), 1, fp);
    fclose(fp);
    
    return pstBoot;
}

/**************************************************************************************************
    函 数 名：lStartupParam()
    功能描述：启动系统相关配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lStartupParam(SATvm *pstSavm)
{
    CtsParam    stParam;
    TBoot       *pstBoot = (TBoot *)pBootDefault();

    memset(&stParam, 0, sizeof(CtsParam));
    if(!bIsTvmBoot() && RC_SUCC != lStartupTvm(pstBoot))
    {
        fprintf(stderr, "failed to boot TVM, (%s)\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    fprintf(stderr, "start TVM : (%s)\n", sGetTError(pstSavm->m_lErrno));
    if(RC_SUCC != lParseParam(pstSavm, &stParam))
        return RC_FAIL;

    vSetUserSize(stParam.m_lUserSize);
    stParam.m_lState = SYSTEM_PLT_BING;        //    刚启动设置为正在启动
    if(RC_SUCC != lCreateTable(pstSavm, CTS_PLAT_PARAM, 1, C_CTS_PLAT_PARAM))
    {
        fprintf(stderr, "创建系统运行参数表失败, (%s)\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lImportTable(CTS_PLAT_PARAM, 1, (void *)&stParam))
    {
        fprintf(stderr, "导入系统参数表失败, (%s)\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lCreateTable(pstSavm, CTS_ASYN_IDENTIFY, stParam.m_lMaxAsyn, 
        C_CTS_ASYN_IDENTIFY))
    {
        fprintf(stderr, "创建异步标识表失败, (%s)\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lCreateTable(pstSavm, CTS_USER_KEYCODE, stParam.m_lMaxKey, C_CTS_USER_KEYCODE))
	{
        fprintf(stderr, "创建用户定义密码表失败, (%s)\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lCreateTable(pstSavm, CTS_RUNTIME_POOL, stParam.m_lPoolNum, 
        C_CTS_RUNTIME_POOL))
    {
        fprintf(stderr, "创建交易共享池表失败, (%s)\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    lRecvSysInfo(pstSavm);

    //  加载系统运行参数配置信息    
    if(RC_SUCC != lInitSetup(pstSavm))
    {
        fprintf(stderr, "加载运行配置信息失败, (%s)\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return lInitAllSatvm(pstSavm);
}

/**************************************************************************************************
    函 数 名：lRebootSystem()
    功能描述：重新启动进程
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lRebootSystem(SATvm *pstSavm)
{
    CtsParam    stParam;
    CfgHead        *pstRoot = NULL;
    CtsGroup    *pstGroup = NULL;
    size_t        lCount = 0, lRet;

    if(bTableIsExist(CTS_PLAT_GROUP) && bTableIsExist(CTS_PLAT_PROCESS))
    {
        fprintf(stdout, "平台已启动，无须再次启动\n");
        return RC_SUCC;
    }

    if(RC_SUCC != lInitialGroup(pstSavm))
        return RC_FAIL;

    //  将平台设置正在启动状态, 如果不设置那么进程可能无法找到进行运行信息 
    if(RC_SUCC != lSetSysState(pstSavm, SYSTEM_PLT_BING))
         return RC_FAIL;

    if(RC_SUCC != lBootProcess())
    {
        IBPerror("启动配置进程失败!");
        return RC_FAIL;
    }

    //    将平台设置启动状态
    if(RC_SUCC != lSetSysState(pstSavm, SYSTEM_PLT_START))
        return RC_FAIL;

    usleep(50000);
    fprintf(stdout, "启动平台所有进程成功!\n\n");

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vExpRunTime(SATvm *pstSavm, char *pszFile)
    功能描述：启动系统相关配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
void    vExpRunTime(SATvm *pstSavm, char *pszFile)
{
    CtsParam    stParam;
    xmlNodePtr  pxRoot = NULL;
    RunTime     *pstRun = NULL;
    SHTruck     *pstTruck = NULL;
    size_t      lIdx = 0, lOffset;
    xmlDocPtr   pDoc = (xmlDocPtr)pGetDoc();
    GCts        *pstCts = (GCts *)pGetGloble();

    if(!pszFile || !strlen(pszFile))    return ;

    if(!bIsTvmBoot())
    {
        fprintf(stderr, "\n平台未启动!\n\n");
        return ;
    }

    if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
    {
        fprintf(stderr, "获取系统配置失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return ;
    }
    vSetUserSize(stParam.m_lUserSize);    //    初始化共享池大小

    pDoc   = xmlNewDoc(BAD_CAST"1.0");
    pxRoot = xmlNewNode(NULL, (xmlChar *)"cts");
    if(!pxRoot)
    {
        fprintf(stderr, "新建XML节点cts失败!\n");
        return ;
    }
    xmlDocSetRootElement(pDoc, pxRoot);

    //    导出系统信息
    if(RC_SUCC != lExportParam(pxRoot, &stParam))
        goto ErrRecyle;

    pstSavm->tblName = CTS_RUNTIME_POOL;
    if(NULL == (pstRun = (RunTime *)pInitHitTest(pstSavm, pstSavm->tblName)))
        goto ErrRecyle;

    for(lOffset = lGetTblData(pstSavm->tblName); lOffset <= lGetTableSize(pstSavm->tblName);
        lOffset += lGetRowTruck(pstSavm->tblName))
    {
        pstTruck = (PSHTruck)pGetNode(pstRun->m_pvAddr, lOffset);
        if(IS_TRUCK_NULL(pstTruck))
            continue;

        if(RC_SUCC != lExportGloble(pxRoot, (GCts *)pstTruck->m_pvData, ++ lIdx))
        {
            vTblDisconnect(pstSavm, pstSavm->tblName);
            goto ErrRecyle;
        }
    }
    vTblDisconnect(pstSavm, pstSavm->tblName);

    if(RC_FAIL == xmlSaveFormatFileEnc(pszFile, pDoc, "UTF-8", 1))
    {
        fprintf(stdout, "一个xml文档被创建,写入字节\n");
        return ;
    }

ErrRecyle:
    xmlFreeDoc(pDoc);
    pDoc = NULL;
    xmlCleanupParser();
    xmlMemoryDump();
    return ;
}

/**************************************************************************************************
    函 数 名：lSaveSysInfo()
    功能描述：保存系统相关配置信息
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lSaveSysInfo(SATvm *pstSavm)
{
    CtsParam    stParam;
    FILE        *fp = NULL;
    char        szPath[1024];

    memset(szPath, 0, sizeof(szPath));
    memset(&stParam, 0, sizeof(CtsParam));
    snprintf(szPath, sizeof(szPath), "%s/etc/.%s", getenv("HOME"), SYSTEM_NAME);    
    if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
    {
        IBPerror("获取系统信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_FAIL == (stParam.m_lSeqNo = lGetSystSeqNo(pstSavm)))
    {
        IBPerror("获取系统流水号失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(NULL == (fp = fopen(szPath, "w")))
    {
        IBPerror("打开配置文件(%s)失败, err:%s", szPath, strerror(errno));
        return RC_FAIL;
    }

    fwrite(&stParam, sizeof(CtsParam), 1, fp);
    fclose(fp);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lNotifyPack(char *pszPGName, long lHost)
    功能描述：在修改报文配置后，需要通知报文处理进程重新刷新二级缓存
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lNotifyPack(char *pszPGName, long lHost)
{
    CtsEvent    stEvent;
    CtsGroup    stGroup;
    size_t        i, lCount = 0;
    CtsProcess  stProcess, *pstProcess = NULL;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    if(!pszPGName || 0 == strlen(pszPGName))    return RC_SUCC;

    memset(&stProcess, 0, sizeof(stProcess));
    if(RC_SUCC != lGetGroup(pstSavm, &stGroup, pszPGName, lHost))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            return RC_SUCC;

        IBPerror("获取进程(%s)配置信息失败, err:%s", pszPGName, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    stProcess.m_lHost = lSetEXLong(stGroup.m_lHost);
    strncpy(stProcess.m_szPGName, stGroup.m_szPGName, sizeof(stProcess.m_szPGName));

    pstSavm->lFind   = CHK_SELECT;
    pstSavm->tblName = CTS_PLAT_PROCESS;
    pstSavm->lSize   = sizeof(CtsProcess);
    pstSavm->pstVoid = &stProcess;
    if(RC_SUCC != lQuery(pstSavm, &lCount, (void **)&pstProcess))
    {
        IBPerror("获取进程(%s)别表信息失败, err:%s", stProcess.m_szPGName, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(i = 0; i < lCount; i ++)
    {
        if(pstProcess[i].m_lPid <= 0)
            continue;

        //    对于未启动的进程，不需刷新二级缓存
        if(BCS_APP_BUSY != pstProcess[i].m_lState && BCS_APP_IDLE != pstProcess[i].m_lState)
            continue;

        memset(&stEvent, 0, sizeof(CtsEvent));
        stEvent.m_lState = EVENT_EVCACHE;
        stEvent.m_lMType = pstProcess[i].m_lPid;
        stEvent.m_lResv  = getpid();
        if(RC_SUCC != lEventWrite(pstSavm, stGroup.m_lQid, &stEvent, sizeof(CtsEvent)))
        {
            IBPerror("发送队列(%d)进程(%s)(%d)重置信号失败, err:%s", stGroup.m_lQid,
                stProcess.m_szPGName, pstProcess[i].m_lPid, sGetTError(pstSavm->m_lErrno));
            IBPFree(pstProcess);
            return RC_FAIL;
        }
    }

    IBPFree(pstProcess);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lRefreshPack(long lMid)
    功能描述：在修改报文配置后，需要对应的进程刷新缓存
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lRefreshPack(long lMid)
{
    CtsEvent    stEvent;
    size_t        i, lCount = 0;
    CtsHost        stHost;
    CtsClass    *pstClass = NULL;
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    if(lMid <= 0)    return RC_SUCC;

    if(RC_SUCC != lExportTable(CTS_TRADE_CLASS, &lCount, (void *)&pstClass))
    {
        IBPerror("获取所有组件信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    for(i = 0; i < lCount; i ++)
    {
        if(lMid == pstClass[i].m_lInMid || lMid == pstClass[i].m_lOutMid || 
            lMid == pstClass[i].m_lErrMid)
            break;
    }

    if(lCount <= i)    return RC_SUCC;
    
    if(RC_SUCC != lGetHost(pstSavm, &stHost, pstClass[i].m_lHost))
    {
        IBPerror("获取主机(%d)信息失败, err:%s", pstClass[i].m_lHost, 
            sGetTError(pstSavm->m_lErrno));
        IBPFree(pstClass);
        return RC_FAIL;
    }
    IBPFree(pstClass);

    return lNotifyPack(stHost.m_szPGName, stHost.m_lHost);
}

/**************************************************************************************************
    函 数 名：lResetSAinit()
    功能描述：通知进程重新重置二级缓存
    返回说明：
        RC_SUCC            --成功
        RC_FAIL            --失败
 **************************************************************************************************/
long    lResetSAinit()
{
    CtsEvent    stEvent;
    CtsGroup    *pstGroup = NULL;
    size_t      i, j, lCount = 0, lNum = 0;
    CtsProcess  stProcess, *pstProcess = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();
    
    if(RC_SUCC != lExportTable(CTS_PLAT_GROUP, &lCount, (void *)&pstGroup))
        return RC_FAIL;

    for(i = 0; i < lCount; i ++)
    {
        if(pstGroup[i].m_lCurNum <= 0)
            continue;

        memset(&stProcess, 0, sizeof(CtsProcess));
        stProcess.m_lHost = lSetEXLong(pstGroup[i].m_lHost);
        strcpy(stProcess.m_szPGName, pstGroup[i].m_szPGName);

        pstSavm->lFind   = CHK_SELECT;
        pstSavm->tblName = CTS_PLAT_PROCESS;
        pstSavm->lSize   = sizeof(stProcess);
        pstSavm->pstVoid = &stProcess;
        if(RC_SUCC != lQuery(pstSavm, &lNum, (void **)&pstProcess))
        {
            IBPerror("获取平台进程(%s)(%d)信息失败, %s", pstGroup[i].m_szPGName, 
                stProcess.m_lHost, sGetTError(pstSavm->m_lErrno));
            IBPFree(pstGroup);
            return RC_FAIL;
        }

        for(j = 0; j < lNum; j ++)
        {
            if(pstProcess[j].m_lPid <= 0)
                continue;

            //    对于未启动的进程，不需刷新二级缓存
            if(BCS_APP_BUSY != pstProcess[j].m_lState && BCS_APP_IDLE != pstProcess[j].m_lState)
                continue;

            memset(&stEvent, 0, sizeof(CtsEvent));
            stEvent.m_lState = EVENT_EVCACHE;
            stEvent.m_lMType = pstProcess[j].m_lPid;
            stEvent.m_lResv  = getpid();
            if(RC_SUCC != lEventWrite(pstSavm, pstGroup[i].m_lQid, &stEvent, sizeof(CtsEvent)))
            {
                IBPerror("发送队列(%d)进程(%s)(%d)重置信号失败, err:%s", pstGroup[i].m_lQid,
                    pstProcess[j].m_szPGName, pstProcess[j].m_lPid, sGetTError(pstSavm->m_lErrno));
                break;
            }
        }
        IBPFree(pstProcess);
    }
    IBPFree(pstGroup);

    vLogSetPath("%s/%s", getenv("LOG_FILE_PATH"), sGetDate());
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vResetObject(char *pszOpt)
    功能描述：重置运行参数信息
    返回说明：
        无
 **************************************************************************************************/
void    vResetObject(char *pszOpt)
{
    long    lRet = 0, lQid = 0;
    SATvm    *pstSavm = (SATvm *)pGetSATvm();
    
    if(!bIsTvmBoot())
    {
        fprintf(stderr, "\n平台未启动!\n\n");
        return ;
    }

    if(!strcasecmp(pszOpt, "i"))
    {
        if(RC_SUCC != lCleanupAsyn())
        {
            fprintf(stderr, "清理运行参数数据失败, %s\n", sGetError());
            return ;
        }
    }
    else if(!strcasecmp(pszOpt, "c"))
    {
        //    同步一下交易日期
        if(RC_SUCC != lSetSysTxDate(pstSavm, sGetDate()))
        {
            fprintf(stderr, "同步系统交易日期失败, err:%s", sGetError());
            return ;
        }

        if(RC_SUCC != lResetSAinit())                //    重置二级缓存
        {
            fprintf(stderr, "通知进程同步二级缓存失败!");
            return ;
        }
    }
    else
    {
        if(0 > (lQid = atol(pszOpt)))
        {
            fprintf(stderr, "\n待清理的队列参数有误!\n\n");
            return ;
        }
        
        if(RC_FAIL == (lRet = lResetQueue(pstSavm, lQid)))
        {
            fprintf(stderr, "\n清理队列(%d)失败, err:%s!\n\n", lQid, sGetTError(pstSavm->m_lErrno));
            return ;
        }

        fprintf(stdout, "\n清理队列编号(%d)个数:%d\n\n", lQid, lRet);

        return ;
    }

    fprintf(stdout, "\n重置&清理参数成功，completed successfully!!!\n\n");
}

/**************************************************************************************************
    函 数 名：lRefreshParam(char *pszOpt)
    功能描述：重置运行参数信息
    返回说明：
        无
 **************************************************************************************************/
long    lRefreshParam(SATvm *pstSavm)
{
    CtsParam    stUpdate, stParam;

    if(!bIsTvmBoot())
    {
        fprintf(stderr, "平台未启动!");
        return RC_FAIL;
    }

    memset(&stParam, 0, sizeof(CtsParam));
    memset(&stUpdate, 0, sizeof(CtsParam));
    if(RC_SUCC != lGetSysParam(pstSavm, &stParam))
    {
        fprintf(stderr, "获取系统配置信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;    
    }

    if(RC_SUCC != lParseParam(pstSavm, &stUpdate))
        return RC_FAIL;    

    strcpy(stUpdate.m_szCrDate, stParam.m_szCrDate);          //  重启后生效
    strcpy(stUpdate.m_szTxDate, stParam.m_szTxDate);          //  重启后生效
    strcpy(stUpdate.m_szSysName, stParam.m_szSysName);        //  重启后生效
    stUpdate.m_lSeqNo        = stParam.m_lSeqNo;              //  重启后生效
    stUpdate.m_lState        = stParam.m_lState;              //  重启后生效
    stUpdate.m_lTrCount      = stParam.m_lTrCount;            //  重启后生效
    stUpdate.m_lSuccCount    = stParam.m_lSuccCount;          //  重启后生效
    stUpdate.m_lFailCount    = stParam.m_lFailCount;          //  重启后生效
    stUpdate.m_lUserSize     = stParam.m_lUserSize;           //  重启后生效
    stUpdate.m_lPoolNum      = stParam.m_lPoolNum;            //  重启后生效
    stUpdate.m_lMaxServers   = stParam.m_lMaxServers;         //  重启后生效
    stUpdate.m_lMaxProcess   = stParam.m_lMaxProcess;         //  重启后生效
    stUpdate.m_lMaxAsyn      = stParam.m_lMaxAsyn;            //  重启后生效
    stUpdate.m_lMaxKey       = stParam.m_lMaxKey;             //  重启后生效

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_PARAM;
    pstSavm->lSize   = sizeof(CtsParam);
    pstSavm->pstVoid = (void *)&stParam;
    if(RC_SUCC != lUpdate(pstSavm, (void *)&stUpdate, false))
    {
        IBPerror("更新运行配置信息失败, err:%s", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;    
    }

    return RC_SUCC;
}

/**************************************************************************************************
    code end
 **************************************************************************************************/
