#ifndef DDS_DDS3_TASK_LIFECYCLE_H

#define DDS_DDS3_TASK_LIFECYCLE_H
#include "kwln.h"

/* Whether the scheduler accepted a destruction request; this does not mean
 * that the task has completed destruction. */
typedef s32 dds3TaskKillRequestResult_t;

#define DDS3_TASK_KILL_REQUEST_REJECTED 0

#define DDS3_TASK_KILL_REQUEST_ACCEPTED 1

/* Reserved invalid task-handle value; lifecycle requests reject it. */

#define DDS3_TASK_HANDLE_INVALID ((dds3ProcessID_t *)-1)

#endif /* DDS_DDS3_TASK_LIFECYCLE_H */
