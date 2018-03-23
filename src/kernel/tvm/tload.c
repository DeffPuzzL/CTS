#include    "tvm.h"
#include    "tmain.h"

void    vCreateTest()
{
	long	lNum = 0;
	TBoot	stBoot;
	FILE	*fp = NULL;
	TIndex	stIndex;
	TDomain	stDomain;

	memset(&stBoot, 0, sizeof(TBoot));
	memset(&stIndex, 0, sizeof(TIndex));
	memset(&stDomain, 0, sizeof(TDomain));
	stBoot.m_lMaxTable = 255;
	stBoot.m_lMaxField = 3000;
	stBoot.m_lMaxDomain = 1024;
	stBoot.m_lMaxSeque = 1024;
	stBoot.m_lSvrBoot = 4;
	stBoot.m_lSvrPort = 2000;
	stBoot.m_lDomPort = 2001;
	strcpy(stBoot.m_szNode, "TVM");

	fp = fopen("tvm.run", "wb");
	fwrite(TVM_RUNCFG_TAG, 4, 1, fp);
	fwrite(&stBoot, sizeof(stBoot), 1, fp);
	fwrite(&lNum, sizeof(long), 1, fp);

	lNum = 1;
	stIndex.m_table = 8;
	stIndex.m_lType = TYPE_CLIENT;
	strcpy(stIndex.m_szTable, "TBL_ACCT_INFO");
	strcpy(stIndex.m_szPart, "BCS");
	strcpy(stIndex.m_szOwner, "BCS");
	stIndex.m_lLocal = RES_REMOT_SID;
	stIndex.m_lState = RESOURCE_INIT;
	
	fwrite(&lNum, sizeof(long), 1, fp);
	fwrite(&stIndex, sizeof(TIndex), 1, fp);

	lNum = 1;
	fwrite(&lNum, sizeof(long), 1, fp);
	stDomain.m_table = stIndex.m_table;
	stDomain.m_lGroup = 1;
	stDomain.m_lKeepLive = 30;
	stDomain.m_lTimeOut = 4;
	stDomain.m_lTryMax = 3;
	stDomain.m_lStatus = RESOURCE_STOP;
	stDomain.m_lLIPort = stBoot.m_lSvrPort;
	stDomain.m_lLDPort = stBoot.m_lDomPort;
	stDomain.m_lRDPort = 1801;
	strcpy(stDomain.m_szIp, "172.30.0.67");
	strcpy(stDomain.m_szTable, "TBL_ACCT_INFO");
	strcpy(stDomain.m_szPart, "BCS");
	strcpy(stDomain.m_szOwner, "BCS");
	fwrite(&stDomain, sizeof(TDomain), 1, fp);
/*
	strcpy(stDomain.m_szTable, "TBL_ACCT_INFO");
	fwrite(&stDomain, sizeof(TDomain), 1, fp);

	strcpy(stDomain.m_szTable, "TBL_BRH_PROFIT_ALGO");
	fwrite(&stDomain, sizeof(TDomain), 1, fp);
*/
	fclose(fp);
}

/*************************************************************************************************
	函数说明：解析配置文件
	参数说明：
		pstSavm                    --操作句柄
	返回值：
		RC_SUCC                    --成功
		RC_FAIL                    --失败
 *************************************************************************************************/
long    _lParseFile(SATvm *pstSavm, CMList **ppstRoot, char *pszFile, const char *pszTarget)
{
    FILE    *fp = NULL;
    char    szLine[4098];
    BOOL    bFlag = FALSE;
    CMList  *pstList = NULL;

    if(NULL == (fp = fopen(pszFile, "rb")))
    {
        pstSavm->m_lErrno = FILE_NOTFOUND;
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

        if(!strcmp(pszTarget, szLine) && !bFlag)
        {
            bFlag = TRUE;
            memset(szLine, 0, sizeof(szLine));
            continue;
        }
        else if(szLine[0] == '*' && bFlag)
            break;

        if(!bFlag)
        {
            memset(szLine, 0, sizeof(szLine));
            continue;
        }

        if(NULL == (pstList = pInsertList(pstList, (void *)szLine, sizeof(szLine))))
        {
            fclose(fp);
            vDestroyList(pstList);
            return RC_FAIL;
        }
    }

    fclose(fp);
    *ppstRoot = pstList;

    return RC_SUCC;
}

