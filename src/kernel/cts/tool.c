/************************************************************************************************** 
    文 件 名：conf.c
    代码作者：DeffPuzzL
    编写版本：V3.0.0.0
    创建日期：2016-07-14
    功能描述：cts系统操作入口
 **************************************************************************************************/
#include    <curses.h>
#include    "face.h"

/**************************************************************************************************
    函 数 名：sGetToolVers()
    功能描述：返回cts系统版本
    返回说明：
        szVersion            --当前版本
 **************************************************************************************************/
static    char*    sGetToolVers()
{
    struct    utsname    sinf;
    static    char    szVersion[512];

    memset(szVersion, 0, sizeof(szVersion));
    sprintf(szVersion, "\nCompile %s %s on ", __DATE__, __TIME__);    
    if(RC_SUCC != uname(&sinf))
        strcat(szVersion, "Linux");
    else
        strcat(szVersion, sinf.sysname);

    if(8 == sizeof(long))
        strcat(szVersion, " x64 Runtime\n");
    else
        strcat(szVersion, " x32 Runtime\n");

    sprintf(szVersion + strlen(szVersion), "Release tool for cts Version V%s \n", CTS_VERSION);

    strcat(szVersion, "Author：DeffPuzzL\n");
    strcat(szVersion, "Mail：liuzhenhao@iboxpay.com\n");

    return szVersion;
}

/**************************************************************************************************
    函 数 名：vPrintFunc(char *s)
    功能描述：cts操作功能说明
    返回说明：
        无
 **************************************************************************************************/
void    vPrintFunc(char *s)
{
    fprintf(stdout, "\nUsage:\t%s -hcsoIMv\n", s);
    fprintf(stdout, "\t-h\t\t--导出容器配置信息\n");
    fprintf(stdout, "\t-c\t\t--导出组件配置信息\n");
//  fprintf(stdout, "\t-s\t\t--导出配置信息\n");
    fprintf(stdout, "\t-s\t\t--导出服务配置\n");
    fprintf(stdout, "\t-o\t\t--导出文件名\n");
    fprintf(stdout, "\t-i\t\t--导入配置信息\n");
//  fprintf(stdout, "\t-M\t\t--系统实时监控\n");
    fprintf(stdout, "\t-v\t\t--打印版本信息\n\n");
}

/**************************************************************************************************
    函 数 名：vActualMonitor()
    功能描述：显示系统实时监控
    返回说明：
        无
 **************************************************************************************************/
void    vActualMonitor()
{

}

/**************************************************************************************************
    函 数 名：vExportPack(SATvm *pstSavm, long lMid, FILE *fp)
    功能描述：导出报文配置信息
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
void    vExportPack(SATvm *pstSavm, long lMid, FILE *fp)
{
    size_t    i, lRow = 0;
    char      szBuffer[1024];
    CtsPack   *pstPack = NULL;

    if(lMid <= 0)    return ;

    if(RC_SUCC != lGetPackList(pstSavm, lMid, &pstPack, &lRow))
    {
        fprintf(stderr, "警告：报文(%d)导出异常, err:%s\n", lMid, sGetTError(pstSavm->m_lErrno));
        return ;
    }

    for(i = 0; i < lRow; i ++)
    {
        memset(szBuffer, 0, sizeof(szBuffer));
        lConvPackCfg(szBuffer, sizeof(szBuffer), &pstPack[i]);
        fprintf(fp, "%s", szBuffer);    
    }

    IBPFree(pstPack);
    return ;
}

/**************************************************************************************************
    函 数 名：lExportClass(long lBuss, char *pszFile)
    功能描述：导出组件配置信息(包括报文配置)
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lExportClass(long lBuss, char *pszFile)
{
    CtsClass    stClass;
    FILE        *fp = NULL;
    char        szBuffer[1024];
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    memset(szBuffer, 0, sizeof(szBuffer));
    if(NULL == (fp = fopen(pszFile, "w")))
    {
        fprintf(stderr, "打开文件(%s)失败, err:%s\n", pszFile, strerror(errno));
        return RC_FAIL;
    }

    if(RC_SUCC != lGetClassByIdx(pstSavm, &stClass, lBuss))
    {
        fclose(fp);    
        fprintf(stderr, "\n错误：获取组件(%d)信息失败, err:%s\n\n", lBuss, 
            sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }
    
    lConvClassCfg(szBuffer, sizeof(szBuffer), &stClass);
    fprintf(fp, "<Class>\n%s</Class>\n", szBuffer);

    //    导出报文配置
    fprintf(fp, "<Pack>\n");
    vExportPack(pstSavm, stClass.m_lInMid, fp);
    vExportPack(pstSavm, stClass.m_lOutMid, fp);
    vExportPack(pstSavm, stClass.m_lErrMid, fp);
    fprintf(fp, "</Pack>\n");

    fclose(fp);    
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lExportService(char *pszTrCode, char *pszFile)
    功能描述：导出服务配置信息（包括流程跟交易码映射）
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
long    lExportService(char *pszTrCode, char *pszFile, long *plSvrId)
{
    CtsService  stService;
    FILE        *fp = NULL;
    char        szBuffer[1024];
    CtsFlow     *pstFlow = NULL;
    size_t      i = 0, lOut = 0;
    CtsTrCode   stCode, *pstCode = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    memset(szBuffer, 0, sizeof(szBuffer));
    memset(&stCode, 0, sizeof(CtsTrCode));
    if(NULL == (fp = fopen(pszFile, "w")))
    {
        fprintf(stderr, "打开文件(%s)失败, err:%s\n", pszFile, strerror(errno));
        return ;
    }

    if(RC_SUCC != lGetService(pstSavm, &stService, pszTrCode))
    {
        fprintf(stderr, "\n错误：获取服务(%s)信息失败, err:%s\n\n", pszTrCode, 
            sGetTError(pstSavm->m_lErrno));
        goto ERROR_SERVICE;
    }

    lConvServiceCfg(szBuffer, sizeof(szBuffer), &stService);
    fprintf(fp, "<Service>\n%s</Service>\n", szBuffer);

    *plSvrId = stService.m_lSvrID;
    if(RC_SUCC != lGetSvrFlow(pstSavm, stService.m_lSvrID, &pstFlow, &lOut))
    {
        fprintf(stderr, "\n错误：获取服务(%s)(%d)信息失败, err:%s\n\n", pszTrCode, 
            stService.m_lSvrID, sGetTError(pstSavm->m_lErrno));
        goto ERROR_SERVICE;
    }

    fprintf(fp, "<Flow>\n");
    for(i = 0; i < lOut; i ++)
    {
        memset(szBuffer, 0, sizeof(szBuffer));
        lConvFlowCfg(szBuffer, sizeof(szBuffer), &pstFlow[i]);
        fprintf(fp, "%s", szBuffer);
    }
    fprintf(fp, "</Flow>\n");
    IBPFree(pstFlow);

    strncpy(stCode.m_szTrCode, pszTrCode, sizeof(stCode.m_szTrCode));

    pstSavm->pstVoid = &stCode;
    pstSavm->tblName = CTS_TRADE_CODE;
    pstSavm->lSize   = sizeof(CtsTrCode);
    pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
    if(RC_SUCC != lQuery(pstSavm, &lOut, (void **)&pstCode))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
        {
            fclose(fp);
            fprintf(stderr, "\n错误：获取服务(%s)映射信息失败, err:%s\n\n", 
                stCode.m_szTrCode, sGetTError(pstSavm->m_lErrno));
            return ;
        }
    }

    fprintf(fp, "<TrCode>\n");
    for(i = 0; i < lOut; i ++)
    {
        memset(szBuffer, 0, sizeof(szBuffer));
        lConvTrCodeCfg(szBuffer, sizeof(szBuffer), &pstCode[i]);
        fprintf(fp, "%s", szBuffer);
    }
    fprintf(fp, "</TrCode>\n");
    IBPFree(pstCode);

    fclose(fp);
    return RC_SUCC;

ERROR_SERVICE:
    fclose(fp);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：lDumpService(SATvm *pstSavm, CtsService *pstService, long lOut, FILE *fp)
    功能描述：导出容器下服务配置信息（包括流程跟交易码映射）
    返回说明：
        无
 **************************************************************************************************/
