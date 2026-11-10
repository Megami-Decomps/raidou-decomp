#ifndef DDS_EFF_PARTICLE_VIEW_H

#define DDS_EFF_PARTICLE_VIEW_H
#include "eff.h"

/* Single-particle callbacks interpret selected fields in the shared emitter
 * owner differently; color/alpha/lastSpeed remain aliases inside its fade span. */
typedef struct EffParticleOverlay {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[4];
    f32 speed;
    u8 pad14[0x10];
    u32 unk24;
    u8 pad28[0x24];
    u32 color;
    u8 pad50[8];
    u32 alpha;
    u8 pad5C[0x30];
    f32 lastSpeed;
    u8 pad90[4];
    f32 speedJitter;
    f32 angleJitter;
    u8 pad9C[0x54];
    u32 colorMask;
    u8 padF4[4];
    EffectBufferTail *buffer;
} EffParticleOverlay;

/* The single-particle clone is a 0x150-byte emitter owner; callbacks use
 * the specialized 0xFC prefix and leave the owner's remaining bytes opaque. */
typedef union EffParticleView {
    EffEmitterHead emitter;
    EffParticleOverlay particle;
} EffParticleView;

typedef char EffParticleOverlay_size_check[
    sizeof(EffParticleOverlay) == 0xFC ? 1 : -1];

typedef char EffParticleView_size_check[
    sizeof(EffParticleView) == 0x150 ? 1 : -1];

#endif
