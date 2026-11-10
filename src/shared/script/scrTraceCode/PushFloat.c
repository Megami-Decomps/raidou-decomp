#include "shared/script/scrTraceCode/decls.h"

void PushFloat(scrProcessWork_s *pw, f32 value)
{
    pw->StackType[pw->SP] = SCR_STACK_TYPE_FLOAT;
    pw->StackData[pw->SP].f = value;
    pw->SP++;
}
