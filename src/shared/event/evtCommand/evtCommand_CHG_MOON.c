#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CHG_MOON(void)
{
    evtSetAgeOfMoon(scrGetIntPara(0));
    return 1;
}
