#ifndef DDS_BTL_CAMERA_SNAPSHOTS_H

#define DDS_BTL_CAMERA_SNAPSHOTS_H
#include "btl_command.h"

typedef struct BtlCameraStoredPose {
    BtlCamState pose;
    u8 pad28[8];
} BtlCameraStoredPose;

typedef struct BtlCameraParameterSnapshot {
    f32 pathEnd[4];
    f32 pathStart[4];
    f32 targetFrom[4];
    f32 targetTo[4];
    f32 cameraFrom[4];
    f32 cameraTo[4];
    f32 direction[4];
    f32 distance;
    u8 pad74[0xC];
    f32 angleVector[4];
    f32 startFrame;
    u8 pad94[0xC];
    f32 duration;
    u8 padA4[0xC];
    u8 parameterD0;
    u8 padB1[0xF];
    f32 parameterE0;
    u8 padC4[0xC];
    f32 blendRate;
    u8 padD4[0xC];
    f32 rangeStart;
    u8 padE4[0xC];
    f32 rangeEnd;
    u8 padF4[0xC];
} BtlCameraParameterSnapshot;

typedef char BtlCameraParameterSnapshot_size_check[
    (sizeof(BtlCameraParameterSnapshot) == 0x100) ? 1 : -1];

typedef char BtlCameraStoredPose_size_check[
    (sizeof(BtlCameraStoredPose) == 0x30) ? 1 : -1];

#endif /* DDS_BTL_CAMERA_SNAPSHOTS_H */
