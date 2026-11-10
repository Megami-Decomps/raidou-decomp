#include "shared/mc/mcManager/decls.h"

McPollResult sdfMcFuncMkdir1(void)
{
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return pollResult;
        }
        if (commandResult < MC_SDK_EXTENDED_ERROR_LIMIT) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}
