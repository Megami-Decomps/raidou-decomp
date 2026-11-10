#include "shared/basic/dds3SceneBasic/decls.h"

dds3Basic_t *dds3SetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pUnitBasic)
{
    dds3SceneBasic_t *work;
    dds3Basic_t *previous;

    work = pSceneBasic->workdata;
    previous = dds3GetActiveUnitSceneBasic(pSceneBasic);
    work->activeunit = pUnitBasic;
    return previous;
}
