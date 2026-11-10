#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_Mul(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_MUL);
    pwork->CP++;
    return CODE_RET_NEXT;
}
