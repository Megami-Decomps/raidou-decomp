#include "shared/effect/effManager/decls.h"

s32 dds3EffectManagerRun(dds3ProcessID_t *ProcessID)
{
    dds3PolygonLinkOt();
    dds3TrackPolyLinkOt();
    dds3BillboardLinkOt();
    dds3MultiBillboardLinkOt();
    dds3CrossfadeRun();
    Eve_EffectDraw();
    return 0;
}
