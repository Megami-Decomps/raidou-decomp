#include "shared/basic/dds3WorldBasic/decls.h"

dds3Basic_t *dds3GetActiveScene(void)
{
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world == NULL) {
        return NULL;
    }
    return ((dds3WorldBasic_t *)world->workdata)->activescene;
}
