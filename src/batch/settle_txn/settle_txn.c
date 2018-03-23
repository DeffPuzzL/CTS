
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
           char  filnam[16];
};
static const struct sqlcxp sqlfpn =
{
    15,
    "settle_txn.pcpp"
};


static unsigned int sqlctx = 2599452;


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
   unsigned char  *sqhstv[47];
   unsigned long  sqhstl[47];
            int   sqhsts[47];
            short *sqindv[47];
            int   sqinds[47];
   unsigned long  sqharm[47];
   unsigned long  *sqharc[47];
   unsigned short  sqadto[47];
   unsigned short  sqtdso[47];
} sqlstm = {12,47};

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

 static const char *sq0009 = 
"select *  from TBL_MCHT_FUND where (((SETTLE_DATE=:b0 and RESULT_FLAG in (:\
b1,:b2)) and SETTLE_NUM=:b3) and MERT_NO=:b4)           ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4274,871,0,0,
5,0,0,1,97,0,5,108,0,0,5,5,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
40,0,0,2,178,0,5,207,0,0,10,10,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,
5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
95,0,0,3,302,0,4,225,0,0,45,1,0,1,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
2,4,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,
0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,
0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,1,
5,0,0,
290,0,0,4,111,0,5,249,0,0,6,6,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,
5,0,0,
329,0,0,5,314,0,4,264,0,0,47,1,0,1,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,4,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,
0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,
0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,5,0,0,1,5,0,0,
532,0,0,6,926,0,3,278,0,0,5,5,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
567,0,0,7,45,0,5,295,0,0,1,1,0,1,0,1,5,0,0,
586,0,0,8,470,0,5,306,0,0,28,28,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,
0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,
0,0,1,5,0,0,1,5,0,0,1,5,0,0,
713,0,0,9,131,0,9,426,0,0,5,5,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
748,0,0,9,0,0,13,436,0,0,30,0,0,1,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,4,0,0,2,3,0,0,2,4,0,0,2,4,0,0,
2,4,0,0,2,4,0,0,2,4,0,0,2,4,0,0,2,4,0,0,2,5,0,0,2,4,0,0,2,5,0,0,2,5,0,0,2,5,0,
0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
883,0,0,10,291,0,3,454,0,0,44,44,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,
0,1,4,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,
0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,
5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1074,0,0,9,0,0,15,459,0,0,0,0,0,1,0,
1089,0,0,11,99,0,5,464,0,0,5,5,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1124,0,0,9,0,0,15,474,0,0,0,0,0,1,0,
1139,0,0,12,0,0,29,481,0,0,0,0,0,1,0,
1154,0,0,13,0,0,29,483,0,0,0,0,0,1,0,
1169,0,0,9,0,0,15,484,0,0,0,0,0,1,0,
1184,0,0,14,302,0,4,524,0,0,45,1,0,1,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,
0,2,4,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,
0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
1,5,0,0,
1379,0,0,15,41,0,2,548,0,0,1,1,0,1,0,1,5,0,0,
1398,0,0,16,178,0,5,550,0,0,10,10,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,
0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1453,0,0,17,111,0,5,584,0,0,6,6,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1,5,0,0,
1492,0,0,18,314,0,4,599,0,0,47,1,0,1,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,
0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,4,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,
0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
2,5,0,0,2,5,0,0,1,5,0,0,
1695,0,0,19,926,0,3,613,0,0,5,5,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1730,0,0,20,45,0,5,630,0,0,1,1,0,1,0,1,5,0,0,
1749,0,0,21,470,0,5,641,0,0,28,28,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,
0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,
0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,
5,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
};


#line 1 "settle_txn.pcpp"
/**************************************************************************************************
    文 件 名：demo.pc
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：demo实现业务模块
 **************************************************************************************************/
#include    "settle_txn.h"
#include    "algo.h"

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

#line 12 "settle_txn.pcpp"

/* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 13 "settle_txn.pcpp"

#include    "tbl_mcht_fund.h"
#include    "tbl_settle_txn.h"
#include    "tbl_settle_err.h"
/* EXEC SQL END DECLARE SECTION; */ 
#line 17 "settle_txn.pcpp"


/**************************************************************************************************
    方法定义
 **************************************************************************************************/
CSettle::stEvent CSettle::m_stEvent[] =
{
    {"settle",               &CSettle::vGenSettleTxn },
    {"update",               &CSettle::vUpdSettleTxn },
    { "", NULL }
};

/**************************************************************************************************
    函 数 名：CSettle()
    功能描述：构造函数
    返回说明：
 **************************************************************************************************/
CSettle::CSettle()
{

}

/**************************************************************************************************
    函 数 名：~CSettle()
    功能描述：析构函数
    返回说明：
 **************************************************************************************************/
CSettle::~CSettle()
{


}

/**************************************************************************************************
    函 数 名：lRunObject(char* in_szClass, char* in_szAction)
    功能描述：运行类中的方法
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
long    CSettle::lRunObject(char *in_szClass, char *in_szAction)
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
    函 数 名：lUpdateError(TSettleTxn *pstSettle)
    功能描述：直接将清算流水重置失败
    返回说明：
        RC_SUCC
        RC_FAIL
 **************************************************************************************************/
long    CSettle::lUpdateError(TSettleTxn *pstSettle)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 97 "settle_txn.pcpp"

    TSettleTxn   stSettle;
    /* EXEC SQL END DECLARE SECTION; */ 
#line 99 "settle_txn.pcpp"

    GCts         *pstCts = (GCts *)pGetGloble();
    SATvm        *pstSavm = (SATvm *)pGetSATvm();

    memcpy(&stSettle, pstSettle, sizeof(TSettleTxn));
    stSettle.result_flag[0] = SETTLE_IN_FLAG_FAIL;
    strncpy(stSettle.status, pstCts->m_szRetCode, sizeof(stSettle.status) - 1);
    strncpy(stSettle.status_msg, pstCts->m_szRetMsg, sizeof(stSettle.status_msg) - 1);
    
    /* EXEC SQL UPDATE TBL_SETTLE_TXN SET
        RESULT_FLAG     = :stSettle.result_flag,
        STATUS          = :stSettle.status,
        STATUS_MSG      = :stSettle.status_msg,
        UPDATE_TIME     = :stSettle.update_time
    WHERE ID = :stSettle.id; */ 
#line 113 "settle_txn.pcpp"

{
#line 108 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 108 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 108 "settle_txn.pcpp"
    sqlstm.arrsiz = 5;
#line 108 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 108 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 108 "settle_txn.pcpp"
    sqlstm.stmt = "update TBL_SETTLE_TXN  set RESULT_FLAG=:b0,STATUS=:b1,ST\
ATUS_MSG=:b2,UPDATE_TIME=:b3 where ID=:b4";
#line 108 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 108 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )5;
#line 108 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 108 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 108 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.result_flag);
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )2;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)(stSettle.status);
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )31;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)(stSettle.status_msg);
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )257;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.update_time);
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )15;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.id);
#line 108 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )33;
#line 108 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 108 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 108 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 108 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 108 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 108 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 108 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 108 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 108 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 108 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 108 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 108 "settle_txn.pcpp"
}

#line 113 "settle_txn.pcpp"

    if (SQLCODE)
    {
        SYSError(RET_DATABASE, "更新清算流水(%s)失败, err:(%d)(%s)", stSettle.id,     
            SQLCODE, sDbsError());
        return RC_FAIL;
    }
  
    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lInsertSettle(TSettleTxn *pstSettle, TMchtFund *pstFund, long lSeqNo)
    功能描述：初始化清算流水
    返回说明：
        RC_SUCC
        RC_FAIL
 **************************************************************************************************/
