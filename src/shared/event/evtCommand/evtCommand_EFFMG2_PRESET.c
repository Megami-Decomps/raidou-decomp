#include "shared/event/evtCommand/decls.h"

u32 evtCommand_EFFMG2_PRESET(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3EffectObjectPresetMG12(basic);
    }
    return 1;
}
