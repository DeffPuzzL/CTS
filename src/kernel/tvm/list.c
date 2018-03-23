#include    "tmain.h"

/****************************************************************************************

 ****************************************************************************************/
/*************************************************************************************************
    函数说明：获取链表尾部节点
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        list                       --链表尾部节点
 *************************************************************************************************/
CMList* pGetCMTail(CMList *root)
{
    CMList *list = root;

    if(!list)    return NULL;

    while(list->pstNext)
        list = list->pstNext;

    return list;
}

/*************************************************************************************************
    函数说明：新增链表节点
    参数说明：
    返回值：
        root                    --链表root节点
 *************************************************************************************************/
CMList* pInsertList(CMList *root, void *pszData, long lSize)
{
    CMList *node = NULL, *tail = pGetCMTail(root);

    if(NULL == (node = (CMList *)malloc(sizeof(CMList))))
    {
        vSetTErrno(MALLC_MEM_ERR);
        return root;
    }

    node->m_lSize = lSize;
    if(NULL == (node->m_psvData = (char *)malloc(node->m_lSize)))
    {
        vSetTErrno(MALLC_MEM_ERR);
        return root;
    }

    node->pstNext = NULL;
    node->pstLast = NULL;
    memcpy(node->m_psvData, pszData, node->m_lSize);

    if(!root)
        root = node;
    else
    {
        node->pstLast = tail;
        tail->pstNext = node;            
    }

    return root;
}

/*************************************************************************************************
    函数说明：在链表中查找数据
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        root                    --链表root节点
 *************************************************************************************************/
CMList* pSearchNode(CMList *root, void *pv, long n)
{
    CMList *node;
    
    for(node = root; node; node = node->pstNext)
    {
        if(!memcmp(node->m_psvData, pv, n))
            return node;
    }

    return NULL;
}

/****************************************************************************************
    删除链表
 ****************************************************************************************/
void    vDestroyList(CMList *root)
{
    CMList    *node = root, *list = NULL;

    while(node)
    {
        list = node;
        node = node->pstNext;
        IBPFree(list->m_psvData);
        IBPFree(list);
    }

    root = NULL;
}

/*************************************************************************************************
    函数说明：删除链表节点
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        root                       --链表root节点
 *************************************************************************************************/
CMList*    pDeleteNode(CMList *root, void *psvData, long lSize)
{
    CMList    *node = root;

    while(node)
    {
        if(!memcmp(node->m_psvData, psvData, lSize))
            break;
        node = node->pstNext;
    }
    if(!node)   return root;

    if(node->pstNext)
        node->pstNext->pstLast = node->pstLast;

    if(!node->pstLast)
        root = node->pstNext;
    else
        node->pstLast->pstNext = node->pstNext;

    IBPFree(node->m_psvData);
    IBPFree(node);

    return root;
}

/*************************************************************************************************
    函数说明：链表节点个数
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        lCount                     --节点个数
 *************************************************************************************************/
long    lListNodeCount(CMList *root)
{
    long    lCount = 0;
    CMList  *node = root;

    while(node)
    {
        ++ lCount;
        node = node->pstNext;
    }

    return lCount;
}

/*************************************************************************************************
    函数说明：获取rowgrp组链表尾节点
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        node                       --链表root节点
 *************************************************************************************************/
Rowgrp* pRowgrpTail(Rowgrp *root)
{
    Rowgrp *node = root;

    if(!node)    return NULL;

    while(node->pstNext)
        node = node->pstNext;

    return node;
}

/*************************************************************************************************
    函数说明：删除rowgrp组链表
    参数说明：
        root                    --root节点
    返回值：
        无
 *************************************************************************************************/
void    vDeleteRowgrp(Rowgrp *node)
{
    Rowgrp *list = NULL;

    if(!node)    return ;

    list = node->pstNext;
    vDeleteRowgrp(node->pstNode);
    IBPFree(node->psvData);    
    IBPFree(node);    
    vDeleteRowgrp(list);

    return ;
}

