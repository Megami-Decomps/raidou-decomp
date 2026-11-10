#include "shared/facility/fclMisc/decls.h"

void fclScriptForcedEnd(u8 *PID)
{
    if (PID != NULL && dds3GetProcessStat(PID) != 0) {
        dds3KillProcess(PID, 0);
        PID = NULL;
    }
}
