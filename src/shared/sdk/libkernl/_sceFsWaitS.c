#include "shared/sdk/libkernl/decls.h"

s32 _sceFsWaitS(s32 command)
{
    _sceFsSemInit();
    WaitSema(fsCommandSema);
    return 0;
}
