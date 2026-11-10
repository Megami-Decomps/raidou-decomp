#include "shared/effect/effMath/decls.h"

dds3Bezier4PPart_t *dds3Bezier4PGetPart(effBezier4P_t *pEffBezier4P, s32 index) {
    return &((dds3Bezier4PPart_t *)pEffBezier4P->pPart)[index];
}
