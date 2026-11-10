#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_Sub(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SUB);
    pwork->CP++;
    return CODE_RET_NEXT;
}
