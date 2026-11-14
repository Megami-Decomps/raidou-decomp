#ifndef DDS_KWLN_TASK_RUNNER_H

#define DDS_KWLN_TASK_RUNNER_H
#include "common.h"

#include "kwln.h"

typedef enum KwlnTaskCursorAction {
    /* The callback finished an active task; EndProcess may unlink and free it. */
    KWLN_TASK_CURSOR_ACTION_RELOAD = 0,
    /* RunProcess did not perform the finish transition. */
    KWLN_TASK_CURSOR_ACTION_ADVANCE = 1
} KwlnTaskCursorAction;

/* Sentinel callback results interpreted by RunProcess. */

#define KWLN_TASK_UPDATE_RETAIN_CALLBACK 0

#define KWLN_TASK_UPDATE_FINISH_CALLBACK (-1)

#endif /* DDS_KWLN_TASK_RUNNER_H */
