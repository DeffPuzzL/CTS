#define _GNU_SOURCE
#include    "tvm.h"
#include    "tmain.h"
#include    <readline/readline.h>
#include    <readline/history.h>


#define     DEL_TAIL_CHAR(s,c)         if(c == s[strlen(s) - 1])    s[strlen(s) - 1] = 0x00;
/*************************************************************************************************
 * 函数区
 *************************************************************************************************/
Benum  elog  =  0;
extern char     **environ;
extern long     lShutdownTvm();
extern long     lStartupTvm(TBoot *pstBoot);

#define    UNLOAD(fp, ...)        do{if(fp)    fprintf(fp, __VA_ARGS__); }while(0)

/*************************************************************************************************
    函数说明：获取TVM版本
    参数说明：
    返回值：
        RC_SUCC                    --成功
        RC_FAIL                    --失败
  *************************************************************************************************/
static  char*   sGetTVMVers()
{
    struct  utsname sinf;
    static  char    szVersion[512];

    memset(szVersion, 0, sizeof(szVersion));
    sprintf(szVersion, "Compile %s %s on ", __DATE__, __TIME__);
    if(RC_SUCC != uname(&sinf))
        strcat(szVersion, "Linux");
    else
        strcat(szVersion, sinf.sysname);

    if(8 == sizeof(long))
        strcat(szVersion, "&Oracle x64 Runtime\n");
    else
        strcat(szVersion, "&Oracle x32 Runtime\n");

    sprintf(szVersion + strlen(szVersion), "Release TVM C/C++ Library Version V%s on", TVM_VERSION);
    if(8 == sizeof(long))
        strcat(szVersion, " Linux x64\n");
    else
        strcat(szVersion, " Linux x32\n");

    strcat(szVersion, "Author：DeffPuzzL\n");
    strcat(szVersion, "Mail：liuzhenhao@iboxpay.com\n");

    return szVersion;
}

/*************************************************************************************************
    函数说明：屏幕输出调试信息
    参数说明：
    返回值：
        无
 *************************************************************************************************/
void	vSCRDebug(const char *fmt, ...)
{
	char	szMsg[1024];
	va_list ap;

	if(0 == elog)	return ;

	memset(szMsg, 0, sizeof(szMsg));
	va_start(ap, fmt);
	vsnprintf(szMsg, sizeof(szMsg), fmt, ap);	
	va_end(ap);

	fprintf(stdout, "%s\n", szMsg);
}

/*************************************************************************************************
    函数说明：获取Domain版本
    参数说明：
    返回值：
        无
 *************************************************************************************************/
char*   sFuncVersion()
{
    struct  utsname sinf;
    static  char    szVersion[512];
    
    memset(szVersion, 0, sizeof(szVersion));
    sprintf(szVersion, "Compile %s %s on ", __DATE__, __TIME__);
    if(RC_SUCC != uname(&sinf))
        strcat(szVersion, "Linux");
    else
        strcat(szVersion, sinf.sysname);
    
    sprintf(szVersion + strlen(szVersion), " Release %s Production on", TVM_VERSION);
    if(8 == sizeof(long)) 
        strcat(szVersion, " Linux x64\n\n");
    else
        strcat(szVersion, " Linux x32\n\n");
    
    strcat(szVersion, "Copyright (C) 2016-2020 盒子支付(iBOXPAY.com) Technology Co., "
        "Ltd. All Rights Reversed\n");
    
    return szVersion;
}

/*************************************************************************************************
    函数说明：尾部节点
    参数说明：
    返回值：
        SQLFld                        --尾部节点
  *************************************************************************************************/
SQLFld* pGetFieldTail(SQLFld *pstRoot)
{
    SQLFld  *pstList = pstRoot;

    if(!pstList)    return NULL;

    while(pstList->pstNext)
        pstList = pstList->pstNext;

    return pstList;
}

/*************************************************************************************************
    函数说明：新增节点
    参数说明：
    返回值：
        SQLFld                        --尾部节点
  *************************************************************************************************/
SQLFld* pInsertFiled(SQLFld *pstRoot, SQLFld *pstNode)
{
    SQLFld  *pstNext = NULL, *pstTail = pGetFieldTail(pstRoot);

    if(NULL == (pstNext = (SQLFld *)malloc(sizeof(SQLFld))))
        return pstRoot;

    memcpy(pstNext, pstNode, sizeof(SQLFld));
    pstNext->pstNext = NULL;

    if(!pstRoot)
        pstRoot = pstNext;
    else
        pstTail->pstNext = pstNext;

    return pstRoot;
}

/*************************************************************************************************
    函数说明：清理链表
    参数说明：
    返回值：
        SQLFld                        --尾部节点
  *************************************************************************************************/
void    vDestroyFiled(SQLFld *pstRoot)
{
    SQLFld  *pstNode = pstRoot, *pstList = NULL;

    while(pstNode)
    {
        pstList = pstNode;
        pstNode = pstNode->pstNext;
        IBPFree(pstList);
    }
}

/*************************************************************************************************
    函数说明：获取节点个数
    参数说明：
    返回值：
        SQLFld                        --尾部节点
  *************************************************************************************************/
long    lGetNodeFiled(SQLFld *pstRoot)
{
    long    lCnt;
    SQLFld  *pstNode = NULL;

    for(pstNode = pstRoot, lCnt = 0; pstNode; pstNode = pstNode->pstNext, lCnt ++);

    return lCnt;
}

/*************************************************************************************************
    函数说明：排序
    参数说明：
    返回值：
        SQLFld                        --尾部节点
  *************************************************************************************************/
SQLFld*    pSortSQLFiled(SQLFld *pstRoot)
{
    TblKey   stKey;
    SQLFld   *pstNode = NULL, *pstList = NULL;

    for(pstNode = pstRoot; pstNode; pstNode = pstNode->pstNext)
    {
        for(pstList = pstNode->pstNext; pstList; pstList = pstList->pstNext)
        {
            if(pstNode->m_stKey.m_lFrom < pstList->m_stKey.m_lFrom)
                continue;

            memcpy(&stKey, &pstNode->m_stKey, sizeof(TblKey));
            memcpy(&pstNode->m_stKey, &pstList->m_stKey, sizeof(TblKey));
            memcpy(&pstList->m_stKey, &stKey, sizeof(TblKey));
        }
    }

     return pstRoot;
}

/*************************************************************************************************
    函数说明：返回当前时间毫秒
    参数说明：
    返回值：
        无
  *************************************************************************************************/
long    lGetTiskTime()
{
    long    lTime;
    struct timeval  t;
    struct tm   *ptm;

    gettimeofday(&t, NULL);
    ptm = localtime(&t.tv_sec);

    lTime = ptm->tm_min * 60 * 1000 + ptm->tm_sec * 1000 + t.tv_usec / 1000;    //微秒（除以1000就是毫秒)
    return lTime;
}

/*************************************************************************************************
    函数说明：计算消耗时间毫秒
    参数说明：
    返回值：
        无
  *************************************************************************************************/
char*   sGetCostTime(long lTime)
{
    static  char    szTime[30];

    memset(szTime, 0, sizeof(szTime));

    if(lTime < 0)
        return szTime;
    if(lTime < 1000)
        snprintf(szTime, sizeof(szTime), "cost:%dms", lTime);
    else
        snprintf(szTime, sizeof(szTime), "cost:%ds%dms", lTime / 1000, lTime % 1000);
    return szTime;
}

/*************************************************************************************************
    函数说明：打印系统索引
    参数说明：
    返回值：
        无
  *************************************************************************************************/
void    vPrintIndex()
{
    TIndex  stIndex;
    SATvm   *pstSavm = NULL;
    long    lRet = 0, nRecord = 0, lTime = 0;

    memset(&stIndex, 0, sizeof(TIndex));
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_INDEX)))
    {
        fprintf(stderr, "初始化表(SYS_TABLE_IDX)失败, %s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    pstSavm->lFind = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->lSize = sizeof(TIndex);
    lRet = lTableDeclare(pstSavm, SYS_TVM_INDEX);
    if(RC_SUCC != lRet)
    {
        fprintf(stderr, "定义表(SYS_TABLE_IDX)游标失败, err:(%d)(%s)\n", pstSavm->m_lErrno, 
            sGetTError(pstSavm->m_lErrno), lGetEffect());
        return ;
    }
    fprintf(stdout, "------------------------------------------------------------------"
        "------------------------------------------------------------------------------\n");
    fprintf(stdout, "line  table type    table_name              mark          ownner     Key"
        "     shm_id   sem_id   maxrows size flag stat pers     update-time   \n");
    fprintf(stdout, "------------------------------------------------------------------"
        "------------------------------------------------------------------------------\n");
    lTime = lGetTiskTime();
    while(1)
    {
        memset(&stIndex, 0, sizeof(stIndex));
        lRet = lTableFetch(pstSavm, SYS_TVM_INDEX, (void *)&stIndex);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "获取表(SYS_TABLE_IDX)数据失败, err:(%d)(%s)\n", pstSavm->m_lErrno, 
                sGetTError(pstSavm->m_lErrno), lGetEffect());
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        nRecord ++;

        fprintf(stdout, "[%3d]: %4d|%4d|%-22s|%-15s|%10s|%8d|%8d|%8d|%8d|%4d|%4d|%4d|%4d|%20s|\n",
            nRecord, stIndex.m_table, stIndex.m_lType, stIndex.m_szTable, stIndex.m_szPart, 
            stIndex.m_szOwner, stIndex.m_yKey, stIndex.m_shmID, stIndex.m_semID, stIndex.m_lMaxRows, 
            stIndex.m_lRowSize, stIndex.m_lLocal, stIndex.m_lState, stIndex.m_lPers, stIndex.m_szTime);
    }
    lTime -= lGetTiskTime();
    vTableClose(pstSavm);
    fprintf(stdout, "----------------------------------------------------------------------%s----"
        "--------------------------------------------------------------\n", sGetCostTime(-1 * lTime));

    return ;
}

/*************************************************************************************************
    函数说明：打印表字段信息
    参数说明：
    返回值：
        无
  *************************************************************************************************/
void    vPrintField()
{
    TField    stField;
    SATvm   *pstSavm = NULL;
    char    szPrint[256];
    long    lRet = 0, lRow = 0, lTime = 0;

    memset(&stField, 0, sizeof(TField));
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_FIELD)))
    {
        fprintf(stderr, "初始化表(SYS_TVM_FIELD)失败\n");
        return ;
    }

    pstSavm->lFind = IDX_SELECT;
    pstSavm->lSize = sizeof(TField);
    lRet = lTableDeclare(pstSavm, SYS_TVM_FIELD);
    if(RC_SUCC != lRet)
    {
        fprintf(stderr, "定义表(SYS_TVM_FIELD)游标失败(%d), err:(%d)(%s)\n", pstSavm->m_lErrno, 
            pstSavm->lSize, sGetTError(pstSavm->m_lErrno), lGetEffect());
        return ;
    }
    fprintf(stdout, "------------------------------------------------------------------------"
        "------------------------\n");
    fprintf(stdout, "line  table seq      owner                table_name           field_name"
        "       attr   len  PK\n");
    fprintf(stdout, "------------------------------------------------------------------------"
        "------------------------\n");
    lTime = lGetTiskTime();
    while(1)
    {
        memset(&stField, 0, sizeof(TField));
        lRet = lTableFetch(pstSavm, SYS_TVM_FIELD, (void *)&stField);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "获取表(SYS_TVM_FIELD)数据失败, err:(%d)(%s)\n", pstSavm->m_lErrno, 
                sGetTError(pstSavm->m_lErrno), lGetEffect());
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        lRow ++;

        memset(szPrint, 0, sizeof(szPrint));
        if(FIELD_CHAR == stField.m_lAttr)
        {
            snprintf(szPrint, sizeof(szPrint), "[%3d]: %4d|%3d|%-20s|%-22s|%18s|STRING|"
                "%4d|%4d|\n", lRow, stField.m_table, stField.m_lSeq, stField.m_szOwner, 
                stField.m_szTable, stField.m_szField, stField.m_lLen, stField.m_lIsPk);
        }
        else if(FIELD_LONG == stField.m_lAttr)
        {
            snprintf(szPrint, sizeof(szPrint), "[%3d]: %4d|%3d|%-20s|%-22s|%18s|NUMBER|"
                "%4d|%4d|\n", lRow, stField.m_table, stField.m_lSeq, stField.m_szOwner, 
                stField.m_szTable, stField.m_szField, stField.m_lLen, stField.m_lIsPk);
        }
        else if(FIELD_DOUBLE == stField.m_lAttr)
        {
            snprintf(szPrint, sizeof(szPrint), "[%3d]: %4d|%3d|%-20s|%-22s|%18s|DOUBLE|"
                "%4d|%4d|\n", lRow, stField.m_table, stField.m_lSeq, stField.m_szOwner, 
                stField.m_szTable, stField.m_szField, stField.m_lLen, stField.m_lIsPk);
        }
        else
        {
            snprintf(szPrint, sizeof(szPrint), "[%3d]: %4d|%3d|%-20s|%-22s|%18s| NAN  |"
                "%4d|%4d|\n", lRow, stField.m_table, stField.m_lSeq, stField.m_szOwner, 
                stField.m_szTable, stField.m_szField, stField.m_lLen, stField.m_lIsPk);
        }

        fprintf(stdout, "%s", szPrint);
    }
    lTime -= lGetTiskTime();
    vTableClose(pstSavm);
    fprintf(stdout, "-------------------------------------------%s-----------------------------"
        "----------------\n", sGetCostTime(-1 * lTime));

    return ;
}

