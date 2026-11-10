#include "shared/event/evtCommand/decls.h"

u32 evtCommand_EFFBED_LOAD(void)
{
    evtVec_t pos QWORD_ALIGNED;
    evtVec_t scale QWORD_ALIGNED;
    dds3Basic_t *basic;

    memset(&pos, 0, sizeof(pos));
    memset(&scale, 0, sizeof(scale));
    scale.w = 1.0f;
    basic = dds3AddBEDEffectObjectBasic(scrGetStrPara(0), &pos, &scale);
    if (basic == NULL) {
        printf(evtMsgEffectLoadHint);
        printf(evtMsgNameNotFound, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}
