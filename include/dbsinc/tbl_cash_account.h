#ifndef __tbl_cash_account_CH__
#define __tbl_cash_account_CH__
typedef struct tbl_cash_account_c
{
	char      id[33];
	char      app_type[2];
	char      app_no[31];
	char      account_no[31];
	char      account_nm[257];
	char      account_type[2];
	char      account_status[2];
	double    total_amt;
	double    total_fee;
	double    total_pay;
	double    trans_amt;
	double    balance_ing;
	double    balance_amt;
	double    freeze_amt;
	double    rsv_amt;
	char      mark[65];
	char      update_time[15];
	char      create_time[15];
}TCashAccount;
#endif 
