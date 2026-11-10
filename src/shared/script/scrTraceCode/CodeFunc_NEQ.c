#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_NEQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_NEQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}
