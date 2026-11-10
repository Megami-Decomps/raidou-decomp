#include "shared/effect/effPart/decls.h"

void dds3EffectPartModelInit(mdlObj_t *hModel)
{
    SDF_VU0_LOADVECTOR(&effPartModelPosInit);
    mdlSetPosition(hModel);
    SDF_VU0_LOADVECTOR(&effPartModelRotInit);
    mdlSetRotate(hModel);
    SDF_VU0_LOADVECTOR(&effPartModelScaleInit);
    mdlSetScale(hModel);
    mdlSetColor(hModel, 0x80808080);
    if (hModel->first != NULL) {
        mdlLoopMotion(hModel, 0, 0);
        hModel->first->speed = 1.0f;
    }
    hModel->flags &= ~1;
}
