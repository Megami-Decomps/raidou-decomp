#include "shared/effect/effPCPMisc/decls.h"

effPCPRingPolyExp_tag *effPCPRingPolyExpCreate(effPCPRingPolyExpParam_t *pParam)
{
    effPCPRingPolyExp_tag *work = sdfChipAllocDirect(sizeof(effPCPRingPolyExp_tag));
    u32 segments;
    dds3RingPolyExpList_tag *node;
    u32 *entry;
    s32 groups;
    u32 i;

    work->param = *pParam;
    work->abgr = 0x80808080;
    work->frame = 0;
    segments = pParam->split;
    if (segments < 3) {
        pParam->split = 3;
        segments = 3;
    }
    node = dds3RingPolyExpListCreate(segments);
    i = 0;
    work->hPoly = node;
    work->point_max = node->vertexCount;
    groups = (s32)node->vertexCount >> 2;
    entry = node->pVC;
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = pParam->color_e_in;
        second = pParam->color_c;
        entry[1] = entry[2] = second;
        entry[3] = pParam->color_e_out;
        entry += 4;
    }
    effPCPRingPolyExpInit(work);
    work->hPoly->blend = pParam->blend;
    return work;
}
