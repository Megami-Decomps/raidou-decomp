#include "shared/script/scrTraceCode/decls.h"

void scrSetIntReturnValue(s32 i)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_INT;
    nowpwork->StackData[27].i = i;
}
