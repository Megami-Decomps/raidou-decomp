#include "shared/script/scrTraceCode/decls.h"

s32 scrGetProcedureCodeAddress(s32 procindex)
{
    return nowpwork->ProcTable[procindex].CodeAddress;
}
