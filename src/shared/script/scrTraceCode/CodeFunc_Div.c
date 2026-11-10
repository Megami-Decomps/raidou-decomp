#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_Div(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_DIV);
    pwork->CP++;
    return CODE_RET_NEXT;
}
