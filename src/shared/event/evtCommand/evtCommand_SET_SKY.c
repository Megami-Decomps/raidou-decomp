#include "shared/event/evtCommand/decls.h"

u32 evtCommand_SET_SKY(void)
{
    s32 r;
    s32 g;

    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    r = scrGetIntPara(0);
    g = scrGetIntPara(1);
    fldSkySetFilter(r, g, scrGetIntPara(2));
    return 1;
}
