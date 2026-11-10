#include "shared/sdk/libkernl/decls.h"

s32 sceSifLoadStartModule(const char *path, s32 argumentSize, const void *arguments,
                          s32 *moduleResult)
{
    return _sceSifLoadModule(path, argumentSize, arguments, moduleResult, 0);
}
