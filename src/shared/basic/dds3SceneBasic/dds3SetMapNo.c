#include "shared/basic/dds3SceneBasic/decls.h"

void dds3SetMapNo(dds3Basic_t *scene, u32 mapno)
{
    if (scene != NULL) {
        ((dds3SceneBasic_t *)scene->workdata)->mapno = mapno;
    }
}
