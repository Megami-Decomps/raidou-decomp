#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_SE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SE);
    pwork->CP++;
    return CODE_RET_NEXT;
}
