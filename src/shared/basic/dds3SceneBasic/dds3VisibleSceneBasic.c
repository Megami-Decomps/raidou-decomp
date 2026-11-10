#include "shared/basic/dds3SceneBasic/decls.h"

void dds3VisibleSceneBasic(dds3Basic_t *basic, s8 visible)
{
    if ((dds3SceneBasic_t *)basic->workdata != NULL) {
        ((dds3SceneBasic_t *)basic->workdata)->visible = visible;
    }
}
