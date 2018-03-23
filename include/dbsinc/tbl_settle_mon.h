#ifndef __tbl_settle_mon_CH__
#define __tbl_settle_mon_CH__
typedef struct tbl_settle_mon_c
{
	char      id[23];
	char      settle_date[9];
	long      que_cnt;
	char      result_flag[2];
	char      mark[65];
	long      resv;
	char      rec_crt_ts[15];
	char      rec_upd_ts[15];
	char      resv_1[65];
	char      resv_2[65];
}TSettleMon;
#endif 
