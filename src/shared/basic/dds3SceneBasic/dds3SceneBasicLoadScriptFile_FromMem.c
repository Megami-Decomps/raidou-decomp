#include "shared/basic/dds3SceneBasic/decls.h"

/* A null handle preserves the old script; replacement releases it first. */
s32 dds3SceneBasicLoadScriptFile_FromMem(dds3Basic_t *scene, sdfMemHandle_t *mem)
{
    s32 notAttached = 0;
    void *address;
    dds3SceneBasic_t *work = scene->workdata;

    if (mem == NULL) {
        return notAttached;
    }
    if (work->hScrFileHandle != NULL) {
        dds3SceneBasicReleaseScriptFile(scene);
    }
    address = sdfMemLock(mem);
    if (address != NULL) {
        work->hScrFileHandle = mem;
        work->pScrFileData = address;
        return 1;
    }
    return notAttached;
}
