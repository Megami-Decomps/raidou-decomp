#include "shared/sdk/libkernl/decls.h"

s32 sceRmdir(const char *path)
{
    return _sceCallCode(path, 8);
}
