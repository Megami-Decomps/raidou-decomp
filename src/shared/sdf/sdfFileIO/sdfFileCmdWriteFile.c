#include "shared/sdf/sdfFileIO/decls.h"

/* Enqueue a one-shot write using the supplied buffer, byte count and creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteFile(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
                        s32 callbackContext, s32 options) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_FILE_CMD_WRITEFILE, callback, callbackContext);
    state->length = byteCount;
    state->buffer = EE_ADDR_AS(void *, buffer);
    state->cmdParm = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    state->offset = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}
