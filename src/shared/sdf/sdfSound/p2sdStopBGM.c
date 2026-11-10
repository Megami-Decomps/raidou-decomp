#include "shared/sdf/sdfSound/decls.h"

s32 p2sdStopBGM(s32 aId)
{
    s32 arg[4];

    arg[0] = aId;
    return sdfSndSendCmd(0xD0, 0, arg, sizeof(arg));
}
