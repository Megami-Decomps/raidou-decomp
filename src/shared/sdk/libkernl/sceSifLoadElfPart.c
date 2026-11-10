#include "shared/sdk/libkernl/decls.h"

s32 sceSifLoadElfPart(const char *path, const char *section, u32 *result)
{
    return _sceSifLoadElfPart(path, section, result, 1);
}