char*    sPermitConv(long lPers)
{
    static    char    szPers[5];

    strcpy(szPers, "----");
    szPers[4] = 0x00;

    if(lPers & OPERATE_SELECT)
        szPers[0] = 's';
    if(lPers & OPERATE_UPDATE)
        szPers[1] = 'u';
    if(lPers & OPERATE_DELETE)
        szPers[2] = 'd';
    if(lPers & OPERATE_INSERT)
        szPers[3] = 'i';

    return szPers;
}

/*************************************************************************************************
    函数说明：打印表域信息
    参数说明：
    返回值：
        无
  *************************************************************************************************/
void    vPrintDomain()
{
    TDomain stDomain;
    SATvm   *pstSavm = NULL;
    char    szPrint[512];
    long    lRet = 0, lRow = 0, lTime = 0;

    memset(&stDomain, 0, sizeof(TDomain));
    if(NULL == (pstSavm = (SATvm *)pInitSATvm(SYS_TVM_DOMAIN)))
    {
        fprintf(stderr, "初始化表(SYS_TVM_DOMAIN)失败\n");
        return ;
    }

    pstSavm->lFind = IDX_SELECT;
    pstSavm->lSize = sizeof(TDomain);
    lRet = lTableDeclare(pstSavm, SYS_TVM_DOMAIN);
    if(RC_SUCC != lRet)
    {
        fprintf(stderr, "定义表(SYS_TVM_DOMAIN)游标失败(%d), err:(%d)(%s)\n", pstSavm->m_lErrno, 
            pstSavm->lSize, sGetTError(pstSavm->m_lErrno));
        return ;
    }
    fprintf(stdout, "------------------------------------------------------------------------"
        "-----------------------------------------------\n");
    fprintf(stdout, "line table     table_name         part     size       ip         port "
        "keep tmout mtry ctry stat pers RLISP/LLISP/LRDSP\n");
    fprintf(stdout, "------------------------------------------------------------------------"
        "-----------------------------------------------\n");
    lTime = lGetTiskTime();
    while(1)
    {
        memset(&stDomain, 0, sizeof(TDomain));
        lRet = lTableFetch(pstSavm, SYS_TVM_DOMAIN, (void *)&stDomain);
        if(RC_FAIL == lRet)
        {
            fprintf(stderr, "获取表(SYS_TVM_DOMAIN)数据失败, %s\n", sGetTError(pstSavm->m_lErrno));
            vTableClose(pstSavm);
            return ;
        }
        else if(RC_NOTFOUND == lRet)
            break;

        lRow ++;

        memset(szPrint, 0, sizeof(szPrint));
        snprintf(szPrint, sizeof(szPrint), "[%3d]: %3d|%18s|%12s|%4d|%15s|%5d|%4d|%5d|%4d|%4d|", 
            lRow, stDomain.m_table, stDomain.m_szTable, stDomain.m_szPart, stDomain.m_lRowSize, 
            stDomain.m_szIp, stDomain.m_lRDPort, stDomain.m_lKeepLive, stDomain.m_lTimeOut, 
            stDomain.m_lTryMax, stDomain.m_lTryTimes);

        if(RESOURCE_INIT == stDomain.m_lStatus)
            strcat(szPrint, "INIT|");
        else if(RESOURCE_EXCP == stDomain.m_lStatus)
            strcat(szPrint, "EXCP|");
        else if(RESOURCE_ABLE == stDomain.m_lStatus)
            strcat(szPrint, "ABLE|");
        else if(RESOURCE_STOP == stDomain.m_lStatus)
            strcat(szPrint, "STOP|");
        else if(RESOURCE_AUTH == stDomain.m_lStatus)
            strcat(szPrint, "AUTH|");

        strcat(szPrint, sPermitConv(stDomain.m_lPers));
        fprintf(stdout, "%s|%5d/%5d/%5d|\n", szPrint, stDomain.m_lRIPort, stDomain.m_lLIPort, stDomain.m_lLDPort);
        fflush(stdout);
    }
    lTime -= lGetTiskTime();
    vTableClose(pstSavm);
    fprintf(stdout, "-------------------------------------------%s-----------------------------"
        "---------------------------------------\n", sGetCostTime(-1 * lTime));

    return ;
}

/*************************************************************************************************
    函数说明：获取字符串o/d之前的值, 
    参数说明：
		flag				--忽略大小写
    返回值：
        无
  *************************************************************************************************/
char*   sGetTruckValue(const char *s, char *o, char *d, bool flag, char *out, int olen)
{
    int     len = 0;
    char    *p = NULL, *q = NULL;

    if (!o)
    {
        if (!d)
        {
            len = strlen(s);
            return memcpy(out, s, len > olen ? olen : len);
        }

        if (!(q = (flag ? strcasestr(s, d) : strstr(s, d))))
            return NULL;

        return memcpy(out, s, q - s > olen ? olen : q - s);
    }
    else if(!d)
    {
        if (!o)
        {
            len = strlen(s);
            return memcpy(out, s, len > olen ? olen : len);
        }

        if (!(p = (flag ? strcasestr(s, o) : strstr(s, o))))
            return NULL;

        snprintf(out, olen, "%s", p + strlen(o));
        return out;
    }
    else
    {
        if (!(p = (flag ? strcasestr(s, o) : strstr(s, o))))
            return NULL;

        len = strlen(o);

        if (!(q = (flag ? strcasestr(p + len, d) : strstr(s, d))))
            return NULL;

        return memcpy(out, p + len, q - p - len > olen ? olen : q - p - len);
    }
}

/*************************************************************************************************
    函数说明：对字符串中的字符进行忽略大小写替换
    参数说明：
    返回值：
        s                                --处理后的字符串
  *************************************************************************************************/
char*    sUpperWord(char *s, char *w)
{
    long    len;
    char    *p = NULL, *q = s;

    if(!s || !w || !strlen(s))    return NULL;

    if(0 == (len = strlen(w)))    return NULL;
    
    while(NULL != (p = strcasestr(q, w)))
    {
        memcpy(p, w, len);
        q += len;
    }

    return s;
}

