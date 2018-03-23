/**************************************************************************************************
    文 件 名：pool.c
    代码作者：DeffPuzzL、liujie
    编写版本：V3.0.0.0 
    创建日期：2016-07-14
    功能描述：cts变量池模块
 **************************************************************************************************/
#include    "face.h"

/****************************************************************************************
    变量池操作
 ****************************************************************************************/
/****************************************************************************************
    函 数 名：pGetPoolTail()
    功能描述：获取变量池的尾节点
    函数返回：
        pstPool                --返回尾节点地址
 ****************************************************************************************/
CtsPool*    pGetTail(CtsPool *roop)
{
    CtsPool    *pstTail = roop;

    if(!pstTail)    return NULL;

    for(; pstTail->pstNext; pstTail = pstTail->pstNext);

    return pstTail;
}

/****************************************************************************************
    函 数 名：_lGetPoolIdx(const char *pszTarg)
    功能描述：获取target新的下标
    函数返回：
        lIdx                --新的下标索引
 ****************************************************************************************/
long    _lGetPoolIdx(CtsPool *roop, const char *pszTarg, long lLen)
{
    long    lIdx = -1;
    CtsPool *pstPool = roop;

    for(; pstPool; pstPool = pstPool->pstNext)
    {
        if(!memcmp(pstPool->stVar.szTarget, pszTarg, lLen))
            lIdx = pstPool->stVar.lIdx > lIdx ? pstPool->stVar.lIdx : lIdx;
    }

    return ++ lIdx;
}

/****************************************************************************************
    函 数 名：_pPutPool(CtsPool *, long , long lLen, long lAttr, const char *, char *)
    功能描述：将target和idx的变量池存放到POOL中
    函数返回：
        pstPool                --新的POOL根节点
 ****************************************************************************************/
CtsPool*    _pPutPool(CtsPool **roop, long idx, char *value, long vlen, long attr, 
				const char *name, long nlen)
{
    CtsPool    *pstPool = NULL, *pstTail = (CtsPool *)pGetTail(*roop);

    pstPool = (CtsPool *)malloc(sizeof(CtsPool));
    if(!pstPool)    return NULL;

    pstPool->pstNext = NULL;
    pstPool->pstLast = NULL;
    pstPool->stVar.lAttr = attr;
    pstPool->stVar.lIdx = idx < 0 ? 0 : idx;
	nlen = MIN(nlen, TARGET_NAME_LEN - 1);
	memcpy(pstPool->stVar.szTarget, name, MIN(nlen, TARGET_NAME_LEN));
	pstPool->stVar.szTarget[nlen] = 0x00;

    pstPool->stVar.lLen = MIN(TARGET_VALUE_LEN - 1, vlen);
    memcpy(pstPool->szValue, value, pstPool->stVar.lLen);
    pstPool->szValue[pstPool->stVar.lLen]    = 0x00;

    if(NULL == *roop)
		*roop = pstPool;
    else
    {
        pstPool->pstLast = pstTail;
        pstTail->pstNext = pstPool;            
    }

    return *roop;
}

/****************************************************************************************
    函 数 名：_pGetPool(CtsPool *roop, const char *pszTarg, long lIdx)
    功能描述：获取POOL中对用target和idx的变量池
    函数返回：
        pstPool                --返回变量池节点
 ****************************************************************************************/
CtsPool*    _pGetPool(CtsPool *roop, const char *pszTarg, long lIdx)
{
    CtsPool    *pstPool = roop;

    while(pstPool)
    {
        if(PONLY_AUTO == lIdx)    //    不需要匹配下标
        {    
            if(!strcmp(pstPool->stVar.szTarget, pszTarg))
				return pstPool;
        }
        else
        {    
            if(!strcmp(pstPool->stVar.szTarget, pszTarg) && lIdx == pstPool->stVar.lIdx)
				return pstPool;
        }

        pstPool = pstPool->pstNext;
    }

    return NULL;
}

/****************************************************************************************
    函 数 名：pDeletePool(CtsPool **roop, char *pszTarg, long lIdx)
    功能描述：删除变量池的某一节点
    函数返回：
        返回根节点    
 ****************************************************************************************/
