#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_DIST_BLUR_PARA(void)
{
    s32 spread;
    s32 size;
    s32 count;
    s32 blendMode;
    s32 blendControl;
    f32 f4;
    f32 f3;
    f32 f5;

    blendMode = scrGetIntPara(6);
    switch (blendMode)
    {
    case 1:
        blendControl = 0x48;
        break;
    case 2:
        blendControl = 0x42;
        break;
    case 0:
    default:
        blendControl = 0x44;
        break;
    }
    count = scrGetIntPara(0);
    size = scrGetIntPara(1);
    spread = scrGetIntPara(2);
    f3 = scrGetFloatPara(3);
    f4 = scrGetFloatPara(4);
    f5 = scrGetFloatPara(5);
    dds3SetDistBlurPara(count, size, spread, f3, f4, f5, blendControl);
    return 1;
}