/*************************************************************************************************
    函数说明：获取Select所选字段
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lSelectField(SATvm *pstSavm, char *p, long lCount, TField *pstField, SQLFld **ppstRoot)
{
    TField  *pv = NULL;
    BOOL    bFlag = false;
    long    lRec = 0, i, j;
    char    szField[MAX_FIELD_LEN];
    SQLFld  stQField, *pstRoot = NULL;

    if(pstSavm->lReMatch & GROUP_BY)
    {
        for(j = 0; j < lCount && NULL == pv; j ++)
        {
            if(pstField[j].m_lFrom == REFrom(pstSavm->lReField) && 
                pstField[j].m_lLen == REFLen(pstSavm->lReField))
                pv = &pstField[j];
        }
    }

    memset(szField, 0, sizeof(szField));
    if(0 == strcmp(p, "*"))
    {
        if(pstSavm->lReMatch & GROUP_BY)
            return RC_FAIL;

        for(j = 0; j < lCount; j ++)
        {
            memset(&stQField, 0, sizeof(SQLFld));
            stQField.m_stKey.m_lLen  = pstField[j].m_lLen;
            stQField.m_stKey.m_lAttr = pstField[j].m_lAttr;
            stQField.m_stKey.m_lFrom = pstField[j].m_lFrom;
            strcpy(stQField.m_stKey.m_szField, pstField[j].m_szField);

            if(NULL == (pstRoot = pInsertFiled(pstRoot, &stQField)))
                return RC_FAIL;
        }

        *ppstRoot = pSortSQLFiled(pstRoot);

        return RC_SUCC;
    }

    for(i = 0, lRec = lfieldnum(p, ","); i < lRec; i ++)
    {
        memset(szField, 0, sizeof(szField));
        strncpy(szField, sfieldvalue(p, ",", i + 1), MAX_FIELD_LEN);
        strimall(szField);
        if((pstSavm->lReMatch & GROUP_BY) && (!strcasecmp(szField, "count(*)") || 
            !strcasecmp(szField, "count(1)")))
        {
            bFlag = true;
            memset(&stQField, 0, sizeof(SQLFld));
            strcpy(stQField.m_stKey.m_szField, szField);
            if(NULL == (pstRoot = pInsertFiled(pstRoot, &stQField)))
                return RC_FAIL;
            continue;
        }

        for(j = 0, bFlag = false; j < lCount; j ++)
        {

            if(strcasecmp(szField, pstField[j].m_szField))
                continue;

            if((pstSavm->lReMatch & GROUP_BY) && pv != &pstField[j])
                return RC_FAIL;

            bFlag = true;
            memset(&stQField, 0, sizeof(SQLFld));
            stQField.m_stKey.m_lLen  = pstField[j].m_lLen;
            stQField.m_stKey.m_lAttr = pstField[j].m_lAttr;
            stQField.m_stKey.m_lFrom = pstField[j].m_lFrom;
            strcpy(stQField.m_stKey.m_szField, pstField[j].m_szField);

            if(NULL == (pstRoot = pInsertFiled(pstRoot, &stQField)))
                return RC_FAIL;
        }

        if(!bFlag)
        {
            fprintf(stderr, "字段(%s)未定义\n", szField);
            return RC_FAIL;
        }
    }

    *ppstRoot = pstRoot;

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取Update所选字段
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lUpdateField(char *pszField, long lCount, TField *pstField, void *pvUpdate)
{
    void    *v = NULL;
    BOOL    bFlag = false;
    long    lRec = 0, i, j;
    char    szValue[512], szContent[1024], szField[512], szNumber[16];

    memset(szNumber, 0, sizeof(szNumber));
    lRec = lfieldnum(pszField, ",");
    for(i = 0, v = (void *)szNumber; i < lRec; i ++)
    {
        memset(szValue, 0, sizeof(szValue));
        memset(szField, 0, sizeof(szField));
        memset(szContent, 0, sizeof(szContent));
        strncpy(szContent, sfieldvalue(pszField, ",", i + 1), sizeof(szContent));
        strncpy(szField, sfieldvalue(szContent, "=", 1), sizeof(szField));
        strimall(szField);

        strncpy(szValue, sfieldvalue(szContent, "=", 2), sizeof(szValue));
        sltrim(szValue);
        srtrim(szValue);
        strimabout(szValue, "\'", "\'");

        for(bFlag = false, j = 0; j < lCount; j ++)
        {
            if(strcmp(pstField[j].m_szField, szField))
                continue;

            bFlag = true;
            switch(pstField[j].m_lAttr)
            {
            case FIELD_DOUBLE:
                strimall(szValue);
                switch(pstField[j].m_lLen)
                {
                case    4:
                    *((float *)v) = atof(szValue);
                    if(*((float *)v) <= 0.00)    *((float *)v) += NUMBER_ZEOR;
                    memcpy(pvUpdate + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                case    8:
                    *((double *)v) = atof(szValue);
                    if(*((double *)v) <= 0.00)    *((double *)v) += NUMBER_ZEOR;
                    memcpy(pvUpdate + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                default:
                    break;
                }
                break;
            case FIELD_LONG:
                strimall(szValue);
                switch(pstField[j].m_lLen)
                {
                case    2:
                    *((sint *)v) = atoi(szValue);
                    if(*((sint *)v) <= 0)    *((sint *)v) += (sint)NUMBER_ZEOR;
                    memcpy(pvUpdate + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                case    4:
                    *((int *)v) = atoi(szValue);
                    if(*((int *)v) <= 0)    *((int *)v) += (sint)NUMBER_ZEOR;
                    memcpy(pvUpdate + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                case    8:
                    *((llong *)v) = atol(szValue);
                    if(*((llong *)v) <= 0)    *((llong *)v) += (sint)NUMBER_ZEOR;
                    memcpy(pvUpdate + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                default:
                    break;
                }
                break;
            case FIELD_CHAR:
                memcpy(pvUpdate + pstField[j].m_lFrom, szValue, 
                    MIN(strlen(szValue), pstField[j].m_lLen));
                break;
            default:
                break;
            }
        }

        if(!bFlag)
        {
            fprintf(stderr, "字段(%s)未定义\n", szField);
            return RC_FAIL;
        }
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：执行SQL-select
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lExeUpdate(SATvm *pstSavm, TIndex *pstIndex, void *pvNew, char *pvData)
{

    if(RC_SUCC != lInitSATvm(pstSavm, pstIndex->m_table))
        return RC_FAIL;

    if(bIsRealZore(pvData, pstIndex->m_lRowSize))
        pstSavm->pstVoid = NULL;
    else
        pstSavm->pstVoid = (void *)pvData;
    pstSavm->lReMatch= MATCH_YES;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = pstIndex->m_lRowSize;
    if(RC_SUCC != lUpdate(pstSavm, (void *)pvNew, false))
    {
        fprintf(stderr, "更新表(%s)数据失败, %s\n", pstIndex->m_szTable, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    fprintf(stdout, "---(%d)条记录被更新---\n", pstSavm->m_lEffect);
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：执行SQL-delete
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lExeDelete(SATvm *pstSavm, TIndex *pstIndex, char *pvData)
{
    if(!pvData)    return RC_FAIL;

    if(RC_SUCC != lInitSATvm(pstSavm, pstIndex->m_table))
        return RC_FAIL;

    if(bIsRealZore(pvData, pstIndex->m_lRowSize))
        pstSavm->pstVoid = NULL;
    else
        pstSavm->pstVoid = (void *)pvData;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = pstIndex->m_lRowSize;
    if(RC_SUCC != lDelete(pstSavm))
    {
        fprintf(stderr, "删除表(%s)数据失败, %s\n", pstIndex->m_szTable, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    fprintf(stdout, "---(%d)条记录被删除---\n", pstSavm->m_lEffect);
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：执行SQL-insert
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lExeInsert(SATvm *pstSavm, TIndex *pstIndex, void *pvInsert)
{
    if(RC_SUCC != lInitSATvm(pstSavm, pstIndex->m_table))
        return RC_FAIL;

    if(bIsRealZore(pvInsert, pstIndex->m_lRowSize))
    {
        pstSavm->m_lErrno = CONDIT_IS_NIL;
        return RC_FAIL;
    }

    pstSavm->pstVoid = (void *)pvInsert;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = pstIndex->m_lRowSize;
    if(RC_SUCC != lInsert(pstSavm))
    {
        fprintf(stderr, "新增表(%s)数据失败, %s\n", pstIndex->m_szTable, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    fprintf(stdout, "---(%d)条记录被新增---\n", pstSavm->m_lEffect);
    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：获取SQL条件
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lConditField(SATvm *pstSavm, char *pszWhere, long lCount, TField *pstField, void *pvWhere, 
            char *pszAdorn)
{
    void    *v = NULL;
    BOOL    bFlag = false;
    long    lRec = 0, i, j;
    char    szValue[512], szCondit[1024], szField[512], szNumber[16];

    if(strcasestr(pszWhere, " or "))
    {
        fprintf(stderr, "暂不支持的查询方式\n");
        return RC_FAIL;
    }

    for(bFlag = false, j = 0; j < lCount && strlen(pszAdorn); j ++)
    {
        if(strcmp(pstField[j].m_szField, pszAdorn))
            continue;

        bFlag = true;
        pstSavm->lReField = (pstField[j].m_lFrom << 16 | pstField[j].m_lLen);
        break;
    }
    if(!bFlag && strlen(pszAdorn))
    {
        fprintf(stderr, "字段(%s)未定义\n", pszAdorn);
        return RC_FAIL;
    }

    memset(szNumber, 0, sizeof(szNumber));
    lRec = lfieldnum(pszWhere, " AND ");
    for(i = 0, v = (void *)szNumber; i < lRec; i ++)
    {
        memset(szValue, 0, sizeof(szValue));
        memset(szField, 0, sizeof(szField));
        memset(szCondit, 0, sizeof(szCondit));
        strncpy(szCondit, sfieldvalue(pszWhere, " AND ", i + 1), sizeof(szCondit));

        strncpy(szField, sfieldvalue(szCondit, "=", 1), sizeof(szField));
        strimall(szField);
       	strncpy(szValue, sfieldvalue(szCondit, "=", 2), sizeof(szValue));
		if(strstr(szCondit, "<"))
		{
        	strncpy(szField, sfieldvalue(szCondit, "<", 1), sizeof(szField));
        	strimall(szField);
			if(strcasecmp(szField, "rownum"))
				continue;

        	strncpy(szValue, sfieldvalue(szCondit, "<", 2), sizeof(szValue));
			strimall(szValue);
			if('=' == szValue[0])
			{	
        		sltrim(szValue + 1);
        		srtrim(szValue + 1);
				pstSavm->m_lTimes = atol(szValue + 1);	
			}
			else
			{
        		sltrim(szValue);
        		srtrim(szValue);
				pstSavm->m_lTimes = atol(szValue) - 1;	
			}

			continue;
		}

		if(!strcasecmp(szField, "rownum"))
		{
			pstSavm->m_lTimes = atol(szValue);	
			continue;
		}

       	sltrim(szValue);
       	srtrim(szValue);
        strimabout(szValue, "\'", "\'");
        if(!strcmp(szField, "1") && !strcmp(szValue, "1"))    continue;

        for(bFlag = false, j = 0; j < lCount; j ++)
        {
            if(strcmp(pstField[j].m_szField, szField))
                continue;

            bFlag = true;
            switch(pstField[j].m_lAttr)
            {
            case FIELD_DOUBLE:
                strimall(szValue);
                switch(pstField[j].m_lLen)
                {
                case    4:
                    *((float *)v) = atof(szValue);
                    if(*((float *)v) <= 0.00)    *((float *)v) += NUMBER_ZEOR;
                    memcpy(pvWhere + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                case    8:
                    *((double *)v) = atof(szValue);
                    if(*((double *)v) <= 0.00)    *((double *)v) += NUMBER_ZEOR;
                    memcpy(pvWhere + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                default:
                    break;
                }
                break;
            case FIELD_LONG:
                strimall(szValue);
                switch(pstField[j].m_lLen)
                {
                case    2:
                    *((sint *)v) = atoi(szValue);
                    if(*((sint *)v) <= 0)    *((sint *)v) += (sint)NUMBER_ZEOR;
                    memcpy(pvWhere + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                case    4:
                    *((int *)v) = atoi(szValue);
                    if(*((int *)v) <= 0)    *((int *)v) += (int)NUMBER_ZEOR;
                    memcpy(pvWhere + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                case    8:
                    *((llong *)v) = atol(szValue);
                    if(*((llong *)v) <= 0)    *((llong *)v) += (llong)NUMBER_ZEOR;
                    memcpy(pvWhere + pstField[j].m_lFrom, v, pstField[j].m_lLen);
                    break;
                default:
                    break;
                }
                break;
            case FIELD_CHAR:
                memcpy(pvWhere + pstField[j].m_lFrom, szValue, 
                    MIN(strlen(szValue), pstField[j].m_lLen));
                break;
            default:
                break;
            }
        }

        if(!bFlag)
        {
            fprintf(stderr, "字段(%s)未定义\n", pszWhere);
            return RC_FAIL;
        }
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：执行SQL-count
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lCountSelect(SATvm *pstSavm, TIndex *pstIndex, char *pvData)
{
    size_t    lSum = 0;

    if(RC_SUCC != lInitSATvm(pstSavm, pstIndex->m_table))
        return RC_FAIL;

    if(bIsRealZore(pvData, pstIndex->m_lRowSize))
        pstSavm->pstVoid = NULL;
    else
        pstSavm->pstVoid = (void *)pvData;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = pstIndex->m_lRowSize;
    if(RC_SUCC != lCount(pstSavm, &lSum))
    {
        fprintf(stderr, "统计表(%s)记录，%s\n", pstIndex->m_szTable, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    fprintf(stdout, "COUNT(*)\n");
    fprintf(stdout, "%zu\n", lSum);
    fflush(stdout);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：执行SQL-select
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lExeSelect(SATvm *pstSavm, TIndex *pstIndex, SQLFld *pstRoot, char *pvData, char *pszFile,
            char *pszDem)
{
    FILE    *fp = NULL;
    TblKey  *pstKey = NULL;
    SQLFld  *pstNode = NULL;
    Rowgrp  *pstList = NULL;
    size_t  i, lOffset, lRows;
    char    szDelmit[64], *pvRes = NULL;

    memset(szDelmit, 0, sizeof(szDelmit));
    lRows   = pstSavm->lReField;
    lOffset = pstSavm->lReMatch;
    if(RC_SUCC != lInitSATvm(pstSavm, pstIndex->m_table))
        return RC_FAIL;

    if(bIsRealZore(pvData, pstIndex->m_lRowSize))
        pstSavm->pstVoid = NULL;
    else
        pstSavm->pstVoid = (void *)pvData;
    pstSavm->lReField = lRows;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->lReMatch = MATCH_YES | lOffset;
    pstSavm->lSize    = pstIndex->m_lRowSize;
    if(RC_SUCC != lQuery(pstSavm, &lRows, (void **)&pvRes))
    {
		if(NO_DATA_FOUND != pstSavm->m_lErrno)
		{
        	fprintf(stderr, "获取表(%s)数据失败，%s\n", pstIndex->m_szTable, 
            	sGetTError(pstSavm->m_lErrno));
		}

        return RC_FAIL;
    }

    if(pszFile)
    {
        if(NULL == (fp = fopen(pszFile, "wb")))
        {
            pstSavm->m_lErrno = FILE_NOTFOUND;
            return RC_FAIL;    
        }    

        strncpy(szDelmit, pszDem, sizeof(szDelmit));
    }
    else
    {
        fp = stdout;
        strcpy(szDelmit, " ");
    }

    for(pstNode = pstRoot; pstNode; pstNode = pstNode->pstNext)
        fprintf(fp, "%s ", pstNode->m_stKey.m_szField);
    fprintf(fp, "\n");

	if(pstSavm->m_lTimes <= 0)
		pstSavm->m_lTimes = pstSavm->m_lEffect;	

    if(lOffset & GROUP_BY)
    {
        for(i = 0, pstList = (Rowgrp *)pvRes; pstList && i < pstSavm->m_lTimes; 
			pstList = pstList->pstNext, ++ i)
        {
            for(pstNode = pstRoot; pstNode; pstNode = pstNode->pstNext)
            {
                pstKey = &pstNode->m_stKey;
                if(!strcasecmp(pstKey->m_szField, "count(*)") || 
                    !strcasecmp(pstKey->m_szField, "count(1)"))
                {
                    fprintf(fp, "%d%s", pstList->lCount, szDelmit);
                    continue;
                }

                switch(pstKey->m_lAttr)
                {
                case FIELD_DOUBLE:
                    switch(pstKey->m_lLen)
                    {
                    case    4:
                        fprintf(fp, "%f%s", *((float *)pstList->psvData), szDelmit);
                        break;
                    case    8:
                        fprintf(fp, "%f%s", *((double *)pstList->psvData), szDelmit);
                        break;
                    default:
                        break;
                    }
                    break;
                case FIELD_LONG:
                    switch(pstKey->m_lLen)
                    {
                    case    2:
                        fprintf(fp, "%d%s", *((sint *)pstList->psvData), szDelmit);
                        break;
                    case    4:
                        fprintf(fp, "%d%s", *((int *)pstList->psvData), szDelmit);
                        break;
                    case    8:
                        fprintf(fp, "%ld%s", *((llong *)pstList->psvData), szDelmit);
                        break;
                    default:
                        break;
                    }
                    break;
                case FIELD_CHAR:
                    fprintf(fp, "%.*s%s", pstKey->m_lLen, (char *)pstList->psvData, szDelmit);
                    break;
                default:
                    break;
                }
            }

            fprintf(fp, "\n");
            fflush(fp);
        }

        pstList = (Rowgrp *)pvRes;
        IBPFgrp(pstList);
    }
    else
    {
        for(i = 0; i < lRows && i < pstSavm->m_lTimes; i ++)
        {
            lOffset = pstIndex->m_lRowSize * i;
            for(pstNode = pstRoot; pstNode; pstNode = pstNode->pstNext)
            {
                if(!strcasecmp(pstNode->m_stKey.m_szField, "count(*)") || 
                    !strcasecmp(pstNode->m_stKey.m_szField, "count(1)"))
                    continue;

                pstKey = &pstNode->m_stKey;
                switch(pstKey->m_lAttr)
                {
                case FIELD_DOUBLE:
                    switch(pstKey->m_lLen)
                    {
                    case    4:
                        fprintf(fp, "%f%s", *((float *)(pvRes + lOffset + pstKey->m_lFrom)), szDelmit);
                        break;
                    case    8:
                        fprintf(fp, "%f%s", *((double *)(pvRes + lOffset + pstKey->m_lFrom)), szDelmit);
                        break;
                    default:
                        break;
                    }
                    break;
                case FIELD_LONG:
                    switch(pstKey->m_lLen)
                    {
                    case    2:
                        fprintf(fp, "%d%s", *((sint *)(pvRes + lOffset + pstKey->m_lFrom)), szDelmit);
                        break;
                    case    4:
                        fprintf(fp, "%d%s", *((int *)(pvRes + lOffset + pstKey->m_lFrom)), szDelmit);
                        break;
                    case    8:
                        fprintf(fp, "%ld%s", *((llong *)(pvRes + lOffset + pstKey->m_lFrom)), szDelmit);
                        break;
                    default:
                        break;
                    }
                    break;
                case FIELD_CHAR:
                    fprintf(fp, "%.*s%s", pstKey->m_lLen, pvRes + lOffset + pstKey->m_lFrom, szDelmit);
                    break;
                default:
                    break;
                }
            }
            fprintf(fp, "\n");
            fflush(fp);
        }
        IBPFree(pvRes);
    }

	pstSavm->m_lEffect = i;
    if(pszFile)
    {
        fprintf(stdout, "---(%d)条记录被导出---\n", pstSavm->m_lEffect);
        fclose(fp);
    }
    else
        fprintf(stdout, "---(%d)条记录被选中, ep(%d)---\n", pstSavm->m_lEffect, pstSavm->m_lEType);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：解析SEQUE语句并执行
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    lParseSequece(SATvm *pstSavm, char *pszSQName, char *pszFiled)
{
    ulong   ulSeque;

    if(strcasecmp(pszFiled, "nextval"))
    {
        pstSavm->m_lErrno = SQL_ERR_FIELD;
        return RC_FAIL;
    }

    if(RC_SUCC != lSelectSeque(pstSavm, pszSQName, &ulSeque))
        return RC_FAIL;

    fprintf(stdout, "%s\n", pszFiled);
    fprintf(stdout, "%lu\n", ulSeque);

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：解析order group 字段
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    lParseAdorn(SATvm *pstSavm, char *pszAdorn, long lOffset)
{
    char    szWord[64], szOrder[64], szLead[64], szField[64];

    sltrim(pszAdorn + lOffset);    
    if(0 == strlen(pszAdorn + lOffset))
    {    
        pszAdorn[0] = 0x00;
        return RC_SUCC;
    }

    memset(szLead, 0, sizeof(szLead));
    memset(szWord, 0, sizeof(szWord));
    memset(szOrder, 0, sizeof(szOrder));
    memset(szField, 0, sizeof(szField));
    strncpy(szWord, sfieldvalue(pszAdorn + lOffset, " ", 1), sizeof(szWord));
    strncpy(szLead, sfieldvalue(pszAdorn + lOffset, " ", 2), sizeof(szLead));
    if(!strcasecmp(szWord, "group") && !strcasecmp(szLead, "by"))
    {
        pstSavm->lReMatch |= GROUP_BY;
        strncpy(szField, sfieldvalue(pszAdorn + lOffset, " ", 3), sizeof(szField));
        if(!strlen(szField))    return RC_FAIL;
        strncpy(szOrder, sfieldvalue(pszAdorn + lOffset, " ", 4), sizeof(szOrder));
        if(!strlen(szOrder))
        {
            strcpy(pszAdorn, szField);
            return RC_SUCC;    
        }
        memset(szLead, 0, sizeof(szLead));
        strncpy(szLead, sfieldvalue(pszAdorn + lOffset, " ", 5), sizeof(szLead));
        if(strcasecmp(szOrder, "order") ||  strcasecmp(szLead, "by"))
            return RC_FAIL;

        memset(szWord, 0, sizeof(szWord));
        strncpy(szWord, sfieldvalue(pszAdorn + lOffset, " ", 6), sizeof(szWord));
        if(strcasecmp(szWord, szField))
        {
            strcpy(pszAdorn, szField);
            return RC_SUCC;
        }

        memset(szWord, 0, sizeof(szWord));
        strncpy(szWord, sfieldvalue(pszAdorn + lOffset, " ", 7), sizeof(szWord));
        strimall(szWord);
        if(!strcasecmp(szWord, "desc"))
            pstSavm->lReMatch |= ORDER_DESC;
        else
            pstSavm->lReMatch |= ORDER_ASC;

        strcpy(pszAdorn, szField);
        return RC_SUCC;    
    }
    else if(!strcasecmp(szWord, "order") && !strcasecmp(szLead, "by"))
    {
        strncpy(szField, sfieldvalue(pszAdorn + lOffset, " ", 3), sizeof(szField));
        if(!strlen(szField))    return RC_FAIL;
        memset(szOrder, 0, sizeof(szOrder));
        strncpy(szOrder, sfieldvalue(pszAdorn + lOffset, " ", 4), sizeof(szOrder));
        strimall(szOrder);
        if(!strcasecmp(szOrder, "desc"))
            pstSavm->lReMatch |= ORDER_DESC;
        else
            pstSavm->lReMatch |= ORDER_ASC;

        strncpy(szOrder, sfieldvalue(pszAdorn + lOffset, " ", 5), sizeof(szOrder));
        if(strlen(szOrder))   return RC_FAIL;    
        strcpy(pszAdorn, szField);
        return RC_SUCC;    
    }


//            pstSavm->lReMatch |= FIRST_ROW;


    return RC_FAIL;
}

/*************************************************************************************************
	函数说明：获取Select所选字段
	参数说明：
	返回值：
	    RC_SUCC                            --成功
	    RC_FAIL                            --失败
 *************************************************************************************************/
