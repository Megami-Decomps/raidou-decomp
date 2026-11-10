#include "shared/model/mdlManager/decls.h"

void mdlRegistMotion(s32 aMajor, s32 aMinor, u32 aFlags, s32 aFirstMotion, s32 aGroup, s32 aPlayGroup,
                     sdfMotionPlayBook_t *aData, sdfMemHandle_t *aHandle)
{
    dds3ModelResrc_t *owner;
    dds3ModelResrcPlayBook_t *slot;

    owner = mdlSearchResrc(aMajor, aMinor);
    mdlUnRegistResrcMotion(owner, aGroup);
    slot = &owner->playbooks[aGroup];
    slot->playGroup = aPlayGroup;
    slot->firstMotion = aFirstMotion;
    slot->ptr = aData;
    slot->handle = aHandle;
    slot->flags = 0;
    if (aFlags & 0x100)
    {
        slot->flags = 1;
    }
    if (aFlags & 0x200)
    {
        slot->flags |= 2;
    }
}
