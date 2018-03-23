/**
 * desc: 源码负责无卡交易流水汇总统计
 * note: 目前无卡交易流水表的唯一索引为订单号
 */
#include "cust/cust.h"

static iCheckOriTxn(tbl_r_txn_def *ptOriTblRTxn)
{
    tbl_r_txn_def stTblRTxn;
    int iRet = -1;

    setnull(stTblRTxn);

    /* 订单号 */
    IBPgetstring("orderNo", stTblRTxn.order_no, "订单号");
    /* where order_no */
    iRet = DbsTBL_R_TXN(DBS_SELECT1, &stTblRTxn);

    if (!iRet)
    {
        memcpy(ptOriTblRTxn->cha_nm, stTblRTxn.cha_nm, sizeof(ptOriTblRTxn->cha_nm) - 1);
        memcpy(ptOriTblRTxn->cha_modid, stTblRTxn.cha_modid, sizeof(ptOriTblRTxn->cha_modid) - 1);
        memcpy(ptOriTblRTxn->cha_merno, stTblRTxn.cha_merno, sizeof(ptOriTblRTxn->cha_merno) - 1);
        memcpy(ptOriTblRTxn->chld_merno, stTblRTxn.chld_merno, sizeof(ptOriTblRTxn->chld_merno) - 1);
        memcpy(ptOriTblRTxn->tx_code, stTblRTxn.tx_code, sizeof(ptOriTblRTxn->tx_code) - 1);
        ptOriTblRTxn->tx_amt = stTblRTxn.tx_amt;
        memcpy(ptOriTblRTxn->cha_zsnm, stTblRTxn.cha_zsnm, sizeof(ptOriTblRTxn->cha_zsnm) - 1);
    }
    else
    {
        IBPwarn("未找到原交易表[tbl_r_txn], 请检查订单号[%s][%d][%s]",  stTblRTxn.order_no, iRet, dbsErrStr());
        return -1;
    }

    return 0;
}

static iUpdateTxnStat(tbl_r_txn_def *ptOriTblRTxn)
{
    int iRet = -1;
    /* where order_no */
    iRet = DbsTBL_R_TXN(DBS_UPDATE1, ptOriTblRTxn);

    if (iRet)
    {
        IBPwarn("原交易状态更新失败表[tbl_r_txn]，订单号[%s][%d][%s]",  ptOriTblRTxn->order_no, iRet, dbsErrStr());
        return -1;
    }

    return 0;
}

