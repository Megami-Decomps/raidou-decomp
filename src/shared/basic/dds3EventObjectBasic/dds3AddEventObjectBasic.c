#include "shared/basic/dds3EventObjectBasic/decls.h"

dds3Basic_t *dds3AddEventObjectBasic(s32 serialID, const sdfVector4_t *pos,
                                   const sdfVector4_t *rot)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_EVENTOBJECT);

    basic->serialID = serialID;
    dds3SetPosPoseSet(basic, pos);
    dds3SetRotPoseSet(basic, rot);
    dds3SetRadiusPoseSet(basic->poseset);
    return basic;
}