/*************************************************************************************************
	函数说明：解析启动参数
	参数说明：
		pstSavm                    --操作句柄
	返回值：
		RC_SUCC                    --成功
		RC_FAIL                    --失败
 *************************************************************************************************/
long    lParseBoot(SATvm *pstSavm, char *pszFile, TBoot *pstBoot)
{
    char    szTarg[128], szValue[64];
    CMList  *pstNode = NULL, *pstRoot = NULL;

    if(RC_SUCC != _lParseFile(pstSavm, &pstRoot, pszFile, "*GLOBLE"))
    {
        fprintf(stderr, "parse file, err:(%d)(%s)\n", lGetTErrno(), sGetTError(lGetTErrno()));
        return RC_FAIL;
    }

    for(pstNode = pstRoot; pstNode; pstNode = pstNode->pstNext)
    {
        if(!pstNode->m_psvData) continue;

        if(!strstr(pstNode->m_psvData, "="))
        {
            fprintf(stdout, "%s\n*缺失=\n", pstNode->m_psvData);
            goto PBOOT_ERROR;
        }

        memset(szTarg, 0, sizeof(szTarg));
        memset(szValue, 0, sizeof(szValue));
        strncpy(szTarg, sgetvalue(pstNode->m_psvData, "=", 1), sizeof(szTarg));
        strncpy(szValue, sgetvalue(pstNode->m_psvData, "=", 2), sizeof(szValue));
        srtrim(szTarg);
        sltrim(szValue);
        strimabout(szValue, "\"", "\"");
        if(!strlen(szValue))
        {
            fprintf(stdout, "%s\n*配置错误, 未设置取值\n", pstNode->m_psvData);
            goto PBOOT_ERROR;
        }

        if(!strcasecmp(szTarg, "MACHINE"))
            strncpy(pstBoot->m_szNode, szValue, sizeof(pstBoot->m_szNode));
        else if(!strcasecmp(szTarg, "LOGNAME"))
            strncpy(pstBoot->m_szLog, szValue, sizeof(pstBoot->m_szLog));
		else if(!strcasecmp(szTarg, "EXTERN"))	
            pstBoot->m_lExtern = atol(szValue);
		else if(!strcasecmp(szTarg, "DEPLOY"))
		{
			if(!strcasecmp(szValue, "cluster"))
				pstBoot->m_lBootType = TVM_BOOT_CLUSTER;
			else if(!strcasecmp(szValue, "local"))
				pstBoot->m_lBootType = TVM_BOOT_LOCAL;
			else	// local
				pstBoot->m_lBootType = TVM_BOOT_SIMPLE;
		}
        else if(!strcasecmp(szTarg, "MAXTABLE"))
        {
            pstBoot->m_lMaxTable = atol(szValue);
            if(pstBoot->m_lMaxTable <= 5)
            {
                fprintf(stdout, "%s\n*设置TVM最大支持表个数错误\n", pstNode->m_psvData);
                goto PBOOT_ERROR;
            }
            else if(pstBoot->m_lMaxTable > 255)
            {
                fprintf(stdout, "%s\n*设置TVM最大支持表个数超限, 最大255\n", pstNode->m_psvData);
                goto PBOOT_ERROR;
            }
        }
        else if(!strcasecmp(szTarg, "MAXFILED"))
        {
            pstBoot->m_lMaxField = atol(szValue);
            if(pstBoot->m_lMaxField <= 100)
            {
                fprintf(stdout, "%s\n*平台记录字段记录设置错误\n", pstNode->m_psvData);
                goto PBOOT_ERROR;
            }
        }
        else if(!strcasecmp(szTarg, "MAXDOMAIN"))
        {
            pstBoot->m_lMaxDomain = atol(szValue);
            if(pstBoot->m_lMaxDomain <= 0)
            {
                fprintf(stdout, "%s\n*域信息最大记录设置错误\n", pstNode->m_psvData);
                goto PBOOT_ERROR;
            }
        }
        else if(!strcasecmp(szTarg, "MAXSEQUE"))
        {
            pstBoot->m_lMaxSeque = atol(szValue);
            if(pstBoot->m_lMaxSeque <= 0)
            {
                fprintf(stdout, "%s\n*序列最大个数设置错误\n", pstNode->m_psvData);
                goto PBOOT_ERROR;
            }
        }
        else if(!strcasecmp(szTarg, "SERVER_EXEC"))
        {
            pstBoot->m_lSvrBoot = atol(szValue);
            if(pstBoot->m_lSvrBoot <= 0)
            {
                fprintf(stdout, "%s\n*LIS启动个数设置错误\n", pstNode->m_psvData);
                goto PBOOT_ERROR;
            }
        }
        else if(!strcasecmp(szTarg, "SERVER_PORT"))
        {
            pstBoot->m_lSvrPort = atol(szValue);
            if(pstBoot->m_lSvrPort <= 0)
            {
                fprintf(stdout, "%s\n*LIS启动端口设置错误\n", pstNode->m_psvData);
                goto PBOOT_ERROR;
            }
        }
        else if(!strcasecmp(szTarg, "DOMAIN_PORT"))
        {
            pstBoot->m_lDomPort = atol(szValue);
            if(pstBoot->m_lDomPort <= 0)
            {
                fprintf(stdout, "%s\n*RDS启动端口设置错误\n", pstNode->m_psvData);
                goto PBOOT_ERROR;
            }
        }
        else
        {
            fprintf(stdout, "%s\n*无效的参数\n", pstNode->m_psvData);
            goto PBOOT_ERROR;
        }
    }

    if(!strlen(pstBoot->m_szNode))
    {
        fprintf(stdout, "MACHINE\n*本地节点未配置\n");
        goto PBOOT_ERROR;
    }

    if(pstBoot->m_lMaxTable <= 0)
        pstBoot->m_lMaxTable = TVM_MAX_TABLE;
    if(pstBoot->m_lMaxField <= 0)
        pstBoot->m_lMaxField = 3000;
    if(pstBoot->m_lMaxDomain <= 0)
        pstBoot->m_lMaxDomain = 500;
    if(pstBoot->m_lMaxSeque <= 0)
       pstBoot->m_lMaxSeque = 500;
    if(pstBoot->m_lSvrBoot <= 0)
        pstBoot->m_lSvrBoot = get_nprocs() > 0 ? get_nprocs() : 1;
    if(pstBoot->m_lSvrPort <= 0)
        pstBoot->m_lSvrPort = TVM_PORT_LISTEN;
    if(pstBoot->m_lDomPort <= 0)
        pstBoot->m_lDomPort = pstBoot->m_lSvrPort + 1;

    vDestroyList(pstRoot);

    return RC_SUCC;

PBOOT_ERROR:
    vDestroyList(pstRoot);
    return RC_FAIL;
}

