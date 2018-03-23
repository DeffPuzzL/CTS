/**************************************************************************************************
    文 件 名：pack.c
    代码作者：DeffPuzzL、liujie
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：报文解析模块
 **************************************************************************************************/
#include    "face.h"

typedef long    (*UFunc)(SATvm *pstSavm, char *pszOut, long *plLen);

long    _lBuildProto(SATvm *, CtsPack *, char *pszOut, long *plLen, long lp, HHCMIE cm);
long    _lParseProto(SATvm *, CtsPack *, char *psvData, long *plLen, long lp, HHCMIE cm);

/*************************************************************************************************
    报文解析节点全局二级缓存
 *************************************************************************************************/
PKCache     *g_pstCache = NULL;
/*************************************************************************************************
    宏定义
 *************************************************************************************************/
#define IsParseNext(h,l,t,p)            if(t <= l)   \
                                        {   \
                                            IBTmsg(h, "节点:%2d->字段(%s)因报文数据不足而无法解析",\
                                                p.m_lSeq, p.m_szMark); \
                                            return RC_SUCC; \
                                        }

#define IsFixedSatify(h,p,to,ne)        if(to < ne)                \
                                        {   \
                                            IBTmsg(h, "字段(%s)(%d)<(%d)数据超长", p->m_szMark, to, ne);\
                                            to = 0; \
                                            return p->m_lLen;  \
                                        }   \
                                        else   \
                                        {   \
                                            to = ne;  \
                                        }
            
#define IsLenSatisfy(h,p,to,ne)         if(to < ne)   \
                                        {   \
                                            IBTmsg(h, "字段(%s)(%d)<(%d)数据超长", p->m_szMark, to, ne);\
                                            to = 0; \
                                            return ne + p->m_lLen;    \
                                        }   \
                                        else    \
                                        {   \
                                            to = ne;   \
                                        }

#define IsLocalLoop(p, t)               (PACK_PARENT_LOOP == p.m_lIsParant) && (t == p.m_lType)
#define IsLocalParent(p, t)             (t == p.m_lType)
#define IsLoop(p)                       (PACK_PARENT_LOOP == p.m_lIsParant)
#define IsConstValue(p)                 (p.m_lIsConst > 0)
#define IsParent(p)                     (p.m_lIsParant > 0)
#define IsChildren(p)                   (p.m_lIsParant == 0)
#define IsRemain(p)                     (ITEM_ALL_REMAIN == p.m_lSLen)
#define IsUserFunc(p)                   (p->m_lUser & 0x01)
#define IsSpecFunc(p)                   (p->m_lUser & 0x02)

/*************************************************************************************************
    函数定义:lPackReadFile(char *pszFile)
    ppszFile    --待解读的文件
 *************************************************************************************************/
void    vDebugPack(CtsPack *pstPack)
{
    if(!pstPack)    return ;

    IBPuser("lMid:%d, lQid:%d, lPQid:%d, lType:%d, lChild:%d, IsParant:%d, Seq:%d, Pos:%d, From:%d, Len:%d, "
        "IsConst:%d, lUser:%d, lPlSet:%d, lPlIdx:%d, lSLen:%d, szLabel:%s, szMark:%s, szConst:%s, "
        "szPlName:%s", pstPack->m_lMid, pstPack->m_lQid, pstPack->m_lPQid, pstPack->m_lType, pstPack->m_lChild, 
        pstPack->m_lIsParant, pstPack->m_lSeq, pstPack->m_lPos, pstPack->m_lFrom, pstPack->m_lLen, pstPack->m_lIsConst, 
        pstPack->m_lUser, pstPack->m_lPlSet, pstPack->m_lPlIdx, pstPack->m_lSLen, pstPack->m_szLabel, 
        pstPack->m_szMark, pstPack->m_szConst, pstPack->m_szPlName);
}

/**************************************************************************************************
    函 数 名：vFreeCache(PKCache **ppstCache, long lCnt)
    功能描述：释放报文解析缓存
    返回说明：
        无
 **************************************************************************************************/
void    vFreeCache()
{
    long     i;
    PKCache  *pv = &g_pstCache[0];

    if(!pv)       return ;

    for(i = pv->m_lMax - 1; 0 <= i; -- i)
    {
        pv = &g_pstCache[i];
        IBPFree(pv->m_pstRoot);
    }

    IBPFree(g_pstCache);
    g_pstCache = NULL;
}

/**************************************************************************************************
    函 数 名：pGetCache(long lMid)
    功能描述：获取对应报文编号的配置
    返回说明：
        无
 **************************************************************************************************/
CtsPack*    pGetCache(long lMid)
{
    PKCache    *pv = &g_pstCache[0];
    
    if(!pv)    return NULL;
    if(!pv || pv->m_lMax < lMid)
        return NULL;

    return g_pstCache[lMid].m_pstRoot;
}

/**************************************************************************************************
    函 数 名：nCallSort(const void *pvFrom, const void *pvNext)
    功能描述：比较排序回调函数
    返回说明：
        true
        false
 **************************************************************************************************/
int    nCallSort(const void *pvFrom, const void *pvNext)
{
    if (((CtsPack *)pvFrom)->m_lMid == ((CtsPack *)pvNext)->m_lMid)
        return ((CtsPack *)pvFrom)->m_lSeq > ((CtsPack *)pvNext)->m_lSeq;
    else
        return ((CtsPack *)pvFrom)->m_lMid > ((CtsPack *)pvNext)->m_lMid;
}

/**************************************************************************************************
    函 数 名：lCacheMalloc(SATvm *pstSavm)
    功能描述：初始化报文二级缓存空间
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
long    lCacheMalloc(SATvm *pstSavm)
{
    size_t    lMax;

    if((lMax = lGenPackIdx()) <= 0)
    {
        IBPerror("无效报文(%d)序号, %s", lMax, sGetError());
        return RC_FAIL;
    }
    
    vFreeCache();
    if(NULL == (g_pstCache = (PKCache *)calloc(lMax, sizeof(PKCache))))
    {
        IBPerror("创建系统内存(%d)失败, %s", lMax, strerror(errno));
        return RC_FAIL;
    }

    g_pstCache[0].m_lMax = lMax;

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInitCachePack(SATvm *pstSavm)
    功能描述：初始化报文二级缓存
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
long    lInitCachePack(SATvm *pstSavm)
{
    PKCache   *pv = NULL;
    size_t    i = 0, lRow = 0, lData;
    CtsPack   *pstPack = NULL, *pk = NULL;

    if(RC_SUCC != lExportTable(CTS_PACK_CONFIG, &lRow, (void **)&pstPack))
    {
        IBPerror("获取平台报文配置信息失败, err:(%s)\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(lRow <= 0)    return RC_SUCC;

    if(RC_SUCC != lCacheMalloc(pstSavm))
        return RC_FAIL;

    qsort((void *)pstPack, lRow, sizeof(CtsPack), nCallSort);
    for(i = 0, pk = &pstPack[i]; i < lRow; pk = &pstPack[i])
    {
		if(g_pstCache[0].m_lMax <= pk->m_lMid)
			break;

        pv = &g_pstCache[pk->m_lMid];
        pv->m_lMid = pk->m_lMid;
        lData = (pk->m_lChild + 1) * sizeof(CtsPack);
        if(NULL == (pv->m_pstRoot = (CtsPack *)malloc(lData)))
        {
            vFreeCache();
			IBPFree(pstPack);
            IBPerror("创建系统内存(%d)失败, %s", lData, strerror(errno));    
            return RC_FAIL;
        }
        memcpy(pv->m_pstRoot, pk, lData);
        i += pk->m_lChild + 1;
    }

    IBPFree(pstPack);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInitPackCache(SATvm *pstSavm, long lMid, PKCache **ppstCache)
    功能描述：初始化指定报文编号的二级缓存
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
long    lInitPackCache(SATvm *pstSavm, long lMid)
{
    size_t    lRow = 0;
    PKCache   *pv = NULL;

    if (lMid <= 0)    return RC_SUCC;

    pv = &g_pstCache[lMid];
    pv->m_lMid = lMid;
    if (RC_SUCC != lGetPackList(pstSavm, pv->m_lMid, &pv->m_pstRoot, &lRow))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            return RC_SUCC;

        IBPerror("获取报文编号(%d)配置失败, %s", lMid, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    qsort((void *)pv->m_pstRoot, lRow, sizeof(CtsPack), nCallSort);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInitClassPack(SATvm *pstSavm, CtsProcess *pstProcess)
    功能描述：加载执行码对应组件的报文配置信息
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
long    lInitClassPack(SATvm *pstSavm, CtsProcess *pstProcess)
{
    CtsHost     stHost;
    CtsClass    *pstClass = NULL;
    long        i = 0, lCount = 0;

    if(RC_SUCC != lGetHost(pstSavm, &stHost, pstProcess->m_lHost))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            return RC_SUCC;

        IBPerror("获取执行码(%s)主机(%s)信息失败, err:%s", pstProcess->m_szPGName, 
            pstProcess->m_lHost, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lGetHostClass(pstSavm, stHost.m_lHost, &pstClass, &lCount))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
            return RC_SUCC;

        IBPerror("获取主机(%s)组件信息失败, err:%s", pstProcess->m_lHost, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(RC_SUCC != lCacheMalloc(pstSavm))
        return RC_FAIL;

    for(i = 0; i < lCount; i++)
    {
        if (RC_SUCC != lInitPackCache(pstSavm, pstClass[i].m_lInMid) || 
            RC_SUCC != lInitPackCache(pstSavm, pstClass[i].m_lOutMid) || 
            RC_SUCC != lInitPackCache(pstSavm, pstClass[i].m_lErrMid))
        {
            vFreeCache();
            IBPFree(pstClass);
            return RC_FAIL;
        }
    }

    IBPFree(pstClass);

    return RC_SUCC;
}

/*************************************************************************************************
    函数定义:lPackReadFile(char *pszFile)
    ppszFile    --待解读的文件
 *************************************************************************************************/
long    lPackReadFile(char *pszFile)
{
    FILE    *fp = NULL;
    char    szPath[1024], szLine[1024], szName[TARGET_NAME_LEN];

    if(!pszFile || !strlen(pszFile))
        return RC_SUCC;

    memset(szPath, 0, sizeof(szPath));
    snprintf(szPath, sizeof(szPath), "%s/%s", getenv("FILEDIR"), pszFile);
    
    if(NULL == (fp = fopen(szPath, "rb")))
    {
        IBPerror("打开文件(%s)失败, err:%s", szPath, strerror(errno));
        return RC_FAIL;
    }

    memset(szLine, 0, sizeof(szLine));
    while(fgets(szLine, sizeof(szLine), fp))
    {
        if(!strstr(szLine, "="))
        {
            memset(szLine, 0, sizeof(szLine));
            continue;
        }

        strimcrlf(szLine);        
        sltrim(szLine);        
        srtrim(szLine);        
        
        memset(szName, 0, sizeof(szName));
        strncpy(szName, sGetValueByIdx(szLine, "=", 1), sizeof(szName));
        IBPaddstring(szName, sGetValueByIdx(szLine, "=", 2));
    }
    fclose(fp);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_vPutPoolValue(CtsPack *pstItem, char *pszValue, long lp)
    功能描述：对定长报文解析到变量池过程
    参数说明：
        pstItem                --字段报文配置信息
        pszValue            --待压入的值
        lp                    --变量池下标
    返回说明：
        无
 **************************************************************************************************/
void    _vPutPoolValue(CtsPack *pstItem, char *pszValue, long lLen, long lp, HHCMIE cm)
{
    if(0 == pstItem->m_lPlSet)    return ;

    switch(pstItem->m_lAttr)
    {
    case FILED_BINARY:
        IBTmsg(cm, "节点:%2d->字段(%s)类型(BINARY)取值(%s)", pstItem->m_lSeq, pstItem->m_szMark, 
            pszValue);
        IBTbin(cm, pszValue, lLen);
		IBTputbinaryidx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszValue, lLen);
        break;
    case FIELD_STRING:
		if(0x00 == pszValue[0] && pstItem->m_lNilOn)
			return ;
        IBTmsg(cm, "节点:%2d->字段(%s)类型(STRING)取值(%s)", pstItem->m_lSeq, pstItem->m_szMark, 
            pszValue);
		IBTputstringidx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszValue);
        break;
    case FIELD_NUMBER:
        IBTmsg(cm, "节点:%2d->字段(%s)类型(NUMBER)取值(%s)", pstItem->m_lSeq, pstItem->m_szMark, 
            pszValue);
		IBTputstringidx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszValue);
