#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_Or(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_OR);
    pwork->CP++;
    return CODE_RET_NEXT;
}
