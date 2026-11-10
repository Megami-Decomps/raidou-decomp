#include "shared/interface/itfMesManager/decls.h"

s32 itfMesMngSelectDefKey(void)
{
    s32 window = scrGetMessageHandle();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    itfMesMngSetSelectDefKey(window, first, second);
    return 1;
}
