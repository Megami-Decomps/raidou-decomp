#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_S(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_S);
    pwork->CP++;
    return CODE_RET_NEXT;
}
