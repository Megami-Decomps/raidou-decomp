#include "shared/effect/effPCPFlash/decls.h"

dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 face) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u8 *block;
    s32 vertexCount;
    s32 recordBytes;
    s32 size;
    s32 quads;

    vertexCount = face * 5;
    recordBytes = vertexCount * 5 * 4;
    quads = recordBytes >> 4;
    recordBytes = ((recordBytes & 0xF) != 0 ? quads + 1 : quads) << 4;
    size = recordBytes + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = EE_ADDR_AS(u8 *, sdfMemLock(handle));
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + recordBytes);
    pool->pVertex = (u8 *)block;
    pool->pVC = (u8 *)(block + vertexCount * 16);
    pool->abgr = EFF_NEUTRAL_COLOR;
    pool->blend = 2;
    pool->point = vertexCount;
    pool->hMem = handle;
    pool->scale = 1.0f;
    pool->pMat = sdfMatNew();
    sdfMatSetConstant(pool->pMat, 1.0f);
    memset(effPCPFlashPolyParams, 0, EFF_PACKET_PARAMS_BYTES);
    effPCPFlashPolyParams->primitive = 0x4000;
    return pool;
}
