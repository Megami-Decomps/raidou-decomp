#include "shared/sdf/sdfFile/decls.h"

/* Open a path using the shared reply callback and wait for its initial completion. */
sdfFileCmdHandle_t *sdfFileOpen(const char *aFileName) {
    sdfFileCmdHandle_t *state;
    if (sdfFileSemaId < 0) {
        sdfFileSemaId = sdfCreateSema(0, 0x80, 0);
    }
    state = sdfFileCmdReadOpen(aFileName, (void (*)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32))sdfFileCallback, 0);
    WaitSema(sdfFileSemaId);
    sdfFileCmdClearError((sdfFileCmd_t *)state);
    return state;
}
