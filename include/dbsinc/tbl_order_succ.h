#ifndef __tbl_order_succ_CH__
#define __tbl_order_succ_CH__
typedef struct tbl_order_succ_c
{
	char      id[33];
	char      tx_date[9];
	char      tx_time[7];
	char      channel_no[5];
	char      pay_type[33];
	char      seq_order[33];
	char      bank_code[101];
	char      cup_key[49];
	char      result_flag[2];
	double    tx_amt;
	double    cup_fee;
	long      disc_cycle;
	char      bank_card_no[31];
	char      user_name[31];
	char      ext_properties[1001];
	char      bank_acc_type[4];
	char      bank_full_name[31];
	char      bank_branch_name[101];
	char      bank_areacode[5];
	char      cnaps_no[16];
	char      cnaps_bankno[16];
	char      currency[6];
	char      remark[101];
	char      status[31];
	char      bank_respcode[31];
	char      bank_respmsg[101];
	char      vid_respcode[31];
	char      vid_respmsg[101];
	long      card_type;
	char      app_code[33];
	char      mert_no[21];
	char      mert_nm[201];
	char      brh_code[31];
	char      cert_type[3];
	char      cert_no[21];
	char      mobile_no[12];
	char      order_id[33];
	char      product_code[21];
	char      product_name[91];
	char      pay_kind[4];
	char      bank_seqno[33];
	char      batch_seqno[33];
	char      create_time[15];
	char      update_time[15];
}TOrderSucc;
#endif 