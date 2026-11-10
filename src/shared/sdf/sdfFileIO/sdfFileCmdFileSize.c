#include "shared/sdf/sdfFileIO/decls.h"

/* Query file size without changing the file position; return -1 unless active. */
s32 sdfFileCmdFileSize(void *aHandle) {
    sdfFileCmdHandle_t *state = aHandle;
    if (state->stat != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    state->cmd = SDF_FILE_CMD_FILESIZE;
    SignalSema(sdfFileDeviceCB[state->device].semaId);
    return 0;
}