CtsPool*    pDeletePool(CtsPool **roop, const char *pszTarg, long lIdx)
{
    CtsPool    *pstPool = NULL;

    if(!(pstPool = _pGetPool(*roop, pszTarg, lIdx)))
        return NULL;

    if(pstPool->pstNext)
        pstPool->pstNext->pstLast = pstPool->pstLast;

    if(!pstPool->pstLast)
        *roop = pstPool->pstNext;
    else
        pstPool->pstLast->pstNext = pstPool->pstNext;
    IBPFree(pstPool);

    return *roop;
}

/****************************************************************************************
    函 数 名：lDeletePool(long lIdx, char pszTarg)
    功能描述：删除变量池的某一节点
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 ****************************************************************************************/
long    lDeletePool(HHCMIE cm, long lIdx, const char *pszTarg)
{
	if(!cm)	return RC_SUCC;

    if(PONLY_AUTO == lIdx)
    {
        while(pDeletePool(&cm->m_Roop, pszTarg, lIdx));
        return RC_SUCC;
    }

    pDeletePool(&cm->m_Roop, pszTarg, lIdx);

    return RC_SUCC;
}

/****************************************************************************************
    函 数 名：lPutdouble(long lIdx, long lTarg, char *pszTarg, dValue)
    功能描述： 向变量池中新增double类型的数据
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 ****************************************************************************************/
long    lPutdouble(HHCMIE cm, long lIdx, const char *pszTarg, double dValue, long lPercs) 
{
    char    szValue[32];
    long    lCopy, lTarg;
    CtsPool *pstPool = NULL;

    if(!cm || 0 == (lTarg = strlen(pszTarg)))
        return RC_FAIL;

    lCopy = snprintf(szValue, sizeof(szValue), "%.*f", lPercs, dValue);
    if(PONLY_AUTO == lIdx)
    {
        _pPutPool(&cm->m_Roop, _lGetPoolIdx(cm->m_Roop, pszTarg, lTarg), szValue, lCopy, 
			FIELD_DOUBLE, pszTarg, lTarg);
        return RC_SUCC;
    }

    if((pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx)))    //    找到该记录
    {
        pstPool->stVar.lAttr = FIELD_DOUBLE;
        pstPool->stVar.lLen = MIN((sizeof(pstPool->szValue) - 1), lCopy);
        strncpy(pstPool->szValue, szValue, pstPool->stVar.lLen);
        pstPool->szValue[pstPool->stVar.lLen] = 0x00;
        return RC_SUCC;
    }

    _pPutPool(&cm->m_Roop, lIdx, szValue, lCopy, FIELD_DOUBLE, pszTarg, lTarg);

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lPutlong(long lIdx, long lTarg, char *pszTarg, dValue)
    功能描述： 向变量池中新增long的数据
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 **************************************************************************************************/
long    lPutlong(HHCMIE cm, long lIdx, const char *pszTarg, long lValue) 
{
    char    szValue[32];
    long    lCopy, lTarg;
    CtsPool *pstPool = NULL;

    if(!cm || 0 == (lTarg = strlen(pszTarg)))
        return RC_FAIL;

    lCopy = snprintf(szValue, sizeof(szValue), "%d", lValue);
    if(PONLY_AUTO == lIdx)
    {
        _pPutPool(&cm->m_Roop, _lGetPoolIdx(cm->m_Roop, pszTarg, lTarg), szValue, lCopy, 
			FIELD_LONG, pszTarg, lTarg);
        return RC_SUCC;
    }

    if(pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx))    //    找到该记录
    {
        pstPool->stVar.lAttr = FIELD_LONG;
        pstPool->stVar.lLen  = MIN((sizeof(pstPool->szValue) - 1), lCopy);
        memcpy(pstPool->szValue, szValue, pstPool->stVar.lLen);
        pstPool->szValue[pstPool->stVar.lLen] = 0x00;
        return RC_SUCC;
    }

    _pPutPool(&cm->m_Roop, lIdx, szValue, lCopy, FIELD_LONG, pszTarg, lTarg);
    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lPutstring(long lIdx, long lLen, char *pszTarg, dValue)
    功能描述： 向变量池中新增string的数据
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 *************************************************************************************************/
long    lPutstring(HHCMIE cm, long lIdx, long lLen, const char *pszTarg, char *pszValue, long lAttr) 
{
    long    lTarg;
    CtsPool *pstPool = NULL;

    if(!cm || !pszValue || 0 == (lTarg = strlen(pszTarg)))
        return RC_FAIL;

    if(PONLY_AUTO == lIdx)
    {
        _pPutPool(&cm->m_Roop, _lGetPoolIdx(cm->m_Roop, pszTarg, lTarg), pszValue, lLen, lAttr, 
            pszTarg, lTarg);
        return RC_SUCC;
    }

    if(pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx))    //    找到该记录
    {
        pstPool->stVar.lAttr = lAttr;
        pstPool->stVar.lLen  = MIN((sizeof(pstPool->szValue) - 1), lLen);
        memcpy(pstPool->szValue, pszValue, pstPool->stVar.lLen);
        pstPool->szValue[pstPool->stVar.lLen] = 0x00;
        return RC_SUCC;
    }

    _pPutPool(&cm->m_Roop, lIdx, pszValue, lLen, lAttr, pszTarg, lTarg);

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lGetvalues(long lIdx, long lLen, char *pszTarg, dValue)
    功能描述： 向变量池获取对应变量的值
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 *************************************************************************************************/
long    lGetvalues(HHCMIE cm, long lIdx, long lAttr, const char *pszTarg, void *psvValue, 
			long lOut, const char *pszDesc)
{
    long    lResult;
    double  dResult;
    CtsPool *pstPool = NULL;

	if(!cm)	return RC_FAIL;

    if(!(pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx)))    //    找到该记录
    {
        SYSTrror(cm, RET_SYSTEM, "获取标签(%s)(%d)失败, %s", pszTarg, lIdx, pszDesc);
        return RC_NOTFOUND;
    }
    
    switch(lAttr)
    {
    case FIELD_LONG:
        lResult = atol(pstPool->szValue);
        memcpy(psvValue, &lResult, sizeof(long));
        break;
    case FIELD_DOUBLE:
        dResult = atof(pstPool->szValue);
        memcpy(psvValue, &dResult, sizeof(double));
        break;
    case FIELD_CHAR:
    case FILED_SENACT:
    case FILED_SENNAM:
    case FILED_SENTEL:
    case FILED_SENMAL:
        lOut = MIN(pstPool->stVar.lLen, lOut);
        memcpy(psvValue, (void *)pstPool->szValue, lOut);
        ((char *)psvValue)[lOut] = 0x00;
        break;
    default:
        break;
    }

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lGetBinvalue(long lIdx, long lLen, char *pszTarg, dValue)
    功能描述： 向变量池获取对应变量的值
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 *************************************************************************************************/
long    lGetBinvalue(HHCMIE cm, long lIdx, const char *pszTarg, void *psvValue, long *plOut, 
			const char *pszDesc)
{
    CtsPool    *pstPool = NULL;

	if(!cm)	return RC_FAIL;

    if(!(pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx)))    //    找到该记录
    {
        *plOut = 0;
        ((char *)psvValue)[*plOut] = 0x00;
        SYSTrror(cm, RET_SYSTEM, "获取标签(%s)(%d)失败, %s", pszTarg, lIdx, pszDesc);
        return RC_NOTFOUND;
    }
    
    *plOut = MIN(pstPool->stVar.lLen, *plOut);
    memcpy(psvValue, (void *)pstPool->szValue, *plOut);
    ((char *)psvValue)[*plOut] = 0x00;

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lGetPackBinval(long lIdx, long lLen, char *pszTarg, dValue)
    功能描述： 向变量池获取对应变量的值
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 *************************************************************************************************/
long    lGetPackBinval(HHCMIE cm, long lIdx, const char *pszTarg, void *psvValue, long *plOut, 
            const char *pszDesc)
{
    CtsPool    *pstPool = NULL;

	if(!cm)	return RC_FAIL;

    if(!(pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx))) 
    {
        *plOut = 0;
        ((char *)psvValue)[*plOut] = 0x00;
        IBPinfo("获取标签(%s)(%d)失败, %s", pszTarg, lIdx, pszDesc);
        return RC_NOTFOUND;
    }
    
    *plOut = MIN(pstPool->stVar.lLen, *plOut);
    memcpy(psvValue, (void *)pstPool->szValue, *plOut);
    ((char *)psvValue)[*plOut] = 0x00;

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lGetDefstring(long lIdx, const char *pszTarg, void *psvValue, long lOut, 
                char *pszDef)
    功能描述： 获取变量池指定标签的值，如果标签不存在那么获取用户指定的值
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 *************************************************************************************************/
long    lGetDefstring(HHCMIE cm, long lIdx, const char *pszTarg, void *psvValue, long lOut, 
			char *pszDef)
{
    CtsPool    *pstPool = NULL;

	if(!cm)	return RC_FAIL;

    if(!(pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx))) 
    {
        strncpy(psvValue, pszDef, lOut);
        return RC_SUCC;
    }
    
    lOut = MIN(pstPool->stVar.lLen, lOut);
    memcpy(psvValue, pstPool->szValue, lOut);
    ((char *)psvValue)[lOut] = 0x00;

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lGetDefdouble(long lIdx, const char *pszTarg, void *psvValue, long lOut, double dDef)
    功能描述： 获取变量池指定标签的值，如果标签不存在那么获取用户指定的值
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 *************************************************************************************************/
long    lGetDefdouble(HHCMIE cm, long lIdx, const char *pszTarg, void *psvValue, long lOut, 
			double dDef)
{
    double    dResult = 0.00;
    CtsPool   *pstPool = NULL;

	if(!cm)	return RC_FAIL;

    if(!(pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx))) 
    {
        memcpy(psvValue, &dDef, lOut);
        return RC_SUCC;
    }

    dResult = atof(pstPool->szValue);
    memcpy(psvValue, &dResult, sizeof(double));

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lGetDeflong(long lIdx, const char *pszTarg, void *psvValue, long lOut, long lDef)
    功能描述： 获取变量池指定标签的值，如果标签不存在那么获取用户指定的值
        lIdx <= -1 如果该标签在变量池找到，那么即使下标不匹配也会更新该字段值
                如未找到，新增下标为0的变量到变量池中
        lIdx >= 0  如果变量池中存在该标签那么也必须匹配下标，相同者更新，
                反之新增该下标对应的数据到变量池中
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 *************************************************************************************************/
long    lGetDeflong(HHCMIE cm, long lIdx, const char *pszTarg, void *psvValue, long lOut, 
			long lDef)
{
    long    lResult     = 0;
    CtsPool    *pstPool = NULL;

	if(!cm)	return RC_FAIL;

    if(!(pstPool = _pGetPool(cm->m_Roop, pszTarg, lIdx)))    //    找到该记录
    {
        memcpy(psvValue, &lDef, lOut);
        return RC_SUCC;
    }

    lResult = atof(pstPool->szValue);
    memcpy(psvValue, &lResult, lOut);

    return RC_SUCC;
}

