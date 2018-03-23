#include    "tvm.h"


#define     DEBUG_HEAD_INFO     1
#define     DEBUG_UNIQ_IDEX     2
#define     DEBUG_GROP_IDEX     4
#define     DEBUG_GROP_LIST     8
#define     DEBUG_DATA_LIST     16  
#define     DEBUG_IDEX_TREE     32
#define     DEBUG_IDEX_DATA     64
#define     DEBUG_IDEX_DALL     128
#define     DEBUG_ALLD_INFO     (DEBUG_HEAD_INFO|DEBUG_UNIQ_IDEX|DEBUG_GROP_IDEX|DEBUG_GROP_LIST|DEBUG_DATA_LIST)

void    vDebugTree(void *pvData, SHTree *pstTree)
{

    if(!pvData || !pstTree) return ;

    fprintf(stderr, "DugTree:%08X-SePos:[%8d], Idx(%d):[%15s](%2d), Color[%d], lSePos:[%4d], lParent[%4d], left[%4d], right[%4d]\n" ,
        (void *)pstTree, (void *)pstTree - pvData,
        pstTree->m_lIdx,
        pstTree->m_szIdx,
        pstTree->m_lData,
        pstTree->m_eColor,
        pstTree->m_lSePos,
        pstTree->m_lParent,
        pstTree->m_lLeft,
        pstTree->m_lRight);

    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
        return ;

    if(NODE_NULL != pstTree->m_lLeft)
        vDebugTree(pvData, (SHTree *)pGetNode(pvData, pstTree->m_lLeft));

    if(NODE_NULL != pstTree->m_lRight)
        vDebugTree(pvData, (SHTree *)pGetNode(pvData, pstTree->m_lRight));
}



