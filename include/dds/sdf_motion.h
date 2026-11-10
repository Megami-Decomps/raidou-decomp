#ifndef DDS_SDF_MOTION_H

#define DDS_SDF_MOTION_H
#include "common.h"

struct sdfMotionCtrlr_t;

struct MotionTable;

struct dds_sdfModelRootNode_t;

/* Controller storage keeps the current and suspended state in one byte. */
typedef u8 SdfMotionState;

#endif /* DDS_SDF_MOTION_H */