/*************************************************************************************************
	函数说明：解析本地表配置参数
	参数说明：
		pstSavm                    --操作句柄
	返回值：
		RC_SUCC                    --成功
		RC_FAIL                    --失败
 *************************************************************************************************/
long    lParseIndex(SATvm *pstSavm, char *pszFile, long *plOut, TIndex **ppstIndex)
{
    long    i, j, n;
    TIndex  *pstIndex = NULL;
    CMList  *pstNode = NULL, *pstRoot = NULL;
    char    szTarg[128], szValue[64], szAttr[1024];

    memset(szTarg, 0, sizeof(szTarg));
    memset(szAttr, 0, sizeof(szAttr));
    memset(szValue, 0, sizeof(szValue));
    if(RC_SUCC != _lParseFile(pstSavm, &pstRoot, pszFile, "*LOCAL_RESOURCE"))
    {
        fprintf(stderr, "parse file, err:(%d)(%s)\n", lGetTErrno(), sGetTError(lGetTErrno()));
        return RC_FAIL;
    }

    for(pstNode = pstRoot, j = 0; pstNode; pstNode = pstNode->pstNext)
    {
        if(!pstNode->m_psvData) continue;

        sfieldreplace(pstNode->m_psvData, '\t', ' ');
        if(!strlen(pstNode->m_psvData)) continue;

        if(NULL == (pstIndex = (TIndex *)realloc(pstIndex, (++ j) * sizeof(TIndex))))
        {
            pstSavm->m_lErrno = MALLC_MEM_ERR;
            return RC_FAIL;
        }

        memset(&pstIndex[j - 1], 0, sizeof(TIndex));
        for(i = 0, n = lfieldnum(pstNode->m_psvData, " "); i < n; i ++)
        {
            memset(szAttr, 0, sizeof(szAttr));
            strncpy(szAttr, sfieldvalue(pstNode->m_psvData, " ",  i + 1), sizeof(szAttr));

            if(!strstr(szAttr, "="))
            {
                fprintf(stdout, "%s\n*缺失=\n", pstNode->m_psvData);
                goto PINDEX_ERROR;
            }

            memset(szTarg, 0, sizeof(szTarg));
            memset(szValue, 0, sizeof(szValue));
            strncpy(szTarg, sgetvalue(szAttr, "=", 1), sizeof(szTarg));
            strncpy(szValue, sgetvalue(szAttr, "=", 2), sizeof(szValue));
            srtrim(szTarg);
            sltrim(szValue);
            strimabout(szValue, "\"", "\"");
            if(!strlen(szValue))
            {
                fprintf(stdout, "%s\n*配置错误, 未设置取值\n", pstNode->m_psvData);
                goto PINDEX_ERROR;
            }

            if(!strcasecmp(szTarg, "TABLE"))
                pstIndex[j - 1].m_table = atol(szValue);
            else if(!strcasecmp(szTarg, "PERMIT"))
                pstIndex[j - 1].m_lPers = atol(szValue);
            else
            {
                fprintf(stdout, "%s\n*无效的参数\n", pstNode->m_psvData);
                goto PINDEX_ERROR;
            }
        }

        if(pstIndex[j - 1].m_table <= 0)
        {
            fprintf(stdout, "%s\n*表设置错误\n", pstNode->m_psvData);
            goto PINDEX_ERROR;
        }

        if(pstIndex[j - 1].m_lPers <= 0)
            pstIndex[j - 1].m_lPers = OPERATE_DEFAULT;
    }
    *plOut = j;
    *ppstIndex = pstIndex;

    vDestroyList(pstRoot);
    return RC_SUCC;

PINDEX_ERROR:
    IBPFree(*ppstIndex);
    vDestroyList(pstRoot);
    return RC_FAIL;
}

