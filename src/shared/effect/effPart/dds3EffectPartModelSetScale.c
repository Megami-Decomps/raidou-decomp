#include "shared/effect/effPart/decls.h"

void dds3EffectPartModelSetScale(void *hData, f32 scale)
{
    sdfVector4_t v;

    v.x = v.y = v.z = scale;
    SDF_VU0_LOADVECTOR_MEMOPS(v);
    mdlSetScale(hData);
}
