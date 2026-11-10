#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_Jump(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}
