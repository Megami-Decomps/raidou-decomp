#include "shared/sdk/libcdvd/decls.h"

cdvdCallback_t sceCdCallback(cdvdCallback_t func)
{
    s32 intr;
    cdvdCallback_t old;

    if (sceCdSync(1) != 0) {
        return 0;
    }
    intr = DIntr();
    old = cdvdCallback;
    cdvdCallback = func;
    if (intr) {
        EIntr();
    }
    return old;
}
