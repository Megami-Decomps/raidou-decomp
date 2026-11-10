#include "shared/sdf/sdfModel/decls.h"

void sdfModelDrawEnvLinkOt(sdfGrOt_t **aOt, sdfModelDrawEnv_t *aEnv)
{
    s32 i;

    for (i = 0; i != 4; i++) {
        ((void (*)(sdfGrOt_t *, s32, sdfDrawTag_t *))aOt[i]->callbackFunc)(aOt[i], 1,
                                                                          &aEnv->set[i].drawTag);
    }
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))aOt[3]->regFunc)(aOt[3], &aEnv->term.drawTag);
}
