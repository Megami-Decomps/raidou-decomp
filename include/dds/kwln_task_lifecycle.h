#ifndef DDS_KWLN_TASK_LIFECYCLE_H

#define DDS_KWLN_TASK_LIFECYCLE_H
#include "kwln.h"

/* Whether the scheduler accepted a destruction request; this does not mean
 * that the task has completed destruction. */
typedef s32 KwlnTaskKillRequestResult;

#define KWLN_TASK_KILL_REQUEST_REJECTED 0

#define KWLN_TASK_KILL_REQUEST_ACCEPTED 1

#endif /* DDS_KWLN_TASK_LIFECYCLE_H */
