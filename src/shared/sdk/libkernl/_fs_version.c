#include "shared/sdk/libkernl/decls.h"

s32 _fs_version(void)
{
    const void *expected = kernelBuiltinVersion;
    const void *version = fsDetectedVersion;
    s32 mismatch = 0;
    if (memcmp(version, expected, 4) != 0) {
        if (memcmp(version, fsFallbackVersion, 4) != 0) {
            mismatch = memcmp(expected, fsFallbackVersion, 4) != 0;
        }
    }
    return mismatch;
}
