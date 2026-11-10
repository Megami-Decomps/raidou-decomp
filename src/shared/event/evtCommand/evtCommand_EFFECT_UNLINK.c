#include "shared/event/evtCommand/decls.h"

u32 evtCommand_EFFECT_UNLINK(void)
{
    dds3Basic_t *basic;

    basic = evtSearchBasic(7, scrGetIntPara(0));
    if (basic == NULL) {
        return 1;
    }
    dds3EffectObjectBasicUnlinkObject(basic);
    return 1;
}
