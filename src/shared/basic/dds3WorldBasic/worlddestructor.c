#include "shared/basic/dds3WorldBasic/decls.h"

void worlddestructor(dds3Basic_t *basic)
{
    dds3WorldBasic_t *world;

    world = basic->workdata;
    if (world == NULL) {
        return;
    }
    while (world->firstscene != NULL) {
        dds3DeleteScene(world->firstscene);
    }
    while (world->firstheader != NULL) {
        dds3FreeBasicArrayHandle(world->firstheader);
    }
    if (world->basicarray_hndl != NULL) {
        sdfMemFreeDirect(world->basicarray_hndl);
    }
    sdfChipFreeDirect(world);
}
