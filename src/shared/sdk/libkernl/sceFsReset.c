#include "shared/sdk/libkernl/decls.h"

s32 sceFsReset(void)
{
    fsInitialized = 0;
    memset(fsDetectedVersion, 0, 4);
    return 0;
}
