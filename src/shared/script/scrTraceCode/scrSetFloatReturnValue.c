#include "shared/script/scrTraceCode/decls.h"

void scrSetFloatReturnValue(f32 f)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_FLOAT;
    nowpwork->StackData[27].f = f;
}
