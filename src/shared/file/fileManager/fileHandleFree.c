#include "shared/file/fileManager/decls.h"

s32 fileHandleFree(fileRequest_t *fileHandle) {
    if (fileHandle->kind == 1) {
        return pacFileHandleFree(fileHandle);
    }
    if (fileHandle->state == 6) {
        if (fileHandle->dev != NULL) {
            sdfFileCmdFree((sdfFileCmd_t *)fileHandle->dev);
        }
        sdfChipFreeDirect(fileHandle->name);
        sdfChipFreeDirect(fileHandle);
        return 0;
    }
    return 1;
}
