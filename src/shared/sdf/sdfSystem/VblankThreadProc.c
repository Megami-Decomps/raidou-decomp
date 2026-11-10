#include "shared/sdf/sdfSystem/decls.h"

void VblankThreadProc(void) {
    sdfThCB_t *threadEntry;

    DmacAddHandler(0, 2, EE_ADDR_S(VblankIntcHandler), -1, 0);
    EnableIntc(2);
    for (;;) {
        SleepThread();
        sdfVblankFlag++;
        WaitSema(sdfThreadSema);
        for (threadEntry = sdfVblankThreadId; threadEntry != NULL; threadEntry = threadEntry->next) {
            WakeupThread(threadEntry->thId);
        }
        SignalSema(sdfThreadSema);
    }
}
