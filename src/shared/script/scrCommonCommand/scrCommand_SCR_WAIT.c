#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SCR_WAIT(void)
{
    return dds3ExistProcess(EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0))) == 0;
}