//		IBTputlongidx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, atol(pszValue));
        break;
    case FIELD_DOUBLE:
        IBTmsg(cm, "节点:%2d->字段(%s)类型(DOUBLE)取值(%s)", pstItem->m_lSeq, pstItem->m_szMark, 
            pszValue);
		IBTputstringidx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszValue);
//		IBTputdoubleidx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, atof(pszValue), 6);
        break;
    case FILED_SENACT:
		if(0x00 == pszValue[0] && pstItem->m_lNilOn)
			return ;
        IBTmsg(cm, "节点:%2d->字段(%s)类型(SENACT)取值(%s)", pstItem->m_lSeq, pstItem->m_szMark, 
            sDesensAct(pszValue));
		IBTputsenstridx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszValue, FILED_SENACT);
        break;
    case FILED_SENNAM:
		if(0x00 == pszValue[0] && pstItem->m_lNilOn)
			return ;
        IBTmsg(cm, "节点:%2d->字段(%s)类型(SENNAM)取值(%s)", pstItem->m_lSeq, pstItem->m_szMark, 
            sDesensName(pszValue));
		IBTputsenstridx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszValue, FILED_SENNAM);
        break;
    case FILED_SENTEL:
		if(0x00 == pszValue[0] && pstItem->m_lNilOn)
			return ;
        IBTmsg(cm, "节点:%2d->字段(%s)类型(SENTEL)取值(%s)", pstItem->m_lSeq, pstItem->m_szMark, 
            sDesensTel(pszValue));
		IBTputsenstridx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszValue, FILED_SENTEL);
        break;
    case FILED_SENMAL:
		if(0x00 == pszValue[0] && pstItem->m_lNilOn)
			return ;
        IBTmsg(cm, "节点:%2d->字段(%s)类型(SENMAL)取值(%s)", pstItem->m_lSeq, pstItem->m_szMark, 
            sDesensMail(pszValue));
		IBTputsenstridx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszValue, FILED_SENMAL);
        break;
    default:
        break;    
    }

    return ;
}

/**************************************************************************************************
    函 数 名：lGetSpecValue(CtsPack *pstItem, char *s, char *o, long *plen, HHCMIE cm)
    功能描述：获取报文字段真实值
    参数说明：
        lSLen                --指定长度类型
        lLen                --指定长度
        pszSpec                --字符串BUFFER
    返回说明：
        lLen                --整个字段所占用的长度
 **************************************************************************************************/
long    lGetSpecValue(CtsPack *pstItem, char *s, char *o, long *plen, HHCMIE cm)
{
    long    i, j, m, lLen = 0;
    char    szLen[9], szValue[TARGET_VALUE_LEN];

    memset(szLen, 0, sizeof(szLen));
    switch(pstItem->m_lSLen)
    {
    //    针对剩余全部，不需要在这里判断，这里默认定长报文
    case ITEM_ALL_REMAIN:            
        memcpy(o, s, *plen);
        o[*plen] = 0x00;
        return *plen;
    case ITEM_LEN_FIXED:                    //  指定长度为固定
        IsFixedSatify(cm, pstItem, *plen, pstItem->m_lLen);

        memcpy(o, s, pstItem->m_lLen);
        o[pstItem->m_lLen] = 0x00;
        return pstItem->m_lLen;
    case ITEM_BCD_FIXED:                    //  指定长度为BCD
        memcpy(szLen, s, pstItem->m_lLen);
        lLen = atol(szLen);
        IsLenSatisfy(cm, pstItem, *plen, lLen);

        memcpy(o, s + pstItem->m_lLen, lLen);
        o[lLen] = 0x00;
        return lLen + pstItem->m_lLen;
    case ITEM_BCDP_FIXED:                    //  根据压缩BCD来读取固定字节
        m = pstItem->m_lLen / 2 + pstItem->m_lLen % 2;
        lLen = btol(s, m);
        IsLenSatisfy(cm, pstItem, *plen, lLen);

        memcpy(o, s + m, lLen);
        o[lLen] = 0x00;
        return lLen + m;
    case ITEM_BCDP_BCDPL:                    //  根据压缩BCD来读取取整字节，左对齐
        m = pstItem->m_lLen / 2 + pstItem->m_lLen % 2;
        lLen = btol(s, m);
        IsLenSatisfy(cm, pstItem, *plen, lLen);

        j = lLen / 2 + lLen % 2;            //    获取的字节
        ptob(s + m, j, o, lLen + 1);
        o[lLen] = 0x00;
        return j + m;
    case ITEM_BCDP_BCDPR:                    //  根据压缩BCD来读取取整字节，右对齐
        m = pstItem->m_lLen / 2 + pstItem->m_lLen % 2;
        lLen = btol(s, m);
        IsLenSatisfy(cm, pstItem, *plen, lLen);

        j = lLen / 2 + lLen % 2;            //    获取的字节
        ptob(s + m, j, szValue, TARGET_VALUE_LEN);
        memcpy(o, szValue + lLen % 2, lLen + 1);
        return j + m;
    case ITEM_FIXED_BCDPL:                  //  根据固定来读取取整字节，左对齐
        IsFixedSatify(cm, pstItem, *plen, pstItem->m_lLen);

        j = pstItem->m_lLen / 2 + pstItem->m_lLen % 2;            //    获取的字节
        ptob(s, j, o, pstItem->m_lLen + 1);
        o[pstItem->m_lLen] = 0x00;
        return j;
    case ITEM_FIXED_BCDPR:                  //  根据固定来读取取整字节，右对齐
        IsFixedSatify(cm, pstItem, *plen, pstItem->m_lLen);

        j = pstItem->m_lLen / 2 + pstItem->m_lLen % 2;            //    获取的字节
        ptob(s, j, szValue, TARGET_VALUE_LEN);
        memcpy(o, szValue + pstItem->m_lLen % 2, pstItem->m_lLen + 1);
        return j;
    case ITEM_ASCII_BCDPL:                  //  根据字符ASCII读取取整字节，左对齐
        lLen = asctol(s, pstItem->m_lLen);
        IsLenSatisfy(cm, pstItem, *plen, lLen);

        j = lLen / 2 + lLen % 2;            //    获取的字节
        ptob(s + pstItem->m_lLen, j, o, lLen + 1);
        o[lLen] = 0x00;
        return j + pstItem->m_lLen;
    case ITEM_ASCII_BCDPR:                  //  根据字符ASCII读取取整字节，右对齐
        lLen = asctol(s, pstItem->m_lLen);
        IsLenSatisfy(cm, pstItem, *plen, lLen);

        j = lLen / 2 + lLen % 2;            //    获取的字节
        ptob(s + pstItem->m_lLen, j, szValue, TARGET_VALUE_LEN);
        memcpy(o, szValue + lLen % 2, lLen + 1);
        return j + pstItem->m_lLen;
    case ITEM_ASCII_FIXED:                  //  根据字符ASCII读取固定字节
        lLen = asctol(s, pstItem->m_lLen);
        IsLenSatisfy(cm, pstItem, *plen, lLen);

        memcpy(o, s + pstItem->m_lLen, lLen);
        o[lLen] = 0x00;
        return lLen + pstItem->m_lLen;
    default:
        break;
    }

    return 0;
}

/**************************************************************************************************
    函 数 名：sGetInnerValue(CtsPack *pstItem, char *pszVal, long lSize)
    功能描述：获取组织报文的内部变量
    参数说明：
        pstItem                  --字段取值标签
        pszVal                   --取值首地址
        lSize                    --取值最大长度
    返回说明：
        pszVal                    --结果字符串
 **************************************************************************************************/
char*    sGetInnerValue(CtsPack *pstItem, char *pszVal, long *plLen, long lp, HHCMIE cm)
{
    if(!strcmp("__RET_CODE__", pstItem->m_szPlName))
        *plLen = strlen(strncpy(pszVal, pCmieGloble(cm)->m_szRetCode, *plLen));
    else if(!strcmp("__RET_MSG__", pstItem->m_szPlName))
        *plLen = strlen(strncpy(pszVal, pCmieGloble(cm)->m_szRetMsg, *plLen));
    else if(!strcmp("__MAP_CODE__", pstItem->m_szPlName))
		IBTgetpackbinidx(cm, 0, MAP_POOL_COD, pszVal, plLen, pstItem->m_szMark);
    else if(!strcmp("__MAP_MSG__", pstItem->m_szPlName))
		IBTgetpackbinidx(cm, 0, MAP_POOL_MSG, pszVal, plLen, pstItem->m_szMark);
    else if(!strcmp("__SY_TIME__", pstItem->m_szPlName))
    {    
        *plLen = MIN(*plLen, 6);
        memcpy(pszVal, sGetTime(), *plLen);
        pszVal[*plLen] = 0x00;
    }
    else if(!strcmp("__SY_DATE__", pstItem->m_szPlName))
    {
        *plLen = MIN(*plLen, 8);
        memcpy(pszVal, sGetDate(), *plLen);
        pszVal[*plLen] = 0x00;
    }
    else
    {
		IBTgetpackbinidx(cm, pstItem->m_lPlIdx + lp, pstItem->m_szPlName, pszVal, plLen, 
			pstItem->m_szMark);
        return pszVal;
    }

    return pszVal;
}

/**************************************************************************************************
    函 数 名：sSetSpecValue(CtsPack *pstItem, char *s, char *o, long *plen)
    功能描述：获取报文字段真实值
    参数说明：
        pstItem                 --配置信息
        o                        --结果返回字符串
        plen                    --字符串BUFFER长度
    返回说明：
        o                        --结果字符串
        *plen                    --整个字段所占用的长度
 **************************************************************************************************/
