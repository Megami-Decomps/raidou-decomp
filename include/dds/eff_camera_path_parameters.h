#ifndef DDS_DDS_EFF_CAMERA_PATH_PARAMETERS_H

#define DDS_DDS_EFF_CAMERA_PATH_PARAMETERS_H
#include "common.h"

typedef struct EffCameraPathParameters {
    u32 startFrame;
    u8 actorSelection;
    u8 count;
    u8 directionMode;
    u8 pad07;
    u32 defeatSelection;
} EffCameraPathParameters;

typedef char EffCameraPathParameters_size_check[
    (sizeof(EffCameraPathParameters) == 0xC) ? 1 : -1];

#endif
