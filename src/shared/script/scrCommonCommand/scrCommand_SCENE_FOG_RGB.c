#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SCENE_FOG_RGB(void)
{
    scrVecW_t v;

    v.x = scrGetFloatPara(1);
    v.y = scrGetFloatPara(2);
    v.z = scrGetFloatPara(3);
    v.w = 0;
    dds3SetFogColor(scrGetIntPara(0), &v.x);
    return 1;
}