char*    sSetSpecValue(CtsPack *pstItem, char *o, long *plen, long lp, HHCMIE cm)
{
    long    lLen = 0, j;
    char    szLen[9], szValue[TARGET_VALUE_LEN];    //    如果有指示长度的化，最大也只有4位(1024)

    memset(szLen, 0, sizeof(szLen));
    switch(pstItem->m_lSLen)
    {
    case ITEM_LEN_FIXED:                    //    指定长度为固定
    case ITEM_ALL_REMAIN:                    //    指定长度为固定
        sGetInnerValue(pstItem, o, plen, lp, cm);
        break;
    case ITEM_BCD_FIXED:                    //    指定长度为BCD
        lLen = *plen - pstItem->m_lLen;
        sGetInnerValue(pstItem, o + pstItem->m_lLen, &lLen, lp, cm);
        *plen = pstItem->m_lLen + lLen;

        snprintf(szLen, 4, "%0*d", pstItem->m_lLen, lLen);
        memcpy(o, szLen, pstItem->m_lLen);
        break;
    case ITEM_BCDP_FIXED:                    //    根据压缩BCD来读取固定字节
        lLen = *plen - pstItem->m_lLen;
        sGetInnerValue(pstItem, o + pstItem->m_lLen, &lLen, lp, cm);
        *plen = pstItem->m_lLen + lLen;

        ltob(szLen, pstItem->m_lLen, lLen);
        memcpy(o, szLen, pstItem->m_lLen);
        break;
    case ITEM_BCDP_BCDPL:                    //    根据压缩BCD来读取取整字节，左对齐
        j = pstItem->m_lLen % 2 + pstItem->m_lLen / 2;        //    长度占用的字节
        lLen = *plen - j;                                    //    可用字节
        sGetInnerValue(pstItem, szValue, &lLen, lp, cm);
        szValue[lLen] = 0x00;
        btop(szValue, lLen, o + j, true);
        *plen = j + lLen / 2 + lLen % 2;                    //    该字段真正占用字节
        ltob(o, j, lLen);                                    //    压缩长度字节
        break;
    case ITEM_BCDP_BCDPR:                    //    根据压缩BCD来读取取整字节，右对齐
        j = pstItem->m_lLen % 2 + pstItem->m_lLen / 2;        //    长度占用的字节
        lLen = *plen - j;                                    //    可用字节
        sGetInnerValue(pstItem, szValue, &lLen, lp, cm); 
        szValue[lLen] = 0x00;
        btop(szValue, lLen, o + j, false);                    //    压缩报文字节
        *plen = j + lLen / 2 + lLen % 2;
        ltob(o, j, lLen);                                    //    压缩长度字节
        break;
    case ITEM_FIXED_BCDPL:                    //    根据固定来读取取整字节，左对齐
        sGetInnerValue(pstItem, szValue, plen, lp, cm);    //    plen不可能>TARGET_VALUE_LEN
        szValue[*plen] = 0x30;
        btop(szValue, *plen, o, true);
        j = *plen % 2 + *plen / 2;
        *plen = pstItem->m_lLen % 2 + pstItem->m_lLen / 2;        //    长度占用的字节
        if(j < *plen)                                        //    指定长度大于真是长度
            memset(o + j, 0, *plen - j);
        break;
    case ITEM_FIXED_BCDPR:                    //    根据固定来读取取整字节，右对齐
        sGetInnerValue(pstItem, szValue, plen, lp, cm);
        szValue[*plen] = 0x30;
        j = *plen % 2 + *plen / 2;
        lLen = pstItem->m_lLen % 2 + pstItem->m_lLen / 2;        //    长度占用的字节
        if(j < lLen)                                        //    指定长度大于真是长度
        {
            j = lLen - j;     
            memset(o, 0, j);
        }
        else
            j = 0;

        btop(szValue, *plen, o + j, false);
        *plen = lLen;
        break;
    case ITEM_ASCII_BCDPL:                    //    根据字符ASCII读取取整字节，左对齐
        lLen = *plen - pstItem->m_lLen;
        sGetInnerValue(pstItem, szValue, &lLen, lp, cm);
        szValue[lLen] = 0x30;
        *plen = pstItem->m_lLen + lLen / 2 + lLen % 2;

        btop(szValue, lLen, o + pstItem->m_lLen, true);
        ltoasc(o, pstItem->m_lLen, lLen);
        break;
    case ITEM_ASCII_BCDPR:                    //    根据字符ASCII读取取整字节，右对齐
        lLen = *plen - pstItem->m_lLen;
        sGetInnerValue(pstItem, szValue, &lLen, lp, cm);
        szValue[lLen] = 0x30;
        *plen = pstItem->m_lLen + lLen / 2 + lLen % 2;

        btop(szValue, lLen, o + pstItem->m_lLen, false);
        ltoasc(o, pstItem->m_lLen, lLen);
        break;
    case ITEM_ASCII_FIXED:                    //    根据字符ASCII读取固定字节
        lLen = *plen - pstItem->m_lLen;
        sGetInnerValue(pstItem, o + pstItem->m_lLen, &lLen, lp, cm);
        szValue[lLen] = 0x00;
        *plen = pstItem->m_lLen + lLen;
        ltoasc(o, pstItem->m_lLen, lLen);
        break;
    default:
		o[0] = 0x00;
        *plen = 0;
        break;
    }

    return o;
}

/**************************************************************************************************
    函 数 名：_vSetJSonValue(CtsPack *pv, char *v)
    功能描述：组织数据到json结构数组中
    参数说明：
        pv                --字段报文配置信息
        v                --待压入的值
    返回说明：
        无
 **************************************************************************************************/
void    _vSetJSonValue(cJSON *root, CtsPack *pv, char *v)
{
    switch(pv->m_lAttr)
    {
    case FILED_BINARY:
    case FILED_SENACT:
    case FILED_SENNAM:
    case FILED_SENTEL:
    case FILED_SENMAL:
    case FIELD_STRING:
        if(0x00 == v[0] && pv->m_lNilOn)
            return ;
        cJSON_AddStringToObject(root, pv->m_szLabel, v);
        break;
    case FIELD_NUMBER:
        cJSON_AddNumberToObject(root, pv->m_szLabel, atol(v));
        break;
    case FIELD_DOUBLE:
        cJSON_AddNumberToObject(root, pv->m_szLabel, atof(v));
        break;
    default:
        break;
    }

    return ;
}

/**************************************************************************************************
    函 数 名：_pBuildJson(SATvm *pstSavm, CtsPack *pstMeta, long lMeta)
    功能描述：组织Json子报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置列表
        lMeta                --报文解析配置列表个数
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
cJSON*    _pBuildJson(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, long lp, HHCMIE cm)
{
    register long    i = 0, j = 0, m = 0;
    long    lLen = 0, lLoop = 0;
    char    szValue[TARGET_VALUE_LEN];
    cJSON   *node = NULL, *root = NULL, *array = NULL, *temp = NULL;

    if(!pstMeta)    return NULL;

    if(NULL == (root = cJSON_CreateObject()))
    {
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Parse JSON failed");
        return NULL;
    }

    memset(szValue, 0, sizeof(szValue));
    for(i = 0; i < lMeta; i ++)
    {
        if(IsConstValue(pstMeta[i]))
        {
            _vSetJSonValue(root, &pstMeta[i], pstMeta[i].m_szConst);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            lLen = TARGET_VALUE_LEN;
            sSetSpecValue(&pstMeta[i], szValue, &lLen, lp, cm);
            if(0x00 == szValue[0] && pstMeta[i].m_lNilOn)
                continue ;
	
            _vSetJSonValue(root, &pstMeta[i], szValue);
            continue;    
        }

//        if(IsParent(pstMeta[i]))
        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_JSON))
        {
			IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            array = cJSON_CreateArray();
            for(j = 0; j < lLoop; j ++)        //  获取循环次数
            {
                if(NULL == (node = cJSON_CreateObject()))
                {
                    vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Parse JSON failed");
                    return NULL;
                }

                for(m = 1, lLen = TARGET_VALUE_LEN; m <= pstMeta[i].m_lChild; m ++, 
                    lLen = TARGET_VALUE_LEN)
                {
                    if(IsParent(pstMeta[i + m]))
                    {
                        if(IsLocalParent(pstMeta[i + m], PACK_ADAPTER_JSON))
                        {
                            if(NULL == (temp = _pBuildJson(pstSavm, &pstMeta[i + m + 1], 
                                pstMeta[i + m].m_lChild, lp + j, cm)))
                                return NULL; 
                                
                            cJSON_AddItemToObject(node, pstMeta[i + m].m_szLabel, temp);
                        }
                        else
                        {
                            _lBuildProto(pstSavm, &pstMeta[i + m], szValue, &lLen, lp + j, cm);
                            _vSetJSonValue(node, &pstMeta[i + m], szValue);
                        }

                        m += pstMeta[i + m].m_lChild;
                    }
                    else
                    {
                        sSetSpecValue(&pstMeta[i + m], szValue, &lLen, lp + j, cm);
            			if(0x00 == szValue[0] && pstMeta[i + m].m_lNilOn)
                			continue ;

                        _vSetJSonValue(node, &pstMeta[i + m], szValue);
                    }
                }

                cJSON_AddItemToArray(array, node);
            }
            cJSON_AddItemToObject(root, pstMeta[i].m_szLabel, array);
        }
        else
        {
            if(IsLoop(pstMeta[i]))    //    其他报文的循环
            {
				IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
                if(NULL == (node = cJSON_CreateObject()))
                {
                    vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Parse JSON failed");
                    return NULL;
                }

                for(j = 0, lLen = TARGET_VALUE_LEN; j < lLoop; j ++, lLen = TARGET_VALUE_LEN) 
                {
                    memset(szValue, 0, sizeof(szValue));
                    _lBuildProto(pstSavm, &pstMeta[i], szValue, &lLen, lp + j, cm);
                    _vSetJSonValue(node, &pstMeta[i], szValue);
                }

                cJSON_AddItemToObject(root, pstMeta[i].m_szLabel, node);
            }
            else
            {
                if(IsLocalParent(pstMeta[i], PACK_ADAPTER_JSON))
                {
                    if(NULL == (node = _pBuildJson(pstSavm, &pstMeta[i + 1], pstMeta[i].m_lChild, 
						lp, cm)))
                        return NULL;

                    cJSON_AddItemToObject(root, pstMeta[i].m_szLabel, node);
                }
                else
                {
                    lLen = TARGET_VALUE_LEN;
                    memset(szValue, 0, sizeof(szValue));
                    _lBuildProto(pstSavm, &pstMeta[i], szValue, &lLen, lp, cm);
                    _vSetJSonValue(root, &pstMeta[i], szValue);
                }
            }
        }

        i += pstMeta[i].m_lChild;
    }

    return root;
}

/**************************************************************************************************
    函 数 名：_vSetJSonValue(CtsPack *pv, char *v)
    功能描述：组织数据到json结构数组中
    参数说明：
        pv                --字段报文配置信息
        v                --待压入的值
    返回说明：
        无
 **************************************************************************************************/
void    _vSetXmlValue(xmlNodePtr xNode, CtsPack *pv, char *v)
{
    if(0x00 == v[0] && pv->m_lNilOn)
        return ;

    switch(pv->m_lAttr)
    {
    case FILED_SENACT:
    case FILED_SENNAM:
    case FILED_SENTEL:
    case FILED_SENMAL:
    case FILED_BINARY:
    case FIELD_DOUBLE:
    case FIELD_NUMBER:
    case FIELD_STRING:
        xmlNewTextChild(xNode, NULL, (xmlChar *)pv->m_szLabel, v);
        break;
    default:
        break;
    }

	return ;
}