/*************************************************************************************************
    函 数 名：lGetCountPool(const char *pszTarg)
    功能描述：获取变量池中标签个数
    函数返回：
        RC_SUCC                --处理成功
        RC_FAIL                --处理失败
 *************************************************************************************************/
long    lGetCountPool(HHCMIE cm, const char *pszTarg)
{
    long    lResult  = 0;
    CtsPool *pstPool = NULL;

	if(!cm)	return RC_FAIL;

    for(pstPool = cm->m_Roop; pstPool; pstPool = pstPool->pstNext)
    {
        if(!strcmp(pstPool->stVar.szTarget, pszTarg))
            lResult ++;    
    }

    return lResult;
}

/****************************************************************************************
    函 数 名：vResetZore()
    功能描述：清理变量池空值池
    函数返回：
        无
 ****************************************************************************************/
void    vResetZero(HHCMIE cm)
{
    CtsPool *pstPool = NULL, *pstDelete = NULL;

	if(!cm)	return ;

    for(pstPool = cm->m_Roop, pstDelete = cm->m_Roop; pstPool; pstDelete = pstPool)
    {
        if(!strncmp(pstPool->stVar.szTarget, CTS_POOL_TAR, 3) ||
            pstPool->stVar.lLen > 0)
        {
            pstPool = pstPool->pstNext;
            continue;
        }

        if(pstPool->pstNext)
            pstPool->pstNext->pstLast = pstPool->pstLast;

        if(!pstPool->pstLast)
            cm->m_Roop = pstPool->pstNext;
        else
            pstPool->pstLast->pstNext = pstPool->pstNext;

        pstPool = pstPool->pstNext;
        pstDelete->pstNext = NULL;
        pstDelete->pstLast = NULL;
        IBPFree(pstDelete);
    }
}

