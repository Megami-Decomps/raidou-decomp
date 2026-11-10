#include "shared/event/evtEventPack/decls.h"

/* Return this event task's loaded state, or -1 when it does not exist. */
s32 evtGetEventPackStat(u32 eventno) {
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventno);
    if (task != 0) {
        evtPackProcessData_t *state = (evtPackProcessData_t *)dds3GetProcessData(task);
        return state->stat;
    }
    return -1;
}
