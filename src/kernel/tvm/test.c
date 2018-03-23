#define _GNU_SOURCE
#include    "tvm.h"
#include    "tmain.h"

char*   sfieldvalue11(char *p, char *k, int id)
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
        }
    }

    if(idx + 1 < id) return szOut;
    memcpy(szOut, y, i - (long )(y - p));

    return szOut;
}

int		main(int argc, char *argv[])
{
	long	i, lRec;
	char    szTemp[4096], szSQL[2048];	
	
	memset(szTemp, 0, sizeof(szTemp));
	strcpy(szTemp, "update tbl_acct_info set acct_no='6222000055521222A' where app_id='017532995003977';\n");

	strimcrlf(szTemp);
	strcat(szTemp, ";");
	lRec = lfieldnum(szTemp, ";");
    for(i = 0; i < lRec; i ++)
    {
        memset(szSQL, 0, sizeof(szSQL));
        strncpy(szSQL, sfieldvalue11(szTemp, ";", i + 1), sizeof(szSQL));
        sltrim(szSQL);
        srtrim(szSQL);
        if(!strlen(szSQL))  continue;
	
		fprintf(stdout, "[%d]:%s\n", i, szSQL);
	}

	return RC_SUCC;
}

