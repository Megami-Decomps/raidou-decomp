#include "shared/sdf/sdfFileIO/decls.h"

/* Store an error/result, release the request, and notify its callback of inactivity. */
void sdfFileCmdFinish(sdfFileCmdHandle_t *state, s32 result) {
    state->errorCode = result;
    state->stat = SDF_FILE_CMD_STAT_ERROR;
    sdfFileCmdReleaseHandle((sdfFileCmd_t *)state);
    if (state->callback != NULL) {
        state->callback(state, 0, 0, 0, state->userData);
    }
}
