#include "shared/basic/dds3WorldBasic/decls.h"

s32 worldconstructor(dds3Basic_t *basic)
{
    dds3WorldBasic_t *world;

    world = sdfChipAllocDirect(sizeof(dds3WorldBasic_t));
    if (world == NULL) {
        return 0;
    }
    world->firstscene = NULL;
    world->lastscene = NULL;
    world->activescene = NULL;
    world->controlscene = NULL;
    world->basicarray_hndl = NULL;
    world->basicarray = NULL;
    world->basicarraycnt = 0;
    world->firstempty = -1;
    world->emptycount = 0;
    world->firstheader = NULL;
    world->lastheader = NULL;
    world->lastempty = -1;
    basic->workdata = world;
    return 1;
}
