#include "shared/sdf/sdfFileIO/decls.h"

/* Allocate a request state and retain the callback's opaque context word. */
sdfFileCmdHandle_t *sdfFileCmdNewHandle(void *resource, s32 workerIndex, s32 operation,
                        void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32), s32 callbackContext) {
    sdfFileCmdHandle_t *state = (sdfFileCmdHandle_t *)sdfChipAllocZeroDirect(sizeof(sdfFileCmdHandle_t));
    state->filename = resource;
    state->device = workerIndex;
    state->cmd = operation;
    state->stat = 0;
    state->fd = -1;
    state->callback = callback;
    state->userData = callbackContext;
    return state;
}
