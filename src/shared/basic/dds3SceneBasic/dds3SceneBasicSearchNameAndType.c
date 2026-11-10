#include "shared/basic/dds3SceneBasic/decls.h"

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name)
{
    dds3Basic_t *basic;
    u8 *candidate;
    s32 i;

    if (scene == NULL || name == NULL) {
        return NULL;
    }
    basic = ((dds3SceneBasic_t *)scene->workdata)->basictable[type].firstbasic;
    while (basic != NULL) {
        candidate = (u8 *)basic->name;
        if (candidate == NULL) {
            basic = basic->next;
            continue;
        }
        if (candidate[0] == '\0') {
            basic = basic->next;
            continue;
        }
        i = 0;
        if (((const u8 *)name)[0] == candidate[0]) {
            do {
                if (((const u8 *)name)[i] == '\0') {
                    return basic;
                }
                i++;
            } while (((const u8 *)name)[i] == ((u8 *)basic->name)[i]);
        }
        basic = basic->next;
    }
    return NULL;
}
