#include "shared/sdk/libkernl/decls.h"

s32 sceSifLoadStartModuleBuffer(void *buffer, s32 argumentSize, const void *arguments,
                                s32 *moduleResult)
{
    return _sceSifLoadModuleBuffer(buffer, argumentSize, arguments, moduleResult);
}
