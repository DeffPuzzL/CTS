int Movetbl_txn_hce2Iml(tbl_txn_hce_def *ptTBL_TXN_HCE)
{
    int  iRet = -1;
    char sTmp[128] = {0};

    iRet = DbsTBL_TXN_HCE(DBS_SELECT, ptTBL_TXN_HCE);
    if(iRet != 0) {
        logger(LOG_ERROR, "SELECT TBL_TXN_HCE, error [%d]!", iRet);
        return -1;
    }

    /* 交易日期 */
    logger(LOG_DEBUG, "ipath [/Doc/Sys/Loc/Dt], value [%s]", ptTBL_TXN_HCE->txn_date);
    ImlSetNodeVal("/Doc/Sys/Loc/Dt", ptTBL_TXN_HCE->txn_date, -1);

    /* 交易流水 */
    logger(LOG_DEBUG, "ipath [/App/txn_ssn], value [%s]", ptTBL_TXN_HCE->txn_ssn);
    ImlSetNodeVal("/App/txn_ssn", ptTBL_TXN_HCE->txn_ssn, -1);

    /* 交易时间 */
    logger(LOG_DEBUG, "ipath [/Doc/Sys/Loc/Tm], value [%s]", ptTBL_TXN_HCE->txn_time);
    ImlSetNodeVal("/Doc/Sys/Loc/Tm", ptTBL_TXN_HCE->txn_time, -1);

    /* 银联交易代码 */
    logger(LOG_DEBUG, "ipath [/App/TranCode], value [%s]", ptTBL_TXN_HCE->trancode);
    ImlSetNodeVal("/App/TranCode", ptTBL_TXN_HCE->trancode, -1);

    /* 系统交易代码 */
    logger(LOG_DEBUG, "ipath [/App/txn/syscode], value [%s]", ptTBL_TXN_HCE->syscode);
    ImlSetNodeVal("/App/txn/syscode", ptTBL_TXN_HCE->syscode, -1);

    /* 交易名称 */
    logger(LOG_DEBUG, "ipath [/App/txn_name], value [%s]", ptTBL_TXN_HCE->txn_name);
    ImlSetNodeVal("/App/txn_name", ptTBL_TXN_HCE->txn_name, -1);

    /* 日志唯一戳 */
    logger(LOG_DEBUG, "ipath [/App/log_stamp], value [%s]", ptTBL_TXN_HCE->log_stamp);
    ImlSetNodeVal("/App/log_stamp", ptTBL_TXN_HCE->log_stamp, -1);

    /* key值 */
    logger(LOG_DEBUG, "ipath [/App/txn/biz_id], value [%s]", ptTBL_TXN_HCE->biz_id);
    ImlSetNodeVal("/App/txn/biz_id", ptTBL_TXN_HCE->biz_id, -1);

    /* 交易状态 */
    logger(LOG_DEBUG, "ipath [/App/txn/txn_stat], value [%s]", ptTBL_TXN_HCE->txn_stat);
    ImlSetNodeVal("/App/txn/txn_stat", ptTBL_TXN_HCE->txn_stat, -1);

    /* 撤销标志 */
    logger(LOG_DEBUG, "ipath [/App/txn/cancel_flag], value [%s]", ptTBL_TXN_HCE->cancel_flag);
    ImlSetNodeVal("/App/txn/cancel_flag", ptTBL_TXN_HCE->cancel_flag, -1);

    /* 冲正标志 */
    logger(LOG_DEBUG, "ipath [/App/txn/rev_flag], value [%s]", ptTBL_TXN_HCE->rev_flag);
    ImlSetNodeVal("/App/txn/rev_flag", ptTBL_TXN_HCE->rev_flag, -1);

    /* 交易渠道 */
    logger(LOG_DEBUG, "ipath [/App/applyChannel], value [%s]", ptTBL_TXN_HCE->channelid);
    ImlSetNodeVal("/App/applyChannel", ptTBL_TXN_HCE->channelid, -1);

    /* 交易渠道 */
    logger(LOG_DEBUG, "ipath [/App/CHANNELNAME], value [%s]", ptTBL_TXN_HCE->channelname);
    ImlSetNodeVal("/App/CHANNELNAME", ptTBL_TXN_HCE->channelname, -1);

    /* 应用版本 */
    logger(LOG_DEBUG, "ipath [/App/AppVer], value [%s]", ptTBL_TXN_HCE->app_ver);
    ImlSetNodeVal("/App/AppVer", ptTBL_TXN_HCE->app_ver, -1);

    /* 银联交易时间 */
    logger(LOG_DEBUG, "ipath [/App/cupreqtime], value [%s]", ptTBL_TXN_HCE->cup_time);
    ImlSetNodeVal("/App/cupreqtime", ptTBL_TXN_HCE->cup_time, -1);

    /* 银联流水号 */
    logger(LOG_DEBUG, "ipath [/App/cups_ssn], value [%s]", ptTBL_TXN_HCE->cupssn);
    ImlSetNodeVal("/App/cups_ssn", ptTBL_TXN_HCE->cupssn, -1);

    /* 系统流水号 */
    logger(LOG_DEBUG, "ipath [/App/txn_ssn], value [%s]", ptTBL_TXN_HCE->respserial);
    ImlSetNodeVal("/App/txn_ssn", ptTBL_TXN_HCE->respserial, -1);

    /* 报文请求方 */
    logger(LOG_DEBUG, "ipath [/App/OutOrgCode], value [%s]", ptTBL_TXN_HCE->outorgcode);
    ImlSetNodeVal("/App/OutOrgCode", ptTBL_TXN_HCE->outorgcode, -1);

    /* 报文接收方 */
    logger(LOG_DEBUG, "ipath [/App/InOrgCode], value [%s]", ptTBL_TXN_HCE->inorgcode);
    ImlSetNodeVal("/App/InOrgCode", ptTBL_TXN_HCE->inorgcode, -1);

    /* 安全载体标识 */
    logger(LOG_DEBUG, "ipath [/App/SEID], value [%s]", ptTBL_TXN_HCE->seid);
    ImlSetNodeVal("/App/SEID", ptTBL_TXN_HCE->seid, -1);

    /* 实体卡卡号 */
    logger(LOG_DEBUG, "ipath [/App/span], value [%s]", ptTBL_TXN_HCE->span);
    ImlSetNodeVal("/App/span", ptTBL_TXN_HCE->span, -1);

    /* 实体卡主账户标识 */
    logger(LOG_DEBUG, "ipath [/App/spanId], value [%s]", ptTBL_TXN_HCE->span_id);
    ImlSetNodeVal("/App/spanId", ptTBL_TXN_HCE->span_id, -1);

    /* 设备卡卡号 */
    logger(LOG_DEBUG, "ipath [/App/mpan], value [%s]", ptTBL_TXN_HCE->mpan);
    ImlSetNodeVal("/App/mpan", ptTBL_TXN_HCE->mpan, -1);

    /* 设备卡主账户标识 */
    logger(LOG_DEBUG, "ipath [/App/mpanId], value [%s]", ptTBL_TXN_HCE->mpan_id);
    ImlSetNodeVal("/App/mpanId", ptTBL_TXN_HCE->mpan_id, -1);

    /* 借贷应用AID */
    logger(LOG_DEBUG, "ipath [/App/instanceAid], value [%s]", ptTBL_TXN_HCE->instanceaid);
    ImlSetNodeVal("/App/instanceAid", ptTBL_TXN_HCE->instanceaid, -1);

    /* 应答码 */
    logger(LOG_DEBUG, "ipath [/App/status], value [%s]", ptTBL_TXN_HCE->status);
    ImlSetNodeVal("/App/status", ptTBL_TXN_HCE->status, -1);

    /* 应答描述 */
    logger(LOG_DEBUG, "ipath [/App/statusDsp], value [%s]", ptTBL_TXN_HCE->statusdsp);
    ImlSetNodeVal("/App/statusDsp", ptTBL_TXN_HCE->statusdsp, -1);

    /* 持卡人姓名拼音 */
    logger(LOG_DEBUG, "ipath [/App/cardHolderName], value [%s]", ptTBL_TXN_HCE->cardholdername);
    ImlSetNodeVal("/App/cardHolderName", ptTBL_TXN_HCE->cardholdername, -1);

    /* 持卡人证件类型 */
    logger(LOG_DEBUG, "ipath [/App/cardHolderIdType], value [%s]", ptTBL_TXN_HCE->cardholderidtype);
    ImlSetNodeVal("/App/cardHolderIdType", ptTBL_TXN_HCE->cardholderidtype, -1);

    /* 持卡人证件号码 */
    logger(LOG_DEBUG, "ipath [/App/cardHolderIdNo], value [%s]", ptTBL_TXN_HCE->cardholderidno);
    ImlSetNodeVal("/App/cardHolderIdNo", ptTBL_TXN_HCE->cardholderidno, -1);

    /* 持卡人联系号码 */
    logger(LOG_DEBUG, "ipath [/App/msisdn], value [%s]", ptTBL_TXN_HCE->msisdn);
    ImlSetNodeVal("/App/msisdn", ptTBL_TXN_HCE->msisdn, -1);

    /* 操作结果 */
    logger(LOG_DEBUG, "ipath [/App/Result], value [%s]", ptTBL_TXN_HCE->txn_result);
    ImlSetNodeVal("/App/Result", ptTBL_TXN_HCE->txn_result, -1);

    /* 免密限额 */
    logger(LOG_DEBUG, "ipath [/App/initQuota], value [%s]", ptTBL_TXN_HCE->initquota);
    ImlSetNodeVal("/App/initQuota", ptTBL_TXN_HCE->initquota, -1);

    /* 卡面配置ID */
    logger(LOG_DEBUG, "ipath [/App/cardArtId], value [%s]", ptTBL_TXN_HCE->cardartid);
    ImlSetNodeVal("/App/cardArtId", ptTBL_TXN_HCE->cardartid, -1);

    /* 保留域 */
    logger(LOG_DEBUG, "ipath [/App/txn/misc], value [%s]", ptTBL_TXN_HCE->misc);
    ImlSetNodeVal("/App/txn/misc", ptTBL_TXN_HCE->misc, -1);

    /* 保留域1 */
    logger(LOG_DEBUG, "ipath [/App/txn/misc_1], value [%s]", ptTBL_TXN_HCE->misc_1);
    ImlSetNodeVal("/App/txn/misc_1", ptTBL_TXN_HCE->misc_1, -1);

    /* 保留域2 */
    logger(LOG_DEBUG, "ipath [/App/txn/misc_2], value [%s]", ptTBL_TXN_HCE->misc_2);
    ImlSetNodeVal("/App/txn/misc_2", ptTBL_TXN_HCE->misc_2, -1);

    /* 保留域3 */
    logger(LOG_DEBUG, "ipath [/App/txn/misc_3], value [%s]", ptTBL_TXN_HCE->misc_3);
    ImlSetNodeVal("/App/txn/misc_3", ptTBL_TXN_HCE->misc_3, -1);

    /* 保留域4 */
    logger(LOG_DEBUG, "ipath [/App/txn/misc_4], value [%s]", ptTBL_TXN_HCE->misc_4);
    ImlSetNodeVal("/App/txn/misc_4", ptTBL_TXN_HCE->misc_4, -1);

    /* 最后更新时间 */
    logger(LOG_DEBUG, "ipath [/App/txn/last_upd_ts], value [%s]", ptTBL_TXN_HCE->last_upd_ts);
    ImlSetNodeVal("/App/txn/last_upd_ts", ptTBL_TXN_HCE->last_upd_ts, -1);

    /* 创建时间 */
    logger(LOG_DEBUG, "ipath [/App/txn/create_ts], value [%s]", ptTBL_TXN_HCE->create_ts);
    ImlSetNodeVal("/App/txn/create_ts", ptTBL_TXN_HCE->create_ts, -1);

    return 0;
}

