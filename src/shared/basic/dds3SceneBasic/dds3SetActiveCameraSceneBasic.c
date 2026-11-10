#include "shared/basic/dds3SceneBasic/decls.h"

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic)
{
    dds3SceneBasic_t *work;
    dds3Basic_t *previous;

    work = pSceneBasic->workdata;
    previous = dds3GetActiveCameraSceneBasic(pSceneBasic);
    work->activecamera = pCameraBasic;
    return previous;
}
