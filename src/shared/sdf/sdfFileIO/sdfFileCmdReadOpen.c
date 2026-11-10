#include "shared/sdf/sdfFileIO/decls.h"

/* Resolve a path to its worker and enqueue an asynchronous read-only open. */
sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
                        s32 callbackContext) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_FILE_CMD_READOPEN, callback, callbackContext);
    sdfFileCmdAllocHandle(state);
    return state;
}
