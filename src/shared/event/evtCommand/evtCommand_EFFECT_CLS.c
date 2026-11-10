#include "shared/event/evtCommand/decls.h"

u32 evtCommand_EFFECT_CLS(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3DeleteSceneBasicData(basic);
    }
    return 1;
}
