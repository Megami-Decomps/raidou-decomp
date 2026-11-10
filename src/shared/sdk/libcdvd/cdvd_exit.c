#include "shared/sdk/libcdvd/decls.h"

void cdvd_exit(void)
{
    s32 intr;

    if (cdvdCallbackThreadId != 0) {
        /* The one-pass block preserves the SDK compiler's native ordering:
         * publish the stop reason before loading the semaphore to signal. */
        do {
            cdvdCallbackReason = -1;
        } while (0);
        SignalSema(cdvdScmdSema);
    }
    DeleteSema(cdvdNcmdSema);
    DeleteSema(cdvdScmdLockSema);
    DeleteSema(cdvdScmdSema);
    intr = DIntr();
    sceSifRemoveCmdHandler(0x80000012);
    if (intr) {
        EIntr();
    }
}
