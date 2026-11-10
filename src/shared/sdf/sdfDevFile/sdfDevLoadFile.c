#include "shared/sdf/sdfDevFile/decls.h"

/* Read a named file through the dev RPC into a freshly allocated block; returns the block's handle.
 * outData receives the block address, outSize the file size; without outData the block is released. */
void *sdfDevLoadFile(const char *name, u32 *outData, u32 *outSize, s32 extra) {
    sdfFileCmdHandle_t *state = sdfFileOpen(name);
    s32 size = sdfFileLength(state);
    sdfMemHandle_t *handle = sdfMemAllocDirect(size + extra);
    u32 address = sdfMemLock(handle);
    sdfFileRead(state, EE_ADDR_AS(void *, address), size);
    sdfFileClose(state);
    if (outData != NULL) {
        *outData = address;
    } else {
        sdfMemUnlock(handle);
    }
    if (outSize != NULL) {
        *outSize = size;
    }
    return handle;
}
