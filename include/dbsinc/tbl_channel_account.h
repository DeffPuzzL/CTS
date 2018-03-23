#ifndef __tbl_channel_account_CH__
#define __tbl_channel_account_CH__
typedef struct tbl_channel_account_c
{
	char      id[33];
	char      channel_no[5];
	char      channel_act[31];
	char      channel_nm[257];
	char      account_type[2];
	char      account_status[2];
	double    channel_amt;
	double    channel_fee;
	double    channel_pay;
	double    trans_amt;
	double    balance_ing;
	double    balance_amt;
	double    freeze_amt;
	double    rsv_amt;
	char      mark[65];
	char      update_time[15];
	char      create_time[15];
}TChannelAccount;
#endif 
