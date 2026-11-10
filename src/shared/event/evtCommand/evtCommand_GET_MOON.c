#include "shared/event/evtCommand/decls.h"

s32 evtCommand_GET_MOON(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon());
    return 1;
}
