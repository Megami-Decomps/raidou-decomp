#include "shared/effect/effPCPFlash/decls.h"

dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 face) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u32 *block;
    s32 vertexCount;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    vertexCount = face * EFF_TRIANGLE_VERTEX_COUNT;
    positionWordCount = vertexCount * 4;
    colorWordCount = vertexCount;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = EE_ADDR_AS(u32 *, sdfMemLock(handle));
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + (positionWordCount + colorWordCount));
    pool->pVertex = (u8 *)block;
    pool->pVC = (u8 *)(block + positionWordCount);
    pool->abgr = EFF_NEUTRAL_COLOR;
    pool->blend = 2;
    pool->point = colorWordCount;
    pool->hMem = handle;
    pool->scale = 1.0f;
    pool->pMat = sdfMatNew();
    sdfMatSetConstant(pool->pMat, 1.0f);
    memset(effPCPFlashPolyParams, 0, EFF_PACKET_PARAMS_BYTES);
    effPCPFlashPolyParams->primitive = 0x4000;
    return pool;
}
