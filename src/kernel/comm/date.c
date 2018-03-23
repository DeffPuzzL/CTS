#include    "Ibp.h" 

/**************************************************************************************************
    功 能 ： 计算当前时间的偏移lMin后的时间，不计秒
    参 数 ： 
    返回值： 时间字符串
 **************************************************************************************************/
char*    sGetTimeAlgo(long lMin)
{
    time_t  ctm;
    long    tim;
    struct  tm   am;
    static char  szTime[7];

    time(&ctm);
	localtime_r(&ctm, &am);

    tim = am.tm_hour * 3600 + am.tm_min * 60 + lMin * 60;
    if(tim < 0)    tim = 0;
    snprintf(szTime, sizeof(szTime), "%02d%02d00", tim / 3600, tim % 3600 / 60);
	szTime[6] = 0x00;
    
    return szTime;
}

/**************************************************************************************************
    功 能 ： 计算指定时间的偏移lMin后的时间，不计秒
    参 数 ： 
    返回值： 时间字符串
 **************************************************************************************************/
char*    sGetMinsOfs(char *pszTime, long lOfs)
{
    long    tim;
    struct  tm  tm;
    static char szTime[7], s[3];

    if(6 != strlen(pszTime))
        return pszTime;

	memset(s, 0, sizeof(s));
    memcpy(s, pszTime, 2);
    tm.tm_hour = atoi(s);
    memcpy(s, pszTime + 2, 2);
    tm.tm_min = atoi(s);
    memcpy(s, pszTime + 4, 2);
    tm.tm_sec = atoi(s);

    tim = tm.tm_hour * 60 + tm.tm_min + lOfs % (60 * 24);
    if(tim < 0)    tim = 0;
    snprintf(szTime, sizeof(szTime), "%02d%02d%02d", tim / 60, tim % 60, tm.tm_sec);
	szTime[7] = 0x00;

    return szTime;
}

/**************************************************************************************************
    功 能 ： 获取当前日期(针对多线程优化函数)
    参 数 ： 无    
    返回值： 当前日期字符串
 **************************************************************************************************/
char*    sFormatDate(time_t t)
{
    struct tm am;
    static char  szTime[20];
   
	localtime_r(&t, &am);
    snprintf(szTime, sizeof(szTime), "%4d-%02d-%02d %02d:%02d:%02d", am.tm_year + 1900, 
        am.tm_mon + 1, am.tm_mday, am.tm_hour, am.tm_min, am.tm_sec);
	szTime[19] = 0x00;

    return szTime;
}

/**************************************************************************************************
    功 能 ： 获取当前日期(针对多线程优化函数)
    参 数 ： 无    
    返回值： 当前日期字符串
 **************************************************************************************************/
char*    _sGetDate(char *p, long n)
{
    time_t ctm;
    struct tm am;
   
    if(!p)    return p;

    time(&ctm);
	localtime_r(&ctm, &am);
    snprintf(p, n, "%4d%02d%02d", am.tm_year + 1900, am.tm_mon + 1, am.tm_mday);
    p[8] = 0x00;

    return p;
}

/**************************************************************************************************
    功 能 ： 获取当前日期
    参 数 ： 无    
    返回值： 当前日期字符串
 **************************************************************************************************/
char*    sGetDate()
{  
    static char szDate[9];
   
    memset(szDate, 0, sizeof(szDate));
    _sGetDate(szDate, sizeof(szDate));

    return szDate;
}

/**************************************************************************************************
    功 能 ： 获取日志时间
    参 数 ： 无    
    返回值： 当前日期字符串
 **************************************************************************************************/
char*    sGetLogTime()
{
    struct tm     am;
    struct timeb  tb;
    static char   szDate[20];

    ftime(&tb);
	localtime_r(&tb.time, &am);
    snprintf(szDate, sizeof(szDate), "%04d%02d%02d %02d%02d%02d:%03d", am.tm_year + 1900, 
		am.tm_mon + 1, am.tm_mday, am.tm_hour, am.tm_min, am.tm_sec, tb.millitm);
    szDate[19] = 0;

    return szDate;
}

/**************************************************************************************************
    功 能 ： 获取当前时间
    参 数 ： 
    返回值： 当前日期字符串
 **************************************************************************************************/
char*    sGetTime()
{
    long    ctm;
    struct  tm  am;
    static  char  szTime[7];

    time(&ctm);
	localtime_r(&ctm, &am);

    snprintf(szTime, sizeof(szTime), "%02d%02d%02d", am.tm_hour, am.tm_min, am.tm_sec);
	szTime[6] = 0x00; 
    
    return szTime;
}

/**************************************************************************************************
    功 能 ： 获取当前时间,时间格式YYYYMMDDhhmmss
    参 数 ： 无
    返回值： 当前日期字符串
 **************************************************************************************************/
