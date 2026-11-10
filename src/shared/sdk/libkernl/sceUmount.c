#include "shared/sdk/libkernl/decls.h"

s32 sceUmount(const char *path)
{
    return _sceCallCode(path, 21);
}
