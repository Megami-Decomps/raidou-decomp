#include "shared/effect/effMagatuhi/decls.h"

/* Clone drift parameters; return the work after its 0x30-byte state array. */
effMagaSmoke_tag *effMagaSmokeCreate(effMagaSmokeParam_t *src) {
    u32 count = src->particleCount;
    u32 size = count * sizeof(effMagaSmokePart_tag);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size + sizeof(effMagaSmoke_tag));
    effMagaSmokePart_tag *particle = EE_ADDR_AS(effMagaSmokePart_tag *, sdfMemLock(allocation));
    effMagaSmoke_tag *work = (effMagaSmoke_tag *)((u8 *)particle + size);
    s32 spread;
    u32 i;

    work->param = *src;
    work->pPart = particle;
    work->abgr = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->hMem = allocation;
    SDF_VU0_STOREIDENTITY(work->matrix);
    if (work->param.delaySpread <= 0) {
        work->param.delaySpread = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->param.particleCount);
    spread = work->param.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}
