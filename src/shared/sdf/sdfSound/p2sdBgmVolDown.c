#include "shared/sdf/sdfSound/decls.h"

s32 p2sdBgmVolDown(void)
{
    return sdfSndSendCmd(0x180, 0, 0, 0);
}