/****************************************************************************************
    函 数 名：vResetUser()
    功能描述：清理变量池中业务共享池
    函数返回：
        无
 ****************************************************************************************/
void    vResetUser(HHCMIE cm)
{
    CtsPool *pstPool = NULL, *pstDelete = NULL;

	if(!cm)	return ;

    for(pstPool = cm->m_Roop, pstDelete = cm->m_Roop; pstPool; pstDelete = pstPool)
    {
        if(!strncmp(pstPool->stVar.szTarget, CTS_POOL_TAR, 3) || 
            !strncmp(pstPool->stVar.szTarget, CTS_POOL_BAT, 4))
        {
            pstPool = pstPool->pstNext;
            continue;
        }

        if(pstPool->pstNext)
            pstPool->pstNext->pstLast = pstPool->pstLast;

        if(!pstPool->pstLast)
            cm->m_Roop = pstPool->pstNext;
        else
            pstPool->pstLast->pstNext = pstPool->pstNext;

        pstPool = pstPool->pstNext;
        pstDelete->pstNext = NULL;
        pstDelete->pstLast = NULL;
        IBPFree(pstDelete);
    }
}

/****************************************************************************************
    函 数 名：vCopyPool(CtsPool **root, CtsPool *node)
    功能描述：链表拷贝
    函数返回：
        无
 ****************************************************************************************/
