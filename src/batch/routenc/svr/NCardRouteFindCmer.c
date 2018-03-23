/**
 * desc: 源码负责寻找通道以及通道可用商户
 */
#include "cust/cust.h"
#include "model.h"

#define ROUTE_NORL_PAY "1"
#define ROUTE_WCHT_PAY "41"
#define ROUTE_ALPY_PAY "42" 

typedef struct
{
    char sPayType    [8  + 1];
    char sPayTypeDesc[16 + 1];
} PayType_t;

static PayType_t gsPayTypes[] =
{
    {ROUTE_NORL_PAY,  "Normal"},
    {ROUTE_ALPY_PAY,  "Alpy"  },
    {ROUTE_WCHT_PAY,  "Wcht"  },
};

static const char *sGetPayDescById(const char *type)
{
    int ii;

    for (ii = 0; ii < sizeof(gsPayTypes) / sizeof(PayType_t); ii ++)
    {
        if (strlen(gsPayTypes[ii].sPayType) == strlen(type)  &&
            !memcmp(type, gsPayTypes[ii].sPayType, strlen(gsPayTypes[ii].sPayType)))
        {
            break;
        }
    }

    if (ii >= sizeof(gsPayTypes) / sizeof(PayType_t))
    {
        /* 如果未找到描述则只返回报文上送的值 */
        return type;
    }

    return gsPayTypes[ii].sPayTypeDesc;
}

static void vGetLocalDate(char *ptLocDt)
{
    time_t current;
    struct tm *tmCurrentTime;

    time(&current);
    tmCurrentTime = localtime(&current);
    sprintf(ptLocDt, "%4d%02d%02d", tmCurrentTime->tm_year + 1900,
            tmCurrentTime->tm_mon + 1, tmCurrentTime->tm_mday);
    return;
}

static int iCheckExceedCmerTotalAmt(T_TOP_DBS_STMT dbSpecMcht)
{
    char sTransType[2 + 1] = {0};
    char sTransAmt [43 + 1] = {0};
    tbl_r_cmer_record_def stTblRCmerRecord;

    //IBPgetstring("transType", sTransType, "交易类型");
    IBPgetstring("cmd_type", sTransType, "交易类型");
    //IBPgetstring("transAmt", sTransAmt, "交易金额");
    IBPgetstring("amount", sTransAmt, "交易金额");

    /* 检查是否超过通道商户的最大限额 */
    if (atol((char *) dbsGetStringByName(dbSpecMcht, "max_total_amout")) > 0 &&
        memcmp(sTransType, "30", 2))
    {

        setnull(stTblRCmerRecord);

        /* 日期 */
        vGetLocalDate(stTblRCmerRecord.tx_date);
        /* 通道商户号 */
        memcpy(stTblRCmerRecord.mcht_no,
               (char *) dbsGetStringByName(dbSpecMcht, "channel_mcht_no"),
               sizeof(stTblRCmerRecord.mcht_no) - 1);

        DbsStatTBL_R_CMER_RECORD(DBS_STAT1, &stTblRCmerRecord, NULL, 0);

        if (stTblRCmerRecord.csm_amount + atof(sTransAmt) >
            atof((char *) dbsGetStringByName(dbSpecMcht, "max_total_amout")))
        {

            return 1;

        }
    }

    return 0;
}

