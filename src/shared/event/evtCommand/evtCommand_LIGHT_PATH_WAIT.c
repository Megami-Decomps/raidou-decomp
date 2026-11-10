#include "shared/event/evtCommand/decls.h"

s32 evtCommand_LIGHT_PATH_WAIT(void)
{
    s32 stat;

    stat = evtCheckPathStat(evtSearchBasic(9, scrGetIntPara(0)));
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}
