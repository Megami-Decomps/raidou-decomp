#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SEL_JMP(void)
{
    s32 argumentIndex;
    s32 labelIndex;

    argumentIndex = scrGetIntPara(0);
    if (argumentIndex < 0)
    {
        return 1;
    }
    labelIndex = scrGetIntPara(argumentIndex + 1);
    if (labelIndex < 0)
    {
        return 1;
    }
    scrSetCodePointer(scrGetLabelCodeAddress(labelIndex));
    return 1;
}
