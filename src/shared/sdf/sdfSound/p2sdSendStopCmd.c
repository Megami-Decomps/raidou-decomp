#include "shared/sdf/sdfSound/decls.h"

s32 p2sdSendStopCmd(s32 aId)
{
    s32 arg[4];

    arg[0] = aId;
    return sdfSndSendCmd(0x30, 0, arg, sizeof(arg));
}
