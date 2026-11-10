#include "shared/sdf/sdfFileIO/decls.h"

void sdfFileCmdReleaseHandle(sdfFileCmd_t *aHandle)
{
    s32 fd = aHandle->fd;

    aHandle->fd = -1;
    if (fd >= 0) {
        sceClose(fd);
    }
    sdfFileCmdDequeue(aHandle);
}
