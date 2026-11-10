#include "shared/script/scrTraceCode/decls.h"

u8 *scrGetStrPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;
    s32 stackType = (s8)pw->StackType[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return EE_ADDR_AS(u8 *, pw->StackData[stackIndex].i);
            }
        }
    }
    return NULL;
}
