#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_And(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_AND);
    pwork->CP++;
    return CODE_RET_NEXT;
}
