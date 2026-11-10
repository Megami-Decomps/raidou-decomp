#ifndef DDS_EFF_LOOK_AT_RING_H

#define DDS_EFF_LOOK_AT_RING_H
#include "eff.h"

/* Kind-specific extension used by the paired look-at ring emitter clients. */
typedef struct EffLookAtRingEmitter {
    EffEmitterHead head; /* shared emitter prefix, 0x150 bytes */
    u8 mode;             /* 0x150 */
    u8 loop;             /* 0x151 */
    u8 opaque152[2];
    s32 decayStep;        /* 0x154: frame delay between packet groups */
    u8 opaque158[4];
    u32 period;          /* 0x15C */
    f32 f160;            /* 0x160; meaning remains unresolved */
    f32 f164;            /* 0x164; meaning remains unresolved */
    u8 opaque168[8];      /* Complete 0x170-byte template allocation. */
} EffLookAtRingEmitter;

typedef char EffLookAtRingSizeCheck[sizeof(EffLookAtRingEmitter) == 0x170 ? 1 : -1];

#endif
