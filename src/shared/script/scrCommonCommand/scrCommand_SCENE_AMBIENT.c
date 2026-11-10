#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SCENE_AMBIENT(void)
{
    scrVec4_t v;
    f32 x;
    f32 y;
    f32 z;

    x = scrGetFloatPara(1);
    SDF_VU0_SETVECTOR_X_BITS(x);
    y = scrGetFloatPara(2);
    SDF_VU0_SETVECTOR_Y_BITS(y);
    z = scrGetFloatPara(3);
    SDF_VU0_SETVECTOR_Z_BITS(z);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR_MEMOPS(v);
    dds3SetAmbientColor(scrGetIntPara(0), &v);
    return 1;
}