void    vDebugTable(TABLE t, long eType)
{
    long    i = 0, j = 0;
	RunTime	*pstRun = NULL;
    TblKey  *pstKey = NULL;
    SHTree  *pstTree = NULL;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    TIndex  *pstIndex = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(t)))
    {
        fprintf(stderr, "获取表(%d)链接Key失败, err:(%d)(%s)\n", t,lGetTErrno(), sGetTError(lGetTErrno()));
        return ;
    }

    if(NULL == (pstRun = pInitHitTest(pstSavm, pstSavm->tblName)))
    {
        fprintf(stderr, "初始化表(%d)链接失败, err:(%d)(%s)\n", t,lGetTErrno(), sGetTError(lGetTErrno()));
        return ;
    }

    if(eType & DEBUG_HEAD_INFO)
    {
        fprintf(stdout, "\n==========================================TABLE HEAND INFO============="
        "============================\n");
        fprintf(stdout, "TABLE:%d, NAME:%s\tSHTree(%d),SHList(%d),TblDef(%d)\n"
			"extern:%d, Group:%d, MaxRow:%d, Valid:%d, lNodeNil:%d, lIType:%d, "
			"Table:%d\nIdxLen:%d, TreePos:%d,  TreeRoot:%d, GrpLen:%d, GroupPos:%d, "
			"GroupRoot:%d\nListPos:%d, ListOfs:%d, Data:%d, ReSize:%d, Truck:%d\n", 
            ((TblDef *)pGetTblDef(t))->m_table, ((TblDef *)pGetTblDef(t))->m_szTable,
			sizeof(SHTree), sizeof(SHList), sizeof(TblDef), ((TblDef *)pGetTblDef(t))->m_lExtern, 
            ((TblDef *)pGetTblDef(t))->m_lGroup, ((TblDef *)pGetTblDef(t))->m_lMaxRow, 
            ((TblDef *)pGetTblDef(t))->m_lValid, ((TblDef *)pGetTblDef(t))->m_lNodeNil,
			((TblDef *)pGetTblDef(t))->m_lIType, 
            ((TblDef *)pGetTblDef(t))->m_lTable, ((TblDef *)pGetTblDef(t))->m_lIdxLen, 
            ((TblDef *)pGetTblDef(t))->m_lTreePos, ((TblDef *)pGetTblDef(t))->m_lTreeRoot,
            ((TblDef *)pGetTblDef(t))->m_lGrpLen, ((TblDef *)pGetTblDef(t))->m_lGroupPos, 
            ((TblDef *)pGetTblDef(t))->m_lGroupRoot, ((TblDef *)pGetTblDef(t))->m_lListPos,
            ((TblDef *)pGetTblDef(t))->m_lListOfs, ((TblDef *)pGetTblDef(t))->m_lData, 
            ((TblDef *)pGetTblDef(t))->m_lReSize, ((TblDef *)pGetTblDef(t))->m_lTruck);

        pstTree = &((TblDef *)pGetTblDef(t))->m_stNil;
        fprintf(stdout, ">>NODE_NULL POS:[%8d], Idx:[%s](%d)(%d), Color[%d], lSePos:[%4d], lParent[%4d]"
            ", left[%4d], right[%4d]\n" , (void *)pstTree - (void *)pGetTblDef(t), pstTree->m_szIdx, 
            pstTree->m_lIdx, pstTree->m_lData, pstTree->m_eColor, pstTree->m_lSePos, pstTree->m_lParent, 
            pstTree->m_lLeft, pstTree->m_lRight); 

        fprintf(stdout, "==========UNIQ INDEX FIELD=========\n");
        for(i = 0, pstKey = pGetTblIdx(t); i < lGetIdxNum(t); i ++)
        {
            fprintf(stdout, "From:%4d, len:%3d, attr:%d, IsPk:%d, field:%s\n", pstKey[i].m_lFrom,
                pstKey[i].m_lLen, pstKey[i].m_lAttr, pstKey[i].m_lIsPk, pstKey[i].m_szField);
        }

        fprintf(stdout, "==========GROUP INDEX FIELD========\n");
        for(i = 0, pstKey = pGetTblGrp(t); i < lGetGrpNum(t); i ++)
        {
            fprintf(stdout, "From:%4d, len:%3d, attr:%d, IsPk:%d, field:%s\n", pstKey[i].m_lFrom,
                pstKey[i].m_lLen, pstKey[i].m_lAttr, pstKey[i].m_lIsPk, pstKey[i].m_szField);
        }

        fprintf(stdout, "==================== TABLE FIELD ====================\n");
        for(i = 0, pstKey = pGetTblKey(t); i < lGetFldNum(t); i ++)
        {
            fprintf(stdout, "From:%4d, len:%3d, attr:%d, IsPk:%d, field:%s\n", pstKey[i].m_lFrom,
                pstKey[i].m_lLen, pstKey[i].m_lAttr, pstKey[i].m_lIsPk, pstKey[i].m_szField);
        }
    }                


    if(eType & DEBUG_UNIQ_IDEX)
    {
        fprintf(stdout, "\n===================================UNIQUE_INDEX====================="
            "==============lValid(%d)=========================\n", lGetTblValid(t));
		if(eType & DEBUG_IDEX_DALL)
		{
			for(i = 0; i < lGetTblRow(t); i ++)
			{
			    pstTree = (SHTree *)(pstRun->m_pvAddr + lGetIdxPos(t) + i * sizeof(SHTree));
				vPrintHex(pstTree->m_szIdx, pstTree->m_lIdx, 0);
			    fprintf(stdout, "NODE:[%d](%02d), Idx:[%4s](%d)(%2d), Color[%d], lSePos:[%4d], lParent[%4d], left[%4d], right[%4d]\n" ,
			        (void *)pstTree - pstRun->m_pvAddr, i, pstTree->m_szIdx, pstTree->m_lIdx, pstTree->m_lData, pstTree->m_eColor,
			        pstTree->m_lSePos, pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);
			}
		}
		else
		{
    		for(i = 0; i < lGetTblValid(t); i ++)
    		{
    		    pstTree = (SHTree *)(pstRun->m_pvAddr + lGetIdxPos(t) + i * sizeof(SHTree));
    		    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
    		        continue;

				vPrintHex(pstTree->m_szIdx, pstTree->m_lIdx, 0);
    		    fprintf(stdout, "NODE:[%6d]->(%02d), Idx:[%15s](%6d), Color[%d], lSePos:[%4d], lParent[%4d], left[%4d], right[%4d]\n" ,
    		        (void *)pstTree - pstRun->m_pvAddr, i, pstTree->m_szIdx, pstTree->m_lData, pstTree->m_eColor, pstTree->m_lSePos,
    		        pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);
    		}
		}
    }

    if(eType & DEBUG_GROP_IDEX)
    {
		fprintf(stdout, "\n===================================INDEX_GROUP====================="
		    "==============Valid:%d, Group:%d================\n", lGetTblValid(t), lGetTblGroup(t));
		if(eType & DEBUG_IDEX_DALL)
		{
			for(i = 0; i < lGetTblRow(t); i ++)
			{
			    pstTree = (SHTree *)(pstRun->m_pvAddr + lGetGrpPos(t) + i * sizeof(SHTree));
				vPrintHex(pstTree->m_szIdx, pstTree->m_lIdx, 0);
			    fprintf(stdout, "NODE:[%d](%02d), Idx:[%4s](%d)(%2d), Color[%d], lSePos:[%4d], lParent[%4d], left[%4d], right[%4d]\n" ,
			        (void *)pstTree - pstRun->m_pvAddr, i, pstTree->m_szIdx, pstTree->m_lIdx, pstTree->m_lData, pstTree->m_eColor,
			        pstTree->m_lSePos, pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);
			}
		}
		else
		{
			for(i = 0; i < lGetTblValid(t); i ++)
			{
			    pstTree = (SHTree *)(pstRun->m_pvAddr + lGetGrpPos(t) + i * sizeof(SHTree));
			    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
					continue;
			
				vPrintHex(pstTree->m_szIdx, pstTree->m_lIdx, 0);
			    fprintf(stdout, "NODE:[%d](%02d), Idx:[%4s](%d)(%2d), Color[%d], lSePos:[%4d], lParent[%4d], left[%4d], right[%4d]\n" ,
			        (void *)pstTree - pstRun->m_pvAddr, i, pstTree->m_szIdx, pstTree->m_lIdx, pstTree->m_lData, pstTree->m_eColor,
			        pstTree->m_lSePos, pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);
			}
		}
    }

    if(eType & DEBUG_GROP_LIST)
    {
        fprintf(stdout, "\n=================================INDEX_LIST========================"
            "==============Valid(%d)=============\n", lGetTblValid(t));
		if(eType & DEBUG_IDEX_DALL)
		{
			for(i = 0, j = lGetListOfs(t); i < lGetTblRow(t); i ++)
			{
			    pstList = (SHList *)(pstRun->m_pvAddr + j + i * sizeof(SHList));
			    fprintf(stdout, "LIST:[%8d][%02d], lSePos:[%4d], lData[%8d], Node[%8d], Next[%8d], Last[%8d]\n" ,
			        (void *)pstList - pstRun->m_pvAddr, i, pstList->m_lPos, pstList->m_lData, pstList->m_lNode,
			        pstList->m_lNext, pstList->m_lLast);
			}
		}
		else
		{
			for(i = 0, j = lGetListOfs(t); i < lGetTblValid(t); i ++)
			{
			    pstList = (SHList *)(pstRun->m_pvAddr + j + i * sizeof(SHList));
			    if(SELF_POS_UNUSE == pstList->m_lPos)
			        continue;
			
			    fprintf(stdout, "LIST:[%8d][%02d], lSePos:[%4d], lData[%8d], Node[%8d], Next[%8d], Last[%8d]\n" ,
			        (void *)pstList - pstRun->m_pvAddr, i, pstList->m_lPos, pstList->m_lData, pstList->m_lNode,
			        pstList->m_lNext, pstList->m_lLast);
			}
		}
    }

    if(eType & DEBUG_IDEX_TREE)
    {
        fprintf(stdout, "\n=================================TREE DEUBG========================"
            "==============Valid(%d)=============\n", lGetTblValid(t));
        vDebugTree(pstRun->m_pvAddr, pGetNode(pstRun->m_pvAddr, lGetIdxRoot(t)));
    }

	if(eType & DEBUG_IDEX_DATA)
	{
        fprintf(stdout, "\n===================================UNIQUE_INDEX====================="
            "==============lValid(%d)=========================\n", lGetTblValid(t));
        for(i = 0; i < lGetTblRow(t); i ++)
        {
            pstTruck = (void *)(pstRun->m_pvAddr + lGetTblData(t) + i * lGetRowTruck(t));
			fprintf(stdout, "SePos[%d]\n", (void *)pstTruck - pstRun->m_pvAddr);
			vPrintHex(pstTruck->m_pvData, lGetRowSize(t), 0);
        }
	}

    vTblDisconnect(pstSavm, pstSavm->tblName);
    fprintf(stdout, "=========================================================================="
        "===================================\n");
}

