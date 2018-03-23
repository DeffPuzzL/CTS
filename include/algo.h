#ifndef __HHH_ALGO_DEFINE_HHH__
#define __HHH_ALGO_DEFINE_HHH__


/**************************************************************************************************
	平台业务日切时间
 **************************************************************************************************/
#define CTS_SETTLE_TIME                            "223000"    /*平台清算时间 */

/**************************************************************************************************
    账户宏定义
 **************************************************************************************************/
#define ACCOUNT_TRANS_AMT                          '1'    // 在途金额
#define ACCOUNT_BALAN_AMT                          '2'    // 账户余额
#define ACCOUNT_FREEZ_AMT                          '3'    // 冻结金额

#define ACCOUNT_MERT_NO                            '1'    // 代付商户帐号
#define ACCOUNT_BRH_NO                             '2'    // 机构分润账户

#define ACCOUNT_FLAG_D                             'D'
#define ACCOUNT_FLAG_C                             'C'

#define ACCOUNT_STAT_OK                            '0'    // 账户状态正常
#define ACCOUNT_STAT_ER                            '1'    // 账户状态异常

#define ACCOUNT_FLG_NMAL                           '0'    // 明细入账正常
#define ACCOUNT_FLG_RESV                           '1'    // 明细入账冲正
/**************************************************************************************************
    清算入帐处理标示
 **************************************************************************************************/
#define SETTLE_IN_FLAG_INIT                        '6' /*-- 数据初始化*/
#define SETTLE_IN_FLAG_PREQ                        '4' /*-- 提交请求*/
#define SETTLE_IN_FLAG_REQS                        '3' /*-- 请求受理成功*/
#define SETTLE_IN_FLAG_TMOT                        '2' /*-- 超时*/
#define SETTLE_IN_FLAG_FAIL                        '1' /*-- 入帐失败*/
#define SETTLE_IN_FLAG_SUCC                        '0' /*-- 入帐成功*/

/**************************************************************************************************
	快捷、代扣流水表导出标识
 **************************************************************************************************/
#define EXPORT_TXN_NULL                            '0'
#define EXPORT_TXN_DONE                            '1'

/**************************************************************************************************
	本地流水表对账标识
 **************************************************************************************************/
#define TXN_SUCC_SUCCESS                           '0'  // 已清分
#define TXN_SUCC_NOTMATCH                          '1'  // 存在差错
#define TXN_SUCC_INIT                              '2'  // 成功流水
#define TXN_SUCC_UPACT                             '3'  // 挂账处理
#define TXN_SUCC_ALGO                              '5'  // 对平待清分
#define TXN_SUCC_REVERSED                          '9'  // 撤销流水


/**************************************************************************************************
	清分流水标识
 **************************************************************************************************/
#define ALGO_MCHT_FLAG_NULL                        '2' // 流水未清分
#define ALGO_MCHT_FLAG_INIT                        '1' // 商户未清分
#define ALGO_MCHT_FLAG_SUCC                        '0' // 商户已清算

#define ALGO_BRH_FLAG_INIT                         '1' // 机构未汇总
#define ALGO_BRH_FLAG_SUCC                         '0' // 机构已汇总
#define ALGO_BRH_FLAG_NULL                         ' ' // 机构未分润

#define MCHT_FREE_NON                   '0' /*-- 商户无手续费优惠 --*/
#define ALGO_FREE_AMT                   '1' /*-- 按金额 --*/
#define ALGO_FREE_RATE                  '2' /*-- 按比例 --*/
#define ALGO_FREE_NUM                   '3' /*-- 按笔数 --*/

/*商户清算明细表*/
#define SETTLE_MCHT_NOTDEAL             '8' /*-- 未处理*/
#define SETTLE_MCHT_RESETT              '7' /*-- 重新出款*/
#define SETTLE_MCHT_WAITE               '6' /*-- 直接插入入账差错表进行手工清算*/
#define SETTLE_MCHT_SPLITE              '5' /*-- 已拆分批次 */
#define SETTLE_MCHT_PUTREQ              '4' /*-- 提交请求*/
#define SETTLE_MCHT_RESEQS              '3' /*-- 请求受理成功*/
#define SETTLE_MCHT_TIMOUT              '2' /*-- 超时*/
#define SETTLE_MCHT_ERROR               '1' /*-- 入账失败*/
#define SETTLE_MCHT_SUCC                '0' /*-- 入账成功*/


/*入账差错信息表标志*/
#define SETTLE_ERR_FK_HOLD              'A' /*-- 风控截留*/
#define SETTLE_ERR_NOT_DEAL             '9' /*-- 未处理 待运营处理*/
#define SETTLE_ERR_HL_WAITE             '3' /*-- 待清算人员处理*/
#define SETTLE_ERR_HL_SETTLE            '2' /*-- 处理后手工清算*/
#define SETTLE_ERR_TO_DONE              '1' /*-- 处理后参加清算*/
#define SETTLE_ERR_SETTLE_SUCC          '0' /*-- 参入清算后已纳入清算*/




