#include "shared/event/evtEventPack/decls.h"

evtPackProcessData_t *evtGetPackProcessData(u32 eventno) {
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventno);
    if (task != 0) {
        return (evtPackProcessData_t *)dds3GetProcessData(task);
    }
    return NULL;
}
