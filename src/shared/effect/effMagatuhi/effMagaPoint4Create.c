#include "shared/effect/effMagatuhi/decls.h"

/* Clone history parameters; signed delays follow the returned work block. */
effMagaPoint4_tag *effMagaPoint4Create(effMagaPoint4Param_t *src) {
    u32 count = src->reserve;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * EFF_MAGATUHI_DELAY_WORD_BYTES + sizeof(effMagaPoint4_tag));
    effMagaPoint4_tag *work = EE_ADDR_AS(effMagaPoint4_tag *, sdfMemLock(allocation));
    s32 *delays = (s32 *)(work + 1);
    s32 spread;
    u32 i;

    work->param = *src;
    work->hMem = allocation;
    work->pPart = delays;
    if (work->param.timing <= 0) {
        work->param.timing = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->param.reserve);
    work->pBezier4P = dds3Bezier4PCreate(count);
    spread = work->param.timing;
    for (i = 0; i < count; i++) {
        *delays++ = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
    }
    return work;
}