long    CSettle::lInsertSettle(TSettleTxn *pstSettle, TMchtFund *pstFund, long lSeqNo)
{
    dbMchtAcct   stAcct;

    pstSettle->settle_amt = pstFund->settle_amt;
    strncpy(pstSettle->currency, "CNY", sizeof(pstSettle->currency));
    snprintf(pstSettle->id, sizeof(pstSettle->id), "%s%012d", sGetTxDate(), lSeqNo);
    snprintf(pstSettle->trace_no, sizeof(pstSettle->trace_no), "%s%012d", pstSettle->settle_date + 1, lSeqNo);
    strncpy(pstSettle->pay_order, pstFund->id, sizeof(pstSettle->pay_order) - 1);
    strncpy(pstSettle->tx_date, pstFund->tx_date, sizeof(pstSettle->tx_date) - 1);
    strncpy(pstSettle->settle_num, pstFund->settle_num, sizeof(pstSettle->settle_num) - 1);
    strncpy(pstSettle->channel_no, pstFund->channel_no, sizeof(pstSettle->channel_no) - 1);
    strncpy(pstSettle->mert_no, pstFund->mert_no, sizeof(pstSettle->mert_no) - 1);
    strncpy(pstSettle->mert_nm, pstFund->mert_nm, sizeof(pstSettle->mert_nm) - 1);
    strncpy(pstSettle->pay_acct, sGetDbsEnv(pstSettle->channel_no), sizeof(pstSettle->pay_acct) - 1);

    if(RC_SUCC != lGetMchtAcct(&stAcct, pstSettle->mert_no))
    {
        SYSError(RET_SYSTEM, "获取商户(%s)账户信息失败, err:(%s)", pstSettle->mert_no, 
            sGetError());
        return RC_FAIL;
    }

    pstSettle->result_flag[0] = SETTLE_IN_FLAG_PREQ;
    strncpy(pstSettle->acct_no, stAcct.acct_no, sizeof(pstSettle->acct_no));
    strncpy(pstSettle->acct_nm, stAcct.acct_nm, sizeof(pstSettle->acct_nm));
    strncpy(pstSettle->acct_type, stAcct.acct_type, sizeof(pstSettle->acct_type));
    strncpy(pstSettle->acct_bank_no, stAcct.acct_bank_no, sizeof(pstSettle->acct_bank_no));
    strncpy(pstSettle->acct_bank_nm, stAcct.acct_bank_nm, sizeof(pstSettle->acct_bank_nm));
    strncpy(pstSettle->acct_zbank_no, stAcct.acct_zbank_no, sizeof(pstSettle->acct_zbank_no));
    strncpy(pstSettle->acct_zbank_nm, stAcct.acct_zbank_nm, sizeof(pstSettle->acct_zbank_nm));
    strncpy(pstSettle->acct_zbank_addr, stAcct.acct_zbank_addr, sizeof(pstSettle->acct_zbank_addr));
    strncpy(pstSettle->acct_zbank_code, stAcct.acct_zbank_code, sizeof(pstSettle->acct_zbank_code));
    strncpy(pstSettle->acct_net_no, stAcct.acct_net_no, sizeof(pstSettle->acct_net_no));
    strncpy(pstSettle->acct_union_no, stAcct.acct_union_no, sizeof(pstSettle->acct_union_no));
    strncpy(pstSettle->cert_type, stAcct.cert_type, sizeof(pstSettle->cert_type) - 1);
    strncpy(pstSettle->cert_no, stAcct.cert_no, sizeof(pstSettle->cert_no) - 1);
    strncpy(pstSettle->resv_phone, stAcct.resv_phone, sizeof(pstSettle->resv_phone) - 1);
    strncpy(pstSettle->create_time, sGetCurrentTime(), sizeof(pstSettle->create_time) - 1);

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lUpdateSettle()
    功能描述：更新商户资金清算状态
    返回说明：
        RC_SUCC
        RC_FAIL
 **************************************************************************************************/
long    CSettle::lUpdateSettle(TSettleTxn *pstSettle)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 183 "settle_txn.pcpp"

    TMchtFund    stFund;
    TSettleErr   stError;
    TSettleTxn   stSettle;
    /* EXEC SQL END DECLARE SECTION; */ 
#line 187 "settle_txn.pcpp"


    memset(&stFund, 0, sizeof(TMchtFund));
    memset(&stError, 0, sizeof(TSettleErr));
    memcpy(&stSettle, pstSettle, sizeof(TSettleTxn));
    IBPgetdefstring("pay_BankSeqNo", stSettle.bank_seqno, "");
    IBPgetdefstring("pay_BankBatNo", stSettle.bank_batch_no, "");
    IBPgetdefstring("pay_BankCode", stSettle.bank_status, "");
    IBPgetdefstring("pay_BankMsg", stSettle.bank_status_msg, "");
    IBPgetdefstring("pay_BankBatchNo", stSettle.bank_batch_no, "");
    IBPgetdefstring("pay_BankSeqNo", stSettle.bank_seqno, "");
    IBPgetstring("pay_OrderSerial", stSettle.id, "idx");
    IBPgetstring("pay_Status", stSettle.result_flag, "业务响应码");
    IBPgetstring("pay_BussCode", stSettle.status, "业务响应码");
    IBPgetstring("pay_BussMsg", stSettle.status_msg, "业务响应信息");
    strncpy(stSettle.in_date, sGetTxDate(), sizeof(stSettle.in_date));
    strncpy(stSettle.update_time, sGetCurrentTime(), sizeof(stSettle.update_time));
    if(IBPIserr())
        return lUpdateError(pstSettle);

    /* EXEC SQL UPDATE TBL_SETTLE_TXN SET
        RESULT_FLAG     = :stSettle.result_flag,
        IN_DATE         = :stSettle.in_date,    
        STATUS          = :stSettle.status,
        STATUS_MSG      = :stSettle.status_msg,
        BANK_STATUS     = :stSettle.bank_status,
        BANK_STATUS_MSG = :stSettle.bank_status_msg,
        BANK_BATCH_NO   = :stSettle.bank_batch_no,
        BANK_SEQNO      = :stSettle.bank_seqno,
        UPDATE_TIME     = :stSettle.update_time
    WHERE ID = :stSettle.id; */ 
#line 217 "settle_txn.pcpp"

{
#line 207 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 207 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 207 "settle_txn.pcpp"
    sqlstm.arrsiz = 10;
#line 207 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 207 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 207 "settle_txn.pcpp"
    sqlstm.stmt = "update TBL_SETTLE_TXN  set RESULT_FLAG=:b0,IN_DATE=:b1,S\
TATUS=:b2,STATUS_MSG=:b3,BANK_STATUS=:b4,BANK_STATUS_MSG=:b5,BANK_BATCH_NO=:b\
6,BANK_SEQNO=:b7,UPDATE_TIME=:b8 where ID=:b9";
#line 207 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 207 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )40;
#line 207 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 207 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 207 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.result_flag);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )2;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)(stSettle.in_date);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )9;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)(stSettle.status);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )31;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.status_msg);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )257;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.bank_status);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )31;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)(stSettle.bank_status_msg);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )257;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)(stSettle.bank_batch_no);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )33;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)(stSettle.bank_seqno);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )33;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)(stSettle.update_time);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )15;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)(stSettle.id);
#line 207 "settle_txn.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )33;
#line 207 "settle_txn.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 207 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 207 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 207 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 207 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 207 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 207 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 207 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 207 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 207 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 207 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 207 "settle_txn.pcpp"
}

#line 217 "settle_txn.pcpp"

    if (SQLCODE)
    {
       SYSError(RET_DATABASE, "更新清算流水(%s)失败, err:(%d)(%s)", stSettle.id,     
           SQLCODE, sDbsError());
       return RC_FAIL;
    }

    /* EXEC SQL SELECT * INTO :stSettle FROM TBL_SETTLE_TXN WHERE ID = :stSettle.id; */ 
#line 225 "settle_txn.pcpp"

{
#line 225 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 225 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 225 "settle_txn.pcpp"
    sqlstm.arrsiz = 45;
#line 225 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 225 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 225 "settle_txn.pcpp"
    sqlstm.stmt = "select * into :s1 ,:s2 ,:s3 ,:s4 ,:s5 ,:s6 ,:s7 ,:s8 ,:s\
9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18 ,:s19 ,:s20 ,:s21 ,:s\
22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 ,:s31 ,:s32 ,:s33 ,:s34 ,:\
s35 ,:s36 ,:s37 ,:s38 ,:s39 ,:s40 ,:s41 ,:s42 ,:s43 ,:s44   from TBL_SETTLE_T\
XN where ID=:b1";
#line 225 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 225 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )95;
#line 225 "settle_txn.pcpp"
    sqlstm.selerr = (unsigned short)1;
#line 225 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 225 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 225 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)stSettle.id;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)stSettle.pay_order;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)stSettle.settle_date;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)stSettle.in_date;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )9;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)stSettle.tx_date;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )9;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)stSettle.settle_num;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )5;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)stSettle.channel_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )5;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)stSettle.batch_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)stSettle.trace_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)stSettle.app_type;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )2;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[10] = (unsigned char  *)stSettle.app_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[10] = (unsigned long )21;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[10] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[10] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[10] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[10] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[10] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[10] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[11] = (unsigned char  *)stSettle.app_nm;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[11] = (unsigned long )201;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[11] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[11] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[11] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[11] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[11] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[11] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[12] = (unsigned char  *)stSettle.order_req;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[12] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[12] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[12] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[12] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[12] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[12] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[12] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[13] = (unsigned char  *)stSettle.order_time;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[13] = (unsigned long )15;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[13] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[13] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[13] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[13] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[13] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[13] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[14] = (unsigned char  *)stSettle.pay_acct;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[14] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[14] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[14] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[14] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[14] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[14] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[14] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[15] = (unsigned char  *)&stSettle.settle_amt;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[15] = (unsigned long )sizeof(double);
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[15] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[15] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[15] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[15] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[15] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[15] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[16] = (unsigned char  *)stSettle.currency;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[16] = (unsigned long )6;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[16] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[16] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[16] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[16] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[16] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[16] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[17] = (unsigned char  *)stSettle.result_flag;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[17] = (unsigned long )2;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[17] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[17] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[17] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[17] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[17] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[17] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[18] = (unsigned char  *)stSettle.status;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[18] = (unsigned long )31;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[18] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[18] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[18] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[18] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[18] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[18] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[19] = (unsigned char  *)stSettle.status_msg;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[19] = (unsigned long )257;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[19] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[19] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[19] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[19] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[19] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[19] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[20] = (unsigned char  *)stSettle.bank_status;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[20] = (unsigned long )31;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[20] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[20] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[20] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[20] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[20] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[20] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[21] = (unsigned char  *)stSettle.bank_status_msg;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[21] = (unsigned long )257;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[21] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[21] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[21] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[21] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[21] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[21] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[22] = (unsigned char  *)stSettle.bank_batch_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[22] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[22] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[22] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[22] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[22] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[22] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[22] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[23] = (unsigned char  *)stSettle.bank_seqno;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[23] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[23] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[23] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[23] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[23] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[23] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[23] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[24] = (unsigned char  *)stSettle.resv_phone;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[24] = (unsigned long )12;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[24] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[24] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[24] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[24] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[24] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[24] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[25] = (unsigned char  *)stSettle.acct_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[25] = (unsigned long )101;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[25] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[25] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[25] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[25] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[25] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[25] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[26] = (unsigned char  *)stSettle.acct_nm;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[26] = (unsigned long )101;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[26] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[26] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[26] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[26] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[26] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[26] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[27] = (unsigned char  *)stSettle.acct_type;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[27] = (unsigned long )4;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[27] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[27] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[27] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[27] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[27] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[27] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[28] = (unsigned char  *)stSettle.acct_bank_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[28] = (unsigned long )21;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[28] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[28] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[28] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[28] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[28] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[28] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[29] = (unsigned char  *)stSettle.acct_bank_nm;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[29] = (unsigned long )101;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[29] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[29] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[29] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[29] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[29] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[29] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[30] = (unsigned char  *)stSettle.acct_zbank_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[30] = (unsigned long )21;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[30] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[30] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[30] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[30] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[30] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[30] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[31] = (unsigned char  *)stSettle.acct_zbank_nm;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[31] = (unsigned long )101;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[31] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[31] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[31] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[31] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[31] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[31] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[32] = (unsigned char  *)stSettle.acct_zbank_addr;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[32] = (unsigned long )161;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[32] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[32] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[32] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[32] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[32] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[32] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[33] = (unsigned char  *)stSettle.acct_zbank_code;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[33] = (unsigned long )11;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[33] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[33] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[33] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[33] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[33] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[33] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[34] = (unsigned char  *)stSettle.acct_union_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[34] = (unsigned long )21;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[34] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[34] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[34] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[34] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[34] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[34] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[35] = (unsigned char  *)stSettle.acct_net_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[35] = (unsigned long )21;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[35] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[35] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[35] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[35] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[35] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[35] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[36] = (unsigned char  *)stSettle.remark;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[36] = (unsigned long )101;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[36] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[36] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[36] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[36] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[36] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[36] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[37] = (unsigned char  *)stSettle.cert_type;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[37] = (unsigned long )3;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[37] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[37] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[37] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[37] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[37] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[37] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[38] = (unsigned char  *)stSettle.cert_no;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[38] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[38] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[38] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[38] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[38] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[38] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[38] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[39] = (unsigned char  *)&stSettle.card_type;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[39] = (unsigned long )sizeof(long);
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[39] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[39] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[39] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[39] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[39] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[39] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[40] = (unsigned char  *)stSettle.product_code;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[40] = (unsigned long )21;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[40] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[40] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[40] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[40] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[40] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[40] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[41] = (unsigned char  *)stSettle.ext_properties;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[41] = (unsigned long )1001;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[41] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[41] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[41] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[41] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[41] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[41] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[42] = (unsigned char  *)stSettle.create_time;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[42] = (unsigned long )15;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[42] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[42] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[42] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[42] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[42] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[42] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[43] = (unsigned char  *)stSettle.update_time;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[43] = (unsigned long )15;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[43] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[43] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[43] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[43] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[43] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[43] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstv[44] = (unsigned char  *)(stSettle.id);
#line 225 "settle_txn.pcpp"
    sqlstm.sqhstl[44] = (unsigned long )33;
#line 225 "settle_txn.pcpp"
    sqlstm.sqhsts[44] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqindv[44] = (         short *)0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqinds[44] = (         int  )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqharm[44] = (unsigned long )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqadto[44] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqtdso[44] = (unsigned short )0;
#line 225 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 225 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 225 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 225 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 225 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 225 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 225 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 225 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 225 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 225 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 225 "settle_txn.pcpp"
}

#line 225 "settle_txn.pcpp"

    if(SQLCODE)
    {
       SYSError(RET_DATABASE, "获取清算流水(%s)失败, err:(%d)(%s)", stSettle.id,     
            SQLCODE, sDbsError());
       return RC_FAIL;
    }

    if(SETTLE_IN_FLAG_SUCC == stSettle.result_flag[0])
    {
        stFund.result_flag[0] = SETTLE_MCHT_SUCC;
        stError.err_type[0] = ERROR_ER_SETTLE_SUCC;
    }
    else if(SETTLE_IN_FLAG_FAIL == stSettle.result_flag[0]) 
    {
        stFund.result_flag[0] = SETTLE_MCHT_ERROR;
        stError.err_type[0] = ERROR_ER_SETTLE_FAIL;
    }
    else
    {
        stFund.result_flag[0] = SETTLE_MCHT_PUTREQ;
        stError.err_type[0] = ERROR_ER_SETTLE_UNKOWN;
    }

    /* EXEC SQL UPDATE TBL_MCHT_FUND SET
        RESULT_FLAG = :stSettle.result_flag,    
        RESULT_DESC = :stSettle.status_msg,
        IN_DATE     = :stSettle.in_date,    
        PAY_TIME    = :stSettle.update_time,    
        UPDATE_TIME = :stSettle.update_time
    WHERE ID = :stSettle.pay_order; */ 
#line 255 "settle_txn.pcpp"

{
#line 249 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 249 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 249 "settle_txn.pcpp"
    sqlstm.arrsiz = 45;
#line 249 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 249 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 249 "settle_txn.pcpp"
    sqlstm.stmt = "update TBL_MCHT_FUND  set RESULT_FLAG=:b0,RESULT_DESC=:b\
1,IN_DATE=:b2,PAY_TIME=:b3,UPDATE_TIME=:b3 where ID=:b5";
#line 249 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 249 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )290;
#line 249 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 249 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 249 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.result_flag);
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )2;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)(stSettle.status_msg);
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )257;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)(stSettle.in_date);
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.update_time);
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )15;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.update_time);
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )15;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)(stSettle.pay_order);
#line 249 "settle_txn.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )33;
#line 249 "settle_txn.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 249 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 249 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 249 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 249 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 249 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 249 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 249 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 249 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 249 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 249 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 249 "settle_txn.pcpp"
}

#line 255 "settle_txn.pcpp"

    if(SQLCODE || EFFECT_NUM != 1)
    {
        SYSError(RET_DATABASE, "更新商户汇总(%s)失败, err:(%d)(%s)", stSettle.mert_no, 
            SQLCODE, sDbsError());
        return RC_FAIL;
    } 

    //    处理差错TBL_SETTLE_ERR表记录
    /* EXEC SQL SELECT * INTO :stError FROM TBL_SETTLE_ERR WHERE ID = :stSettle.pay_order; */ 
#line 264 "settle_txn.pcpp"

