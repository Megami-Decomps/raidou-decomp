#include "shared/sdf/sdfFile/decls.h"

u32 sdfFileSeek(void *aHandle, s32 aPos, s32 aWhence)
{
    sdfFileCmdSeek(aHandle, aPos, aWhence);
    WaitSema(sdfFileSemaId);
    return sdfFileSeekWork;
}
