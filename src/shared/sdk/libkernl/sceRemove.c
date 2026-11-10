#include "shared/sdk/libkernl/decls.h"

s32 sceRemove(const char *path)
{
    return _sceCallCode(path, 6);
}
