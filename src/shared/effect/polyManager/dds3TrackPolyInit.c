#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyInit(void)
{
    memset(&dds3PolygonPrimParm, 0, sizeof(dds3PolygonPrimParm));
    dds3PolygonPrimParm.attr = 0x4000;
}
