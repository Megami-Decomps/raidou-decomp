#ifndef DDS_DDS3_TASK_QUEUE_QUERY_H

#define DDS_DDS3_TASK_QUEUE_QUERY_H
#include "common.h"

#include "kwln.h"

#include "dds3_task_state.h"

/* Zero-based position in the current linked order of one scheduler queue. */
typedef s32 dds3TaskQueueIndex_t;

/* Nonnegative queue size; unknown or detached states have no entries. */
typedef s32 dds3TaskQueueCount_t;

#endif /* DDS_DDS3_TASK_QUEUE_QUERY_H */
