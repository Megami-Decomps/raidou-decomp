#include "shared/effect/billManager/decls.h"

void dds3P2APolygonGetResource(dds3P2APolygon_tag *hP2APoly, dds3P2APolygonResource_t *pP2APolyRes) {
    billHandle_t *billboard = hP2APoly->hBillboard;

    if (billboard->type == 1) {
        u32 modeFlags = billboard->modeFlags;

        if (modeFlags & 0x40) {
            pP2APolyRes->mode = 2;
            pP2APolyRes->secondary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else if (modeFlags & 0x80) {
            pP2APolyRes->mode = 3;
            pP2APolyRes->secondary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else {
            pP2APolyRes->mode = 0;
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
        }
    }
}
