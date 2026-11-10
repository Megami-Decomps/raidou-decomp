#include "shared/basic/dds3WorldBasic/decls.h"

dds3Basic_t *dds3AddScene(void)
{
    dds3WorldBasic_t *world;
    dds3Basic_t *scene;

    if (dds3TheWorld == NULL) {
        return NULL;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    scene = dds3AllocBasic(BASIC_TYPE_SCENE);
    if (scene == NULL) {
        return NULL;
    }
    if (world->lastscene == NULL) {
        world->firstscene = scene;
        world->lastscene = scene;
    } else {
        world->lastscene->next = scene;
        scene->prev = world->lastscene;
        world->lastscene = scene;
    }
    return scene;
}
