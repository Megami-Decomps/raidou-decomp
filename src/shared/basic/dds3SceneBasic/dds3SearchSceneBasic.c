#include "shared/basic/dds3SceneBasic/decls.h"

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type)
{
    dds3Basic_t *basic;

    if (((dds3SceneBasic_t *)pBasic->workdata)->basictable[type].linkcount == 0) {
        return NULL;
    }
    basic = ((dds3SceneBasic_t *)pBasic->workdata)->basictable[type].firstbasic;
    do {
        if (serialID == basic->serialID) {
            return basic;
        }
        basic = basic->next;
    } while (basic != NULL);
    return NULL;
}