char*    sGetCurrentTime()
{  
    time_t    tb;
    struct    tm am;
    static    char    szTime[15];
   
    time(&tb);
	localtime_r(&tb, &am);

    snprintf(szTime, sizeof(szTime), "%4d%02d%02d%02d%02d%02d", am.tm_year + 1900, 
        am.tm_mon + 1, am.tm_mday, am.tm_hour, am.tm_min, am.tm_sec);
	szTime[14] = 0x00;

    return szTime;
}

/*************************************************************************************************/
/*   功能  ： 获取1970年到现在的秒速                                                              */
/*   参数  ： 无                                                                                 */
/*  返回值 ：                                                                                    */
/*************************************************************************************************/
long    lGetUnixTime()
{
    return (long)time(NULL);
}

/**************************************************************************************************/
/* FUNC:   double DecRound(double value,int dot)                                                  */
/* INPUT:  value: 要进行转换的双精度浮点数据                                                        */
/*           dot    : 四舍五入要保留的位数                                                         */
/* OUTPUT: NULL                                                                                   */
/* RETURN: 成功：转换后的数据                                                                      */
/* DESC     : 四舍五入转换函数                                                                     */
/**************************************************************************************************/
double    dDecRound(double dValue, long lDot)
{
    double  dAa;
    char    szDot[40], szDec[40];

    memset(szDot, 0, sizeof(szDot));
    memset(szDec, 0, sizeof(szDec));
    sprintf(szDec, "%s%dlf", "%.", lDot);    
    if(dValue >= 0)
        dAa = dValue + 0.00000001;
    else
        dAa = dValue - 0.00000001;

    snprintf(szDot, sizeof(szDot), szDec, dAa);    
    dAa = atof(szDot);

    return dAa;
} 

/**************************************************************************************************/
/*   声明  ： char* sGetChgDate(char *pszInDay, long lDay)                                        */
/*   功能  ： 取指定日期与nDay相隔的日期                                                            */
/*   参数  ： pszInDay --指定原日期                                                                */
/*         ： lDay     --相隔天数                                                                  */
/*  返回值 ： 0--  成功                                                                            */
/*           -1--  失败                                                                           */
/**************************************************************************************************/
char*    sGetChgDate(char *pszInDay, long lDay)
{
    time_t    tb;
    struct tm am;
    char      szTmp[9];
    static    char    szDate[9];

    memset(szTmp, 0, sizeof(szTmp));
	memset(&am, 0, sizeof(struct tm));
    memcpy(szTmp, pszInDay, 4);
    am.tm_year = atoi(szTmp) - 1900;

    memset(szTmp, 0, sizeof(szTmp));
    memcpy(szTmp, pszInDay + 4, 2);
    am.tm_mon = atoi(szTmp) - 1;

    memset(szTmp, 0, sizeof(szTmp));
    memcpy(szTmp, pszInDay + 6, 2);
    am.tm_mday = atoi(szTmp);
    
    tb = mktime(&am) + lDay * 24l * 3600l;
	localtime_r(&tb, &am);
    snprintf(szDate, sizeof(szDate), "%04d%02d%02d", am.tm_year + 1900, 
        am.tm_mon + 1, am.tm_mday);
	szDate[8] = 0x00;

    return szDate;
}

/**************************************************************************************************/
/*   声明  ： char* sGetChgDate(char *pszInDay, long lDay)                                        */
/*   功能  ： 取指定日期与nDay相隔的日期                                                           */
/*   参数  ： pszInDay --指定原日期                                                               */
/*         ： lDay     --相隔天数                                                                 */
/*  返回值 ： 0--  成功                                                                           */
/*           -1--  失败                                                                           */
/**************************************************************************************************/
long    lIsValidDate(char *pszDate)
{
    char    szTemp[5];    
    long    lYear, lMonth, lDay, i;

    if(!pszDate || 8 != strlen(pszDate))
        return -1;

    memset(szTemp, 0, sizeof(szTemp));
    for(i = 0; i < 8; i ++)
    {
        if('0' <= pszDate[i] && pszDate[i] <= '9')
            continue;
    }
    memcpy(szTemp, pszDate, 4);
    lYear = atoi(szTemp);

    memset(szTemp, 0, 4);
    memcpy(szTemp, pszDate + 4, 2);
    if((lMonth = atoi(szTemp)) > 12)
        return -1;

    memcpy(szTemp, pszDate + 6, 2);
    lDay = atoi(szTemp);
    switch(lMonth)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(lDay > 31) return -1;    
            return 0;
        case 4:
        case 6:
        case 9:
        case 11:
            if(lDay > 30) return -1;    
            return 0;
        case 2: /*2月份比较特殊，闰年: 29天，平年: 28天*/
        {
            if((0 == lYear % 4 && 0 != lYear % 100) || (0 == lYear % 400))
            {
                if(lDay > 29)   return -1;
                else            return 0;
            }
            else
            {
                if(lDay > 28)   return -1;
                else            return 0;
            }
        }
        default: return -1;
    }

    return 0;
}
