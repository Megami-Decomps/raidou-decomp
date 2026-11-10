#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SOFTEN_BIAS(void)
{
    s32 frames;

    frames = scrGetIntPara(0);
    dds3SetSoftenBias(frames, scrGetFloatPara(1), scrGetFloatPara(2));
    return 1;
}
