#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PushLIX(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->LocalInt[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}
