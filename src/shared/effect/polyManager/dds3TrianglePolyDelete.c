#include "shared/effect/polyManager/decls.h"

void dds3TrianglePolyDelete(dds3TrianglePoly_t *hTriPoly) {
    sdfMatFreeDelayed(hTriPoly->pMat);
    sdfMemFreeDirect(hTriPoly->hMem);
}
