#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_MUL_BLUR_PARA(void)
{
    u32 blendControl;
    f32 rotation;
    f32 scale;

    switch (scrGetIntPara(2))
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
    rotation = scrGetFloatPara(0);
    scale = scrGetFloatPara(1);
    dds3SetMultiBlurPara(rotation, scale, blendControl);
    return 1;
}
