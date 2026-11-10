#ifndef DDS_MDL_MOTION_SEARCH_ID_H

#define DDS_MDL_MOTION_SEARCH_ID_H
#include "common.h"

/* Key selecting one of a model group's motion registrations and its matching
 * per-context controller. The public API keeps the native signed word width;
 * each controller stores the key in its existing signed halfword field. */
typedef s32 MdlMotionSearchId;

#endif /* DDS_MDL_MOTION_SEARCH_ID_H */
