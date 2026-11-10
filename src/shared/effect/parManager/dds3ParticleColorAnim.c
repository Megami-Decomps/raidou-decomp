#include "shared/effect/parManager/decls.h"

u32 dds3ParticleColorAnim(dds3ParticleColorAnim_t *pColAnim, u32 color_now, s32 frame) {
    s32 components[3];
    u32 alpha;
    u32 sum;
    f32 t;

    if (frame >= SDF_FRAMES(pColAnim->frame_end - pColAnim->alpha_e)) {
        if (SDF_FRAMES(pColAnim->frame_end) < frame) {
            return 0;
        }
        alpha = pColAnim->alpha_e_sub * SDF_FRAME_STEP(SDF_FRAMES(pColAnim->frame_end) - frame);
        alpha &= 0xFF000000;
    } else if (frame <= SDF_FRAMES(pColAnim->alpha_s)) {
        alpha = pColAnim->alpha_s_add * SDF_FRAME_STEP(frame);
        alpha &= 0xFF000000;
    } else {
        alpha = pColAnim->alpha << 24;
    }
    if (pColAnim->type == 1) {
        return pColAnim->color0 | alpha;
    }
    t = SDF_FRAME_STEP(frame);
    if (frame < SDF_FRAMES(pColAnim->frame_h)) {
        s32 red = pColAnim->r01 * t;
        s32 green = pColAnim->g01 * t;
        s32 blue = pColAnim->b01 * t;
        sum = pColAnim->color0;
        components[0] = red;
        components[1] = green;
        components[2] = blue;
        sum += components[0];
        sum += (u32)components[1] << 8;
        sum += (u32)components[2] << 16;
        return (sum & 0xFFFFFF) | alpha;
    } else {
        s32 red, green, blue;
        t -= (f32)pColAnim->frame_h;
        red = pColAnim->r12 * t;
        green = pColAnim->g12 * t;
        blue = pColAnim->b12 * t;
        sum = pColAnim->color1;
        components[0] = red;
        components[1] = green;
        components[2] = blue;
        sum += components[0];
        sum += (u32)components[1] << 8;
        sum += (u32)components[2] << 16;
        return (sum & 0xFFFFFF) | alpha;
    }
}