long    _lExeExtreme(SATvm *pstSavm, TIndex *pstIndex, long lOut, TField *pstField, void *pvData, 
			char *pszField)
{
	char	szField[64];
	TblKey  *pstKey = NULL;
    SQLFld  *pstRoot = NULL;

	memset(szField, 0, sizeof(szField));
	if(1 < lfieldnum(pszField, ","))
	{
        fprintf(stderr, "*暂不支持多字段的极值查询\n");
		pstSavm->m_lErrno = SQL_ERR_FIELD;
		return RC_FAIL;
	}

    if(!sGetTruckValue(pszField, "(", ")", true, szField, sizeof(szField)))
    {
		pstSavm->m_lErrno = SQL_SYNTX_ERR;
        return RC_FAIL;
    }

	if(!strlen(szField))
	{
		pstSavm->m_lErrno = SQL_SYNTX_ERR;
		return RC_FAIL;
	}

    if(RC_SUCC != _lSelectField(pstSavm, szField, lOut, pstField, &pstRoot))
    {
    	pstSavm->m_lErrno = SQL_ERR_FIELD;
		return RC_FAIL;
    }

	pstKey = &pstRoot->m_stKey;
    if(RC_SUCC != lInitSATvm(pstSavm, pstIndex->m_table))
        return RC_FAIL;

	if(!strncasecmp(pszField, "max(", 4))
    	pstSavm->lReMatch = MATCH_MAX;
	else if(!strncasecmp(pszField, "min(", 4))
    	pstSavm->lReMatch = MATCH_MIN;

    if(bIsRealZore(pvData, pstIndex->m_lRowSize))
        pstSavm->pstVoid = NULL;
    else
        pstSavm->pstVoid = (void *)pvData;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->lSize    = pstIndex->m_lRowSize;
	pstSavm->lReField = (pstKey->m_lFrom << 16 | pstKey->m_lLen);	
    if(RC_SUCC != lExtreme(pstSavm, pvData))
	{
    	vDestroyFiled(pstRoot);
    	fprintf(stderr, "统计表(%s)数据失败, %s\n", pstIndex->m_szTable, 
			sGetTError(pstSavm->m_lErrno));
		return RC_FAIL;
	}
  
    fprintf(stdout, "%s\n", pszField);
    switch(pstKey->m_lAttr)
    {
    case FIELD_DOUBLE:
        switch(pstKey->m_lLen)
        {
        case    4:
            fprintf(stdout, "%f\n", *((float *)(pvData + pstKey->m_lFrom)));
            break;
        case    8:
            fprintf(stdout, "%f\n", *((double *)(pvData + pstKey->m_lFrom)));
            break;
        default:
            break;
        }
        break;
    case FIELD_LONG:
        switch(pstKey->m_lLen)
        {
        case    2:
            fprintf(stdout, "%d\n", *((sint *)(pvData + pstKey->m_lFrom)));
            break;
        case    4:
            fprintf(stdout, "%d%s", *((int *)(pvData + pstKey->m_lFrom)));
            break;
        case    8:
            fprintf(stdout, "%ld\n", *((llong *)(pvData + pstKey->m_lFrom)));
            break;
        default:
            break;
        }
        break;
    case FIELD_CHAR:
        fprintf(stdout, "%.*s\n", pstKey->m_lLen, pvData + pstKey->m_lFrom);
        break;
    default:
        break;
    }
    fflush(stdout);

    vDestroyFiled(pstRoot);
	return RC_SUCC;
}

