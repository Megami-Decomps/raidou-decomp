#include "shared/model/mdlManager/decls.h"

dds3ModelResrc_t *mdlWaitLoadEnd(s32 aMajor, s32 aMinor)
{
    while (mdlCheckLoading(aMajor, aMinor))
    {
        fileManager();
    }
    return mdlSearchResrc(aMajor, aMinor);
}
