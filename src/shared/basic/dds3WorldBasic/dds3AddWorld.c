#include "shared/basic/dds3WorldBasic/decls.h"

dds3Basic_t *dds3AddWorld(s32 bufcnt)
{
    dds3Basic_t *world;
    dds3WorldBasic_t *work;
    sdfMemHandle_t *handle;
    s32 i;

    if (dds3TheWorld != NULL) {
        return NULL;
    }
    if ((u32)(bufcnt - 32) >= 0x7FE0) {
        return NULL;
    }
    world = dds3AllocBasic(BASIC_TYPE_WORLD);
    work = world->workdata;
    handle = sdfMemAllocDirect(bufcnt * sizeof(dds3BasicArray_t));
    work->basicarray_hndl = handle;
    work->basicarray = sdfMemLock(handle);
    work->basicarraycnt = bufcnt;
    for (i = 0; i < bufcnt; i++) {
        if (i == 0) {
            work->basicarray[i].prev = -1;
        } else {
            work->basicarray[i].prev = i - 1;
        }
        if (i == bufcnt - 1) {
            work->basicarray[i].next = -1;
        } else {
            work->basicarray[i].next = i + 1;
        }
        work->basicarray[i].target = NULL;
    }
    work->firstempty = 0;
    work->lastempty = bufcnt - 1;
    work->emptycount = bufcnt;
    dds3TheWorld = world;
    return world;
}
