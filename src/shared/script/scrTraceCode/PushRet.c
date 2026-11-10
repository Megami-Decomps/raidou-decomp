#include "shared/script/scrTraceCode/decls.h"

void PushRet(scrProcessWork_s *pwork, s32 cp)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_RET;
    pwork->StackData[pwork->SP].i = cp;
    pwork->SP++;
}
