#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_End(scrProcessWork_s *pwork)
{
    if (pwork->SP == 0) {
        return CODE_RET_END;
    }

    pwork->CP = PopInt(pwork) + 1;
    return CODE_RET_NEXT;
}
