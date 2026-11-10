#include "shared/effect/polyManager/decls.h"

dds3TrianglePoly_t *dds3TrianglePolyCreate(s32 number) {
    s32 points = number * 3;
    s32 colorBytes = points * 4;
    sdfMemHandle_t *allocation = sdfMemAllocDirect((colorBytes + points) * 4 + 0x18);
    s32 base = sdfMemLock(allocation);
    u8 *vertices = EE_ADDR_AS(u8 *, base) + points * 16;
    dds3TrianglePoly_t *block = (dds3TrianglePoly_t *)(vertices + colorBytes);
    block->abgr = 0x80808080;
    block->number = number;
    block->pVC = (u32 *)vertices;
    block->hMem = allocation;
    block->pVertex = EE_ADDR_AS(u128 *, base);
    block->pMat = sdfMatNew();
    sdfMatSetConstant(block->pMat, 1.0f);
    return block;
}
