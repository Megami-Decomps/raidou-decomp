#include "shared/effect/effMagatuhi/decls.h"

/* Clone the first-family parameters; return the work after its state array.
 * Clamp only the copied delay modulus, leaving the caller's parameters intact. */
effMagaSmokeConv_tag *effMagaSmokeConvCreate(effMagaSmokeConvParam_t *src) {
    u32 count = src->reserve;
    u32 size = count * sizeof(effMagaSmokePart_tag);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size + sizeof(effMagaSmokeConv_tag));
    effMagaSmokePart_tag *particle = EE_ADDR_AS(effMagaSmokePart_tag *, sdfMemLock(allocation));
    effMagaSmokeConv_tag *work = (effMagaSmokeConv_tag *)((u8 *)particle + size);
    s32 spread;
    u32 particleIndex;

    work->head = *src;
    work->hMem = allocation;
    work->pPart = particle;
    if (work->head.timing <= 0) {
        work->head.timing = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->head.reserve);
    work->pBezier4P = dds3Bezier4PCreate(count);
    spread = work->head.timing;
    for (particleIndex = 0; particleIndex < count; particleIndex++) {
        particle->age = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}
