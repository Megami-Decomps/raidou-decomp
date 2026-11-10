#include "shared/event/evtCommand/decls.h"

s32 evtCommand_EFFECT_ON(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        evtEffectLoopControl(basic, 1);
        dds3EffectObjectBasicStatOn(basic, 1);
    }
    return 1;
}
