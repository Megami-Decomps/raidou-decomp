#include "shared/effect/effPCPMisc/decls.h"

effPCPRingPolyExp2_tag *effPCPRingPolyExp2Create(effPCPRingPolyExp2Param_t *hData)
{
    effPCPRingPolyExp2_tag *work = sdfChipAllocDirect(sizeof(effPCPRingPolyExp2_tag));
    u32 segments;
    dds3RingPolyExpList_tag *node;
    u32 *entry;
    s32 groups;
    u32 i;

    work->head = *hData;
    work->abgr = 0x80808080;
    work->frame = 0;
    segments = hData->split;
    if (segments < 3) {
        hData->split = 3;
        segments = 3;
    }
    node = dds3RingPolyExpListCreate(segments);
    i = 0;
    work->hPoly = node;
    groups = (s32)node->vertexCount >> 2;
    entry = node->pVC;
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = hData->firstColor;
        second = hData->middleColor;
        entry[1] = entry[2] = second;
        entry[3] = hData->lastColor;
        entry += 4;
    }
    effPCPRingPolyExp2Init(work);
    work->hPoly->blend = hData->drawKind;
    return work;
}
