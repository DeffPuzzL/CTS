#ifndef __tbl_mcht_fund_CH__
#define __tbl_mcht_fund_CH__
typedef struct tbl_mcht_fund_c
{
	char      id[33];
	char      settle_date[9];
	char      settle_num[5];
	char      channel_no[5];
	char      in_date[9];
	char      tx_date[9];
	char      batch_no[21];
	char      trace_no[21];
	char      result_flag[2];
	char      result_desc[61];
	long      tx_num;
	double    tx_amt;
	long      credit_num;
	double    credit_amt;
	double    credit_percent;
	double    fee_amt;
	double    other_fee;
	double    repair_amt;
	double    unrepair_amt;
	double    uperr_amt;
	char      dc_flag[2];
	double    settle_amt;
	char      mert_no[21];
	char      mert_nm[201];
	char      order_req[33];
	char      order_time[15];
	char      res[129];
	char      pay_time[15];
	char      create_time[15];
	char      update_time[15];
}TMchtFund;
#endif 
