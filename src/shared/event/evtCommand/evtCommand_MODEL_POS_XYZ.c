#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_POS_XYZ(void)
{
    sdfVector4_t pos;
    void *scene;
    s32 key;
    void *unit;

    memset(&pos, 0, 0x10);
    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return 1;
    }
    pos.x = scrGetFloatPara(1);
    pos.y = scrGetFloatPara(2);
    pos.z = scrGetFloatPara(3);
    dds3SetPosPoseSet(unit, (u128 *)&pos);
    return 1;
}
