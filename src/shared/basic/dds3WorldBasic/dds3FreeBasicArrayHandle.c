#include "shared/basic/dds3WorldBasic/decls.h"

void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl)
{
    dds3WorldBasic_t *world;

    if (hndl == NULL) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    dds3DeleteAllBasicArray(hndl);
    if (hndl->prev == NULL) {
        world->firstheader = hndl->next;
    } else {
        hndl->prev->next = hndl->next;
    }
    if (hndl->next == NULL) {
        world->lastheader = hndl->prev;
    } else {
        hndl->next->prev = hndl->prev;
    }
    sdfChipFreeDirect(hndl);
}