{
#line 264 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 264 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 264 "settle_txn.pcpp"
    sqlstm.arrsiz = 47;
#line 264 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 264 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 264 "settle_txn.pcpp"
    sqlstm.stmt = "select * into :s1 ,:s2 ,:s3 ,:s4 ,:s5 ,:s6 ,:s7 ,:s8 ,:s\
9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18 ,:s19 ,:s20 ,:s21 ,:s\
22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 ,:s31 ,:s32 ,:s33 ,:s34 ,:\
s35 ,:s36 ,:s37 ,:s38 ,:s39 ,:s40 ,:s41 ,:s42 ,:s43 ,:s44 ,:s45 ,:s46   from \
TBL_SETTLE_ERR where ID=:b1";
#line 264 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 264 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )329;
#line 264 "settle_txn.pcpp"
    sqlstm.selerr = (unsigned short)1;
#line 264 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 264 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 264 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)stError.id;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )33;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)stError.settle_date;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )9;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)stError.in_date;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)stError.tx_date;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )9;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)stError.settle_num;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )5;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)stError.channel_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )5;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)stError.batch_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )33;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)stError.trace_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )33;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)stError.do_flag;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )2;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)stError.opr_date;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )9;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[10] = (unsigned char  *)&stError.opr_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[10] = (unsigned long )sizeof(long);
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[10] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[10] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[10] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[10] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[10] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[10] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[11] = (unsigned char  *)stError.next_date;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[11] = (unsigned long )9;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[11] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[11] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[11] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[11] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[11] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[11] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[12] = (unsigned char  *)stError.next_num;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[12] = (unsigned long )5;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[12] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[12] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[12] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[12] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[12] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[12] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[13] = (unsigned char  *)stError.next_cup;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[13] = (unsigned long )5;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[13] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[13] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[13] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[13] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[13] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[13] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[14] = (unsigned char  *)stError.next_do;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[14] = (unsigned long )1025;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[14] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[14] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[14] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[14] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[14] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[14] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[15] = (unsigned char  *)stError.err_type;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[15] = (unsigned long )2;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[15] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[15] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[15] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[15] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[15] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[15] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[16] = (unsigned char  *)stError.mert_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[16] = (unsigned long )21;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[16] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[16] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[16] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[16] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[16] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[16] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[17] = (unsigned char  *)stError.mert_nm;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[17] = (unsigned long )201;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[17] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[17] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[17] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[17] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[17] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[17] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[18] = (unsigned char  *)stError.order_req;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[18] = (unsigned long )33;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[18] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[18] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[18] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[18] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[18] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[18] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[19] = (unsigned char  *)stError.order_time;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[19] = (unsigned long )15;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[19] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[19] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[19] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[19] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[19] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[19] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[20] = (unsigned char  *)&stError.settle_amt;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[20] = (unsigned long )sizeof(double);
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[20] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[20] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[20] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[20] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[20] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[20] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[21] = (unsigned char  *)stError.currency;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[21] = (unsigned long )6;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[21] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[21] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[21] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[21] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[21] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[21] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[22] = (unsigned char  *)stError.status;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[22] = (unsigned long )31;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[22] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[22] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[22] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[22] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[22] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[22] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[23] = (unsigned char  *)stError.status_msg;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[23] = (unsigned long )257;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[23] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[23] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[23] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[23] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[23] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[23] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[24] = (unsigned char  *)stError.bank_status;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[24] = (unsigned long )31;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[24] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[24] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[24] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[24] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[24] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[24] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[25] = (unsigned char  *)stError.bank_status_msg;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[25] = (unsigned long )257;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[25] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[25] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[25] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[25] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[25] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[25] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[26] = (unsigned char  *)stError.bank_batch_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[26] = (unsigned long )33;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[26] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[26] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[26] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[26] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[26] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[26] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[27] = (unsigned char  *)stError.bank_seqno;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[27] = (unsigned long )33;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[27] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[27] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[27] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[27] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[27] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[27] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[28] = (unsigned char  *)stError.resv_phone;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[28] = (unsigned long )12;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[28] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[28] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[28] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[28] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[28] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[28] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[29] = (unsigned char  *)stError.acct_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[29] = (unsigned long )101;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[29] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[29] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[29] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[29] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[29] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[29] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[30] = (unsigned char  *)stError.acct_nm;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[30] = (unsigned long )101;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[30] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[30] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[30] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[30] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[30] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[30] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[31] = (unsigned char  *)stError.acct_type;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[31] = (unsigned long )4;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[31] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[31] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[31] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[31] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[31] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[31] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[32] = (unsigned char  *)stError.acct_bank_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[32] = (unsigned long )21;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[32] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[32] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[32] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[32] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[32] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[32] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[33] = (unsigned char  *)stError.acct_bank_nm;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[33] = (unsigned long )101;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[33] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[33] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[33] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[33] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[33] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[33] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[34] = (unsigned char  *)stError.acct_zbank_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[34] = (unsigned long )21;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[34] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[34] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[34] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[34] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[34] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[34] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[35] = (unsigned char  *)stError.acct_zbank_nm;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[35] = (unsigned long )101;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[35] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[35] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[35] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[35] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[35] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[35] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[36] = (unsigned char  *)stError.acct_zbank_addr;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[36] = (unsigned long )161;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[36] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[36] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[36] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[36] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[36] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[36] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[37] = (unsigned char  *)stError.acct_zbank_code;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[37] = (unsigned long )11;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[37] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[37] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[37] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[37] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[37] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[37] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[38] = (unsigned char  *)stError.acct_union_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[38] = (unsigned long )21;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[38] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[38] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[38] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[38] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[38] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[38] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[39] = (unsigned char  *)stError.acct_net_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[39] = (unsigned long )21;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[39] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[39] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[39] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[39] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[39] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[39] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[40] = (unsigned char  *)stError.remark;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[40] = (unsigned long )101;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[40] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[40] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[40] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[40] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[40] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[40] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[41] = (unsigned char  *)stError.cert_type;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[41] = (unsigned long )3;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[41] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[41] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[41] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[41] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[41] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[41] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[42] = (unsigned char  *)stError.cert_no;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[42] = (unsigned long )33;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[42] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[42] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[42] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[42] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[42] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[42] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[43] = (unsigned char  *)stError.ext_properties;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[43] = (unsigned long )1001;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[43] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[43] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[43] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[43] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[43] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[43] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[44] = (unsigned char  *)stError.create_time;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[44] = (unsigned long )15;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[44] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[44] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[44] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[44] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[44] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[44] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[45] = (unsigned char  *)stError.update_time;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[45] = (unsigned long )15;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[45] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[45] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[45] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[45] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[45] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[45] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstv[46] = (unsigned char  *)(stSettle.pay_order);
#line 264 "settle_txn.pcpp"
    sqlstm.sqhstl[46] = (unsigned long )33;
#line 264 "settle_txn.pcpp"
    sqlstm.sqhsts[46] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqindv[46] = (         short *)0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqinds[46] = (         int  )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqharm[46] = (unsigned long )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqadto[46] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqtdso[46] = (unsigned short )0;
#line 264 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 264 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 264 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 264 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 264 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 264 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 264 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 264 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 264 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 264 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 264 "settle_txn.pcpp"
}

#line 264 "settle_txn.pcpp"

    if(SQLERR)
    {
        SYSError(RET_DATABASE, "查询户汇总(%s)入账失败表失败, err:(%d)(%s)", stSettle.mert_no,
            SQLCODE, sDbsError());
        return RC_FAIL;
    }
    else if(SQLNOTFOUND)
    {
        if(SETTLE_IN_FLAG_SUCC == stSettle.result_flag[0])
            return RC_SUCC;
        
        stError.do_flag[0] = SETTLE_ERR_NOT_DEAL;

        /* EXEC SQL INSERT INTO TBL_SETTLE_ERR (ID, SETTLE_DATE, IN_DATE, TX_DATE, SETTLE_NUM, 
            CHANNEL_NO, BATCH_NO, TRACE_NO, DO_FLAG, ERR_TYPE, MERT_NO, MERT_NM, ORDER_REQ, 
            ORDER_TIME, SETTLE_AMT, CURRENCY, STATUS, STATUS_MSG, BANK_STATUS, BANK_STATUS_MSG, 
            BANK_BATCH_NO, BANK_SEQNO, RESV_PHONE, ACCT_NO, ACCT_NM, ACCT_TYPE, ACCT_BANK_NO, 
            ACCT_BANK_NM, ACCT_ZBANK_NO, ACCT_ZBANK_NM, ACCT_ZBANK_ADDR, ACCT_ZBANK_CODE, 
            ACCT_UNION_NO, ACCT_NET_NO, REMARK, CERT_TYPE, CERT_NO, EXT_PROPERTIES, CREATE_TIME) 
        SELECT PAY_ORDER, SETTLE_DATE, :stSettle.in_date, TX_DATE, SETTLE_NUM, CHANNEL_NO, 
            BATCH_NO, TRACE_NO, :stError.do_flag, :stError.err_type, MERT_NO, MERT_NM, 
            ORDER_REQ, ORDER_TIME, SETTLE_AMT, CURRENCY, STATUS, STATUS_MSG, BANK_STATUS, 
            BANK_STATUS_MSG, BANK_BATCH_NO, BANK_SEQNO, RESV_PHONE, ACCT_NO, ACCT_NM, 
            ACCT_TYPE, ACCT_BANK_NO, ACCT_BANK_NM, ACCT_ZBANK_NO, ACCT_ZBANK_NM, 
            ACCT_ZBANK_ADDR, ACCT_ZBANK_CODE, ACCT_UNION_NO, ACCT_NET_NO, REMARK, CERT_TYPE, 
            CERT_NO, EXT_PROPERTIES, :stSettle.update_time
        FROM TBL_SETTLE_TXN WHERE ID = :stSettle.id; */ 
#line 291 "settle_txn.pcpp"

{
#line 278 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 278 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 278 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 278 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 278 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 278 "settle_txn.pcpp"
        sqlstm.stmt = "insert into TBL_SETTLE_ERR (ID,SETTLE_DATE,IN_DATE,T\
X_DATE,SETTLE_NUM,CHANNEL_NO,BATCH_NO,TRACE_NO,DO_FLAG,ERR_TYPE,MERT_NO,MERT_\
NM,ORDER_REQ,ORDER_TIME,SETTLE_AMT,CURRENCY,STATUS,STATUS_MSG,BANK_STATUS,BAN\
K_STATUS_MSG,BANK_BATCH_NO,BANK_SEQNO,RESV_PHONE,ACCT_NO,ACCT_NM,ACCT_TYPE,AC\
CT_BANK_NO,ACCT_BANK_NM,ACCT_ZBANK_NO,ACCT_ZBANK_NM,ACCT_ZBANK_ADDR,ACCT_ZBAN\
K_CODE,ACCT_UNION_NO,ACCT_NET_NO,REMARK,CERT_TYPE,CERT_NO,EXT_PROPERTIES,CREA\
TE_TIME)select PAY_ORDER ,SETTLE_DATE ,:b0 ,TX_DATE ,SETTLE_NUM ,CHANNEL_NO ,\
BATCH_NO ,TRACE_NO ,:b1 ,:b2 ,MERT_NO ,MERT_NM ,ORDER_REQ ,ORDER_TIME ,SETTLE\
_AMT ,CURRENCY ,STATUS ,STATUS_MSG ,BANK_STATUS ,BANK_STATUS_MSG ,BANK_BATCH_\
NO ,BANK_SEQNO ,RESV_PHONE ,ACCT_NO ,ACCT_NM ,ACCT_TYPE ,ACCT_BANK_NO ,ACCT_B\
ANK_NM ,ACCT_ZBANK_NO ,ACCT_ZBANK_NM ,ACCT_ZBANK_ADDR ,ACCT_ZBANK_CODE ,ACCT_\
UNION_NO ,ACCT_NET_NO ,REMARK ,CERT_TYPE ,CERT_NO ,EXT_PROPERTIES ,:b3  from \
TBL_SETTLE_TXN where ID=:b4";
#line 278 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 278 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )532;
#line 278 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 278 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 278 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.in_date);
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )9;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstv[1] = (unsigned char  *)(stError.do_flag);
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstl[1] = (unsigned long )2;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhsts[1] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqindv[1] = (         short *)0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqinds[1] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqharm[1] = (unsigned long )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqadto[1] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqtdso[1] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstv[2] = (unsigned char  *)(stError.err_type);
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstl[2] = (unsigned long )2;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhsts[2] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqindv[2] = (         short *)0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqinds[2] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqharm[2] = (unsigned long )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqadto[2] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqtdso[2] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.update_time);
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstl[3] = (unsigned long )15;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhsts[3] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqindv[3] = (         short *)0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqinds[3] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqharm[3] = (unsigned long )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqadto[3] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqtdso[3] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.id);
#line 278 "settle_txn.pcpp"
        sqlstm.sqhstl[4] = (unsigned long )33;
#line 278 "settle_txn.pcpp"
        sqlstm.sqhsts[4] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqindv[4] = (         short *)0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqinds[4] = (         int  )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqharm[4] = (unsigned long )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqadto[4] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqtdso[4] = (unsigned short )0;
#line 278 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 278 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 278 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 278 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 278 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 278 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 278 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 278 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 278 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 278 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 278 "settle_txn.pcpp"
}