/*************************************************************************************************
    函数说明：遍历rowgrp链表，找到则操作统计+1
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        node                       --存在记录统计个数
 *************************************************************************************************/
long    lCountRowgrp(Rowgrp *root, void *pv, long lLen)
{
    Rowgrp *list;

    for(list = root; list; list = list->pstNext)
    {
        if(!memcmp(list->psvData, pv, lLen))
            return list->lCount ++;
    }

    return 0;
}

/*************************************************************************************************
    函数说明：新增rowgrp链表节点
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        node                       --链表root节点
 *************************************************************************************************/
Rowgrp* pInsertRowgrp(SATvm *pstSavm, Rowgrp *root, Rowgrp *pstPres, void *pv, long n)
{
    Rowgrp *node = NULL, *tail = (Rowgrp *)pRowgrpTail(root);

    if(NULL == (node = (Rowgrp *)calloc(1, sizeof(Rowgrp))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return root;
    }

    if(NULL == (node->psvData = (char *)calloc(n + 1, sizeof(char))))
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return root;
    }

    node->lLen = n;
    node->lCount ++;
    node->pstPres = pstPres;
    memcpy(node->psvData, pv, node->lLen);

    if(!root)
    {
        root = node;
        tail = node;
        return root;
    }

    node->pstLast = tail;
    tail->pstNext = node;            
    tail = node;

    return root;
}

/*************************************************************************************************
    函数说明：归并排序获取链表中间节点
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        node                       --链表root节点
 *************************************************************************************************/
Rowgrp* pGetListMid(Rowgrp *root)
{
    Rowgrp *fast = NULL, *slow = NULL;

    //    无节点或者单个节点，无须排序
    if(root == NULL || root->pstNext == NULL)  
        return NULL;

    for(slow = root, fast = root->pstNext; fast; slow = slow->pstNext)
    {
        fast = fast->pstNext;  
        if(NULL == fast)
            break;
        fast = fast->pstNext;  
    }

    fast = slow->pstNext;  
    slow->pstNext = NULL; 
    return fast;
}

/*************************************************************************************************
    函数说明：以ASC排序链表
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        node                       --链表root节点
 *************************************************************************************************/
