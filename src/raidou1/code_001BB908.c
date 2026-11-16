#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "evtEventViewer.h"
#include "evtEventViewerMenu.h"

/* body: matched in dds @ 18d1fa88f: src/dds1/event/evtEventViewerFrameVar.c:34, found, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
dds3TaskUpdateResult_t evtUpdateFrameVariableTask(dds3ProcessID_t *task) {
    u8 unused[16]; /* retail frame 0x20: unused local storage */

    dds3GetProcessData(task->parent);
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}
