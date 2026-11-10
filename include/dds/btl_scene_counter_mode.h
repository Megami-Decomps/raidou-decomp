#ifndef DDS_BTL_SCENE_COUNTER_MODE_H

#define DDS_BTL_SCENE_COUNTER_MODE_H
#include "common.h"

/* Action-result classification forwarded to the scene counter task. */
typedef s32 BtlSceneCounterMode;

enum {
    BTL_SCENE_COUNTER_MODE_NOOP = 0,
    BTL_SCENE_COUNTER_MODE_NORMAL = 1,
    BTL_SCENE_COUNTER_MODE_CRITICAL = 2,
    BTL_SCENE_COUNTER_MODE_WEAKNESS = 3,
};

#endif /* DDS_BTL_SCENE_COUNTER_MODE_H */
