#include "shared/effect/effPCPBezier/decls.h"

dds3BezierTrackPoly_tag *dds3BezierTrackPolySetVertex(s32 hPoly, s32 pVertex) {
    s32 count = hPoly * pVertex * 3 + 6;
    u32 colorBytes = count * sizeof(u32);
    u32 bufferBytes = count * (sizeof(u128) + sizeof(u32)) + 8 * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *allocation = sdfMemAllocDirect(bufferBytes + sizeof(dds3BezierTrackPoly_tag));
    u8 *storage = (u8 *)sdfMemLock(allocation);
    dds3BezierTrackPoly_tag *history = (dds3BezierTrackPoly_tag *)(storage + bufferBytes);
    sdfMat_t *asset;

    history->pVertex = (u128 *)storage;
    storage += count * sizeof(u128);
    history->pHeadVertex = (u128 *)storage;
    storage += 8 * sizeof(u128);
    history->pVC = (u32 *)storage;
    storage += colorBytes;
    history->pHeadVC = (u32 *)storage;
    history->blend = 2;
    history->point = count;
    history->current = 3;
    history->split = pVertex;
    history->abgr = 0x80808080;
    history->hMem = allocation;
    history->count = 0;
    memcpy(history->pHeadVertex, dds3BezierTrackPolyInitVertex, sizeof(dds3BezierTrackPolyInitVertex));
    asset = sdfMatNew();
    history->pMat = asset;
    sdfMatSetConstant(asset, 1.0f);
    memset(&dds3BezierVertexCount, 0, 0x2C);
    dds3BezierVertexCount.flags = 0x4000;
    dds3BezierVertexCount.primitiveIndices = dds3BezierVertexCountIndices;
    memset(&dds3BezierTrackPolyVertex, 0, 0x2C);
    dds3BezierTrackPolyVertex.flags = 0x4000;
    dds3BezierTrackPolyVertex.primitiveIndices = dds3BezierTrackPolyVertexIndices;
    dds3BezierTrackPolyVertex.primitiveCount = 6;
    dds3BezierTrackPolyVertex.pointCount = 8;
    return history;
}
