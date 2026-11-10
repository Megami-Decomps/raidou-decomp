#include "shared/sdf/sdfFileIO/decls.h"

/* Start worker thread `index` if it isn't running; slot 3 runs the alternate entry point. */
void sdfFileCmdStartThread(s32 deviceType) {
    sdfFileDeviceCB_t *worker = &sdfFileDeviceCB[deviceType];
    void (*entry)(sdfFileDeviceCB_t *);
    s32 thread;
    thread = worker->thId;
    if (thread < 0) {
        worker->semaId = sdfCreateSema(0, 0xFF, 0);
        entry = sdfFileCdImageCmdThread;
        if (deviceType != 3) {
            entry = sdfFileCmdThread;
        }
        sdfFileCmdThreadPrio = 0x48;
        thread = sdfCreateThreadWithStack(entry, 0x4000, 0x48);
        worker->thId = thread;
        _StartThread(thread, worker);
    }
}
