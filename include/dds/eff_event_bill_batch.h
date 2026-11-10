#ifndef DDS_EFF_EVENT_BILL_BATCH_H

#define DDS_EFF_EVENT_BILL_BATCH_H
#include "common.h"

#define EFF_EVENT_BILL_BATCH_PARTICLE_COUNT 15

typedef struct EffEventBillBatch {
    u128 positions[EFF_EVENT_BILL_BATCH_PARTICLE_COUNT]; /* 0x000 */
    u32 colors[EFF_EVENT_BILL_BATCH_PARTICLE_COUNT];     /* 0x0F0 */
    s16 uv[EFF_EVENT_BILL_BATCH_PARTICLE_COUNT][8];      /* 0x12C */
    f32 corners[EFF_EVENT_BILL_BATCH_PARTICLE_COUNT][8]; /* 0x21C */
    s32 count;                                           /* 0x3FC */
} EffEventBillBatch;

typedef char EffEventBillBatch_size_must_be_0x400[
    (sizeof(EffEventBillBatch) == 0x400) ? 1 : -1];

#endif
