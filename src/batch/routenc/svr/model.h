#ifndef __MODEL_H__
#define __MODEL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "cts.h"
#include "face.h"
#include "DbsApi.h"
#include "dbsdef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG(fmt, args...) fprintf(stdout, "[%s:%d]" fmt "\n" , __FUNCTION__, __LINE__, ##args);

typedef enum {
    LMER_CON = 0, /* 本地商户条件 */
    CHA_CON,      /* 通道条件 */
    CMER_CON,     /* 通道商户条件 */
    CON_COUNT
} CON_TYPE;

extern int model_init();
extern int model_judge(CON_TYPE con_type, int model_id);
extern void model_sort();
extern void model_print();
extern void model_destroy();

#ifdef __cplusplus
}
#endif

#endif
