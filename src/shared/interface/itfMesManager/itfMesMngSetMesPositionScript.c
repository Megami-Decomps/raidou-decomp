#include "shared/interface/itfMesManager/decls.h"

s32 itfMesMngSetMesPositionScript(void)
{
    s32 window = scrGetMessageHandle();
    s32 x;
    s32 y;

    if (window < 0) {
        return 1;
    }
    x = scrGetIntPara(0);
    y = scrGetIntPara(1);
    itfMesMngSetMesPosition(window, x << 4, y << 3);
    return 1;
}
