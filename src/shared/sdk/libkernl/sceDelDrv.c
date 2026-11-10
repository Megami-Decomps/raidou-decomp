#include "shared/sdk/libkernl/decls.h"

s32 sceDelDrv(const char *path)
{
    return _sceCallCode(path, 16);
}
