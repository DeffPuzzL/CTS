/**************************************************************************************************
    文 件 名：demo.pc
    代码作者：DeffPuzzL
    编写版本：
    创建日期：2016-07-14
    功能描述：demo实现业务模块
 **************************************************************************************************/
#include    "demo_test.h"


/**************************************************************************************************
    方法定义
 **************************************************************************************************/
CDemo::stEvent CDemo::m_stEvent[] =
{
    {"test",            &CDemo::vDemoTest },
    {"print",           &CDemo::vDemoPrint },
    {"trade",           &CDemo::vDemoTrade },
    {"error",           &CDemo::vDemoError },
    {"sleep",           &CDemo::vDemoSleep },
    { "", NULL }
};

/**************************************************************************************************
    函 数 名：CDemo()
    功能描述：构造函数
    返回说明：
 **************************************************************************************************/
CDemo::CDemo()
{

}

/**************************************************************************************************
    函 数 名：~CDemo()
    功能描述：析构函数
    返回说明：
 **************************************************************************************************/
CDemo::~CDemo()
{


}

/**************************************************************************************************
    函 数 名：lRunObject(char* in_szClass, char* in_szAction)
    功能描述：运行类中的方法
    返回说明：
        RC_SUCC         --成功
        RC_FAIL         --失败
 **************************************************************************************************/
long    CDemo::lRunObject(char *in_szClass, char *in_szAction)
{
    int     i = 0;
    char    *p, szAction[256];

    memset(szAction, 0, sizeof(szAction));
    if ((p = strstr(in_szAction, "__")) != NULL)
        strncpy(szAction, in_szAction, p - in_szAction);
    else
        strcpy(szAction, in_szAction);

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
    函 数 名：vDemoTest()
    功能描述：
    返回说明：
        无
 **************************************************************************************************/
void    CDemo::vDemoTest()
{
    double  d = 10.32421;
    GCts    *pstCts = (GCts *)pGetGloble();

    IBPrace("当前交易流水(%d), 日期(%s)(%s)", pstCts->m_lSeqNo, pstCts->m_szTxDate,
    	pstCts->m_szTxTime);

    IBPrace("%6f", d);
    IBPputdoublepcs("test_double_1", d, 1);
    IBPputdoublepcs("test_double_2", d, 2);
    IBPputdoublepcs("test_double_3", d, 3);
    IBPputdoublepcs("test_double_4", d, 4);


    IBPputstring("tel", (char *)"18672966419");
    IBPputstring("ibox42", (char *)"001000123321781");
    IBPputstring("ibox43", (char *)"盒子支付");
    IBPputstring("R_CON", (char *)"0");
    IBPputlong("K_CON", 8);
}


/*************************************************************************************************
    组件函数
**************************************************************************************************/
void    CDemo::vDemoPrint()
{
    long    lNo = 0, lSeqNo = 0;
    double  dAmt = 0.00;
    GCts    *pstCts = (GCts *)pGetGloble();
    SATvm   *pstSamo = (SATvm *)pGetSATvm();

    IBPgetdeflong("C_CON", &lNo, 800);
    IBPgetdefdouble("TX_amt", &dAmt, 8.23);
    if (IBPIserr())
        return ;

    IBPrace("当前交易流水(%d), 接入PID(%d), lNo(%d), (%.6f)", pstCts->m_lSeqNo, 
		pstCts->m_lInPid, lNo, dAmt);
    IBPrace("Param:[%s]", pstCts->m_szParam);

    return ;
}

/*************************************************************************************************
    组件函数
**************************************************************************************************/
void	CDemo::vDemoError()
{
 	SYSError(RET_SYSTEM, "设置系统错误");
	return ;
}

/*************************************************************************************************
    组件函数
**************************************************************************************************/
void	CDemo::vDemoSleep()
{
	long	lSeep = 0;

	IBPgetdeflong("SLEEP", &lSeep, 5);

	IBPrace("Sleep睡眠%ds", lSeep);

//	SYSError(RET_SYSTEM, "测试返回出错");

	sleep(lSeep);	
}

/*************************************************************************************************
    组件函数
**************************************************************************************************/
void	CDemo::vDemoTrade()
{
	long	lSeqNo;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

	IBPputstring("Trade", "9000000");

	if(RC_FAIL == (lSeqNo = lNewTrade(pstSavm, pGetCmie(), "100000", false)))
	{
    	SYSError(RET_SYSTEM, "发起新交易(10000)失败, %s", sGetTError(pstSavm->m_lErrno)); 
		return ;
	}

	IBPprintpool();
}

/*************************************************************************************************
    code end
**************************************************************************************************/
