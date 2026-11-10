#ifndef DDS_EVT_EVENT_PACK_TASK_H

#define DDS_EVT_EVENT_PACK_TASK_H
#include "dds3_task_runner.h"

dds3TaskUpdateResult_t evtTickPackLoad(dds3ProcessID_t *task);

void evtDestroyEventPackTask(dds3ProcessID_t *task);

#endif /* DDS_EVT_EVENT_PACK_TASK_H */
