#include "shared/basic/dds3WorldBasic/decls.h"

void dds3DeleteAllBasicArray(dds3BasicArrayHandle_t *hndl)
{
    dds3WorldBasic_t *world;
    s32 index;

    if (hndl == NULL) {
        return;
    }
    if (hndl->count == 0) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = dds3TheWorld->workdata;
    index = hndl->firstindex;
    while (index >= 0) {
        world->basicarray[index].target = NULL;
        index = world->basicarray[index].next;
    }
    if (world->lastempty < 0) {
        world->firstempty = hndl->firstindex;
        world->lastempty = hndl->lastindex;
    } else {
        world->basicarray[world->lastempty].next = hndl->firstindex;
        world->basicarray[hndl->firstindex].prev = world->lastempty;
        world->lastempty = hndl->lastindex;
    }
    world->emptycount += hndl->count;
    hndl->count = 0;
    hndl->firstindex = -1;
    hndl->lastindex = -1;
    hndl->nowindex = -1;
}