void	vCopyPool(CtsPool **root, CtsPool *node)
{


}

/****************************************************************************************
    函 数 名：vCombinePool(CtsPool **roop, CtsPool *node)
    功能描述：组合非业务变量池到本交易中
    函数返回：
        无
 ****************************************************************************************/
void    vCombinePool(HHCMIE cm, CtsPool **node)
{
    CtsPool    *pstPool = *node, *pstDelete, *pstTail = (CtsPool *)pGetTail(cm->m_Roop);
	
	if(*node == cm->m_Roop)
	{
		cm->m_Roop = *node;
		return ;
	}

    while(pstPool)
    {
        if(!strncmp(pstPool->stVar.szTarget, CTS_POOL_TAR, 3) || 
            !strncmp(pstPool->stVar.szTarget, CTS_POOL_BAT, 4))
        {
            pstDelete = pstPool;
            pstPool   = pstPool->pstNext;
            IBPFree(pstDelete);    
            continue;
        }

        if((pstDelete = _pGetPool(cm->m_Roop, pstPool->stVar.szTarget, pstPool->stVar.lIdx)))
        {
            strncpy(pstDelete->szValue, pstPool->szValue, pstPool->stVar.lLen);
            pstDelete->szValue[pstPool->stVar.lLen] = 0x00;
            pstDelete = pstPool;
            pstPool   = pstPool->pstNext;
            IBPFree(pstDelete);    
            continue;
        }

        if(NULL == cm->m_Roop)
		{
			pstPool->pstLast = NULL;
			cm->m_Roop = pstPool;
		}
        else
        {
            pstPool->pstLast = pstTail;
            pstTail->pstNext = pstPool;            
        }

        pstTail = pstPool;
        pstPool = pstPool->pstNext;
        pstTail->pstNext = NULL;
    }

	*node = NULL;
}

/****************************************************************************************
    函 数 名：vResetPool()
    功能描述：清理变量池
    函数返回：
        无
 ****************************************************************************************/
void    vResetPool(HHCMIE cm)
{
    CtsPool    *pstPool, *pstList;

	if(!cm)	return ;

	pstPool = cm->m_Roop;
    while(pstPool)
    {
        pstList = pstPool;
        pstPool = pstPool->pstNext;

        pstList->pstNext = NULL;
        pstList->pstLast = NULL;
        IBPFree(pstList);
    }

	cm->m_Roop = NULL;
}

/****************************************************************************************
    函 数 名：vPrintPool()
    功能描述：打印当前变量池
    函数返回：
        无
 ****************************************************************************************/
