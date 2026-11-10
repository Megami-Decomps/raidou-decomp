#include "shared/basic/dds3WorldBasic/decls.h"

dds3Basic_t *dds3GetControlScene(void)
{
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world == NULL) {
        return NULL;
    }
    return ((dds3WorldBasic_t *)world->workdata)->controlscene;
}
