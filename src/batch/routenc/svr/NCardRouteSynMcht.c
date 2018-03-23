/**
 * desc: 源码负责无卡渠道商户同步
 */
#include "cust/cust.h"

int iNCardRouteSynMchtPro()
{
    IBPrace("************************** 无卡渠道商户同步 开始 ******************************");

    /* 商户同步不做了 */
    return 0;

SUCCESS:
    IBPrace("************************** 无卡渠道商户同步 结束 ******************************");
    lDbsCommit();
    return 0;
ERROR:
    lDbsRollback();
    IBPrace("************************** 无卡渠道商户同步 异常 ******************************");
    return -1;
}
