#ifndef DDS_BTL_SCENE_LIGHT_PARAMS_H

#define DDS_BTL_SCENE_LIGHT_PARAMS_H
#include "common.h"

/* Position and two RGB values passed to the linkage-scene effect task. */
typedef struct BtlSceneLightParams {
    f32 position[3]; /* 0x00 */
    u8 opaque0C[4]; /* 0x0C */
    f32 color[3]; /* 0x10 */
    u8 opaque1C[4]; /* 0x1C */
    f32 secondaryColor[3]; /* 0x20 */
    u8 opaque2C[4]; /* 0x2C */
} BtlSceneLightParams;

typedef char BtlSceneLightParams_size_must_be_0x30[
    (sizeof(BtlSceneLightParams) == 0x30) ? 1 : -1];

#endif /* DDS_BTL_SCENE_LIGHT_PARAMS_H */
