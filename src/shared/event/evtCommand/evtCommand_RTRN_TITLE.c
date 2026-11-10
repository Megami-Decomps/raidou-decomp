#include "shared/event/evtCommand/decls.h"

s32 evtCommand_RTRN_TITLE(void)
{
    dds3AdminiCallSequence(2, 0, 0, 0);
    return 1;
}
