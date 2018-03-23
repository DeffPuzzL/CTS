#include    "cts.h"
#include    "tvm.h"
#include    "dbsapi.h"

static const  char    g_szCTab[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                '9', 'A', 'B', 'C', 'D', 'E', 'F'};

long	hxtol(char *s, int j)
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

****************************************************************************************/
int     main(int argc, char *argv[])
{
    size_t  i = 0, lCount = 0;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();
	char	s[20] = "239";

//char*   ptob(char *s, long j, char *o, long ol)
	fprintf(stdout, "RES:%d\n", hxtol(s, 3));


    return RC_SUCC;
}

/****************************************************************************************
    code end
****************************************************************************************/
