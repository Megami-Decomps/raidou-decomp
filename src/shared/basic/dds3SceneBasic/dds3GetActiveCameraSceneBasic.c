#include "shared/basic/dds3SceneBasic/decls.h"

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic)
{
    return ((dds3SceneBasic_t *)pBasic->workdata)->activecamera;
}