#line 291 "settle_txn.pcpp"

    }
    else
    {
        /* EXEC SQL UPDATE TBL_SETTLE_ERR SET ID = ID WHERE ID = :stSettle.id; */ 
#line 295 "settle_txn.pcpp"

{
#line 295 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 295 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 295 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 295 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 295 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 295 "settle_txn.pcpp"
        sqlstm.stmt = "update TBL_SETTLE_ERR  set ID=ID where ID=:b0";
#line 295 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 295 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )567;
#line 295 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 295 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 295 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 295 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 295 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.id);
#line 295 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )33;
#line 295 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 295 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 295 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 295 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 295 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 295 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 295 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 295 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 295 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 295 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 295 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 295 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 295 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 295 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 295 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 295 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 295 "settle_txn.pcpp"
}

#line 295 "settle_txn.pcpp"

        if(SQL_UD_ERR)
        {
            SYSError(RET_DATABASE, "锁定入账差错记录(%s)失败, err:(%d)(%s)", stSettle.id, 
                SQLCODE, sDbsError());
            return RC_FAIL;
        }
    
        if(SETTLE_MCHT_SUCC == stFund.result_flag[0])
            stError.do_flag[0] = SETTLE_ERR_SETTLE_SUCC;

        /* EXEC SQL UPDATE TBL_SETTLE_ERR SET 
            DO_FLAG         = :stError.do_flag,
            ERR_TYPE        = :stError.err_type,
            IN_DATE         = :stSettle.in_date,    
            STATUS          = :stSettle.status,
            STATUS_MSG      = :stSettle.status_msg,
            BANK_STATUS     = :stSettle.bank_status,
            BANK_STATUS_MSG = :stSettle.bank_status_msg,
            BANK_BATCH_NO   = :stSettle.bank_batch_no,
            BANK_SEQNO      = :stSettle.bank_seqno,
            ORDER_REQ       = :stSettle.order_req,
            ORDER_TIME      = :stSettle.order_time,
            CURRENCY        = :stSettle.currency,
            RESV_PHONE      = :stSettle.resv_phone,
            ACCT_NO         = :stSettle.acct_no,
            ACCT_NM         = :stSettle.acct_nm,
            ACCT_TYPE       = :stSettle.acct_type,
            ACCT_BANK_NO    = :stSettle.acct_bank_no,
            ACCT_BANK_NM    = :stSettle.acct_bank_nm,
            ACCT_ZBANK_NO   = :stSettle.acct_zbank_no,
            ACCT_ZBANK_NM   = :stSettle.acct_zbank_nm,
            ACCT_ZBANK_ADDR = :stSettle.acct_zbank_addr,
            ACCT_ZBANK_CODE = :stSettle.acct_zbank_code,
            ACCT_UNION_NO   = :stSettle.acct_union_no,
            ACCT_NET_NO     = :stSettle.acct_net_no,
            CERT_TYPE       = :stSettle.cert_type,
            CERT_NO         = :stSettle.cert_no,
            UPDATE_TIME     = :stSettle.update_time
        WHERE ID = :stSettle.pay_order; */ 
#line 334 "settle_txn.pcpp"

{
#line 306 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 306 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 306 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 306 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 306 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 306 "settle_txn.pcpp"
        sqlstm.stmt = "update TBL_SETTLE_ERR  set DO_FLAG=:b0,ERR_TYPE=:b1,\
IN_DATE=:b2,STATUS=:b3,STATUS_MSG=:b4,BANK_STATUS=:b5,BANK_STATUS_MSG=:b6,BAN\
K_BATCH_NO=:b7,BANK_SEQNO=:b8,ORDER_REQ=:b9,ORDER_TIME=:b10,CURRENCY=:b11,RES\
V_PHONE=:b12,ACCT_NO=:b13,ACCT_NM=:b14,ACCT_TYPE=:b15,ACCT_BANK_NO=:b16,ACCT_\
BANK_NM=:b17,ACCT_ZBANK_NO=:b18,ACCT_ZBANK_NM=:b19,ACCT_ZBANK_ADDR=:b20,ACCT_\
ZBANK_CODE=:b21,ACCT_UNION_NO=:b22,ACCT_NET_NO=:b23,CERT_TYPE=:b24,CERT_NO=:b\
25,UPDATE_TIME=:b26 where ID=:b27";
#line 306 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 306 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )586;
#line 306 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 306 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 306 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)(stError.do_flag);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )2;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[1] = (unsigned char  *)(stError.err_type);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[1] = (unsigned long )2;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[1] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[1] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[1] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[1] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[1] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[1] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[2] = (unsigned char  *)(stSettle.in_date);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[2] = (unsigned long )9;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[2] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[2] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[2] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[2] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[2] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[2] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.status);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[3] = (unsigned long )31;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[3] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[3] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[3] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[3] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[3] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[3] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.status_msg);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[4] = (unsigned long )257;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[4] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[4] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[4] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[4] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[4] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[4] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[5] = (unsigned char  *)(stSettle.bank_status);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[5] = (unsigned long )31;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[5] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[5] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[5] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[5] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[5] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[5] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[6] = (unsigned char  *)(stSettle.bank_status_msg);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[6] = (unsigned long )257;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[6] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[6] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[6] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[6] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[6] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[6] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[7] = (unsigned char  *)(stSettle.bank_batch_no);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[7] = (unsigned long )33;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[7] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[7] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[7] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[7] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[7] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[7] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[8] = (unsigned char  *)(stSettle.bank_seqno);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[8] = (unsigned long )33;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[8] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[8] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[8] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[8] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[8] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[8] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[9] = (unsigned char  *)(stSettle.order_req);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[9] = (unsigned long )33;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[9] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[9] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[9] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[9] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[9] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[9] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[10] = (unsigned char  *)(stSettle.order_time);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[10] = (unsigned long )15;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[10] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[10] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[10] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[10] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[10] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[10] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[11] = (unsigned char  *)(stSettle.currency);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[11] = (unsigned long )6;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[11] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[11] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[11] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[11] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[11] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[11] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[12] = (unsigned char  *)(stSettle.resv_phone);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[12] = (unsigned long )12;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[12] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[12] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[12] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[12] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[12] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[12] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[13] = (unsigned char  *)(stSettle.acct_no);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[13] = (unsigned long )101;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[13] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[13] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[13] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[13] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[13] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[13] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[14] = (unsigned char  *)(stSettle.acct_nm);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[14] = (unsigned long )101;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[14] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[14] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[14] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[14] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[14] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[14] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[15] = (unsigned char  *)(stSettle.acct_type);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[15] = (unsigned long )4;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[15] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[15] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[15] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[15] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[15] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[15] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[16] = (unsigned char  *)(stSettle.acct_bank_no);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[16] = (unsigned long )21;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[16] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[16] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[16] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[16] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[16] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[16] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[17] = (unsigned char  *)(stSettle.acct_bank_nm);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[17] = (unsigned long )101;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[17] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[17] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[17] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[17] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[17] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[17] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[18] = (unsigned char  *)(stSettle.acct_zbank_no);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[18] = (unsigned long )21;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[18] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[18] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[18] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[18] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[18] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[18] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[19] = (unsigned char  *)(stSettle.acct_zbank_nm);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[19] = (unsigned long )101;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[19] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[19] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[19] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[19] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[19] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[19] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[20] = (unsigned char  *)(stSettle.acct_zbank_addr);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[20] = (unsigned long )161;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[20] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[20] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[20] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[20] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[20] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[20] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[21] = (unsigned char  *)(stSettle.acct_zbank_code);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[21] = (unsigned long )11;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[21] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[21] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[21] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[21] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[21] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[21] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[22] = (unsigned char  *)(stSettle.acct_union_no);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[22] = (unsigned long )21;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[22] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[22] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[22] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[22] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[22] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[22] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[23] = (unsigned char  *)(stSettle.acct_net_no);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[23] = (unsigned long )21;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[23] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[23] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[23] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[23] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[23] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[23] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[24] = (unsigned char  *)(stSettle.cert_type);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[24] = (unsigned long )3;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[24] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[24] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[24] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[24] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[24] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[24] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[25] = (unsigned char  *)(stSettle.cert_no);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[25] = (unsigned long )33;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[25] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[25] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[25] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[25] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[25] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[25] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[26] = (unsigned char  *)(stSettle.update_time);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[26] = (unsigned long )15;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[26] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[26] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[26] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[26] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[26] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[26] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstv[27] = (unsigned char  *)(stSettle.pay_order);
#line 306 "settle_txn.pcpp"
        sqlstm.sqhstl[27] = (unsigned long )33;
#line 306 "settle_txn.pcpp"
        sqlstm.sqhsts[27] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqindv[27] = (         short *)0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqinds[27] = (         int  )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqharm[27] = (unsigned long )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqadto[27] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqtdso[27] = (unsigned short )0;
#line 306 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 306 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 306 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 306 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 306 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 306 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 306 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 306 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 306 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 306 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 306 "settle_txn.pcpp"
}

#line 334 "settle_txn.pcpp"

    }
    if(SQLCODE)
    {
        SYSError(RET_DATABASE, "操作入账失败表(%s)失败, err:(%d)(%s)", stSettle.id, 
            SQLCODE, sDbsError());
        return RC_FAIL;
    }

    return RC_SUCC;
}

/**************************************************************************************************
    函 数 名：lCallSettlePay(TSettleTxn *pstSettle)
    功能描述：同步发起代付指令
    返回说明：
        RC_SUCC
        RC_FAIL
 **************************************************************************************************/
long   CSettle::lCallSettlePay(TSettleTxn *pstSettle)
{
    GCts     *pstCts = (GCts *)pGetGloble();
    SATvm    *pstSavm = (SATvm *)pGetSATvm();

    vResetUser();
    IBPputlong("loop", 1);
    IBPputstring("id", pstSettle->id);
    IBPputstring("acct_no", pstSettle->acct_no);
    IBPputstring("acct_nm", pstSettle->acct_nm);
    IBPputstring("cert_no", pstSettle->cert_no);
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

    if(RC_FAIL == lNewTrade(pstSavm, pstCts, "200000", true))
        return lUpdateError(pstSettle);

    if(IBPIsok())
        return lUpdateSettle(pstSettle);
    else
        return lUpdateError(pstSettle);
}

/**************************************************************************************************
    函 数 名：vGenSettleTxn()
    功能描述：统计本周期商户资金
    返回说明：
        无
 **************************************************************************************************/
void    CSettle::vGenSettleTxn()
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 398 "settle_txn.pcpp"

    TMchtFund    stFund;
    TSettleTxn   stSettle;
    char         szFlag[2], szOrder[33], szTime[15];
    /* EXEC SQL END DECLARE SECTION; */ 
#line 402 "settle_txn.pcpp"

    double       dAmt = 0.00;
    long         lCount = 0, lSeqNo = lGetSeqNo();

    memset(szFlag, 0, sizeof(szFlag));
    memset(szTime, 0, sizeof(szTime));
    memset(szOrder, 0, sizeof(szOrder));
    memset(&stFund, 0, sizeof(TMchtFund));
    IBPgetstring("BAT_NUM", stFund.settle_num, "批次编号");
    IBPgetstring("BAT_DATE", stFund.settle_date, "清算日期");
    IBPgetstring("BAT_MCHTNO", stFund.mert_no, "代付商户号");
    IBPgetstring("BAT_PAYTIME", szTime, "代付清算时间");
    IBPgetstring("BAT_PAYORDER", szOrder, "代付请求订单号");
    if(IBPIserr())
        return ;

    szFlag[0] = SETTLE_MCHT_NOTDEAL;
    stFund.result_flag[0] = SETTLE_MCHT_RESETT;
    IBPrace("开始处理日期(%s)商户代付清算流水批次(%s)", stFund.settle_date, stFund.settle_num);
    /* EXEC SQL DECLARE Cur_mcht_fund CURSOR FOR 
        SELECT * FROM TBL_MCHT_FUND 
    WHERE SETTLE_DATE = :stFund.settle_date AND RESULT_FLAG IN (:szFlag, :stFund.result_flag) 
        AND SETTLE_NUM = :stFund.settle_num AND MERT_NO = :stFund.mert_no; */ 
#line 424 "settle_txn.pcpp"


    /* EXEC SQL OPEN Cur_mcht_fund; */ 
#line 426 "settle_txn.pcpp"

{
#line 426 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 426 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 426 "settle_txn.pcpp"
    sqlstm.arrsiz = 47;
#line 426 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 426 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 426 "settle_txn.pcpp"
    sqlstm.stmt = sq0009;
#line 426 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 426 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )713;
#line 426 "settle_txn.pcpp"
    sqlstm.selerr = (unsigned short)1;
#line 426 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 426 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 426 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqcmod = (unsigned int )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)(stFund.settle_date);
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )9;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)szFlag;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )2;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)(stFund.result_flag);
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )2;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)(stFund.settle_num);
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )5;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)(stFund.mert_no);
#line 426 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )21;
#line 426 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 426 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 426 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 426 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 426 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 426 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 426 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 426 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 426 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 426 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 426 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 426 "settle_txn.pcpp"
}