/**************************************************************************************************
    函 数 名：_pBuildXml(SATvm *pstSavm, xmlNodePtr xRoot, CtsPack *pstMeta, long lMeta, long lp)
    功能描述：组织响应XML报文
    参数说明：
        pstSavm                --操作内存句柄
        xRoot                --XMLroot节点
        pstMeta                --报文解析配置列表
        pszOut                --待组织的报文内容
        plLen                --待组织的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
xmlNodePtr  _pBuildXml(SATvm *pstSavm, xmlNodePtr xRoot, CtsPack *pstMeta, long lMeta, long lp, 
				HHCMIE cm)
{
    register long    i = 0, j = 0, m = 0;
    long    lLen = 0, lLoop = 0;
    char    szValue[TARGET_VALUE_LEN];
    xmlNodePtr  xNode = NULL;

    if(!pstMeta || !xRoot)    return NULL;

    memset(szValue, 0, sizeof(szValue));
    for(i = 0; i < lMeta; i ++)
    {
        if(IsConstValue(pstMeta[i]))
        {
            _vSetXmlValue(xRoot, &pstMeta[i], pstMeta[i].m_szConst);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            lLen = TARGET_VALUE_LEN;
            sSetSpecValue(&pstMeta[i], szValue, &lLen, lp, cm);
            if(0x00 == szValue[0] && pstMeta[i].m_lNilOn)
                continue ;

            _vSetXmlValue(xRoot, &pstMeta[i], szValue);
            continue;    
        }

//        if(IsParent(pstMeta[i]))
        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_XML))
        {
			IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            for(j = 0; j < lLoop; j ++)        //  获取循环次数
            {
                if(!(xNode = xmlNewNode(NULL, (xmlChar *)pstMeta[i].m_szLabel)))
                {
                    vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "create xml failed");
                    return xRoot;
                }

                xmlAddChild(xRoot, xNode);
                for(m = 1, lLen = TARGET_VALUE_LEN; m <= pstMeta[i].m_lChild; m ++, lLen = TARGET_VALUE_LEN)
                {
                    if(IsParent(pstMeta[i + m]))
                    {
                        _lBuildProto(pstSavm, &pstMeta[i + m], szValue, &lLen, lp + j, cm);
                        _vSetXmlValue(xNode, &pstMeta[i + m], szValue);
                        m += pstMeta[i + m].m_lChild;
                    }
                    else
                    {
                        sSetSpecValue(&pstMeta[i + m], szValue, &lLen, lp + j, cm);
            			if(0x00 == szValue[0] && pstMeta[i + m].m_lNilOn)
                			continue ;

                        _vSetXmlValue(xNode, &pstMeta[i + m], szValue);
                    }
                }
            }
        }
        else
        {
            if(IsLoop(pstMeta[i]))    //    其他报文的循环
            {
				IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
                if(!(xNode = xmlNewNode(NULL, (xmlChar *)pstMeta[i].m_szLabel)))
                {
                    vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "create xml failed");
                    return xRoot;
                }

                xmlAddChild(xRoot, xNode);
                for(j = 0, lLen = TARGET_VALUE_LEN; j < lLoop; j ++, lLen = TARGET_VALUE_LEN)
                {
                    memset(szValue, 0, sizeof(szValue));
                    _lBuildProto(pstSavm, &pstMeta[i], szValue, &lLen, lp + j, cm);
                    _vSetXmlValue(xNode, &pstMeta[i], szValue);
                }
            }
            else
            {
                if(IsLocalParent(pstMeta[i], PACK_ADAPTER_XML))
                {
                    if(!(xNode = xmlNewNode(NULL, (xmlChar *)pstMeta[i].m_szLabel)))
                    {
                        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "create xml failed");
                        return xRoot;
                    }
                    xmlAddChild(xRoot, xNode);

                    if(NULL == _pBuildXml(pstSavm, xNode, &pstMeta[i + 1], pstMeta[i].m_lChild, lp, cm))
                        return xRoot;
                }
                else
                {
                    lLen = TARGET_VALUE_LEN;
                    memset(szValue, 0, sizeof(szValue));
                    _lBuildProto(pstSavm, &pstMeta[i], szValue, &lLen, lp, cm);
                    _vSetXmlValue(xNode, &pstMeta[i], szValue);
                }
            }
        }

        i += pstMeta[i].m_lChild;
    }

    return xRoot;
}

/**************************************************************************************************
    函 数 名：lBuildJson(SATvm *pstSavm, CtsPack *pstMeta, void *pszOut, long *plLen)
    功能描述：组织响应Json报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置列表
        pszOut                --待组织的报文内容
        plLen                --待组织的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lBuildJson(SATvm *pstSavm, CtsPack *pstMeta, char *pszOut, long *plLen, long lp, HHCMIE cm)
{
    long    lLen = 0;
    char    *p = NULL;
    cJSON   *root = NULL;

    if(!pstMeta)
    {
        *plLen = 0;
        return RC_SUCC;
    }

    if(NULL == (root = _pBuildJson(pstSavm, &pstMeta[1], pstMeta[0].m_lChild, lp, cm)))
    {
        cJSON_Delete(root);
        return RC_FAIL;
    }

    p = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    lLen = snprintf(pszOut, *plLen, "%s", p);    
    IBPFree(p);

    return lLen;
}

/**************************************************************************************************
    函 数 名：lBuildXml(SATvm *pstSavm, CtsPack *pstMeta, void *pszOut, long *plLen)
    功能描述：组织响应Json报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置列表
        pszOut                --待组织的报文内容
        plLen                --待组织的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lBuildXml(SATvm *pstSavm, CtsPack *pstMeta, char *pszOut, long *plLen, long lp, HHCMIE cm)
{
    int         nLen = 0;
    xmlChar     *p = NULL;
    xmlNodePtr  pxRoot = NULL;
    xmlDocPtr   pDoc = (xmlDocPtr)pGetDoc();

    if(!pstMeta)
    {
        *plLen = 0;
        return RC_SUCC;
    }
    
    if(pstMeta[0].m_lChild <= 0)
        return RC_SUCC;

    if(!IsParent(pstMeta[1]))
    {
        IBTerror(cm, "目前平台XML配置仅支持有且只有一个Root节点");
        return RC_FAIL;
    }

    pDoc    = xmlNewDoc(BAD_CAST"1.0");
    pxRoot  = xmlNewNode(NULL, (xmlChar *)pstMeta[1].m_szLabel);
    if(!pxRoot)
    {
        fprintf(stderr, "新建XML节点cts失败!\n");
        return ;
    }
    xmlDocSetRootElement(pDoc, pxRoot);

    if(NULL == _pBuildXml(pstSavm, pxRoot, &pstMeta[2], pstMeta[0].m_lChild - 1, lp, cm))
        return RC_FAIL;

    xmlKeepBlanksDefault(0);    //  去除缩进
    xmlDocDumpMemoryEnc(pDoc, &p, &nLen, "GBK");
    if(!p)
    {
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "create xml packet failed");
        return RC_FAIL;
    }

    nLen = MIN(*plLen, nLen);
    memcpy(pszOut, p, nLen);
    pszOut[nLen] = 0x00;
    xmlFree(p);

    return lTrimCRLF(pszOut, nLen);
}

/**************************************************************************************************
    函 数 名：_lBuildSepar(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *pszOut, long *plLen)
    功能描述：对报文中的子格式进行解析
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(父节点下级子节点开始)
        lMeta                --子报文配置个数
        pszOut                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lBuildSepar(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *pszOut, long *plLen, long lp, 
			HHCMIE cm)
{
    register long    i = 0, j = 0, m = 0;
    ulong    lLen = 0, lLoop = 0, lOfs = 0;
    char     szValue[TARGET_VALUE_LEN + 1];

    if(!pstMeta)    return lOfs;

    memset(szValue, 0, sizeof(szValue));
    for(i = 0; i < lMeta && (lOfs < *plLen); i ++)
    {
        if(IsConstValue(pstMeta[i]))
        {
            //    如果未设置解析到指定变量池也无需处理
            IBTmsg(cm, "节点:%2d->类型(%s)字段(%s)取常数(%s)", i, pstMeta[i].m_szAttr, 
                pstMeta[i].m_szLabel, pstMeta[i].m_szConst);
            lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s%s", pstMeta[i].m_szConst, 
                pstMeta[i].m_szLabel);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            if(0 == pstMeta[i].m_lPlSet)    continue; 

            lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
            sSetSpecValue(&pstMeta[i], szValue, &lLen, lp, cm);
            if(0x00 == szValue[0] && pstMeta[i].m_lNilOn)
                continue ;

            lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s%s", szValue, pstMeta[i].m_szLabel);
            continue;
        }

//        if(IsParent(pstMeta[i]))    //    父节点
        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_SEPAR))
        {
			IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            for(j = 0; j < lLoop; j ++)      //  获取循环次数
            {
                for(m = 1; m <= pstMeta[i].m_lChild; m ++)
                {
                    if(IsParent(pstMeta[i + m]))
                    {
                        lLen = *plLen - lOfs;
                        lOfs += _lBuildProto(pstSavm, &pstMeta[i + m], pszOut + lOfs, &lLen, lp + j, cm);
                        lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s", pstMeta[i + m].m_szLabel);
                        m += pstMeta[i + m].m_lChild;
                    }
                    else
                    {
                        lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
                        lOfs += _lBuildSepar(pstSavm, &pstMeta[i + m], 1, pszOut + lOfs, &lLen, lp + j, cm);
                    }
                }
            }
        }
        else
        {
            if(IsLoop(pstMeta[i]))
				IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            else
                lLoop = 1;
            
            for(j = 0; j < lLoop; j ++)      //  获取循环次数
            {
                lLen = *plLen - lOfs;
                lOfs += _lBuildProto(pstSavm, &pstMeta[i], pszOut + lOfs, &lLen, lp + j, cm);
                lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s", pstMeta[i].m_szLabel);
            }
        }

        i += pstMeta[i].m_lChild;
    }

    return lOfs;
}

/**************************************************************************************************
    函 数 名：_lBuildFrom(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *pszOut, long *plLen)
    功能描述：对报文中的子格式进行解析
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(父节点下级子节点开始)
        lMeta                --子报文配置个数
        pszOut                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lBuildFrom(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *pszOut, long *plLen, long lp, 
			HHCMIE cm)
{
    register long    i, j = 0, m = 0;
    ulong   lLen = 0, lLoop = 0, lOfs = 0;
    char    szValue[TARGET_VALUE_LEN + 1];

    if(!pstMeta)    return lOfs;

    memset(szValue, 0, sizeof(szValue));
    for(i = 0; i < lMeta && (lOfs < *plLen); i ++)
    {
        if(IsConstValue(pstMeta[i]))
        {
            //    如果未设置解析到指定变量池也无需处理
            IBTmsg(cm, "节点:%2d->类型(%s)字段(%s)取常数(%s)", i, pstMeta[i].m_szAttr, pstMeta[i].m_szLabel, 
                pstMeta[i].m_szConst);
            lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s=%s&", pstMeta[i].m_szLabel, 
                pstMeta[i].m_szConst);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            if(0 == pstMeta[i].m_lPlSet)    continue; 

            lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
			sSetSpecValue(&pstMeta[i], szValue, &lLen, lp, cm);
            if(0x00 == szValue[0] && pstMeta[i].m_lNilOn)
                continue ;
	
            IBTmsg(cm, "节点:%2d->类型(%s)字段(%s)取值(%s)", i, pstMeta[i].m_szAttr, pstMeta[i].m_szLabel, 
                szValue);
            lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s=%s&", pstMeta[i].m_szLabel, szValue);
            continue;
        }

//        if(IsParent(pstMeta[i]))    //    父节点
        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_FROM))
        {
			IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            for(j = 0; j < lLoop; j ++)      //  获取循环次数
            {
                for(m = 1; m <= pstMeta[i].m_lChild; m ++)
                {
                    if(IsParent(pstMeta[i + m]))
                    {
                        lLen = *plLen - lOfs;
                        lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s=", pstMeta[i + m].m_szLabel);
                        lOfs += _lBuildProto(pstSavm, &pstMeta[i + m], pszOut + lOfs, &lLen, lp + j, cm);
                        memcpy(pszOut + lOfs, "&", MIN(*plLen - lOfs, 1)); lOfs ++;
                        m += pstMeta[i + m].m_lChild;
                    }
                    else
                    {
                        lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
                        lOfs += _lBuildFrom(pstSavm, &pstMeta[i + m], 1, pszOut + lOfs, &lLen, lp + j, cm);
                    }
                }
            }
        }
        else
        {
            if(IsLoop(pstMeta[i]))
				IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            else
                lLoop = 1;
            
            for(j = 0; j < lLoop; j ++)      //  获取循环次数
            {
                lLen = *plLen - lOfs;
                lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s=", pstMeta[i + m].m_szLabel);
                lOfs += _lBuildProto(pstSavm, &pstMeta[i], pszOut + lOfs, &lLen, lp + j, cm);
                memcpy(pszOut + lOfs, "&", MIN(*plLen - lOfs, 1)); lOfs ++;
            }
        }

        i += pstMeta[i].m_lChild;
    }

    if(lOfs > 0)    pszOut[-- lOfs] = 0x00;

    return lOfs;
}

/**************************************************************************************************
    函 数 名：_lBuildFixed(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *pszOut, 
                long *plLen, long lp)
    功能描述：对报文中的子格式进行解析
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(父节点下级子节点开始)
        lMeta                --子报文配置个数
        pszOut                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lBuildFixed(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *pszOut, long *plLen, long lp,
			HHCMIE cm)
{
    char    szValue[TARGET_VALUE_LEN + 1];
    long    i, j, m;
    ulong    lOfs = 0, lLen, lLoop = 0;

    if(!pstMeta)    return lOfs;

    memset(szValue, 0, sizeof(szValue));
    for(i = 0, lOfs = 0; i < lMeta && (lOfs < *plLen); i ++)    
    {
        if(IsConstValue(pstMeta[i]))
        {
            //    如果未设置解析到指定变量池也无需处理
            IBTmsg(cm, "节点:%2d->类型(%s)字段(%s)取常数(%s)", pstMeta[i].m_lSeq, 
                pstMeta[i].m_szAttr, pstMeta[i].m_szLabel, pstMeta[i].m_szConst);
            lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s", pstMeta[i].m_szConst);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            if(0 == pstMeta[i].m_lPlSet)    continue; 

            lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
            sSetSpecValue(&pstMeta[i], pszOut + lOfs, &lLen, lp, cm);
            lOfs += lLen;
            continue;
        }

        //    父节点
//        if(IsParent(pstMeta[i]))
        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_FIXED))
        {
			IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            for(j = 0; j < lLoop; j ++)      //  获取循环次数
            {
                for(m = 1; m <= pstMeta[i].m_lChild; m ++)
                {
                    lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
                    if(IsParent(pstMeta[i + m]))
                    {
                        lOfs += _lBuildProto(pstSavm, &pstMeta[i + m], pszOut + lOfs, 
                            &lLen, lp + j, cm);
                        m += pstMeta[i + m].m_lChild;
                    }
                    else
                    {
                        lOfs += _lBuildFixed(pstSavm, &pstMeta[i + m], 1, pszOut + lOfs, 
                            &lLen, lp + j, cm);
                    }
                }
            }
        }
        else
        {
            if(IsLoop(pstMeta[i]))
				IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            else
                lLoop = 1;
            
            for(j = 0; j < lLoop; j ++)      //  获取循环次数
            {
                lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
                lOfs += _lBuildProto(pstSavm, &pstMeta[i], pszOut + lOfs, &lLen, lp + j, cm);
            }
        }

        i += pstMeta[i].m_lChild;
    }

    return lOfs;
}

/**************************************************************************************************
    函 数 名：lBuildSepar(SATvm *pstSavm, CtsPack *pstMeta, void *pszOut, long *plLen)
    功能描述：解析分隔符报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        pszOut                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lBuildSepar(SATvm *pstSavm, CtsPack *pstMeta, char *pszOut, long *plLen, long lp, HHCMIE cm)
{
    if(!pstMeta)    return RC_SUCC;

    return _lBuildSepar(pstSavm, &pstMeta[1], pstMeta[0].m_lChild, pszOut, plLen, lp, cm);
}

/**************************************************************************************************
    函 数 名：lBuildFrom(SATvm *pstSavm, CtsPack *pstMeta, void *pszOut, long *plLen)
    功能描述：组织http-form报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        pszOut                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lBuildFrom(SATvm *pstSavm, CtsPack *pstMeta, char *pszOut, long *plLen, long lp, HHCMIE cm)
{
    return _lBuildFrom(pstSavm, &pstMeta[1], pstMeta[0].m_lChild, pszOut, plLen, lp, cm);
}

/**************************************************************************************************
    函 数 名：lBuildFixed(SATvm *pstSavm, CtsPack *pstMeta, void *pszOut, long *plLen, long lp)
    功能描述：解析分隔符报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        pszOut                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lBuildFixed(SATvm *pstSavm, CtsPack *pstMeta, char *pszOut, long *plLen, long lp, HHCMIE cm)
{
    if(!pstMeta)    return RC_SUCC;

    return _lBuildFixed(pstSavm, &pstMeta[1], pstMeta[0].m_lChild, pszOut, plLen, lp, cm);
}

/**************************************************************************************************
    函 数 名：_vPutJSonValue(CtsPack *pv, cJson *node, long lp, HHCMIE cm)
    功能描述：对Json报文解析到变量池过程
    参数说明：
        pstItem             --字段报文配置信息
        pszValue            --待压入的值
        lp                  --变量池下标
    返回说明：
        无
 **************************************************************************************************/
