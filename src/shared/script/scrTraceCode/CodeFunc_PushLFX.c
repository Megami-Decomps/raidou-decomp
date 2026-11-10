#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PushLFX(scrProcessWork_s *pwork)
{
    PushFloat(pwork, pwork->LocalFloat[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}