/**************************************************************************************************
 **************************************************************************************************/
void    vGetAction(char *s, long *plAction)
{
    int     i, nLen;

    if(!s)    return ;

    for(i = 0, nLen = strlen(s); i < nLen; i ++)
    {
        switch(s[i])
        {
        case 'h':
            *plAction |= DEBUG_HEAD_INFO;                
            break;
        case 'u':
            *plAction |= DEBUG_UNIQ_IDEX;                
            break;
        case 'g':
            *plAction |= DEBUG_GROP_IDEX;                
            break;
        case 'l':
            *plAction |= DEBUG_GROP_LIST;                
            break;
        case 'd':
            *plAction |= DEBUG_DATA_LIST;                
            break;
        case 't':
            *plAction |= DEBUG_IDEX_TREE;                
			break;
        case 'e':
            *plAction |= DEBUG_IDEX_DALL;                
			break;
        case 'a':
            *plAction |= DEBUG_IDEX_DATA;                
            break;
        default:
            break;
        }
    }
}

void    vPrintFunc(char *s)
{
    fprintf(stdout, "\nUsage:\t%s -[tpu][hugldtui]\n", s);
    fprintf(stdout, "\t-t\t\t--表序号\n");
    fprintf(stdout, "\t-p[hugldta]\t--debug表节点\n");
    fprintf(stdout, "\t-u(ui)\t\t--重置表读写锁\n");
    fprintf(stdout, "\n");
}

