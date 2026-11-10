#include "shared/effect/effPCPMisc/decls.h"

void effPCPBufuIceDraw(effPCPBufuIce_tag *hData) {
    effPartHandle_t *handle = hData->handle0;
    u128 mtx[4];

    dds3EffectPartSetPosition(handle, hData);
    dds3EffectPartSetScale(handle, hData->scale);
    dds3EffectPartSetColor(handle, hData->color);
    SetRotateMatrixY(hData->angle);
    SDF_VU0_LOADMATRIX_B(hData->matrix);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX(mtx);
    dds3EffectPartSetRotateMatrix(handle, mtx);
    dds3EffectPartDraw(handle);
    hData->frame++;
}
