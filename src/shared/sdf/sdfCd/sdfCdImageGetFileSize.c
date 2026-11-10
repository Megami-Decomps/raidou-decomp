#include "shared/sdf/sdfCd/decls.h"

s32 sdfCdImageGetFileSize(const char *fname)
{
    s32 lsn;
    sdfCdImageDirEntry_t *entry = sdfCdImageSearchFile(fname, &lsn, NULL);

    if (entry == NULL) {
        return -1;
    }
    return entry->size_;
}
