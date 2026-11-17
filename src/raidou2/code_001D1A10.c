#include "common.h"
#include "evtEventViewer.h"
#include "evtEventViewerMenu.h"
#include "dds3KernelCore.h"

dds3TaskUpdateResult_t evtUpdateFrameVariableTask(dds3ProcessID_t *task) {
    u8 unused[16]; /* retail frame 0x20: unused local storage */

    dds3GetProcessData(task->parent);
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}
