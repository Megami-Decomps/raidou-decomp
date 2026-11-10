#ifndef DDS_MDL_MOTION_API_H

#define DDS_MDL_MOTION_API_H
#include "common.h"

#include "mdl_motion_search_id.h"

#if !defined(SMT3_DEFINES_dds3ModelHandle_t)
typedef struct dds3ModelHandle_t dds3ModelHandle_t;
#endif

typedef u8 MdlMotionPresence;

enum {
    MDL_MOTION_ABSENT = 0,
    MDL_MOTION_PRESENT = 1
};

/* A controller's u16 motion index widened to s32; -1 means no node was found. */
typedef s32 MdlMotionIndexResult;

enum {
    MDL_MOTION_INDEX_MISSING = -1
};

#endif /* DDS_MDL_MOTION_API_H */
