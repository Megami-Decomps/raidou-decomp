#include "shared/sdf/sdfFileIO/decls.h"

/* Enqueue a one-shot read: negative byte count requests the whole file. */
sdfFileCmdHandle_t *sdfFileCmdReadFile(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32), s32 callbackContext) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_FILE_CMD_READFILE, callback, callbackContext);
    state->length = byteCount;
    state->buffer = EE_ADDR_AS(void *, buffer);
    state->offset = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}
