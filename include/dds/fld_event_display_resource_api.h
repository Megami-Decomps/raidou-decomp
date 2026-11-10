#ifndef DDS_FLD_EVENT_DISPLAY_RESOURCE_API_H

#define DDS_FLD_EVENT_DISPLAY_RESOURCE_API_H
#include "common.h"

struct FldFileResource;

struct dds3Basic_t;

/* Index for a borrowed event display resource stored in fldDisplayValues.
 * Its owner keeps the record alive through room-event cleanup. */
typedef u32 FldEventDisplayResourceIndex;

#endif /* DDS_FLD_EVENT_DISPLAY_RESOURCE_API_H */
