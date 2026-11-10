#include "shared/effect/polyManager/decls.h"

void dds3SpPolySetScale(dds3SpPoly_Basic_t *hData, f32 scale) {
    dds3SpPolySetScaleTable[hData->type](hData, scale);
    dds3SpPolyReset(hData);
}