/*入账失败表标识*/
#define ERROR_ER_SETTLE_SUCC            '0' /*-- 入账成功*/
#define ERROR_ER_SETTLE_FAIL            '1' /*-- 入账失败*/
#define ERROR_ER_SETTLE_UNKOWN          '2' /*-- 入账未确定*/



// tbl_cup_succ->result_flag
#define CUP_SUCC_SUCCESS                '0'  // 对账平
#define CUP_SUCC_NOTMATCH               '1'  // 对账不平
#define CUP_SUCC_INIT                   '2'  // 成功流水
#define CUP_SUCC_REVERSED               '3'  // 撤销流水

/*费率模型*/
#define DISC_STATE_ENABLE               '0' /*手续费模型 --启用*/
#define DISC_STATE_DISABLE              '1' /*手续费模型 --停用*/

/*费率*/
#define DISC_ALGO_FIXED                 2   /*手续费 --按笔固定*/
#define DISC_ALGO_RATE                  1   /*手续费 --按笔比例*/

/*流水标志*/
#define TRADE_NOT_MATCH                 '7' /*-- 本地无渠道账单流水*/

#define TRADE_TXN_CHECK                 '5' /*-- 流水待对账*/
#define TRADE_TXN_REVERSED              '3' /*-- 撤销或交易已撤销*/
#define TRADE_TXN_INIT                  '2' /*-- 第三方流水未对账*/
#define TRADE_TXN_NOTMATCH              '1' /*-- 已对账但不平*/
#define TRADE_TXN_SUCCESS               '0' /*-- 已对账且对平*/

#define STLM_FLG_OK                     '0' /*-- 对账结果平*/

/*对账平结果标志*/
#define STLM_SUCC_FLG_UPACT             '3' /*-- 不用清分挂账处理*/
#define STLM_SUCC_FLG_ERR               '2' /*-- 对账后有差错，未清分*/
#define STLM_SUCC_FLG_INIT              '1' /*-- 对平后初始化状态，未清分*/
#define STLM_SUCC_FLG_OK                '0' /*-- 已清分*/

/*差错交易信息标志*/
#define ERR_CUP_NOT_CHECK               '\0'/*-- 未对账*/ 
#define STLM_FLG_OK_UNNORMAL            '0' /*-- 对账平，处理异常*/
#define STLM_CUP_FLG_POSP               '1' /*-- POSP有，CUPS没有*/
#define STLM_CUP_FLG_CUPS               '2' /*-- POSP无，CUPS有*/
#define STLM_CUP_FLG_AMT                '3' /*-- POSP与CUPS金额不一致;*/
#define STLM_CUP_FLG_ACCT               '4' /*-- POSP与CUPS帐号不一致;*/

//差错类型
#define ERR_CUP_FLG_NOTMACHT            '1' /*-- 对账不平*/ 
#define ERR_CUP_FLG_RISKCRTL            '2' /*-- 风控拦截延迟清算*/ 
#define ERR_CUP_FLG_MANUAL              '3' /*-- 手工延迟清算*/ 
#define ERR_CUP_MCHT_UNNORMAL           '4' /*-- 商户不正常延迟清算*/ 
#define ERR_CUP_MCHT_UNUSUAL            '5' /*-- 商户信息异常*/ 
#define ERR_CUP_MCHT_OK                 '\0'/*-- 商户信息正常*/

#define STLM_CUP_SETTLE_SUCC            '0' /*-- 参入清算后并已清分*/
#define STLM_CUP_TO_DONE                '1' /*-- 处理后参加清算*/
#define STLM_CUP_HL_CANCLE              '2' /*-- 手工处理退货*/
#define STLM_CUP_HL_REQMN               '3' /*-- 手工处理请款*/
#define STLM_CUP_HL_UPACT               '4' /*-- 手工处理挂账*/
#define STLM_CUP_HL_RETURN              '5' /*-- 手动退货*/
#define STLM_CUP_REPAIR                 '6' /*-- 补帐处理*/
#define STLM_CUP_NOT_DEAL               '9' /*-- 未处理*/

#define ERR_ENT_NOT_CHECK               '\0'/*-- 未对账*/ 
#define STLM_ENT_FLG_OK                 '0' /*-- 对账平*/
#define STLM_ENT_FLG_POSP               '1' /*-- POSP成功, ENT失败 --*/
#define STLM_ENT_FLG_ENTS               '2' /*-- POSP失败, ENT成功 --*/
#define STLM_ENT_FLG_UNKOWN             '3' /*-- POSP不存在, ENT成功 --*/

