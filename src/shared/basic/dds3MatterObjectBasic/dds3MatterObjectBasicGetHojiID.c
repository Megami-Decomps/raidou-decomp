#include "shared/basic/dds3MatterObjectBasic/decls.h"

u32 dds3MatterObjectBasicGetHojiID(u32 resID) {
    dds3Basic_t *scene;
    dds3Basic_t *basic;
    dds3MatterObjectBasicWorkData_t *work;

    scene = dds3GetControlScene();
    basic = dds3SearchSceneBasic(scene, resID, BASIC_TYPE_MATTEROBJECT);
    work = basic->workdata;
    return work->hojiID;
}
