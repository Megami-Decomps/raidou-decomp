#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SCR_KILL(void)
{
    s32 taskHandle;

    taskHandle = scrGetIntPara(0);
    if (dds3ExistProcess(EE_ADDR_AS(dds3ProcessID_t *, taskHandle)) == 0)
    {
        return 1;
    }
    dds3KillProcess(EE_ADDR_AS(dds3ProcessID_t *, taskHandle), 1);
    return 1;
}
