
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[17];
};
static const struct sqlcxp sqlfpn =
{
    16,
    "settle_algo.pcpp"
};


static unsigned int sqlctx = 5192700;


static struct sqlexd {
   unsigned long  sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned char  *sqhstv[50];
   unsigned long  sqhstl[50];
            int   sqhsts[50];
            short *sqindv[50];
            int   sqinds[50];
   unsigned long  sqharm[50];
   unsigned long  *sqharc[50];
   unsigned short  sqadto[50];
   unsigned short  sqtdso[50];
} sqlstm = {12,50};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned int *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(unsigned char *, signed int *); }

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4274,871,0,0,
5,0,0,1,308,0,3,354,0,0,47,47,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,
5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,4,0,0,1,4,0,0,
1,4,0,0,1,4,0,0,1,4,0,0,1,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,
0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,
0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,
5,0,0,1,5,0,0,1,5,0,0,
208,0,0,2,206,0,3,361,0,0,30,30,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,3,0,0,1,4,0,0,1,3,0,0,1,4,0,0,1,4,0,
0,1,4,0,0,1,4,0,0,1,4,0,0,1,4,0,0,1,4,0,0,1,5,0,0,1,4,0,0,1,5,0,0,1,5,0,0,1,5,
0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
343,0,0,3,291,0,3,371,0,0,44,44,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,
0,1,4,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,
0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,
5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
534,0,0,4,0,0,31,388,0,0,0,0,0,1,0,
549,0,0,5,121,0,3,448,0,0,7,7,0,1,0,1,5,0,0,1,5,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,
3,0,0,1,5,0,0,
592,0,0,6,57,0,4,516,0,0,2,1,0,1,0,2,3,0,0,1,5,0,0,
615,0,0,7,326,0,3,524,0,0,50,50,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1,5,0,0,1,5,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,
0,1,5,0,0,1,4,0,0,1,4,0,0,1,4,0,0,1,4,0,0,1,4,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,
0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,
5,0,0,1,5,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
830,0,0,8,0,0,29,532,0,0,0,0,0,1,0,
845,0,0,9,136,0,4,556,0,0,3,2,0,1,0,2,3,0,0,1,5,0,0,1,5,0,0,
};


#line 1 "settle_algo.pcpp"
/**************************************************************************************************
    文 件 名：demo.pc
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：demo实现业务模块
 **************************************************************************************************/
#include    "settle_algo.h"

/* EXEC SQL INCLUDE SQLCA;
 */ 
#line 1 "/usr/lib/oracle/product/11.2.0/client/precomp/public/SQLCA.H"
/*
 * $Header: sqlca.h 24-apr-2003.12:50:58 mkandarp Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */

#line 11 "settle_algo.pcpp"

/* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 12 "settle_algo.pcpp"

#include    "tbl_order_err.h"
#include    "tbl_algo_dtls.h"
#include    "tbl_mcht_fund.h"
#include    "tbl_settle_txn.h"
#include    "tbl_settle_mon.h"
/* EXEC SQL END DECLARE SECTION; */ 
#line 18 "settle_algo.pcpp"


/**************************************************************************************************
    方法定义
 **************************************************************************************************/
CClear::stEvent CClear::m_stEvent[] =
{
    {"settle",          &CClear::vGenSettleTxn },
    {"check",           &CClear::vChkSettleTxn },
    { "", NULL }
};

/**************************************************************************************************
    函 数 名：CClear()
    功能描述：构造函数
    返回说明：
 **************************************************************************************************/
CClear::CClear()
{

}

/**************************************************************************************************
    函 数 名：~CClear()
    功能描述：析构函数
    返回说明：
 **************************************************************************************************/
CClear::~CClear()
{


}

/**************************************************************************************************
    函 数 名：lRunObject(char* in_szClass, char* in_szAction)
    功能描述：运行类中的方法
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
long    CClear::lRunObject(char *in_szClass, char *in_szAction)
{
    int     i = 0;
    char    *p, szAction[256];

    memset(szAction, 0, sizeof(szAction));

    if ((p = strstr(in_szAction, "__")) != NULL)
    {
        strncpy(szAction, in_szAction, p - in_szAction);
    }
    else
    {
        strcpy(szAction, in_szAction);
    }

    while (0 != strcmp(m_stEvent[i].m_szName, ""))
    {
        if (0 == strcmp(m_stEvent[i].m_szName, szAction))
        {
            (this->*m_stEvent[i].m_pEvent)();
            return RC_SUCC;
        }

        i++;
    }

    SYSError(RET_SYSTEM, "组件(%s)的事件(%s)没注册", in_szClass, in_szAction);
    return RC_FAIL;
}

/**************************************************************************************************
    函 数 名：dGetAlgoFee(dbDiscAlgo *pstAlgo, double dTxAmt, long *plFeeType)
    功能描述：计算交易金额对应档费率
    返回说明：
        无
 **************************************************************************************************/
double  CClear::dGetAlgoFee(dbDiscAlgo *pstAlgo, double dTxAmt, long *plFeeType)
{
    double dFee = 0.00, dCalFee = 0.00;

    if (DISC_ALGO_FIXED == pstAlgo->flag)              // 按笔(固定)收费
    {
        *plFeeType = 4;
        dCalFee = pstAlgo->fee_value;
    }
    else if (DISC_ALGO_RATE == pstAlgo->flag)          // 按比收费
    {
        *plFeeType = 1;                              // 1-扣率类
        dFee = dTxAmt * pstAlgo->fee_value / 100;
        if (pstAlgo->min_fee >= dFee)                  // 取最小最大区间的费用
            dFee = pstAlgo->min_fee;
            
        if (pstAlgo->max_fee > 0)
        {
            if (pstAlgo->max_fee <= dFee)
            {
                *plFeeType = 3;                      // 封顶类并达到封顶值
                dFee = pstAlgo->max_fee;             
            }   
            else
            {
                *plFeeType = 2;                      // 固定手续费
            }   
        }   
        dCalFee = dFee;
    }   
    else
    {
        *plFeeType = 0;                              //  其他方式未定，无费率处理
        dCalFee = 0.00;                              
    }   

    return dCalFee;
}

/**************************************************************************************************
    函 数 名：nCalculateFee(char *pszDiscId, TAlgoDtls *pstAlgoDtl)
    功能描述：获取当前交易费率
    返回说明：
        无
 **************************************************************************************************/
