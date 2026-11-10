#include "shared/basic/dds3WorldBasic/decls.h"

u32 dds3AddBasicArray(dds3BasicArrayHandle_t *hndl, s32 bufcnt)
{
    dds3WorldBasic_t *world;
    s16 first;
    s16 last;
    s32 i;

    if (bufcnt == 0) {
        return 1;
    }
    if (dds3TheWorld == NULL) {
        return 0;
    }
    world = dds3TheWorld->workdata;
    if (world->emptycount < bufcnt || bufcnt < 0) {
        return 0;
    }
    if (hndl == NULL) {
        return 0;
    }
    first = world->firstempty;
    for (i = 0; i < bufcnt - 1; i++) {
        world->firstempty = world->basicarray[world->firstempty].next;
    }
    last = world->firstempty;
    world->firstempty = world->basicarray[last].next;
    if (world->firstempty == -1) {
        world->lastempty = -1;
    } else {
        world->basicarray[last].next = -1;
        world->basicarray[world->firstempty].prev = -1;
    }
    world->emptycount -= bufcnt;
    if (hndl->lastindex < 0) {
        hndl->lastindex = last;
        hndl->firstindex = first;
    } else {
        world->basicarray[hndl->lastindex].next = first;
        world->basicarray[first].prev = hndl->lastindex;
        hndl->lastindex = last;
    }
    hndl->nowindex = first;
    hndl->count += bufcnt;
    return 1;
}
