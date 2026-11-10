#include "shared/sdk/libmc/decls.h"

s32 sceMcMkdir(s32 a, s32 b, char *c)
{
    s32 ret = sceMcOpen(a, b, c, 0x40);

    if (ret == 0) {
        mcLastCmd = 11;
    }
    return ret;
}
