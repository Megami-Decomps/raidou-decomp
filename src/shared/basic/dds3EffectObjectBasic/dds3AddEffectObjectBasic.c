#include "shared/basic/dds3EffectObjectBasic/decls.h"

dds3Basic_t *dds3AddEffectObjectBasic(u32 serialID, void *aPos, void *aRot) {
    dds3Basic_t *obj;
    dds3EffectObjectBasicWorkData_t *data;

    obj = (dds3Basic_t *)dds3AddSceneBasicData(EFF_WORLD_KIND_EFFECT_OBJECT);
    if (obj == NULL) {
        return NULL;
    }
    obj->serialID = serialID;
    dds3ObjectBaseCreateMover(obj);
    dds3SetPosPoseSet(obj, aPos);
    dds3SetRotPoseSet(obj, aRot);
    dds3SetRadiusPoseSet(obj->poseset);
    data = obj->workdata;
    data->stat = 0;
    data->type = 0;
    data->handle = NULL;
    data->linkbasic = NULL;
    data->linkindex = 0;
    data->linkbasictype = 0;
    return obj;
}
