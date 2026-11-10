#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_PLAYER(void)
{
    void *scene;
    s32 *basic;
    s32 key;

    scene = dds3GetActiveScene();
    basic = dds3GetActiveUnitSceneBasic(scene);
    key = -1;
    if (basic != NULL) {
        key = basic[1];
    }
    scrSetIntReturnValue(key);
    return 1;
}