static int iAnalyseMatchCmerByLocMod(const char *ptChaModId)
{
    /* 通过本地模型关联通道模型找到满足条件的通道商户 */

    T_TOP_DBS_STMT tdb;
    int iRet = -1;

    iRet = dbsOpenStatement(&tdb,
                            "SELECT "\
                            "rowid, "\
                            "id, "\
                            "name, "\
                            "status, "\
                            "priority, "\
                            "channel_nm, "\
                            "channel_name_cn, "\
                            "oprate, "\
                            "resv, "\
                            "remark, "\
                            "create_tm, "\
                            "create_id, "\
                            "lupd_tm, "\
                            "lupd_id "\
                            "FROM tbl_r_cha_mod "\
                            "WHERE "\
                            "status = '0' and id in "\
                            "(select channel_model_id from tbl_r_lmer_cha "\
                            " where local_mcht_model_id = :tsId) order by priority desc", ptChaModId);

    if (iRet)
    {
        IBPwarn("查询表[tbl_r_cha_mod]错误, [%d][%s]", iRet, dbsErrStr());
        return -1;
    }

    while (dbsNext(tdb) == 0)
    {

        /* 通道条件匹配 (need function) */
        iRet = model_judge(CHA_CON, atoi((char *)dbsGetStringByName(tdb, "id")));

        if (iRet)
        {
            continue;
        }

#if 0
        else if (1 == 1)
        {

            /* 满足通道关停  */
            /* 无需判断成功与否 */
            dbsExecuteV("update tbl_r_cha_mod "\
                        "set status = :status "\
                        "where id = :id ", "2",
                        (char *)dbsGetStringByName(tdb, "rowid"));
            lDbsCommit();
        }

#endif
        else
        {
            /* 找到可用通道, 继续寻找可用通道商户 */
            char sLocMerNo[20 + 1];
            char sChaMerNo[40 + 1];
            char sXntdFlag[1  + 1];
            char sChaZsnm [16 + 1];

            setnull(sLocMerNo);
            setnull(sChaMerNo);
            setnull(sXntdFlag);
            setnull(sChaZsnm);
            //IBPgetstring("mchtNo", sLocMerNo, "本地商户号");
            IBPgetstring("mcht_no", sLocMerNo, "本地商户号");

            iRet = dbsExecuteV(
                       "SELECT "\
                       "a.channel_mcht_no as channel_mcht_no, "\
                       "b.xntd_flag as xntd_flag, "\
                       "b.cha_zsnm as cha_zsnm "\
                       "FROM tbl_r_sync_mcht a, tbl_r_cha_param b "\
                       "WHERE a.status = '0' "\
                       "and b.status = '0' "\
                       "and a.channel_nm = b.cha_nm "\
                       "and a.local_mcht_no = :local_mcht_no "\
                       "and a.channel_nm = :channel_nm",
                       sLocMerNo, (char *) dbsGetStringByName(tdb, "channel_nm"),
                       sChaMerNo, sXntdFlag, sChaZsnm);

            if (iRet)
            {
                IBPwarn("本地商户号[%s], 通道[%s]在表[tbl_r_sync_mcht]中未找到可用商户号, [%d][%s]",
                        sLocMerNo, (char *) dbsGetStringByName(tdb, "channel_nm"), iRet, dbsErrStr());
                continue;
            }


            /* 模型ID */
            IBPputstring("modId", (char *) dbsGetStringByName(tdb, "id"));

            /* 通道商户 */
            IBPputstring("chaMerNo", sChaMerNo);

            if (memcmp(sXntdFlag, "1", 1) == 0)
            {
                /* 虚拟通道 */
                IBPputstring("xntdFlag", "1");
                /* 真实通道名称 */
                IBPputstring("chazsName", sChaZsnm);
            }
            else
            {
                IBPputstring("xntdFlag", "0");

                IBPputstring("chazsName", (char *) dbsGetStringByName(tdb, "channel_nm"));
            }

            /* 通道名称 */
            IBPputstring("channelName", (char *) dbsGetStringByName(tdb, "channel_nm"));

            /* FIXME 可以根据数据库中的某个字控制返回信息 */
            if (memcmp((char *) dbsGetStringByName(tdb, "channel_nm"), "mspa", 4) == 0 ||
                memcmp((char *) dbsGetStringByName(tdb, "channel_nm"), "mspb", 4) == 0)
            {
                IBPputstring("channelName", (char *) dbsGetStringByName(tdb, "cha_zsnm"));
            }

            if (memcmp((char *) dbsGetStringByName(tdb, "channel_nm"), "wfta", 4) == 0 ||
                memcmp((char *) dbsGetStringByName(tdb, "channel_nm"), "wftb", 4) == 0)
            {
                IBPputstring("channelName", (char *) dbsGetStringByName(tdb, "cha_zsnm"));
            }

            /* 如果找到则停止搜索 */
            dbsCloseStatement(tdb);
            return 0;
        }
    }

    IBPrace("实时规则筛选没有满足本地商户模型ID[%s]的筛选规则通道可用", ptChaModId);
    dbsCloseStatement(tdb);
    return -1;
}

