#ifndef DDS_BTL_SCENE_LIGHTING_TASK_ARGS_H

#define DDS_BTL_SCENE_LIGHTING_TASK_ARGS_H
#include "common.h"

/* Both task creators allocate this four-byte payload and initialize its value. */
typedef struct SceneLightRestoreArgs {
    u32 value;
} SceneLightRestoreArgs;

typedef char SceneLightRestoreArgs_size_must_be_0x04[
    (sizeof(SceneLightRestoreArgs) == 0x04) ? 1 : -1];

/* The deferred transition task copies three four-float vectors and appends its value. */
typedef struct SceneLightTransitionArgs {
    f32 direction[4];
    f32 lightColor[4];
    f32 ambientColor[4];
    u32 value;
} SceneLightTransitionArgs;

typedef char SceneLightTransitionArgs_size_must_be_0x34[
    (sizeof(SceneLightTransitionArgs) == 0x34) ? 1 : -1];

#endif /* DDS_BTL_SCENE_LIGHTING_TASK_ARGS_H */
