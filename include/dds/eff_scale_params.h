#ifndef DDS_EFF_SCALE_PARAMS_H

#define DDS_EFF_SCALE_PARAMS_H
#include "common.h"

typedef struct ScaleEntry {
    f32 unk0;
    f32 value;
} ScaleEntry;

/* Access view through +0xF4; intervening storage and any allocation tail stay opaque. */
typedef struct ScaleSet {
    u8 pad0[0x64];
    f32 unk64;
    f32 unk68;
    u8 pad6C[4];
    ScaleEntry entries[3];
    u8 pad88[0x40];
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    f32 unkD8;
    f32 unkDC;
    f32 unkE0;
    f32 unkE4;
    u8 padE8[8];
    f32 unkF0;
} ScaleSet;

typedef struct ScaleOwner {
    u8 pad0[0x20];
    ScaleSet *dst;
    ScaleSet *src;
} ScaleOwner;

typedef char ScaleEntry_size_must_be_0x08[(sizeof(ScaleEntry) == 0x08) ? 1 : -1];

#endif /* DDS_EFF_SCALE_PARAMS_H */
