#include "shared/mc/mcManager/decls.h"

McPollResult sdfMcFuncClose1(void)
{
    s32 commandId;
    s32 commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}