void    _vPutJSonValue(CtsPack *pv, cJSON *node, long lp, HHCMIE cm)
{
    switch(pv->m_lAttr)
    {
    case FILED_BINARY:
        if(!node->valuestring)    //    NULL
			return ;
        IBTmsg(cm, "节点:%2d->类型(STRING)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            node->valuestring);
        IBTbin(cm, node->valuestring, strlen(node->valuestring));
		IBTputstringidx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, node->valuestring);
        break;
    case FIELD_STRING:
        if(!node->valuestring || (0x00 == node->valuestring[0] && pv->m_lNilOn))
			return ;
        IBTmsg(cm, "节点:%2d->类型(STRING)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            node->valuestring);
		IBTputstringidx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, node->valuestring);
        break;
    case FILED_SENACT:
        if(!node->valuestring || (0x00 == node->valuestring[0] && pv->m_lNilOn))
			return ;

        IBTmsg(cm, "节点:%2d->类型(SENACT)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            sDesensAct(node->valuestring));
		IBTputsenstridx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, node->valuestring, FILED_SENACT);
        break;
    case FILED_SENNAM:
        if(!node->valuestring || (0x00 == node->valuestring[0] && pv->m_lNilOn))
			return ;

        IBTmsg(cm, "节点:%2d->类型(SENNAM)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            sDesensName(node->valuestring));
		IBTputsenstridx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, node->valuestring, FILED_SENNAM);
        break;
    case FILED_SENTEL:
        if(!node->valuestring || (0x00 == node->valuestring[0] && pv->m_lNilOn))
			return ;

        IBTmsg(cm, "节点:%2d->类型(SENTEL)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            sDesensTel(node->valuestring));
		IBTputsenstridx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, node->valuestring, FILED_SENTEL);
        break;
    case FILED_SENMAL:
        if(!node->valuestring || (0x00 == node->valuestring[0] && pv->m_lNilOn))
			return ;

        IBTmsg(cm, "节点:%2d->类型(SENMAL)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            sDesensMail(node->valuestring));
		IBTputsenstridx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, node->valuestring, FILED_SENMAL);
        break;
    case FIELD_NUMBER:
        IBTmsg(cm, "节点:%2d->类型(NUMBER)标签(%s)取值(%d)", pv->m_lSeq, pv->m_szLabel, 
            node->valueint);
		IBTputlongidx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, node->valueint);
        break;
    case FIELD_DOUBLE:
        IBTmsg(cm, "节点:%2d->类型(DOUBLE)标签(%s)取值(%.6f)", pv->m_lSeq, pv->m_szLabel, 
            node->valuedouble);
		IBTputdoubleidx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, node->valuedouble, 6);
        break;
    default:
        break;    
    }

    return ;
}

