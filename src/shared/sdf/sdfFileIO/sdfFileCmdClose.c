#include "shared/sdf/sdfFileIO/decls.h"

/* Active state and close operation share an ID; retain the original byte assignment. */
s32 sdfFileCmdClose(void *aHandle) {
    sdfFileCmdHandle_t *request = aHandle;
    sdfDevStateCode_t requestState = request->stat;
    if (requestState != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    request->cmd = requestState;
    SignalSema(sdfFileDeviceCB[request->device].semaId);
    return 0;
}
