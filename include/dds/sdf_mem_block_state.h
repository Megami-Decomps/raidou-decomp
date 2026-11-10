#ifndef DDS_SDF_MEM_BLOCK_STATE_H

#define DDS_SDF_MEM_BLOCK_STATE_H
#include "common.h"

/* General-heap descriptor lifecycle state; distinct from resource type. */
typedef u16 SdfMemBlockState;

#define SDF_MEM_BLOCK_STATE_FREE 0

#define SDF_MEM_BLOCK_STATE_USED 1

#define SDF_MEM_BLOCK_STATE_END_SENTINEL 2

#endif /* DDS_SDF_MEM_BLOCK_STATE_H */
