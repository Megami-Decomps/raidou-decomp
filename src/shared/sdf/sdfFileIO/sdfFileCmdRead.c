#include "shared/sdf/sdfFileIO/decls.h"

/* Queue a byte-counted read; return -1 without changing an inactive request. */
s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength) {
    sdfFileCmdHandle_t *state = aHandle;
    if (state->stat != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    state->length = aLength;
    state->cmd = SDF_FILE_CMD_READ;
    state->buffer = aBuffer;
    SignalSema(sdfFileDeviceCB[state->device].semaId);
    return 0;
}
