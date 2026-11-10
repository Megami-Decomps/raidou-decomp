#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SCENE_FOG_PARA(void)
{
    s32 frames;
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    frames = scrGetIntPara(0);
    x = (f32)(u32)scrGetIntPara(1);
    y = (f32)(u32)scrGetIntPara(3);
    z = (f32)(u32)scrGetIntPara(2);
    w = (f32)(u32)scrGetIntPara(4);
    dds3SetFogPara(frames, x, y, z, w);
    return 1;
}
