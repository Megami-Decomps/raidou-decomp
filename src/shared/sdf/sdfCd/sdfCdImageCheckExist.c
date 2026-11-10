#include "shared/sdf/sdfCd/decls.h"

s32 sdfCdImageCheckExist(const char *fname)
{
    s32 lsn;

    return sdfCdImageSearchFile(fname, &lsn, NULL) != 0;
}
