#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PushF(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushFloat(pwork, pwork->Code[pwork->CP].f);
    pwork->CP++;
    return CODE_RET_NEXT;
}
