#ifndef DDS_DDS_EFF_FRAME_STATE_H

#define DDS_DDS_EFF_FRAME_STATE_H
#include "common.h"

struct sdfMemHandle_t;

/* Shared 0xC per-instance prefix: entries, asset view and allocation handle. */
typedef struct EffFrameState {
    u8 *entries;
    u8 *asset;
    struct sdfMemHandle_t *allocation;
} EffFrameState;

typedef char EffFrameState_size_check[
    (sizeof(EffFrameState) == 0xC) ? 1 : -1];

#endif
