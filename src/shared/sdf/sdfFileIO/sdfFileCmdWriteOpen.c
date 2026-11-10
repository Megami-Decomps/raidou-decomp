#include "shared/sdf/sdfFileIO/decls.h"

/* Enqueue a write open; zero options select the device's default creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
                        s32 callbackContext, s32 options) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_FILE_CMD_WRITEOPEN, callback, callbackContext);
    state->cmdParm = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    sdfFileCmdAllocHandle(state);
    return state;
}