#line 426 "settle_txn.pcpp"

    if(SQLCODE)
    {
        SYSError(RET_DATABASE, "打开游标失败, err:(%d)(%s)", SQLCODE, sDbsError());
        return ;
    }

    while (1)
    {
        memset(&stFund, 0, sizeof(TMchtFund));
        /* EXEC SQL FETCH Cur_mcht_fund INTO :stFund; */ 
#line 436 "settle_txn.pcpp"

{
#line 436 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 436 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 436 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 436 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 436 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 436 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 436 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )748;
#line 436 "settle_txn.pcpp"
        sqlstm.selerr = (unsigned short)1;
#line 436 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 436 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 436 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqfoff = (         int )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqfmod = (unsigned int )2;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)stFund.id;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )33;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[1] = (unsigned char  *)stFund.settle_date;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[1] = (unsigned long )9;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[1] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[1] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[1] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[1] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[1] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[1] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[2] = (unsigned char  *)stFund.settle_num;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[2] = (unsigned long )5;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[2] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[2] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[2] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[2] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[2] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[2] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[3] = (unsigned char  *)stFund.channel_no;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[3] = (unsigned long )5;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[3] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[3] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[3] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[3] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[3] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[3] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[4] = (unsigned char  *)stFund.in_date;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[4] = (unsigned long )9;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[4] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[4] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[4] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[4] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[4] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[4] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[5] = (unsigned char  *)stFund.tx_date;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[5] = (unsigned long )9;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[5] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[5] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[5] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[5] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[5] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[5] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[6] = (unsigned char  *)stFund.batch_no;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[6] = (unsigned long )21;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[6] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[6] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[6] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[6] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[6] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[6] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[7] = (unsigned char  *)stFund.trace_no;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[7] = (unsigned long )21;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[7] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[7] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[7] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[7] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[7] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[7] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[8] = (unsigned char  *)stFund.result_flag;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[8] = (unsigned long )2;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[8] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[8] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[8] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[8] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[8] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[8] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[9] = (unsigned char  *)stFund.result_desc;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[9] = (unsigned long )61;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[9] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[9] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[9] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[9] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[9] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[9] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[10] = (unsigned char  *)&stFund.tx_num;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[10] = (unsigned long )sizeof(long);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[10] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[10] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[10] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[10] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[10] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[10] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[11] = (unsigned char  *)&stFund.tx_amt;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[11] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[11] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[11] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[11] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[11] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[11] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[11] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[12] = (unsigned char  *)&stFund.credit_num;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[12] = (unsigned long )sizeof(long);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[12] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[12] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[12] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[12] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[12] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[12] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[13] = (unsigned char  *)&stFund.credit_amt;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[13] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[13] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[13] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[13] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[13] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[13] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[13] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[14] = (unsigned char  *)&stFund.credit_percent;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[14] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[14] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[14] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[14] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[14] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[14] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[14] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[15] = (unsigned char  *)&stFund.fee_amt;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[15] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[15] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[15] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[15] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[15] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[15] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[15] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[16] = (unsigned char  *)&stFund.other_fee;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[16] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[16] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[16] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[16] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[16] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[16] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[16] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[17] = (unsigned char  *)&stFund.repair_amt;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[17] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[17] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[17] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[17] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[17] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[17] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[17] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[18] = (unsigned char  *)&stFund.unrepair_amt;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[18] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[18] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[18] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[18] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[18] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[18] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[18] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[19] = (unsigned char  *)&stFund.uperr_amt;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[19] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[19] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[19] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[19] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[19] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[19] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[19] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[20] = (unsigned char  *)stFund.dc_flag;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[20] = (unsigned long )2;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[20] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[20] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[20] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[20] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[20] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[20] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[21] = (unsigned char  *)&stFund.settle_amt;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[21] = (unsigned long )sizeof(double);
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[21] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[21] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[21] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[21] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[21] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[21] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[22] = (unsigned char  *)stFund.mert_no;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[22] = (unsigned long )21;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[22] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[22] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[22] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[22] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[22] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[22] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[23] = (unsigned char  *)stFund.mert_nm;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[23] = (unsigned long )201;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[23] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[23] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[23] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[23] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[23] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[23] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[24] = (unsigned char  *)stFund.order_req;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[24] = (unsigned long )33;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[24] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[24] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[24] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[24] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[24] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[24] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[25] = (unsigned char  *)stFund.order_time;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[25] = (unsigned long )15;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[25] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[25] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[25] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[25] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[25] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[25] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[26] = (unsigned char  *)stFund.res;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[26] = (unsigned long )129;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[26] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[26] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[26] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[26] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[26] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[26] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[27] = (unsigned char  *)stFund.pay_time;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[27] = (unsigned long )15;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[27] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[27] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[27] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[27] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[27] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[27] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[28] = (unsigned char  *)stFund.create_time;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[28] = (unsigned long )15;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[28] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[28] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[28] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[28] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[28] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[28] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstv[29] = (unsigned char  *)stFund.update_time;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhstl[29] = (unsigned long )15;
#line 436 "settle_txn.pcpp"
        sqlstm.sqhsts[29] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqindv[29] = (         short *)0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqinds[29] = (         int  )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqharm[29] = (unsigned long )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqadto[29] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqtdso[29] = (unsigned short )0;
#line 436 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 436 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 436 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 436 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 436 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 436 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 436 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 436 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 436 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 436 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 436 "settle_txn.pcpp"
}

#line 436 "settle_txn.pcpp"

        if(SQLERR)
        {
            SYSError(RET_DATABASE, "汇总商户资金失败, err:(%d)(%s)", SQLCODE, sDbsError());
            break;
        }
        if(SQLNOTFOUND)
            break;

        ++ lCount;
        dAmt += stFund.settle_amt;
        memset(&stSettle, 0, sizeof(TSettleTxn));
        strncpy(stSettle.order_req, szOrder, sizeof(stSettle.order_req) - 1);
        strncpy(stSettle.order_time, szTime, sizeof(stSettle.order_time) - 1);
        strncpy(stSettle.settle_date, stFund.settle_date, sizeof(stSettle.settle_date));
        if(RC_SUCC != lInsertSettle(&stSettle, &stFund, ++ lSeqNo))
            continue;

        /* EXEC SQL INSERT INTO TBL_SETTLE_TXN VALUES(:stSettle); */ 
#line 454 "settle_txn.pcpp"

{
#line 454 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 454 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 454 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 454 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 454 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 454 "settle_txn.pcpp"
        sqlstm.stmt = "insert into TBL_SETTLE_TXN  values (:s1 ,:s2 ,:s3 ,:\
s4 ,:s5 ,:s6 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,\
:s18 ,:s19 ,:s20 ,:s21 ,:s22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 \
,:s31 ,:s32 ,:s33 ,:s34 ,:s35 ,:s36 ,:s37 ,:s38 ,:s39 ,:s40 ,:s41 ,:s42 ,:s43\
 ,:s44 )";
#line 454 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 454 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )883;
#line 454 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 454 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 454 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)stSettle.id;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[1] = (unsigned char  *)stSettle.pay_order;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[1] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[1] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[1] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[1] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[1] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[1] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[1] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[2] = (unsigned char  *)stSettle.settle_date;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[2] = (unsigned long )9;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[2] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[2] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[2] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[2] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[2] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[2] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[3] = (unsigned char  *)stSettle.in_date;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[3] = (unsigned long )9;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[3] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[3] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[3] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[3] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[3] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[3] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[4] = (unsigned char  *)stSettle.tx_date;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[4] = (unsigned long )9;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[4] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[4] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[4] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[4] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[4] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[4] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[5] = (unsigned char  *)stSettle.settle_num;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[5] = (unsigned long )5;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[5] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[5] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[5] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[5] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[5] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[5] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[6] = (unsigned char  *)stSettle.channel_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[6] = (unsigned long )5;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[6] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[6] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[6] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[6] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[6] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[6] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[7] = (unsigned char  *)stSettle.batch_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[7] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[7] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[7] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[7] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[7] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[7] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[7] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[8] = (unsigned char  *)stSettle.trace_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[8] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[8] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[8] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[8] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[8] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[8] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[8] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[9] = (unsigned char  *)stSettle.app_type;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[9] = (unsigned long )2;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[9] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[9] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[9] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[9] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[9] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[9] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[10] = (unsigned char  *)stSettle.app_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[10] = (unsigned long )21;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[10] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[10] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[10] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[10] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[10] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[10] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[11] = (unsigned char  *)stSettle.app_nm;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[11] = (unsigned long )201;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[11] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[11] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[11] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[11] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[11] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[11] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[12] = (unsigned char  *)stSettle.order_req;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[12] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[12] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[12] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[12] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[12] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[12] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[12] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[13] = (unsigned char  *)stSettle.order_time;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[13] = (unsigned long )15;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[13] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[13] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[13] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[13] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[13] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[13] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[14] = (unsigned char  *)stSettle.pay_acct;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[14] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[14] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[14] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[14] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[14] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[14] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[14] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[15] = (unsigned char  *)&stSettle.settle_amt;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[15] = (unsigned long )sizeof(double);
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[15] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[15] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[15] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[15] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[15] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[15] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[16] = (unsigned char  *)stSettle.currency;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[16] = (unsigned long )6;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[16] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[16] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[16] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[16] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[16] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[16] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[17] = (unsigned char  *)stSettle.result_flag;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[17] = (unsigned long )2;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[17] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[17] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[17] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[17] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[17] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[17] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[18] = (unsigned char  *)stSettle.status;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[18] = (unsigned long )31;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[18] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[18] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[18] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[18] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[18] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[18] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[19] = (unsigned char  *)stSettle.status_msg;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[19] = (unsigned long )257;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[19] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[19] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[19] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[19] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[19] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[19] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[20] = (unsigned char  *)stSettle.bank_status;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[20] = (unsigned long )31;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[20] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[20] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[20] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[20] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[20] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[20] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[21] = (unsigned char  *)stSettle.bank_status_msg;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[21] = (unsigned long )257;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[21] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[21] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[21] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[21] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[21] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[21] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[22] = (unsigned char  *)stSettle.bank_batch_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[22] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[22] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[22] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[22] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[22] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[22] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[22] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[23] = (unsigned char  *)stSettle.bank_seqno;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[23] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[23] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[23] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[23] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[23] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[23] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[23] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[24] = (unsigned char  *)stSettle.resv_phone;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[24] = (unsigned long )12;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[24] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[24] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[24] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[24] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[24] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[24] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[25] = (unsigned char  *)stSettle.acct_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[25] = (unsigned long )101;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[25] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[25] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[25] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[25] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[25] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[25] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[26] = (unsigned char  *)stSettle.acct_nm;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[26] = (unsigned long )101;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[26] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[26] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[26] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[26] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[26] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[26] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[27] = (unsigned char  *)stSettle.acct_type;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[27] = (unsigned long )4;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[27] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[27] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[27] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[27] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[27] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[27] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[28] = (unsigned char  *)stSettle.acct_bank_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[28] = (unsigned long )21;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[28] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[28] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[28] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[28] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[28] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[28] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[29] = (unsigned char  *)stSettle.acct_bank_nm;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[29] = (unsigned long )101;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[29] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[29] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[29] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[29] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[29] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[29] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[30] = (unsigned char  *)stSettle.acct_zbank_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[30] = (unsigned long )21;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[30] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[30] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[30] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[30] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[30] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[30] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[31] = (unsigned char  *)stSettle.acct_zbank_nm;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[31] = (unsigned long )101;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[31] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[31] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[31] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[31] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[31] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[31] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[32] = (unsigned char  *)stSettle.acct_zbank_addr;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[32] = (unsigned long )161;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[32] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[32] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[32] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[32] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[32] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[32] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[33] = (unsigned char  *)stSettle.acct_zbank_code;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[33] = (unsigned long )11;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[33] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[33] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[33] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[33] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[33] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[33] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[34] = (unsigned char  *)stSettle.acct_union_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[34] = (unsigned long )21;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[34] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[34] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[34] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[34] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[34] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[34] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[35] = (unsigned char  *)stSettle.acct_net_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[35] = (unsigned long )21;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[35] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[35] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[35] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[35] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[35] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[35] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[36] = (unsigned char  *)stSettle.remark;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[36] = (unsigned long )101;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[36] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[36] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[36] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[36] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[36] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[36] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[37] = (unsigned char  *)stSettle.cert_type;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[37] = (unsigned long )3;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[37] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[37] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[37] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[37] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[37] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[37] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[38] = (unsigned char  *)stSettle.cert_no;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[38] = (unsigned long )33;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[38] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[38] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[38] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[38] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[38] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[38] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[39] = (unsigned char  *)&stSettle.card_type;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[39] = (unsigned long )sizeof(long);
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[39] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[39] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[39] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[39] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[39] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[39] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[40] = (unsigned char  *)stSettle.product_code;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[40] = (unsigned long )21;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[40] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[40] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[40] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[40] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[40] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[40] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[41] = (unsigned char  *)stSettle.ext_properties;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[41] = (unsigned long )1001;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[41] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[41] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[41] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[41] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[41] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[41] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[42] = (unsigned char  *)stSettle.create_time;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[42] = (unsigned long )15;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[42] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[42] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[42] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[42] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[42] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[42] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstv[43] = (unsigned char  *)stSettle.update_time;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhstl[43] = (unsigned long )15;
#line 454 "settle_txn.pcpp"
        sqlstm.sqhsts[43] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqindv[43] = (         short *)0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqinds[43] = (         int  )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqharm[43] = (unsigned long )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqadto[43] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqtdso[43] = (unsigned short )0;
#line 454 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 454 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 454 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 454 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 454 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 454 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 454 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 454 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 454 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 454 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 454 "settle_txn.pcpp"
}

#line 454 "settle_txn.pcpp"

        if(SQLCODE)
        {
            SYSError(RET_DATABASE, "新增(%s)商户(%s)代发数据失败, err:(%d)(%s)", stSettle.id, 
                stSettle.mert_no, SQLCODE, sDbsError());
            /* EXEC SQL CLOSE Cur_mcht_fund; */ 
#line 459 "settle_txn.pcpp"

{
#line 459 "settle_txn.pcpp"
            struct sqlexd sqlstm;
#line 459 "settle_txn.pcpp"
            sqlstm.sqlvsn = 12;
#line 459 "settle_txn.pcpp"
            sqlstm.arrsiz = 47;
#line 459 "settle_txn.pcpp"
            sqlstm.sqladtp = &sqladt;
#line 459 "settle_txn.pcpp"
            sqlstm.sqltdsp = &sqltds;
#line 459 "settle_txn.pcpp"
            sqlstm.iters = (unsigned int  )1;
#line 459 "settle_txn.pcpp"
            sqlstm.offset = (unsigned int  )1074;
#line 459 "settle_txn.pcpp"
            sqlstm.cud = sqlcud0;
#line 459 "settle_txn.pcpp"
            sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 459 "settle_txn.pcpp"
            sqlstm.sqlety = (unsigned short)4352;
#line 459 "settle_txn.pcpp"
            sqlstm.occurs = (unsigned int  )0;
#line 459 "settle_txn.pcpp"
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 459 "settle_txn.pcpp"
}

#line 459 "settle_txn.pcpp"

            return ;
        }

        stFund.result_flag[0] = SETTLE_MCHT_PUTREQ;
        /* EXEC SQL UPDATE TBL_MCHT_FUND SET
            RESULT_FLAG = :stSettle.result_flag,    
            ORDER_REQ   = :stSettle.order_req,
            ORDER_TIME  = :stSettle.order_time,
            UPDATE_TIME = :stSettle.create_time
        WHERE ID = :stFund.id; */ 
#line 469 "settle_txn.pcpp"

{
#line 464 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 464 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 464 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 464 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 464 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 464 "settle_txn.pcpp"
        sqlstm.stmt = "update TBL_MCHT_FUND  set RESULT_FLAG=:b0,ORDER_REQ=\
:b1,ORDER_TIME=:b2,UPDATE_TIME=:b3 where ID=:b4";
#line 464 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 464 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )1089;
#line 464 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 464 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 464 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.result_flag);
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )2;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstv[1] = (unsigned char  *)(stSettle.order_req);
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstl[1] = (unsigned long )33;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhsts[1] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqindv[1] = (         short *)0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqinds[1] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqharm[1] = (unsigned long )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqadto[1] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqtdso[1] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstv[2] = (unsigned char  *)(stSettle.order_time);
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstl[2] = (unsigned long )15;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhsts[2] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqindv[2] = (         short *)0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqinds[2] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqharm[2] = (unsigned long )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqadto[2] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqtdso[2] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.create_time);
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstl[3] = (unsigned long )15;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhsts[3] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqindv[3] = (         short *)0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqinds[3] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqharm[3] = (unsigned long )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqadto[3] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqtdso[3] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstv[4] = (unsigned char  *)(stFund.id);
#line 464 "settle_txn.pcpp"
        sqlstm.sqhstl[4] = (unsigned long )33;
#line 464 "settle_txn.pcpp"
        sqlstm.sqhsts[4] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqindv[4] = (         short *)0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqinds[4] = (         int  )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqharm[4] = (unsigned long )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqadto[4] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqtdso[4] = (unsigned short )0;
#line 464 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 464 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 464 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 464 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 464 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 464 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 464 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 464 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 464 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 464 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 464 "settle_txn.pcpp"
}

#line 469 "settle_txn.pcpp"

        if(SQLCODE)
        {
            SYSError(RET_DATABASE, "修改(%s)商户代发数据失败, err:(%d)(%s)", stFund.id, 
                SQLCODE, sDbsError());
            /* EXEC SQL CLOSE Cur_mcht_fund; */ 
#line 474 "settle_txn.pcpp"

{
#line 474 "settle_txn.pcpp"
            struct sqlexd sqlstm;
#line 474 "settle_txn.pcpp"
            sqlstm.sqlvsn = 12;
#line 474 "settle_txn.pcpp"
            sqlstm.arrsiz = 47;
#line 474 "settle_txn.pcpp"
            sqlstm.sqladtp = &sqladt;
#line 474 "settle_txn.pcpp"
            sqlstm.sqltdsp = &sqltds;
#line 474 "settle_txn.pcpp"
            sqlstm.iters = (unsigned int  )1;
#line 474 "settle_txn.pcpp"
            sqlstm.offset = (unsigned int  )1124;
#line 474 "settle_txn.pcpp"
            sqlstm.cud = sqlcud0;
#line 474 "settle_txn.pcpp"
            sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 474 "settle_txn.pcpp"
            sqlstm.sqlety = (unsigned short)4352;
#line 474 "settle_txn.pcpp"
            sqlstm.occurs = (unsigned int  )0;
#line 474 "settle_txn.pcpp"
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 474 "settle_txn.pcpp"
}

#line 474 "settle_txn.pcpp"

            return ;
        }

        if(RC_SUCC != lCallSettlePay(&stSettle))
             break;

        /* EXEC SQL COMMIT WORK; */ 
#line 481 "settle_txn.pcpp"

{
#line 481 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 481 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 481 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 481 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 481 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 481 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 481 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )1139;
#line 481 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 481 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 481 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 481 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 481 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 481 "settle_txn.pcpp"
}

#line 481 "settle_txn.pcpp"

    }
    /* EXEC SQL COMMIT WORK; */ 
#line 483 "settle_txn.pcpp"

{
#line 483 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 483 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 483 "settle_txn.pcpp"
    sqlstm.arrsiz = 47;
#line 483 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 483 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 483 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 483 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )1154;
#line 483 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 483 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 483 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 483 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 483 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 483 "settle_txn.pcpp"
}

