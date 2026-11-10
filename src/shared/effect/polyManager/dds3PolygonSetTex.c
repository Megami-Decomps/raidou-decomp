#include "shared/effect/polyManager/decls.h"

void dds3PolygonSetTex(dds3Polygon_t *hPolygon, sdfTexHandle_t *hTex) {
    sdfMatSetTex(hPolygon->pMat, hTex);
}
