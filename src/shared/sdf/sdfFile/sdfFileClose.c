#include "shared/sdf/sdfFile/decls.h"

void sdfFileClose(void *aHandle)
{
    sdfFileCmdClose(aHandle);
    WaitSema(sdfFileSemaId);
    sdfFileCmdFree(aHandle);
}
