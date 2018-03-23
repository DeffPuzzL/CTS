#include	"face.h"
#include	"dbsapi.h"

/**************************************************************************************************
    函 数 名：功能描述
    功能描述：
    返回说明：
 **************************************************************************************************/
void	vLoadDbsTvm(TABLE t)
{
	TblCreat *pstCreate = NULL;
	SATvm    *pstSavm = (SATvm *)pGetSATvm();

	if(NULL == (pstCreate = pGetTblCreate(t)))
	{
		fprintf(stderr, "表(%u)装载未定义\n", t);
		return ;
	}

    if(RC_SUCC != lDbsConnect(pstCreate->lDbsFrom))
    {
        fprintf(stderr, "连接数据库失败, %s", sGetError());
		return ;
    }

    if(RC_SUCC != pstCreate->pfCreate(pstSavm, NULL))
    {
        vDbsDisconnect();
        fprintf(stderr, "初始化表(0X%X)失败, %s", pstCreate->table, sGetError());
		return ;
    }

    vDbsDisconnect();
    fprintf(stdout, "初始化表(0X%X): %s\n", pstCreate->table, sGetError());
	return ;
}

/**************************************************************************************************
    函 数 名：main
    功能描述：dbstvm操作功能说明
    返回说明：
        无
 **************************************************************************************************/
int     main(int argc, char *argv[])
{
    int     iChoose = 0;

    while(-1 != (iChoose = getopt(argc, argv, "t:v::?::")))
    {
        switch(iChoose)
        {
        case    't':
			vLoadDbsTvm(atol(optarg));
			return ;
        case    'v':
        case    '?':
        default:
    		fprintf(stdout, "\nUsage:\t%s -t talbe\n\n", basename(argv[0]));
            return RC_FAIL;
        }
    }

    fprintf(stdout, "\nUsage:\t%s -t talbe\n\n", basename(argv[0]));

    return RC_SUCC;
}