int MoveIml2tbl_txn_hce(void)
{
    int  iRet = -1;
    char sTmp[128]={0};

    tbl_txn_hce_def stTBL_TXN_HCE;

    memset(&stTBL_TXN_HCE, 0x00, sizeof(tbl_txn_hce_def));

    /* 交易日期 */
    ImlGetNodeVal("/Doc/Sys/Loc/Dt", sizeof(stTBL_TXN_HCE.txn_date), stTBL_TXN_HCE.txn_date, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [txn_date], value [%s]", stTBL_TXN_HCE.txn_date);

    /* 交易流水 */
    ImlGetNodeVal("/App/txn_ssn", sizeof(stTBL_TXN_HCE.txn_ssn), stTBL_TXN_HCE.txn_ssn, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [txn_ssn], value [%s]", stTBL_TXN_HCE.txn_ssn);

    /* 交易时间 */
    ImlGetNodeVal("/Doc/Sys/Loc/Tm", sizeof(stTBL_TXN_HCE.txn_time), stTBL_TXN_HCE.txn_time, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [txn_time], value [%s]", stTBL_TXN_HCE.txn_time);

    /* 银联交易代码 */
    ImlGetNodeVal("/App/TranCode", sizeof(stTBL_TXN_HCE.trancode), stTBL_TXN_HCE.trancode, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [trancode], value [%s]", stTBL_TXN_HCE.trancode);

    /* 系统交易代码 */
    ImlGetNodeVal("/App/txn/syscode", sizeof(stTBL_TXN_HCE.syscode), stTBL_TXN_HCE.syscode, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [syscode], value [%s]", stTBL_TXN_HCE.syscode);

    /* 交易名称 */
    ImlGetNodeVal("/App/txn_name", sizeof(stTBL_TXN_HCE.txn_name), stTBL_TXN_HCE.txn_name, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [txn_name], value [%s]", stTBL_TXN_HCE.txn_name);

    /* 日志唯一戳 */
    ImlGetNodeVal("/App/log_stamp", sizeof(stTBL_TXN_HCE.log_stamp), stTBL_TXN_HCE.log_stamp, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [log_stamp], value [%s]", stTBL_TXN_HCE.log_stamp);

    /* key值 */
    ImlGetNodeVal("/App/txn/biz_id", sizeof(stTBL_TXN_HCE.biz_id), stTBL_TXN_HCE.biz_id, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [biz_id], value [%s]", stTBL_TXN_HCE.biz_id);

    /* 交易状态 */
    ImlGetNodeVal("/App/txn/txn_stat", sizeof(stTBL_TXN_HCE.txn_stat), stTBL_TXN_HCE.txn_stat, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [txn_stat], value [%s]", stTBL_TXN_HCE.txn_stat);

    /* 撤销标志 */
    ImlGetNodeVal("/App/txn/cancel_flag", sizeof(stTBL_TXN_HCE.cancel_flag), stTBL_TXN_HCE.cancel_flag, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [cancel_flag], value [%s]", stTBL_TXN_HCE.cancel_flag);

    /* 冲正标志 */
    ImlGetNodeVal("/App/txn/rev_flag", sizeof(stTBL_TXN_HCE.rev_flag), stTBL_TXN_HCE.rev_flag, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [rev_flag], value [%s]", stTBL_TXN_HCE.rev_flag);

    /* 交易渠道 */
    ImlGetNodeVal("/App/applyChannel", sizeof(stTBL_TXN_HCE.channelid), stTBL_TXN_HCE.channelid, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [channelid], value [%s]", stTBL_TXN_HCE.channelid);

    /* 交易渠道 */
    ImlGetNodeVal("/App/CHANNELNAME", sizeof(stTBL_TXN_HCE.channelname), stTBL_TXN_HCE.channelname, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [channelname], value [%s]", stTBL_TXN_HCE.channelname);

    /* 应用版本 */
    ImlGetNodeVal("/App/AppVer", sizeof(stTBL_TXN_HCE.app_ver), stTBL_TXN_HCE.app_ver, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [app_ver], value [%s]", stTBL_TXN_HCE.app_ver);

    /* 银联交易时间 */
    ImlGetNodeVal("/App/cupreqtime", sizeof(stTBL_TXN_HCE.cup_time), stTBL_TXN_HCE.cup_time, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [cup_time], value [%s]", stTBL_TXN_HCE.cup_time);

    /* 银联流水号 */
    ImlGetNodeVal("/App/cups_ssn", sizeof(stTBL_TXN_HCE.cupssn), stTBL_TXN_HCE.cupssn, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [cupssn], value [%s]", stTBL_TXN_HCE.cupssn);

    /* 系统流水号 */
    ImlGetNodeVal("/App/txn_ssn", sizeof(stTBL_TXN_HCE.respserial), stTBL_TXN_HCE.respserial, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [respserial], value [%s]", stTBL_TXN_HCE.respserial);

    /* 报文请求方 */
    ImlGetNodeVal("/App/OutOrgCode", sizeof(stTBL_TXN_HCE.outorgcode), stTBL_TXN_HCE.outorgcode, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [outorgcode], value [%s]", stTBL_TXN_HCE.outorgcode);

    /* 报文接收方 */
    ImlGetNodeVal("/App/InOrgCode", sizeof(stTBL_TXN_HCE.inorgcode), stTBL_TXN_HCE.inorgcode, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [inorgcode], value [%s]", stTBL_TXN_HCE.inorgcode);

    /* 安全载体标识 */
    ImlGetNodeVal("/App/SEID", sizeof(stTBL_TXN_HCE.seid), stTBL_TXN_HCE.seid, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [seid], value [%s]", stTBL_TXN_HCE.seid);

    /* 实体卡卡号 */
    ImlGetNodeVal("/App/span", sizeof(stTBL_TXN_HCE.span), stTBL_TXN_HCE.span, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [span], value [%s]", stTBL_TXN_HCE.span);

    /* 实体卡主账户标识 */
    ImlGetNodeVal("/App/spanId", sizeof(stTBL_TXN_HCE.span_id), stTBL_TXN_HCE.span_id, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [span_id], value [%s]", stTBL_TXN_HCE.span_id);

    /* 设备卡卡号 */
    ImlGetNodeVal("/App/mpan", sizeof(stTBL_TXN_HCE.mpan), stTBL_TXN_HCE.mpan, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [mpan], value [%s]", stTBL_TXN_HCE.mpan);

    /* 设备卡主账户标识 */
    ImlGetNodeVal("/App/mpanId", sizeof(stTBL_TXN_HCE.mpan_id), stTBL_TXN_HCE.mpan_id, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [mpan_id], value [%s]", stTBL_TXN_HCE.mpan_id);

    /* 借贷应用AID */
    ImlGetNodeVal("/App/instanceAid", sizeof(stTBL_TXN_HCE.instanceaid), stTBL_TXN_HCE.instanceaid, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [instanceaid], value [%s]", stTBL_TXN_HCE.instanceaid);

    /* 应答码 */
    ImlGetNodeVal("/App/status", sizeof(stTBL_TXN_HCE.status), stTBL_TXN_HCE.status, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [status], value [%s]", stTBL_TXN_HCE.status);

    /* 应答描述 */
    ImlGetNodeVal("/App/statusDsp", sizeof(stTBL_TXN_HCE.statusdsp), stTBL_TXN_HCE.statusdsp, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [statusdsp], value [%s]", stTBL_TXN_HCE.statusdsp);

    /* 持卡人姓名拼音 */
    ImlGetNodeVal("/App/cardHolderName", sizeof(stTBL_TXN_HCE.cardholdername), stTBL_TXN_HCE.cardholdername, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [cardholdername], value [%s]", stTBL_TXN_HCE.cardholdername);

    /* 持卡人证件类型 */
    ImlGetNodeVal("/App/cardHolderIdType", sizeof(stTBL_TXN_HCE.cardholderidtype), stTBL_TXN_HCE.cardholderidtype, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [cardholderidtype], value [%s]", stTBL_TXN_HCE.cardholderidtype);

    /* 持卡人证件号码 */
    ImlGetNodeVal("/App/cardHolderIdNo", sizeof(stTBL_TXN_HCE.cardholderidno), stTBL_TXN_HCE.cardholderidno, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [cardholderidno], value [%s]", stTBL_TXN_HCE.cardholderidno);

    /* 持卡人联系号码 */
    ImlGetNodeVal("/App/msisdn", sizeof(stTBL_TXN_HCE.msisdn), stTBL_TXN_HCE.msisdn, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [msisdn], value [%s]", stTBL_TXN_HCE.msisdn);

    /* 操作结果 */
    ImlGetNodeVal("/App/Result", sizeof(stTBL_TXN_HCE.txn_result), stTBL_TXN_HCE.txn_result, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [txn_result], value [%s]", stTBL_TXN_HCE.txn_result);

    /* 免密限额 */
    ImlGetNodeVal("/App/initQuota", sizeof(stTBL_TXN_HCE.initquota), stTBL_TXN_HCE.initquota, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [initquota], value [%s]", stTBL_TXN_HCE.initquota);

    /* 卡面配置ID */
    ImlGetNodeVal("/App/cardArtId", sizeof(stTBL_TXN_HCE.cardartid), stTBL_TXN_HCE.cardartid, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [cardartid], value [%s]", stTBL_TXN_HCE.cardartid);

    /* 保留域 */
    ImlGetNodeVal("/App/txn/misc", sizeof(stTBL_TXN_HCE.misc), stTBL_TXN_HCE.misc, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [misc], value [%s]", stTBL_TXN_HCE.misc);

    /* 保留域1 */
    ImlGetNodeVal("/App/txn/misc_1", sizeof(stTBL_TXN_HCE.misc_1), stTBL_TXN_HCE.misc_1, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [misc_1], value [%s]", stTBL_TXN_HCE.misc_1);

    /* 保留域2 */
    ImlGetNodeVal("/App/txn/misc_2", sizeof(stTBL_TXN_HCE.misc_2), stTBL_TXN_HCE.misc_2, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [misc_2], value [%s]", stTBL_TXN_HCE.misc_2);

    /* 保留域3 */
    ImlGetNodeVal("/App/txn/misc_3", sizeof(stTBL_TXN_HCE.misc_3), stTBL_TXN_HCE.misc_3, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [misc_3], value [%s]", stTBL_TXN_HCE.misc_3);

    /* 保留域4 */
    ImlGetNodeVal("/App/txn/misc_4", sizeof(stTBL_TXN_HCE.misc_4), stTBL_TXN_HCE.misc_4, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [misc_4], value [%s]", stTBL_TXN_HCE.misc_4);

    /* 最后更新时间 */
    ImlGetNodeVal("/App/txn/last_upd_ts", sizeof(stTBL_TXN_HCE.last_upd_ts), stTBL_TXN_HCE.last_upd_ts, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [last_upd_ts], value [%s]", stTBL_TXN_HCE.last_upd_ts);

    /* 创建时间 */
    ImlGetNodeVal("/App/txn/create_ts", sizeof(stTBL_TXN_HCE.create_ts), stTBL_TXN_HCE.create_ts, NULL);
    logger(LOG_DEBUG, "table name [TBL_TXN_HCE], field name [create_ts], value [%s]", stTBL_TXN_HCE.create_ts);

    iRet = DbsTBL_TXN_HCE(DBS_INSERT, &stTBL_TXN_HCE);
    if(iRet != 0) {
        logger(LOG_ERROR, "insert into table tbl_txn_hce error [%d]", iRet);
        return -1;
    }
    return 0;
}
