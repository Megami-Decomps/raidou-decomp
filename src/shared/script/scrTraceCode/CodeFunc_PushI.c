#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PushI(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushInt(pwork, pwork->Code[pwork->CP].i);
    pwork->CP++;
    return CODE_RET_NEXT;
}