long    CClear::nCalculateFee(char *pszDiscId, TAlgoDtls *pstAlgoDtl) 
{
    dbDiscAlgo  stAlgo;

    if(RC_SUCC != lGetDiscAlgo(pszDiscId, pstAlgoDtl->tx_amt, pstAlgoDtl->card_type, &stAlgo))
    {
        SYSError(RET_SYSTEM, "获取费率模型(%s)失败, %s", pszDiscId, sGetError());
        return RC_FAIL;
    }

    pstAlgoDtl->fee_amt = dGetAlgoFee(&stAlgo, pstAlgoDtl->tx_amt, &pstAlgoDtl->fee_type);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vFillAlgoDtl(TAlgoDtls *pstAlgo)
    功能描述：填充清分数据
    返回说明：
        无
 **************************************************************************************************/
void    CClear::vFillAlgoDtl(TAlgoDtls *pstAlgo)
{
    char    szState[20];

    memset(szState, 0, sizeof(szState));
    IBPgetstring("id", pstAlgo->id, "id");
    IBPgetstring("tx_date", pstAlgo->tx_date, "tx_date");
    IBPgetstring("tx_time", pstAlgo->tx_time, "tx_time");
    IBPgetstring("channel_no", pstAlgo->channel_no, "channel_no");
    IBPgetstring("pay_type", pstAlgo->pay_type, "pay_type");
    IBPgetstring("seq_order", pstAlgo->seq_order, "seq_order");
    IBPgetdouble("tx_amt", &pstAlgo->tx_amt, "tx_amt");
    IBPgetlong("card_type", &pstAlgo->card_type, "card_type");
    IBPgetlong("disc_cycle", &pstAlgo->disc_cycle, "disc_cycle");
    IBPgetstring("app_code", pstAlgo->app_code, "app_code");
    IBPgetstring("mert_no", pstAlgo->mert_no, "mert_no");
    IBPgetstring("mert_nm", pstAlgo->mert_nm, "mert_nm");
    IBPgetstring("brh_code", pstAlgo->brh_code, "brh_code");
    IBPgetstring("product_code", pstAlgo->product_code, "product_code");

    IBPgetdefstring("bank_code", pstAlgo->bank_code, "");
    IBPgetdefstring("bank_card_no", &pstAlgo->bank_card_no, "");
    IBPgetdefstring("user_name", pstAlgo->user_name, "");
    IBPgetdefstring("bank_acc_type", pstAlgo->bank_acc_type, "");
    IBPgetdefstring("bank_full_name", pstAlgo->bank_full_name, "");
    IBPgetdefstring("bank_branch_name", pstAlgo->bank_branch_name, "");
    IBPgetdefstring("bank_areacode", pstAlgo->bank_areacode, "");
    IBPgetdefstring("cnaps_no", pstAlgo->cnaps_no, "");
    IBPgetdefstring("cnaps_bankno", pstAlgo->cnaps_bankno, "");
    IBPgetdefstring("currency", pstAlgo->currency, "CNY");
    IBPgetdefstring("cert_type", pstAlgo->cert_type, "");
    IBPgetdefstring("cert_no", pstAlgo->cert_no, "");
    IBPgetdefstring("mobile_no", pstAlgo->mobile_no, "");
    IBPgetdefstring("order_id", pstAlgo->order_id, "");
    IBPgetdefstring("product_name", pstAlgo->product_name, "");
    IBPgetdefstring("pay_kind", pstAlgo->pay_kind, "");
    IBPgetdefstring("status", szState, "success");

    if(strcmp(szState, "success") || 0 != pstAlgo->disc_cycle)
    {
        SYSError(RET_SYSTEM, "收到非成功(%s)或非S0(%d)的交易", szState, pstAlgo->disc_cycle);
        return ;
    }

    return ;
}

/**************************************************************************************************
    函 数 名：lAlgoSettle(TAlgoDtls *pstAlgo, TMchtFund *pstMcht, TSettleTxn *pstSettle)
    功能描述：
    返回说明：
        RC_SUCC
        RC_FAIL
 **************************************************************************************************/
long    CClear::lAlgoSettle(TAlgoDtls *pstAlgo, TMchtFund *pstMcht, TSettleTxn *pstSettle)
{
    dbMchtAcct  stAcct;
    dbMchtBase  stBase;
    dbMchtProd  stProd;

    if(RC_SUCC != lGetMchtBase(&stBase, pstAlgo->mert_no))
    {
        SYSError(RET_SYSTEM, "获取商户(%s)信息失败, err:(%s)", pstAlgo->mert_no, sGetError());
        return RC_FAIL;
    }

    if(strcmp(stBase.mcht_status, "ZC"))
    {
        SYSError(RET_SYSTEM, "商户(%s)状态(%s)异常, err:(%s)", pstAlgo->mert_no, 
            stBase.mcht_status, sGetError());
        return RC_FAIL;
    }

    //    获取商户产品费率
    if(RC_SUCC != lGetMchtPord(&stProd, pstAlgo->mert_no, pstAlgo->product_code))
    {
        SYSError(RET_SYSTEM, "获取商户(%s)(%s)信息失败, err:(%s)", pstAlgo->mert_no, 
            pstAlgo->product_code, sGetError());
        return RC_FAIL;
    }

    pstAlgo->brh_flag[0]  = ALGO_BRH_FLAG_NULL;
    pstAlgo->mcht_flag[0] = ALGO_MCHT_FLAG_SUCC;
    memcpy(pstAlgo->disc_id, stProd.base_disc, sizeof(pstAlgo->disc_id));

    if(RC_SUCC != nCalculateFee(stProd.base_disc, pstAlgo))
    {
        SYSError(RET_SYSTEM, "记录商户(%s)(%s)手续费失败, err:(%s)", pstAlgo->mert_no, 
            pstAlgo->disc_id, sGetError());
        return RC_FAIL;
    }
    
    memcpy(pstAlgo->algo_date, sGetTxDate(), sizeof(pstAlgo->algo_date) - 1);
    memcpy(pstAlgo->settle_num, BATCH_REAL_SETTLE, sizeof(pstAlgo->settle_num) - 1);
    memcpy(pstAlgo->stlm_date, pstAlgo->algo_date, sizeof(pstAlgo->stlm_date) - 1);
    memcpy(pstAlgo->create_time, sGetCurrentTime(), sizeof(pstAlgo->create_time) - 1 );

    pstMcht->tx_num = 1; 
    pstMcht->tx_amt = pstAlgo->tx_amt; 
    pstMcht->fee_amt = pstAlgo->fee_amt;
    pstMcht->settle_amt = pstMcht->tx_amt - pstMcht->fee_amt;
    memcpy(pstMcht->channel_no, pstAlgo->channel_no, sizeof(pstMcht->channel_no) - 1);
    memcpy(pstMcht->mert_no, pstAlgo->mert_no, sizeof(pstMcht->mert_no) - 1);
    memcpy(pstMcht->mert_nm, pstAlgo->mert_nm, sizeof(pstMcht->mert_nm) - 1);
    memcpy(pstMcht->tx_date, pstAlgo->tx_date, sizeof(pstMcht->tx_date) - 1);
    memcpy(pstMcht->settle_num, pstAlgo->settle_num, sizeof(pstMcht->settle_num) - 1);
    memcpy(pstMcht->settle_date, pstAlgo->algo_date, sizeof(pstMcht->settle_date) - 1);
    snprintf(pstMcht->id, sizeof(pstMcht->id), "%4s%s", pstMcht->channel_no, pstAlgo->id);
    snprintf(pstMcht->trace_no, sizeof(pstMcht->trace_no), "%7s%4s%08d", 
        pstMcht->settle_date + 1, pstMcht->channel_no, lGetSeqNo());
    memcpy(pstMcht->create_time, pstAlgo->create_time, sizeof(pstMcht->create_time));

    //    只有2中情况不需要往下执行  1、商户资金汇总为0、清算流水直接进入账失败表中
    if(fabs(pstMcht->settle_amt) < 0.0005)
    {
        pstMcht->dc_flag[0] = 'C';
        pstMcht->result_flag[0] = SETTLE_MCHT_SUCC;
        return RC_SUCC;
    }

    pstMcht->dc_flag[0] = 'D';
    pstMcht->result_flag[0] = SETTLE_MCHT_PUTREQ;    //    直接请求中

    pstSettle->settle_amt = pstMcht->settle_amt;
    memcpy(pstSettle->currency, "CNY", sizeof(pstSettle->currency));
    memcpy(pstSettle->settle_date, pstAlgo->algo_date, sizeof(pstMcht->settle_date));
    snprintf(pstSettle->id, sizeof(pstSettle->id), "%s%s%010d", pstMcht->channel_no, 
		sGetTxDate(), lGetSeqNo());
    snprintf(pstSettle->trace_no, sizeof(pstSettle->trace_no), "%s%012d", 
        pstSettle->settle_date + 1, lGetSeqNo());
    memcpy(pstSettle->pay_order, pstMcht->id, sizeof(pstSettle->pay_order) - 1);
    memcpy(pstSettle->tx_date, pstMcht->tx_date, sizeof(pstSettle->tx_date) - 1);
    memcpy(pstSettle->settle_num, pstMcht->settle_num, sizeof(pstSettle->settle_num) - 1);
    memcpy(pstSettle->channel_no, pstMcht->channel_no, sizeof(pstSettle->channel_no) - 1);
    memcpy(pstSettle->mert_no, pstMcht->mert_no, sizeof(pstSettle->mert_no) - 1);
    memcpy(pstSettle->mert_nm, pstMcht->mert_nm, sizeof(pstSettle->mert_nm) - 1);
    memcpy(pstSettle->pay_acct, sGetDbsEnv(pstSettle->channel_no), sizeof(pstSettle->pay_acct) - 1);

    if(RC_SUCC != lGetMchtAcct(&stAcct, pstSettle->mert_no))
    {
        SYSError(RET_SYSTEM, "获取商户(%s)账户信息失败, (%s)", pstSettle->mert_no, sGetError());
        return RC_FAIL;
    }

    pstSettle->result_flag[0] = SETTLE_IN_FLAG_PREQ;
    memcpy(pstSettle->acct_no, stAcct.acct_no, sizeof(pstSettle->acct_no) - 1);
    memcpy(pstSettle->acct_nm, stAcct.acct_nm, sizeof(pstSettle->acct_nm) - 1);
    memcpy(pstSettle->acct_type, stAcct.acct_type, sizeof(pstSettle->acct_type) - 1);
    memcpy(pstSettle->acct_bank_no, stAcct.acct_bank_no, sizeof(pstSettle->acct_bank_no) - 1);
    memcpy(pstSettle->acct_bank_nm, stAcct.acct_bank_nm, sizeof(pstSettle->acct_bank_nm) - 1);
    memcpy(pstSettle->acct_zbank_no, stAcct.acct_zbank_no, sizeof(pstSettle->acct_zbank_no) - 1);
    memcpy(pstSettle->acct_zbank_nm, stAcct.acct_zbank_nm, sizeof(pstSettle->acct_zbank_nm) - 1);
    memcpy(pstSettle->acct_zbank_addr, stAcct.acct_zbank_addr, sizeof(pstSettle->acct_zbank_addr) - 1);
    memcpy(pstSettle->acct_zbank_code, stAcct.acct_zbank_code, sizeof(pstSettle->acct_zbank_code) - 1);
    memcpy(pstSettle->acct_net_no, stAcct.acct_net_no, sizeof(pstSettle->acct_net_no) - 1);
    memcpy(pstSettle->acct_union_no, stAcct.acct_union_no, sizeof(pstSettle->acct_union_no) - 1);
    memcpy(pstSettle->cert_type, stAcct.cert_type, sizeof(pstSettle->cert_type) - 1);
    memcpy(pstSettle->cert_no, stAcct.cert_no, sizeof(pstSettle->cert_no) - 1);
    memcpy(pstSettle->resv_phone, stAcct.resv_phone, sizeof(pstSettle->resv_phone) - 1);
    memcpy(pstSettle->create_time, pstAlgo->create_time, sizeof(pstSettle->create_time) - 1);

// 帐号检查，如果异常，新增记录到入账失败表中
// stSettle.result_flag[0] = SETTLE_IN_FLAG_FAIL;


    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vGenSettleTxn()
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
void    CClear::vGenSettleTxn()
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 337 "settle_algo.pcpp"

    TAlgoDtls  stAlgo;
    TMchtFund  stMcht;
    TSettleTxn stSettle;
    /* EXEC SQL END DECLARE SECTION; */ 
#line 341 "settle_algo.pcpp"


    memset(&stAlgo, 0, sizeof(TAlgoDtls));
    memset(&stMcht, 0, sizeof(TMchtFund));
    memset(&stSettle, 0, sizeof(TSettleTxn));
    IBPputstring("TR_RETCODE", "s0-010"); 
    vFillAlgoDtl(&stAlgo);
    if(IBPIserr())    return;

    IBPrace("开始处理(%s)商户(%s)交易金额(%.2f)清分", stAlgo.id, stAlgo.mert_no, stAlgo.tx_amt);
    if(RC_SUCC != lAlgoSettle(&stAlgo, &stMcht, &stSettle))
        goto SettleError;
    
    /* EXEC SQL INSERT INTO TBL_ALGO_DTLS VALUES (:stAlgo); */ 
#line 354 "settle_algo.pcpp"

{
#line 354 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 354 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 354 "settle_algo.pcpp"
    sqlstm.arrsiz = 47;
#line 354 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 354 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 354 "settle_algo.pcpp"
    sqlstm.stmt = "insert into TBL_ALGO_DTLS  values (:s1 ,:s2 ,:s3 ,:s4 ,:\
s5 ,:s6 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18 \
,:s19 ,:s20 ,:s21 ,:s22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 ,:s31\
 ,:s32 ,:s33 ,:s34 ,:s35 ,:s36 ,:s37 ,:s38 ,:s39 ,:s40 ,:s41 ,:s42 ,:s43 ,:s4\
4 ,:s45 ,:s46 ,:s47 )";
#line 354 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 354 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )5;
#line 354 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 354 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 354 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)stAlgo.id;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )33;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)stAlgo.algo_date;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )9;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)stAlgo.stlm_date;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)stAlgo.tx_date;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )9;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)stAlgo.tx_time;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )7;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)stAlgo.channel_no;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )5;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)stAlgo.pay_type;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )9;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)stAlgo.seq_order;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )33;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)stAlgo.bank_code;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )101;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)stAlgo.mcht_flag;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )2;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[10] = (unsigned char  *)stAlgo.mcht_trace_no;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[10] = (unsigned long )21;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[10] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[10] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[10] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[10] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[10] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[10] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[11] = (unsigned char  *)stAlgo.brh_flag;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[11] = (unsigned long )2;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[11] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[11] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[11] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[11] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[11] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[11] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[12] = (unsigned char  *)stAlgo.brh_trace_no;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[12] = (unsigned long )21;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[12] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[12] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[12] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[12] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[12] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[12] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[13] = (unsigned char  *)&stAlgo.tx_amt;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[13] = (unsigned long )sizeof(double);
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[13] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[13] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[13] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[13] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[13] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[13] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[14] = (unsigned char  *)&stAlgo.fee_amt;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[14] = (unsigned long )sizeof(double);
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[14] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[14] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[14] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[14] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[14] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[14] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[15] = (unsigned char  *)&stAlgo.cup_fee;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[15] = (unsigned long )sizeof(double);
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[15] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[15] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[15] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[15] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[15] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[15] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[16] = (unsigned char  *)&stAlgo.base_amt;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[16] = (unsigned long )sizeof(double);
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[16] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[16] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[16] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[16] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[16] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[16] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[17] = (unsigned char  *)&stAlgo.brh_fee;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[17] = (unsigned long )sizeof(double);
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[17] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[17] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[17] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[17] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[17] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[17] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[18] = (unsigned char  *)&stAlgo.fee_type;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[18] = (unsigned long )sizeof(long);
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[18] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[18] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[18] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[18] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[18] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[18] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[19] = (unsigned char  *)&stAlgo.disc_cycle;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[19] = (unsigned long )sizeof(long);
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[19] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[19] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[19] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[19] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[19] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[19] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[20] = (unsigned char  *)stAlgo.settle_num;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[20] = (unsigned long )5;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[20] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[20] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[20] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[20] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[20] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[20] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[21] = (unsigned char  *)stAlgo.bank_card_no;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[21] = (unsigned long )31;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[21] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[21] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[21] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[21] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[21] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[21] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[22] = (unsigned char  *)stAlgo.user_name;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[22] = (unsigned long )31;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[22] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[22] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[22] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[22] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[22] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[22] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[23] = (unsigned char  *)stAlgo.bank_acc_type;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[23] = (unsigned long )4;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[23] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[23] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[23] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[23] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[23] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[23] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[24] = (unsigned char  *)stAlgo.bank_full_name;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[24] = (unsigned long )31;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[24] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[24] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[24] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[24] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[24] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[24] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[25] = (unsigned char  *)stAlgo.bank_branch_name;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[25] = (unsigned long )101;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[25] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[25] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[25] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[25] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[25] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[25] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[26] = (unsigned char  *)stAlgo.bank_areacode;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[26] = (unsigned long )5;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[26] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[26] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[26] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[26] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[26] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[26] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[27] = (unsigned char  *)stAlgo.cnaps_no;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[27] = (unsigned long )16;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[27] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[27] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[27] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[27] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[27] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[27] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[28] = (unsigned char  *)stAlgo.cnaps_bankno;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[28] = (unsigned long )16;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[28] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[28] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[28] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[28] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[28] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[28] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[29] = (unsigned char  *)stAlgo.currency;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[29] = (unsigned long )6;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[29] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[29] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[29] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[29] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[29] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[29] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[30] = (unsigned char  *)&stAlgo.card_type;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[30] = (unsigned long )sizeof(long);
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[30] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[30] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[30] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[30] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[30] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[30] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[31] = (unsigned char  *)stAlgo.app_code;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[31] = (unsigned long )33;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[31] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[31] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[31] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[31] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[31] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[31] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[32] = (unsigned char  *)stAlgo.mert_no;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[32] = (unsigned long )21;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[32] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[32] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[32] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[32] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[32] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[32] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[33] = (unsigned char  *)stAlgo.mert_nm;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[33] = (unsigned long )201;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[33] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[33] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[33] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[33] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[33] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[33] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[34] = (unsigned char  *)stAlgo.brh_code;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[34] = (unsigned long )31;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[34] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[34] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[34] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[34] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[34] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[34] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[35] = (unsigned char  *)stAlgo.settle_brhno;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[35] = (unsigned long )31;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[35] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[35] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[35] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[35] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[35] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[35] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[36] = (unsigned char  *)stAlgo.disc_id;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[36] = (unsigned long )31;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[36] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[36] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[36] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[36] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[36] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[36] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[37] = (unsigned char  *)stAlgo.cert_type;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[37] = (unsigned long )3;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[37] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[37] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[37] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[37] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[37] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[37] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[38] = (unsigned char  *)stAlgo.cert_no;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[38] = (unsigned long )21;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[38] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[38] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[38] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[38] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[38] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[38] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[39] = (unsigned char  *)stAlgo.mobile_no;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[39] = (unsigned long )12;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[39] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[39] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[39] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[39] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[39] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[39] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[40] = (unsigned char  *)stAlgo.order_id;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[40] = (unsigned long )33;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[40] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[40] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[40] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[40] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[40] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[40] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[41] = (unsigned char  *)stAlgo.product_code;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[41] = (unsigned long )21;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[41] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[41] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[41] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[41] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[41] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[41] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[42] = (unsigned char  *)stAlgo.product_name;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[42] = (unsigned long )91;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[42] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[42] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[42] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[42] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[42] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[42] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[43] = (unsigned char  *)stAlgo.pay_kind;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[43] = (unsigned long )3;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[43] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[43] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[43] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[43] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[43] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[43] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[44] = (unsigned char  *)stAlgo.remark;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[44] = (unsigned long )101;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[44] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[44] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[44] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[44] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[44] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[44] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[45] = (unsigned char  *)stAlgo.create_time;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[45] = (unsigned long )15;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[45] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[45] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[45] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[45] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[45] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[45] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstv[46] = (unsigned char  *)stAlgo.update_time;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhstl[46] = (unsigned long )15;
#line 354 "settle_algo.pcpp"
    sqlstm.sqhsts[46] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqindv[46] = (         short *)0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqinds[46] = (         int  )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqharm[46] = (unsigned long )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqadto[46] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqtdso[46] = (unsigned short )0;
#line 354 "settle_algo.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 354 "settle_algo.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 354 "settle_algo.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 354 "settle_algo.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 354 "settle_algo.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 354 "settle_algo.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 354 "settle_algo.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 354 "settle_algo.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 354 "settle_algo.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 354 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 354 "settle_algo.pcpp"
}

#line 354 "settle_algo.pcpp"

    if(SQLCODE)
    {
        SYSError(RET_DATABASE, "新增清分记录(%s)失败, %s", stAlgo.id, sDbsError());
        goto SettleError;
    }

    /* EXEC SQL INSERT INTO TBL_MCHT_FUND VALUES (:stMcht); */ 
#line 361 "settle_algo.pcpp"

{
#line 361 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 361 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 361 "settle_algo.pcpp"
    sqlstm.arrsiz = 47;
#line 361 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 361 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 361 "settle_algo.pcpp"
    sqlstm.stmt = "insert into TBL_MCHT_FUND  values (:s1 ,:s2 ,:s3 ,:s4 ,:\
s5 ,:s6 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18 \
,:s19 ,:s20 ,:s21 ,:s22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 )";
#line 361 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 361 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )208;
#line 361 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 361 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 361 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)stMcht.id;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )33;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)stMcht.settle_date;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )9;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)stMcht.settle_num;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )5;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)stMcht.channel_no;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )5;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)stMcht.in_date;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )9;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)stMcht.tx_date;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )9;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)stMcht.batch_no;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )21;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)stMcht.trace_no;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )21;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)stMcht.result_flag;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )2;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)stMcht.result_desc;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )61;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[10] = (unsigned char  *)&stMcht.tx_num;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[10] = (unsigned long )sizeof(long);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[10] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[10] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[10] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[10] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[10] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[10] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[11] = (unsigned char  *)&stMcht.tx_amt;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[11] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[11] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[11] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[11] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[11] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[11] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[11] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[12] = (unsigned char  *)&stMcht.credit_num;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[12] = (unsigned long )sizeof(long);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[12] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[12] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[12] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[12] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[12] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[12] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[13] = (unsigned char  *)&stMcht.credit_amt;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[13] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[13] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[13] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[13] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[13] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[13] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[13] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[14] = (unsigned char  *)&stMcht.credit_percent;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[14] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[14] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[14] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[14] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[14] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[14] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[14] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[15] = (unsigned char  *)&stMcht.fee_amt;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[15] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[15] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[15] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[15] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[15] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[15] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[15] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[16] = (unsigned char  *)&stMcht.other_fee;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[16] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[16] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[16] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[16] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[16] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[16] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[16] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[17] = (unsigned char  *)&stMcht.repair_amt;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[17] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[17] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[17] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[17] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[17] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[17] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[17] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[18] = (unsigned char  *)&stMcht.unrepair_amt;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[18] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[18] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[18] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[18] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[18] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[18] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[18] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[19] = (unsigned char  *)&stMcht.uperr_amt;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[19] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[19] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[19] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[19] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[19] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[19] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[19] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[20] = (unsigned char  *)stMcht.dc_flag;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[20] = (unsigned long )2;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[20] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[20] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[20] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[20] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[20] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[20] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[21] = (unsigned char  *)&stMcht.settle_amt;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[21] = (unsigned long )sizeof(double);
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[21] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[21] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[21] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[21] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[21] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[21] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[22] = (unsigned char  *)stMcht.mert_no;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[22] = (unsigned long )21;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[22] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[22] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[22] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[22] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[22] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[22] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[23] = (unsigned char  *)stMcht.mert_nm;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[23] = (unsigned long )201;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[23] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[23] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[23] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[23] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[23] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[23] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[24] = (unsigned char  *)stMcht.order_req;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[24] = (unsigned long )33;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[24] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[24] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[24] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[24] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[24] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[24] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[25] = (unsigned char  *)stMcht.order_time;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[25] = (unsigned long )15;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[25] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[25] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[25] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[25] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[25] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[25] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[26] = (unsigned char  *)stMcht.res;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[26] = (unsigned long )129;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[26] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[26] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[26] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[26] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[26] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[26] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[27] = (unsigned char  *)stMcht.pay_time;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[27] = (unsigned long )15;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[27] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[27] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[27] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[27] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[27] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[27] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[28] = (unsigned char  *)stMcht.create_time;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[28] = (unsigned long )15;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[28] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[28] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[28] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[28] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[28] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[28] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstv[29] = (unsigned char  *)stMcht.update_time;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhstl[29] = (unsigned long )15;
#line 361 "settle_algo.pcpp"
    sqlstm.sqhsts[29] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqindv[29] = (         short *)0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqinds[29] = (         int  )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqharm[29] = (unsigned long )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqadto[29] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqtdso[29] = (unsigned short )0;
#line 361 "settle_algo.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 361 "settle_algo.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 361 "settle_algo.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 361 "settle_algo.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 361 "settle_algo.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 361 "settle_algo.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 361 "settle_algo.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 361 "settle_algo.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 361 "settle_algo.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 361 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 361 "settle_algo.pcpp"
}

#line 361 "settle_algo.pcpp"

    if(SQLCODE)
    {
        SYSError(RET_DATABASE, "汇总商户资金(%s)失败, %s", stMcht.id, sDbsError());
        goto SettleError;
    }

    if(SETTLE_MCHT_SUCC == stMcht.result_flag[0])
        return ;

    /* EXEC SQL INSERT INTO TBL_SETTLE_TXN VALUES (:stSettle); */ 
#line 371 "settle_algo.pcpp"

{
#line 371 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 371 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 371 "settle_algo.pcpp"
    sqlstm.arrsiz = 47;
#line 371 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 371 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 371 "settle_algo.pcpp"
    sqlstm.stmt = "insert into TBL_SETTLE_TXN  values (:s1 ,:s2 ,:s3 ,:s4 ,\
:s5 ,:s6 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18\
 ,:s19 ,:s20 ,:s21 ,:s22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 ,:s3\
1 ,:s32 ,:s33 ,:s34 ,:s35 ,:s36 ,:s37 ,:s38 ,:s39 ,:s40 ,:s41 ,:s42 ,:s43 ,:s\
44 )";
#line 371 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 371 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )343;
#line 371 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 371 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 371 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)stSettle.id;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)stSettle.pay_order;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)stSettle.settle_date;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)stSettle.in_date;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )9;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)stSettle.tx_date;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )9;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)stSettle.settle_num;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )5;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)stSettle.channel_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )5;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)stSettle.batch_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)stSettle.trace_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)stSettle.app_type;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )2;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[10] = (unsigned char  *)stSettle.app_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[10] = (unsigned long )21;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[10] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[10] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[10] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[10] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[10] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[10] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[11] = (unsigned char  *)stSettle.app_nm;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[11] = (unsigned long )201;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[11] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[11] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[11] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[11] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[11] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[11] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[12] = (unsigned char  *)stSettle.order_req;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[12] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[12] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[12] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[12] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[12] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[12] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[12] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[13] = (unsigned char  *)stSettle.order_time;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[13] = (unsigned long )15;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[13] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[13] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[13] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[13] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[13] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[13] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[14] = (unsigned char  *)stSettle.pay_acct;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[14] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[14] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[14] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[14] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[14] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[14] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[14] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[15] = (unsigned char  *)&stSettle.settle_amt;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[15] = (unsigned long )sizeof(double);
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[15] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[15] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[15] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[15] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[15] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[15] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[16] = (unsigned char  *)stSettle.currency;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[16] = (unsigned long )6;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[16] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[16] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[16] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[16] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[16] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[16] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[17] = (unsigned char  *)stSettle.result_flag;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[17] = (unsigned long )2;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[17] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[17] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[17] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[17] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[17] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[17] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[18] = (unsigned char  *)stSettle.status;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[18] = (unsigned long )31;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[18] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[18] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[18] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[18] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[18] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[18] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[19] = (unsigned char  *)stSettle.status_msg;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[19] = (unsigned long )257;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[19] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[19] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[19] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[19] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[19] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[19] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[20] = (unsigned char  *)stSettle.bank_status;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[20] = (unsigned long )31;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[20] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[20] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[20] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[20] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[20] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[20] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[21] = (unsigned char  *)stSettle.bank_status_msg;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[21] = (unsigned long )257;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[21] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[21] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[21] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[21] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[21] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[21] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[22] = (unsigned char  *)stSettle.bank_batch_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[22] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[22] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[22] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[22] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[22] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[22] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[22] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[23] = (unsigned char  *)stSettle.bank_seqno;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[23] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[23] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[23] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[23] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[23] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[23] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[23] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[24] = (unsigned char  *)stSettle.resv_phone;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[24] = (unsigned long )12;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[24] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[24] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[24] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[24] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[24] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[24] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[25] = (unsigned char  *)stSettle.acct_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[25] = (unsigned long )101;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[25] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[25] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[25] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[25] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[25] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[25] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[26] = (unsigned char  *)stSettle.acct_nm;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[26] = (unsigned long )101;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[26] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[26] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[26] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[26] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[26] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[26] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[27] = (unsigned char  *)stSettle.acct_type;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[27] = (unsigned long )4;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[27] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[27] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[27] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[27] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[27] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[27] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[28] = (unsigned char  *)stSettle.acct_bank_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[28] = (unsigned long )21;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[28] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[28] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[28] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[28] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[28] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[28] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[29] = (unsigned char  *)stSettle.acct_bank_nm;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[29] = (unsigned long )101;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[29] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[29] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[29] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[29] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[29] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[29] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[30] = (unsigned char  *)stSettle.acct_zbank_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[30] = (unsigned long )21;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[30] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[30] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[30] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[30] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[30] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[30] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[31] = (unsigned char  *)stSettle.acct_zbank_nm;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[31] = (unsigned long )101;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[31] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[31] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[31] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[31] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[31] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[31] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[32] = (unsigned char  *)stSettle.acct_zbank_addr;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[32] = (unsigned long )161;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[32] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[32] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[32] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[32] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[32] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[32] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[33] = (unsigned char  *)stSettle.acct_zbank_code;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[33] = (unsigned long )11;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[33] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[33] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[33] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[33] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[33] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[33] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[34] = (unsigned char  *)stSettle.acct_union_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[34] = (unsigned long )21;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[34] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[34] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[34] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[34] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[34] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[34] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[35] = (unsigned char  *)stSettle.acct_net_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[35] = (unsigned long )21;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[35] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[35] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[35] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[35] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[35] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[35] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[36] = (unsigned char  *)stSettle.remark;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[36] = (unsigned long )101;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[36] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[36] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[36] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[36] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[36] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[36] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[37] = (unsigned char  *)stSettle.cert_type;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[37] = (unsigned long )3;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[37] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[37] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[37] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[37] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[37] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[37] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[38] = (unsigned char  *)stSettle.cert_no;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[38] = (unsigned long )33;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[38] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[38] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[38] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[38] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[38] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[38] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[39] = (unsigned char  *)&stSettle.card_type;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[39] = (unsigned long )sizeof(long);
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[39] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[39] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[39] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[39] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[39] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[39] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[40] = (unsigned char  *)stSettle.product_code;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[40] = (unsigned long )21;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[40] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[40] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[40] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[40] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[40] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[40] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[41] = (unsigned char  *)stSettle.ext_properties;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[41] = (unsigned long )1001;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[41] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[41] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[41] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[41] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[41] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[41] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[42] = (unsigned char  *)stSettle.create_time;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[42] = (unsigned long )15;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[42] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[42] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[42] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[42] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[42] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[42] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstv[43] = (unsigned char  *)stSettle.update_time;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhstl[43] = (unsigned long )15;
#line 371 "settle_algo.pcpp"
    sqlstm.sqhsts[43] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqindv[43] = (         short *)0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqinds[43] = (         int  )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqharm[43] = (unsigned long )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqadto[43] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqtdso[43] = (unsigned short )0;
#line 371 "settle_algo.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 371 "settle_algo.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 371 "settle_algo.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 371 "settle_algo.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 371 "settle_algo.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 371 "settle_algo.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 371 "settle_algo.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 371 "settle_algo.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 371 "settle_algo.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 371 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 371 "settle_algo.pcpp"
}

#line 371 "settle_algo.pcpp"

    if(SQLCODE)
    {
        SYSError(RET_DATABASE, "生成代付清算流水(%s)失败, %s", stSettle.id, sDbsError());
        goto SettleError;
    }

    if(SETTLE_IN_FLAG_FAIL == stSettle.result_flag[0])
        return ;

    IBPputstring("TR_RETCODE", "s0-000"); 
    vPutSettleTxn(&stSettle);
    lSettleMonIns(&stSettle);
    IBPputstring("s0_flow", "1");
    return ;

SettleError:
    /* EXEC SQL ROLLBACK WORK; */ 
#line 388 "settle_algo.pcpp"

{
#line 388 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 388 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 388 "settle_algo.pcpp"
    sqlstm.arrsiz = 47;
#line 388 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 388 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 388 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 388 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )534;
#line 388 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 388 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 388 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 388 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 388 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 388 "settle_algo.pcpp"
}

#line 388 "settle_algo.pcpp"

    nInsertError(&stAlgo);    
    return ;
}

/**************************************************************************************************
    函 数 名：vPutSettleTxn(TSettleTxn *pstSettleTxn)
    功能描述：将结算信息压入变量池
    返回说明：
        无
**************************************************************************************************/
void    CClear::vPutSettleTxn(TSettleTxn *pstSettle)
{
    vResetUser();
    IBPputlong("loop", 1);
    IBPputstring("id", pstSettle->id);
    IBPputsenstr("acct_no", pstSettle->acct_no, FILED_SENNAM);
    IBPputsenstr("acct_nm", pstSettle->acct_nm, FILED_SENACT);
    IBPputsenstr("cert_no", pstSettle->cert_no, FILED_SENACT);
    IBPputstring("pay_acct", pstSettle->pay_acct);
    IBPputstring("cert_type", pstSettle->cert_type);
    if('0' == pstSettle->acct_type[0])
        pstSettle->acct_type[0] = '1';
    else
        pstSettle->acct_type[0] = '2';
    IBPputstring("acct_type", pstSettle->acct_type);
    IBPputdouble("settle_amt", pstSettle->settle_amt);
    IBPputstring("acct_net_no", pstSettle->acct_net_no);
    IBPputstring("acct_union_no", pstSettle->acct_union_no);
    IBPputstring("acct_bank_no", pstSettle->acct_bank_no);
    IBPputstring("acct_bank_nm", pstSettle->acct_bank_nm);
    IBPputstring("acct_zbank_no", pstSettle->acct_zbank_no);
    IBPputstring("acct_zbank_nm", pstSettle->acct_zbank_nm);
    IBPputstring("acct_zbank_addr", pstSettle->acct_zbank_addr);
    IBPputstring("acct_zbank_code", pstSettle->acct_zbank_code);
}

/**************************************************************************************************
    函 数 名：lSettleMonIns(TSettleTxn *pstSettle)
    功能描述：写TBL_SETTLE_MON表
    返回说明：
        RC_SUCC      --成功
        RC_FAIL      --失败
 **************************************************************************************************/
long    CClear::lSettleMonIns(TSettleTxn *pstSettle)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 434 "settle_algo.pcpp"

    TSettleMon  stMon;
    /* EXEC SQL END DECLARE SECTION; */ 
#line 436 "settle_algo.pcpp"


    memset(&stMon, 0, sizeof(TSettleMon));

    stMon.que_cnt = 5;
    stMon.result_flag[0] = SETTLE_IN_FLAG_PREQ;
    memcpy(stMon.id, pstSettle->id, sizeof(stMon.id));
    memcpy(stMon.mark, pstSettle->pay_acct, sizeof(stMon.mark));
    memcpy(stMon.rec_crt_ts, pstSettle->create_time, sizeof(stMon.rec_crt_ts));
    memcpy(stMon.rec_upd_ts, pstSettle->create_time, sizeof(stMon.rec_upd_ts));
    memcpy(stMon.settle_date, pstSettle->settle_date, sizeof(stMon.settle_date));

    /* EXEC SQL INSERT INTO TBL_SETTLE_MON(ID, SETTLE_DATE, QUE_CNT, RESULT_FLAG, MARK, RESV,
        REC_CRT_TS) VALUES(:stMon.id, :stMon.settle_date, :stMon.que_cnt, :stMon.result_flag, 
        :stMon.mark, :stMon.resv, :stMon.rec_crt_ts); */ 
#line 450 "settle_algo.pcpp"

{
#line 448 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 448 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 448 "settle_algo.pcpp"
    sqlstm.arrsiz = 47;
#line 448 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 448 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 448 "settle_algo.pcpp"
    sqlstm.stmt = "insert into TBL_SETTLE_MON (ID,SETTLE_DATE,QUE_CNT,RESUL\
T_FLAG,MARK,RESV,REC_CRT_TS) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6)";
#line 448 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 448 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )549;
#line 448 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 448 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 448 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)(stMon.id);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )23;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)(stMon.settle_date);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )9;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)&(stMon.que_cnt);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )sizeof(long);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)(stMon.result_flag);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )2;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)(stMon.mark);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )65;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)&(stMon.resv);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )sizeof(long);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)(stMon.rec_crt_ts);
#line 448 "settle_algo.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )15;
#line 448 "settle_algo.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 448 "settle_algo.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 448 "settle_algo.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 448 "settle_algo.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 448 "settle_algo.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 448 "settle_algo.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 448 "settle_algo.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 448 "settle_algo.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 448 "settle_algo.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 448 "settle_algo.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 448 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 448 "settle_algo.pcpp"
}

#line 450 "settle_algo.pcpp"

    if (SQLCODE)
    {
        SYSError(RET_DATABASE, "新增监控流水(%s)失败, err:(%d)(%s)", stMon.id,
            SQLCODE, sDbsError());
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：nInsertError(TAlgoDtls *pstAlgo)
    功能描述：写TBL_STLM_ERR表数据
    返回说明：
        RC_SUCC      --成功
        RC_FAIL      --失败
**************************************************************************************************/
long    CClear::nInsertError(TAlgoDtls *pstAlgo)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 470 "settle_algo.pcpp"

    long        lRec;
    TOrderErr   stErr;
    /* EXEC SQL END DECLARE SECTION; */ 
#line 473 "settle_algo.pcpp"


    memset(&stErr, 0, sizeof(TOrderErr));
    stErr.tx_amt         = stErr.local_amt;
    stErr.local_amt      = pstAlgo->tx_amt;
    stErr.card_type      = pstAlgo->card_type;
    stErr.disc_cycle     = pstAlgo->disc_cycle;
    stErr.err_type[0]    = ERR_CUP_MCHT_UNUSUAL;
    stErr.stlm_type[0]   = ERR_CUP_NOT_CHECK;
    stErr.result_flag[0] = STLM_CUP_NOT_DEAL;
    memcpy(stErr.id, pstAlgo->id, sizeof(stErr.id) - 1);
    memcpy(stErr.tx_date, pstAlgo->tx_date, 8);
    memcpy(stErr.tx_time, pstAlgo->tx_time, 6);
    memcpy(stErr.channel_no, pstAlgo->channel_no, sizeof(stErr.channel_no) - 1);
    memcpy(stErr.pay_type, pstAlgo->pay_type, sizeof(stErr.pay_type) - 1);
    memcpy(stErr.seq_order, pstAlgo->seq_order, sizeof(stErr.seq_order) - 1);
    memcpy(stErr.bank_code, pstAlgo->bank_code, sizeof(stErr.bank_code) - 1);
    memcpy(stErr.bank_card_no, pstAlgo->bank_card_no, sizeof(stErr.bank_card_no) - 1);
    memcpy(stErr.user_name, pstAlgo->user_name, sizeof(stErr.user_name) - 1);
    memcpy(stErr.bank_acc_type, pstAlgo->bank_acc_type, sizeof(stErr.bank_acc_type) - 1);
    memcpy(stErr.bank_full_name, pstAlgo->bank_full_name, sizeof(stErr.bank_full_name) - 1);
    memcpy(stErr.bank_branch_name, pstAlgo->bank_branch_name, sizeof(stErr.bank_branch_name) - 1);
    memcpy(stErr.bank_areacode, pstAlgo->bank_areacode, sizeof(stErr.bank_areacode) - 1);
    memcpy(stErr.cnaps_no, pstAlgo->cnaps_no, sizeof(stErr.cnaps_no) - 1);
    memcpy(stErr.cnaps_bankno, pstAlgo->cnaps_bankno, sizeof(stErr.cnaps_bankno) - 1);
    memcpy(stErr.currency, pstAlgo->currency, sizeof(stErr.currency) - 1);
    memcpy(stErr.remark, pstAlgo->remark, sizeof(stErr.remark) - 1);
    memcpy(stErr.app_code, pstAlgo->app_code, sizeof(stErr.app_code) - 1);
    memcpy(stErr.mert_no, pstAlgo->mert_no, sizeof(stErr.mert_no) - 1);
    memcpy(stErr.mert_nm, pstAlgo->mert_nm, sizeof(stErr.mert_nm) - 1);
    memcpy(stErr.brh_code, pstAlgo->brh_code, sizeof(stErr.brh_code) - 1);
    memcpy(stErr.cert_type, pstAlgo->cert_type, sizeof(stErr.cert_type) - 1);
    memcpy(stErr.cert_no, pstAlgo->cert_no, sizeof(stErr.cert_no) - 1);
    memcpy(stErr.mobile_no, pstAlgo->mobile_no, sizeof(stErr.mobile_no) - 1);
    memcpy(stErr.order_id, pstAlgo->order_id, sizeof(stErr.order_id) - 1);
    memcpy(stErr.product_code, pstAlgo->product_code, sizeof(stErr.product_code) - 1);
    memcpy(stErr.product_name, pstAlgo->product_name, sizeof(stErr.product_name) - 1);
    memcpy(stErr.pay_kind, pstAlgo->pay_kind, sizeof(stErr.pay_kind) - 1);
    memcpy(stErr.err_desc, "商户信息异常", sizeof(stErr.err_desc) - 1);
    memcpy(stErr.settle_num, BATCH_REAL_SETTLE, 4);
    memcpy(stErr.create_time, sGetCurrentTime(), 14);
    IBPgetdefstring("status", stErr.status, "");

    /* EXEC SQL SELECT COUNT(*) INTO :lRec FROM TBL_ALGO_DTL WHERE ID = :stErr.id; */ 
#line 516 "settle_algo.pcpp"

{
#line 516 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 516 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 516 "settle_algo.pcpp"
    sqlstm.arrsiz = 47;
#line 516 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 516 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 516 "settle_algo.pcpp"
    sqlstm.stmt = "select count(*)  into :b0  from TBL_ALGO_DTL where ID=:b\
1";
#line 516 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 516 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )592;
#line 516 "settle_algo.pcpp"
    sqlstm.selerr = (unsigned short)1;
#line 516 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 516 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 516 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)&lRec;
#line 516 "settle_algo.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )sizeof(long);
#line 516 "settle_algo.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)(stErr.id);
#line 516 "settle_algo.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )33;
#line 516 "settle_algo.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 516 "settle_algo.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 516 "settle_algo.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 516 "settle_algo.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 516 "settle_algo.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 516 "settle_algo.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 516 "settle_algo.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 516 "settle_algo.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 516 "settle_algo.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 516 "settle_algo.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 516 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 516 "settle_algo.pcpp"
}

#line 516 "settle_algo.pcpp"

    if(lRec > 0)
    {
        IBPputstring("TR_RETCODE", "s0-001");    //  返回给POSP为登记成功
        SYSError(RET_DATABASE, "流水(%s)已清分, 无法登记", stErr.id);
        return RC_FAIL;
    }

    /* EXEC SQL INSERT INTO TBL_ORDER_ERR VALUES (:stErr); */ 
#line 524 "settle_algo.pcpp"

{
#line 524 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 524 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 524 "settle_algo.pcpp"
    sqlstm.arrsiz = 50;
#line 524 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 524 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 524 "settle_algo.pcpp"
    sqlstm.stmt = "insert into TBL_ORDER_ERR  values (:s1 ,:s2 ,:s3 ,:s4 ,:\
s5 ,:s6 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18 \
,:s19 ,:s20 ,:s21 ,:s22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 ,:s31\
 ,:s32 ,:s33 ,:s34 ,:s35 ,:s36 ,:s37 ,:s38 ,:s39 ,:s40 ,:s41 ,:s42 ,:s43 ,:s4\
4 ,:s45 ,:s46 ,:s47 ,:s48 ,:s49 ,:s50 )";
#line 524 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 524 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )615;
#line 524 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 524 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 524 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)stErr.id;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )33;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)stErr.stlm_date;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )9;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)stErr.tx_date;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)stErr.tx_time;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )7;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)stErr.channel_no;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )5;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)stErr.result_flag;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )2;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)stErr.opr_date;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )9;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)&stErr.opr_no;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )sizeof(long);
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)stErr.next_date;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )9;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)stErr.next_do;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )1025;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[10] = (unsigned char  *)stErr.err_type;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[10] = (unsigned long )2;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[10] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[10] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[10] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[10] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[10] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[10] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[11] = (unsigned char  *)stErr.err_desc;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[11] = (unsigned long )1025;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[11] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[11] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[11] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[11] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[11] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[11] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[12] = (unsigned char  *)stErr.stlm_type;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[12] = (unsigned long )2;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[12] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[12] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[12] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[12] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[12] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[12] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[13] = (unsigned char  *)stErr.pay_type;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[13] = (unsigned long )9;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[13] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[13] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[13] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[13] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[13] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[13] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[14] = (unsigned char  *)stErr.seq_order;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[14] = (unsigned long )33;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[14] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[14] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[14] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[14] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[14] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[14] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[15] = (unsigned char  *)stErr.bank_code;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[15] = (unsigned long )101;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[15] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[15] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[15] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[15] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[15] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[15] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[16] = (unsigned char  *)&stErr.tx_amt;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[16] = (unsigned long )sizeof(double);
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[16] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[16] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[16] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[16] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[16] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[16] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[17] = (unsigned char  *)&stErr.local_amt;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[17] = (unsigned long )sizeof(double);
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[17] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[17] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[17] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[17] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[17] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[17] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[18] = (unsigned char  *)&stErr.cups_amt;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[18] = (unsigned long )sizeof(double);
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[18] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[18] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[18] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[18] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[18] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[18] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[19] = (unsigned char  *)&stErr.fee_amt;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[19] = (unsigned long )sizeof(double);
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[19] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[19] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[19] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[19] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[19] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[19] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[20] = (unsigned char  *)&stErr.cup_fee;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[20] = (unsigned long )sizeof(double);
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[20] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[20] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[20] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[20] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[20] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[20] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[21] = (unsigned char  *)&stErr.disc_cycle;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[21] = (unsigned long )sizeof(long);
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[21] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[21] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[21] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[21] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[21] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[21] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[22] = (unsigned char  *)stErr.settle_num;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[22] = (unsigned long )5;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[22] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[22] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[22] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[22] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[22] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[22] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[23] = (unsigned char  *)stErr.bank_card_no;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[23] = (unsigned long )31;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[23] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[23] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[23] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[23] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[23] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[23] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[24] = (unsigned char  *)stErr.user_name;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[24] = (unsigned long )31;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[24] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[24] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[24] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[24] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[24] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[24] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[25] = (unsigned char  *)stErr.bank_acc_type;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[25] = (unsigned long )4;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[25] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[25] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[25] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[25] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[25] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[25] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[26] = (unsigned char  *)stErr.bank_full_name;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[26] = (unsigned long )31;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[26] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[26] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[26] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[26] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[26] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[26] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[27] = (unsigned char  *)stErr.bank_branch_name;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[27] = (unsigned long )101;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[27] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[27] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[27] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[27] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[27] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[27] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[28] = (unsigned char  *)stErr.bank_areacode;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[28] = (unsigned long )5;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[28] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[28] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[28] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[28] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[28] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[28] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[29] = (unsigned char  *)stErr.cnaps_no;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[29] = (unsigned long )16;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[29] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[29] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[29] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[29] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[29] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[29] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[30] = (unsigned char  *)stErr.cnaps_bankno;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[30] = (unsigned long )16;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[30] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[30] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[30] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[30] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[30] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[30] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[31] = (unsigned char  *)stErr.currency;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[31] = (unsigned long )6;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[31] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[31] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[31] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[31] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[31] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[31] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[32] = (unsigned char  *)stErr.remark;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[32] = (unsigned long )101;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[32] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[32] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[32] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[32] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[32] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[32] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[33] = (unsigned char  *)stErr.status;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[33] = (unsigned long )31;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[33] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[33] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[33] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[33] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[33] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[33] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[34] = (unsigned char  *)stErr.vid_respcode;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[34] = (unsigned long )31;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[34] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[34] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[34] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[34] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[34] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[34] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[35] = (unsigned char  *)stErr.vid_respmsg;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[35] = (unsigned long )101;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[35] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[35] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[35] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[35] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[35] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[35] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[36] = (unsigned char  *)&stErr.card_type;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[36] = (unsigned long )sizeof(long);
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[36] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[36] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[36] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[36] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[36] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[36] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[37] = (unsigned char  *)stErr.app_code;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[37] = (unsigned long )33;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[37] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[37] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[37] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[37] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[37] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[37] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[38] = (unsigned char  *)stErr.mert_no;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[38] = (unsigned long )21;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[38] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[38] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[38] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[38] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[38] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[38] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[39] = (unsigned char  *)stErr.mert_nm;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[39] = (unsigned long )201;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[39] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[39] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[39] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[39] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[39] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[39] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[40] = (unsigned char  *)stErr.brh_code;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[40] = (unsigned long )31;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[40] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[40] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[40] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[40] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[40] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[40] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[41] = (unsigned char  *)stErr.cert_type;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[41] = (unsigned long )3;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[41] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[41] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[41] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[41] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[41] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[41] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[42] = (unsigned char  *)stErr.cert_no;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[42] = (unsigned long )21;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[42] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[42] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[42] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[42] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[42] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[42] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[43] = (unsigned char  *)stErr.mobile_no;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[43] = (unsigned long )12;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[43] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[43] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[43] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[43] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[43] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[43] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[44] = (unsigned char  *)stErr.order_id;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[44] = (unsigned long )33;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[44] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[44] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[44] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[44] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[44] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[44] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[45] = (unsigned char  *)stErr.product_code;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[45] = (unsigned long )21;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[45] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[45] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[45] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[45] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[45] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[45] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[46] = (unsigned char  *)stErr.product_name;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[46] = (unsigned long )91;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[46] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[46] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[46] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[46] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[46] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[46] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[47] = (unsigned char  *)stErr.pay_kind;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[47] = (unsigned long )3;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[47] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[47] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[47] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[47] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[47] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[47] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[48] = (unsigned char  *)stErr.create_time;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[48] = (unsigned long )15;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[48] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[48] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[48] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[48] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[48] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[48] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstv[49] = (unsigned char  *)stErr.update_time;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhstl[49] = (unsigned long )15;
#line 524 "settle_algo.pcpp"
    sqlstm.sqhsts[49] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqindv[49] = (         short *)0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqinds[49] = (         int  )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqharm[49] = (unsigned long )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqadto[49] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqtdso[49] = (unsigned short )0;