void    vPrintPool(HHCMIE cm)
{
    CtsPool    *pstPool;

    if(!cm || LOG_TRACE < lGetSysLevel(cm))
        return ;

    IBTrace(cm, "当前变量池数据如下:");
    IBTpool(cm, "----------------------------------------------------------------------------"    
        "-----------------------");
    for(pstPool = cm->m_Roop; pstPool; pstPool = pstPool->pstNext)
    {
        switch(pstPool->stVar.lAttr)
        {
        case FIELD_NUMBER:
            IBTpool(cm, "N=%d, (NUMBER)(%2d), [%s](%d)=[%s]", lGetSeqNo(), 
                pstPool->stVar.lLen, pstPool->stVar.szTarget, pstPool->stVar.lIdx, 
                pstPool->szValue);
            break;
        case FIELD_DOUBLE:
            IBTpool(cm, "N=%d, (DOUBLE)(%2d), [%s](%d)=[%s]", lGetSeqNo(), 
                pstPool->stVar.lLen, pstPool->stVar.szTarget, pstPool->stVar.lIdx, 
                pstPool->szValue);
            break;
        case FIELD_STRING:
            IBTpool(cm, "N=%d, (STRING)(%2d), [%s](%d)=[%s]", lGetSeqNo(),
                pstPool->stVar.lLen, pstPool->stVar.szTarget, pstPool->stVar.lIdx, 
                pstPool->szValue);
            break;
        case FILED_SENACT:
            IBTpool(cm, "N=%d, (SENACT)(%2d), [%s](%d)=[%s]", lGetSeqNo(),
                pstPool->stVar.lLen, pstPool->stVar.szTarget, pstPool->stVar.lIdx, 
                sDesensAct(pstPool->szValue));
            break;
        case FILED_SENNAM:
            IBTpool(cm, "N=%d, (SENNAM)(%2d), [%s](%d)=[%s]", lGetSeqNo(),
                pstPool->stVar.lLen, pstPool->stVar.szTarget, pstPool->stVar.lIdx, 
                sDesensName(pstPool->szValue));
            break;
        case FILED_SENTEL:
            IBTpool(cm, "N=%d, (SENTEL)(%2d), [%s](%d)=[%s]", lGetSeqNo(),
                pstPool->stVar.lLen, pstPool->stVar.szTarget, pstPool->stVar.lIdx, 
                sDesensTel(pstPool->szValue));
            break;
        case FILED_SENMAL:
            IBTpool(cm, "N=%d, (SENMAL)(%2d), [%s](%d)=[%s]", lGetSeqNo(),
                pstPool->stVar.lLen, pstPool->stVar.szTarget, pstPool->stVar.lIdx, 
                sDesensMail(pstPool->szValue));
            break;
        default:
            IBPwarn("未知变量池类型");
            break;
        }
    }
    IBTpool(cm, "----------------------------------------------------------------------------"    
        "-----------------------");
}

/****************************************************************************************
    解析放入变量池中
 ****************************************************************************************/
long    lParsePool(HHCMIE cm, void *pvData, long lSize, long lCount)
{
    CtsPool stPool;
    long    lOfs = sizeof(CtsAttr), lNext = 0, lPos = 0;

	if(!cm)	return RC_FAIL;

    while((lOfs * (lNext + 1) + lPos) <= lSize)
    {
        memset(&stPool, 0, sizeof(stPool));
        memcpy(&stPool.stVar, pvData + lOfs * lNext + lPos, lOfs);
        if(!IBPIsvariable(stPool.stVar.lAttr) || lCount <= lNext)
            break;

        lNext ++;
        memcpy(stPool.szValue, pvData + lOfs * lNext + lPos, stPool.stVar.lLen);
        lPos += stPool.stVar.lLen;
        
        if(NULL == _pPutPool(&cm->m_Roop, stPool.stVar.lIdx, stPool.szValue, stPool.stVar.lLen, 
            stPool.stVar.lAttr, stPool.stVar.szTarget, strlen(stPool.stVar.szTarget)))
        {
            IBTerror(cm, "解析变量池失败");
            return RC_FAIL;
        }    
    }

    return RC_SUCC;
}

/****************************************************************************************
    组装放入共享池中
 ****************************************************************************************/
long    lBuildPool(HHCMIE cm, void *pvData, long lSize, long *plCount)
{
    CtsPool *pstPool;
    long    lOfs = sizeof(CtsAttr), lNext = 0, lPos = 0;

	if(!cm)	return RC_SUCC;

	pstPool = cm->m_Roop;

    while(pstPool)
    {
        if(lSize <= (lOfs * (lNext + 1) + lPos + pstPool->stVar.lLen))
        {
            IBTerror(cm, "变量池数据超限，请扩大系统参数(%d)", lNext + 1);
            return RC_FAIL;
        }

        memcpy(pvData + lOfs * lNext + lPos, &pstPool->stVar, lOfs);
        lNext ++;

        memcpy(pvData + lOfs * lNext + lPos, pstPool->szValue, pstPool->stVar.lLen);
        lPos += pstPool->stVar.lLen;
        pstPool = pstPool->pstNext;
    }
    memset(pvData + lOfs * lNext + lPos, 0, lSize - (lOfs * lNext + lPos));
    *plCount = lNext;

    return RC_SUCC;
}

/****************************************************************************************
    code end
 ****************************************************************************************/
