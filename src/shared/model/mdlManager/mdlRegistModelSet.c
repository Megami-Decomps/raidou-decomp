#include "shared/model/mdlManager/decls.h"

void mdlRegistModelSet(s32 aMajor, s32 aMinor, s32 aFlag, mdlSetDef_t *aSet)
{
    dds3ModelResrc_t *entity;

    mdlRegist(aMajor, aMinor, aFlag, aSet->texList, aSet->modelDefPtr, aSet->modelDefHandle);
    if (aSet->playbookPtr != NULL)
    {
        mdlRegistMotion(aMajor, aMinor, aFlag, 0, 0, 0, aSet->playbookPtr, aSet->playbookHandle);
    }
    if (aSet->effectData != NULL)
    {
        entity = mdlSearchResrc(aMajor, aMinor);
        entity->effectData = aSet->effectData;
        entity->effectMemHandle = aSet->effectMemHandle;
        entity->effectResrcList = aSet->effectResrcList;
    }
}
