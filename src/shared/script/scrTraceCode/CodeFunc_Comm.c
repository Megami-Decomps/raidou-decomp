#include "shared/script/scrTraceCode/decls.h"

/* A pending command yields without consuming its arguments. */
s32 CodeFunc_Comm(scrProcessWork_s *pwork)
{
    s32 commandCP = pwork->CP;
    s32 commandIndex = pwork->Code[commandCP].s.s1;

    nowpwork = pwork;
    if (scrCommandTable[commandIndex].comm() == 0) {
        return CODE_RET_SYNC;
    }
    pwork->SP -= scrCommandTable[commandIndex].paranum;
    if (commandCP == pwork->CP) {
        pwork->CP = commandCP + 1;
    }
    return CODE_RET_NEXT;
}