static int iFindRDirectStlCmer()
{
    /* 直清通道商户筛选(规则筛选) */
    T_TOP_DBS_STMT tdb;
    char sLocalMchtNo[20 + 1];
    int  iRet = -1;

    setnull(sLocalMchtNo);
    //IBPgetstring("mchtNo", sLocalMchtNo, "本地商户号");
    IBPgetstring("mcht_no", sLocalMchtNo, "本地商户号");

    /* where local_mcht_no */
    iRet = dbsOpenStatement(&tdb,
                            "SELECT "\
                            "t1.rowid as rid, "\
                            "t1.id as id, "\
                            "t1.status as status, "\
                            "t1.local_mcht_no as local_mcht_no, "\
                            "t1.local_mcht_nm as local_mcht_nm, "\
                            "t1.channel_nm as channel_nm, "\
                            "t1.channel_mcht_no as channel_mcht_no, "\
                            "t1.channel_mcht_nm as channel_mcht_nm, "\
                            "t1.rate as rate, "\
                            "t1.max_fee as max_fee, "\
                            "t1.min_fee as min_fee, "\
                            "t1.scan_mode as scan_mode, "\
                            "t1.credit_supp as credit_supp, "\
                            "t1.region_code as region_code, "\
                            "t1.max_total_amout as max_total_amout, "\
                            "t1.resv as resv, "\
                            "t1.remark as remark, "\
                            "t1.create_tm as create_tm, "\
                            "t1.create_id as create_id, "\
                            "t1.lupd_tm as lupd_tm, "\
                            "t1.lupd_id as lupd_id, "\
                            "t1.priority as priority, "\
                            "t2.id as chn_id, "\
                            "t2.cha_zsnm as cha_zsnm, "\
                            "t2.xntd_flag as xntd_flag "
                            "FROM tbl_r_special_mcht  t1, tbl_r_cha_param t2 "\
                            "WHERE t1.status = t2.status "\
                            "and t1.status = :status "
                            "and t1.local_mcht_no = :local_mcht_no "\
                            "and t1.channel_nm = t2.cha_nm "\
                            "order by t2.priority desc", "0", sLocalMchtNo);

    if (iRet)
    {
        IBPwarn("查询表[tbl_r_special_mcht]错误, [%d][%s]", iRet, dbsErrStr());
        return 1;
    }

    while (dbsNext(tdb) == 0)
    {

        if (strlen(dbsGetStringByName(tdb, "channel_mcht_no")) == 0)
        {
            IBPwarn("配置错误, 没有指定通道商户号, ID[%s], 本地商户号[%s]",
                    (char *)dbsGetStringByName(tdb, "id"),
                    (char *)dbsGetStringByName(tdb, "local_mcht_no"));
            continue;
        }


        /* 检查通道条件 (need function) */
        iRet = model_judge(CHA_CON, atoi((char *)dbsGetStringByName(tdb, "chn_id")));

        if (iRet)
        {
            continue;
        }

#if 0
        else if (1 == 1)
        {
            /* 满足通道关停  */

            /* 无需判断成功与否 */
            dbsExecuteV("update tbl_r_cha_param "\
                        "set status = :status "\
                        "where id = :id ", "2",
                        (char *)dbsGetStringByName(tdb, "chn_id"));
            lDbsCommit();
        }

#endif

        /* 检查通道商户条件 (need function) */
        iRet = model_judge(CMER_CON, atoi((char *)dbsGetStringByName(tdb, "id")));

        if (iRet)
        {
            continue;
        }

        /* 检查是否超过通道商户的最大限额 */
        if (iCheckExceedCmerTotalAmt(tdb))
        {

            IBPwarn("specical通道商户[%s]已超过当日最大额度...",
                    (char *) dbsGetStringByName(tdb, "channel_mcht_no"));
            continue;
        }

        /* 模型ID */
        IBPputstring("modId", (char *) dbsGetStringByName(tdb, "id"));

        /* 通道商户 */
        IBPputstring("chaMerNo", (char *) dbsGetStringByName(tdb, "channel_mcht_no"));

        if (memcmp((char *)dbsGetStringByName(tdb, "xntd_flag"), "1", 1) == 0)
        {
            /* 虚拟通道 */
            IBPputstring("xntdFlag", "1");
            /* 真实通道名称 */
            IBPputstring("chazsName", (char *) dbsGetStringByName(tdb, "cha_zsnm"));
        }
        else
        {
            IBPputstring("xntdFlag", "0");

            IBPputstring("chazsName", (char *) dbsGetStringByName(tdb, "channel_nm"));
        }

        /* 通道名称 */
        IBPputstring("channelName", (char *) dbsGetStringByName(tdb, "channel_nm"));

        char sChaNm[16 + 1];
        setnull(sChaNm);

        IBPgetstring("channelName", sChaNm, "通道");

        /* FIXME 可以根据数据库中的某个字控制返回信息 */
        if (memcmp((char *) dbsGetStringByName(tdb, "channel_nm"), "mspa", 4) == 0 ||
            memcmp((char *) dbsGetStringByName(tdb, "channel_nm"), "mspb", 4) == 0)
        {
            IBPputstring("channelName", (char *) dbsGetStringByName(tdb, "cha_zsnm"));
        }

        if (memcmp((char *) dbsGetStringByName(tdb, "channel_nm"), "wfta", 4) == 0 ||
            memcmp((char *) dbsGetStringByName(tdb, "channel_nm"), "wftb", 4) == 0)
        {
            IBPputstring("channelName", (char *) dbsGetStringByName(tdb, "cha_zsnm"));
        }

        dbsCloseStatement(tdb);
        return 0;
    }

    IBPrace("该商户不是一清商户或无可用一清直清通道");
    dbsCloseStatement(tdb);

    return 1;
}