#line 524 "settle_algo.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 524 "settle_algo.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 524 "settle_algo.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 524 "settle_algo.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 524 "settle_algo.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 524 "settle_algo.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 524 "settle_algo.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 524 "settle_algo.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 524 "settle_algo.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 524 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 524 "settle_algo.pcpp"
}

#line 524 "settle_algo.pcpp"

    if (SQLCODE)
    {
        if(-1 == SQLCODE)
            IBPputstring("TR_RETCODE", "s0-001");    //  返回给POSP为登记成功
        SYSError(RET_DATABASE, "新增差错明细(%s)失败, (%s)", stErr.id, sDbsError());
        return RC_FAIL;
    }
    /* EXEC SQL COMMIT WORK; */ 
#line 532 "settle_algo.pcpp"

{
#line 532 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 532 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 532 "settle_algo.pcpp"
    sqlstm.arrsiz = 50;
#line 532 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 532 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 532 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 532 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )830;
#line 532 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 532 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 532 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 532 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 532 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 532 "settle_algo.pcpp"
}

#line 532 "settle_algo.pcpp"


    IBPputstring("TR_RETCODE", "s0-000");    //  返回给POSP为登记成功
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：vChkSettleTxn()
    功能描述：
    返回说明：
        无
**************************************************************************************************/
void    CClear::vChkSettleTxn()
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 546 "settle_algo.pcpp"

    long    lCount = 0;
    char    szIdx[30];
    /* EXEC SQL END DECLARE SECTION; */ 
