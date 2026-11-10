#include "shared/sdf/sdfFileIO/decls.h"

/* Update non-negative worker IDs only; skip an unchanged numeric priority. */
void sdfFileCmdThreadSetPrio(s32 prio) {
    sdfFileDeviceCB_t *worker;
    u32 index;
    if (sdfFileCmdThreadPrio == prio) {
        return;
    }
    sdfFileCmdThreadPrio = prio;
    worker = sdfFileDeviceCB;
    index = 0;
    do {
        s32 threadId = worker->thId;
        worker++;
        if (threadId >= 0) {
            ChangeThreadPriority(threadId, prio);
        }
        index++;
    } while (index < 4);
}
