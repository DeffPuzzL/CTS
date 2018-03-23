#include	"tstr.h"

/**********************************************************************************
 **********************************************************************************/
/**********************************************************************************
 *  *  Function:  输出日志
 *  *  Description:		
 **********************************************************************************/
void    vTraceLog(char *pszLog, const char *pszFile, long lLine, long lState, 
			const char *fmt, ...)
{
    va_list ap;
    struct  stat st;
    long    lRet = 0;
    struct  timeb   tb;
    FILE    *fp = NULL;
    struct  tm  *ptm = NULL;
    char    szLog[256], szMsg[5120], szBak[512];

    memset(szLog, 0, sizeof(szLog));
    memset(szMsg, 0, sizeof(szMsg));
    va_start(ap, fmt);
    vsnprintf(szMsg, sizeof(szMsg) - 1, fmt, ap);
    va_end(ap);

	if(!pszLog || !strlen(pszLog))
		strcpy(szLog, LOG_DEFU_NAME);
	else
		strncpy(szLog, pszLog, sizeof(szLog));

    if(NULL == (fp = fopen(szLog, "a+")))
    {
        fprintf(stderr, "P(%d), 打开文件(%s)失败, err:%s\n", getpid(), szLog,
            strerror(errno));
        return ;
    }

    ftime(&tb);
    ptm = localtime(&tb.time);
    if(0 == lState) //  正常的trace
    {
    	fprintf(fp, "F=%-15s L=%-5d P=%-7d T=%04d%02d%02d %02d%02d%02d:%03d  %s\n",
    		pszFile, lLine, getpid(), ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, 
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tb.millitm, szMsg);
	}
	else
		fprintf(fp, "%s\n", szMsg);	
    fclose(fp);

    lRet = stat(szLog, &st);
    if(0 == lRet && st.st_size >= LOG_DEFU_SIZE)
    {
        memset(szBak, 0, sizeof(szBak));
        snprintf(szBak, sizeof(szBak), "%s.%04d%02d%02d%02d%02d%02d", szLog, 
			ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, 
			ptm->tm_min, ptm->tm_sec);
        rename(szLog, szBak);
    }

	return ;
}

/**********************************************************************************
 *  *  Function:  supper		将英文字符串改成大写
 *  *  Description:				改变输入字符串的值
 **********************************************************************************/
char	*supper(char *s)
{
	long	i, l = strlen(s), fg = 0;

	for(i=0; i < l; i++)
		s[i] = toupper(s[i]);

	return s;
}

/**********************************************************************************
 *  *  Function:  sLower		将英文字符串改成小写
 *  *  Description:				改变输入字符串的值
 **********************************************************************************/
char	*slower(char *s)
{
	long	i, l = strlen(s), fg = 0;

	for(i = 0; i < l; i++)
		s[i] = tolower(s[i]);

	return s;
}

/**********************************************************************************
 *  *  Function:  strimcrlf		去掉字符串中的回车换行
 *  *  Description:				改变输入字符串的值
 **********************************************************************************/
char*   strimcrlf(char *p)
{
	int	 i = 0, j = 0, m = 0;
	char	*s = NULL;

	if(!p)		   return p;
	if(!strlen(p))   return p;

	s = p;
	m = strlen(p);
	for(i = 0; i < m; i ++)
	{
		if(p[i] == 10 || p[i] == 13)
			continue;
		s[j] = p[i];
		j ++;
	}
	s[j] = 0x00;
	return s;
}

/**********************************************************************************
 *  *  Function:  sltrim		去掉字符串中的回车换行
 *  *  Description:				改变输入字符串的值
 **********************************************************************************/
char*	sltrim(char *p)
{
	char	*s = p;
	long	i, k = 0, l;

	if(!p || (0 == (l = strlen(p))))
		return p;

	for(i = 0; i < l; i ++, k ++)
	{
		if(p[i] != ' ' && '\t' != p[i])
			break;
	}

	if(0 == k)	return p;

	for(i = 0; i < l - k; i ++)
		p[i] = s[i + k];
	p[i] = 0x00;

	return p;
}

/**********************************************************************************
 *  *  Function:  srtrim		去掉字符串中的回车换行
 *  *  Description:				改变输入字符串的值
 **********************************************************************************/
char*	srtrim(char *p)
{
	long	i, k = 0, l = 0;

	if(!p || 0 == (l = strlen(p)))
		return p;

	for(i = l - 1; i >= 0; i --)
	{
		if(p[i] == ' ' || '\t' == p[i])
			continue;
		break;
	}

//	p[i + 1] = '\0';
	memset(p + i + 1, 0, l - i - 1);

	return p;
}

/**********************************************************************************
 *  *  Function:  strimall		去掉字符串中的所有空格
 *  *  Description:				改变输入字符串的值
 **********************************************************************************/
char*	strimall(char *p)
{
	long	i, k = 0, l = 0;
	char	*q = p;

	if(!p || !strlen(p))
		return p;

	l = strlen(p);
	for(i = 0; i < l; i ++)
	{
		if(p[i] == ' ')
			continue;
		q[k ++] = p[i];
	}
	memset(q + k, 0, l - k);
	p = q;

	return q;
}

/**********************************************************************************
 *  *  Function: trim非字段空格
 *  *  Description:	
 **********************************************************************************/
char*   strimfield(char *s)
{
    register int    i, n, m;
    BOOL    bf = false;

    if(!s || 0 == (n = strlen(s)))
        return s;

    for(i = 0, m = 0; i < n; i ++)
    {
        if(s[i] == '\"')
            bf = !bf;

        if(bf)  s[m ++] = s[i];
        else if(s[i] != ' ' && s[i] != '\t')
            s[m ++] = s[i];
    }

    s[m] = 0x00;

    return s;
}

