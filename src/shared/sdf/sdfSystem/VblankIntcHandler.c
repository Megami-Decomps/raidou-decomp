#include "shared/sdf/sdfSystem/decls.h"

s32 VblankIntcHandler(s32 cause)
{
    u8 clock;

    if (cause == 2) {
        iWakeupThread(sdfVblankSema);
        clock = sdfCdClock;
        if (clock != 0) {
            sdfCdClock = clock - 1;
        }
    }
    return 0;
}
