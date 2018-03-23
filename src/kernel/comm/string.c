#include	"Ibp.h"

/**********************************************************************************
	数字转化字符编码表 
 **********************************************************************************/
static const  char    g_szCTab[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', 
								'9', 'A', 'B', 'C', 'D', 'E', 'F'};

/**********************************************************************************
	Function:  sUpper        将英文字符串改成大写
	Description:             改变输入字符串的值
 **********************************************************************************/
char    *sUpper(char *s)
{
    long    i, l = strlen(s);

    for(i = 0; i < l; i++)
		s[i] = tolower(s[i]);
	return s;
}

/**********************************************************************************
	Function:  sLower        将英文字符串改成小写
	Description:             改变输入字符串的值
 **********************************************************************************/
char    *sLower(char *s)
{
    long    i, l = strlen(s);

    for(i = 0; i < l; i++)
		s[i] = tolower(s[i]);
    return s;
}

/**********************************************************************************
	Function:  sTrimCRLF     去掉字符串中的回车换行
	Description:             改变输入字符串的值
 **********************************************************************************/
char*   sTrimCRLF(char *p)
{
    int  i = 0, j = 0, m = 0;
    char    *s = NULL;

    if(!p)         return p;
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
	Function:  sTrimLeft     去掉字符串中的回车换行
	Description:             改变输入字符串的值
 **********************************************************************************/
char*   sTrimLeft(char *p)
{
    long    i, k = 0, l;

    if(!p || !strlen(p))
        return p;

    l = strlen(p);
    for(i = 0; i < l; i ++)
    {
        if(p[i] == ' ' || '\t' == p[i])
        {
            k ++;
            continue;
        }
        break;
    }

    return p + k;
}

/**********************************************************************************
	Function: sTrimRight        去掉字符串中的回车换行
	Description:             改变输入字符串的值
 **********************************************************************************/
char*   sTrimRight(char *p)
{
    long    i, k = 0, l = 0;

    if(!p || !strlen(p))
        return p;

    l = strlen(p);
    for(i = strlen(p) - 1; i >= 0; i --)
    {
        if(p[i] == ' ' || '\t' == p[i])
            continue;
        break;
    }

	memset(p + i + 1, 0, l - i - 1);

	return p;
}

/**********************************************************************************
	Function:  sTrimAll      去掉字符串中的所有空格
	Description:             改变输入字符串的值
 **********************************************************************************/
char*   sTrimAll(char *p)
{
    long    i, k = 0, l = 0;
    char    *q = p;

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
 *  *  Function:  sTrimCRLF		去掉字符串中的回车换行
 *  *  Description:				改变输入字符串的值
 **********************************************************************************/
long	lTrimCRLF(char *p, long l)
{
	int		i = 0, j = 0;
	char	*s = NULL;

	if(!p)		   return 0;

	for(s = p, i = 0; i < l; i ++)
	{
		if(p[i] == 10 || p[i] == 13)
			continue;
		s[j ++] = p[i];
	}
	s[j] = 0x00;

	return j;
}

/**********************************************************************************
 *  *  Function:  lGetRecordNum		符串s出现总次数
 *  *  Description:					获取p 字符串中  s字符串出现总次数
 **********************************************************************************/
char*	sAlterPath(char *p, long l, char *s)
{
	long	i;

	if(!p || !s) return p;

	for(i = strlen(p) - 1; 0 < i; -- i)
	{
#if(defined(__WIN32__) || defined(_WIN32))
		if(p[i] == '\\')
#else
		if(p[i] == '/')
#endif
		{
			++ i;
			strncpy(p + i, s, l - i);
			return p;
		}
	}

	return p;
}

/**********************************************************************************
 *  *  Function:  lGetRecordNum		符串s出现总次数
 *  *  Description:					获取p 字符串中  s字符串出现总次数
 **********************************************************************************/
long	lGetRecordNum(char *p, char *s, long l)
{
	long	i, iLen, iCount = 0;
	char	*q = p;

	if(!p)		return 0;

	iLen = strlen(s);
	for(i = 0; i < l; i++, q++ )
	{
		if(0 == memcmp(q, s, iLen))
		{
			q = q + iLen - 1;
			iCount ++;
		}
	}

	return iCount;
}

/**********************************************************************************
 *  *  Function:  获取k字符分割，有效字符个数
 *  *  Description:				配合sGetValueByIdx使用
 **********************************************************************************/
long	lGetValidNum(char *s, char *k)
{
	long	n = 0, l, i = 0;
	char	*p = NULL;

	if(!s || !k || !strlen(k))  return n;

	l = strlen(k);
	while((p = strstr(s + i, k)))
	{
		++ n;
		i = l + (p - s);
	}

	if(strlen(s + i) > 0)   n ++;

	return n;
}

/**********************************************************************************
 *  *  Function:  sGetValueByIdx	符串s出现总次数
 *  *  Description:					根据分隔符和域值取数据
 **********************************************************************************/
char*   sGetValueByIdx(char *p, char *s, long id) 
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
* *  Function:  lStringReplace 	在字符中将o替换成d 
* *  Description:				返回字符串长度, 返回内存需自己释放
****************************************************************************************/
long	lStringReplace(char *s, char *o, char *d, char **pp)
{
	char	*p = NULL;
	long	l, i, j = 0, n, m = 0, len = 0;

	if(!s || !o || !d || !strlen(o) || !strlen(d))  return -1;

	l = strlen(s);
	n = strlen(d);
	m = strlen(o);

	len = lGetRecordNum(s, o, l) * (n - m) + l + 1;

	if(!(p = (char *)malloc(len)))
		return -1;

	bzero(p, len);

	for(i = 0, j = 0; i < l; i ++)
	{
		if(!memcmp(s + i, o, m))
		{
			memcpy(p + j, d, n);
			j += n;
		}
		else
			p[j ++] = s[i];
	}

	*pp = p;

	return len - 1;
}


/****************************************************************************************
* *  Function:  sCharReplace	字符替换，修改原字符串
* *  Description:			
****************************************************************************************/
char*	sCharReplace(char *s, char o, char d)
{
	long	l, i;

	if(!s)  return s;

	l = strlen(s);
	for(i = 0; i < l; i ++)
	{
		if(o == s[i])
			s[i] = d;
	}

	return s;
}

/****************************************************************************************
	Function:  sDesensTel   对手机号脱敏
	Description:			
 ****************************************************************************************/
char    *sDesensTel(char *s)
{
    long    l = 0;
    static  char    szOut[20];

    memset(szOut, 0, sizeof(szOut));
    if(!s)  return NULL;
    if((l = strlen(s)) < 4) return s;

    strncpy(szOut, s, 3);
    strcat(szOut, "****");
    strcat(szOut, s + (l - 4));

    return szOut;
}

/****************************************************************************************
	Function:  sDesensAct   对账号脱敏
	Description:			
 ****************************************************************************************/
char    *sDesensAct(char *s)
{
    long    l = 0;
    static  char    szOut[20];

    memset(szOut, 0, sizeof(szOut));
    if(!s)  return NULL;
    if((l = strlen(s)) < 7) return s;

    strncpy(szOut, s, 6);
    strcat(szOut, "********");
    strcat(szOut, s + (l - 4));

    return szOut;
}

/****************************************************************************************
	Function:  sDesensName   对用户名脱敏
	Description:			
 ****************************************************************************************/
char    *sDesensName(char *s)
{
    static  char    szOut[10];

    memset(szOut, 0, sizeof(szOut));
    if(!s || !strlen(s))  return s;

//#ifdef        CODING_GBK
    if((uchar)s[0] > 0x80)
        strncpy(szOut, s, 2);
    else
        strncpy(szOut, s, 1);
    strcat(szOut, "***");

    return szOut;
}

/****************************************************************************************
	Function:  sDesensMail   对邮件名称脱敏
	Description:			
 ****************************************************************************************/
char    *sDesensMail(char *s)
{
    char    *p = NULL;
    static  char    szOut[64];

    memset(szOut, 0, sizeof(szOut));
    if(!s || !(p = strrchr(s, 64)))  return s;

    szOut[0] = s[0];
    strcat(szOut, "******");
    strcat(szOut, p);

    return szOut;
}

/****************************************************************************************
* *  Function:  sEncryAcNo   加密卡号 采用前6后4方式，其他用*代替
* *  Description:			
****************************************************************************************/
char	*sEncryAcNo(char *s)
{
	long	l = 0, i = 0; 
	static  char	szDesen[64];

	memset(szDesen, 0, sizeof(szDesen));
	if(!s)	return NULL;
	l = strlen(s);
	if(l <= 10)		return szDesen;
	strncpy(szDesen, s, 10);

	for(i = 6; i < l - 4; i ++)
		szDesen[i] = '*'; 
	memcpy(szDesen + i, s + (l - 4), 4);
	return szDesen;
}

/****************************************************************************************
 *     从字符串s中删除指定字符k
 ****************************************************************************************/
char*   sDropCharset(char *s, char k)
{
    long    i = 0, l, j = 0;
    char    *p = NULL;

    if(!s)  return s;
    l = strlen(s);
    if(0 == l)  return s;

    for(i = 0, p = s; i < l; i ++)
    {
		if(s[i] == '\\')
		{
			++ i;
			continue;
		}

        if(k == s[i])
            continue;
        p[j ++] = s[i];
    }
    p[j] = 0x00;

    return p;
}

/****************************************************************************************
* *  Function:  sStringToHex  将内存BCD码转换明文BCD码
* *  Description:			
****************************************************************************************/
long	lStringToHex(char *pszOrg, long lLen, char *pszOut, long lSize)
{
	long	i = 0, j = 0;
	char	*p = pszOut;

	if(!pszOrg || !pszOut || 0 == lLen)	return 0;

	for(i, j = lSize; i < lLen && j > 0; i ++)
		snprintf(p + i * 2, j - i * 2, "%02X", (uchar )pszOrg[i]);

	return i * 2;
}

/****************************************************************************************
* *  Function:  vHexToString 将明文BCD码转换内存BCD码
* *  Description:			
****************************************************************************************/
long	lHexToString(char *pszOrg, long lLen, char *pszOut, long lSize)
{
	long	i = 0, j = 0, n;
	char	szTemp[3], *p = pszOut;

	if(!pszOrg || !pszOut)	return 0;

	memset(szTemp, 0, sizeof(szTemp));
	for(i, j = lSize; (i * 2) < lLen && i < j; i ++)
	{
		memcpy(szTemp, pszOrg + i * 2, 2);
		sscanf(szTemp, "%02X", &n);
		p[i] = n;
	}

	return i;
}

/****************************************************************************************
	Function:  Round   实现四舍五入保留iPlaces位小数
	Description:            
 ****************************************************************************************/
double  Round(double dVal, short iPlaces)
{
    double  dAmt = 0.00;
    double  dMod = 0.0000001;

    if(dVal < 0.0)  dMod = -0.0000001;

    dAmt  = dVal;
    dAmt += (5.0/pow(10.0, iPlaces + 1.0));
    dAmt *= pow(10.0, iPlaces);
    dAmt  = floor(dAmt + dMod);
    dAmt /= pow(10.0, iPlaces);

    return  dAmt;
}

/****************************************************************************************
	在字符串中查找指定字符串最后一次出现的位置	
 ****************************************************************************************/
char*   strrstr(char *s, const char *d)
{
    register int    i, l, j;

    if(!s || 0 == (l = strlen(s)))  return NULL;
    if(!d || 0 == (j = strlen(d)))  return NULL;

    for(i = l - j; i >= 0; i --)
    {   
        if(!memcmp(s + i, d, j))
            return s + i;
    }

    return NULL;
}

/****************************************************************************************
	获取字符串s在o和d区间的值
 ****************************************************************************************/
char*   sGetBlockValue(char *s, const char *o, const char *d)
{ 
	char    *p = NULL, *q = NULL;
	long    l = 0, m, i, n;

	if(!s || !o || !d)  return s;

	n = strlen(s);
	l = strlen(o);
	if((NULL == (p = strstr(s, o))) || (NULL == (q = strrstr(s, d))))
		return NULL;

	if((m = (q - p) - l) <= 0)  return NULL;
    for(i = 0, p = p + l; i < m; i ++, p ++)
		s[i] = *p;
	memset(s + i, 0, n - i);

    return s;
}

/****************************************************************************************
	字符串拷贝遇到指定字符停止
 ****************************************************************************************/
long	chrcpy(char *d, char *s, char c, int l)
{
	char	*p = d;
	register    int     i = 0;

	if(p == NULL || s == NULL)
        return;

    for(i = 0; i < l && '\0' != *s && c != *s; *s ++, i ++)
        *p++ = *s;
    *p = '\0';

	return i;
}

/****************************************************************************************
	去除空格和制表符字符串拷贝
 ****************************************************************************************/
void	trmcpy(char *d, char *s, int l)
{
	register    int     i = 0;

	if(d == NULL || s == NULL || 0 >= l)
        return;

    for(i = 0; i < l & '\0' != *s; *s ++)
    {
        if(*s == ' ' || '\t' == *s)
            continue;
        *d++ = *s;
        i ++;
    }
    *d = '\0';
}

/****************************************************************************************
	将十六进制 "239" -> 569
 ****************************************************************************************/
long    hxtol(char *s, int j)
{
    int    i, r, t;

    for(i = 0, r = 0; i < j; i ++)
    {
        if('0' <= s[i] && s[i] <= '9')
            t = s[i] - 48;
        else if('a' <= s[i] && s[i] <= 'f')
            t = s[i] - 87;
        else if('A' <= s[i] && s[i] <= 'F')
            t = s[i] - 55;
        else return 0;

        r += t;
        r = r << 4;
    }

    return r >> 4;
}

/****************************************************************************************
	将binary code 字符串转化成long数字
 ****************************************************************************************/
long	btol(char *s, long l)
{
    uchar   ch; 
    long    i = 0, len = 0;
    
    if(l < 1 || !s)   return 0;
    
    for(i = 0; i < l; i ++)
    {   
        ch = s[i];
        len = len * 100 + ch / 16 * 10 + ch % 16;
    }
    
    return len;
}

/****************************************************************************************
	将整形数字转化成binary字符串
 ****************************************************************************************/
long	ltob(char *s, long l, long n)
{   
    long    m = 0;

    for (m = n % 100, l--; n > 0; n /= 100, m = n % 100)
    {   
        if (l < 0)  return -1;

        s[l--] = m / 10 * 16 + m % 10;
    }

    return 0;
}

/****************************************************************************************
 * 	\x00\x00\x00\x00\x09   数字 -> 9
 * 	\x00\x00\x00\x00\x19   数字 -> 25
	asc转换成数字
 ****************************************************************************************/
long    asctol(char *s, long l)
{
    long    i = 0, len = 0;
    char    *p = (char *)&len;
    
    if(l < 1 || !s)   return 0;

    for(i = 0; (i < l) && (i < sizeof(long)); i ++)
        p[l - 1 - i] = s[i];
        
    return len;
}

/****************************************************************************************
	将数字转换成asc	
	s转化结果字符串
	t转化长度
	l待转化的数字
 ****************************************************************************************/
void	ltoasc(char *s, long t, long l)
{
    int     i = 0;
    char    *p = (char *)&l;
    
    for(i = 0, t -= 1; i < sizeof(long) && (i <= t); i ++)
        s[t - i] = p[i];
}

/****************************************************************************************
	将bcd码字符串压缩成bcd码字符串保存
	示例: 字符串 "6223" 转化成 "\x62\x23"
	flag	0	右对齐
	flag	1	左对齐
 ****************************************************************************************/
char*   btop(char *s, long l, char *o, int flag)
{
    int     i, n, m = 0;

	if(!l || !o)	return o;

    n = (l % 2);
    if(0 == n)
    {
        for(i = 0; i < l; i += 2)
            o[m ++] = (s[i + 1] - 48) | (s[i] - 48) << 4;
        return o;
    }

    if(0 == flag)   //  左补
    {
        o[m ++] = (s[0] - 48);
        for(i = 1; i < l; i += 2)
            o[m ++] = (s[i + 1] - 48) | (s[i] - 48) << 4;
        return o;
    }
    else
    {
        for(i = 0; i < l - 1; i += 2)
            o[m ++] = (s[i + 1] - 48) | (s[i] - 48) << 4;
        o[m] = (s[i] - 48) << 4 | 0;
        return o;
    }
    return o;
}

/****************************************************************************************
	将压缩BCD码转换成bcd码字符串
	示例: 将"\x62\x23" 转化成字符串 "6223" 
	flag	0	右对齐
	flag	1	左对齐
 ****************************************************************************************/
char*   ptob(char *s, long j, char *o, long ol)
{
    int     i, m = 0;

    if(!s || (ol < j * 2))  return o;

    for(i = 0; i < j; i ++)
    {
        o[m ++] = g_szCTab[s[i] >> 4 & 0x0f];
        o[m ++] = g_szCTab[s[i] & 0x0f];
    }
	o[m] = 0x00;

    return o;
}

/****************************************************************************************
	将urlencode字符转码
	示例: s
 ****************************************************************************************/
int    urltoi(char *s)
{
    int    v, c;

    c = ((unsigned char *)s)[0];
    if (isupper(c))
        c = tolower(c);
    v = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

    c = ((unsigned char *)s)[1];
    if (isupper(c))
        c = tolower(c);
    v += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

    return v;
}

/****************************************************************************************
	将urlencode处理
	示例: text
 ****************************************************************************************/
long    lUrlencode(unsigned char *text, int len, unsigned char *out, int o)
{
    register unsigned char c;
    unsigned char *from, *end, *to, *start;
    
    from = (unsigned char *)text;
    end  = (unsigned char *)text + len;
    start = to = out;

    while (from < end) 
    {
        c = *from ++;

        if(c == ' ') 
            *to++ = '+';
        else if((c < '0' && c != '-' && c != '.') || (c < 'A' && c > '9') ||
            (c > 'Z' && c < 'a' && c != '_') || (c > 'z')) 
        {
            to[0] = '%';
            to[1] = g_szCTab[c >> 4];
            to[2] = g_szCTab[c & 15];
            to += 3;
        }
        else 
           *to++ = c;
    }
    *to = 0;

    return to - start;
}

/****************************************************************************************
	lUrldecode
	示例: text
 ****************************************************************************************/
long    lUrldecode(char *ucode, int len, char *out, int o)
{
    register unsigned int c;
    char *dest = out, *data = ucode;

    if(!out)    return -1;

    for(c = 0, len --; c < o && 0 <= len; c ++, len --)
    {
        if (*data == '+')
            *dest = ' ';
        else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) &&
            isxdigit((int) *(data + 2)))
        {
            *dest = (char)urltoi(data + 1);
            data += 2;
            len -= 2;
        }
        else
            *dest = *data;

        data ++;
        dest ++;
    }
    *dest = '\0';

    return c;
}

/****************************************************************************************
	code end
****************************************************************************************/

