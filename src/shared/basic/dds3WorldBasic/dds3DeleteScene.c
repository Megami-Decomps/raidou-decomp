#include "shared/basic/dds3WorldBasic/decls.h"

void dds3DeleteScene(dds3Basic_t *scene)
{
    dds3WorldBasic_t *world;

    if (scene == NULL) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    if (world->firstscene == scene) {
        world->firstscene = scene->next;
    }
    if (world->lastscene == scene) {
        world->lastscene = scene->prev;
    }
    if (world->activescene == scene) {
        world->activescene = NULL;
    }
    if (world->controlscene == scene) {
        world->controlscene = NULL;
    }
    dds3DeleteBasic(scene);
}
