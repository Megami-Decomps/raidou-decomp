#include "shared/sdk/libkernl/decls.h"

s32 sceSifLoadFileReset(void)
{
    lfBindStatus = -1;
    memset(lfDetectedVersion, 0, 4);
    return 0;
}
