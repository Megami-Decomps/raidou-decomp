#include "shared/model/mdlManager/decls.h"

s32 mdlGetNumOfMinor(s32 aMajor)
{
    return modelFileKindTable[aMajor].count;
}