#line 483 "settle_txn.pcpp"

    /* EXEC SQL CLOSE Cur_mcht_fund; */ 
#line 484 "settle_txn.pcpp"

{
#line 484 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 484 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 484 "settle_txn.pcpp"
    sqlstm.arrsiz = 47;
#line 484 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 484 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 484 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 484 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )1169;
#line 484 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 484 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 484 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 484 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 484 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 484 "settle_txn.pcpp"
}

#line 484 "settle_txn.pcpp"


    vResetUser();
    IBPrace("处理日期(%s)商户代付清算笔数(%d), 金额(%.2f)", stFund.settle_date, lCount, dAmt);

    return ;
}

/**************************************************************************************************
    函 数 名：vCountErrFound()
    功能描述：统计本周期商户资金
    返回说明：
        无
 **************************************************************************************************/
void    CSettle::vUpdSettleTxn()
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 500 "settle_txn.pcpp"

    TMchtFund    stFund;
    TSettleErr   stError;
    TSettleTxn   stSettle;
    char         szCode[20];
    /* EXEC SQL END DECLARE SECTION; */ 
#line 505 "settle_txn.pcpp"

    long         lCount = 0, lSeqNo = 0;
    GCts         *pstCts = (GCts *)pGetGloble();

    memset(szCode, 0, sizeof(szCode));
    memset(&stFund, 0, sizeof(TMchtFund));
    memset(&stError, 0, sizeof(TSettleErr));
    memset(&stSettle, 0, sizeof(TSettleTxn));
    IBPgetdefstring("pay_BankSeqNo", stSettle.bank_seqno, "");
    IBPgetdefstring("pay_BankBatNo", stSettle.bank_batch_no, "");
    IBPgetdefstring("pay_BankCode", stSettle.bank_status, "");
    IBPgetdefstring("pay_BankMsg", stSettle.bank_status_msg, "");
    IBPgetdefstring("pay_BankBatchNo", stSettle.bank_batch_no, "");
    IBPgetdefstring("pay_BankSeqNo", stSettle.bank_seqno, "");
    IBPgetstring("pay_OrderSerial", stSettle.id, "idx");
    if(IBPIserr())
        return ;

	sTrimAll(stSettle.id);
    /* EXEC SQL SELECT * INTO :stSettle FROM TBL_SETTLE_TXN WHERE ID = :stSettle.id; */ 
#line 524 "settle_txn.pcpp"

{
#line 524 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 524 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 524 "settle_txn.pcpp"
    sqlstm.arrsiz = 47;
#line 524 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 524 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 524 "settle_txn.pcpp"
    sqlstm.stmt = "select * into :s1 ,:s2 ,:s3 ,:s4 ,:s5 ,:s6 ,:s7 ,:s8 ,:s\
9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18 ,:s19 ,:s20 ,:s21 ,:s\
22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 ,:s31 ,:s32 ,:s33 ,:s34 ,:\
s35 ,:s36 ,:s37 ,:s38 ,:s39 ,:s40 ,:s41 ,:s42 ,:s43 ,:s44   from TBL_SETTLE_T\
XN where ID=:b1";
#line 524 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 524 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )1184;
#line 524 "settle_txn.pcpp"
    sqlstm.selerr = (unsigned short)1;
#line 524 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 524 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 524 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)stSettle.id;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)stSettle.pay_order;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)stSettle.settle_date;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)stSettle.in_date;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )9;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)stSettle.tx_date;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )9;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)stSettle.settle_num;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )5;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)stSettle.channel_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )5;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)stSettle.batch_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)stSettle.trace_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)stSettle.app_type;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )2;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[10] = (unsigned char  *)stSettle.app_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[10] = (unsigned long )21;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[10] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[10] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[10] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[10] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[10] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[10] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[11] = (unsigned char  *)stSettle.app_nm;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[11] = (unsigned long )201;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[11] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[11] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[11] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[11] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[11] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[11] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[12] = (unsigned char  *)stSettle.order_req;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[12] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[12] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[12] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[12] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[12] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[12] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[12] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[13] = (unsigned char  *)stSettle.order_time;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[13] = (unsigned long )15;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[13] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[13] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[13] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[13] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[13] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[13] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[14] = (unsigned char  *)stSettle.pay_acct;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[14] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[14] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[14] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[14] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[14] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[14] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[14] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[15] = (unsigned char  *)&stSettle.settle_amt;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[15] = (unsigned long )sizeof(double);
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[15] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[15] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[15] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[15] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[15] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[15] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[16] = (unsigned char  *)stSettle.currency;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[16] = (unsigned long )6;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[16] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[16] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[16] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[16] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[16] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[16] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[17] = (unsigned char  *)stSettle.result_flag;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[17] = (unsigned long )2;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[17] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[17] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[17] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[17] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[17] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[17] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[18] = (unsigned char  *)stSettle.status;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[18] = (unsigned long )31;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[18] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[18] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[18] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[18] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[18] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[18] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[19] = (unsigned char  *)stSettle.status_msg;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[19] = (unsigned long )257;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[19] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[19] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[19] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[19] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[19] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[19] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[20] = (unsigned char  *)stSettle.bank_status;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[20] = (unsigned long )31;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[20] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[20] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[20] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[20] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[20] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[20] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[21] = (unsigned char  *)stSettle.bank_status_msg;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[21] = (unsigned long )257;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[21] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[21] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[21] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[21] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[21] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[21] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[22] = (unsigned char  *)stSettle.bank_batch_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[22] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[22] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[22] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[22] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[22] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[22] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[22] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[23] = (unsigned char  *)stSettle.bank_seqno;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[23] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[23] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[23] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[23] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[23] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[23] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[23] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[24] = (unsigned char  *)stSettle.resv_phone;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[24] = (unsigned long )12;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[24] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[24] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[24] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[24] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[24] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[24] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[25] = (unsigned char  *)stSettle.acct_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[25] = (unsigned long )101;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[25] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[25] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[25] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[25] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[25] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[25] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[26] = (unsigned char  *)stSettle.acct_nm;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[26] = (unsigned long )101;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[26] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[26] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[26] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[26] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[26] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[26] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[27] = (unsigned char  *)stSettle.acct_type;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[27] = (unsigned long )4;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[27] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[27] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[27] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[27] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[27] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[27] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[28] = (unsigned char  *)stSettle.acct_bank_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[28] = (unsigned long )21;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[28] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[28] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[28] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[28] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[28] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[28] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[29] = (unsigned char  *)stSettle.acct_bank_nm;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[29] = (unsigned long )101;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[29] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[29] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[29] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[29] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[29] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[29] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[30] = (unsigned char  *)stSettle.acct_zbank_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[30] = (unsigned long )21;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[30] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[30] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[30] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[30] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[30] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[30] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[31] = (unsigned char  *)stSettle.acct_zbank_nm;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[31] = (unsigned long )101;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[31] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[31] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[31] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[31] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[31] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[31] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[32] = (unsigned char  *)stSettle.acct_zbank_addr;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[32] = (unsigned long )161;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[32] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[32] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[32] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[32] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[32] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[32] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[33] = (unsigned char  *)stSettle.acct_zbank_code;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[33] = (unsigned long )11;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[33] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[33] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[33] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[33] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[33] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[33] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[34] = (unsigned char  *)stSettle.acct_union_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[34] = (unsigned long )21;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[34] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[34] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[34] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[34] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[34] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[34] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[35] = (unsigned char  *)stSettle.acct_net_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[35] = (unsigned long )21;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[35] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[35] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[35] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[35] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[35] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[35] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[36] = (unsigned char  *)stSettle.remark;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[36] = (unsigned long )101;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[36] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[36] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[36] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[36] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[36] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[36] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[37] = (unsigned char  *)stSettle.cert_type;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[37] = (unsigned long )3;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[37] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[37] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[37] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[37] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[37] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[37] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[38] = (unsigned char  *)stSettle.cert_no;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[38] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[38] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[38] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[38] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[38] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[38] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[38] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[39] = (unsigned char  *)&stSettle.card_type;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[39] = (unsigned long )sizeof(long);
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[39] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[39] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[39] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[39] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[39] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[39] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[40] = (unsigned char  *)stSettle.product_code;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[40] = (unsigned long )21;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[40] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[40] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[40] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[40] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[40] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[40] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[41] = (unsigned char  *)stSettle.ext_properties;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[41] = (unsigned long )1001;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[41] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[41] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[41] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[41] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[41] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[41] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[42] = (unsigned char  *)stSettle.create_time;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[42] = (unsigned long )15;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[42] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[42] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[42] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[42] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[42] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[42] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[43] = (unsigned char  *)stSettle.update_time;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[43] = (unsigned long )15;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[43] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[43] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[43] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[43] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[43] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[43] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstv[44] = (unsigned char  *)(stSettle.id);
#line 524 "settle_txn.pcpp"
    sqlstm.sqhstl[44] = (unsigned long )33;
#line 524 "settle_txn.pcpp"
    sqlstm.sqhsts[44] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqindv[44] = (         short *)0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqinds[44] = (         int  )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqharm[44] = (unsigned long )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqadto[44] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqtdso[44] = (unsigned short )0;
#line 524 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 524 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 524 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 524 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 524 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 524 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 524 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 524 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 524 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 524 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 524 "settle_txn.pcpp"
}

#line 524 "settle_txn.pcpp"

    if(SQLCODE)
    {
        SYSError(RET_DATABASE, "获取清算流水(%s)失败, err:(%d)(%s)", stSettle.id,     
         SQLCODE, sDbsError());
       return ;
    }

    if(SETTLE_IN_FLAG_SUCC == stSettle.result_flag[0])
    {
        IBPwarn("交易(%s)已成功，忽略结算返回(%s)", stSettle.id, stSettle.result_flag);
        return ;
    }

    IBPgetstring("pay_Status", stSettle.result_flag, "业务响应码");
    IBPgetstring("pay_BussCode", stSettle.status, "业务响应码");
    IBPgetstring("pay_BussMsg", stSettle.status_msg, "业务响应信息");
    strncpy(stSettle.in_date, pstCts->m_szTxDate, sizeof(stSettle.in_date));
    strncpy(stSettle.update_time, sGetCurrentTime(), sizeof(stSettle.update_time));
    if(IBPIserr())
        return ;

    if(SETTLE_IN_FLAG_SUCC == stSettle.result_flag[0] && 
		!strcmp(stSettle.settle_num, BATCH_REAL_SETTLE))
        /* EXEC SQL DELETE FROM TBL_SETTLE_MON WHERE ID = :stSettle.id; */ 
#line 548 "settle_txn.pcpp"

{
#line 548 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 548 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 548 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 548 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 548 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 548 "settle_txn.pcpp"
        sqlstm.stmt = "delete  from TBL_SETTLE_MON  where ID=:b0";
#line 548 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 548 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )1379;
#line 548 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 548 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 548 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 548 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 548 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.id);
#line 548 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )33;
#line 548 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 548 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 548 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 548 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 548 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 548 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 548 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 548 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 548 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 548 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 548 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 548 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 548 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 548 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 548 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 548 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 548 "settle_txn.pcpp"
}

#line 548 "settle_txn.pcpp"


    /* EXEC SQL UPDATE TBL_SETTLE_TXN SET
        RESULT_FLAG     = :stSettle.result_flag,
        IN_DATE         = :stSettle.in_date,    
        STATUS          = :stSettle.status,
        STATUS_MSG      = :stSettle.status_msg,
        BANK_STATUS     = :stSettle.bank_status,
        BANK_STATUS_MSG = :stSettle.bank_status_msg,
        BANK_BATCH_NO   = :stSettle.bank_batch_no,
        BANK_SEQNO      = :stSettle.bank_seqno,
        UPDATE_TIME     = :stSettle.update_time
    WHERE ID = :stSettle.id; */ 
#line 560 "settle_txn.pcpp"

{
#line 550 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 550 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 550 "settle_txn.pcpp"
    sqlstm.arrsiz = 47;
#line 550 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 550 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 550 "settle_txn.pcpp"
    sqlstm.stmt = "update TBL_SETTLE_TXN  set RESULT_FLAG=:b0,IN_DATE=:b1,S\
TATUS=:b2,STATUS_MSG=:b3,BANK_STATUS=:b4,BANK_STATUS_MSG=:b5,BANK_BATCH_NO=:b\
6,BANK_SEQNO=:b7,UPDATE_TIME=:b8 where ID=:b9";
#line 550 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 550 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )1398;
#line 550 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 550 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 550 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.result_flag);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )2;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)(stSettle.in_date);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )9;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)(stSettle.status);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )31;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.status_msg);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )257;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.bank_status);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )31;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)(stSettle.bank_status_msg);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )257;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)(stSettle.bank_batch_no);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )33;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)(stSettle.bank_seqno);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )33;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)(stSettle.update_time);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )15;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)(stSettle.id);
#line 550 "settle_txn.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )33;
#line 550 "settle_txn.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 550 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 550 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 550 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 550 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 550 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 550 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 550 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 550 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 550 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 550 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 550 "settle_txn.pcpp"
}

#line 560 "settle_txn.pcpp"

    if (SQLCODE)
    {
        SYSError(RET_DATABASE, "更新清算流水(%s)失败, err:(%d)(%s)", stSettle.id,     
         SQLCODE, sDbsError());
       return ;
    }

    if(SETTLE_IN_FLAG_SUCC == stSettle.result_flag[0])
    {
        stFund.result_flag[0] = SETTLE_MCHT_SUCC;
        stError.err_type[0] = ERROR_ER_SETTLE_SUCC;
    }
    else if(SETTLE_IN_FLAG_FAIL == stSettle.result_flag[0]) 
    {
        stFund.result_flag[0] = SETTLE_MCHT_ERROR;
        stError.err_type[0] = ERROR_ER_SETTLE_FAIL;
    }
    else
    {
        stFund.result_flag[0] = SETTLE_MCHT_PUTREQ;
        stError.err_type[0] = ERROR_ER_SETTLE_UNKOWN;
    }

    /* EXEC SQL UPDATE TBL_MCHT_FUND SET
        RESULT_FLAG = :stSettle.result_flag,    
        RESULT_DESC = :stSettle.status_msg,
        IN_DATE     = :stSettle.in_date,    
        PAY_TIME    = :stSettle.update_time,    
        UPDATE_TIME = :stSettle.update_time
    WHERE ID = :stSettle.pay_order; */ 
