#include "shared/script/scrTraceCode/decls.h"

void PushInt(scrProcessWork_s *pwork, s32 data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_INT;
    pwork->StackData[pwork->SP].i = data;
    pwork->SP++;
}
