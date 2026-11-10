#include "shared/sdk/libcdvd/decls.h"

s32 sceCdStStart(u32 lsn, void *mode)
{
    cdvdStreamActive = 1;
    return sceCdStream(lsn, 0, 0, 1, mode);
}
