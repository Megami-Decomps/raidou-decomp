#include "shared/effect/effPCPScatter/decls.h"

void dds3WhirlwindPolyListDraw(dds3WhirlwindPolyList_tag *object) {
    f32 matrix[16] __attribute__((aligned(16)));
    sdfDrawTag_t *packet;
    effDebugPolyParams_t *draw;
    s32 count;
    s32 remaining;
    s32 particleIndex;
    sdfGrOt_t *surface;

    packet = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(packet);
    SDF_VU0_STOREIDENTITY(matrix);
    matrix[10] = matrix[5] = matrix[0] = object->scale;
    object->matrix[12] = object->origin[0];
    object->matrix[13] = object->origin[1];
    object->matrix[14] = object->origin[2];
    SDF_VU0_LOADMATRIX(object->matrix);
    SDF_VU0_LOADMATRIX_B(matrix);
    MultMatrixNoArg();
    sdf3DPrimLinkTm(packet, 0);
    if (object->sharedResource != NULL) {
        sdfMatSetTex((struct sdfMat_t *)object->asset,
                      object->sharedResource->textureHandle);
    }
    sdf3DPrimLinkMat(packet, (sdfMat_t *)object->asset, 0);
    count = (s32)object->particleCount;
    effDebugPoly.parameters = (const u32 *)D_0031F3E0;
    for (particleIndex = 0; particleIndex < count; particleIndex++) {
        draw = &effDebugPoly;
        remaining = object->vectorsPerParticle;
        draw->positions = (u128 *)dds3WhirlwindPolyListGetVertex(object, particleIndex);
        draw->texcoords = dds3WhirlwindPolyListGetUV(object, particleIndex);
        draw->colors = object->vertexColors;
        draw->color = dds3WhirlwindPolyListGetABGR(object, particleIndex);
        draw->parameterCount = 16;
        draw->vertexCount = 18;
        if ((draw->color & 0xFF000000) != 0) {
            while (remaining >= 18) {
                remaining -= 16;
                sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(&effDebugPoly)));
                effDebugPoly.positions += 16;
                effDebugPoly.colors += 16;
                effDebugPoly.texcoords += 32;
            }
            if (remaining >= 4) {
                draw->parameterCount = remaining - 2;
                draw->vertexCount = remaining;
                sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(draw)));
            }
        }
    }
    surface = effPCPScatterSurfaceByQueue[object->packetQueueIndex];
    surface->regFunc(surface, packet);
}