/**************************************************************************************************
    函 数 名：_lParseJson(SATvm *pstSavm, cJSON *pRoot, CtsPack *pstMeta, long lMeta)
    功能描述：解析Json子报文
    参数说明：
        pstSavm                --操作内存句柄
        pRoot                --报文解析json根节点
        pstMeta                --报文解析配置列表
        lMeta                --报文解析配置列表个数
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lParseJson(SATvm *pstSavm, cJSON *pRoot, CtsPack *pstMeta, long lMeta, long lp, HHCMIE cm)
{
    register long    i = 0, j  = 0;
    long         lLen = 0, lLoop = 0;
    cJSON        *pChild = NULL, *node = NULL;

    for(i = 0; i < lMeta; i ++)
    {
        if(IsConstValue(pstMeta[i]))    //    为常数
        {
            //    如果未设置解析到指定变量池也无需处理
            IBTmsg(cm, "节点:%2d->标签(%s)类型(%s)取常数(%s)", pstMeta[i].m_lSeq, 
                pstMeta[i].m_szMark, pstMeta[i].m_szAttr, pstMeta[i].m_szConst);

			IBTputstringidx(cm, pstMeta[i].m_lPlIdx + lp, pstMeta[i].m_szPlName, pstMeta[i].m_szConst);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            if(NULL == (pChild = cJSON_GetObjectItem(pRoot, pstMeta[i].m_szLabel)))
                continue;
            _vPutJSonValue(&pstMeta[i], pChild, lp, cm);
            continue;
        }

//      if(IsParent(pstMeta[i]))    //    父节点
        if(NULL == (pChild = cJSON_GetObjectItem(pRoot, pstMeta[i].m_szLabel)))
            continue;

        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_JSON))
        {
            lLoop = cJSON_GetArraySize(pChild);
			IBTputlong(cm, pstMeta[i].m_szLoop, lLoop);
            for(j = 0; j < lLoop; j ++)
            {
                _lParseJson(pstSavm, cJSON_GetArrayItem(pChild, j), &pstMeta[i + 1], 
                    pstMeta[i].m_lChild, lp + j, cm);
            }
        }
        else
        {
            if(IsLoop(pstMeta[i]))
            {
                lLoop = cJSON_GetArraySize(pChild);
				IBTputlong(cm, pstMeta[i].m_szLoop, lLoop);
                for(j = 0; j < lLoop; j ++)
                {
                    if(!(node = cJSON_GetArrayItem(pChild, j)))    
                        continue;
                    if(!node->valuestring)    
                        continue;

                    lLen = strlen(node->valuestring);    
                    _lParseProto(pstSavm, &pstMeta[i], node->valuestring, &lLen, lp + j, cm);
                }
            }
            else
            {
                if(IsLocalParent(pstMeta[i], PACK_ADAPTER_JSON))
                {
                    _lParseJson(pstSavm, pChild, &pstMeta[i + 1], pstMeta[i].m_lChild, lp, cm);
                }
                else
                {
                    if(!pChild->valuestring)    //    只有string才可以去分配子格式报文
                    {
                        i += pstMeta[i].m_lChild;
                        continue;
                    }

                    lLen = strlen(pChild->valuestring);    
                    _lParseProto(pstSavm, &pstMeta[i], pChild->valuestring, &lLen, lp, cm);
                }
            }
        }

        i += pstMeta[i].m_lChild;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lParseSepar(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *psvData, long *plLen)
    功能描述：对报文中的子格式进行解析
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(父节点下级子节点开始)
        lMeta                --子报文配置个数
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lParseSepar(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *psvData, long *plLen, long lp, 
			HHCMIE cm)
{
    register    long    i = 0;
    long        lLen = 0;
    char        szValue[TARGET_VALUE_LEN + 1];

    for(i = 0; i < lMeta; i ++)        //    i 从1开始 剔除头节点
    {
        if(IsConstValue(pstMeta[i]))            //    为常数, 一般设置常数肯定存在变量池
        {
            IBTmsg(cm, "节点:%2d->字段(%s)类型(%s)取常数(%s)", pstMeta[i].m_lSeq, pstMeta[i].m_szMark,
                pstMeta[i].m_szAttr, pstMeta[i].m_szConst);
        
			IBTputstringidx(cm, pstMeta[i].m_lPlIdx + lp, pstMeta[i].m_szPlName, pstMeta[i].m_szConst);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            _vPutPoolValue(&pstMeta[i], sGetValueByIdx((char *)psvData, pstMeta[i].m_szLabel, 
                pstMeta[i].m_lPos), 0, lp, cm);
            continue;
        }

//        if(IsParent(pstMeta[i]))
        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_SEPAR))    //    循环节点将不解析
            continue;

        lLen = snprintf(szValue, TARGET_VALUE_LEN, "%s", 
            sGetValueByIdx((char *)psvData, pstMeta[i].m_szLabel, pstMeta[i].m_lPos));
        szValue[lLen] = 0x00;

        if(RC_SUCC != _lParseProto(pstSavm, &pstMeta[i], szValue, &lLen, lp, cm))
            return RC_FAIL;

        i += pstMeta[i].m_lChild;    //  有m_lChild个子节点已经解析, 直接跳过
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lParseFrom(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *psvData, long *plLen)
    功能描述：对报文中的子格式进行解析
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(父节点下级子节点开始)
        lMeta                --子报文配置个数
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lParseFrom(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *psvData, long *plLen, long lp, 
			HHCMIE cm)
{
    register    long    i = 0;
    long        lLen = 0;
    char        *p, szValue[TARGET_VALUE_LEN + 1];

    for(i = 0; i < lMeta; i ++)        //    i 从1开始 剔除头节点
    {
        if(IsConstValue(pstMeta[i]))            //    为常数, 一般设置常数肯定存在变量池
        {
            IBTmsg(cm, "节点:%2d->字段(%s)类型(%s)取常数(%s)", pstMeta[i].m_lSeq, pstMeta[i].m_szMark,
                pstMeta[i].m_szAttr, pstMeta[i].m_szConst);
        
			IBTputstringidx(cm, pstMeta[i].m_lPlIdx + lp, pstMeta[i].m_szPlName, pstMeta[i].m_szConst);
            continue;
        }

        lLen = snprintf(szValue, TARGET_VALUE_LEN, "%s=", pstMeta[i].m_szLabel);
        szValue[lLen] = 0x00;
        if(NULL == (p = strstr((char *)psvData, szValue)))
            continue;

        if(IsChildren(pstMeta[i]))
        {
//          _vPutPoolValue(&pstMeta[i], sGetValueByIdx(p + lLen, "&", 1), 0, lp);
			chrcpy(szValue, p + lLen, '&', sizeof(szValue));
            _vPutPoolValue(&pstMeta[i], szValue, 0, lp, cm);
            continue;
        }

        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_FROM))    //    循环节点将不解析
            continue;

        chrcpy(szValue, p + lLen, '&', TARGET_VALUE_LEN);
        if(RC_SUCC != _lParseProto(pstSavm, &pstMeta[i], szValue, &lLen, lp, cm))
            return RC_FAIL;

        i += pstMeta[i].m_lChild;    //  有m_lChild个子节点已经解析, 直接跳过
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lParseFixed(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *psvData, long *plLen)
    功能描述：对报文中的子格式进行解析
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(父节点下级子节点开始)
        lMeta                --子报文配置个数
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lParseFixed(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *psvData, long *plLen, long lp, 
			HHCMIE cm)
{
    register    long    i = 0, j = 0, m = 0;
    char     szValue[TARGET_VALUE_LEN + 1];
    ulong    lLen = 0, lOfs = 0, lLoop = 0;

    if(!pstMeta)    return RC_SUCC;

    memset(szValue, 0, sizeof(szValue));
    for(i = 0; i < lMeta; i ++)
    {
        if(IsConstValue(pstMeta[i]))            //    为常数, 一般设置常数肯定存在变量池
        {
            IBTmsg(cm, "节点:%2d->字段(%s)类型(%s)取常数(%s)", pstMeta[i].m_lSeq, pstMeta[i].m_szMark,
                pstMeta[i].m_szAttr, pstMeta[i].m_szConst);
        
			IBTputstringidx(cm, pstMeta[i].m_lPlIdx + lp, pstMeta[i].m_szPlName, pstMeta[i].m_szConst);
            continue;
        }

        IsParseNext(cm, lOfs, *plLen, pstMeta[i]);
        if(IsChildren(pstMeta[i]))
        {
            lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);    
            lOfs += lGetSpecValue(&pstMeta[i], psvData + lOfs, szValue, &lLen, cm);
            _vPutPoolValue(&pstMeta[i], szValue, lLen, lp, cm);
            continue;
        }
        
//        if(IsParent(pstMeta[i]))
        //    本报文模式循环
        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_FIXED))
        {
			IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            for(j = 0, lLen = TARGET_VALUE_LEN; j < lLoop; j ++)
            {
                for(m = 1; m <= pstMeta[i].m_lChild; m ++, lLen = TARGET_VALUE_LEN)
                {
                    if(IsParent(pstMeta[i + m]))
                    {
                        IsParseNext(cm, lOfs, *plLen, pstMeta[i + m]);
                        lOfs += lGetSpecValue(&pstMeta[i + m], psvData + lOfs, szValue, &lLen, cm);
                        _lParseProto(pstSavm, &pstMeta[i + m], szValue, &lLen, lp + j, cm);
                        m += pstMeta[i + m].m_lChild;
                    }
                    else
                    {
                        if(IsConstValue(pstMeta[i + m]))    //    为常数, 一般设置常数肯定存在变量池
                        {
                            IBTmsg(cm, "节点:%2d->字段(%s)类型(%s)取常数(%s)", pstMeta[i + m].m_lSeq, 
                                pstMeta[i + m].m_szMark, pstMeta[i + m].m_szAttr, pstMeta[i + m].m_szConst);
                        
							IBTputstringidx(cm, pstMeta[i + m].m_lPlIdx + lp + j, pstMeta[i + m].m_szPlName, 
								pstMeta[i + m].m_szConst);
                            continue;
                        }

                        IsParseNext(cm, lOfs, *plLen, pstMeta[i + m]);
                        lOfs += lGetSpecValue(&pstMeta[i + m], psvData + lOfs, szValue, &lLen, cm);
                        _vPutPoolValue(&pstMeta[i + m], szValue, lLen, lp + j, cm);
                    }
                }
            }
        }
        else
        {
            if(IsLoop(pstMeta[i]))
				IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            else
            {
                lLoop = 1;
                if(IsRemain(pstMeta[i]))
                {
                    lLen = *plLen - lOfs;
                    return _lParseProto(pstSavm, &pstMeta[i], psvData + lOfs, &lLen, lp, cm);
                }
            }

            for(j = 0; j < lLoop; j ++)      //  获取循环次数
            {
                lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);    
                lOfs += lGetSpecValue(&pstMeta[i], psvData + lOfs, szValue, &lLen, cm);
                if(RC_SUCC != _lParseProto(pstSavm, &pstMeta[i], szValue, &lLen, lp + j, cm))
                    return RC_FAIL;
            }

        }

        i += pstMeta[i].m_lChild;    //  有m_lChild个子节点已经解析, 直接跳过
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lParse8583(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *psvData, long *plLen, 
                char *pszBit, long lBit, long lp)
    功能描述：对报文中的子格式进行解析
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(父节点下级子节点开始)
        lMeta                --子报文配置个数
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
        pszBit                --位图
        lBit                --位图大小
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lParse8583(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *psvData, long *plLen, 
            char *pszBit, long lBit, long lp, HHCMIE cm)
{
    register    long    i = 0, j = 0;
    ulong    lMap, lOfs = 0, lNext = 0, lLen;
    char     szLen[5], szValue[TARGET_VALUE_LEN];
    uchar    ch = 0x00;

    for(i = 0, lMap = 1, ch = pszBit[i]; i < lBit; i ++, ch = pszBit[i])
    {
        for(j = 0; (j < 8) && (lOfs < *plLen); j ++, lMap ++, ch = ch << 1)
        {
            if(!(0x80 & ch))
                continue;

            //    为了提高效率，只能正向跳跃配置，不能反向
            for(lNext; lNext < lMeta; lNext ++)
            {
                if(IsConstValue(pstMeta[lNext]))
                {
                    //    如果未设置解析到指定变量池也无需处理
                    IBTmsg(cm, "节点:%2d->域(%d)类型(%s)取值(%s)", pstMeta[lNext].m_lSeq, 
                        pstMeta[lNext].m_lPos, pstMeta[lNext].m_szAttr, pstMeta[lNext].m_szConst);
            
					IBTputstringidx(cm, pstMeta[lNext].m_lPlIdx, pstMeta[lNext].m_szPlName, 
						pstMeta[lNext].m_szConst);
                }

                if(lMap == pstMeta[lNext].m_lPos)
                    break;
            }
            
            //    无记录
            if(lMeta <= lNext)                    return RC_SUCC;
            if(IsConstValue(pstMeta[lNext]))    continue;

            lLen = TARGET_VALUE_LEN;
            lOfs += lGetSpecValue(&pstMeta[lNext], psvData + lOfs, szValue, &lLen, cm);
            if(pstMeta[lNext].m_lIsParant > 0)
            {
                //    可以是其他类型的报文
                if(RC_SUCC != _lParseProto(pstSavm, &pstMeta[lNext], szValue, &lLen, 0, cm))
                    return RC_FAIL;
                continue;
            }
        
            if(0 == pstMeta[lNext].m_lPlSet)
                continue;

            //    指示长度类型FILED_BINARY 与正常变量类型矛盾        
            switch(pstMeta[lNext].m_lAttr)
            {
            case FILED_BINARY:
                IBTmsg(cm, "节点:%2d->域(%d)类型(STRING)取值如下:", pstMeta[lNext].m_lSeq, 
                    pstMeta[lNext].m_lPos);
                IBTbin(cm, szValue, lLen);
				IBTputbinaryidx(cm, pstMeta[lNext].m_lPlIdx + lp, pstMeta[lNext].m_szPlName, 
					szValue, lLen);
                break;
            case FIELD_STRING:
				if(0x00 == szValue[0] && pstMeta[lNext].m_lNilOn)
            		break ;
                IBTmsg(cm, "节点:%2d->域(%d)类型(STRING)取值(%s)", pstMeta[lNext].m_lSeq, 
                    pstMeta[lNext].m_lPos, szValue);
				IBTputbinaryidx(cm, pstMeta[lNext].m_lPlIdx + lp, pstMeta[lNext].m_szPlName, 
					szValue, lLen);
                break;
            case FIELD_NUMBER:
                IBTmsg(cm, "节点:%2d->域(%d)类型(NUMBER)取值(%s)", pstMeta[lNext].m_lSeq,
                    pstMeta[lNext].m_lPos, szValue);
				IBTputlongidx(cm, pstMeta[lNext].m_lPlIdx + lp, pstMeta[lNext].m_szPlName, 
					atol(szValue));
                break;
            case FIELD_DOUBLE:
                IBTmsg(cm, "节点:%2d->域(%d)类型(DOUBLE)取值(%s)", pstMeta[lNext].m_lSeq, 
                    pstMeta[lNext].m_lPos, szValue);
				IBTputdoubleidx(cm, pstMeta[lNext].m_lPlIdx + lp, pstMeta[lNext].m_szPlName, 
					atof(szValue), 6);
                break;
            case FILED_SENACT:
				if(0x00 == szValue[0] && pstMeta[lNext].m_lNilOn)
            		break ;
                IBTmsg(cm, "节点:%2d->域(%d)类型(SENACT)取值(%s)", pstMeta[lNext].m_lSeq, 
					pstMeta[lNext].m_lPos, sDesensAct(szValue));
				lPutstring(cm, pstMeta[lNext].m_lPlIdx + lp, lLen, pstMeta[lNext].m_szPlName, 
					szValue, FILED_SENACT);
                break;
            case FILED_SENNAM:
				if(0x00 == szValue[0] && pstMeta[lNext].m_lNilOn)
            		break ;
                IBTmsg(cm, "节点:%2d->域(%d)类型(SENNAM)取值(%s)", pstMeta[lNext].m_lSeq, 
					pstMeta[lNext].m_lPos, sDesensName(szValue));
				lPutstring(cm, pstMeta[lNext].m_lPlIdx + lp, lLen, pstMeta[lNext].m_szPlName, 
					szValue, FILED_SENNAM);
                break;
            case FILED_SENTEL:
				if(0x00 == szValue[0] && pstMeta[lNext].m_lNilOn)
            		break ;
                IBTmsg(cm, "节点:%2d->域(%d)类型(SENTEL)取值(%s)", pstMeta[lNext].m_lSeq, 
					pstMeta[lNext].m_lPos, sDesensTel(szValue));
				lPutstring(cm, pstMeta[lNext].m_lPlIdx + lp, lLen, pstMeta[lNext].m_szPlName, 
					szValue, FILED_SENTEL);
                break;
            case FILED_SENMAL:
				if(0x00 == szValue[0] && pstMeta[lNext].m_lNilOn)
            		break ;
                IBTmsg(cm, "节点:%2d->域(%d)类型(SENMAL)取值(%s)", pstMeta[lNext].m_lSeq, 
					pstMeta[lNext].m_lPos, sDesensMail(szValue));
				lPutstring(cm, pstMeta[lNext].m_lPlIdx + lp, lLen, pstMeta[lNext].m_szPlName, 
					szValue, FILED_SENMAL);
                break;
            default:
                break;    
            }
        }
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lBuild8583(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *pszOut, long *plLen)
    功能描述：对报文中的子格式进行解析
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(父节点下级子节点开始)
        lMeta                --子报文配置个数
        pszOut                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lBuild8583(SATvm *pstSavm, CtsPack *pstMeta, long lMeta, char *pszOut, long *plLen,
            char *pszBit, long lBit, long lp, HHCMIE cm)
{
    register    long    i = 0, j = 0;
    ulong    lMap, lOfs = 0, lLen, lLoop;
    char    szValue[TARGET_VALUE_LEN];
    uchar    ch = 0x00, szMap[129];

    if(sizeof(szMap) < lBit * 8)
    {
        IBTwarn(cm, "无法组织8583报文，位图设置过大, 平台不支持");
        return lOfs;
    }

    memset(szMap, 0, sizeof(szMap));
    for(i = 0; i < lMeta && (lOfs < *plLen); i ++)
    {
        if(IsConstValue(pstMeta[i]))
        {
            szMap[pstMeta[i].m_lPos] = 0x80;
            //    如果未设置解析到指定变量池也无需处理
            IBTmsg(cm, "节点:%2d->域(%d)类型(%s)取值(%s)", pstMeta[i].m_lSeq, 
                pstMeta[i].m_lPos, pstMeta[i].m_szAttr, pstMeta[i].m_szConst);    
            lOfs += snprintf(pszOut + lOfs, *plLen - lOfs, "%s", pstMeta[i].m_szConst);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            if(0 == pstMeta[i].m_lPlSet)    continue; 

            szMap[pstMeta[i].m_lPos] = 0x80;
            lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
            sSetSpecValue(&pstMeta[i], pszOut + lOfs, &lLen, lp, cm);
            lOfs += lLen;
            continue;
        }

//        if(IsParent(pstMeta[i]))
        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_8583))
        {
            i += pstMeta[i].m_lChild;
            continue;
        }

        if(IsLoop(pstMeta[i]))
			IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
        else
            lLoop = 1;
        
        szMap[pstMeta[i].m_lPos] = 0x80;
        for(j = 0; j < lLoop; j ++)      //  获取循环次数
        {
            lLen = MIN(*plLen - lOfs, TARGET_VALUE_LEN);
            lOfs += _lBuildProto(pstSavm, &pstMeta[i], pszOut + lOfs, &lLen, lp + j, cm);
        }

        i += pstMeta[i].m_lChild;
    }

    for(i = 0, lMap = 1; i < lBit; i ++)
    {
        for(j = 0; j < 8; j ++, lMap ++)
        {
            if(0x00 == szMap[lMap])
                continue;

            pszBit[i] = pszBit[i] | (szMap[lMap] >> j);
        }
    }

    return lOfs;
}

/**************************************************************************************************
    函 数 名：lParseJson(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen)
    功能描述：解析Json报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lParseJson(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, long lp, HHCMIE cm)
{
    cJSON    *root = NULL;

    if(!pstMeta)    return RC_SUCC;

    if(!(root = cJSON_Parse((char *)psvData)))
    {
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Parse JSON failed");
        return RC_FAIL;
    }

    if(RC_SUCC != _lParseJson(pstSavm, root, &pstMeta[1], pstMeta[0].m_lChild, lp, cm))
    {
        cJSON_Delete(root);
        return RC_FAIL;
    }

    cJSON_Delete(root);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：pGetXmlObject(xmlNodePtr xRoot, char *pszTarget)
    功能描述：获取指定节点的指针
    参数说明：
        xRoot             --字段报文配置信息
        pszTarget        --待压入的值
    返回说明：
        无
 **************************************************************************************************/
