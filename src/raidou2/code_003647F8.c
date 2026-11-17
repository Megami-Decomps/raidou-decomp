#include "common.h"
#include "fldNme.h"

s32 titleStartSub(s32 page)
{
    return titleStartSubData[page][0];
}

s32 titleStartSub2(s32 page)
{
    return titleStartSubData[page][1];
}

INCLUDE_ASM(const s32, "code_003647F8", titleFunc2);
