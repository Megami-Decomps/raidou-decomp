#ifndef DDS_BTL_ACTION_CAMERA_PRESET_H

#define DDS_BTL_ACTION_CAMERA_PRESET_H
#include "common.h"

/* Action-filtered camera-pair preset consumed by both title providers. */
typedef struct BtlActionCameraPreset {
    f32 frontQuaternion[4]; /* 0x00 */
    f32 backQuaternion[4]; /* 0x10 */
    f32 frontDistanceScale; /* 0x20 */
    f32 backDistanceScale; /* 0x24 */
    f32 frontCameraYScale; /* 0x28 */
    u32 actionMask; /* 0x2C */
} BtlActionCameraPreset;

typedef char BtlActionCameraPresetSizeCheck[
    (sizeof(BtlActionCameraPreset) == 0x30) ? 1 : -1];

#endif /* DDS_BTL_ACTION_CAMERA_PRESET_H */
