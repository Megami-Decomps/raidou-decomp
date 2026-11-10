#include "shared/basic/dds3WorldBasic/decls.h"

/* The cursor advances to the slot after the one deleted. */
void dds3DeleteBasicArray(dds3BasicArrayHandle_t *hndl)
{
    dds3WorldBasic_t *world;
    dds3BasicArray_t *array;
    s16 prev;
    s16 next;

    if (hndl == NULL) {
        return;
    }
    if (hndl->count == 0) {
        return;
    }
    if (hndl->nowindex < 0) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    array = world->basicarray;
    prev = array[hndl->nowindex].prev;
    array[hndl->nowindex].target = NULL;
    next = array[hndl->nowindex].next;
    if (prev < 0) {
        hndl->firstindex = next;
    } else {
        array[prev].next = next;
    }
    if (next < 0) {
        hndl->lastindex = prev;
    } else {
        array[next].prev = prev;
    }
    array[hndl->nowindex].prev = -1;
    array[hndl->nowindex].next = -1;
    if (world->lastempty < 0) {
        world->firstempty = hndl->nowindex;
        world->lastempty = hndl->nowindex;
    } else {
        array[world->lastempty].next = hndl->nowindex;
        array[hndl->nowindex].prev = world->lastempty;
        world->lastempty = hndl->nowindex;
    }
    world->emptycount++;
    hndl->nowindex = next;
    hndl->count--;
}