xmlNodePtr    pGetXmlObject(xmlNodePtr xRoot, char *pszTarget)
{
    xmlNodePtr  xChild = NULL;

    for(xChild = xRoot; xChild; xChild = xChild->next)
    {
        if(!xmlStrcmp(xChild->name, BAD_CAST(pszTarget)))
            return xChild;
    }

    return xChild;
}

/**************************************************************************************************
    函 数 名：_vPutXmlValue(CtsPack *pv, cJson *node, long lp)
    功能描述：对Json报文解析到变量池过程
    参数说明：
        pstItem             --字段报文配置信息
        pszValue            --待压入的值
        lp                  --变量池下标
    返回说明：
        无
 **************************************************************************************************/
void    _vPutXmlValue(CtsPack *pv, xmlNodePtr xNode, long lp, HHCMIE cm)
{
    xmlChar        *pszKey = NULL;

    if(NULL == (pszKey = xmlNodeGetContent(xNode)))
        return ;

    switch(pv->m_lAttr)
    {
    case FILED_BINARY:
        IBTmsg(cm, "节点:%2d->类型(BINARY)标签(%s)取值如下:", pv->m_lSeq, pv->m_szLabel);
        IBTbin(cm, pszKey, strlen(pszKey));
		IBTputstringidx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, pszKey);
        break;
    case FIELD_STRING:
        if(0x00 == pszKey[0] && pv->m_lNilOn)
            return ;
        IBTmsg(cm, "节点:%2d->类型(STRING)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, pszKey);
		IBTputstringidx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, pszKey);
        break;
    case FIELD_NUMBER:
        IBTmsg(cm, "节点:%2d->类型(NUMBER)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, pszKey);
		IBTputlongidx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, atol(pszKey));
        break;
    case FIELD_DOUBLE:
        IBTmsg(cm, "节点:%2d->类型(DOUBLE)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, pszKey);
		IBTputdoubleidx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, atof(pszKey), 6);
        break;
    case FILED_SENACT:
        if(0x00 == pszKey[0] && pv->m_lNilOn)
            return ;
        IBTmsg(cm, "节点:%2d->类型(SENACT)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            sDesensAct(pszKey));
		IBTputsenstridx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, pszKey, FILED_SENACT);
        break;
    case FILED_SENNAM:
        if(0x00 == pszKey[0] && pv->m_lNilOn)
            return ;
        IBTmsg(cm, "节点:%2d->类型(SENNAM)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            sDesensName(pszKey));
		IBTputsenstridx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, pszKey, FILED_SENNAM);
        break;
    case FILED_SENTEL:
        if(0x00 == pszKey[0] && pv->m_lNilOn)
            return ;
        IBTmsg(cm, "节点:%2d->类型(SENTEL)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            sDesensTel(pszKey));
		IBTputsenstridx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, pszKey, FILED_SENTEL);
        break;
    case FILED_SENMAL:
        if(0x00 == pszKey[0] && pv->m_lNilOn)
            return ;
        IBTmsg(cm, "节点:%2d->类型(SENMAL)标签(%s)取值(%s)", pv->m_lSeq, pv->m_szLabel, 
            sDesensMail(pszKey));
		IBTputsenstridx(cm, pv->m_lPlIdx + lp, pv->m_szPlName, pszKey, FILED_SENMAL);
        break;
    default:
        break;    
    }

    xmlFree(pszKey);

    return ;
}

/**************************************************************************************************
    函 数 名：_lParseXml(SATvm *pstSavm, xmlDocPtr pRoot, CtsPack *pstMeta, long lMeta)
    功能描述：解析Xml子报文
    参数说明：
        pstSavm                --操作内存句柄
        pRoot                --报文解析json根节点
        pstMeta                --报文解析配置列表
        lMeta                --报文解析配置列表个数
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lParseXml(SATvm *pstSavm, xmlNodePtr pRoot, CtsPack *pstMeta, long lMeta, long lp, 
			HHCMIE cm)
{
    register long    i = 0, j  = 0;
    long        lLen = 0, lLoop = 0;
    xmlNodePtr    xChild = NULL;
    xmlChar        *pszKey = NULL;

    for(i = 0; i < lMeta; i ++)
    {
        if(IsConstValue(pstMeta[i]))    //    为常数
        {
            //    如果未设置解析到指定变量池也无需处理
            IBTmsg(cm, "节点:%2d->标签(%s)类型(%s)取常数(%s)", pstMeta[i].m_lSeq, 
                pstMeta[i].m_szMark, pstMeta[i].m_szAttr, pstMeta[i].m_szConst);

			IBTputstringidx(cm, pstMeta[i].m_lPlIdx + lp, pstMeta[i].m_szPlName, pstMeta[i].m_szConst);
            continue;
        }

        if(IsChildren(pstMeta[i]))
        {
            if(NULL == (xChild = pGetXmlObject(pRoot, pstMeta[i].m_szLabel)))
                continue;
            _vPutXmlValue(&pstMeta[i], xChild, lp, cm);
            continue;
        }

//        if(IsParent(pstMeta[i]))    //    父节点
        if(NULL == (xChild = pGetXmlObject(pRoot, pstMeta[i].m_szLabel)))
            return ;

        if(IsLocalLoop(pstMeta[i], PACK_ADAPTER_XML))
        {
			IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
            for(j = 0; (j < lLoop) & (NULL != xChild); j ++, xChild = xChild->next)
                _lParseXml(pstSavm, xChild->children, &pstMeta[i + 1], pstMeta[i].m_lChild, lp + j, cm);
        }
        else
        {
            if(IsLoop(pstMeta[i]))
            {
				IBTgetdeflong(cm, pstMeta[i].m_szLoop, &lLoop, 0);
                for(j = 0; (j < lLoop) & (NULL != xChild); j ++, xChild = xChild->next)
                {
                    if(NULL == (pszKey = xmlNodeGetContent(xChild)))
                        break;

                    lLen = strlen(pszKey);    
                    _lParseProto(pstSavm, &pstMeta[i], pszKey, &lLen, lp + j, cm);
                    xmlFree(pszKey);
                }
            }
            else
            {
                if(IsLocalParent(pstMeta[i], PACK_ADAPTER_XML))
                {
                    _lParseXml(pstSavm, xChild->children, &pstMeta[i + 1], pstMeta[i].m_lChild, lp, cm);
                }
                else
                {
                    if(NULL == (pszKey = xmlNodeGetContent(xChild)))
                    {
                        i += pstMeta[i].m_lChild;
                        continue;
                    }

                    lLen = strlen(pszKey);
                    _lParseProto(pstSavm, &pstMeta[i], pszKey, &lLen, lp, cm);
                    xmlFree(pszKey);
                }
            }
        }

        i += pstMeta[i].m_lChild;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lParseXml(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen)
    功能描述：解析XML报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lParseXml(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, long lp, HHCMIE cm)
{
    long        lRet;
    xmlNodePtr  pNode;
    xmlDocPtr   pDoc = (xmlDocPtr)pGetDoc();

    if(!pstMeta)    return RC_SUCC;

    xmlKeepBlanksDefault(0);    //    去除缩进
    if(!(pDoc = xmlParseMemory(psvData, *plLen)))
//    if(!(pDoc = xmlReadMemory(psvData, *plLen, NULL, "GB2312", XML_PARSE_NOBLANKS)))
    {
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Parse xml packet failed");
        return RC_FAIL;
    }

    /*检查确认当前文档中包含内容*/
    if(!(pNode = xmlDocGetRootElement(pDoc))) 
    {
        xmlFreeDoc(pDoc);
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "xml Document is Null");
        return RC_FAIL;
    }

/*    可能不存在root节点也或者多个root节点
    if(xmlStrcmp(pNode->name, BAD_CAST(pstMeta[0].m_szLabel)))
    {
        xmlFreeDoc(pDoc);
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "xml root set Null");
        return RC_FAIL;
    }
*/

    lRet = _lParseXml(pstSavm, pNode, &pstMeta[1], pstMeta[0].m_lChild, lp, cm);
    xmlFreeDoc(pDoc);
    xmlMemoryDump();

    return lRet;
}

/**************************************************************************************************
    函 数 名：lParseFrom(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen)
    功能描述：解析http-form报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        psvData                --待解析的报文内容
        plLen                  --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lParseFrom(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, long lp, HHCMIE cm)
{
    return _lParseFrom(pstSavm, &pstMeta[1], pstMeta[0].m_lChild, psvData, plLen, lp, cm);
}

/**************************************************************************************************
    函 数 名：lParseSepar(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, HHCMIE cm)
    功能描述：解析分隔符报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lParseSepar(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, long lp, HHCMIE cm)
{
    if(!pstMeta)    return RC_SUCC;

    return _lParseSepar(pstSavm, &pstMeta[1], pstMeta[0].m_lChild, psvData, plLen, lp, cm);
}

/**************************************************************************************************
    函 数 名：lParseFixed(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen)
    功能描述：解析分隔符报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lParseFixed(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, long lp, HHCMIE cm)
{
    if(!pstMeta)    return RC_SUCC;

    return _lParseFixed(pstSavm, &pstMeta[1], pstMeta[0].m_lChild, psvData, plLen, lp, cm);
}

/**************************************************************************************************
    函 数 名：lParse8583(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, long lp)
    功能描述：解析分隔符报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lParse8583(SATvm *pstSavm, CtsPack *pstMeta, char *psvData, long *plLen, long lp, HHCMIE cm)
{
    char    szBit[17];
    long    lBit = sizeof(szBit);

    if(!pstMeta)    return RC_SUCC;

    IBTsysm(cm, "开始解析8583报文数据如下(%d):", *plLen);
    IBTbin(cm, psvData, *plLen);

    /*  第一位必须是位图位    */
    lGetSpecValue(&pstMeta[1], psvData, szBit, &lBit, cm);
    if(16 < lBit || lBit < 0)
    {
        IBTerror(cm, "当前系统不支持超过128位8583报文(%d)", lBit);
        return RC_FAIL;
    }
    *plLen -= lBit;

    if(pstMeta[0].m_lChild <= 1)
        return RC_SUCC;

    return  _lParse8583(pstSavm, &pstMeta[2], pstMeta[0].m_lChild - 1, psvData + lBit, plLen,
                szBit, lBit, lp, cm);
}

