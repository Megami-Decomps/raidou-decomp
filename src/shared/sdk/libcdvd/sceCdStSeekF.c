#include "shared/sdk/libcdvd/decls.h"

s32 sceCdStSeekF(u32 lsn)
{
    return sceCdStream(lsn, 0, 0, 9, cdvdDefaultStreamMode);
}
