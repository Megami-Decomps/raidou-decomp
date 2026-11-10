#include "shared/sdk/libc/decls.h"

void __malloc_lock(_reent *r) {
    s32 owner = GetThreadId();

    if (mallocLockOwner == owner) {
        mallocLockCount++;
    } else {
        WaitSema(mallocSema);
        mallocLockOwner = owner;
        mallocLockCount++;
    }
}
