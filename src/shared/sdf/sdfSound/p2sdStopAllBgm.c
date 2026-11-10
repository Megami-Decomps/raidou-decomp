#include "shared/sdf/sdfSound/decls.h"

s32 p2sdStopAllBgm(void)
{
    return sdfSndSendCmd(0x1A0, 0, 0, 0);
}
