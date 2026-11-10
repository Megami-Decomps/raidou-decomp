#include "shared/sdk/libcdvd/decls.h"

s32 sceCdStInit(u32 bufmax, u32 bankmax, void *buf)
{
    cdvdStreamActive = 0;
    return sceCdStream(bufmax, bankmax, buf, 5, cdvdDefaultStreamMode);
}
