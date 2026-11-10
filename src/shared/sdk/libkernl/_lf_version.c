#include "shared/sdk/libkernl/decls.h"

s32 _lf_version(void)
{
    const void *builtin = kernelBuiltinVersion;
    const void *detected = lfDetectedVersion;
    s32 mismatch = 0;

    if (memcmp(detected, builtin, 4) != 0) {
        if (memcmp(detected, lfFallbackVersion, 4) != 0) {
            mismatch = memcmp(builtin, lfFallbackVersion, 4) != 0;
        }
    }
    return mismatch;
}
