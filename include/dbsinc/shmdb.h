#ifndef	__TBL_XX_DATA_INTERFACE__
#define	__TBL_XX_DATA_INTERFACE__

typedef struct  __TBL_MCHT_STLM
{
    long    acct_id;
    char    mcht_code[21];
    long    acct_default;
    char    acct_type[2];
    char    acct_no[40];
    char    acct_nm[81];
    char    acct_bank_no[17];
    char    acct_bank_nm[151];
    char    acct_zbank_no[17];
    char    acct_zbank_nm[151];
    char    acct_zbank_addr[161];
    char    acct_zbank_code[11];
    char    acct_net_no[17];
    char    acct_union_no[17];
    char    cert_type[4];
    char    cert_no[33];
    char    resv_phone[13];
}dbMchtAcct;

typedef struct  __TBL_MCHT_BASE
{
    char    mcht_no[26];
    char    mcht_nm[201];
    char    mcht_status[3];
    char    mcht_kind[3];
    char    mcht_addr[301];
    char    mcht_brh[21];
}dbMchtBase;

typedef struct  __TBL_MCHT_PROD
{
    char    prod_code[21];
    char    buss_type[7];
	char	sub_buss_type[7];
    char    status[3];
	long	disc_num;
    char    dis_groups[500];
    char    pay_kind[3];
	long	disc_cycle;
	long	settle_model;
}dbMchtProd;

typedef struct  __TBL_DISC_ALGO
{
    long    id;
    char    disc_id[9];
    long    index_num;
    double  min_fee;
    double  max_fee;
    double  floor_amount;
    double  upper_amount;
    long    flag;
    double  fee_value;
    long    card_type;
}dbDiscAlgo;

#endif	//	__TBL_XX_DATA_INTERFACE__
