#include "shared/basic/dds3SceneBasic/decls.h"

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic)
{
    return ((dds3SceneBasic_t *)pSceneBasic->workdata)->activeunit;
}
