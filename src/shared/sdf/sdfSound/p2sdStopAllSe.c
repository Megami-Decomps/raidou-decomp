#include "shared/sdf/sdfSound/decls.h"

s32 p2sdStopAllSe(void)
{
    return sdfSndSendCmd(0x40, 0, 0, 0);
}
