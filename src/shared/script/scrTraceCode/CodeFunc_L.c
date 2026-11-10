#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_L(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_L);
    pwork->CP++;
    return CODE_RET_NEXT;
}
