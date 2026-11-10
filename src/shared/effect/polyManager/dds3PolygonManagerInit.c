#include "shared/effect/polyManager/decls.h"

void dds3PolygonManagerInit(void)
{
    hDrawListTop = 0;
    dds3TrackPolyInit();
    sdfRandomizeByTime(dds3PolygonRandState);
}
