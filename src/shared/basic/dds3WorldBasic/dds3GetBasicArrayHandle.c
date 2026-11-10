#include "shared/basic/dds3WorldBasic/decls.h"

dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 bufcnt)
{
    dds3WorldBasic_t *world;
    dds3BasicArrayHandle_t *handle;

    if (dds3TheWorld == NULL) {
        return NULL;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    if (world->emptycount < bufcnt) {
        return NULL;
    }
    handle = sdfChipAllocDirect(sizeof(dds3BasicArrayHandle_t));
    if (handle == NULL) {
        return NULL;
    }
    handle->prev = NULL;
    handle->next = NULL;
    handle->firstindex = -1;
    handle->lastindex = -1;
    handle->nowindex = -1;
    handle->count = 0;
    if (world->lastheader == NULL) {
        world->firstheader = handle;
        world->lastheader = handle;
    } else {
        world->lastheader->next = handle;
        handle->prev = world->lastheader;
        world->lastheader = handle;
    }
    dds3AddBasicArray(handle, bufcnt);
    return handle;
}
