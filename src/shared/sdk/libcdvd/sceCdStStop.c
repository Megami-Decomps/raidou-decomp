#include "shared/sdk/libcdvd/decls.h"

s32 sceCdStStop(void)
{
    cdvdStreamActive = 0;
    return sceCdStream(0, 0, 0, 3, cdvdDefaultStreamMode);
}
