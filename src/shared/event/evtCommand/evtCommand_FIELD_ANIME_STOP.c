#include "shared/event/evtCommand/decls.h"

s32 evtCommand_FIELD_ANIME_STOP(void)
{
    s32 value;

    value = scrGetIntPara(0);
    evtStageAnimeControl(1, value);
    return 1;
}
