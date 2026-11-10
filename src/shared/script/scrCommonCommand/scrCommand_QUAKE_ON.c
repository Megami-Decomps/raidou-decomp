#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_QUAKE_ON(void)
{
    s32 frames;

    frames = scrGetIntPara(0);
    dds3SetQuake(frames, scrGetIntPara(1));
    return 1;
}
