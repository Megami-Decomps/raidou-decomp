#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_Add(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_ADD);
    pwork->CP++;
    return CODE_RET_NEXT;
}
