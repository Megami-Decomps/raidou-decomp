#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_LE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_LE);
    pwork->CP++;
    return CODE_RET_NEXT;
}
