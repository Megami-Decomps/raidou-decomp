#include "shared/sdk/libcdvd/decls.h"

s32 sceCdStSeek(u32 lsn)
{
    return sceCdStream(lsn, 0, 0, 4, cdvdDefaultStreamMode);
}
