#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_FILTER2_ON(void)
{
    dds3Filter2On(scrGetIntPara(0));
    return 1;
}
