#ifndef __tbl_cups_succ_CH__
#define __tbl_cups_succ_CH__
typedef struct tbl_cups_succ_c
{
	char      id[25];
	char      stlm_date[9];
	char      tx_date[9];
	char      tx_time[11];
	char      trace_no[13];
	char      cup_key[49];
	char      result_flag[2];
	char      channel_no[5];
	char      term_no[9];
	char      ac_no[20];
	char      user_name[31];
	char      ac_bank_no[41];
	double    tx_amt;
	double    cup_fee;
	char      tx_code[21];
	char      sys_ref_no[33];
	char      brf[61];
	char      fill[61];
	char      res[65];
	char      channel_nm[61];
	char      bank_branch_name[101];
	char      create_time[15];
	char      update_time[15];
}TCupsSucc;
#endif 
