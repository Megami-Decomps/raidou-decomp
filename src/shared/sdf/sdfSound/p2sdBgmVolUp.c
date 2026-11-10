#include "shared/sdf/sdfSound/decls.h"

s32 p2sdBgmVolUp(void)
{
    return sdfSndSendCmd(0x190, 0, 0, 0);
}
