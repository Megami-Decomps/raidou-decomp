#include "shared/newbattle/nbCameraHelper/decls.h"

u32 dds3EffectFileListWindowGetFilename(nbCameraHelper_effFLWinHandle_t *a, char *b) {
    sprintf(b, dds3EffectResourcePathFormat, a->resourceList->directoryPath, a->selectedEntry->name);
    return a->selectedEntry->type;
}
