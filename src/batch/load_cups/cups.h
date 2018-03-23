/****************************************************************************************
 *    文 件 名  : cups.h
 *    作    者  : DeffPuzzL
 *    版    本  : V2.0.0.0
 *    创建日期  : 2016-01-08
 *    描    述  : 渠道文件
 * ****************************************************************************************/
#ifndef        HH_CUPS_LOAD_DEFINE__XXX
#define        HH_CUPS_LOAD_DEFINE__XXX

/****************************************************************************************
    宏定义
 ****************************************************************************************/
#define    FILE_FIXD_VALUE               1
#define    FILE_BLOCK_VALUE              2
#define    FILE_EXPRESS_VALUE            32
#define    FILE_COMBINA_VALUE            64
#define    FILE_VARIETY_VALUE            128


#define    FILE_VALUE_TYPE               1
#define    NAME_VALUE_TYPE               2

#define    ECP_CONST_STRING              0
#define    ECP_CONST_CONDIT              1
#define    ECP_CONST_COMBINA             2
#define    ECP_CONST_LESS                3
#define    ECP_CONST_STRCMP              4
#define    ECP_CONST_STRSTR              5
#define    ECP_CONST_SUBSTR              6
#define    ECP_CONST_DECODE              7
#define    ECP_CONST_LENGTH              8
#define    ECP_CONST_OPERATE             9

#define    REVOCATE_SYSREF               1
#define    REVOCATE_TRACE                2
#define    REVOCATE_CUPKEY               3


#define    MAX_LINE_RECORED              1024

#define    IBPTrimAbout(p)               sTrimLeft(p); sTrimRight(p);

/****************************************************************************************
    宏定义
 ****************************************************************************************/
#define    IBPMalloc(p, l)        p = (char *)malloc(l);    \
    if(!p)    \
    {   \
        IBPerror("分配内存(%d)失败!\n", l);   \
        return NULL;    \
    }   \
    memset(p, 0, l);


#define IBPIsVariable(var, p)   if('$' != var[0] || 0 == strlen(var + 1)) \
{   \
    IBPerror("设置标签[%s]错误", var); \
    if(p) \
        IBPFree(p); \
    return RC_FAIL; \
}

#define    IBPClearCfg(n, p)        if(p)    \
{    \
    n = 0;    \
    IBPFree(p);    \
}    

#define    IBPLoadInitial()    vSetIgnore(false); vSetRevocate(false); \
    memset(g_stLoad.szLine, 0, sizeof(g_stLoad.szLine));

/****************************************************************************************
    冲正流水记录结构
 ****************************************************************************************/
typedef struct __RELIST
{
    char        m_szSysRefNo[50];                //    撤销关键字段
    char        m_szTxDate[9];                    //    日期
    struct    __RELIST    *pstNext;
    struct    __RELIST    *pstLast;    
}ReList;

/****************************************************************************************
    组合字段结构记录信息
 ****************************************************************************************/
typedef    struct    __COMBFILED
{
    long        lNum;                            //    组合字段个数
    long        *plIdx;                            //    字段值索引
    char        szFmt[50];                        //    格式字符串
    char        szExecFunc[50];                    //    特殊处理函数
}ComFiled;

/****************************************************************************************
    字段ID结构定义
 ****************************************************************************************/
typedef    struct    __IDXVALUE
{
    long        lVType;                            //    取值方式 从文件列中，还是字段中
    long        lIdx;                            //    索引序号
}IdxValue;

/****************************************************************************************
    表达式结构信息
 ****************************************************************************************/
typedef    struct    __CUPEXPRESS
{
    long        lConst;                            //    是否表达式类型 0、常数 
    long        lNum;                            //    表达式字段个数
    IdxValue    *pstIdx;                        //    取值方式
    char        szResult[300];                    //    结果
    char        szIsMatch[300];                    //    匹配结果
    char        szNotMatch[300];                //    不匹配结果
}CUPExpress;

/****************************************************************************************
    字段结构定义信息
 ****************************************************************************************/
