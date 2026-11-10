#include "shared/effect/parManager/decls.h"

void dds3ParticleSetColorTable(dds3ParticleColorAnim_t *pColAnim, s32 frame) {
    u32 alpha;
    f32 span;
    u32 a, b;

    pColAnim->color0 &= 0xFFFFFF;
    pColAnim->color1 &= 0xFFFFFF;
    pColAnim->color2 &= 0xFFFFFF;
    alpha = pColAnim->alpha << 24;
    pColAnim->alpha_s_add = alpha;
    if (pColAnim->alpha_s > 0) {
        pColAnim->alpha_s_add = alpha / pColAnim->alpha_s;
    }
    pColAnim->alpha_e_sub = alpha;
    if (pColAnim->alpha_e > 0) {
        pColAnim->alpha_e_sub = alpha / pColAnim->alpha_e;
    }
    pColAnim->frame_end = frame;
    if (pColAnim->type == 1) {
        return;
    }
    if (pColAnim->type == 2) {
        pColAnim->frame_h = frame;
    } else {
        pColAnim->frame_h = frame >> 1;
    }
    span = pColAnim->frame_h;
    if (span == 0) {
        span = 1.0f;
    }
    a = pColAnim->color1;
    b = pColAnim->color0;
    pColAnim->r01 = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    pColAnim->g01 = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    pColAnim->b01 = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
    if (pColAnim->type == 2) {
        return;
    }
    a = pColAnim->color2;
    b = pColAnim->color1;
    pColAnim->r12 = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    pColAnim->g12 = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    pColAnim->b12 = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
}
