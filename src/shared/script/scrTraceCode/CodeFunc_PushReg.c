#include "shared/script/scrTraceCode/decls.h"

s32 CodeFunc_PushReg(scrProcessWork_s *pwork)
{
    pwork->StackType[pwork->SP] = pwork->StackType[27];
    pwork->StackData[pwork->SP].i = pwork->StackData[27].i;
    pwork->SP++;
    pwork->CP++;
    return CODE_RET_NEXT;
}