//  对账状态表
#define CHANNEL_CHECK_INIT              '0' /*-- 未对账*/
#define CHANNEL_CHECK_ING               '1' /*-- 对账中*/
#define CHANNEL_CHECK_SUCC              '2' /*-- 对账成功完成*/
#define CHANNEL_CHECK_ERR               '3' /*-- 对账失败*/
#define CHANNEL_SETTLE_NOT              '0' /*-- 不参与汇总*/
#define CHANNEL_SETTLE_DONE             '1' /*-- 参与汇总*/
#define CHANNEL_CLEAR_YES               '1' /*-- 一清通道*/
#define CHANNEL_CLEAR_NO                '0' /*-- 非一清通道*/

/*补帐信息标志*/
#define REPAIR_ALPY_UNFREEZE            '9' /*申请解冻*/
#define REPAIR_FLG_IN_HOLD              '3' /*截留*/
#define REPAIR_SUCC_UNFREEZE            '0' /*已解冻*/

#define REPAIR_FLG_NOT_DEAL             '9' /*-- 未处理*/
#define REPAIR_FLG_IN_SUPP              '8' /*-- 补账中*/
#define REPAIR_FLG_SUPP_OVER            '7' /*-- 补账完毕*/
#define REPAIR_FLG_DEAL_CANCLE          '3' /*-- 处理后用于退货*/
#define REPAIR_FLG_DEAL_UPACT           '2' /*-- 处理后挂账*/
#define REPAIR_FLG_TO_DONE              '1' /*-- 处理后参加清算*/
#define REPAIR_FLG_SETTLE_SUCC          '0' /*-- 参入清算后已纳入清算*/

#define REPAIR_FLG_MCHT                 '1' /*--商户补账*/
#define REPAIR_FLG_BRH                  '2' /*--机构补账*/

#define REPAIR_TYPE_AMT                 '1' /*--按金额补账*/
#define REPAIR_TYPE_ALL                 '2' /*--结算全部*/

#define SETTLE_ERR_FAILED               '1' /*入账失败*/
#define SETTLE_ERR_UNKOWN               '2' /*入账未确定*/


/*机构清算明细表*/
#define BRH_STLM_YES                    '1' /*-- 初始化待选择*/
#define BRH_STLM_NO                     '0' /*-- 初始化待选择*/

#define SETTLE_BRH_INIT                 '9' /*-- 初始化待选择*/
#define SETTLE_BRH_OK_SETTLE            '8' /*-- 已人工选择可付款*/
#define SETTLE_BRH_HL_WAITE             '3' /*-- 直接插入入账错误表进行手工清算*/
#define SETTLE_BRH_NOT_DEAL             '2' /*-- 已汇总未付款*/
#define SETTLE_BRH_ERR                  '1' /*-- 入账失败*/
#define SETTLE_BRH_SUCC                 '0' /*-- 入账成功*/

/*清算汇总表*/
#define SETTLE_SUM_IN_INIT              '9' /*-- 初始化状态*/
#define SETTLE_SUM_SPLIT_BEF            '3' /*-- 拆分批次前*/
#define SETTLE_SUM_SPLIT_AFT            '2' /*-- 已拆分 好批次*/
#define SETTLE_SUM_IN_ACCT              '1' /*-- 入账进行中*/
#define SETTLE_SUM_IN_SUCC              '0' /*-- 入账完成*/

/*机构、商户账户表*/
#define ACCT_INFO_BRH                   1 /*-- 机构服务费账户*/
#define ACCT_INFO_MCHT                  2 /*-- 商户资金清算账户*/
#define ACCT_INFO_DEFAULT               1 /*-- 默认清算账户*/
#define ACCT_INFO_ENABLE                '0' /*-- 账号 正常*/
#define ACCT_INFO_DISABLE               '1' /*-- 账号 异常*/

#define STLM_ACCT_RECEIPTS              '0' /*-- 0-仅为收款账户*/
#define STLM_ACCT_PAYMENT               '1' /*-- 仅为付款账户*/
#define STLM_ACCT_EITHER                '2' /*-- 即为收款又为付款*/

#define SETTLE_TXN_MCHT                 '1' /*-- 商户流水标识 */
#define SETTLE_MCHT_BRH                 '2' /*-- 机构接入清算流水*/
#define SETTLE_TXN_BRH                  '3' /*-- 机构分润流水标识*/

#define BRH_TREATY_ID                   '0' /*  间联类模型ID*/
#define BRH_STANDARD_ID                 '1' /*  直联类模型ID*/

#define RPT_RECORD_NUM_MAX              64
#define RECORD_LEN_MAX                  1024*10

#define IBPBatNum(x)                    strncmp(x, "TN00", 4)?0:1
#define IBP_LARGE_AMT                   50000.00    /*大额金额*/
#define IBP_COM_CUPSNO                  "ibox"
#define BATCH_REAL_SETTLE               "T088"
#define SETTLE_PAY_REALTM               "SPAY"
#define SETTLE_PAY_NORMAL               "TPAY"

#define EXPORT_FLAG_INIT                '0'
#define EXPORT_FLAG_DONE                '1'

#endif        //    __HHH_ALGO_DEFINE_HHH__
