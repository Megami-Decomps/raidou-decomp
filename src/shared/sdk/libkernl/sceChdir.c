#include "shared/sdk/libkernl/decls.h"

s32 sceChdir(const char *path)
{
    return _sceCallCode(path, 18);
}