/*************************************************************************************************
    函数说明：解析SQL 语法字段
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lParseSelect(SATvm *pstSavm, char *pszTable, char *pszField, char *pszWhere, char *pszFile, 
            char *pszDem, char *pszAdorn)
{
    TIndex  stIndex;
    SQLFld  *pstRoot = NULL;
    void    *pvWhere =  NULL;
    TField  *pstField = NULL;
    size_t  lOut = 0, lRematch;

    memset(&stIndex, 0, sizeof(TIndex));
    lRematch = pstSavm->lReMatch;
    pstSavm->lReMatch = 0;
    strncpy(stIndex.m_szPart, sgetvalue(pszTable, "@", 2), sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sgetvalue(pszTable, "@", 1), sizeof(stIndex.m_szTable));
    supper(stIndex.m_szTable);    
    sGetNode(stIndex.m_szPart);

    if(!strcmp(stIndex.m_szTable, "SEQUENCE"))
        return lParseSequece(pstSavm, stIndex.m_szPart, pszField);

    if(RC_SUCC != lGetTblIndex(pstSavm, stIndex.m_szTable, stIndex.m_szPart, &stIndex))
        return RC_FAIL;

    if(RC_SUCC != lGetTblField(stIndex.m_table, &lOut, &pstField))
        return RC_FAIL;

    if(NULL == (pvWhere = (char *)calloc(stIndex.m_lRowSize, sizeof(char))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        goto ERR_SELECT;
    }

    pstSavm->lReMatch = lRematch;
    if(RC_SUCC != _lConditField(pstSavm, pszWhere, lOut, pstField, pvWhere, pszAdorn))
    {
        pstSavm->m_lErrno = SQL_ERR_WHERE;
        goto ERR_SELECT;
    }

    if((!strcasecmp(pszField, "count(*)") || !strcasecmp(pszField, "count(1)")) && 
		!(pstSavm->lReMatch & GROUP_BY))
    {
        IBPFree(pstField);
        if(RC_SUCC != _lCountSelect(pstSavm, &stIndex, pvWhere))
            goto ERR_SELECT;
        
        IBPFree(pvWhere);
        return RC_SUCC;
    }
	else if(!strncasecmp(pszField, "max(", 4) || !strncasecmp(pszField, "min(", 4))
	{
        if(RC_SUCC != _lExeExtreme(pstSavm, &stIndex, lOut, pstField, pvWhere, pszField))
            goto ERR_SELECT;
        
        IBPFree(pstField);
        IBPFree(pvWhere);
		return RC_SUCC;
	}

    if(RC_SUCC != _lSelectField(pstSavm, pszField, lOut, pstField, &pstRoot))
    {
        pstSavm->m_lErrno = SQL_ERR_FIELD;
        goto ERR_SELECT;
    }

    IBPFree(pstField);
    if(RC_SUCC != _lExeSelect(pstSavm, &stIndex, pstRoot, pvWhere, pszFile, pszDem))
        goto ERR_SELECT;

    IBPFree(pvWhere);
    vDestroyFiled(pstRoot);
    return RC_SUCC;

ERR_SELECT:
    IBPFree(pvWhere);
    IBPFree(pstField);
    vDestroyFiled(pstRoot);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：解析SQL-select语法
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    _lSelectSyntax(SATvm *pstSavm, char *pszSQL, char *pszFile, char *pszDem)
{
    char    szTable[MAX_FIELD_LEN], *p = NULL;
    char    szWhere[1024], szField[1024], szAdorn[1024];

    memset(szWhere, 0, sizeof(szWhere));
    memset(szField, 0, sizeof(szField));
    memset(szTable, 0, sizeof(szTable));
    memset(szAdorn, 0, sizeof(szAdorn));

    if(!sGetTruckValue(pszSQL, "select ", "from ", true, szField, sizeof(szField)))
    {
        pstSavm->m_lErrno = SQL_FIELD_NIL;
        return RC_FAIL;
    }

    strimall(szField);
    if(!strlen(szField))
    {
        pstSavm->m_lErrno = SQL_FIELD_NIL;
        return RC_FAIL;
    }

    if(NULL != strcasestr(pszSQL, "where "))
    {
        if(!sGetTruckValue(pszSQL, "from ", "where", true, szTable, sizeof(szTable)))
        {
            pstSavm->m_lErrno = SQL_TABLE_NIL;
            return RC_FAIL;
        }

        if(!sGetTruckValue(pszSQL, "where ", NULL, true, szAdorn, sizeof(szAdorn)))
        {
            pstSavm->m_lErrno = SQL_TABLE_NIL;
            return RC_FAIL;
        }

        if((p = strcasestr(szAdorn, "group")) || (p = strcasestr(szAdorn, "order")))
            strncpy(szWhere, szAdorn, p - szAdorn);
        else
            strncpy(szWhere, szAdorn, sizeof(szWhere));

        if(RC_SUCC != lParseAdorn(pstSavm, szAdorn, strlen(szWhere)))
        {
            pstSavm->m_lErrno = SQL_SYNTX_ERR;
            return RC_FAIL;
        }

        sltrim(szWhere);   //  条件
        srtrim(szWhere);
        sUpperWord(szWhere, " AND ");
        DEL_TAIL_CHAR(szWhere, ';');
    }
    else
    {
        if(!sGetTruckValue(pszSQL, "from ", NULL, true, szAdorn, sizeof(szAdorn)))
        {
            pstSavm->m_lErrno = SQL_TABLE_NIL;
            return RC_FAIL;
        }
			
        sltrim(szAdorn);
        strncpy(szTable, sfieldvalue(szAdorn, " ", 1), MAX_FIELD_LEN);
        if(RC_SUCC != lParseAdorn(pstSavm, szAdorn, strlen(szTable)))
        {
            pstSavm->m_lErrno = SQL_SYNTX_ERR;
            return RC_FAIL;
        }

        DEL_TAIL_CHAR(szTable, ';');
    }

    strimall(szTable);
    vSCRDebug("DEBUG:select field:[%s]", szField);
    vSCRDebug("DEBUG:select table:[%s]", szTable);
    vSCRDebug("DEBUG:select where:[%s]", szWhere);
    vSCRDebug("DEBUG:select adorn:[%d][%s]", pstSavm->lReMatch, szAdorn);

    return _lParseSelect(pstSavm, szTable, szField, szWhere, pszFile, pszDem, szAdorn);
}

/*************************************************************************************************
    函数说明：解析SQL 语法字段
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lParseUpdate(SATvm *pstSavm, char *pszTable, char *pszField, char *pszWhere)
{
    TIndex  stIndex;
    size_t  lOut = 0;
    TField  *pstField = NULL;
    void    *pvWhere =  NULL, *pvUpdate = NULL;

    memset(&stIndex, 0, sizeof(TIndex));
    strncpy(stIndex.m_szPart, sgetvalue(pszTable, "@", 2), sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sgetvalue(pszTable, "@", 1), sizeof(stIndex.m_szTable));
    supper(stIndex.m_szTable);
    sGetNode(stIndex.m_szPart);

    if(RC_SUCC != lGetTblIndex(pstSavm, stIndex.m_szTable, stIndex.m_szPart, &stIndex))
        return RC_FAIL;

    if(RC_SUCC != lGetTblField(stIndex.m_table, &lOut, &pstField))
        return RC_FAIL;

    if(NULL == (pvUpdate = (char *)calloc(stIndex.m_lRowSize, sizeof(char))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        goto ERR_UPDATE;
    }

    if(RC_SUCC != _lUpdateField(pszField, lOut, pstField, pvUpdate))
    {
        pstSavm->m_lErrno = SQL_ERR_FIELD;
        goto ERR_UPDATE;
    }

    if(NULL == (pvWhere = (char *)calloc(stIndex.m_lRowSize, sizeof(char))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        goto ERR_UPDATE;
    }

    if(RC_SUCC != _lConditField(pstSavm, pszWhere, lOut, pstField, pvWhere, ""))
    {
        pstSavm->m_lErrno = SQL_ERR_WHERE;
        goto ERR_UPDATE;
    }

//vPrintHex(pvWhere, stIndex.m_lRowSize);    

    IBPFree(pstField);
    if(RC_SUCC != _lExeUpdate(pstSavm, &stIndex, pvUpdate, pvWhere))
        goto ERR_UPDATE;

    IBPFree(pvWhere);
    IBPFree(pvUpdate);
    return RC_SUCC;

ERR_UPDATE:
    IBPFree(pvWhere);
    IBPFree(pvUpdate);
    IBPFree(pstField);
    return RC_FAIL;
}


/**************************************************************************************************
    函 数 名：解析SQL-update语法
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    _lUpdateSyntax(SATvm *pstSavm, char *pszSQL)
{
    char    szTable[MAX_FIELD_LEN];
    char    szWhere[1024], szField[1024];

    memset(szWhere, 0, sizeof(szWhere));
    memset(szField, 0, sizeof(szField));
    memset(szTable, 0, sizeof(szTable));

    if(!sGetTruckValue(pszSQL, "update ", "set ", true, szTable, sizeof(szTable)))
    {
        pstSavm->m_lErrno = SQL_TABLE_NIL;
        return RC_FAIL;
    }
    strimall(szTable);

    if(!strcasestr(pszSQL, "where"))
    {
        pstSavm->m_lErrno = SQL_WHERE_NIL;
        return RC_FAIL;
    }

    if(!sGetTruckValue(pszSQL, "set ", "where", true, szField, sizeof(szField)))
    {
        pstSavm->m_lErrno = SQL_FIELD_NIL;
        return RC_FAIL;
    }

    sltrim(szField); 
    srtrim(szField);
    if(!strlen(szField))
    {
        pstSavm->m_lErrno = SQL_FIELD_NIL;
        return RC_FAIL;
    }

    if(!sGetTruckValue(pszSQL, "where ", NULL, true, szWhere, sizeof(szWhere)))
    {
        pstSavm->m_lErrno = SQL_WHERE_NIL;
        return RC_FAIL;
    }

    sltrim(szWhere);   //  条件
    srtrim(szWhere);
    sUpperWord(szWhere, " AND ");
    DEL_TAIL_CHAR(szWhere, ';');

    vSCRDebug("DEBUG:update field:[%s]", szField);
    vSCRDebug("DEBUG:update table:[%s]", szTable);
    vSCRDebug("DEBUG:update where:[%s]", szWhere);

    return _lParseUpdate(pstSavm, szTable, szField, szWhere);
}

/*************************************************************************************************
    函数说明：解析SQL 语法字段
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lParseDelete(SATvm *pstSavm, char *pszTable,  char *pszWhere)
{
    TIndex  stIndex;
    size_t  lOut = 0;
    TField  *pstField = NULL;
    void    *pvWhere =  NULL;

    memset(&stIndex, 0, sizeof(TIndex));
    strncpy(stIndex.m_szPart, sgetvalue(pszTable, "@", 2), sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sgetvalue(pszTable, "@", 1), sizeof(stIndex.m_szTable));
    supper(stIndex.m_szTable);
    sGetNode(stIndex.m_szPart);

    if(RC_SUCC != lGetTblIndex(pstSavm, stIndex.m_szTable, stIndex.m_szPart, &stIndex))
        return RC_FAIL;

    if(RC_SUCC != lGetTblField(stIndex.m_table, &lOut, &pstField))
        return RC_FAIL;

    if(NULL == (pvWhere = (char *)calloc(stIndex.m_lRowSize, sizeof(char))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        goto ERR_DELETE;
    }

    if(RC_SUCC != _lConditField(pstSavm, pszWhere, lOut, pstField, pvWhere, ""))
    {
        pstSavm->m_lErrno = SQL_ERR_WHERE;
        goto ERR_DELETE;
    }

//vPrintHex(pvWhere, stIndex.m_lRowSize);    

    IBPFree(pstField);
    if(RC_SUCC != _lExeDelete(pstSavm, &stIndex, pvWhere))
        goto ERR_DELETE;

    IBPFree(pvWhere);
    return RC_SUCC;

ERR_DELETE:
    IBPFree(pvWhere);
    IBPFree(pstField);
    return RC_FAIL;
}
/**************************************************************************************************
    函 数 名：解析SQL-delete语法
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    _lDeleteSyntax(SATvm *pstSavm, char *pszSQL)
{
    char    szTable[MAX_FIELD_LEN], szWhere[1024];

    memset(szWhere, 0, sizeof(szWhere));
    memset(szTable, 0, sizeof(szTable));
    if(strcasestr(pszSQL, " where "))
    {
        if(!sGetTruckValue(pszSQL, " from ", " where ", true, szTable, sizeof(szTable)))
        {
            pstSavm->m_lErrno = SQL_TABLE_NIL;
            return RC_FAIL;
        }
        strimall(szTable);

        if(!sGetTruckValue(pszSQL, " where ", NULL, true, szWhere, sizeof(szWhere)))
        {
            pstSavm->m_lErrno = SQL_WHERE_NIL;
            return RC_FAIL;
        }
        sltrim(szWhere);   //  条件
        srtrim(szWhere);
        sUpperWord(szWhere, " AND ");
        DEL_TAIL_CHAR(szWhere, ';');
    }
    else
    {
        if(!sGetTruckValue(pszSQL, " from ", NULL, true, szTable, sizeof(szTable)))
        {
            pstSavm->m_lErrno = SQL_TABLE_NIL;
            return RC_FAIL;
        }
        DEL_TAIL_CHAR(szTable, ';');
        strimall(szTable);
    }    

    vSCRDebug("DEBUG:delete table:[%s]", szTable);
    vSCRDebug("DEBUG:delete where:[%s]", szWhere);

    return _lParseDelete(pstSavm, szTable, szWhere);
}

/*************************************************************************************************
    函数说明：解析SQL-insert 语法字段
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lInsertField(SATvm *pstSavm, char *pszValues, SQLFld *pstRoot, void *pvInsert)
{
    void    *v = NULL;
    TblKey    *pstKey = NULL;
    SQLFld    *pstNode = NULL;
    long    lRec = 0, i, j, lFld;
    char    szValue[512], szNumber[16];

    memset(szNumber, 0, sizeof(szNumber));
    lFld = lGetNodeFiled(pstRoot);
    lRec = lfieldnum(pszValues, ",");
    if(lFld != lRec)
    {
        pstSavm->m_lErrno = SQL_FAV_MATCH;
        return RC_FAIL;
    }
    
    for(i = 0, v = (void *)szNumber, pstNode = pstRoot; i < lRec; i ++, pstNode = pstNode->pstNext)
    {
        memset(szValue, 0, sizeof(szValue));
        strncpy(szValue, sfieldvalue(pszValues, ",", i + 1), sizeof(szValue));
        sltrim(szValue);
        srtrim(szValue);
        strimabout(szValue, "\'", "\'");

        if(!pstNode)
        {
            pstSavm->m_lErrno = SQL_FAV_MATCH;
            return RC_FAIL;
        }

        pstKey = &pstNode->m_stKey;
        switch(pstKey->m_lAttr)
        {
        case FIELD_DOUBLE:
            strimall(szValue);
            switch(pstKey->m_lLen)
            {
            case    4:
                *((float *)v) = atof(szValue);
                memcpy(pvInsert + pstKey->m_lFrom, v, pstKey->m_lLen);
                break;
            case    8:
                *((double *)v) = atof(szValue);
                memcpy(pvInsert + pstKey->m_lFrom, v, pstKey->m_lLen);
                break;
            default:
                break;
            }
            break;
        case FIELD_LONG:
            strimall(szValue);
            switch(pstKey->m_lLen)
            {
            case    2:
                *((sint *)v) = atoi(szValue);
                memcpy(pvInsert + pstKey->m_lFrom, v, pstKey->m_lLen);
                break;
            case    4:
                *((int *)v) = atoi(szValue);
                memcpy(pvInsert + pstKey->m_lFrom, v, pstKey->m_lLen);
                break;
            case    8:
                *((llong *)v) = atol(szValue);
                memcpy(pvInsert + pstKey->m_lFrom, v, pstKey->m_lLen);
                break;
            default:
                break;
            }
            break;
        case FIELD_CHAR:
            memcpy(pvInsert + pstKey->m_lFrom, szValue,
                MIN(strlen(szValue), pstKey->m_lLen));
            break;
        default:
            break;
        }
    }

    if(pstNode)
    {
        pstSavm->m_lErrno = SQL_FAV_MATCH;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函数说明：解析SQL-insert 语法字段
    参数说明：
    返回值：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
  *************************************************************************************************/
