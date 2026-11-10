#include "shared/effect/polyManager/decls.h"

void dds3PolygonDelete(dds3Polygon_t *hPolygon) {
    hPolygon->flags = 1;
    sdfMatFreeDelayed(hPolygon->pMat);
    sdfMemFreeDirect(hPolygon->hMem);
}
