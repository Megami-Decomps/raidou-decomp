#include "shared/sdf/sdfSound/decls.h"

s32 p2sdSyncMovie(void)
{
    return sdfSndSendSyncCmd(0x100, 0, 0, 0);
}
