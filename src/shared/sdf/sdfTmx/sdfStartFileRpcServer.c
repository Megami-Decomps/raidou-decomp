#include "shared/sdf/sdfTmx/decls.h"

void sdfStartFileRpcServer(void) {
    s32 workerId = sdfCreateThreadWithStack(SdfFileRpcServerThreadProc, 0x1000, 0x4C);

    _StartThread(workerId, 0);
    sdfFileRpcThreadId = GetThreadId();
    SleepThread();
}
