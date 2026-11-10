#include "shared/sdf/sdfDelayedResrc/decls.h"

u64 sdfDelayedResrcCheck(void) {
    s64 interruptState;
    u64 pendingWork;

    interruptState = DIntr();
    pendingWork = sdfDelayedResrcCheckFunc();
    if (interruptState != 0) {
        EIntr();
    }
    return pendingWork;
}