#line 590 "settle_txn.pcpp"

{
#line 584 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 584 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 584 "settle_txn.pcpp"
    sqlstm.arrsiz = 47;
#line 584 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 584 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 584 "settle_txn.pcpp"
    sqlstm.stmt = "update TBL_MCHT_FUND  set RESULT_FLAG=:b0,RESULT_DESC=:b\
1,IN_DATE=:b2,PAY_TIME=:b3,UPDATE_TIME=:b3 where ID=:b5";
#line 584 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 584 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )1453;
#line 584 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 584 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 584 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.result_flag);
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )2;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)(stSettle.status_msg);
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )257;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)(stSettle.in_date);
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.update_time);
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )15;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.update_time);
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )15;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)(stSettle.pay_order);
#line 584 "settle_txn.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )33;
#line 584 "settle_txn.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 584 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 584 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 584 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 584 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 584 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 584 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 584 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 584 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 584 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 584 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 584 "settle_txn.pcpp"
}

#line 590 "settle_txn.pcpp"

    if(SQLCODE || EFFECT_NUM != 1)
    {
        SYSError(RET_DATABASE, "更新商户汇总(%s)失败, err:(%d)(%s)", stSettle.mert_no, 
            SQLCODE, sDbsError());
        return ;
    } 

    // 处理差错TBL_SETTLE_ERR表记录
    /* EXEC SQL SELECT * INTO :stError FROM TBL_SETTLE_ERR WHERE ID = :stSettle.pay_order; */ 
#line 599 "settle_txn.pcpp"

{
#line 599 "settle_txn.pcpp"
    struct sqlexd sqlstm;
#line 599 "settle_txn.pcpp"
    sqlstm.sqlvsn = 12;
#line 599 "settle_txn.pcpp"
    sqlstm.arrsiz = 47;
#line 599 "settle_txn.pcpp"
    sqlstm.sqladtp = &sqladt;
#line 599 "settle_txn.pcpp"
    sqlstm.sqltdsp = &sqltds;
#line 599 "settle_txn.pcpp"
    sqlstm.stmt = "select * into :s1 ,:s2 ,:s3 ,:s4 ,:s5 ,:s6 ,:s7 ,:s8 ,:s\
9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18 ,:s19 ,:s20 ,:s21 ,:s\
22 ,:s23 ,:s24 ,:s25 ,:s26 ,:s27 ,:s28 ,:s29 ,:s30 ,:s31 ,:s32 ,:s33 ,:s34 ,:\
s35 ,:s36 ,:s37 ,:s38 ,:s39 ,:s40 ,:s41 ,:s42 ,:s43 ,:s44 ,:s45 ,:s46   from \
TBL_SETTLE_ERR where ID=:b1";
#line 599 "settle_txn.pcpp"
    sqlstm.iters = (unsigned int  )1;
#line 599 "settle_txn.pcpp"
    sqlstm.offset = (unsigned int  )1492;
#line 599 "settle_txn.pcpp"
    sqlstm.selerr = (unsigned short)1;
#line 599 "settle_txn.pcpp"
    sqlstm.cud = sqlcud0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 599 "settle_txn.pcpp"
    sqlstm.sqlety = (unsigned short)4352;
#line 599 "settle_txn.pcpp"
    sqlstm.occurs = (unsigned int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[0] = (unsigned char  *)stError.id;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[0] = (unsigned long )33;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[0] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[0] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[0] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[0] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[1] = (unsigned char  *)stError.settle_date;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[1] = (unsigned long )9;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[1] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[1] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[1] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[1] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[2] = (unsigned char  *)stError.in_date;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[2] = (unsigned long )9;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[2] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[2] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[2] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[2] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[3] = (unsigned char  *)stError.tx_date;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[3] = (unsigned long )9;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[3] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[3] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[3] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[3] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[4] = (unsigned char  *)stError.settle_num;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[4] = (unsigned long )5;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[4] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[4] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[4] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[4] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[5] = (unsigned char  *)stError.channel_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[5] = (unsigned long )5;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[5] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[5] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[5] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[5] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[6] = (unsigned char  *)stError.batch_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[6] = (unsigned long )33;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[6] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[6] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[6] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[6] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[6] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[6] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[7] = (unsigned char  *)stError.trace_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[7] = (unsigned long )33;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[7] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[7] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[7] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[7] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[7] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[7] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[8] = (unsigned char  *)stError.do_flag;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[8] = (unsigned long )2;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[8] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[8] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[8] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[8] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[8] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[8] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[9] = (unsigned char  *)stError.opr_date;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[9] = (unsigned long )9;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[9] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[9] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[9] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[9] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[9] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[9] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[10] = (unsigned char  *)&stError.opr_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[10] = (unsigned long )sizeof(long);
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[10] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[10] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[10] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[10] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[10] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[10] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[11] = (unsigned char  *)stError.next_date;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[11] = (unsigned long )9;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[11] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[11] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[11] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[11] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[11] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[11] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[12] = (unsigned char  *)stError.next_num;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[12] = (unsigned long )5;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[12] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[12] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[12] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[12] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[12] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[12] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[13] = (unsigned char  *)stError.next_cup;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[13] = (unsigned long )5;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[13] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[13] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[13] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[13] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[13] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[13] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[14] = (unsigned char  *)stError.next_do;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[14] = (unsigned long )1025;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[14] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[14] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[14] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[14] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[14] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[14] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[15] = (unsigned char  *)stError.err_type;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[15] = (unsigned long )2;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[15] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[15] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[15] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[15] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[15] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[15] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[16] = (unsigned char  *)stError.mert_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[16] = (unsigned long )21;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[16] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[16] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[16] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[16] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[16] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[16] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[17] = (unsigned char  *)stError.mert_nm;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[17] = (unsigned long )201;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[17] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[17] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[17] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[17] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[17] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[17] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[18] = (unsigned char  *)stError.order_req;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[18] = (unsigned long )33;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[18] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[18] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[18] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[18] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[18] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[18] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[19] = (unsigned char  *)stError.order_time;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[19] = (unsigned long )15;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[19] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[19] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[19] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[19] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[19] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[19] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[20] = (unsigned char  *)&stError.settle_amt;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[20] = (unsigned long )sizeof(double);
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[20] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[20] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[20] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[20] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[20] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[20] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[21] = (unsigned char  *)stError.currency;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[21] = (unsigned long )6;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[21] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[21] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[21] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[21] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[21] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[21] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[22] = (unsigned char  *)stError.status;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[22] = (unsigned long )31;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[22] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[22] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[22] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[22] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[22] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[22] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[23] = (unsigned char  *)stError.status_msg;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[23] = (unsigned long )257;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[23] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[23] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[23] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[23] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[23] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[23] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[24] = (unsigned char  *)stError.bank_status;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[24] = (unsigned long )31;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[24] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[24] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[24] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[24] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[24] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[24] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[25] = (unsigned char  *)stError.bank_status_msg;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[25] = (unsigned long )257;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[25] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[25] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[25] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[25] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[25] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[25] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[26] = (unsigned char  *)stError.bank_batch_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[26] = (unsigned long )33;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[26] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[26] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[26] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[26] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[26] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[26] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[27] = (unsigned char  *)stError.bank_seqno;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[27] = (unsigned long )33;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[27] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[27] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[27] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[27] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[27] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[27] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[28] = (unsigned char  *)stError.resv_phone;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[28] = (unsigned long )12;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[28] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[28] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[28] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[28] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[28] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[28] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[29] = (unsigned char  *)stError.acct_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[29] = (unsigned long )101;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[29] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[29] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[29] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[29] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[29] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[29] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[30] = (unsigned char  *)stError.acct_nm;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[30] = (unsigned long )101;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[30] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[30] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[30] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[30] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[30] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[30] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[31] = (unsigned char  *)stError.acct_type;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[31] = (unsigned long )4;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[31] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[31] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[31] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[31] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[31] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[31] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[32] = (unsigned char  *)stError.acct_bank_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[32] = (unsigned long )21;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[32] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[32] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[32] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[32] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[32] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[32] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[33] = (unsigned char  *)stError.acct_bank_nm;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[33] = (unsigned long )101;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[33] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[33] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[33] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[33] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[33] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[33] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[34] = (unsigned char  *)stError.acct_zbank_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[34] = (unsigned long )21;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[34] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[34] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[34] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[34] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[34] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[34] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[35] = (unsigned char  *)stError.acct_zbank_nm;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[35] = (unsigned long )101;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[35] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[35] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[35] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[35] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[35] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[35] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[36] = (unsigned char  *)stError.acct_zbank_addr;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[36] = (unsigned long )161;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[36] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[36] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[36] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[36] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[36] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[36] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[37] = (unsigned char  *)stError.acct_zbank_code;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[37] = (unsigned long )11;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[37] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[37] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[37] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[37] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[37] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[37] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[38] = (unsigned char  *)stError.acct_union_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[38] = (unsigned long )21;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[38] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[38] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[38] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[38] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[38] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[38] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[39] = (unsigned char  *)stError.acct_net_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[39] = (unsigned long )21;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[39] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[39] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[39] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[39] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[39] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[39] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[40] = (unsigned char  *)stError.remark;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[40] = (unsigned long )101;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[40] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[40] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[40] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[40] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[40] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[40] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[41] = (unsigned char  *)stError.cert_type;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[41] = (unsigned long )3;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[41] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[41] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[41] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[41] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[41] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[41] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[42] = (unsigned char  *)stError.cert_no;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[42] = (unsigned long )33;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[42] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[42] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[42] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[42] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[42] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[42] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[43] = (unsigned char  *)stError.ext_properties;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[43] = (unsigned long )1001;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[43] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[43] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[43] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[43] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[43] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[43] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[44] = (unsigned char  *)stError.create_time;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[44] = (unsigned long )15;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[44] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[44] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[44] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[44] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[44] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[44] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[45] = (unsigned char  *)stError.update_time;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[45] = (unsigned long )15;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[45] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[45] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[45] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[45] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[45] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[45] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstv[46] = (unsigned char  *)(stSettle.pay_order);
#line 599 "settle_txn.pcpp"
    sqlstm.sqhstl[46] = (unsigned long )33;
#line 599 "settle_txn.pcpp"
    sqlstm.sqhsts[46] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqindv[46] = (         short *)0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqinds[46] = (         int  )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqharm[46] = (unsigned long )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqadto[46] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqtdso[46] = (unsigned short )0;
#line 599 "settle_txn.pcpp"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 599 "settle_txn.pcpp"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 599 "settle_txn.pcpp"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 599 "settle_txn.pcpp"
    sqlstm.sqpind = sqlstm.sqindv;
#line 599 "settle_txn.pcpp"
    sqlstm.sqpins = sqlstm.sqinds;
#line 599 "settle_txn.pcpp"
    sqlstm.sqparm = sqlstm.sqharm;
#line 599 "settle_txn.pcpp"
    sqlstm.sqparc = sqlstm.sqharc;
#line 599 "settle_txn.pcpp"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 599 "settle_txn.pcpp"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 599 "settle_txn.pcpp"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 599 "settle_txn.pcpp"
}

#line 599 "settle_txn.pcpp"

    if(SQLERR)
    {
        SYSError(RET_DATABASE, "查询户汇总(%s)入账失败表失败, err:(%d)(%s)", stSettle.mert_no,
            SQLCODE, sDbsError());
        return ;
    }
    else if(SQLNOTFOUND)
    {
        if(SETTLE_IN_FLAG_SUCC == stSettle.result_flag[0])
            return ;
        
        stError.do_flag[0] = SETTLE_ERR_NOT_DEAL;

        /* EXEC SQL INSERT INTO TBL_SETTLE_ERR (ID, SETTLE_DATE, IN_DATE, TX_DATE, SETTLE_NUM, 
            CHANNEL_NO, BATCH_NO, TRACE_NO, DO_FLAG, ERR_TYPE, MERT_NO, MERT_NM, ORDER_REQ, 
            ORDER_TIME, SETTLE_AMT, CURRENCY, STATUS, STATUS_MSG, BANK_STATUS, BANK_STATUS_MSG, 
            BANK_BATCH_NO, BANK_SEQNO, RESV_PHONE, ACCT_NO, ACCT_NM, ACCT_TYPE, ACCT_BANK_NO, 
            ACCT_BANK_NM, ACCT_ZBANK_NO, ACCT_ZBANK_NM, ACCT_ZBANK_ADDR, ACCT_ZBANK_CODE, 
            ACCT_UNION_NO, ACCT_NET_NO, REMARK, CERT_TYPE, CERT_NO, EXT_PROPERTIES, CREATE_TIME) 
        SELECT PAY_ORDER, SETTLE_DATE, :stSettle.in_date, TX_DATE, SETTLE_NUM, CHANNEL_NO, 
            BATCH_NO, TRACE_NO, :stError.do_flag, :stError.err_type, MERT_NO, MERT_NM, 
            ORDER_REQ, ORDER_TIME, SETTLE_AMT, CURRENCY, STATUS, STATUS_MSG, BANK_STATUS, 
            BANK_STATUS_MSG, BANK_BATCH_NO, BANK_SEQNO, RESV_PHONE, ACCT_NO, ACCT_NM, 
            ACCT_TYPE, ACCT_BANK_NO, ACCT_BANK_NM, ACCT_ZBANK_NO, ACCT_ZBANK_NM, 
            ACCT_ZBANK_ADDR, ACCT_ZBANK_CODE, ACCT_UNION_NO, ACCT_NET_NO, REMARK, CERT_TYPE, 
            CERT_NO, EXT_PROPERTIES, :stSettle.update_time
        FROM TBL_SETTLE_TXN WHERE ID = :stSettle.id; */ 
#line 626 "settle_txn.pcpp"

{
#line 613 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 613 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 613 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 613 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 613 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 613 "settle_txn.pcpp"
        sqlstm.stmt = "insert into TBL_SETTLE_ERR (ID,SETTLE_DATE,IN_DATE,T\
X_DATE,SETTLE_NUM,CHANNEL_NO,BATCH_NO,TRACE_NO,DO_FLAG,ERR_TYPE,MERT_NO,MERT_\
NM,ORDER_REQ,ORDER_TIME,SETTLE_AMT,CURRENCY,STATUS,STATUS_MSG,BANK_STATUS,BAN\
K_STATUS_MSG,BANK_BATCH_NO,BANK_SEQNO,RESV_PHONE,ACCT_NO,ACCT_NM,ACCT_TYPE,AC\
CT_BANK_NO,ACCT_BANK_NM,ACCT_ZBANK_NO,ACCT_ZBANK_NM,ACCT_ZBANK_ADDR,ACCT_ZBAN\
K_CODE,ACCT_UNION_NO,ACCT_NET_NO,REMARK,CERT_TYPE,CERT_NO,EXT_PROPERTIES,CREA\
TE_TIME)select PAY_ORDER ,SETTLE_DATE ,:b0 ,TX_DATE ,SETTLE_NUM ,CHANNEL_NO ,\
BATCH_NO ,TRACE_NO ,:b1 ,:b2 ,MERT_NO ,MERT_NM ,ORDER_REQ ,ORDER_TIME ,SETTLE\
_AMT ,CURRENCY ,STATUS ,STATUS_MSG ,BANK_STATUS ,BANK_STATUS_MSG ,BANK_BATCH_\
NO ,BANK_SEQNO ,RESV_PHONE ,ACCT_NO ,ACCT_NM ,ACCT_TYPE ,ACCT_BANK_NO ,ACCT_B\
ANK_NM ,ACCT_ZBANK_NO ,ACCT_ZBANK_NM ,ACCT_ZBANK_ADDR ,ACCT_ZBANK_CODE ,ACCT_\
UNION_NO ,ACCT_NET_NO ,REMARK ,CERT_TYPE ,CERT_NO ,EXT_PROPERTIES ,:b3  from \
TBL_SETTLE_TXN where ID=:b4";
#line 613 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 613 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )1695;
#line 613 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 613 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 613 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.in_date);
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )9;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstv[1] = (unsigned char  *)(stError.do_flag);
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstl[1] = (unsigned long )2;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhsts[1] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqindv[1] = (         short *)0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqinds[1] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqharm[1] = (unsigned long )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqadto[1] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqtdso[1] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstv[2] = (unsigned char  *)(stError.err_type);
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstl[2] = (unsigned long )2;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhsts[2] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqindv[2] = (         short *)0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqinds[2] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqharm[2] = (unsigned long )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqadto[2] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqtdso[2] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.update_time);
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstl[3] = (unsigned long )15;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhsts[3] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqindv[3] = (         short *)0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqinds[3] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqharm[3] = (unsigned long )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqadto[3] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqtdso[3] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.id);
#line 613 "settle_txn.pcpp"
        sqlstm.sqhstl[4] = (unsigned long )33;
