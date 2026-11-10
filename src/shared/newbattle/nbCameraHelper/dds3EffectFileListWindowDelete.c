#include "shared/newbattle/nbCameraHelper/decls.h"

void dds3EffectFileListWindowDelete(nbCameraHelper_effFLWinHandle_t *a) {
    sdfTexHandle_t *texture = a->textureHandle;

    if (texture != NULL) {
        sdfTexFreeDelayed(texture);
        a->textureHandle = NULL;
    }
    sdfChipFreeDirect(a);
}