typedef    struct    __FILEDDEF
{
    long        lId;                            //    字段ID，一个渠道配置内ID唯一
    char        szFieldName[50];                //    字段名称
    char        szFieldValue[256];                //    字段值
    long        lValueType;                        //    字段取值类型FIELD_CHAR, FIELD_LONG, FIELD_DOUBLE
    long        lFieldType;                        //    取值方式
    char        szFieldCut[20];                    //    分隔阀值
    long        lFiledFrom;                        //    定长取值位置
    long        lFiledPos;                        //    位置索引/有效长度
    char        szExFixFunc[50];                //    修复或者校验函数定义，NULL标识采用缺省, 不设定值，则不校验 
    ComFiled    stComb;                            //    组合结构定义
    CUPExpress    stExp;                            //    表达式定义
    struct    __FILEDDEF    *pstNext;                //    指向下一个字段
    struct    __FILEDDEF    *pstLast;                //    指向上一个字段
}FiledDef;

/****************************************************************************************
    路由检查结构信息
 ****************************************************************************************/
typedef    struct    __ROUTE
{
    long        lType;                            //    路由检查类型，目前只有3中，0表达不检查
    char        szCNo[2][20];                    //    路由检查时候的渠道编号
    char        szFd42[2][20];                    //    渠道商户号
    char        szTerm[2][20];                    //    渠道终端编号
    char        szFeeCheckFunc[50];                //    手续费核对函数
}Route;

/****************************************************************************************
    撤销结构定义信息
 ****************************************************************************************/
typedef    struct    __REVOCATE
{
    BOOL        bLoad;                            //    冲正流水是否装载    
    char        szRevocFunc[50];                //    冲正方法
    long        lNum;                            //    表达式字段个数
    IdxValue    *pstIdx;                        //    取值方式
    ReList        *pstReList;                        //    冲正流水头结点
}Revocate;

/****************************************************************************************
    单个渠道配置信息定义
 ****************************************************************************************/
typedef    struct    __CUPSCFG
{
    char        szCupsNo[5];                    //    渠道定义
    Route        stRoute;                        //    路由定义
    FiledDef    *pstFiled;                        //    字段定义
    Revocate    stRevoct;                        //    撤销定义
    char        szLoadFunc[50];                    //    装载去掉必要行
}CupsCfg;

/****************************************************************************************
    运行参数结构信息
 ****************************************************************************************/
typedef    struct    __LOADRUN
{
    char    szCupsNo[5];                        //    当前渠道编号
    char    szCupsName[60];                        //    当前渠道名称
    char    szDate[9];                            //    运行装载日期
    char    szFileName[256];                    //  当前装载文件名
    long    lLine;                                //    文件读取当前行
    long    lVaild;                                //    装载有效记录
    BOOL    bRevocate;                            //    是否满足冲正条件
    BOOL    bIgnore;                            //    是否为忽略流水
    BOOL    bIsExit;                            //    装载本渠道是否报错退出
    char    szLine[MAX_LINE_RECORED];            //    文件行记录
}LoadRun;

/****************************************************************************************
        函数申明
 ****************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
extern  char*   sGetLoadCups();                    //    装载时运行的渠道编号
extern  char*   sGetLoadDate();                    //    装载日期

extern    void    vSetLoadCups(char *p);            //    设置运行时的渠道编号
extern    void    vSetLoadDate(char *p);            //    设置运行装载日期
extern    char*   sGetLoadContent();                //    读入行数据

extern    BOOL    bIsIgnore();                    //    该行流水是否忽略
extern    void    vSetIgnore(BOOL bf);            //    设置改行是否忽略

extern    BOOL    bIsRevocate();                    //    该行流水是否为撤销流水
extern    void    vSetRevocate(BOOL bf);            //    设置该笔流水为撤销流水


#ifdef __cplusplus
}
#endif

#endif        //    HH_CUPS_LOAD_DEFINE__XXX

/****************************************************************************************
    code end
 ****************************************************************************************/
