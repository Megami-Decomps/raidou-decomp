#include "shared/sdf/sdfGraph/decls.h"

s32 sdfDrawThIntHandler(s32 channel) {
    if (channel == 1) {
        iWakeupThread(sdfDrawThreadId);
    }
    EE_SYNC_EI_BARRIER();
    return 0;
}
