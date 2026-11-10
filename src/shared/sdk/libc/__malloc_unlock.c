#include "shared/sdk/libc/decls.h"

void __malloc_unlock(_reent *r) {
    if (--mallocLockCount != 0) {
        return;
    }
    mallocLockOwner = -1;
    SignalSema(mallocSema);
}