long    lDumpService(SATvm *pstSavm, CtsService *pstService, long lOut, FILE *fp)
{
    CtsHost     stHost;
    CtsClass    stClass;
    char        szBuffer[1024];
    CtsFlow     *pstFlow = NULL;
    size_t      i = 0, j, lRow = 0;
    CtsTrCode   stCode, *pstCode = NULL;
    CMList      *pstCRoot = NULL, *pstHRoot = NULL, *pstList = NULL;

    if(NULL == (pstHRoot = pInsertList(pstHRoot, (void *)&pstService->m_lHost, 
        sizeof(pstService->m_lHost))))
        return RC_FAIL;

    fprintf(fp, "<Flow>\n");
    for(i = 0; i < lOut; i ++)
    {
        if(RC_SUCC != lGetSvrFlow(pstSavm, pstService[i].m_lSvrID, &pstFlow, &lRow))
        {
            if(NO_DATA_FOUND == pstSavm->m_lErrno)
                continue;
            
            fprintf(stderr, "\n错误：获取服务(%d)信息失败, err:%s\n", pstService[i].m_lSvrID, 
                sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        for(j = 0; j < lRow; j ++)
        {
            memset(szBuffer, 0, sizeof(szBuffer));
            lConvFlowCfg(szBuffer, sizeof(szBuffer), &pstFlow[j]);
            fprintf(fp, "%s", szBuffer);

            if(CTS_FLOW_END == pstFlow[j].m_lFlow || CTS_FLOW_BEGIN == pstFlow[j].m_lFlow)
                continue;

            //    新增组件
            if(pSearchNode(pstCRoot, &pstFlow[j].m_lBuss, sizeof(pstFlow[j].m_lBuss)))
                continue;

            if(NULL == (pstCRoot = pInsertList(pstCRoot, (void *)&pstFlow[j].m_lBuss, 
                sizeof(pstFlow[j].m_lBuss))))
            {
                 vDestroyList(pstCRoot);
                 vDestroyList(pstHRoot);
                 return RC_FAIL;
            }

            //    新增容器
            if(pSearchNode(pstHRoot, (void *)&pstFlow[j].m_lHost, sizeof(pstFlow[j].m_lHost)))
                continue;

            if(NULL == (pstHRoot = pInsertList(pstHRoot, (void *)&pstFlow[j].m_lHost, 
                sizeof(pstFlow[j].m_lHost))))
            {
                vDestroyList(pstHRoot);
                vDestroyList(pstHRoot);
                return RC_FAIL;
            }
        }
        IBPFree(pstFlow);
    }
    fprintf(fp, "</Flow>\n");

    fprintf(fp, "<Class>\n");
    for(pstList = pstCRoot; pstList; pstList = pstList->pstNext)
    {
        if(RC_SUCC != lGetClassByIdx(pstSavm, &stClass, *(long *)pstList->m_psvData))
        {
            vDestroyList(pstCRoot);
            vDestroyList(pstHRoot);
            fprintf(stderr, "\n错误：获取组件(%d)信息失败, err:%s\n\n", 
                *(long *)pstList->m_psvData, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    
        memset(szBuffer, 0, sizeof(szBuffer));
        lConvClassCfg(szBuffer, sizeof(szBuffer), &stClass);
        fprintf(fp, "%s", szBuffer);
    }
    fprintf(fp, "</Class>\n");

    //    导出报文配置
    fprintf(fp, "<Pack>\n");
    for(pstList = pstCRoot; pstList; pstList = pstList->pstNext)
    {
        if(RC_SUCC != lGetClassByIdx(pstSavm, &stClass, *(long *)pstList->m_psvData))
        {
            vDestroyList(pstCRoot);
            vDestroyList(pstHRoot);
            fprintf(stderr, "\n错误：获取报文组件(%d)信息失败, err:%s\n\n", 
                *(long *)pstList->m_psvData, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    
        vExportPack(pstSavm, stClass.m_lInMid, fp);
        vExportPack(pstSavm, stClass.m_lOutMid, fp);
        vExportPack(pstSavm, stClass.m_lErrMid, fp);
    }
    fprintf(fp, "</Pack>\n");
    vDestroyList(pstCRoot);

    fprintf(fp, "<Host>\n");
    for(pstList = pstHRoot; pstList; pstList = pstList->pstNext)
    {
        if(RC_SUCC != lGetHost(pstSavm, &stHost, *(long *)pstList->m_psvData))
        {
            vDestroyList(pstHRoot);
            fprintf(stderr, "获取容器(%d)信息失败, err:%s\n", 
                *(long *)pstList->m_psvData, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }
    
        memset(szBuffer, 0, sizeof(szBuffer));
        lConvHostCfg(szBuffer, sizeof(szBuffer), &stHost);
        fprintf(fp, "%s", szBuffer);
    }
    fprintf(fp, "</Host>\n");
    vDestroyList(pstHRoot);

    fprintf(fp, "<TrCode>\n");
    for(i = 0; i < lOut; i ++)
    {
        memset(&stCode, 0, sizeof(CtsTrCode));
        strncpy(stCode.m_szTrCode, pstService[i].m_szTrCode, sizeof(stCode.m_szTrCode));

        pstSavm->pstVoid = &stCode;
        pstSavm->tblName = CTS_TRADE_CODE;
        pstSavm->lSize   = sizeof(CtsTrCode);
        pstSavm->lFind   = IDX_SELECT | NIL_IGNORE;
        if(RC_SUCC != lQuery(pstSavm, &lRow, (void **)&pstCode))
        {
            if(NO_DATA_FOUND == pstSavm->m_lErrno)
                continue;

            fprintf(stderr, "\n错误：获取服务(%s)映射信息失败, err:%s\n\n", 
                pstService[i].m_szTrCode, sGetTError(pstSavm->m_lErrno));
            return RC_FAIL;
        }

        for(j = 0; j < lRow; j ++)
        {
            memset(szBuffer, 0, sizeof(szBuffer));
            lConvTrCodeCfg(szBuffer, sizeof(szBuffer), &pstCode[j]);
            fprintf(fp, "%s", szBuffer);
        }
        IBPFree(pstCode);
    }
    fprintf(fp, "</TrCode>\n");

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vExportHost(long lHost, char *pszFile)
    功能描述：导出容器配置信息（包括流程跟交易码映射或组件报文）
    返回说明：
        RC_SUCC                --成功
        RC_FAIL                --失败
 **************************************************************************************************/
void    vExportHost(long lHost, char *pszFile)
{
    CtsHost     stHost;
    FILE        *fp = NULL;
    long        i = 0, lOut = 0;
    char        szBuffer[1024];
    CtsClass    *pstClass = NULL;
    CtsService  *pstService = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    if(NULL == (fp = fopen(pszFile, "w")))
    {
        fprintf(stderr, "打开文件(%s)失败, err:%s\n", pszFile, strerror(errno));
        return ;
    }

    if(RC_SUCC != lGetHost(pstSavm, &stHost, lHost))
    {
        fclose(fp);
        fprintf(stderr, "获取容器(%d)信息失败, err:%s\n", lHost, sGetTError(pstSavm->m_lErrno));
        return ;
    }

    if(HOST_TYPE_SERVICE == stHost.m_lEnum)
    {
        if(RC_SUCC != lGetHostService(pstSavm, lHost, &pstService, &lOut))
        {
            if(NO_DATA_FOUND != pstSavm->m_lErrno)
            {
                fclose(fp);
                fprintf(stderr, "获取容器(%d)下服务配置信息失败, err:%s\n", lHost, 
                    sGetTError(pstSavm->m_lErrno));
                return ;
            }
        }

        fprintf(fp, "<Service>\n");
        for(i = 0; i < lOut; i ++)
        {
            memset(szBuffer, 0, sizeof(szBuffer));
            lConvServiceCfg(szBuffer, sizeof(szBuffer), &pstService[i]);
            fprintf(fp, "%s", szBuffer);
        }
        fprintf(fp, "</Service>\n");

        if(RC_SUCC != lDumpService(pstSavm, pstService, lOut, fp))
		{
            IBPFree(pstService);
            fprintf(stdout, "\n导出容器(%d)配置失败!!!\n", lHost);
            return ;
		}	
       	IBPFree(pstService);
    }
    else
    {
        memset(szBuffer, 0, sizeof(szBuffer));
        lConvHostCfg(szBuffer, sizeof(szBuffer), &stHost);
        fprintf(fp, "<Host>\n%s</Host>\n", szBuffer);

        if(RC_SUCC != lGetHostClass(pstSavm, lHost, &pstClass, &lOut))
        {
            fclose(fp);
            fprintf(stderr, "获取容器(%d)组件配置信息失败, err:%s\n", lHost, 
                sGetTError(pstSavm->m_lErrno));
            return ;
        }

        fprintf(fp, "<Class>\n");
        for(i = 0; i < lOut; i ++)
        {
            memset(szBuffer, 0, sizeof(szBuffer));
            lConvClassCfg(szBuffer, sizeof(szBuffer), &pstClass[i]);
            fprintf(fp, "%s", szBuffer);
        }
        fprintf(fp, "</Class>\n");

        fprintf(fp, "<Pack>\n");
        for(i = 0; i < lOut; i ++)
        {
            vExportPack(pstSavm, pstClass[i].m_lInMid, fp);
            vExportPack(pstSavm, pstClass[i].m_lOutMid, fp);
            vExportPack(pstSavm, pstClass[i].m_lErrMid, fp);
        }
        fprintf(fp, "</Pack>\n");
        IBPFree(pstClass);
    }

    fclose(fp);
    fprintf(stdout, "\n导出容器(%d)配置成功, completed successfully!!!\n\n", lHost);

    return ;
}

/**************************************************************************************************
    函 数 名：vGetHostList(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
    功能描述：从流程列表中获取对应的容器idx
    返回说明：
        无
 **************************************************************************************************/
void    vGetHostList(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
{
    BOOL    bList = false;
    long    i = 0, j, m = 0;

    for(i = 0, m = 0; i < lFlow; i ++)
    {
        if(pstFlow[i].m_lHost <= 0)
            continue;

        for(j = 0, bList = false; j < *plList; j ++)
        {
            if(pstFlow[i].m_lHost != plGroup[j])
                continue;

            bList = true;    
            break;
        }

        if(!bList)
        {
            plGroup[m] = pstFlow[i].m_lHost;
            m ++;    
        }
    }

    *plList = m;

    return ;
}

/**************************************************************************************************
    函 数 名：vGetClassList(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
    功能描述：从流程列表中获取对应的组件
    返回说明：
        无
 **************************************************************************************************/
void    vGetClassList(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
{
    BOOL    bList = false;
    long    i = 0, j, m = 0;

    for(i = 0, m = 0; i < lFlow; i ++)
    {
        if(pstFlow[i].m_lBuss <= 0)
            continue;

        for(j = 0, bList = false; j < *plList; j ++)
        {
            if(pstFlow[i].m_lBuss != plGroup[j])
                continue;

            bList = true;    
            break;
        }

        if(!bList)
        {
            plGroup[m] = pstFlow[i].m_lBuss;
            m ++;    
        }
    }

    *plList = m;
    return ;
}

/**************************************************************************************************
    函 数 名：vGetFlowList(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
    功能描述：获取流程列表中所用到的服务编号
    返回说明：
        无
 **************************************************************************************************/
void    vGetFlowList(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
{
    BOOL    bList = false;
    long    i = 0, j, m = 0;

    for(i = 0, m = 0; i < lFlow; i ++)
    {
        if(pstFlow[i].m_lSvrID <= 0)
            continue;

        for(j = 0, bList = false; j < *plList; j ++)
        {
            if(pstFlow[i].m_lSvrID != plGroup[j])
                continue;

            bList = true;    
            break;
        }

        if(!bList)
        {
            plGroup[m] = pstFlow[i].m_lSvrID;
            m ++;    
        }
    }

    *plList = m;
    return ;
}

/**************************************************************************************************
    函 数 名：vGetMidList(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
    功能描述：从流程中所用的到的报文格式
    返回说明：
        无
 **************************************************************************************************/
void    vGetMidList(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
{
    BOOL    bList = false;
    long    i = 0, j, m = 0;

    for(i = 0, m = 0; i < lFlow; i ++)
    {
        if(pstFlow[i].m_lInMid <= 0 && pstFlow[i].m_lOutMid <= 0 && pstFlow[i].m_lErrMid <= 0)
            continue;

        for(j = 0, bList = false; (j < *plList) & (pstFlow[i].m_lInMid > 0); j ++)
        {
            if(pstFlow[i].m_lInMid != plGroup[j])
                continue;

            bList = true;    
            break;
        }

        if(!bList)
            plGroup[m ++] = pstFlow[i].m_lInMid;

        for(j = 0, bList = false; (j < *plList) & (pstFlow[i].m_lOutMid > 0); j ++)
        {
            if(pstFlow[i].m_lOutMid != plGroup[j])
                continue;

            bList = true;    
            break;
        }

        if(!bList)
            plGroup[m ++] = pstFlow[i].m_lOutMid;

        for(j = 0, bList = false; (j < *plList) & (pstFlow[i].m_lOutMid > 0); j ++)
        {
            if(pstFlow[i].m_lErrMid != plGroup[j])
                continue;

            bList = true;    
            break;
        }

        if(!bList)
            plGroup[m ++] = pstFlow[i].m_lErrMid;
    }

    *plList = m;
    return ;
}

/**************************************************************************************************
    函 数 名：vGetMidPack(CtsFlow *pstFlow, long lFlow, long *plGroup, long *plList)
    功能描述：按照交易导出配置
    返回说明：
        无
 **************************************************************************************************/
void    vGetMidPack(CtsPack *pstPack, long lFlow, long *plGroup, long *plList)
{
    BOOL    bList = false;
    long    i = 0, j, m = 0;

    for(i = 0, m = 0; i < lFlow; i ++)
    {
        if(pstPack[i].m_lMid <= 0)
            continue;

        for(j = 0, bList = false; j < *plList; j ++)
        {
            if(pstPack[i].m_lMid != plGroup[j])
                continue;

            bList = true;    
            break;
        }

        if(!bList)
        {
            plGroup[m] = pstPack[i].m_lMid;
            m ++;    
        }
    }

    *plList = m;
    return ;
}

/**************************************************************************************************
    函 数 名：vExportTrade(char *pszTrCode)
    功能描述：按照交易导出配置
    返回说明：
        无
 **************************************************************************************************/
long    lExportTrade(char *pszTrCode, char *pszFile)
{
    FILE       *fp = NULL;
    CtsHost    stHost;
    CtsClass   stClass;
    char       szBuffer[1024];
    CtsFlow    *pstFlow = NULL;
    long       i = 0, lOut = 0, lSvrId;
    long       *plList, lList = 0;
    SATvm      *pstSavm = (SATvm *)pGetSATvm();
    CMList     *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != lExportService(pszTrCode, pszFile, &lSvrId))
        return RC_FAIL;

    //    导出流程、导出交易映射码
    if(RC_SUCC != lGetSvrFlow(pstSavm, lSvrId, &pstFlow, &lOut))
    {
        fprintf(stderr, "\n错误：获取服务(%s)(%d)信息失败, err:%s\n\n", pszTrCode, 
            lSvrId, sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

    if(lOut <= 0)    return RC_SUCC;

    if(NULL == (fp = fopen(pszFile, "a")))
    {
        IBPFree(pstFlow);
        fprintf(stderr, "打开文件(%s)失败, err:%s\n", pszFile, strerror(errno));
        return RC_FAIL;
    }

    lList = lOut * 3;
    if(NULL == (plList = malloc(sizeof(long) * lList)))
    {
        fprintf(stderr, "分配列表个数(%d)失败, err:%s\n", lList, strerror(errno));
        goto ErrHandle;
    }

    //    获取容器列表
    memset(plList, 0, sizeof(long) * lList);
    vGetHostList(pstFlow, lOut, plList, &lList);

    fprintf(fp, "<Host>\n");
    for(i = 0; i < lList; i ++)
    {
        if(RC_SUCC != lGetHost(pstSavm, &stHost, plList[i]))
        {
            fprintf(stderr, "获取容器(%d)信息失败, err:%s\n", plList[i], 
                sGetTError(pstSavm->m_lErrno));
            goto ErrHandle;
        }

        memset(szBuffer, 0, sizeof(szBuffer));
        lConvHostCfg(szBuffer, sizeof(szBuffer), &stHost);
        fprintf(fp, "%s", szBuffer);
    }
    fprintf(fp, "</Host>\n");

    lList = lOut * 3;
    memset(plList, 0, sizeof(long) * lList);
    vGetClassList(pstFlow, lOut, plList, &lList);

    fprintf(fp, "<Class>\n");
    for(i = 0; i < lList; i ++)
    {
        if(RC_SUCC != lGetClassByIdx(pstSavm, &stClass, plList[i]))
        {
            fprintf(stderr, "\n错误：获取组件(%d)信息失败, err:%s\n\n", plList[i], 
                sGetTError(pstSavm->m_lErrno));
            goto ErrHandle;
        }

        memset(szBuffer, 0, sizeof(szBuffer));
        lConvClassCfg(szBuffer, sizeof(szBuffer), &stClass);
        fprintf(fp, "%s", szBuffer);
    }
    fprintf(fp, "</Class>\n");

    //    获取报文列表
    lList = lOut * 3;
    memset(plList, 0, sizeof(long) * lList);
    vGetMidList(pstFlow, lOut, plList, &lList);

    //    获取组件列表
    fprintf(fp, "<Pack>\n");
    for(i = 0; i < lList; i ++)
        vExportPack(pstSavm, plList[i], fp);
    fprintf(fp, "</Pack>\n");

    fclose(fp);
    IBPFree(plList);
    IBPFree(pstFlow);

    return RC_SUCC;
ErrHandle:
    fclose(fp);
    IBPFree(pstFlow);
    IBPFree(plList);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：vShowDifferent(TABLE t, void *o, char *n)
    功能描述：显示differnt信息
    返回说明：
        无
 **************************************************************************************************/
void	vShowDifferent(TABLE t, void *o, char *n)
{
	switch(t)
	{
    case CTS_TRADE_HOST:
		if(strcmp(((CtsHost *)o)->m_szName, ((CtsHost *)n)->m_szName))
			fprintf(stderr, ">> 原容器名称(%s)->%s\n", ((CtsHost *)o)->m_szName, 
				((CtsHost *)n)->m_szName);
		if(strcmp(((CtsHost *)o)->m_szPGName, ((CtsHost *)n)->m_szPGName))
			fprintf(stderr, ">> warn:进程名重启平台后生效!\n");
		if(((CtsHost *)o)->m_lMaxNum != ((CtsHost *)n)->m_lMaxNum)
			fprintf(stderr, ">> warn:启动最大个数重启平台后生效!\n");
		break;

    case CTS_TRADE_CLASS:
		if(strcmp(((CtsClass *)o)->m_szClass, ((CtsClass *)n)->m_szClass))
			fprintf(stderr, ">> 原组件类名(%s)->%s\n", ((CtsClass *)o)->m_szClass, 
				((CtsClass *)n)->m_szClass);
		if(strcmp(((CtsClass *)o)->m_szMethod, ((CtsClass *)n)->m_szMethod))
			fprintf(stderr, ">> 原组件方法(%s)->%s\n", ((CtsClass *)o)->m_szMethod, 
				((CtsClass *)n)->m_szMethod);
		if(strcmp(((CtsClass *)o)->m_szMark, ((CtsClass *)n)->m_szMark))
			fprintf(stderr, ">> 原组件名称(%s)->%s\n", ((CtsClass *)o)->m_szMark, 
				((CtsClass *)n)->m_szMark);
		break;
    case CTS_TRADE_SERVICE:
		if(strcmp(((CtsService *)o)->m_szSvrName, ((CtsService *)n)->m_szSvrName))
			fprintf(stderr, ">> 原服务名称(%s)->%s\n", ((CtsService *)o)->m_szSvrName, 
				((CtsService *)n)->m_szSvrName);
		break;
    case CTS_TRADE_CODE:
		if(strcmp(((CtsTrCode *)o)->m_szTrCode, ((CtsTrCode *)n)->m_szTrCode))
			fprintf(stderr, ">> 原服务编码(%s)->%s\n", ((CtsTrCode *)o)->m_szTrCode, 
				((CtsTrCode *)n)->m_szTrCode);
		break;
    case CTS_TRADE_FLOW:
    case CTS_PACK_CONFIG:
    case CTS_RETCODE_MAP:
    default:
    	break;
	}

	return ;
}

/**************************************************************************************************
    函 数 名：lInsertGroup(SATvm *pstSavm, CtsHost *pstHost)
    功能描述：加载进程配置信息
    返回说明：
        无
 **************************************************************************************************/
long    lInsertGroup(SATvm *pstSavm, CtsHost *pstHost)
{
	CtsGroup    stGroup;
    CtsProcess  stProcess;
	long        lIdx, lGrpIdx, i;

    if(!strlen(pstHost->m_szPGName))
        return RC_SUCC;

    memset(&stGroup, 0, sizeof(CtsGroup));
    pstSavm->pstVoid  = NULL;
    pstSavm->lReMatch = MATCH_MAX;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->tblName  = CTS_PLAT_GROUP;
    pstSavm->lSize    = sizeof(CtsGroup);
    pstSavm->lReField = ReField(CtsGroup, m_lGrpIdx);
    if(RC_SUCC != lExtreme(pstSavm, &stGroup))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
           return RC_FAIL;
    }

    memset(&stProcess, 0, sizeof(CtsProcess));
    pstSavm->pstVoid  = NULL;
    pstSavm->lReMatch = MATCH_MAX;
    pstSavm->lFind    = IDX_SELECT;
    pstSavm->tblName  = CTS_PLAT_PROCESS;
    pstSavm->lSize    = sizeof(CtsProcess);
    pstSavm->lReField = ReField(CtsProcess, m_lIdx);
    if(RC_SUCC != lExtreme(pstSavm, &stProcess))
    {
        if(NO_DATA_FOUND != pstSavm->m_lErrno)
           return RC_FAIL;
    }

	lIdx    =  ++ stProcess.m_lIdx;
	lGrpIdx =  ++ stGroup.m_lGrpIdx;
    memset(&stGroup, 0, sizeof(CtsGroup));
    stGroup.m_lAppPos  = lIdx;
    stGroup.m_lGrpIdx  = lGrpIdx;
    stGroup.m_lState   = STATUES_ENABLE;
    stGroup.m_lHost    = pstHost->m_lHost;
    stGroup.m_lType    = pstHost->m_lType;
    stGroup.m_lMinNum  = pstHost->m_lMinNum;
    stGroup.m_lMaxNum  = pstHost->m_lMaxNum;
    stGroup.m_lAutoExt = pstHost->m_lAutoExt;
    stGroup.m_bIsBoot  = pstHost->m_bIsBoot;
    stGroup.m_bDbRcon  = pstHost->m_bDbRcon;
    strncpy(stGroup.m_szTime, sGetUpdTime(), sizeof(stGroup.m_szTime));
    strncpy(stGroup.m_szPGName, pstHost->m_szPGName, sizeof(stGroup.m_szPGName));
    if(RC_SUCC > (stGroup.m_lQid = lCreateQueue(pstSavm, false)))
    {
        fprintf(stderr, "创建消息队列失败, %s\n", sGetTError(pstSavm->m_lErrno));
        return RC_FAIL;
    }

	memset(&stProcess, 0, sizeof(stProcess));
    stProcess.m_lDoNum  = 0;
	stProcess.m_lPend   = 0;
    stProcess.m_lState  = BCS_APP_STOP;
    stProcess.m_lHost   = pstHost->m_lHost;
    stProcess.m_lType   = stGroup.m_lType;
    stProcess.m_lGrpIdx = stGroup.m_lGrpIdx;
    stProcess.m_bDbRcon = stGroup.m_bDbRcon;
    memcpy(stProcess.m_szPGName, stGroup.m_szPGName, sizeof(stProcess.m_szPGName) - 1);
	for(lIdx = 0; lIdx < stGroup.m_lMaxNum; lIdx ++)
	{
		if(lIdx < stGroup.m_lMinNum)
			stProcess.m_lRunExt = BCS_EXPAND_NML;
		else
			stProcess.m_lRunExt = BCS_EXPAND_EXT;

        stProcess.m_lSeq ++;
        stProcess.m_lIdx = stGroup.m_lAppPos + lIdx;
        if(RC_SUCC != lAddProcess(pstSavm, &stProcess, false))
		{
            fprintf(stderr, "登记任务(%s)的进程失败!\n", stGroup.m_szPGName);
			return RC_FAIL;	
		}
	}

    pstSavm->lFind   = IDX_SELECT;
    pstSavm->tblName = CTS_PLAT_GROUP;
    pstSavm->lSize   = sizeof(CtsGroup);
    pstSavm->pstVoid = (void *)&stGroup;
    if(RC_SUCC != lInsert(pstSavm))
	{
		msgctl(stGroup.m_lQid, IPC_RMID, NULL);
        return RC_FAIL;
	}

	return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：_lImportHost(char *pszFile)
    功能描述：根据容器配置导入到运行引擎中
    返回说明：
        无
 **************************************************************************************************/
long    _lImportHost(char *pszFile)
{
	char       ch;
    CtsHost    stHost, *pstHost = NULL;
    size_t     lCount = 0, i = 0, lRet = 0;
    SATvm      *pstSavm = (SATvm *)pGetSATvm();
    CMList     *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != _lInitTruck(&pstRoot, "Host", pszFile))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(lCount <= 0)        return RC_SUCC;

    if(!(pstHost = (CtsHost *)malloc(sizeof(CtsHost) * lCount)))
    {
        fprintf(stderr, "分配内存(%d)失败, err:%s\n", sizeof(CtsHost) * lCount, 
            strerror(errno));
        goto ErrFreeHost;
    }
    memset(pstHost, 0, sizeof(CtsHost) * lCount);

    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetHostCfg((char *)pstNode->m_psvData, &pstHost[i]);
    IBPFreeList(pstRoot);

    for(i = 0; i < lCount; i ++)
    {
        lRet = lGetHost(pstSavm, &stHost, pstHost[i].m_lHost);
        if(RC_SUCC != lRet && NO_DATA_FOUND != pstSavm->m_lErrno)
            goto ErrFreeHost;

        if(NO_DATA_FOUND == pstSavm->m_lErrno)
        {
            if(RC_SUCC != lInsertHost(pstSavm, &pstHost[i]))
                goto ErrFreeHost;

			if(RC_SUCC != lInsertGroup(pstSavm, &pstHost[i]))
                goto ErrFreeHost;

            fprintf(stdout, "重建容器(%d)配置表索引成功, 导入完成\n", pstHost[i].m_lHost);
            continue;
        }
		else
		{
			if(!memcmp(&stHost, &pstHost[i], sizeof(CtsHost)))
				continue;

            fprintf(stdout, "容器(%d)配置已存在, 回车后默认覆盖(Y/N):", stHost.m_lHost);
			vShowDifferent(CTS_TRADE_HOST, (void *)&stHost, (void *)&pstHost[i]);
			ch = getchar();
			if('N' == ch || 'n' == ch)
			{
    			IBPFree(pstHost);
				return RC_FAIL;
			}

			if(RC_SUCC != lUpdateHost(pstSavm, &pstHost[i]))
                goto ErrFreeHost;
        }
    }

    IBPFree(pstHost);
    return RC_SUCC;

ErrFreeHost:
    fprintf(stderr, "处理容器(%d)失败, %s\n", stHost.m_lHost, sGetTError(pstSavm->m_lErrno));
    IBPFree(pstHost);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：_lImportClass(char *pszFile)
    功能描述：根据容器配置导入到运行引擎中
    返回说明：
        无
 **************************************************************************************************/
long    _lImportClass(char *pszFile)
{
	char        ch;
    CtsClass    stClass, *pstClass = NULL;
    long        lCount = 0, i = 0, lRet = 0;
    CMList      *pstNode = NULL, *pstRoot = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != _lInitTruck(&pstRoot, "Class", pszFile))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(lCount <= 0)        return RC_SUCC;
    
    if(!(pstClass = (CtsClass *)malloc(sizeof(CtsClass) * lCount)))
    {
        fprintf(stderr, "分配内存(%d)失败, err:%s\n", sizeof(CtsClass) * lCount, 
            strerror(errno));
        goto ErrFreeClass;
    }
    memset(pstClass, 0, sizeof(CtsClass) * lCount);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetClassCfg((char *)pstNode->m_psvData, &pstClass[i]);
    IBPFreeList(pstRoot);

    for(i = 0; i < lCount; i ++)
    {
        lRet = lGetClassByIdx(pstSavm, &stClass, pstClass[i].m_lBuss);
        if(RC_SUCC != lRet && NO_DATA_FOUND != pstSavm->m_lErrno)
            goto ErrFreeClass;
        
		if(NO_DATA_FOUND == pstSavm->m_lErrno)
        {
            if(RC_SUCC != lInsertClass(pstSavm, &pstClass[i]))
                goto ErrFreeClass;

            fprintf(stdout, "重建组件(%d)配置表索引成功, 导入完成\n");
        }
        else
        {
            if(!memcmp(&stClass, &pstClass[i], sizeof(CtsClass)))
                continue;

            fprintf(stdout, "组件(%d)配置已存在, 回车后默认覆盖(Y/N):", stClass.m_lBuss);
            vShowDifferent(CTS_TRADE_CLASS, (void *)&stClass, (void *)&pstClass[i]);
            ch = getchar();
            if('N' == ch || 'n' == ch)
            {
                IBPFree(pstClass);
                return RC_FAIL;
            }

            if(RC_SUCC != lUpdateClass(pstSavm, &pstClass[i]))
                goto ErrFreeClass;
        }
    }
    IBPFree(pstClass);

    return RC_SUCC;

ErrFreeClass:
    fprintf(stderr, "处理组件(%d)失败, %s\n", stClass.m_lBuss, sGetTError(pstSavm->m_lErrno));
    IBPFree(pstClass);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：_lImportService(char *pszFile)
    功能描述：根据容器配置导入到运行引擎中
    返回说明：
        无
 **************************************************************************************************/
long    _lImportService(char *pszFile)
{
	char        ch;
    long        lRow = 0, i = 0, lRet = 0;
    CMList      *pstNode = NULL, *pstRoot = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();
    CtsService  stService, *pstService = NULL;

    if(RC_SUCC != _lInitTruck(&pstRoot, "Service", pszFile))
        return RC_FAIL;

    lRow = lListNodeCount(pstRoot);
    if(lRow <= 0)    return RC_SUCC;

    if(!(pstService = (CtsService *)malloc(sizeof(CtsService) * lRow)))
    {
        fprintf(stderr, "分配内存(%d)失败, err:%s\n", sizeof(CtsService) * lRow, 
            strerror(errno));
        goto ErrorService;
    }

    memset(pstService, 0, sizeof(CtsService) * lRow);
    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetServiceCfg((char *)pstNode->m_psvData, &pstService[i]);
    IBPFreeList(pstRoot);

    for(i = 0; i < lRow; i ++)
    {
        lRet = lGetService(pstSavm, &stService, pstService[i].m_szTrCode);
        if(RC_SUCC != lRet && NO_DATA_FOUND != pstSavm->m_lErrno)
            goto ErrorService;
        
		if(NO_DATA_FOUND == pstSavm->m_lErrno)
        {
    		memset(&stService, 0, sizeof(stService));
    		stService.m_lSvrID =  pstService[i].m_lSvrID;

    		pstSavm->pstVoid = &stService;
    		pstSavm->lFind   = IDX_SELECT;
    		pstSavm->lSize   = sizeof(CtsService);
    		pstSavm->tblName = CTS_TRADE_SERVICE;
    		if(RC_SUCC != lCount(pstSavm, &lRet))
			    goto ErrorService;

            if(lRet > 0)
            {
                fprintf(stderr, "配置可能存在问题, 服务(%d)编号重复!\n", stService.m_lSvrID);
			    goto ErrorService;
            }

            if(RC_SUCC != lInsertService(pstSavm, &pstService[i]))
                goto ErrorService;

            fprintf(stdout, "重建服务(%s)配置表索引成功, 导入完成\n", pstService[i].m_szTrCode);
		}
        else
        {
            if(!memcmp(&stService, &pstService[i], sizeof(CtsService)))
                continue;

            fprintf(stdout, "服务(%s)配置已存在, 回车后默认覆盖(Y/N):", pstService[i].m_szTrCode);
            vShowDifferent(CTS_TRADE_SERVICE, (void *)&stService, (void *)&pstService[i]);
            ch = getchar();
            if('N' == ch || 'n' == ch)
            {
                IBPFree(pstService);
                return RC_FAIL;
            }

            if(RC_SUCC != lUpdateService(pstSavm, &pstService[i]))
                goto ErrorService;
		}
	}

    IBPFree(pstService);
    return RC_SUCC;

ErrorService:
    IBPFree(pstService);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：_lImportTrCode(char *pszFile)
    功能描述：根据容器配置导入到运行引擎中
    返回说明：
        无
 **************************************************************************************************/
long    _lImportTrCode(char *pszFile)
{
	char		ch;
    CtsTrCode   stCode, *pstCode = NULL;
    long        lRow = 0, i = 0, lRet = 0;
    CMList      *pstNode = NULL, *pstRoot = NULL;
    SATvm       *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != _lInitTruck(&pstRoot, "TrCode", pszFile))
        return RC_FAIL;

    lRow = lListNodeCount(pstRoot);
    if(lRow <= 0)        return RC_SUCC;

    if(!(pstCode = (CtsTrCode *)malloc(sizeof(CtsTrCode) * lRow)))
    {
        fprintf(stderr, "分配内存(%d)失败, err:%s\n", sizeof(CtsTrCode) * lRow, 
            strerror(errno));
         goto ErrorTrCode;
    }
    memset(pstCode, 0, sizeof(CtsTrCode) * lRow);

    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetTrCodeCfg((char *)pstNode->m_psvData, &pstCode[i]);
    IBPFreeList(pstRoot);

    for(i = 0; i < lRow; i ++)
    {
        lRet = lGetTrCode(pstSavm, &stCode, pstCode[i].m_szInCode, pstCode[i].m_szInSCode);
        if(RC_SUCC != lRet && NO_DATA_FOUND != pstSavm->m_lErrno)
            goto ErrorTrCode;
        
		if(NO_DATA_FOUND == pstSavm->m_lErrno)
        {
            memset(&stCode, 0, sizeof(CtsTrCode));
            stCode.m_lIdx  = pstCode[i].m_lIdx;

            pstSavm->pstVoid = &stCode;
            pstSavm->lFind   = IDX_SELECT;
            pstSavm->tblName = CTS_TRADE_CODE;
            pstSavm->lSize   = sizeof(CtsTrCode);
            if(RC_SUCC != lCount(pstSavm, &lRet))
                goto ErrorTrCode;

            if(lRet > 0)
            {
                fprintf(stderr, "配置可能存在问题, 交易码(%d)编号重复!\n", stCode.m_lIdx);
                goto ErrorTrCode;
            }

            if(RC_SUCC != lInsertTrCode(pstSavm, &pstCode[i]))
                goto ErrorTrCode;

            fprintf(stdout, "重建映射(%s)配置表索引成功, 导入完成\n", pstCode[i].m_szTrCode);
		}
        else
        {
            if(!memcmp(&stCode, &pstCode[i], sizeof(CtsTrCode)))
                continue;

            fprintf(stdout, "服务(%s)配置已存在, 回车后默认覆盖(Y/N):", pstCode[i].m_szTrCode);
            vShowDifferent(CTS_TRADE_CODE, (void *)&stCode, (void *)&pstCode[i]);
            ch = getchar();
            if('N' == ch || 'n' == ch)
            {
                IBPFree(pstCode);
                return RC_FAIL;
            }

            if(RC_SUCC != lUpdateTrCode(pstSavm, &pstCode[i]))
                goto ErrorTrCode;
    	}
	}

    IBPFree(pstCode);
    return RC_SUCC;

ErrorTrCode:
    IBPFree(pstCode);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：_lImportFlow(char *pszFile)
    功能描述：根据容器配置导入到运行引擎中
    返回说明：
        无
 **************************************************************************************************/
long    _lImportFlow(char *pszFile)
{
    CtsFlow    *pstFlow = NULL, stFlow;
    long       *plList = NULL, lList = 0;
    long       lCount = 0, i = 0, lRet = 0;
    CMList     *pstNode = NULL, *pstRoot = NULL;
    SATvm      *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != _lInitTruck(&pstRoot, "Flow", pszFile))
        return RC_FAIL;

    lCount = lListNodeCount(pstRoot);
    if(lCount <= 0)    return RC_SUCC;

    if(!(pstFlow = (CtsFlow *)malloc(sizeof(CtsFlow) * lCount)))
    {
        fprintf(stderr, "分配内存(%d)失败, err:%s\n", sizeof(CtsFlow) * lCount, 
            strerror(errno));
         goto ErrFreeFlow;
    }
    memset(pstFlow, 0, sizeof(CtsFlow) * lCount);

    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetFlowCfg((char *)pstNode->m_psvData, &pstFlow[i]);
    IBPFreeList(pstRoot);

    lList = lCount;
    if(NULL == (plList = malloc(sizeof(long) * lList)))
    {
        fprintf(stderr, "分配列表个数(%d)失败, err:%s\n", lList, strerror(errno));
        goto ErrFreeFlow;
    }

    //    获取容器列表
    memset(plList, 0, sizeof(long) * lList);
    vGetFlowList(pstFlow, lCount, plList, &lList);
    for(i = 0; i < lList; i ++)
    {
        if(RC_SUCC != lDeleteFlow(pstSavm, plList[i]))
        {
            fprintf(stderr, "删除流程(%d)服务失败, err:%s\n", plList[i], 
                sGetTError(pstSavm->m_lErrno));
            goto ErrFreeFlow;
        }

        fprintf(stderr, "删除服务流程编号(%d)索引成功\n", plList[i]);
    }

    if(RC_SUCC != lInsertFlow(pstSavm, pstFlow, lCount))
    {    
        fprintf(stderr, "导入流程(%d)服务失败, err:%s\n", plList[i], 
            sGetTError(pstSavm->m_lErrno));
        goto ErrFreeFlow;
    }

    fprintf(stdout, "重建流程配置表索引成功, 导入完成\n");
    IBPFree(plList);
    IBPFree(pstFlow);

    return RC_SUCC;

ErrFreeFlow:
    IBPFree(plList);
    IBPFree(pstFlow);
    IBPFreeList(pstRoot);

    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：_lImportPack(char *pszFile)
    功能描述：根据容器配置导入到运行引擎中
    返回说明：
        无
 **************************************************************************************************/
long    _lImportPack(char *pszFile)
{
    CtsPack   *pstPack = NULL, stPack;
    long      lCount = 0, i = 0, lRet = 0;
    CMList    *pstNode = NULL, *pstRoot = NULL;
    SATvm     *pstSavm = (SATvm *)pGetSATvm();
    long      *plList = NULL, lList = 0;

    if(RC_SUCC != _lInitTruck(&pstRoot, "Pack", pszFile))
        return ;

    lCount = lListNodeCount(pstRoot);
    if(lCount <= 0)        return RC_SUCC;

    if(!(pstPack = (CtsPack *)malloc(sizeof(CtsPack) * lCount)))
    {
        fprintf(stderr, "分配内存(%d)失败, err:%s\n", sizeof(CtsPack) * lCount, 
            strerror(errno));
        goto ErrFreePack;
    }
    memset(pstPack, 0, sizeof(CtsPack) * lCount);

    for(pstNode = pstRoot, i = 0; pstNode; i ++, pstNode = pstNode->pstNext)
        vGetPackCfg((char *)pstNode->m_psvData, &pstPack[i]);
    IBPFreeList(pstRoot);

    lList = lCount;
    if(NULL == (plList = malloc(sizeof(long) * lList)))
    {
        fprintf(stderr, "分配列表个数(%d)失败, err:%s\n", lList, strerror(errno));
        goto ErrFreePack;
    }

    //    获取容器列表
    memset(plList, 0, sizeof(long) * lList);
    vGetMidPack(pstPack, lCount, plList, &lList);
    for(i = 0; i < lList; i ++)
    {
        if(RC_SUCC != lDeletePack(pstSavm, plList[i]))
        {
            fprintf(stderr, "删除流程(%d)服务失败, err:%s\n", plList[i], 
                sGetTError(pstSavm->m_lErrno));
            goto ErrFreePack;
        }

        fprintf(stdout, "删除服务组件报文编号(%d)索引成功\n", plList[i]);
    }

    if(RC_SUCC != lInsertPack(pstSavm, pstPack, lCount))
    {    
        fprintf(stderr, "导入组件报文(%d)配置失败, err:%s\n", plList[i], 
            sGetTError(pstSavm->m_lErrno));
        goto ErrFreePack;
    }
    fprintf(stdout, "重建组件报文配置表索引成功, 导入完成\n");
    IBPFree(plList);
    IBPFree(pstPack);

    return RC_SUCC;

ErrFreePack:
    IBPFree(plList);
    IBPFree(pstPack);

    return RC_FAIL;
}

long	_lImportMapCode(char *pszFile)
{






	return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lImportHost(char *pszFile)
    功能描述：根据配置导入到运行引擎中
    返回说明：
        无
 **************************************************************************************************/
long    lImportConfig(char *pszFile, BOOL bCover)
{
    char    szPath[512];

    memset(szPath, 0, sizeof(szPath));
    fprintf(stdout, "\n开始导入配置文件名:%s  ..?", pszFile);
    getchar();
    fprintf(stdout, "\b\b\n\n");
    snprintf(szPath, sizeof(szPath), ".%s.bak", sGetCurrentTime());

    //    从引擎中备份当前配置
    if(RC_SUCC != lDumpToFile(szPath))
        return RC_FAIL;

    if(RC_SUCC != _lImportHost(pszFile))
        return RC_FAIL;

    if(RC_SUCC != _lImportClass(pszFile))
        return RC_FAIL;

    if(RC_SUCC != _lImportService(pszFile))
        return RC_FAIL;

    if(RC_SUCC != _lImportTrCode(pszFile))
        return RC_FAIL;

    if(RC_SUCC != _lImportFlow(pszFile))
        return RC_FAIL;

    if(RC_SUCC != _lImportPack(pszFile))
        return RC_FAIL;

    if(RC_SUCC != _lImportMapCode(pszFile))
        return RC_FAIL;

    if (!getenv("SETUPCFG"))
    {
        fprintf(stderr, "系统配置变量SETUPCFG未配置\n");
        return RC_FAIL;
    }

    unlink(szPath);
    memset(szPath, 0, sizeof(szPath));
    snprintf(szPath, sizeof(szPath), "%s", getenv("SETUPCFG"));
    if(RC_SUCC != lDumpToFile(szPath))
        return RC_FAIL;

    fprintf(stdout, "\n导入配置(%s)配置成功, completed successfully!!!\n\n", pszFile);
}

/**************************************************************************************************
    函 数 名：vRunAction(long lAction, char *pszPGName, char *pszParam)
    功能描述：ctool导出导入功能操作
    返回说明：
        无
 **************************************************************************************************/
void    vRunAction(long lAction, char *pszPGName, char *pszParam, char *pszFile)
{
    long    lSvrId = 0;

    if(18 != lAction && 20 != lAction && 24 != lAction)
    {
        vPrintFunc(pszPGName);
        return ;
    }

    if(!pszFile || !strlen(pszFile))
    {
        fprintf(stdout, "\n操作错误:未指明导出/导入文件名\n");
        return ;
    }

    //    提示存在是否覆盖
    if(RC_SUCC == access(pszFile, R_OK))
    {
        fprintf(stdout, "\n警告：当前目录里下存在文件(%s), 请确认，回车默认覆盖处理\n.", 
            pszFile);
        fflush(stdout);
        getchar();
    }

    switch(lAction)
    {
    case 18:
        vExportHost(atol(pszParam), pszFile);
        break;
    case 20:
        if(RC_SUCC != lExportClass(atol(pszParam), pszFile))
            break;
        fprintf(stdout, "\n导出组件(%d)配置成功, completed successfully!!!\n\n",     
            atol(pszParam));
        break;
    case 24:
        if(RC_SUCC != lExportTrade(pszParam, pszFile))
            break;
        fprintf(stdout, "\n导出服务(%s)配置成功, completed successfully!!!\n\n",
              pszParam);
        break;
    default:
        vPrintFunc(pszPGName);
        return ;
    }

    return ;
}

/**************************************************************************************************
    函 数 名：cts主函数
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
int     main(int argc, char *argv[])
{
    int        iChoose = 0;
    long    lAction = 0;
    char    szCom[100], szFile[256];
    SATvm    *pstSavm = (SATvm *)pGetSATvm();

    if(RC_SUCC != lInitRunTest(basename(argv[0])))
    {
        fprintf(stderr, "初始化测试进程失败");
        return RC_FAIL;
    }

    memset(szCom, 0, sizeof(szCom));
    memset(szFile, 0, sizeof(szFile));
    while(-1 != (iChoose = getopt(argc, argv, "M::h:c:s:o:f::i::v?::")))
    {
        switch(iChoose)
        {
        case    'i':
            lAction |= 1;
            break;
        case    'M':
            vActualMonitor();
            return RC_SUCC;
        case    'h':
            lAction |= 2;
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            break;
        case    'c':
            lAction |= 4;
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            break;
        case    's':        //    回收
            lAction |= 8;
            snprintf(szCom, sizeof(szCom), "%s", optarg);
            break;
        case    'o':
            lAction |= 16;
            snprintf(szFile, sizeof(szFile), "%s", optarg);
            break;    
        case    'f':
            lAction |= 32;
            break;
        case    'v':
            fprintf(stdout, "%s\n", sGetToolVers());
            fflush(stdout);
            return RC_SUCC;
        case    '?':
        default:
            vPrintFunc(basename(argv[0]));
            return RC_FAIL;
        }
    }

    if(17 == lAction || 49 == lAction)
        return lImportConfig(szFile, (BOOL)(lAction & 0x20));

    vRunAction(lAction, basename(argv[0]), szCom, szFile);

    return RC_SUCC;
}

/**************************************************************************************************
     code end
 **************************************************************************************************/
