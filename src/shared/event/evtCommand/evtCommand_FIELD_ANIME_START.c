#include "shared/event/evtCommand/decls.h"

s32 evtCommand_FIELD_ANIME_START(void)
{
    s32 value;

    value = scrGetIntPara(0);
    evtStageAnimeControl(0, value);
    return 1;
}