static iStatMchtRecord(tbl_r_txn_def *ptTblTxn)
{
    tbl_r_cmer_record_def stTblRCmerRecord;
    int iRet = -1;

    IBPrace("交易日期==>[%s], 商户号==>[%s], 通道==>[%s], 模型ID==>[%s]", \
            ptTblTxn->tx_date, ptTblTxn->cha_merno, ptTblTxn->cha_nm, ptTblTxn->cha_modid);
    setnull(stTblRCmerRecord);

    /* 交易日期 */
    memcpy(stTblRCmerRecord.tx_date, ptTblTxn->tx_date, sizeof(stTblRCmerRecord.tx_date) - 1);
    /* 通道商户号 */
    memcpy(stTblRCmerRecord.mcht_no, ptTblTxn->cha_merno, sizeof(stTblRCmerRecord.mcht_no) - 1);
    /* 通道 */
    memcpy(stTblRCmerRecord.cha_nm, ptTblTxn->cha_nm, sizeof(stTblRCmerRecord.cha_nm) - 1);
    /* 通道模型ID */
    memcpy(stTblRCmerRecord.cha_modid, ptTblTxn->cha_modid, sizeof(stTblRCmerRecord.cha_modid) - 1);

    /* where tx_date && mcht_no && cha_modid && cha_nm */
    iRet = DbsTBL_R_CMER_RECORD(DBS_LOCK1, &stTblRCmerRecord);

    if (iRet && iRet != 1403)
    {

        IBPwarn("查询表[tbl_r_cmer_record]异常, [%d][%s]", iRet, dbsErrStr());
        return -1;
    }

    if (!iRet)
    {
        /* 记录存在 */

        char sCnt[43 + 1];
        tbl_r_txn_def stStatTblRTxn;

        setnull(sCnt);
        setnull(stStatTblRTxn);

        /* 汇总交易数据 */
        /* 交易日期 */
        memcpy(stStatTblRTxn.tx_date, ptTblTxn->tx_date, sizeof(stStatTblRTxn.tx_date) - 1);
        /* 通道商户号 */
        memcpy(stStatTblRTxn.cha_merno, ptTblTxn->cha_merno, sizeof(stStatTblRTxn.cha_merno) - 1);
        /* 通道 */
        memcpy(stStatTblRTxn.cha_nm, ptTblTxn->cha_nm, sizeof(stStatTblRTxn.cha_nm) - 1);
        /* 通道模型ID */
        memcpy(stStatTblRTxn.cha_modid, ptTblTxn->cha_modid, sizeof(stStatTblRTxn.cha_modid) - 1);
        /* 交易状态 */
        memcpy(stStatTblRTxn.stat, "0", sizeof(stStatTblRTxn.stat) - 1);

        /* where cha_merno && cha_modid && cha_nm && stat && tx_date */
        iRet = DbsStatTBL_R_TXN(DBS_STAT1, &stStatTblRTxn, sCnt, sizeof(sCnt));

        if (iRet)
        {
            IBPwarn("汇总商户当日交易数据错误表[tbl_r_txn], [%d][%s]", iRet, dbsErrStr());
            return -1;
        }

        stTblRCmerRecord.csm_num    = atoi(sCnt);
        stTblRCmerRecord.csm_amount = stStatTblRTxn.tx_amt;

        IBPrace("累加后交易笔数[%d], 交易金额[%.2f]", stTblRCmerRecord.csm_num, stTblRCmerRecord.csm_amount);
        /* where rowid */
        iRet = DbsTBL_R_CMER_RECORD(DBS_UPDATE1, &stTblRCmerRecord);

        if (iRet)
        {
            IBPwarn("更新商户当日汇总数据异常表[tbl_r_cmer_record], [%d][%s]", iRet, dbsErrStr());
            return -1;
        }

    }
    else
    {
        /* 记录不存在 */
        stTblRCmerRecord.inq_num = 0;
        stTblRCmerRecord.csm_num = 1;
        stTblRCmerRecord.csm_amount = ptTblTxn->tx_amt;

        iRet = DbsTBL_R_CMER_RECORD(DBS_INSERT, &stTblRCmerRecord);

        if (iRet)
        {
            IBPwarn("登记表[tbl_r_cmer_record]异常, [%d][%s]", iRet, dbsErrStr());
            return -1;
        }
    }

    return 0;
}

int iNCardRouteStatPro()
{
    tbl_r_txn_def stTblRTxn;

    setnull(stTblRTxn);

    IBPrace("************************** 无卡交易流水汇总 开始 ******************************");
    IBPgetstring("orderNo", stTblRTxn.order_no, "订单号");
    IBPgetstring("state", stTblRTxn.stat, "交易状态");
    IBPgetstring("chaRspCode", stTblRTxn.ch_respcd, "相应码");

    IBPrace("订单号==>[%s], 状态==>[%s], 响应码==>[%s]", \
            stTblRTxn.order_no, stTblRTxn.stat, stTblRTxn.ch_respcd);

    /* 获取原始交易信息 */
    if (iCheckOriTxn(&stTblRTxn))
    {
        IBPwarn("原交易异常!!!");
        goto ERROR;
    }

    /* 更新原始交易状态 */
    if (iUpdateTxnStat(&stTblRTxn))
    {
        IBPwarn("更新原交易状态异常!!!");
        goto ERROR;
    }

    /* 交易状态不成功 */
    if (memcmp(stTblRTxn.stat, "0", 1) != 0)
    {
        IBPrace("交易非成功状态不用参与商户数据汇总...");
        goto SUCCESS;
    }

    /* 汇总当日商户交易数据 */
    if (iStatMchtRecord(&stTblRTxn))
    {
        IBPwarn("汇总商户交易数据异常!!!");
        goto ERROR;
    }

SUCCESS:
    IBPrace("************************** 无卡交易流水汇总 结束 ******************************");
    lDbsCommit();
    return 0;
ERROR:
    lDbsRollback();
    IBPrace("************************** 无卡交易流水汇总 异常 ******************************");
    return -1;
}
