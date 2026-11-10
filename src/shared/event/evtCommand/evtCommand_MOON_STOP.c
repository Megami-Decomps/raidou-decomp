#include "shared/event/evtCommand/decls.h"

s32 evtCommand_MOON_STOP(void)
{
    evtDisableMoonCalc();
    return 1;
}
