#include "shared/basic/dds3SceneBasic/decls.h"

s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target)
{
    dds3Basic_t *basic;

    if (scene == NULL || target == NULL) {
        return 0;
    }
    basic = ((dds3SceneBasic_t *)scene->workdata)->basictable[type].firstbasic;
    while (basic != NULL) {
        if (basic == target) {
            return 1;
        }
        basic = basic->next;
    }
    return 0;
}
