#include "shared/mc/mcManager/decls.h"

McPollResult sdfMcFuncFormat1(void)
{
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult != MC_SYNC_COMPLETE) {
        return MC_POLL_PENDING;
    }
    if (commandResult == MC_SDK_COMMAND_SUCCESS) {
        return pollResult;
    }
    return MC_POLL_ERROR;
}
