#ifndef DDS_MDL_BLUR_API_H

#define DDS_MDL_BLUR_API_H
#include "common.h"

#if !defined(SMT3_DEFINES_dds3ModelHandle_t)
typedef struct dds3ModelHandle_t dds3ModelHandle_t;
#endif

/* The kind-four stream record supplies two independent blur controls: its
 * first selector sets the update interval between cloned snapshots; its
 * second sets the total snapshot budget. The model owner stores the remaining
 * budget and the interval countdown separately. */
typedef s32 MdlBlurSpawnInterval;

typedef s32 MdlBlurSnapshotCapacity;

#endif /* DDS_MDL_BLUR_API_H */