#line 549 "settle_algo.pcpp"


    memset(szIdx, 0, sizeof(szIdx));
    IBPgetstring("id", szIdx, "id主键");
    if(IBPIserr())
        return ;

    /* EXEC SQL SELECT COUNT(*) INTO :lCount FROM (
        SELECT A.ID FROM TBL_ALGO_DTL A WHERE A.ID = :szIdx
        UNION
        SELECT E.ID FROM TBL_STLM_ERR E WHERE E.ID = :szIdx); */ 
#line 559 "settle_algo.pcpp"

{
#line 556 "settle_algo.pcpp"
    struct sqlexd sqlstm;
#line 556 "settle_algo.pcpp"
    sqlstm.sqlvsn = 12;
#line 556 "settle_algo.pcpp"
    sqlstm.arrsiz = 50;
#line 556 "settle_algo.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 556 "settle_algo.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 556 "settle_algo.pcpp"
    sqlstm.stmt = "select count(*)  into :b0  from (select A.ID  from TBL_A\
LGO_DTL A where A.ID=:b1 union select E.ID  from TBL_STLM_ERR E where E.ID=:b\
1) ";
#line 556 "settle_algo.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 556 "settle_algo.pcpp"
    sqlstm.offset = (unsigned int  )845;
#line 556 "settle_algo.pcpp"
    sqlstm.selerr = (unsigned short)1;
#line 556 "settle_algo.pcpp"
    sqlstm.cud = sqlcud0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 556 "settle_algo.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 556 "settle_algo.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)&lCount;
#line 556 "settle_algo.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )sizeof(long);
#line 556 "settle_algo.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)szIdx;
#line 556 "settle_algo.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )30;
#line 556 "settle_algo.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)szIdx;
#line 556 "settle_algo.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )30;
#line 556 "settle_algo.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 556 "settle_algo.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 556 "settle_algo.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 556 "settle_algo.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 556 "settle_algo.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 556 "settle_algo.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 556 "settle_algo.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 556 "settle_algo.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 556 "settle_algo.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 556 "settle_algo.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 556 "settle_algo.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 556 "settle_algo.pcpp"
}

#line 559 "settle_algo.pcpp"

    if(SQLCODE)
    {
        SYSError(RET_DATABASE, "查询清分表(%s)记录失败, err:(%d)(%s)",
            szIdx, SQLCODE, sDbsError());
        return ;
    }

    if(1 != lCount)
    {
        SYSError(RET_DATABASE, "无此(%s)流水", szIdx);
        return ;
    }

    return ;
}

/*************************************************************************************************
    code end
**************************************************************************************************/
