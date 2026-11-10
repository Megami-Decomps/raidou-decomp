#include "shared/effect/effPCPBezier/decls.h"

effPCPBezierBill_tag *effPCPBezierBillCopy(effPCPBezierBill_tag *src) {
    u32 count = src->param.number;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effPCPBezierBillPart_tag) + sizeof(effPCPBezierBill_tag));
    effPCPBezierBill_tag *work = (effPCPBezierBill_tag *)sdfMemLock(allocation);
    effPCPBezierBillPart_tag *slot = (effPCPBezierBillPart_tag *)(work + 1);
    effBezierPlacement_t place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->param = src->param;
    work->original = 0;
    work->hMem = allocation;
    work->hBED = src->hBED;
    work->pPart = slot;
    work->abgr = 0x80808080;
    historyLength = work->param.historyLength;
    if (historyLength == 0) {
        work->param.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->param.subdivisions;
    if (subdivisions <= 0) {
        work->param.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->param.startDelayRange <= 0) {
        work->param.startDelayRange = 1;
    }
    palette[0] = work->param.palette[0];
    palette[1] = work->param.palette[1];
    palette[2] = work->param.palette[2];
    palette[3] = work->param.palette[3];
    place.payload[0] = 0;
    place.payload[1] = 0;
    place.payload[2] = 0;
    place.payload[3] = 0;
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->hBztp = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->hBztp, palette);
        slot->frame = 0;
        slot->alpha = 1.0f;
        slot->Bezier7P.pointIndex = 0;
        slot->Bezier7P.t = 0;
        slot->Bezier7P.parameterStep = 0;
        slot->hBEDCut = effEveBEDCutCreate(work->hBED, 2, &place);
    }
    return work;
}
