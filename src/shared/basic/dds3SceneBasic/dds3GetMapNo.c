#include "shared/basic/dds3SceneBasic/decls.h"

s32 dds3GetMapNo(dds3Basic_t *scene)
{
    if (scene == NULL) {
        return -1;
    }
    return ((dds3SceneBasic_t *)scene->workdata)->mapno;
}
