#include "shared/sdf/sdfFile/decls.h"

s32 sdfFileRead(void *aHandle, void *aPtr, s32 aLeng)
{
    sdfFileCmdRead(aHandle, aPtr, aLeng);
    return WaitSema(sdfFileSemaId);
}
