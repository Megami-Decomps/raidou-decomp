#include "shared/interface/itfMesManager/decls.h"

s32 itfMesMngWindowType(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngChangeWindowType(window, scrGetIntPara(0), 0);
    return 1;
}
