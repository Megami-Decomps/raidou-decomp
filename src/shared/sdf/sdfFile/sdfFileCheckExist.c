#include "shared/sdf/sdfFile/decls.h"

s32 sdfFileCheckExist(char *aFilename) {
    char *resolved = sdfFileMakeFilename(aFilename);
    s32 fd;
    if (*resolved == '/') {
        return sdfCdImageCheckExist(resolved);
    }
    fd = sceOpen(resolved, 1);
    sceClose(fd);
    sdfChipFreeDirect(resolved);
    return fd >= 0;
}
