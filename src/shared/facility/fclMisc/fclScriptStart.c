#include "shared/facility/fclMisc/decls.h"

s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID)
{
    saddr_t handle;

    if (pPID != NULL)
    {
        fclScriptForcedEnd(*(u8 **)pPID);
    }
    handle = EE_ADDR_S(scrStartScript2(0x7D0, EE_ADDR_AS(void *, pScriptData), StartNo));
    fclClearFlag(0);
    if (pPID != NULL)
    {
        *(s32 *)pPID = handle;
    }
    return handle;
}
