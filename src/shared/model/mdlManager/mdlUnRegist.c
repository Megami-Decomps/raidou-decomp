#include "shared/model/mdlManager/decls.h"

void mdlUnRegist(s32 aMajor, s32 aMinor)
{
    mdlUnRegistResrc(mdlSearchResrc(aMajor, aMinor));
}
