#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_MFILTER_ON(void)
{
    dds3MFilterOn(scrGetIntPara(0));
    return 1;
}