static int iFindNRDirectStlCmer()
{
    /* 一户一码直清通道商户(无规则筛选) */
    tbl_r_cmer_mod_def stTblRCmerMod;
    char sMchtTp  [2 + 1];
    int  iRet = -1;

    setnull(stTblRCmerMod);
    setnull(sMchtTp);
    /* 判断是否是直清商户 */
    //IBPgetstring("mchtType", sMchtTp, "商户清算方式");
    IBPgetstring("mcht_type", sMchtTp, "商户类型");

    if (memcmp(sMchtTp, "1", 1) == 0)
    {
        memcpy(stTblRCmerMod.status, "0", 1);
        //IBPgetstring("mchtNo", stTblRCmerMod.direct_mcht_no, "本地商户号");
        IBPgetstring("mcht_no", stTblRCmerMod.direct_mcht_no, "本地商户号");
        /* where direct_mcht_no */
        iRet = DbsTBL_R_CMER_MOD(DBS_SELECT1, &stTblRCmerMod);

        if (iRet)
        {
            IBPwarn("未找到一户一码直清通道商户表[tbl_r_cmer_mod], 商户号[%s][%d][%s]",
                    stTblRCmerMod.direct_mcht_no, iRet, dbsErrStr());
        }
        else
        {
            /* 不继续查找 */

            /* 通道名称 */
            IBPputstring("channelName", stTblRCmerMod.channel_nm);

            /* 真实通道名称 */
            IBPputstring("chazsName", stTblRCmerMod.cha_zsnm);

            /* 虚拟通道标志 */
            if (strlen(stTblRCmerMod.channel_nm) == strlen(stTblRCmerMod.cha_zsnm) &&
                !memcmp(stTblRCmerMod.channel_nm, stTblRCmerMod.cha_zsnm, strlen(stTblRCmerMod.cha_zsnm)))
            {

                IBPputstring("xntdFlag", "0");
            }
            else
            {
                /* 虚拟通道 */
                IBPputstring("xntdFlag", "1");
            }

            /* 通道商户 */
            IBPputstring("chaMerNo", stTblRCmerMod.mcht_no);

            /* 模型ID */
            IBPputstring("modId", "1");

            return 0;
        }
    }

    return 1;
}

