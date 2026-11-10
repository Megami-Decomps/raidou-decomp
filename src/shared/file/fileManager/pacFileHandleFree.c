#include "shared/file/fileManager/decls.h"

s32 pacFileHandleFree(fileRequest_t *pacHandle)
{
    if (pacHandle->state == 6) {
        void *deviceRequest = pacHandle->dev;
        if (deviceRequest != NULL) {
            sdfFileCmdFree(deviceRequest);
        }
        sdfPacFree(&pacHandle->pac);
        sdfChipFreeDirect(pacHandle->name);
        sdfChipFreeDirect(pacHandle);
        return 0;
    }
    return 1;
}
