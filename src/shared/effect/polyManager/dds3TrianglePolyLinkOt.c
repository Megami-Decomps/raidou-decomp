#include "shared/effect/polyManager/decls.h"

void dds3TrianglePolyLinkOt(sdfGrOt_t *pOt, dds3TrianglePoly_t *hTriPoly) {
    sdfDrawTag_t *list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdf3DPrimPolyParm_t state;
    s32 remaining;
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    remaining = hTriPoly->number * 3;
    memset(&state, 0, sizeof(state));
    state.faceCount = 0x10;
    state.pointCount = 0x30;
    state.attr = 0x4000;
    state.vertex = (sdfVector4_t *)hTriPoly->pVertex;
    state.vertexColor = hTriPoly->pVC;
    state.color = hTriPoly->abgr;
    while (remaining >= 0x30) {
        remaining -= 0x30;
        sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&state)));
    }
    if (remaining > 0) {
        dds3PolygonPrimParm.faceCount = remaining / 3;
        dds3PolygonPrimParm.pointCount = remaining;
        sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&state)));
    }
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))pOt->regFunc)(pOt, list);
}