void	vDebugAction(TABLE t, int iAction, char *pszApp, char *pszParam)
{
	long	lRet, lDebug = 0;

	if(3 == iAction)
	{
		if(!strcmp(pszParam, "u"))
		{
			fprintf(stdout, "对表(%d)读写锁解锁\n", t);
			lRet = lResetLock(t, true);
		}
        else
		{
			fprintf(stdout, "重置表(%d)读写锁\n", t);
			lRet = lResetLock(t, true);
		}

		if(RC_SUCC != lRet)
		{
        	fprintf(stderr, "对表(%d)重置锁操作失败, err:(%d)(%s)\n", t,
            	lGetTErrno(), sGetTError(lGetTErrno()));
			return ;
		}
	}
	else if(5 == iAction)
	{
    	vGetAction(pszParam, &lDebug);
    	vDebugTable(t, lDebug);
	}
	else
		vPrintFunc(pszApp);

	return ;
}

/**************************************************************************************************
    函 数 名：main
    功能描述：cts操作功能说明
    返回说明：
        无
 **************************************************************************************************/
int     main(int argc, char *argv[])
{
    TABLE   t;
	char	szCom[256];
    int     iChoose = 0, iAction = 0;

	memset(szCom, 0, sizeof(szCom));
    while(-1 != (iChoose = getopt(argc, argv, "t:p:u::v?::")))
    {
        switch(iChoose)
        {
        case    't':
            iAction |= 1;
            t = atol(optarg);
            break;
		case	'u':
			if(!optarg)
				strcpy(szCom, "u");
			else
				strcpy(szCom, optarg);
            iAction |= 2;
			break;
        case    'p':
            iAction |= 4;
			strcpy(szCom, optarg);
            break;
        case    'v':
        case    '?':
        default:
			vPrintFunc(basename(argv[0]));
            return RC_FAIL;
        }
    }

	vDebugAction(t, iAction, basename(argv[0]), szCom);

    return RC_SUCC;
}
