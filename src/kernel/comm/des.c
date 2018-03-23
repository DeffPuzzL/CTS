#include	"Ibp.h"

/**********************************************************************************
 *  *  Function:  DES_Encrypt		将英文字符串改成大写
 *  *  Description:					改变输入字符串的值
 **********************************************************************************/
long	DES_Encrypt(const char *pszInp, const long lInpt, const char *pszKey,
			char **ppszOut, long *plOut, int enc)
{
	const_DES_cblock	yCBlock, yIvec;
	DES_key_schedule	ys;
	long				lLen = lInpt % 8 ? (lInpt / 8 + 1) * 8 : lInpt;
	uchar				*p = NULL;

	memset(yCBlock, 0, 8);

	if(strlen(pszKey) <= 8) memcpy(yCBlock, pszKey, strlen(pszKey));
	else					memcpy(yCBlock, pszKey, 8);

	DES_set_key_unchecked(&yCBlock, &ys);

	memset(yIvec, 0, 8);	//  一般为 初始 0

	*plOut = lLen;
	if(NULL == (p = (uchar *)malloc(*plOut)))
		return RC_FAIL;

	memset(p, 0, *plOut);
	DES_ncbc_encrypt((uchar *)pszInp, (uchar *)p, lInpt, &ys, &yIvec, enc);
	*ppszOut = p;

	return RC_SUCC;
}

/****************************************************************************************
	code end
****************************************************************************************/
