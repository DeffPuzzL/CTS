#ifndef __tbl_bat_main_list_CH__
#define __tbl_bat_main_list_CH__
typedef struct tbl_bat_main_list_c
{
	char      id[41];
	char      bat_code[21];
	char      task_date[9];
	char      settle_num[5];
	char      channel_no[5];
	long      bat_cls;
	char      bat_state[2];
	char      bat_msg[257];
	char      bat_argc[257];
	char      bat_dsp[51];
	char      bat_next[61];
	char      task_split[2];
	char      task_assign[2];
	char      task_redo[2];
	long      bat_cost;
	char      task_upd[21];
	char      create_time[15];
	char      update_time[15];
}TBatMainList;
#endif 