/**********************************************************************************
 *  *  Function:  获取k字符分割，有效字符个数
 *  *  Description:				配合sfieldvalue使用
 **********************************************************************************/
long	lfieldnum(char *p, char *k)
{
    char    *y = p;
    BOOL    bf = false;
    long    idx, i, m, n;

    if(!p || !k) return 0;

    for(i = 0, idx = 0, m = strlen(p), n = strlen(k); i < m; i ++)
    {
        if(p[i] == '\\')
        {
            i ++;
            continue;
        }

        if(p[i] == '\"' || p[i] == '\'')
            bf = !bf;

        if(bf)	continue;

        if(!memcmp(p + i, k, n))
        {
            ++ idx;
            for(i += n; i < m; i ++)
            {
                if(p[i] != ' ' && p[i] != '\t' && memcmp(p + i, k, n))
                    break;
            }

            y = p + i;
			i --;
        }
    }

    for(i = y - p ; i < m; i ++)
    {
        if(y[i] != ' ' && y[i] != '\t')
            return ++ idx;
    }

    return idx;
}

/**********************************************************************************
 *  *  Function:  bIsgbk
 *  *  Description:					判断是否为中文
 **********************************************************************************/
int     bIsgbk(const char *s)
{
    if((ushar)s[0] > 0x80 && (ushar)s[1] >= 0x40)
        return 1;
    return 0;
}

/**********************************************************************************
 *  *  Function:  sfieldvalue	符串s出现总次数
 *  *  Description:					根据分隔符和域值取数据
 **********************************************************************************/
char*	sfieldvalue(char *p, char *k, int id)
{
    char    *y = p;
    BOOL    bf = false;
    long    idx, i = 0, m = 0, n = 0;
    static  char    szOut[1024]; 
    
    memset(szOut, 0, sizeof(szOut));
    if(!p || !k || id <= 0) return szOut;
    
    for(i = 0, idx = 0, m = strlen(p), n = strlen(k); i < m; i ++)
    {
        if(p[i] == '\\')
        {
            i ++;
            continue;
        }   
        
        if(p[i] == '\"' || p[i] == '\'')
        {
            bf = !bf;
            continue;
        }   
        if(bf)  continue;
        
        if(!memcmp(p + i, k, n))
        {
            if(++idx == id)
                break;
                
            for(i += n; i < m; i ++)
            {
                if(p[i] != ' ' && p[i] != '\t' && memcmp(p + i, k, n))
                    break; 
            }       
            
            y = p + i;
			i --;
        }   
    }   
    
    if(idx + 1 < id) return szOut;
    memcpy(szOut, y, i - (long )(y - p));
    
    return szOut; 
}

/****************************************************************************************
	按照标准分隔符获取数据
 ****************************************************************************************/
char*   sgetvalue(char *p, char *s, int id)
{
    char    *y = p;
    long    i, m, n, idx = 0;
    static  char    szOut[1024];

    memset(szOut, 0, sizeof(szOut));
    n = strlen(s);
    if(!p || !s || id <= 0 || n <= 0) return szOut;

    for(i = 0, idx = 0, m = strlen(p); i < m; i ++)
    {
        if(!memcmp(p + i, s, n))
        {
            if(i > 0 && bIsgbk(p + i - 1))
                continue;

            if(++ idx == id)
                break;

            y = p + i + n;
        }
    }

    if(idx + 1 < id) return szOut;
    memcpy(szOut, y, i - (int)(y - p));

    return szOut;
}

/****************************************************************************************
	去除空格和制表符字符串拷贝
 ****************************************************************************************/
char*	strimcpy(char *d, char *s, int l)
{
	register    int     i = 0;

	if(d == NULL || s == NULL || 0 >= l)
        return d;

    for(i = 0; i < l & '\0' != *s; *s ++)
    {
        if(*s == ' ' || '\t' == *s)
            continue;
        *d++ = *s;
        i ++;
    }
    *d = '\0';

	return d;
}

/****************************************************************************************
	字符替换
 ****************************************************************************************/
char*    sfieldreplace(char *p, char o, char d)
{
    BOOL    bf = false;
    long    idx, i, m = 0;

    if(!p) return p;

    for(i = 0, idx = 0, m = strlen(p); i < m; i ++)
    {
        if(p[i] == '\\')
        {
            i ++;
            continue;
        }

        if(p[i] == '\"' || p[i] == '\'')
            bf = !bf;

        if(bf)  continue;

        if(o == p[i])
        {
            if(i > 0 && bIsgbk(p + i - 1))
                continue;

            p[i] = d;
        }
    }

    return p;
}

/*************************************************************************************************
	获取字符串o/d之前的值, 修改值
 *************************************************************************************************/
char*   strimabout(char *s, char *o, char *d)
{
    long    l = 0;
    char    *p = NULL, *q = NULL, *m = NULL;

    if(!s || !o || !d)              return NULL;
    if(0 == (l = strlen(s)))        return NULL;
    if(NULL == (p = strstr(s, o)))    return NULL;

    for(l; l > 0; l --)
    {
        if(NULL != (q = strstr(s + l, d)))
            break;
    }
    if(!q)  return NULL;

    l = strlen(o);
    if((p - q) >= 0)	return NULL;

    if(((p - q + l) == 0))
	{
		s[0] = 0x00;
        return NULL;
	}

    for(m = s, p += l; p != q; *p ++, *m ++)
        *m = *p;
    *m = 0x00;

    return s;
}

/****************************************************************************************
	code end
****************************************************************************************/

