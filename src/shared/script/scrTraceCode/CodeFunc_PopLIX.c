#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PopLIX(scrProcessWork_s *pwork)
{
    pwork->LocalInt[pwork->Code[pwork->CP].s.s1] = PopInt(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}
