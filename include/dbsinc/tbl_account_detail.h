#ifndef __tbl_account_detail_CH__
#define __tbl_account_detail_CH__
typedef struct tbl_account_detail_c
{
	char      id[33];
	char      app_type[2];
	char      app_no[31];
	char      in_date[9];
	char      dc_flag[2];
	char      in_type[2];
	double    in_amt;
	char      in_illust[65];
	char      origin_id[65];
	double    rsv_amt;
	char      mark[65];
	long      resv;
	char      create_time[15];
}TAccountDetail;
#endif 
