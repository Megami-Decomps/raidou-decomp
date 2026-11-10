#include "shared/newbattle/nbCameraHelper/decls.h"

void dds3EffectListSetTexture(nbCameraHelper_effFLWinHandle_t *work, char *textureResource) {
    sdfTexHandle_t *texture;
    sdfMemHandle_t *loadedResource;
    u32 resourceWords[4];

    if (work->textureHandle != NULL) {
        sdfTexFreeDelayed(work->textureHandle);
        work->textureHandle = NULL;
    }
    loadedResource = sdfDevLoadFileEx(textureResource, resourceWords, 0);
    texture = sdfTexSet(EE_ADDR_AS(void *, (resourceWords[0])));
    work->textureHandle = texture;
    sdfMemFreeDirect(loadedResource);
}
