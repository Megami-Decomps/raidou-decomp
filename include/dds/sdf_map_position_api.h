#ifndef DDS_SDF_MAP_POSITION_API_H

#define DDS_SDF_MAP_POSITION_API_H
#include "common.h"

typedef s32 SdfMapPositionLookupResult;

enum SdfMapPositionLookupResultValue {
    SDF_MAP_POSITION_LOOKUP_NOT_FOUND = 0,
    SDF_MAP_POSITION_LOOKUP_FOUND = 1
};

struct dds_sdfModelRootNode_t;

#endif /* DDS_SDF_MAP_POSITION_API_H */
