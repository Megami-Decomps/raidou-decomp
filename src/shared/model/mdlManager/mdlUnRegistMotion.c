#include "shared/model/mdlManager/decls.h"

void mdlUnRegistMotion(s32 aMajor, s32 aMinor, s32 aGroup)
{
    dds3ModelResrc_t *entity;

    entity = mdlSearchResrc(aMajor, aMinor);
    mdlUnRegistResrcMotion(entity, aGroup);
}
