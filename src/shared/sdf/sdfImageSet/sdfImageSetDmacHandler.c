#include "shared/sdf/sdfImageSet/decls.h"

s32 sdfImageSetDmacHandler(s32 channel, s32 threadId)
{
    iWakeupThread(threadId);
    EE_SYNC_EI();
    return -1;
}
