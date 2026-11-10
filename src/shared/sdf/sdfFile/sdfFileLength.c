#include "shared/sdf/sdfFile/decls.h"

u32 sdfFileLength(void *aHandle)
{
    sdfFileCmdFileSize(aHandle);
    WaitSema(sdfFileSemaId);
    return sdfFileLengthWork;
}
