#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_BLUR(void)
{
    s32 frames;
    s32 alpha;

    frames = scrGetIntPara(0);
    alpha = scrGetIntPara(1);
    dds3SetBlur(frames, alpha, scrGetIntPara(2));
    return 1;
}
