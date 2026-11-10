#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetMultiBlurPara(f32 rotate, f32 scale, s32 blend)
{
    mulblurtarget.rotation = rotate;
    mulblurtarget.scale = scale;
    mulblurtarget.blend = blend;
}
