#include "shared/basic/dds3MatterObjectBasic/decls.h"

dds3Basic_t *dds3AddMatterObjectBasic(s32 serialID, f32 *pos, f32 *rot) {
    sdfVector4_t zero;
    dds3Basic_t *basic;

    memset(&zero, 0, sizeof(zero));
    zero.w = 1.0f;
    basic = dds3AddSceneBasicData(BASIC_TYPE_MATTEROBJECT);
    basic->serialID = serialID;
    dds3ObjectBaseCreateMover(basic);
    dds3SetRotPoseSet(basic, rot);
    dds3SetPosPoseSet(basic, &zero);
    dds3SetRadiusPoseSet(basic->poseset);
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[0] = pos[0];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[1] = pos[1];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[2] = pos[2];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[3] = pos[3];
    return basic;
}
