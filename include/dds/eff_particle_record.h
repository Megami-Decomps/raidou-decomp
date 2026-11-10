#ifndef DDS_EFF_PARTICLE_RECORD_H

#define DDS_EFF_PARTICLE_RECORD_H
#include "common.h"

/* Particle-specific view of one 0x40-byte EffectBufferRecord slot. */
typedef struct EffParticleRecord {
    f32 x;
    f32 y;
    f32 z;
    u8 opaque0C[0x14];
    s32 frame;
    u32 color;
    f32 speed;
    f32 angle;
    u8 opaque30[0x10];
} EffParticleRecord;

typedef char EffParticleRecordSizeAssert[
    (sizeof(EffParticleRecord) == 0x40) ? 1 : -1];

#endif
