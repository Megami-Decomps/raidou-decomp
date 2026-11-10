#include "shared/sdk/libmc/decls.h"

s32 sceMcEnd(void)
{
    if (mcSema >= 0) {
        DeleteSema(mcSema);
        mcSema = -1;
    }
    return 1;
}
