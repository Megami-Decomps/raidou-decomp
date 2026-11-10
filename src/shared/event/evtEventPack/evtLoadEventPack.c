#include "shared/event/evtEventPack/decls.h"

/* Schedule the event-pack process only if it does not already exist. */
dds3ProcessID_t *evtLoadEventPack(s32 eventno) {
    char taskName[0x20];
    evtPackProcessData_t *taskData;
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventno);
    if (task == 0) {
        evtMakeEventPackName(eventno, taskName);
        taskData = sdfChipAllocDirect(sizeof(evtPackProcessData_t));
        memset(taskData, 0, sizeof(evtPackProcessData_t));
        taskData->eventno = eventno;
        taskData->stat = 0;
        task = dds3StartProcess(taskName, DDS3_PRIORITY_EVENT_PACK, 1, 1, evtCheckScriptDone, (dds3ProcessFunc_t)evtWaitEventPackLoad, taskData);
    }
    return task;
}
