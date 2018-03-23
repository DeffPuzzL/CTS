#include	"face.h"


/****************************************************************************************

****************************************************************************************/
int     main(int argc, char *argv[])
{
	long	i, j, n, m;
	SATvm   *pstSavm = (SATvm *)pGetSATvm();
//	char	*p = "strcmp(strcat(strcpy($$id, $$name), $$string),\"000  000\");__RUN__;strcat($$idx,\"testse\")";
//	char	*p = "strcmp(strcat(strcpy($$id, $$name), strcat($$string, $$name)),\"000  000\");"
//				"strcmp(strcat(strcpy($$id, $$name), $$string),\"000  000\");__RUN__;strcat($$idx,\"testse\")";
	char	szExpress[256], szTest[300] = "   strcat(   strcat(strcpy(if($$id=$22,0,1), $$name), strcat($$string, $$name    )),\"000  000\");";

	lInitRunTest(basename(argv[0]));

	if(RC_FAIL == lCall(pstSavm, pGetCmie(), "100000"))
	{
		fprintf(stderr, "发起新交易(10000)失败, %s", sGetTError(pstSavm->m_lErrno));
		return RC_FAIL;
	}

	IBPresetpool();

    return RC_SUCC;
}

/****************************************************************************************
	code end
****************************************************************************************/
