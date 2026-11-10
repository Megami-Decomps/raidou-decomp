#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PushIS(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}
