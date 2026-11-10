#ifndef DDS_EFF_CHANNEL_LAYOUT_H

#define DDS_EFF_CHANNEL_LAYOUT_H
#include "eff_channel.h"

#include "eff_param.h"

struct sdfMemHandle_t;

/* effCopyVertRows writes XYZ at a four-float stride, leaves +0x40..+0x57
 * untouched, then copies four scalars at +0x58. */
typedef struct EffFloatRows {
    f32 primary[16];
    u8 opaque40[0x18];
    f32 secondary[4];
} EffFloatRows;

/* Allocated interpolation cursor; effCreateChannel requests exactly 0x18 bytes. */
typedef struct EffChan {
    struct sdfMemHandle_t *allocation;
    u32 recordCount;
    EffFloatRows *rows;
    u32 cursorIndex;
    f32 cursorPosition;
    f32 cursorStep;
} EffChan;

/* XYZ interpolation result; the cubic channel sampler also writes W. */
typedef struct EffVert {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} EffVert;

/* Emitter view: the first eight bytes are not used by this consumer. */
typedef struct EffEmit {
    u8 opaque00[8];
    EffChanWork *primitive;
} EffEmit;

/* Parameter template owner; the four-byte prefix is not read by channel work. */
typedef struct EffChanSourceOwner {
    u8 opaque00[4];
    effPartHandle_t *param;
} EffChanSourceOwner;

/* Source header followed by the owner used to duplicate each record parameter. */
typedef struct EffChanSource {
    EffChanHead head;
    EffChanSourceOwner *owner;
} EffChanSource;

typedef char EffChan_size_must_be_0x18[(sizeof(EffChan) == 0x18) ? 1 : -1];

typedef char EffVert_size_must_be_0x10[(sizeof(EffVert) == 0x10) ? 1 : -1];

#endif