long    _lParseInsert(SATvm *pstSavm, char *pszTable, char *pszField, char *pszValues)
{
    TIndex  stIndex;
    size_t  lOut = 0;
    SQLFld  *pstRoot = NULL;
    TField  *pstField = NULL;
    void    *pvInsert =  NULL;

    memset(&stIndex, 0, sizeof(TIndex));
    strncpy(stIndex.m_szPart, sgetvalue(pszTable, "@", 2), sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sgetvalue(pszTable, "@", 1), sizeof(stIndex.m_szTable));
    supper(stIndex.m_szTable);    
    sGetNode(stIndex.m_szPart);

    if(RC_SUCC != lGetTblIndex(pstSavm, stIndex.m_szTable, stIndex.m_szPart, &stIndex))
        return RC_FAIL;

    if(RC_SUCC != lGetTblField(stIndex.m_table, &lOut, &pstField))
        return RC_FAIL;

    if(RC_SUCC != _lSelectField(pstSavm, pszField, lOut, pstField, &pstRoot))
    {
        pstSavm->m_lErrno = SQL_ERR_FIELD;
        goto ERR_INSERT;
    }

    if(NULL == (pvInsert = (char *)calloc(stIndex.m_lRowSize, sizeof(char))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        goto ERR_INSERT;
    }

    if(RC_SUCC != _lInsertField(pstSavm, pszValues, pstRoot, pvInsert))
        goto ERR_INSERT;

//vPrintHex(pvInsert, stIndex.m_lRowSize);    

    IBPFree(pstField);
    if(RC_SUCC != _lExeInsert(pstSavm, &stIndex, pvInsert))
        goto ERR_INSERT;

    IBPFree(pvInsert);
    return RC_SUCC;

ERR_INSERT:
    IBPFree(pvInsert);
    IBPFree(pstField);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：解析SQL-insert语法
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    _lInsertSyntax(SATvm *pstSavm, char *pszSQL)
{
    char    szTable[MAX_FIELD_LEN];
    char    szValues[1024], szField[1024];

    memset(szField, 0, sizeof(szField));
    memset(szTable, 0, sizeof(szTable));
    memset(szValues, 0, sizeof(szValues));
    if(!sGetTruckValue(pszSQL, " into ", "values", true, szValues, sizeof(szValues)))
    {
        pstSavm->m_lErrno = SQL_TABLE_NIL;
        return RC_FAIL;
    }

    if(strstr(szValues, "("))    //    说明有选定字段
    {
        if(!sGetTruckValue(szValues, NULL, "(", true, szTable, sizeof(szTable)))
        {
            pstSavm->m_lErrno = SQL_TABLE_NIL;
            return RC_FAIL;
        }

        if(!sGetTruckValue(szValues, "(", ")", true, szField, sizeof(szField)))
        {
            pstSavm->m_lErrno = SQL_FIELD_NIL;
            return RC_FAIL;
        }

        if(!strlen(strimall(szField)))
        {
            pstSavm->m_lErrno = SQL_FIELD_NIL;
            return RC_FAIL;
        }
    }
    else
    {
        strcpy(szField, "*");
        strncpy(szTable, szValues, sizeof(szTable));
    }
    strimall(szTable);

    memset(szValues, 0, sizeof(szValues));
    if(!sGetTruckValue(pszSQL, "values", NULL, true, szValues, sizeof(szValues)))
    {
        pstSavm->m_lErrno = SQL_WHERE_NIL;
        return RC_FAIL;
    }

    if(!strimabout(szValues, "(", ")"))
    {
        pstSavm->m_lErrno = SQL_WHERE_NIL;
        return RC_FAIL;
    }

    if(!strlen(szValues))
    {
        pstSavm->m_lErrno = SQL_WHERE_NIL;
        return RC_FAIL;
    }

    sltrim(szValues);   //  条件
    srtrim(szValues);
    sUpperWord(szValues, " AND ");

    vSCRDebug("DEBUG:insert field:[%s]", szField);
    vSCRDebug("DEBUG:insert table:[%s]", szTable);
    vSCRDebug("DEBUG:insert field:[%s]", szValues);

    return _lParseInsert(pstSavm, szTable, szField, szValues);
}

/**************************************************************************************************
    函 数 名：清理表数据
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    _lTruncateSyntax(SATvm *pstSavm, char *pszSQL)
{
    TIndex  stIndex;
    char    szTable[MAX_FIELD_LEN];

    memset(szTable, 0, sizeof(szTable));
    memset(&stIndex, 0, sizeof(TIndex));
    if(!sGetTruckValue(pszSQL, " table ", NULL, true, szTable, sizeof(szTable)))
    {
        pstSavm->m_lErrno = SQL_TABLE_NIL;
        return RC_FAIL;
    }

    strimall(szTable);
    strncpy(stIndex.m_szPart, sgetvalue(szTable, "@", 2), sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sgetvalue(szTable, "@", 1), sizeof(stIndex.m_szTable));
    supper(stIndex.m_szTable);
    sGetNode(stIndex.m_szPart);

    if(RC_SUCC != lGetTblIndex(pstSavm, stIndex.m_szTable, stIndex.m_szPart, &stIndex))
        return RC_FAIL;

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(stIndex.m_table)))
        return RC_FAIL;

    pstSavm->lSize = stIndex.m_lRowSize;
    if(RC_SUCC != lTruncate(pstSavm, stIndex.m_table))
        return RC_FAIL;

    fprintf(stdout, "---(%d)条记录被删除---\n", pstSavm->m_lEffect);
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：删除表
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    _lDropSyntax(SATvm *pstSavm, char *pszSQL)
{
    TIndex  stIndex;
    char    szTable[MAX_FIELD_LEN];

    memset(szTable, 0, sizeof(szTable));
    memset(&stIndex, 0, sizeof(TIndex));
    if(!sGetTruckValue(pszSQL, " table ", NULL, true, szTable, sizeof(szTable)))
    {
        pstSavm->m_lErrno = SQL_TABLE_NIL;
        return RC_FAIL;
    }

    strimall(szTable);
    strncpy(stIndex.m_szPart, sgetvalue(szTable, "@", 2), sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sgetvalue(szTable, "@", 1), sizeof(stIndex.m_szTable));
    supper(stIndex.m_szTable);
    sGetNode(stIndex.m_szPart);

    if(RC_SUCC != lGetTblIndex(pstSavm, stIndex.m_szTable, stIndex.m_szPart, &stIndex))
        return RC_FAIL;

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(stIndex.m_table)))
        return RC_FAIL;

    pstSavm->lSize = stIndex.m_lRowSize;

    if(RC_SUCC != lDropTable(pstSavm, stIndex.m_table))
        return RC_FAIL;

    fprintf(stdout, "---(%s)表已被删除---\n", szTable);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：从文件内容按格式导入表中
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    _lLoadSyntax(SATvm *pstSavm, char *pszSQL)
{
    TIndex  stIndex;
    char    szFile[256], szParam[256], szDelmit[64];

//    load from a.txt DELIMITER ',' insert into tablename@bcs 
    memset(szFile, 0, sizeof(szFile));
    memset(szParam, 0, sizeof(szParam));
    memset(szDelmit, 0, sizeof(szDelmit));
    memset(&stIndex, 0, sizeof(TIndex));

    if(!sGetTruckValue(pszSQL, "insert into ", NULL, true, szParam, sizeof(szParam)))
    {
        pstSavm->m_lErrno = SQL_TABLE_NIL;
        return RC_FAIL;
    }

    sltrim(szParam);
    srtrim(szParam);
    strimall(szParam);
    strncpy(stIndex.m_szPart, sgetvalue(szParam, "@", 2), sizeof(stIndex.m_szPart));
    strncpy(stIndex.m_szTable, sgetvalue(szParam, "@", 1), sizeof(stIndex.m_szTable));
    supper(stIndex.m_szTable);
    sGetNode(stIndex.m_szPart);

    if(RC_SUCC != lGetTblIndex(pstSavm, stIndex.m_szTable, stIndex.m_szPart, &stIndex))
        return RC_FAIL;

    memset(szParam, 0, sizeof(szParam));
    if(!sGetTruckValue(pszSQL, "load from ", "insert", true, szParam, sizeof(szParam)))
    {
        pstSavm->m_lErrno = FILE_NOT_RSET;
        return RC_FAIL;
    }

    strncpy(szFile, sfieldvalue(szParam, " ", 1), sizeof(szFile));
    strimall(szFile);

    strncpy(szDelmit, sfieldvalue(szParam, " ", 2), sizeof(szDelmit));
    strimall(szDelmit);
    if(!strcasecmp(szDelmit, "DELIMITER"))
    {
        strncpy(szDelmit, sfieldvalue(szParam, " ", 3), sizeof(szDelmit));
           strimall(szDelmit);
        strimabout(szDelmit, "\"", "\"");
        strimabout(szDelmit, "\'", "\'");
    }
    else 
        strcpy(szDelmit, ",");

    if(strlen(sfieldvalue(szParam, " ", 4)))
    {
        pstSavm->m_lErrno = SQL_SYNTX_ERR;
        return RC_FAIL;
    }

    if(RC_SUCC != lImportFile(stIndex.m_table, szFile, szDelmit))
        return RC_FAIL;

    fprintf(stdout, "---(%d)条记录被新增---\n", lGetEffect());

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：从表中数据按照格式导出到文件中
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    _lUnloadSyntax(SATvm *pstSavm, char *pszSQL)
{
    char    szFile[256], szParam[256], szDelmit[64], *p = NULL;

//    unload to a.txt DELIMITER ',' select * from tablename@bcs 
    memset(szFile, 0, sizeof(szFile));
    memset(szParam, 0, sizeof(szParam));
    memset(szDelmit, 0, sizeof(szDelmit));

    if(NULL == (p = strcasestr(pszSQL, "select")))
    {
        pstSavm->m_lErrno = SQL_SYNTX_ERR;
        return RC_FAIL;
    }

    memset(szParam, 0, sizeof(szParam));
    if(!sGetTruckValue(pszSQL, "unload to ", "select", true, szParam, sizeof(szParam)))
    {
        pstSavm->m_lErrno = FILE_NOT_RSET;
        return RC_FAIL;
    }

    sltrim(szParam);
    srtrim(szParam);
    strncpy(szFile, sfieldvalue(szParam, " ", 1), sizeof(szFile));
    strimall(szFile);

    strncpy(szDelmit, sfieldvalue(szParam, " ", 2), sizeof(szDelmit));
    strimall(szDelmit);
    if(!strcasecmp(szDelmit, "DELIMITER"))
    {
        strncpy(szDelmit, sfieldvalue(szParam, " ", 3), sizeof(szDelmit));
        strimall(szDelmit);
        strimabout(szDelmit, "\"", "\"");
        strimabout(szDelmit, "\'", "\'");
    }
    else
        strcpy(szDelmit, ",");

    if(strlen(sfieldvalue(szParam, " ", 4)))
    {
        pstSavm->m_lErrno = SQL_SYNTX_ERR;
        return RC_FAIL;
    }

    return _lSelectSyntax(pstSavm, p, szFile, szDelmit);
}

/**************************************************************************************************
    函 数 名：执行SQL语句
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    lExecuteSQL(SATvm *pstSavm, char *pszSQL)
{
    if(!pszSQL || !strlen(pszSQL))
    {
        pstSavm->m_lErrno = SQL_SYNTX_ERR;
        return RC_FAIL;
    }

	pstSavm->m_lTimes = 0;
    pstSavm->lReMatch = 0;
    pstSavm->lReField = 0;
	pstSavm->m_lErrno = 0;
    sfieldreplace(pszSQL, '\t', ' ');
    if(!strncasecmp(pszSQL, "begin ", 6) && strcmp(sfieldvalue(pszSQL, ";", 2), "work"))
	{
		vBeginWork(pstSavm);	
		fprintf(stdout, "---启动事务完成, %s---\n", sGetTError(pstSavm->m_lErrno));
		return RC_SUCC;
	}
    else if(!strncasecmp(pszSQL, "end ", 4) && strcmp(sfieldvalue(pszSQL, ";", 2), "work"))
	{
		vEndWork(pstSavm);	
		fprintf(stdout, "---关闭事务完成, %s---\n", sGetTError(pstSavm->m_lErrno));
		return RC_SUCC;
	}
    else if(!strncasecmp(pszSQL, "commit ", 7) && strcmp(sfieldvalue(pszSQL, ";", 2), "work"))
	{
		lCommitWork(pstSavm);	
		fprintf(stdout, "---提交事完成, %s---\n", sGetTError(pstSavm->m_lErrno));
		return RC_SUCC;
	}
    else if(!strncasecmp(pszSQL, "rollback ", 9) && strcmp(sfieldvalue(pszSQL, ";", 2), "work"))
	{
		lRollbackWork(pstSavm);	
		fprintf(stdout, "---(%d)条记录被回滚---\n", pstSavm->m_lEffect);
		return RC_SUCC;
	}
    else if(!strncasecmp(pszSQL, "select ", 7))
        return _lSelectSyntax(pstSavm, pszSQL, NULL, NULL);
    else if(!strncasecmp(pszSQL, "update ", 7))
        return _lUpdateSyntax(pstSavm, pszSQL);
    else if(!strncasecmp(pszSQL, "delete ", 7))
        return _lDeleteSyntax(pstSavm, pszSQL);
    else if(!strncasecmp(pszSQL, "insert ", 7))
        return _lInsertSyntax(pstSavm, pszSQL);
    else if(!strncasecmp(pszSQL, "truncate ", 9))
        return _lTruncateSyntax(pstSavm, pszSQL);
    else if(!strncasecmp(pszSQL, "drop ", 5))
        return _lDropSyntax(pstSavm, pszSQL);
    else if(!strncasecmp(pszSQL, "load ", 5))
        return _lLoadSyntax(pstSavm, pszSQL);
    else if(!strncasecmp(pszSQL, "unload ", 7))
        return _lUnloadSyntax(pstSavm, pszSQL);
    else
    {
        pstSavm->m_lErrno = SQL_NOT_SUPPT;
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：启动TVM
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    lStartSystem(char *pszMode)
{
    long    lPort, lProcess, lMode = 0;
    TBoot   *pstBoot = (TBoot *)pGetBoot();
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(pszMode && !strcmp(pszMode, "o"))    //    离线
    {
        fprintf(stdout, "Warning:TVM will start offline\n");
        fflush(stdout);
        lMode = 1;
    }

    if(RC_SUCC != lBootLocal(pstSavm, pstBoot))
    {
        fprintf(stderr, "failed to boot LIS, %s\n", sGetTError(pstSavm->m_lErrno));
        return RC_SUCC;
    }

    if(!bIsTvmBoot() && RC_SUCC != lStartupTvm(pstBoot))
    {
        fprintf(stderr, "failed to boot TVM, %s\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lBootDomain(pstSavm, pstBoot, lMode))
    {
        fprintf(stderr, "boot RDS failed, %s\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    fprintf(stderr, "start TVM : (%s)\n", sGetTError(pstSavm->m_lErrno));

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：停止TVM
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
long    lStopSystem(char *pszApp)
{
    long    lPid = 0;
    FILE    *fp = NULL;
    static    char    szCmd[128], szPid[20];
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    memset(szPid, 0, sizeof(szPid));
    memset(szCmd, 0, sizeof(szCmd));

    snprintf(szCmd, sizeof(szCmd), "ps -u %s|grep -E \"%s|%s\"|awk '{print $1}'", 
        getenv("LOGNAME"), TVM_LOCAL_SERV, TVM_REMOTE_DOM);

    lOfflineNotify(pstSavm, lGetLDPort());

    if(NULL == (fp = popen(szCmd, "r")))
    {
        fprintf(stderr, "popen execute comman err:(%s)\n", strerror(errno));
        return RC_FAIL;
    }

    for(fp; fgets(szPid, sizeof(szPid), fp) != NULL; memset(szPid, 0, sizeof(szPid)))
    {
        strimall(szPid);
        lPid = atol(szPid);
        if(lPid <= 0 || lPid == getpid())
            continue;

        kill(atol(szPid), SIGKILL);
    }

    pclose(fp);

    if(!bIsTvmBoot()) return RC_SUCC;

    if(RC_SUCC != lShutdownTvm())
    {
        fprintf(stderr, "showdown node failed, %s\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：输出参数信息
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
void    vPrintParam(char *pszOpt)
{
    long    n = 0, i = 0;

    if(!pszOpt || (0 == (n = strlen(pszOpt))))  return ;

    if(!bIsTvmBoot())
        return ;

    for(i = 0, n = strlen(pszOpt); i < n; i ++)
    {
        switch(pszOpt[i])
        {
        case    'y':
            vPrintIndex();
            break;
        case    't':
            vPrintField();
            break;
        case    'd':
            vPrintDomain();
            break;
        default:
            return ;
        }
    }

    return ;
}

/**************************************************************************************************
    函 数 名：输出表空间使用量
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
void    vPrintAmount(int t, char *pszTable, int nValid, int nMax)
{
    double  dPer;
    int     i, nPer;

    if(nValid < 0 || nMax <= 0)   return ;

    dPer = nValid * 100.0 / nMax;
    nPer = nValid * 50 / nMax > 0 ? nValid * 50 / nMax : 1;

    fprintf(stdout, "TABLE:[%3d][%-20s]: [", t, pszTable);
    if(dPer < 60.00)
        fprintf(stdout, "\033[42;32m");
    else if(dPer < 70.00)
        fprintf(stdout, "\033[45;35m");
    else if(dPer < 80.00)
        fprintf(stdout, "\033[46;36m");
    else  if(dPer < 90.00)
        fprintf(stdout, "\033[43;33m");
    else
        fprintf(stdout, "\033[41;31m");

    fflush(stdout);
    for(i = 0; i < nPer; i ++)
    {
        fprintf(stdout, "|");
        fflush(stdout);
    }

    fprintf(stdout, "\033[0m");
    for(i; i < 50; i ++)
        fprintf(stdout, " ");
    fprintf(stdout, "] %.4f%%, (%d/%d)\n", dPer, nValid, nMax);
    fflush(stdout);
}

/**************************************************************************************************
    函 数 名：查找系统表空间使用量
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
void    vTableAmount()
{
    size_t  i, lOut = 0;
    RunTime *pstRun = NULL;
    TIndex  stIndex, *pstIndex = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    memset(&stIndex, 0, sizeof(TIndex));
    stIndex.m_lLocal = RES_LOCAL_SID;

    pstSavm->pstVoid = &stIndex;
    pstSavm->lFind   = IDX_SELECT;
    pstSavm->bSearch = TYPE_SYSTEM;
    pstSavm->tblName = SYS_TVM_INDEX;
    pstSavm->lSize   = sizeof(TIndex);
    if(RC_SUCC != lQuery(pstSavm, &lOut, (void *)&pstIndex))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            pstSavm->m_lErrno = TBL_NOT_FOUND;
        return ;
    }

    if(lOut <= 0)    return ;

    fprintf(stdout, "The amount of table is using as follows:\n\n");
    for(i = 0; i < lOut; i ++)
    {
        pstRun = (RunTime *)pGetRunTime(pstSavm, pstIndex[i].m_table);
        pstRun->m_shmID  = pstIndex[i].m_shmID;
        if(NULL == (pstRun = pInitHitTest(pstSavm, pstIndex[i].m_table)))
            continue;

        vPrintAmount(pstIndex[i].m_table, pstIndex[i].m_szTable, lGetTblValid(pstIndex[i].m_table),
             lGetTblRow(pstIndex[i].m_table));
        vTblDisconnect(pstSavm, pstIndex[i].m_table);
    }
    IBPFree(pstIndex);
    fprintf(stdout, "\n");

    return ;
}

/**************************************************************************************************
    函 数 名：vPrintFunc(char *s)
    功能描述：cts操作功能说明
    返回说明：
        无
 **************************************************************************************************/
void    vPrintFunc(char *s)
{
    fprintf(stdout, "\nUsage:\t%s -[SWUERFTkcstwpoth][aeswd]\n", s);
    fprintf(stdout, "\t-w[o]\t--启动TVM系统\n");
    fprintf(stdout, "\t-s[o]\t--停止TVM系统\n");
    fprintf(stdout, "\t-p(ytd)\t--打印系统运行信息\n");
    fprintf(stdout, "\t-f\t--显示当前各表用量\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "\033[4m\033[45;33mDESCRIPTION\033[0m\n");
    fprintf(stdout, "\t\033[0m\033[33;40mSQL\t\t--启动SQL控制台\033[0m\n");
    fprintf(stdout, "\t\033[0m\033[33;40mDOM\t\t--启动DOM控制台\033[0m\n");
    fprintf(stdout, "\n");
}

/****************************************************************************************
 *     输入检测
 ****************************************************************************************/
void    vGetInput(char *pszPut, long l, long m)
{
    long    n = 0, len;

    for(n = 0; n < l; n ++)
    {
        pszPut[n] = getchar();
        if(10 == pszPut[n])
        {
            pszPut[n] = 0x00;
            if(1 == m) return ;

            if(!strlen(pszPut)) //  非1模式
                n = -1;         //  重新等待输入
            else
                return ;
        }
    }
}

/**************************************************************************************************
    函 数 名：执行SQL功能
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
void    vSQLStatement(int argc, char *argv[])
{
    long    i = 0, lRec;
    char    *pszUser, szSQL[2048];
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

	for(i = 0; i < argc; i ++)
	{
		if(!strcmp(argv[i], "debug=on"))
			elog = 1;
	}			

    system("stty erase ^?");
    system("stty erase ^H");
    fprintf(stdout, "\n%s\n", sFuncVersion());
    while(1)
    {
        if(NULL == (pszUser = readline("M-SQL>")))
            continue;

        add_history(pszUser);
        if(!strcmp(pszUser, "q") || !strcmp(pszUser, "Q") || !strcmp(pszUser, "exit"))
            exit(0);
        else if(!strcasecmp(pszUser, "clear"))
        {
            system("clear");
            IBPFree(pszUser);
            continue;
        }

        strimcrlf(pszUser);
        lRec = lfieldnum(pszUser, ";");
        for(i = 0; i < lRec; i ++)
        {
            memset(szSQL, 0, sizeof(szSQL));
            strncpy(szSQL, sfieldvalue(pszUser, ";", i + 1), sizeof(szSQL));
            sltrim(szSQL);
            srtrim(szSQL);
            if(!strlen(szSQL))    continue;

            fprintf(stdout, "\n------------------------------------------------------SQL Result"
                "-----------------------------------------------------\n");
            if(RC_SUCC != lExecuteSQL(pstSavm, szSQL))
            {
                fprintf(stderr, "execute M-SQL error, (%d)(%s)\n", pstSavm->m_lErrno, 
                    sGetTError(pstSavm->m_lErrno));
                IBPFree(pszUser);
                continue;
            }
        }
        IBPFree(pszUser);
        fprintf(stdout, "\n\n\n");
    }

    exit(0);
}

/**************************************************************************************************
    函 数 名：输出域表信息
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
void    vPrintDomTable()
{
    size_t  i, lOut = 0;
    char    szPrint[512];
    TDomain *pstDomain = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lExportTable(SYS_TVM_DOMAIN, &lOut, (void **)&pstDomain))
    {
        fprintf(stderr, "get domain node info failed, %s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    fprintf(stdout, "\tROW    DOMAINID   TABLE_NAME         PART       STAT  PERS\n");
    fprintf(stdout, "\t----------------------------------------------------------\n");
    for(i = 0; i < lOut; i ++)
    {
        memset(szPrint, 0, sizeof(szPrint));
        snprintf(szPrint, sizeof(szPrint), "\t[%-3d]: %-10s %-18s %-10s ", i, 
            pstDomain[i].m_szOwner, pstDomain[i].m_szTable, pstDomain[i].m_szPart);

        if(RESOURCE_INIT == pstDomain[i].m_lStatus)
            strcat(szPrint, "INIT ");
        else if(RESOURCE_EXCP == pstDomain[i].m_lStatus)
            strcat(szPrint, "EXCP ");
        else if(RESOURCE_ABLE == pstDomain[i].m_lStatus)
            strcat(szPrint, "ABLE ");
        else if(RESOURCE_STOP == pstDomain[i].m_lStatus)
            strcat(szPrint, "STOP ");
        else if(RESOURCE_AUTH == pstDomain[i].m_lStatus)
            strcat(szPrint, "AUTH ");

        fprintf(stdout, "%s %s\n", szPrint, sPermitConv(pstDomain[i].m_lPers));
        fflush(stdout);
    }
    IBPFree(pstDomain);
    fprintf(stdout, "\t----------------------------------------------------------\n");

    return ;
}

/**************************************************************************************************
    函 数 名：手动连接远程域
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
void    vConnectDomain(char *pszDomain)
{
    size_t  i, lOut = 0;
    TDomain stDomain, *pstDomain = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    memset(&stDomain, 0, sizeof(TDomain));
    strncpy(stDomain.m_szOwner, pszDomain, sizeof(stDomain.m_szOwner));

    if(!strlen(strimall(stDomain.m_szOwner)))
    {
        fprintf(stderr, "*域名输入异常\n");
        return ;
    }

    if(RC_SUCC != lInitSATvm(pstSavm, SYS_TVM_DOMAIN))
    {
        fprintf(stderr, "*获取域配置信息失败, %s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->lSize   = sizeof(TDomain);
    pstSavm->pstVoid = (void *)&stDomain;
    if(RC_SUCC != lQuery(pstSavm, &lOut, (void *)&pstDomain))
    {
        fprintf(stderr, "*获取远程域(%s)信息失败, %s\n", pszDomain, sGetTError(pstSavm->m_lErrno));
        return ;
    }

    if(0 == lOut)
    {
        fprintf(stderr, "*输入错误\n");
        return ;
    }

    if(RC_SUCC != lConnectNotify(pstSavm, &pstDomain[0]))
    {
        fprintf(stderr, "*重连远程域(%s)失败, %s\n", pszDomain, sGetTError(pstSavm->m_lErrno));
        IBPFree(pstDomain);
        return ;
    }

    IBPFree(pstDomain);
    vPrintDomTable();
    return ;
}

/**************************************************************************************************
    函 数 名：pull远程域表数据
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
void    vPullTableDomain(char *pszParam)
{
    TABLE    table = 0;
    TDomain    stDomain;
    char    szCmd[512], szTable[128];
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

//    BCS@TBL_ACCT_INFO/PART --table=1 指定表名, 不指定， 按照m_mtable
    memset(szCmd, 0, sizeof(szCmd));
    memset(szTable, 0, sizeof(szTable));
    memset(&stDomain, 0, sizeof(TDomain));
    if(NULL == strstr(pszParam, "/"))
    {
        fprintf(stderr, "*pull参数格式错误\"%s\"\n", szCmd);
        return ;
    }

    strncpy(stDomain.m_szOwner, sgetvalue(pszParam, "/", 1), MAX_FIELD_LEN);
    if(!strlen(strimall(stDomain.m_szOwner)))
    {
        fprintf(stderr, "*域名输入异常\n");
        return ;
    }

    //    TBL_ACCT_INFO/PART --table=1
    strncpy(szCmd, sgetvalue(pszParam, "/", 2), sizeof(szCmd));
    strncpy(szTable, sgetvalue(szCmd, " ", 1), sizeof(szTable));
    strimall(szTable);

    strncpy(stDomain.m_szTable, sgetvalue(szTable, "@", 1), MAX_FIELD_LEN);
    supper(stDomain.m_szTable);
    if(!strlen(stDomain.m_szTable))
    {
        fprintf(stderr, "*请输入正确的表名\n");
        return ;
    }

    strncpy(stDomain.m_szPart, sgetvalue(szTable, "@", 2), MAX_FIELD_LEN);
    if(!strlen(stDomain.m_szPart))
        strcpy(stDomain.m_szPart, stDomain.m_szOwner);

    memset(szTable, 0, sizeof(szTable));
    strncpy(szTable, sgetvalue(szCmd, " ", 2), sizeof(szTable));
    strimall(szTable);
    if(strlen(szTable) > 0)
    {
        if(!strncmp(szTable, "--table=", 8))
        {
            table = atol(szTable + 8);
            if(table <= 0 || table > TVM_MAX_TABLE)
            {
                fprintf(stderr, "*待导入的表序号设置错误\"%s\"\n", szTable + 8);
                return ;
            }
        }
        else
        {
            fprintf(stderr, "*暂不支持的命令\"%s\"\n", szTable);
            return ;
        }
    }

    if(RC_SUCC != lInitSATvm(pstSavm, SYS_TVM_DOMAIN))
    {
        fprintf(stderr, "*获取域配置信息失败, %s\n", sGetTError(pstSavm->m_lErrno));
        return ;
    }

    pstSavm->lFind   = CHK_SELECT;
    pstSavm->lSize   = sizeof(TDomain);
    pstSavm->pstVoid = (void *)&stDomain;
    if(RC_SUCC != lSelect(pstSavm, (void *)&stDomain))
    {
        fprintf(stderr, "*查询域(%s)(%s)配置信息失败, %s\n", stDomain.m_szOwner, 
            stDomain.m_szTable, sGetTError(pstSavm->m_lErrno));
        return ;
    }

    if(table == 0)
        table = stDomain.m_table;

    if(bTableIsExist(table) || bPartIsExist(stDomain.m_szTable, pstSavm->m_szNode))
    {
        fprintf(stderr, "*待导入的表序号(%d)已存在\n", table);
        return ;
    }

    if((RESOURCE_ABLE != stDomain.m_lStatus) || !(stDomain.m_lPers & OPERATE_SELECT))
    {   
        fprintf(stderr, "*表(%s)(%s)资源不可用\n", stDomain.m_szTable, stDomain.m_szPart);
        return ;
    }

    if(RC_SUCC != lPullNotify(pstSavm, &stDomain, table))
    {
        fprintf(stderr, "*拉取远程域(%s)数据失败, %s\n", stDomain.m_szOwner,
            sGetTError(pstSavm->m_lErrno));
        return ;
    }

    return ;
}

/**************************************************************************************************
    函 数 名：vPrintDomFunc()
    功能描述：domain显示、重连
    返回说明：
        无
 **************************************************************************************************/
void    vPrintDomFunc()
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    table\t\t--show remote tables\n");
    fprintf(stderr, "    connect $DOM\t--connect remote domain\n");
    fprintf(stderr, "    pull\t\t--pull remote table\n");
    fprintf(stderr, "    exp:pull domain/table_name@part --table=table\n");
    return ;
}

/**************************************************************************************************
    函 数 名：vDomainCrontrl()
    功能描述：domain显示、重连
    返回说明：
        无
 **************************************************************************************************/
void    vDomainCrontrl()
{
    long    i = 0;
    char    *p = NULL, *pszUser = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    system("stty erase ^?");
    system("stty erase ^H");
    fprintf(stdout, "\n%s\n", sFuncVersion());
    while(1)
    {
        if(NULL == (pszUser = readline("DOMAIN>")))
            continue;

        strimcrlf(pszUser);
        sltrim(pszUser);
        srtrim(pszUser);
        if(!strlen(pszUser))    continue;

        add_history(pszUser);
        
        if(!strcasecmp(pszUser, "help") || !strcasecmp(pszUser, "--help") || 
            !strcasecmp(pszUser, "-?"))
            vPrintDomFunc();
        else if(!strcasecmp(pszUser, "table"))
            vPrintDomTable();
        else if(!strncasecmp(pszUser, "connect", 7))
        {
            if(strncasecmp(pszUser, "connect ", 8))
            {
                fprintf(stderr, "Usage:\n\t--connect domain\n\n");
                IBPFree(pszUser);    
                continue;    
            }

            vConnectDomain(sgetvalue(pszUser, " ", 2));
        }
        else if(!strncasecmp(pszUser, "pull", 4))
        {
            if(strncasecmp(pszUser, "pull ", 5))
            {
                fprintf(stderr, "Usage:\n\tpull domain/table_name@part --table=table\n\n");
                IBPFree(pszUser);    
                continue;    
            }

            p = pszUser + 5;
            vPullTableDomain(p);
        }
        else if(!strcasecmp(pszUser, "q") || !strcasecmp(pszUser, "exit") || 
            !strcasecmp(pszUser, "quit"))
            exit(0);
        else
        {
            fprintf(stderr, "invalid option -- \"%s\"\n", pszUser);
            vPrintDomFunc();
        }

        fprintf(stdout, "\n");
        IBPFree(pszUser);    
    }

    exit(0);
}

/**************************************************************************************************
    函 数 名：vCheckTvmEnv()
    功能描述：检查tvm运行参数
    返回说明：
        无 
 **************************************************************************************************/
void    vCheckTvmEnv()
{
    int        nRet;

    if(!getenv("TVMCFG") && !getenv("TVMDBD"))
    {
        fprintf(stderr, "TVM runtime config isnot set yet!\n");
        exit(-1);
    }

    if(0 != access(getenv("TVMDBD"), F_OK | X_OK ))
    {
        fprintf(stderr, "\"TVMDBD\" directory isnot set yet!\n");
        exit(-1);
    }

    //    其他参数检查


    return ;
}

/**************************************************************************************************
    函 数 名：main(int argc, char *argv[])
    功能描述：
    返回说明：
        RC_SUCC                            --成功
        RC_FAIL                            --失败
 **************************************************************************************************/
int     main(int argc, char *argv[])
{
    TABLE    table;
    char    szCom[256];
    SATvm   *pstSavm = (SATvm *)pGetSATvm();
    int     iChoose = 0, lAction = 0, lRet = 0;

    vCheckTvmEnv();
    if(NULL == (TBoot *)pBootInitial())
    {
        fprintf(stderr, "tvm initial err, %s\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    memset(szCom, 0, sizeof(szCom));
    if(2 <= argc && (!strcasecmp(argv[1], "sql")))
        vSQLStatement(argc, argv);
    if(2 <= argc && (!strcasecmp(argv[1], "dom")))
        vDomainCrontrl(argc, argv);

    memset(szCom, 0, sizeof(szCom));
    while(-1 != (iChoose = getopt(argc, argv, "w::s::p::f::i:v?::")))
    {
        switch(iChoose)
        {
        case    'w':
            vInitTitle(argc, argv, environ);
            return lStartSystem(optarg);
        case    's': 
            return lStopSystem(argv[0]);
        case    'p':
            vPrintParam(optarg);
            return RC_SUCC;
        case    'f':
            vTableAmount();
            return RC_SUCC;
        case    'i':
            table = atol(optarg);
            if(RC_SUCC != lRebuildIndex(pstSavm, table))
                fprintf(stderr, "重建表索引失败, %s\n", sGetTError(pstSavm->m_lErrno));
            return RC_SUCC;
        case    'v':
            fprintf(stdout, "\nCopyright (C) 2016-2020 盒子支付(iBOXPAY.com) Technology Co., "
                "Ltd. All Rights Reversed\n");
            fprintf(stdout, "%s\n", sGetTVMVers());
            fflush(stdout);
            return RC_SUCC;
        case    '?':
        default:
            break;
        }
    }
    
    vPrintFunc(basename(argv[0]));
    return RC_SUCC;

/*
    vRunAction(lAction, basename(argv[0]), szCom);
*/
}

/**************************************************************************************************
 * code end
 **************************************************************************************************/
