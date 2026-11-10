#include "shared/basic/dds3SceneBasic/decls.h"

dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type)
{
    dds3SceneBasic_t *work = pBasic->workdata;
    dds3BasicArrayHandle_t *handle;
    dds3Basic_t *basic;

    if (work->basictable[type].linkcount == 0) {
        return NULL;
    }
    handle = dds3GetBasicArrayHandle(0);
    basic = work->basictable[type].firstbasic;
    do {
        dds3AddBasicArray(handle, 1);
        dds3SetBasicArray(handle, basic);
        basic = basic->next;
    } while (basic != NULL);
    return handle;
}
