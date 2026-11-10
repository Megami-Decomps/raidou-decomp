#include "shared/event/evtCommand/decls.h"

u32 evtCommand_EFFMG2_LOAD(void)
{
    dds3Basic_t *basic;

    basic = dds3AddMG12EffectObjectBasic(2, scrGetStrPara(0));
    if (basic == NULL) {
        printf(evtMsgEffmg2LoadHint);
        printf(evtMsgNameNotFound, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}
