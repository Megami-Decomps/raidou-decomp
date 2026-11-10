#include "shared/sdk/libkernl/decls.h"

s32 kill(s32 pid, s32 signal)
{
    if (pid == 1) {
        Exit(signal);
    }
    return 0;
}
