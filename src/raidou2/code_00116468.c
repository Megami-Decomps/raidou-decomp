#include "common.h"
#include "scrTraceCode.h"

INCLUDE_ASM(const s32, "code_00116468", scrGetCodePointer);

INCLUDE_ASM(const s32, "code_00116468", scrSetCodePointer);

INCLUDE_ASM(const s32, "code_00116468", scrGetScriptTimer);

s32 scrGetCommandTimer(void)
{
    return nowpwork->CommandTimer;
}

s32 scrGetMessageHandle(void)
{
    return nowpwork->MesHandle;
}

scrProcessWork_s *scrGetScriptHandle(void)
{
    return nowpwork;
}

INCLUDE_ASM(const s32, "code_00116468", scrGetData);
