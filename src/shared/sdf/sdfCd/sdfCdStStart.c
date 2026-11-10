#include "shared/sdf/sdfCd/decls.h"

void sdfCdStStart(s32 lsn) {
    u8 options[4];
    s32 ready;
    options[0] = 0;
    if (sdfCdImageMode == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfCdImageFd);
        ready = sceCdStStart(lsn, options);
        SignalSema(sdfCdImageFd);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfCdWaitTrayClose();
        } else {
            sdfDelayThread(100);
        }
    }
    sdfCdCurrLsn = lsn;
}
