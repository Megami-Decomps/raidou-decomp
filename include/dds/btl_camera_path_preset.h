#ifndef DDS_BTL_CAMERA_PATH_PRESET_H

#define DDS_BTL_CAMERA_PATH_PRESET_H
#include "common.h"

/* Camera-path preset banks have a native 0x60-byte record stride. */
typedef struct BtlCameraPathPreset {
    u16 kind;              /* 0x00 */
    s8 operation;          /* 0x02: endpoint construction mode */
    u8 pad03;
    f32 startFrame;        /* 0x04 */
    f32 duration;          /* 0x08 */
    f32 angleScale;        /* 0x0C */
    f32 firstOffset[3];    /* 0x10 */
    u8 pad1C[4];
    f32 secondOffset[3];   /* 0x20 */
    u8 pad2C[4];
    u8 unk30;
    u8 pad31[0xF];
    f32 unk40;
    u8 pad44[0xC];
    f32 blendRate;         /* 0x50 */
    u8 pad54[0xC];
} BtlCameraPathPreset;

typedef char BtlCameraPathPresetSizeCheck[
    (sizeof(BtlCameraPathPreset) == 0x60) ? 1 : -1];

#endif /* DDS_BTL_CAMERA_PATH_PRESET_H */