/*************************************************************************************************
	函数说明：解析域配置
	参数说明：
		pstSavm                    --操作句柄
	返回值：
		RC_SUCC                    --成功
		RC_FAIL                    --失败
 *************************************************************************************************/
long    lParseDomain(SATvm *pstSavm, char *pszFile, TBoot *pstBoot, long *plOut, TIndex **ppstIndex, 
			long *plCout, TDomain **ppstDomain)
{
    long    i, j, m, n;
    TIndex  *pstIndex = NULL;
	TDomain	stDomain, *pstDomain = NULL;
    CMList  *pstNode = NULL, *pstRoot = NULL;
    char    szTarg[128], szValue[64], szAttr[1024];

    memset(szTarg, 0, sizeof(szTarg));
    memset(szAttr, 0, sizeof(szAttr));
    memset(szValue, 0, sizeof(szValue));
    memset(&stDomain, 0, sizeof(TDomain));
    if(RC_SUCC != _lParseFile(pstSavm, &pstRoot, pszFile, "*REMOTE_DOMAIN"))
    {
        fprintf(stderr, "parse file, err:(%d)(%s)\n", lGetTErrno(), sGetTError(lGetTErrno()));
        return RC_FAIL;
    }

    for(pstNode = pstRoot, j = 0, m = 0; pstNode; pstNode = pstNode->pstNext)
    {
        if(!pstNode->m_psvData) continue;

		if(!strncmp(pstNode->m_psvData, "TABLE", 5))
		{
        	if(NULL == (pstIndex = (TIndex *)realloc(pstIndex, (++ j) * sizeof(TIndex))))
        	{
            	pstSavm->m_lErrno = MALLC_MEM_ERR;
            	return RC_FAIL;
        	}

        	sfieldreplace(pstNode->m_psvData, '\t', ' ');
        	if(!strlen(pstNode->m_psvData)) continue;

    		memset(&stDomain, 0, sizeof(TDomain));
            memset(&pstIndex[j - 1], 0, sizeof(TIndex));
            for(i = 0, n = lfieldnum(pstNode->m_psvData, " "); i < n; i ++)
            {
                memset(szAttr, 0, sizeof(szAttr));
                strncpy(szAttr, sfieldvalue(pstNode->m_psvData, " ",  i + 1), sizeof(szAttr));

                if(!strstr(szAttr, "="))
                {
                    fprintf(stdout, "%s\n*缺失=\n", pstNode->m_psvData);
                    goto PDOMAIN_ERROR;
                }

                memset(szTarg, 0, sizeof(szTarg));
                memset(szValue, 0, sizeof(szValue));
                strncpy(szTarg, sgetvalue(szAttr, "=", 1), sizeof(szTarg));
                strncpy(szValue, sgetvalue(szAttr, "=", 2), sizeof(szValue));
                srtrim(szTarg);
                sltrim(szValue);
                strimabout(szValue, "\"", "\"");
                if(!strlen(szValue))
                {
                    fprintf(stdout, "%s\n*配置错误, 未设置取值\n", pstNode->m_psvData);
                    goto PDOMAIN_ERROR;
                }

                if(!strcasecmp(szTarg, "TABLE"))
                    pstIndex[j - 1].m_table = atol(szValue);
                else if(!strcasecmp(szTarg, "TABLENAME"))
					strncpy(pstIndex[j - 1].m_szTable, szValue, sizeof(pstIndex[j - 1].m_szTable));
                else if(!strcasecmp(szTarg, "PART"))
					strncpy(pstIndex[j - 1].m_szPart, szValue, sizeof(pstIndex[j - 1].m_szPart));
                else if(!strcasecmp(szTarg, "GROUP"))
					stDomain.m_lGroup = atol(szValue);
                else if(!strcasecmp(szTarg, "TIMTOUT"))
					stDomain.m_lTimeOut = atol(szValue);
                else if(!strcasecmp(szTarg, "MAXTRY"))
					stDomain.m_lTryMax = atol(szValue);
                else if(!strcasecmp(szTarg, "KEEPALIVE"))
					stDomain.m_lKeepLive = atol(szValue);
                else
                {
                    fprintf(stdout, "%s\n*无效的参数\n", pstNode->m_psvData);
                    goto PDOMAIN_ERROR;
                }
            }

        	if(pstIndex[j - 1].m_table <= 0)
        	{
            	fprintf(stdout, "%s\n*表设置错误\n", pstNode->m_psvData);
                goto PDOMAIN_ERROR;
        	}

			if(!strlen(pstIndex[j - 1].m_szTable))
        	{
            	fprintf(stdout, "%s\n*表名未设置\n", pstNode->m_psvData);
                goto PDOMAIN_ERROR;
        	}

			stDomain.m_table = pstIndex[j - 1].m_table;
			strncpy(stDomain.m_szTable, pstIndex[j - 1].m_szTable, sizeof(stDomain.m_szTable));
			strncpy(pstIndex[j - 1].m_szOwner, pstBoot->m_szNode, sizeof(pstIndex[j - 1].m_szOwner));
			if(!strlen(pstIndex[j - 1].m_szPart))
				strncpy(pstIndex[j - 1].m_szPart, pstBoot->m_szNode, sizeof(pstIndex[j - 1].m_szPart));
			strncpy(stDomain.m_szPart, pstIndex[j - 1].m_szPart, sizeof(stDomain.m_szPart));
			if(stDomain.m_lGroup <= 0)
				stDomain.m_lGroup = j;
			if(stDomain.m_lTimeOut <= 0)
				stDomain.m_lTimeOut = 5;
			if(stDomain.m_lTryMax <= 0)
				stDomain.m_lTryMax = 3;
			if(stDomain.m_lKeepLive <= 0)
				stDomain.m_lKeepLive = 30;
		}
		else if(!strncmp(pstNode->m_psvData, "DOMAINID", 8))
		{
        	if(NULL == (pstDomain = (TDomain *)realloc(pstDomain, (++ m) * sizeof(TDomain))))
        	{
            	pstSavm->m_lErrno = MALLC_MEM_ERR;
            	return RC_FAIL;
        	}

        	sfieldreplace(pstNode->m_psvData, '\t', ' ');
        	if(!strlen(pstNode->m_psvData)) continue;

            memset(&pstDomain[m - 1], 0, sizeof(TDomain));
            for(i = 0, n = lfieldnum(pstNode->m_psvData, " "); i < n; i ++)
            {
                memset(szAttr, 0, sizeof(szAttr));
                strncpy(szAttr, sfieldvalue(pstNode->m_psvData, " ",  i + 1), sizeof(szAttr));

                if(!strstr(szAttr, "="))
                {
                    fprintf(stdout, "%s\n*缺失=\n", pstNode->m_psvData);
                    goto PDOMAIN_ERROR;
                }

                memset(szTarg, 0, sizeof(szTarg));
                memset(szValue, 0, sizeof(szValue));
                strncpy(szTarg, sgetvalue(szAttr, "=", 1), sizeof(szTarg));
                strncpy(szValue, sgetvalue(szAttr, "=", 2), sizeof(szValue));
                srtrim(szTarg);
                sltrim(szValue);
                strimabout(szValue, "\"", "\"");
                if(!strlen(szValue))
                {
                    fprintf(stdout, "%s\n*配置错误, 未设置取值\n", pstNode->m_psvData);
                    goto PDOMAIN_ERROR;
                }

                if(!strcasecmp(szTarg, "DOMAINID"))
					strncpy(pstDomain[m - 1].m_szOwner, szValue, sizeof(pstDomain[m - 1].m_szOwner));
                else if(!strcasecmp(szTarg, "WSADDR"))
				{
					strncpy(pstDomain[m - 1].m_szIp, sgetvalue(szValue, ":", 1), sizeof(pstDomain[m - 1].m_szIp));
					pstDomain[m - 1].m_lRDPort = atol(sgetvalue(szValue, ":", 2));
				}
                else
                {
                    fprintf(stdout, "%s\n*无效的参数\n", pstNode->m_psvData);
                    goto PDOMAIN_ERROR;
                }
			}

			if(!strlen(pstDomain[m - 1].m_szOwner))
        	{
            	fprintf(stdout, "%s\n*域名未设置\n", pstNode->m_psvData);
                goto PDOMAIN_ERROR;
        	}

			if(!strlen(pstDomain[m - 1].m_szIp))
        	{
            	fprintf(stdout, "%s\n*域地址未设置\n", pstNode->m_psvData);
                goto PDOMAIN_ERROR;
        	}

			if(pstDomain[m - 1].m_lRDPort <= 0)
        	{
            	fprintf(stdout, "%s\n*域端口设置错误或未设置\n", pstNode->m_psvData);
                goto PDOMAIN_ERROR;
        	}

			if(stDomain.m_table <= 0)
			{
				pstDomain[m - 1].m_table  = SYS_TVM_INDEX;
				strcpy(pstDomain[m - 1].m_szPart, pstDomain[m - 1].m_szOwner);
				strcpy(pstDomain[m - 1].m_szTable, "SYS_TVM_INDEX");
			}
			else
			{
				pstDomain[m - 1].m_table  = stDomain.m_table;
				strncpy(pstDomain[m - 1].m_szTable, stDomain.m_szTable, sizeof(pstDomain[m - 1].m_szTable));
				if(!strlen(stDomain.m_szPart))
					strcpy(pstDomain[m - 1].m_szPart, pstDomain[m - 1].m_szOwner);
				else
					strcpy(pstDomain[m - 1].m_szPart, stDomain.m_szPart);
			}

			pstDomain[m - 1].m_lLIPort  = pstBoot->m_lSvrPort;
			pstDomain[m - 1].m_lLDPort  = pstBoot->m_lDomPort;
			pstDomain[m - 1].m_lGroup   = stDomain.m_lGroup > 0 ? stDomain.m_lGroup : m;
			pstDomain[m - 1].m_lTimeOut = stDomain.m_lTimeOut > 0 ? stDomain.m_lTimeOut : m;
			pstDomain[m - 1].m_lTryMax  = stDomain.m_lTryMax > 0 ? stDomain.m_lTryMax : m;
			pstDomain[m - 1].m_lKeepLive= stDomain.m_lKeepLive > 0 ? stDomain.m_lKeepLive : m;
		}
		else
		{
			fprintf(stdout, "%s\n*无效的参数\n", pstNode->m_psvData);
			goto PDOMAIN_ERROR;
		}
    }
    *plOut = j;
    *ppstIndex = pstIndex;
    *plCout = m;
    *ppstDomain = pstDomain;

    vDestroyList(pstRoot);
    return RC_SUCC;

PDOMAIN_ERROR:
    IBPFree(pstIndex);
    IBPFree(pstDomain);
    vDestroyList(pstRoot);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：bDomIsRepeat(long lCount, TDomain *pstDomain)
    功能描述：检查唯一性
    返回说明：
        无
 **************************************************************************************************/
BOOL	bDomIsRepeat(long lCount, TDomain *pstDomain)
{
	int		i, j;

    for(i = 0; i < lCount; i ++)
    {
		for(j = 0; j < lCount; j ++)
		{
			if(i == j)	continue;

			if(pstDomain[i].m_lRDPort == pstDomain[j].m_lRDPort && 
				!strcmp(pstDomain[i].m_szIp, pstDomain[j].m_szIp) &&
				!strcmp(pstDomain[i].m_szPart, pstDomain[j].m_szPart) &&
				!strcmp(pstDomain[i].m_szTable, pstDomain[j].m_szTable))
			{
				if(SYS_TVM_INDEX == pstDomain[i].m_table)
				{
					fprintf(stderr, "*域(%s)(%s:%d)存在重复\n", pstDomain[i].m_szOwner, 
						pstDomain[i].m_szIp, pstDomain[i].m_lRDPort);
				}
				else
				{
					fprintf(stderr, "*配置表(%s)(%s)存在重复\n", pstDomain[i].m_szTable, 
						pstDomain[i].m_szPart);
				}
				return true;
			}
		}
	}

	return false;
}

/**************************************************************************************************
    函 数 名：main
    功能描述：cts操作功能说明
    返回说明：
        无
 **************************************************************************************************/
long	lMakeConfig(char *pszFile)
{
    TBoot   stBoot;
	FILE	*fp = NULL;
    TIndex  *pstIndex = NULL;
	TDomain	*pstDomain = NULL;
    long    i, lOut = 0, lCount = 0;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

	if(!pszFile || !strlen(pszFile))
	{
		fprintf(stderr, "配置文件输入错误\n");
		return RC_FAIL;
	}

	if(RC_SUCC != access(pszFile, R_OK | F_OK ))
	{
		fprintf(stderr, "当前无权限读取文件(%s), 请确认!!\n\n", pszFile);
		return RC_FAIL;
	}

    memset(&stBoot, 0, sizeof(TBoot));
	if(NULL == (fp = fopen(getenv("TVMCFG"), "wb")))
	{
		fprintf(stderr, "写入配置文件(%s)失败, err:(%d)(%s)", getenv("TVMCFG"), 
			errno, strerror(errno));
		return RC_FAIL;
	}

    fwrite(TVM_RUNCFG_TAG, 4, 1, fp);
    if(RC_SUCC != lParseBoot(pstSavm, pszFile, &stBoot))
		goto CREATE_ERROR;

    fwrite(&stBoot, sizeof(stBoot), 1, fp);
	if(TVM_BOOT_CLUSTER != stBoot.m_lBootType)	//	单机部署
	{
		fclose(fp);
		fprintf(stdout, "创建成功，completed successfully!!!\n");
		return RC_FAIL;
	}

    if(RC_SUCC != lParseIndex(pstSavm, pszFile, &lOut, &pstIndex))
		goto CREATE_ERROR;

	fwrite(&lOut, sizeof(long), 1, fp);
	fwrite(pstIndex, sizeof(TIndex), lOut, fp);	
    IBPFree(pstIndex);

	if(RC_SUCC != lParseDomain(pstSavm, pszFile, &stBoot, &lOut, &pstIndex, 
		&lCount, &pstDomain))
		goto CREATE_ERROR;

	//	检查是否重复 (m_szTable, szPart, IP, RDPort)
	if(bDomIsRepeat(lCount, pstDomain))
		goto CREATE_ERROR;

	fwrite(&lOut, sizeof(long), 1, fp);
	fwrite(pstIndex, sizeof(TIndex), lOut, fp);	
    IBPFree(pstIndex);
	fwrite(&lCount, sizeof(long), 1, fp);
	fwrite(pstDomain, sizeof(TDomain), lCount, fp);
    IBPFree(pstDomain);
	fclose(fp);

	fprintf(stdout, "创建成功，completed successfully!!!\n");

	return RC_SUCC;

CREATE_ERROR:
    IBPFree(pstDomain);
   	IBPFree(pstIndex);
	fclose(fp);
	return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：lUnmakeConfig(char *pszFile)
    功能描述：导出配置
    返回说明：
        无
 **************************************************************************************************/
long	lUnmakeConfig(char *pszFile)
{
	char	ch;
    TBoot   stBoot;
	FILE	*fp = NULL;
	BOOL	bf = false;
    TIndex  *pstIndex = NULL;
	TDomain	*pstDomain = NULL;
    long    i, j, lOut = 0, lCount = 0;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

	if(RC_SUCC == access(pszFile, F_OK))
	{
		fprintf(stderr, "当前已存在配置(%s)是否覆盖(Y/N)?:", pszFile);
		ch = getchar();
		if(ch != 'y' && 'Y' != ch)
			return RC_SUCC;
	}

	if(NULL == (fp = fopen(pszFile, "w")))
	{
		fprintf(stderr, "out file is null!\n");
		return RC_FAIL;
	}

    if(RC_SUCC != lGetBootConfig(pstSavm, &stBoot))
		goto UNMAKE_ERR;

	fprintf(fp, "*GLOBLE\n");
	fprintf(fp, "MACHINE=\"%s\"\n", stBoot.m_szNode);
	fprintf(fp, "MAXTABLE=%d\n", stBoot.m_lMaxTable);
	fprintf(fp, "MAXFILED=%d\n", stBoot.m_lMaxField);
	fprintf(fp, "MAXDOMAIN=%d\n", stBoot.m_lMaxDomain);
	fprintf(fp, "MAXSEQUE=%d\n", stBoot.m_lMaxSeque);
	fprintf(fp, "SERVER_EXEC=%d\n", stBoot.m_lSvrBoot);
	fprintf(fp, "SERVER_PORT=%d\n", stBoot.m_lSvrPort);
	fprintf(fp, "DOMAIN_PORT=%d\n", stBoot.m_lDomPort);
	fprintf(fp, "EXTERN=%d\n", stBoot.m_lExtern);
	fprintf(fp, "LOGNAME=\"%s\"\n\n",  stBoot.m_szLog);

	fprintf(fp, "*LOCAL_RESOURCE\n");
    if(RC_SUCC != lGetLocalIndex(pstSavm, &lCount, (void *)&pstIndex))
		goto UNMAKE_ERR;

	for(i = 0; i < lCount; i ++)
		fprintf(fp, "TABLE=%d PERMIT=%d\n", pstIndex[i].m_table, pstIndex[i].m_lPers);
	IBPFree(pstIndex);

	fprintf(fp, "\n*REMOTE_DOMAIN");
    if(RC_SUCC != lGetDomainIndex(pstSavm, &lCount, &pstIndex))
		goto UNMAKE_ERR;

    if(RC_SUCC != lGetDomainTable(pstSavm, &lOut, &pstDomain))
		goto UNMAKE_ERR;
	
	for(i = 0; i < lCount; i ++)
	{
		fprintf(fp, "\nTABLE=%d TABLENAME=\"%s\" PART=\"%s\" ", pstIndex[i].m_table, 
			pstIndex[i].m_szTable, pstIndex[i].m_szPart);
		for(j = 0, bf = false; j < lOut; j ++)
		{
			if(strcmp(pstDomain[j].m_szTable, pstIndex[i].m_szTable) || 	
				strcmp(pstDomain[j].m_szPart, pstIndex[i].m_szPart))
				continue;

			if(!bf)
			{
				bf = !bf;
				fprintf(fp, "GROUP=%d TIMTOUT=%d MAXTRY=%d KEEPALIVE=%d\n", 
					pstDomain[j].m_lGroup, pstDomain[j].m_lTimeOut, pstDomain[j].m_lTryMax, 
					pstDomain[j].m_lKeepLive);
			}

			fprintf(fp, "\tDOMAINID=\"%s\" WSADDR=\"%s:%d\"\n", pstDomain[j].m_szOwner, 
				pstDomain[j].m_szIp, pstDomain[j].m_lRDPort);
		}
	}
	IBPFree(pstIndex);
	IBPFree(pstDomain);

	fclose(fp);
	fprintf(stdout, "导出文件(%s)成功，completed successfully!!!\n", pszFile);

	return RC_SUCC;
UNMAKE_ERR:
	fclose(fp);
	IBPFree(pstIndex);
	IBPFree(pstDomain);
   	fprintf(stderr, "get config err:(%d)(%s)\n", lGetTErrno(), sGetTError(pstSavm->m_lErrno));
	return RC_FAIL;
}

int	main(int argc, char *argv[])
{
	char	szCom[256];
    int     iChoose = 0, iAction = 0;

	if(!getenv("TVMCFG") && !getenv("TVMDBD"))
    {
        fprintf(stderr, "环境变量(TVMCFG)未配置, 无法使用\n");
        return RC_FAIL;
    }

	memset(szCom, 0, sizeof(szCom));
    while(-1 != (iChoose = getopt(argc, argv, "c:o:v?::")))
    {
        switch(iChoose)
        {
        case    'c':
			return lMakeConfig(optarg);
        case    'o':
			return lUnmakeConfig(optarg);
        case    '?':
        default:
            break;
		}
	}

	fprintf(stdout, "\nUsage:\t%s -[co]\n", basename(argv[0]));
	fprintf(stdout, "\t-c[file]\t--编译配置文件转换TVM运行参数\n");
	fprintf(stdout, "\t-o[file]\t--反编译运行参数导出配置文件\n");
	fprintf(stdout, "\n");

	return RC_SUCC;
}

/**************************************************************************************************
 * 	code end
 **************************************************************************************************/
