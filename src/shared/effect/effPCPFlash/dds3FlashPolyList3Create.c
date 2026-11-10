#include "shared/effect/effPCPFlash/decls.h"

dds3FlashPolyList_t *dds3FlashPolyList3Create(s32 face) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u32 *block;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    positionWordCount = face * 16;
    colorWordCount = face * EFF_QUAD_VERTEX_COUNT;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = EE_ADDR_AS(u32 *, sdfMemLock(handle));
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + (positionWordCount + colorWordCount));
    pool->pVertex = (u8 *)block;
    pool->blend = 2;
    pool->pVC = (u8 *)(block + positionWordCount);
    pool->point = colorWordCount;
    pool->hMem = handle;
    pool->scale = 1.0f;
    pool->abgr = EFF_NEUTRAL_COLOR;
    pool->pMat = sdfMatNew();
    sdfMatSetConstant(pool->pMat, 1.0f);
    memset(effPCPFlashPolyParams, 0, EFF_PACKET_PARAMS_BYTES);
    effPCPFlashPolyParams->primitive = 0x4000;
    return pool;
}
