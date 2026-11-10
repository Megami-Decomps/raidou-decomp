#include "shared/sdf/sdfSound/decls.h"

s32 sdfSndSendSyncCmd(s32 aCmd, s32 aParam, void *aExDataPtr, s32 aExDataSize)
{
    s32 handle = sdfSndSendCmd(aCmd, aParam, aExDataPtr, aExDataSize);

    sdfSndSyncCmd(handle);
    return handle;
}
