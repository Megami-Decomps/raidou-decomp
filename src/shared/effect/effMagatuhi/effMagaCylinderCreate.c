#include "shared/effect/effMagatuhi/decls.h"

/* Clone ring parameters; the returned work precedes its individual slots. */
effMagaCylinder_tag *effMagaCylinderCreate(effMagaCylinderParam_t *src) {
    u32 count = src->particleCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effMagaCylinderPart_tag) + sizeof(effMagaCylinder_tag));
    effMagaCylinder_tag *work = EE_ADDR_AS(effMagaCylinder_tag *, sdfMemLock(allocation));
    effMagaCylinderPart_tag *particle = (effMagaCylinderPart_tag *)(work + 1);
    s32 spread;
    u32 i;

    work->param = *src;
    work->pPart = particle;
    work->abgr = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->hMem = allocation;
    SDF_VU0_STOREIDENTITY(work->matrix);
    if (work->param.timing <= 0) {
        work->param.timing = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->param.particleCount);
    spread = work->param.timing;
    for (i = 0; i < count; i++) {
        particle->frame = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}
