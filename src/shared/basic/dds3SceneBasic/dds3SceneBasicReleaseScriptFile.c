#include "shared/basic/dds3SceneBasic/decls.h"

void dds3SceneBasicReleaseScriptFile(dds3Basic_t *scene)
{
    dds3SceneBasic_t *work = scene->workdata;

    if (work->hScrFileHandle != NULL) {
        sdfMemFreeDirect(work->hScrFileHandle);
    }
    work->hScrFileHandle = NULL;
    work->pScrFileData = NULL;
}
