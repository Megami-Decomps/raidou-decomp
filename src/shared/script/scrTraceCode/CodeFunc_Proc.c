#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_Proc(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}
