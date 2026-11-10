#ifndef DDS_EFF_BILLBOARD_WORK_H

#define DDS_EFF_BILLBOARD_WORK_H
#include "common.h"

struct billHandle_t;

/* Shared DDS1/DDS2 animated billboard effect state. */
typedef struct EffBillboardWork {
    f32 position[4];
    f32 rotation[4];
    f32 scale;
    u32 color;
    s32 frame;
    f32 heightScale;
    f32 widthScale;
    u8 opaque34[0x30];
    struct billHandle_t *billboard;
} EffBillboardWork;

typedef char EffBillboardWork_size_must_be_0x68[
    (sizeof(EffBillboardWork) == 0x68) ? 1 : -1];

#endif /* DDS_EFF_BILLBOARD_WORK_H */
