#include "shared/script/scrTraceCode/decls.h"

/* Continue until an instruction completes the procedure or yields a tick. */
s32 scrTraceCode(scrProcessWork_s *pwork)
{
    s32 result;

    while (1) {
        result = codefunc[pwork->Code[pwork->CP].s.s0](pwork);
        if (result == CODE_RET_END) {
            pwork->CommandTimer = 0;
            return SCR_RESULT_END;
        }
        if (result == CODE_RET_SYNC) {
            break;
        }
        if (result == CODE_RET_NEXT) {
            pwork->CommandTimer = 0;
        }
    }
    pwork->CommandTimer++;
    pwork->ScriptTimer++;
    return SCR_RESULT_CONTINUE;
}
