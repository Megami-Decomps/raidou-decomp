#include "shared/event/evtEventPack/decls.h"

void evtKillEventPack(s32 eventno) {
    dds3ProcessID_t *taskHandle;

    taskHandle = evtSearchEventPack(eventno);
    if (taskHandle != 0) {
        dds3KillProcess(taskHandle, 0);
        return;
    }
}
