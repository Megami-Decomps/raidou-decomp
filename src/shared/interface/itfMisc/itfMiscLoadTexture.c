#include "shared/interface/itfMisc/decls.h"

/* Keep the sprite texture handle while releasing the temporary file allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *filename) {
    sdfMemHandle_t *fileAllocation;
    sdfTexHandle_t *textureHandle;
    u32 assetInfo[ITF_VECTOR_WORD_COUNT];

    fileAllocation = sdfDevLoadFileEx(filename, assetInfo, 0);
    textureHandle = sdfTexSet(EE_ADDR_AS(void *, (assetInfo[0])));
    sdfMemFreeDirect(fileAllocation);
    return textureHandle;
}