Rowgrp* pSortMergeAsc(Rowgrp *pa, Rowgrp *pb, TblKey *pstKey)  
{  
    Rowgrp *pstRes = NULL;  
  
    if(pa == NULL)  
        return pb;  
    else if(pb == NULL)  
        return pa;  
  
    switch(pstKey->m_lAttr)
    {
    case FIELD_DOUBLE:
        switch(pstKey->m_lLen)
        {
        case    4:
            if(*((float *)(pa->psvData)) < *((float *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeAsc(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeAsc(pa, pb->pstNext, pstKey);
            }
            break;
        case    8:
            if(*((double *)(pa->psvData)) < *((double *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeAsc(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeAsc(pa, pb->pstNext, pstKey);
            }
            break;
        default:
            break;
        }
        break;
    case FIELD_LONG:
        switch(pstKey->m_lLen)
        {
        case    2:
            if(*((sint *)(pa->psvData)) < *((sint *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeAsc(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeAsc(pa, pb->pstNext, pstKey);
            }
            break;
        case    4:
            if(*((int *)(pa->psvData)) < *((int *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeAsc(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeAsc(pa, pb->pstNext, pstKey);
            }
            break;
        case    8:
            if(*((llong *)(pa->psvData)) < *((llong *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeAsc(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeAsc(pa, pb->pstNext, pstKey);
            }
            break;
        default:
            break;
        }
        break;
    case FIELD_CHAR:
        if(0 < memcmp(pb->psvData, pa->psvData, pstKey->m_lLen))
        {
            pstRes = pa;  
            pstRes->pstNext = pSortMergeAsc(pa->pstNext, pb, pstKey);  
        }
        else
        {
            pstRes = pb;  
            pstRes->pstNext = pSortMergeAsc(pa, pb->pstNext, pstKey);
        }
        break;
    default:
        break;
    }

    if(pstRes->pstNext)    pstRes->pstNext->pstLast = pstRes;    

    return pstRes;  
}  

/*************************************************************************************************
    函数说明：以desc方式排序链表
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        node                       --链表root节点
 *************************************************************************************************/
Rowgrp* pSortMergeDes(Rowgrp *pa, Rowgrp *pb, TblKey *pstKey)  
{  
    Rowgrp *pstRes = NULL;  
  
    if(pa == NULL)  
        return pb;  
    else if(pb == NULL)  
        return pa;  
  
    switch(pstKey->m_lAttr)
    {
    case FIELD_DOUBLE:
        switch(pstKey->m_lLen)
        {
        case    4:
            if(*((float *)(pa->psvData)) > *((float *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeDes(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeDes(pa, pb->pstNext, pstKey);
            }
            break;
        case    8:
            if(*((double *)(pa->psvData)) > *((double *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeDes(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeDes(pa, pb->pstNext, pstKey);
            }
            break;
        default:
            break;
        }
        break;
    case FIELD_LONG:
        switch(pstKey->m_lLen)
        {
        case    2:
            if(*((sint *)(pa->psvData)) > *((sint *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeDes(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeDes(pa, pb->pstNext, pstKey);
            }
            break;
        case    4:
            if(*((int *)(pa->psvData)) > *((int *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeDes(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeDes(pa, pb->pstNext, pstKey);
            }
            break;
        case    8:
            if(*((llong *)(pa->psvData)) > *((llong *)(pb->psvData)))
            {
                pstRes = pa;  
                pstRes->pstNext = pSortMergeDes(pa->pstNext, pb, pstKey);  
            }
            else
            {
                pstRes = pb;  
                pstRes->pstNext = pSortMergeDes(pa, pb->pstNext, pstKey);
            }
            break;
        default:
            break;
        }
        break;
    case FIELD_CHAR:
        if(0 > memcmp(pb->psvData, pa->psvData, pstKey->m_lLen))
        {
            pstRes = pa;  
            pstRes->pstNext = pSortMergeDes(pa->pstNext, pb, pstKey);  
        }
        else
        {
            pstRes = pb;  
            pstRes->pstNext = pSortMergeDes(pa, pb->pstNext, pstKey);
        }
        break;
    default:
        break;
    }

    if(pstRes->pstNext)    pstRes->pstNext->pstLast = pstRes;    

    return pstRes;  
}  

/*************************************************************************************************
    函数说明：排序rowgrp组链表
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        node                       --链表root节点
 *************************************************************************************************/
void   vSortRowgrp(Rowgrp **pproot, TblKey *pstKey, Benum em)  
{  
    Rowgrp  *slow = NULL;  
  
    if(NULL == *pproot || NULL == (*pproot)->pstNext)  
        return;  
      
    slow = pGetListMid(*pproot);  
    vSortRowgrp(pproot, pstKey, em);
    vSortRowgrp(&slow, pstKey, em);

    if(ORDER_DESC & em)
        *pproot = pSortMergeDes(*pproot, slow, pstKey);
    else
        *pproot = pSortMergeAsc(*pproot, slow, pstKey);  
    return ;
}  

/*************************************************************************************************
    函数说明：排序用户所选记录行
    参数说明：
        s                          --源记录
        p                          --待比较记录
        psKey                      --比较字段
    返回值：
        TRUE                       --大于
        FALSE                      --不大于
 *************************************************************************************************/
BOOL    bCompare(void *s, void *p, TblKey *pstKey)
{
    switch(pstKey->m_lAttr)
    {
    case FIELD_DOUBLE:
        switch(pstKey->m_lLen)
        {
        case    4:
            if(*((float *)(s + pstKey->m_lFrom)) >= *((float *)(p + pstKey->m_lFrom)))
                return TRUE;
            else
                return FALSE;
        case    8:
            if(*((double *)(s + pstKey->m_lFrom)) >= *((double *)(p + pstKey->m_lFrom)))
                return TRUE;
            else
                return FALSE;
        default:
            return FALSE;
        }
        break;
    case FIELD_LONG:
        switch(pstKey->m_lLen)
        {
        case    2:
            if(*((sint *)(s + pstKey->m_lFrom)) >= *((sint *)(p + pstKey->m_lFrom)))
                return TRUE;
            else
                return FALSE;
        case    4:
            if(*((int *)(s + pstKey->m_lFrom)) >= *((int *)(p + pstKey->m_lFrom)))
                return TRUE;
            else
                return FALSE;
        case    8:
            if(*((llong *)(s + pstKey->m_lFrom)) >= *((llong *)(p + pstKey->m_lFrom)))
                return TRUE;
            else
                return FALSE;
        default:
            return FALSE;
        }
        break;
    case FIELD_CHAR:
        if(0 <= memcmp(s + pstKey->m_lFrom, p + pstKey->m_lFrom, pstKey->m_lLen))
            return TRUE;
        else
            return FALSE;
        break;
    default:
        break;
    }

    return FALSE;
}

/*************************************************************************************************
    函数说明：ASC排序用户所选记录行
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        无
 *************************************************************************************************/
void    vQsortRowAsc(void *pvData, long low, long high, long lTurck, TblKey *pstKey, void *pvKey)
{
    long  first = low, last = high;

    if(low >= high)
        return;

    memcpy(pvKey, pvData + first * lTurck, lTurck);
    while(first < last)
    {
        while(first < last && bCompare(pvData + last * lTurck, pvKey, pstKey))
            -- last;

        memcpy(pvData + first * lTurck, pvData + last * lTurck, lTurck);

        while(first < last && bCompare(pvKey, pvData + first * lTurck, pstKey))
            ++ first;
        memcpy(pvData + last * lTurck, pvData + first * lTurck, lTurck);
    }

    memcpy(pvData + first * lTurck, pvKey, lTurck);
    vQsortRowAsc(pvData, low, first - 1, lTurck, pstKey, pvKey);
    vQsortRowAsc(pvData, first + 1, high, lTurck, pstKey, pvKey);
}

/*************************************************************************************************
    函数说明：DESC排序用户所选记录行
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        无
 *************************************************************************************************/
void    vQsortRowDes(void *pvData, long low, long high, long lTurck, TblKey *pstKey, void *pvKey)
{
    long  first = low, last = high;

    if(low >= high)
        return;

    memcpy(pvKey, pvData + first * lTurck, lTurck);
    while(first < last)
    {
        while(first < last && bCompare(pvKey, pvData + last * lTurck, pstKey))
            -- last;
        memcpy(pvData + first * lTurck, pvData + last * lTurck, lTurck);

        while(first < last && bCompare(pvData + first * lTurck, pvKey, pstKey))
            ++ first;
        memcpy(pvData + last * lTurck, pvData + first * lTurck, lTurck);
    }

    memcpy(pvData + first * lTurck, pvKey, lTurck);
    vQsortRowDes(pvData, low, first - 1, lTurck, pstKey, pvKey);
    vQsortRowDes(pvData, first + 1, high, lTurck, pstKey, pvKey);

    return ;
}

/*************************************************************************************************
    函数说明：排序数据数组
    参数说明：
        pstSavm                    --操作句柄
        psvOut                     --查询的结果数据
    返回值：
        node                       --链表root节点
 *************************************************************************************************/
long    lSortRowList(SATvm *pstSavm, size_t lRow, void *pvData, TblKey *pstKey, long lTruck)
{
    void    *pvKey = (char *)malloc(lTruck);

    if(!pvKey)
    {
        pstSavm->m_lErrno = MALLC_MEM_ERR;
        return RC_FAIL;
    }

    if(ORDER_DESC & pstSavm->lReMatch)
        vQsortRowDes(pvData, 0, ((long )lRow) - 1, lTruck, pstKey, pvKey);
    else
        vQsortRowAsc(pvData, 0, ((long )lRow) - 1, lTruck, pstKey, pvKey);
    IBPFree(pvKey);

    return RC_SUCC;
}

/****************************************************************************************
    code end
 ****************************************************************************************/
