#include "shared/effect/effPCPScatter/decls.h"

dds3WhirlwindPolyList_tag *dds3WhirlwindPolyListCreate(u32 number, u32 face) {
    s32 vectors = face * 2 + 2;
    s32 totalVectors = vectors * number;
    s32 bytes = (totalVectors * 6 + vectors + number) * 4;
    dds3WhirlwindPolyList_tag *object;
    struct sdfMemHandle_t *allocation;
    u8 *cursor;
    u32 quarter;
    u32 rampIndex;
    u32 *left;
    u32 *right;
    f32 alpha;
    f32 step;
    u32 color;

    object = sdfChipAllocDirect(sizeof(dds3WhirlwindPolyList_tag));
    allocation = sdfMemAllocDirect(bytes);
    cursor = EE_ADDR_AS(u8 *, sdfMemLock(allocation));
    memset(cursor, 0, bytes);
    object->points = (f32 *)cursor;
    cursor += totalVectors * 16;
    object->uv = (f32 *)cursor;
    cursor += totalVectors * 8;
    object->vertexColors = (u32 *)cursor;
    cursor += vectors * 4;
    object->colors = (u32 *)cursor;
    object->color = EFF_SCATTER_NEUTRAL_COLOR;
    object->vectorsPerParticle = vectors;
    object->packetQueueIndex = 1;
    object->particleCount = number;
    object->allocation = allocation;
    object->scale = 1.0f;
    object->sharedResource = NULL;
    object->asset = sdfMatNew();
    sdfMatSetConstant(object->asset, 1.0f);
    memset(object->vertexColors, 0x80, vectors * 4);
    quarter = vectors >> 2;
    if (quarter >= 2) {
        left = object->vertexColors;
        right = left + vectors - 2;
        alpha = 0.0f;
        step = 1.0f / quarter;
        for (rampIndex = 0; rampIndex < quarter; rampIndex++) {
            color = ((u32)(alpha * 128.0f) << 24) | 0x808080;
            alpha += step;
            left[0] = color;
            left[1] = left[0];
            right[0] = left[0];
            right[1] = left[0];
            left += 2;
            right -= 2;
        }
    }
    memset(&effDebugPoly, 0, sizeof(effDebugPolyParams_t));
    effDebugPoly.primitive = 0x4000;
    return object;
}
