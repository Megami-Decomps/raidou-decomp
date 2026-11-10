#include "shared/sdf/sdfFileIO/decls.h"

/* Queue a seek with its offset and origin; return -1 unless the request is active. */
s32 sdfFileCmdSeek(void *aHandle, s32 aOffset, s32 aWhence) {
    sdfFileCmdHandle_t *state = aHandle;
    if (state->stat != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    state->offset = aOffset;
    state->cmdParm = aWhence;
    state->cmd = SDF_FILE_CMD_SEEK;
    SignalSema(sdfFileDeviceCB[state->device].semaId);
    return 0;
}
