#include "shared/effect/effMagatuhi/decls.h"

/* Advance delayed Bezier particles, pack their fade, and submit the owner.
 * Fade-in wins when its interval overlaps fade-out; divisors are unchecked. */
void effMagaPoint4Draw(effMagaPoint4_tag *pData) {
    effArrHdr_t *slots = pData->pBezier4P;
    dds3MagatuhiBill_tag *valueWork = pData->pMagatuhi->valueWork;
    s32 *delays = pData->pPart;
    u8 respawn = pData->param.loop;
    u32 count = pData->param.reserve;
    s32 life = SDF_FRAMES(pData->param.life);
    s32 spread = pData->param.timing;
    s32 fadeIn = SDF_FRAMES(pData->param.fade_in);
    s32 fadeOut = SDF_FRAMES(pData->param.fade_out);
    sdfVector4_t out;
    f32 fade;
    s32 frame;
    u32 particleIndex;

    /* Retain the native slot lookup even though its returned address is unused. */
    dds3Bezier4PGetPart(slots, 0);
    for (particleIndex = 0; particleIndex < count; particleIndex++) {
        frame = *delays;
        if (frame == 0) {
            effMagaPoint4PartInit(pData, particleIndex);
        }
        if (frame > 0 && frame <= life) {
            dds3Bezier4PPartRun(slots, particleIndex, &out.x);
            if (fadeIn > frame) {
                fade = (f32)frame / (f32)fadeIn;
            } else if (life - frame <= fadeOut) {
                fade = (f32)(life - frame) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, particleIndex, ((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB);
            if (SDF_FRAME_DUE(frame)) effMagaCylinderSetPartPos(valueWork, particleIndex, &out);
        }
        if (frame >= life && respawn) {
            *delays = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            (*delays)++;
        }
        delays++;
    }
    effMagaCylinderDrawTrack(pData->pMagatuhi);
}
