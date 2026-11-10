#include "shared/event/evtCommand/decls.h"

u32 evtCommand_EFFMG1_LOAD(void)
{
    dds3Basic_t *basic;

    basic = dds3AddMG12EffectObjectBasic(1, scrGetStrPara(0));
    if (basic == NULL) {
        printf(evtMsgEffmg1LoadHint);
        printf(evtMsgNameNotFound, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}
