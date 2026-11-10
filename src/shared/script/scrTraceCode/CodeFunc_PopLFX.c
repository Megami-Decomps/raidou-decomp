#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PopLFX(scrProcessWork_s *pwork)
{
    pwork->LocalFloat[pwork->Code[pwork->CP].s.s1] = PopFloat(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}
