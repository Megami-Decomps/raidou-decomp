#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_RND(void)
{
    scrSetIntReturnValue(dds3GetRandIntA(0, scrGetIntPara(0)) + 1);
    return 1;
}
