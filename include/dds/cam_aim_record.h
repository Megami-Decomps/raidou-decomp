#ifndef DDS_CAM_AIM_RECORD_H

#define DDS_CAM_AIM_RECORD_H
#include "common.h"

typedef struct CamAim {
    u8 opaque00[0x50];
    f32 quat[4];
    u8 opaque60[8];
    u32 flags;
} CamAim;

typedef char CamAimSizeCheck[(sizeof(CamAim) == 0x6C) ? 1 : -1];

#endif
