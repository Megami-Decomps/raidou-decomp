#include "shared/kernel/dds3SequenceList/decls.h"

s32 CheckBootMc(void)
{
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}
