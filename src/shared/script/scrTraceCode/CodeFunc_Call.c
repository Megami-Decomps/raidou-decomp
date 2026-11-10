#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_Call(scrProcessWork_s *pwork)
{
    PushRet(pwork, pwork->CP);
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}
