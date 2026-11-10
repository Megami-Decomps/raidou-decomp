#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_SCL_XYZ(void)
{
    sdfVector4_t scl;
    void *scene;
    s32 key;
    void *unit;

    memset(&scl, 0, 0x10);
    scl.w = 1.0f;
    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return 1;
    }
    scl.x = scrGetFloatPara(1);
    scl.y = scrGetFloatPara(2);
    scl.z = scrGetFloatPara(3);
    dds3SetSclPoseSet(unit, (u128 *)&scl);
    return 1;
}
