#include "shared/sdf/sdfFileIO/decls.h"

/* Queue a byte-counted write; return -1 without changing an inactive request. */
s32 sdfFileCmdWrite(sdfFileCmdHandle_t *aHandle, const void *aBuffer, s32 aLength) {
    if (aHandle->stat != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    aHandle->length = aLength;
    aHandle->cmd = SDF_FILE_CMD_WRITE;
    aHandle->buffer = (void *)aBuffer;
    SignalSema(sdfFileDeviceCB[aHandle->device].semaId);
    return 0;
}