#line 613 "settle_txn.pcpp"
        sqlstm.sqhsts[4] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqindv[4] = (         short *)0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqinds[4] = (         int  )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqharm[4] = (unsigned long )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqadto[4] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqtdso[4] = (unsigned short )0;
#line 613 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 613 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 613 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 613 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 613 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 613 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 613 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 613 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 613 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 613 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 613 "settle_txn.pcpp"
}

#line 626 "settle_txn.pcpp"

    }
    else
    {
        /* EXEC SQL UPDATE TBL_SETTLE_ERR SET ID = ID WHERE ID = :stSettle.id; */ 
#line 630 "settle_txn.pcpp"

{
#line 630 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 630 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 630 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 630 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 630 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 630 "settle_txn.pcpp"
        sqlstm.stmt = "update TBL_SETTLE_ERR  set ID=ID where ID=:b0";
#line 630 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 630 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )1730;
#line 630 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 630 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 630 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 630 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 630 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)(stSettle.id);
#line 630 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )33;
#line 630 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 630 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 630 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 630 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 630 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 630 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 630 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 630 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 630 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 630 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 630 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 630 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 630 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 630 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 630 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 630 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 630 "settle_txn.pcpp"
}

#line 630 "settle_txn.pcpp"

        if(SQL_UD_ERR)
        {
            SYSError(RET_DATABASE, "锁定入账差错记录(%s)失败, err:(%d)(%s)", stSettle.id, 
                SQLCODE, sDbsError());
            return ;
        }
    
        if(SETTLE_MCHT_SUCC == stFund.result_flag[0])
            stError.do_flag[0] = SETTLE_ERR_SETTLE_SUCC;

        /* EXEC SQL UPDATE TBL_SETTLE_ERR SET 
            DO_FLAG         = :stError.do_flag,
            ERR_TYPE        = :stError.err_type,
            IN_DATE         = :stSettle.in_date,    
            STATUS          = :stSettle.status,
            STATUS_MSG      = :stSettle.status_msg,
            BANK_STATUS     = :stSettle.bank_status,
            BANK_STATUS_MSG = :stSettle.bank_status_msg,
            BANK_BATCH_NO   = :stSettle.bank_batch_no,
            BANK_SEQNO      = :stSettle.bank_seqno,
            ORDER_REQ       = :stSettle.order_req,
            ORDER_TIME      = :stSettle.order_time,
            CURRENCY        = :stSettle.currency,
            RESV_PHONE      = :stSettle.resv_phone,
            ACCT_NO         = :stSettle.acct_no,
            ACCT_NM         = :stSettle.acct_nm,
            ACCT_TYPE       = :stSettle.acct_type,
            ACCT_BANK_NO    = :stSettle.acct_bank_no,
            ACCT_BANK_NM    = :stSettle.acct_bank_nm,
            ACCT_ZBANK_NO   = :stSettle.acct_zbank_no,
            ACCT_ZBANK_NM   = :stSettle.acct_zbank_nm,
            ACCT_ZBANK_ADDR = :stSettle.acct_zbank_addr,
            ACCT_ZBANK_CODE = :stSettle.acct_zbank_code,
            ACCT_UNION_NO   = :stSettle.acct_union_no,
            ACCT_NET_NO     = :stSettle.acct_net_no,
            CERT_TYPE       = :stSettle.cert_type,
            CERT_NO         = :stSettle.cert_no,
            UPDATE_TIME     = :stSettle.update_time
        WHERE ID = :stSettle.pay_order; */ 
#line 669 "settle_txn.pcpp"

{
#line 641 "settle_txn.pcpp"
        struct sqlexd sqlstm;
#line 641 "settle_txn.pcpp"
        sqlstm.sqlvsn = 12;
#line 641 "settle_txn.pcpp"
        sqlstm.arrsiz = 47;
#line 641 "settle_txn.pcpp"
        sqlstm.sqladtp = &sqladt;
#line 641 "settle_txn.pcpp"
        sqlstm.sqltdsp = &sqltds;
#line 641 "settle_txn.pcpp"
        sqlstm.stmt = "update TBL_SETTLE_ERR  set DO_FLAG=:b0,ERR_TYPE=:b1,\
IN_DATE=:b2,STATUS=:b3,STATUS_MSG=:b4,BANK_STATUS=:b5,BANK_STATUS_MSG=:b6,BAN\
K_BATCH_NO=:b7,BANK_SEQNO=:b8,ORDER_REQ=:b9,ORDER_TIME=:b10,CURRENCY=:b11,RES\
V_PHONE=:b12,ACCT_NO=:b13,ACCT_NM=:b14,ACCT_TYPE=:b15,ACCT_BANK_NO=:b16,ACCT_\
BANK_NM=:b17,ACCT_ZBANK_NO=:b18,ACCT_ZBANK_NM=:b19,ACCT_ZBANK_ADDR=:b20,ACCT_\
ZBANK_CODE=:b21,ACCT_UNION_NO=:b22,ACCT_NET_NO=:b23,CERT_TYPE=:b24,CERT_NO=:b\
25,UPDATE_TIME=:b26 where ID=:b27";
#line 641 "settle_txn.pcpp"
        sqlstm.iters = (unsigned int  )1;
#line 641 "settle_txn.pcpp"
        sqlstm.offset = (unsigned int  )1749;
#line 641 "settle_txn.pcpp"
        sqlstm.cud = sqlcud0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 641 "settle_txn.pcpp"
        sqlstm.sqlety = (unsigned short)4352;
#line 641 "settle_txn.pcpp"
        sqlstm.occurs = (unsigned int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[0] = (unsigned char  *)(stError.do_flag);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[0] = (unsigned long )2;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[0] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[0] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[0] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[0] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[0] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[0] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[1] = (unsigned char  *)(stError.err_type);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[1] = (unsigned long )2;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[1] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[1] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[1] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[1] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[1] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[1] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[2] = (unsigned char  *)(stSettle.in_date);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[2] = (unsigned long )9;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[2] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[2] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[2] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[2] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[2] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[2] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[3] = (unsigned char  *)(stSettle.status);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[3] = (unsigned long )31;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[3] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[3] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[3] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[3] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[3] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[3] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[4] = (unsigned char  *)(stSettle.status_msg);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[4] = (unsigned long )257;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[4] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[4] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[4] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[4] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[4] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[4] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[5] = (unsigned char  *)(stSettle.bank_status);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[5] = (unsigned long )31;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[5] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[5] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[5] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[5] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[5] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[5] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[6] = (unsigned char  *)(stSettle.bank_status_msg);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[6] = (unsigned long )257;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[6] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[6] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[6] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[6] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[6] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[6] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[7] = (unsigned char  *)(stSettle.bank_batch_no);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[7] = (unsigned long )33;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[7] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[7] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[7] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[7] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[7] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[7] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[8] = (unsigned char  *)(stSettle.bank_seqno);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[8] = (unsigned long )33;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[8] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[8] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[8] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[8] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[8] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[8] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[9] = (unsigned char  *)(stSettle.order_req);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[9] = (unsigned long )33;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[9] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[9] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[9] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[9] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[9] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[9] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[10] = (unsigned char  *)(stSettle.order_time);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[10] = (unsigned long )15;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[10] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[10] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[10] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[10] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[10] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[10] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[11] = (unsigned char  *)(stSettle.currency);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[11] = (unsigned long )6;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[11] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[11] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[11] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[11] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[11] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[11] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[12] = (unsigned char  *)(stSettle.resv_phone);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[12] = (unsigned long )12;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[12] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[12] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[12] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[12] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[12] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[12] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[13] = (unsigned char  *)(stSettle.acct_no);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[13] = (unsigned long )101;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[13] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[13] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[13] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[13] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[13] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[13] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[14] = (unsigned char  *)(stSettle.acct_nm);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[14] = (unsigned long )101;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[14] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[14] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[14] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[14] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[14] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[14] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[15] = (unsigned char  *)(stSettle.acct_type);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[15] = (unsigned long )4;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[15] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[15] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[15] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[15] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[15] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[15] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[16] = (unsigned char  *)(stSettle.acct_bank_no);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[16] = (unsigned long )21;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[16] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[16] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[16] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[16] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[16] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[16] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[17] = (unsigned char  *)(stSettle.acct_bank_nm);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[17] = (unsigned long )101;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[17] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[17] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[17] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[17] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[17] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[17] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[18] = (unsigned char  *)(stSettle.acct_zbank_no);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[18] = (unsigned long )21;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[18] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[18] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[18] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[18] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[18] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[18] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[19] = (unsigned char  *)(stSettle.acct_zbank_nm);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[19] = (unsigned long )101;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[19] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[19] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[19] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[19] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[19] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[19] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[20] = (unsigned char  *)(stSettle.acct_zbank_addr);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[20] = (unsigned long )161;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[20] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[20] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[20] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[20] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[20] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[20] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[21] = (unsigned char  *)(stSettle.acct_zbank_code);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[21] = (unsigned long )11;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[21] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[21] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[21] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[21] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[21] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[21] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[22] = (unsigned char  *)(stSettle.acct_union_no);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[22] = (unsigned long )21;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[22] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[22] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[22] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[22] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[22] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[22] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[23] = (unsigned char  *)(stSettle.acct_net_no);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[23] = (unsigned long )21;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[23] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[23] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[23] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[23] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[23] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[23] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[24] = (unsigned char  *)(stSettle.cert_type);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[24] = (unsigned long )3;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[24] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[24] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[24] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[24] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[24] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[24] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[25] = (unsigned char  *)(stSettle.cert_no);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[25] = (unsigned long )33;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[25] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[25] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[25] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[25] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[25] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[25] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[26] = (unsigned char  *)(stSettle.update_time);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[26] = (unsigned long )15;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[26] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[26] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[26] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[26] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[26] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[26] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstv[27] = (unsigned char  *)(stSettle.pay_order);
#line 641 "settle_txn.pcpp"
        sqlstm.sqhstl[27] = (unsigned long )33;
#line 641 "settle_txn.pcpp"
        sqlstm.sqhsts[27] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqindv[27] = (         short *)0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqinds[27] = (         int  )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqharm[27] = (unsigned long )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqadto[27] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqtdso[27] = (unsigned short )0;
#line 641 "settle_txn.pcpp"
        sqlstm.sqphsv = sqlstm.sqhstv;
#line 641 "settle_txn.pcpp"
        sqlstm.sqphsl = sqlstm.sqhstl;
#line 641 "settle_txn.pcpp"
        sqlstm.sqphss = sqlstm.sqhsts;
#line 641 "settle_txn.pcpp"
        sqlstm.sqpind = sqlstm.sqindv;
#line 641 "settle_txn.pcpp"
        sqlstm.sqpins = sqlstm.sqinds;
#line 641 "settle_txn.pcpp"
        sqlstm.sqparm = sqlstm.sqharm;
#line 641 "settle_txn.pcpp"
        sqlstm.sqparc = sqlstm.sqharc;
#line 641 "settle_txn.pcpp"
        sqlstm.sqpadto = sqlstm.sqadto;
#line 641 "settle_txn.pcpp"
        sqlstm.sqptdso = sqlstm.sqtdso;
#line 641 "settle_txn.pcpp"
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 641 "settle_txn.pcpp"
}

#line 669 "settle_txn.pcpp"

    }
    if(SQLCODE)
    {
		SYSError(RET_DATABASE, "操作入账失败表(%s)失败, err:(%d)(%s)", stSettle.id, 
            SQLCODE, sDbsError());
        return ;
    }

    return ;
}

/*************************************************************************************************
    code end
**************************************************************************************************/
