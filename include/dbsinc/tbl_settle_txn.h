#ifndef __tbl_settle_txn_CH__
#define __tbl_settle_txn_CH__
typedef struct tbl_settle_txn_c
{
	char      id[33];
	char      pay_order[33];
	char      settle_date[9];
	char      in_date[9];
	char      tx_date[9];
	char      settle_num[5];
	char      channel_no[5];
	char      batch_no[33];
	char      trace_no[33];
	char      app_type[2];
	char      app_no[21];
	char      app_nm[201];
	char      pay_acct[33];
	double    settle_amt;
	double    settle_fee;
	char      currency[6];
	char      result_flag[2];
	char      status[31];
	char      status_msg[257];
	char      bank_status[31];
	char      bank_status_msg[257];
	char      bank_batch_no[33];
	char      bank_seqno[33];
	char      resv_phone[12];
	char      acct_no[101];
	char      acct_nm[101];
	char      acct_type[4];
	char      acct_bank_no[21];
	char      acct_bank_nm[101];
	char      acct_zbank_no[21];
	char      acct_zbank_nm[101];
	char      acct_zbank_addr[161];
	char      acct_zbank_code[11];
	char      acct_union_no[21];
	char      acct_net_no[21];
	char      remark[101];
	char      cert_type[3];
	char      cert_no[33];
	long      card_type;
	char      product_code[21];
	char      ext_properties[1001];
	char      create_time[15];
	char      update_time[15];
}TSettleTxn;
#endif 
