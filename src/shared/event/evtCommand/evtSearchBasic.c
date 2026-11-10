#include "shared/event/evtCommand/decls.h"

void *evtSearchBasic(s32 type, s32 serial)
{
    void *scene;

    scene = dds3GetActiveScene();
    return dds3SearchSceneBasic(scene, serial, type);
}
