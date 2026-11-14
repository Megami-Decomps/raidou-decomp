#ifndef DDS_MDL_MOTION_END_STATUS_H

#define DDS_MDL_MOTION_END_STATUS_H
#include "mdl_motion_api.h"

typedef s32 MdlMotionEndStatus;

enum {
    MDL_MOTION_END_NOT_TERMINAL = 0,
    MDL_MOTION_END_TERMINAL = 1,
    MDL_MOTION_END_NODE_MISSING = 2
};

#endif /* DDS_MDL_MOTION_END_STATUS_H */