static int iFindRProxyStlCmer()
{
    /* 筛选代理清算规则 */
    tbl_r_lmer_mod_def stTblRLmerMod;
    int iRet = -1;

    setnull(stTblRLmerMod);
    memcpy(stTblRLmerMod.status, "0", 1);

    /* where status order by priority */
    iRet = DbsTBL_R_LMER_MOD(DBS_OPEN1, &stTblRLmerMod);

    if (iRet)
    {
        IBPwarn("查询表[tbl_r_lmer_mod]错误, [%d][%s]", iRet, dbsErrStr());
        return 1;
    }

    while (1)
    {
        setnull(stTblRLmerMod);
        iRet = DbsTBL_R_LMER_MOD(DBS_FETCH1, &stTblRLmerMod);

        if (iRet && iRet != 1403)
        {
            IBPwarn("获取表[tbl_r_lmer_mod]数据错误, [%d][%s]", iRet, dbsErrStr());
            break;
        }
        else if (iRet == 1403)
        {
            IBPwarn("全表扫描完毕, 未找到匹配数据");
            break;
        }

        /* 检查本地模型是否匹配 (need function) */
        iRet = model_judge(LMER_CON, stTblRLmerMod.id);

        if (iRet)
        {
            continue;
        }
        else
        {
            /* 继续寻找通道模型 */

            char sModId[43 + 1];
            setnull(sModId);
            snprintf(sModId, sizeof(sModId), "%ld", stTblRLmerMod.id);

            if (!iAnalyseMatchCmerByLocMod(sModId))
            {
                /* 条件满足终止搜索 */
                DbsTBL_R_LMER_MOD(DBS_CLOSE1, &stTblRLmerMod);
                return 0;
            }
        }

    }

    DbsTBL_R_LMER_MOD(DBS_CLOSE1, &stTblRLmerMod);
    IBPwarn("未找到任何可用通道商户");
    return 1;
}

static void vSetErrorCode()
{
	char sTransTp [4 + 1] = {0};
	char sScanMode[4 + 1] = {0};

	IBPgetstring("cmd_type", sTransTp, "交易类型");
	IBPgetstring("scan_mode", sScanMode, "扫码方式");

    if (atoi(sTransTp) == atoi(ROUTE_WCHT_PAY)) {
		/* 微信 */
        if (sScanMode[0] == '1') {
			/* 主扫 */
			IBPputstring("respCode", "0001");
        } else if (sScanMode[0] == '2') {
			/* 被扫 */
			IBPputstring("respCode", "0002");
        } else if (sScanMode[0] == '4') {
			/* 公众号 */
			IBPputstring("respCode", "0003");
        }
    } else if (atoi(sTransTp) == atoi(ROUTE_ALPY_PAY)) {
		/* 支付宝 */
        if (sScanMode[0] == '1') {
			/* 主扫 */
			IBPputstring("respCode", "0004");
        } else if (sScanMode[0] == '2') {
			/* 被扫 */
			IBPputstring("respCode", "0005");
        } else if (sScanMode[0] == '4') {
			/* 公众号 */
			IBPputstring("respCode", "0006");
        }
    }

    return;
}

int iNCardRouteFindCmerPro()
{

    char sPayType[16 + 1];

    setnull(sPayType);

    //IBPgetstring("transType", sPayType, "交易类型");
    IBPgetstring("cmd_type", sPayType, "交易类型");

    IBPrace("************************** 无卡通道商户规则匹配 开始 ******************************");

    IBPrace("支付类型==>[%s]", sGetPayDescById(sPayType));
    //IBPPrint("订单号[%s],  商户号[%s], 交易金额[%s], 商户类型[%s], 证件类型[%s], 扫码方式[%s]",
    //       "orderNo", "mchtNo", "transAmt", "mchtType", "licenseType", "scanMode");
    IBPPrint("订单号[%s],  商户号[%s], 交易金额[%s], 商户类型[%s], 证件类型[%s], 扫码方式[%s], 交易来源[%s]",
			"orderNo", "mcht_no", "amount", "mcht_type", "licen_type", "scan_mode", "fill");

    /* 一户一码直清规则(无规则筛选) */
    if (!iFindNRDirectStlCmer())
    {
        goto SUCCESS;
    }

    /* 直清规则筛选 */
    if (!iFindRDirectStlCmer())
    {
        goto SUCCESS;
    }

    /* 代理清算筛选(实时筛选) */
    if (!iFindRProxyStlCmer())
    {
        goto SUCCESS;
    }

    IBPwarn("没有找到合适通道及商户");
ERROR:
    //IBPputstring("respCode", "9999");
    /* 设置错误应答码 */
	vSetErrorCode();
    lDbsRollback();
    IBPrace("************************** 无卡通道商户规则匹配 异常 ******************************");
    return -1;
SUCCESS:
    IBPputstring("respCode", "0000");
    IBPPrint("找到一户一码直清商户信息, 通道商户号[%s],真实通道[%s],通道[%s], 模型ID[%s]",
             "chaMerNo", "chazsName", "channelName", "modId");

    IBPrace("************************** 无卡通道商户规则匹配 结束 ******************************");
    lDbsCommit();
    return 0;
}
