#include    "tvm.h"
#include    "shmdb.h"

#define ZCHAR(x)    (x == 0x00)?(x = ' '):x
/*************************************************************************************************
 * debug
 *************************************************************************************************/
int        main(int argc, char *argv[])
{
    long    i = 0, lOut = 0;
    SATvm    *pstSavm = (SATvm *)pGetSATvm();
    
    fprintf(stderr, "=============================show detail===============================\n");

    if(RC_SUCC != lRenameTable(40, 4))
    {
        fprintf(stderr, "rename table err:(%d)(%s)\n", lGetTErrno(), 
            sGetTError(lGetTErrno()));
        return RC_FAIL;
    }
           
    return RC_SUCC;
}

