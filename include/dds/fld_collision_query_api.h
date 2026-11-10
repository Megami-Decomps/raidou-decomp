#ifndef DDS_FLD_COLLISION_QUERY_API_H

#define DDS_FLD_COLLISION_QUERY_API_H
#include "common.h"

/* Segment sweeps return the intersected FldValueRecord index; -1 means no hit. */
typedef s32 FldValueRecordIndex;

enum {
    FLD_VALUE_RECORD_INDEX_NO_HIT = -1
};

#endif /* DDS_FLD_COLLISION_QUERY_API_H */
