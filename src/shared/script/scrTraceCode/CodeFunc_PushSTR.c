#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PushSTR(scrProcessWork_s *pwork)
{
    PushStr(pwork, pwork->StringData + pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}