/**************************************************************************************************
    函 数 名：lBuild8583(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, long lp)
    功能描述：解析分隔符报文
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --报文解析配置
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lBuild8583(SATvm *pstSavm, CtsPack *pstMeta, void *psvData, long *plLen, long lp, HHCMIE cm)
{
    char    szBit[17];
    long    lLen = 0;

    if(!pstMeta)    return lLen;

    /*  第一位必须是位图位    */
    if(ITEM_LEN_FIXED != pstMeta[1].m_lSLen)
    {
        IBTwarn(cm, "位图配置错误, 必须明确固定长度");
        return lLen;
    }
    
    if(0 > pstMeta[1].m_lLen || pstMeta[1].m_lLen > 17)
    {
        IBTwarn(cm, "位图配置错误(%d)", pstMeta[1].m_lLen);    
        return lLen;
    }

    if(pstMeta[0].m_lChild <= 1)    return lLen;

    //    剩余长度
    *plLen -= pstMeta[1].m_lLen;
    memset(szBit, 0, sizeof(szBit));

    lLen = _lBuild8583(pstSavm, &pstMeta[2], pstMeta[0].m_lChild - 1, psvData + pstMeta[1].m_lLen, 
        plLen, szBit, pstMeta[1].m_lLen, lp, cm);
    if(RC_SUCC >= lLen)
        return 0;    //    解析失败0个长度

    IBTrace(cm, "位图长度(%d)字节，数据如下:", pstMeta[1].m_lLen);
    IBTbin(cm, szBit, pstMeta[1].m_lLen);

    memcpy(psvData, szBit, pstMeta[1].m_lLen);

    return lLen + pstMeta[1].m_lLen;
}

/**************************************************************************************************
    函 数 名：_lParseProto(SATvm *pstSavm, CtsPack *pstMeta, char *psvData, long *plLen)
    功能描述：解析报文模版
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(从父节点开始)
        lMeta                --子报文配置个数
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lParseProto(SATvm *pstSavm, CtsPack *pstMeta, char *psvData, long *plLen, long lp, HHCMIE cm)
{
    long    lRet;

    if(!pstMeta)    return RC_SUCC;

    switch(pstMeta[0].m_lType)
    {
    case PACK_ADAPTER_JSON:        //    JSON报文
        lRet = lParseJson(pstSavm, pstMeta, psvData, plLen, lp, cm);
        break;
    case PACK_ADAPTER_SEPAR:    //    分隔符报文
        lRet = lParseSepar(pstSavm, pstMeta, psvData, plLen, lp, cm);
        break;
    case PACK_ADAPTER_FIXED:
        lRet = lParseFixed(pstSavm, pstMeta, psvData, plLen, lp, cm);
        break;
    case PACK_ADAPTER_8583:
        lRet = lParse8583(pstSavm, pstMeta, psvData, plLen, lp, cm);
        break;
    case PACK_ADAPTER_XML:
        lRet = lParseXml(pstSavm, pstMeta, psvData, plLen, lp, cm);
        break;
    case PACK_ADAPTER_FROM:
        lRet = lParseFrom(pstSavm, pstMeta, psvData, plLen, lp, cm);
        break;
    case PACK_ADAPTER_TLV:
        break;
    default:
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Undefined packet format");
        return RC_FAIL;
    }

    return lRet;
}

/**************************************************************************************************
    函 数 名：_lBuildProto(SATvm *pstSavm, CtsPack *pstMeta, char *psvData, long *plLen, long lp)
    功能描述：解析报文模版
    参数说明：
        pstSavm                --操作内存句柄
        pstMeta                --子报文配置参数列表(从父节点开始)
        lMeta                --子报文配置个数
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        lLen                --返回组织的报文长度
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lBuildProto(SATvm *pstSavm, CtsPack *pstMeta, char *pszOut, long *plLen, long lp, HHCMIE root)
{
    long    lLen = 0;

    if(!pstMeta)    return RC_SUCC;

    switch(pstMeta[0].m_lType)
    {
    case PACK_ADAPTER_JSON:        //    JSON报文
        lLen = lBuildJson(pstSavm, pstMeta, pszOut, plLen, lp, root);
        break;
    case PACK_ADAPTER_SEPAR:    //    分隔符报文
        lLen = lBuildSepar(pstSavm, pstMeta, pszOut, plLen, lp, root);
        break;
    case PACK_ADAPTER_FIXED:
        lLen = lBuildFixed(pstSavm, pstMeta, pszOut, plLen, lp, root);
        break;
    case PACK_ADAPTER_8583:
        lLen = lBuild8583(pstSavm, pstMeta, pszOut, plLen, lp, root);
        break;
    case PACK_ADAPTER_XML:
        lLen = lBuildXml(pstSavm, pstMeta, pszOut, plLen, lp, root);
        break;
    case PACK_ADAPTER_FROM:
        lLen = lBuildFrom(pstSavm, pstMeta, pszOut, plLen, lp, root);
        break;
    case PACK_ADAPTER_TLV:
        break;
    default:
        *plLen = RC_FAIL;
        vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Undefined packet format");
        return RC_FAIL;
    }

    *plLen = lLen;
    return lLen;
}

/**************************************************************************************************
    函 数 名：_lCallUserFunc(char *pszFunc, char *pszOut, long *plLen)
    功能描述：调用用户自定义函数处理报文
    参数说明：
        pstSavm                --操作内存句柄
        lMid                --报文编号
        psvData                --待组织的报文内容
        plLen                --待组织的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    _lCallUserFunc(SATvm *pstSavm, char *pszFunc, char *pszOut, long *plLen)
{
    void    *pvHand = NULL;
    UFunc    pUser = NULL;
    char    szLib[512];
    char    *pError;

    memset(szLib, 0, sizeof(szLib));
    snprintf(szLib, sizeof(szLib), "%s/lib/libuser.so", getenv("HOME"));
    pvHand = dlopen(szLib, RTLD_LAZY);
    if(!pvHand)
    {
		vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, dlerror());
        return RC_FAIL;
    }

    pUser = (UFunc)dlsym(pvHand, pszFunc);
    if((pError = dlerror()) != NULL)
    {
		vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, dlerror());
        dlclose(pvHand);
        return RC_FAIL;
    }

    if(RC_SUCC != (*pUser)(pstSavm, pszOut, plLen))
    {
		vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "call userfunc error");
        dlclose(pvHand);
        return RC_FAIL;
    }

    dlclose(pvHand);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lCodeMapping(SATvm *pstSavm, CtsPack *pack)
    功能描述：响应码映射
    参数说明：
        无
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lCodeMapping(SATvm *pstSavm, CtsPack *pack, HHCMIE cm)
{
    CtsMapCode    stMCode;

    if(0 == pack->m_lIsConst)    //    存在映射
        return RC_SUCC;

    memset(&stMCode, 0, sizeof(CtsMapCode));
    if(1 == pack->m_lPlSet)
		IBTgetdefstring(cm, pack->m_szPlName, stMCode.m_szEntNo, "");
    else
        strncpy(stMCode.m_szEntNo, pack->m_szPlName, sizeof(stMCode.m_szEntNo));
    if(0x00 == stMCode.m_szEntNo[0])
    {
        IBTwarn(cm, "企业编号未配置，无法映射");
        return RC_SUCC;
    }

	IBTgetdefstring(cm, pack->m_szLabel, stMCode.m_szInCode, "");
	if(0x00 == stMCode.m_szInCode[0])
	{
		IBTputstring(cm, MAP_POOL_COD, pack->m_szConst);
		if(STATUS_ERR == pack->m_lNilOn)
		{
            vSetError(&cm->m_stCts, RET_SYSTEM, "%s", MSG_FAILURE);
			vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Mapped code failure");
			return RC_FAIL;
		}

		return RC_SUCC;
	}

    if(RC_SUCC != lGetMapTruck(pstSavm, &stMCode))
    {
        if(NO_DATA_FOUND == pstSavm->m_lErrno)
		{
			IBTputstring(cm, MAP_POOL_COD, pack->m_szConst);
		    if(STATUS_ERR == pack->m_lNilOn)
		    {
                vSetError(&cm->m_stCts, RET_SYSTEM, "%s", MSG_FAILURE);
				vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Mapped code failure");
			    return RC_FAIL;
		    }

			return RC_SUCC;
		}

        return RC_FAIL;
    }

	IBTputstring(cm, stMCode.m_szLabel, stMCode.m_szEntMsg);
	IBTputstring(cm, stMCode.m_szLcode, stMCode.m_szEntCode);
	if(STATUS_ERR == stMCode.m_lState)
    {
        vSetError(&cm->m_stCts, RET_SYSTEM, "%s", MSG_FAILURE);
		vRedeError(pstSavm->m_lErrno = RESOU_DISABLE, "Mapped code failure");
		return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lPackWrite(SATvm *pstSavm, long lMid, void *psvOut, long *plLen)
    功能描述：组织响应报文
    参数说明：
        pstSavm                --操作内存句柄
        lMid                --报文编号
        psvData                --待组织的报文内容
        plLen                --待组织的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lPackWriteAt(SATvm *pstSavm, HHCMIE cm, long lMid, void *psvOut, long *plLen)
{
    CtsPack    *pstRoot = (CtsPack *)pGetCache(lMid);

    if(!psvOut || !plLen || !pstRoot || !cm)
    {
        *plLen = 0;
        IBTwarn(cm, "响应报文(%d)未配置或报文缓存异常，不处理", lMid);
        return RC_SUCC;
    }

    lCodeMapping(pstSavm, pstRoot, cm);

    if(IsUserFunc(pstRoot))
        return _lCallUserFunc(pstSavm, pstRoot->m_szUserFunc, (char *)psvOut, plLen);
    else
    {
        if(RC_FAIL == _lBuildProto(pstSavm, pstRoot, (char *)psvOut, plLen, 0, cm))
            return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lPackReadAt(SATvm *pstSavm, long lMid, void *psvOut, long *plLen)
    功能描述：按照指定报文编号解析报文
    参数说明：
        pstSavm                --操作内存句柄
        lMid                --报文编号
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lPackReadAt(SATvm *pstSavm, HHCMIE cm, long lMid, void *psvData, long *plLen)
{
    CtsPack    *pstRoot = (CtsPack *)pGetCache(lMid);

    if(!psvData || !pstRoot || !cm)
    {
        IBTwarn(cm, "待解析报文数据或配置为空，不处理");
        return RC_SUCC;
    }

    if(IsUserFunc(pstRoot))
        return _lCallUserFunc(pstSavm, pstRoot->m_szUserFunc, (char *)psvData, plLen);
    else
    {
        if(RC_SUCC != _lParseProto(pstSavm, pstRoot, psvData, plLen, 0, cm))
            return RC_FAIL;
    }

    return lCodeMapping(pstSavm, pstRoot, cm);
}

/**************************************************************************************************
    函 数 名：lPackWrite(SATvm *pstSavm, long lMid, void *psvOut, long *plLen)
    功能描述：组织响应报文
    参数说明：
        pstSavm                --操作内存句柄
        lMid                --报文编号
        psvData                --待组织的报文内容
        plLen                --待组织的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lPackWrite(SATvm *pstSavm, long lMid, void *psvOut, long *plLen)
{
	return lPackWriteAt(pstSavm, pGetCmie(), lMid, psvOut, plLen);
}

/**************************************************************************************************
    函 数 名：lPackRead(SATvm *pstSavm, long lMid, void *psvOut, long *plLen)
    功能描述：按照指定报文编号解析报文
    参数说明：
        pstSavm                --操作内存句柄
        lMid                --报文编号
        psvData                --待解析的报文内容
        plLen                --待解析的报文长度
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lPackRead(SATvm *pstSavm, long lMid, void *psvData, long *plLen)
{
	return lPackReadAt(pstSavm, pGetCmie(), lMid, psvData, plLen);
}

/*************************************************************************************************
    code end
 *************************************************************************************************/
