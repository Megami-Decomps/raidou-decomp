#ifndef DDS_MDL_MOTION_CONTROLLER_LOOKUP_API_H

#define DDS_MDL_MOTION_CONTROLLER_LOOKUP_API_H
#include "common.h"

#include "mdl_motion_search_id.h"

#if !defined(SMT3_DEFINES_dds3ModelHandle_t)
typedef struct dds3ModelHandle_t dds3ModelHandle_t;
#endif

struct sdfMotionCtrlr_t;

/* Use only when the caller must update the borrowed controller in place. */
static inline struct sdfMotionCtrlr_t *mdlGetMutableMotionCtrlr(
        dds3ModelHandle_t *ctx, MdlMotionSearchId searchId) {
    return (struct sdfMotionCtrlr_t *)mdlGetMotionCtrlr(ctx, searchId);
}

#endif /* DDS_MDL_MOTION_CONTROLLER_LOOKUP_API_H */
