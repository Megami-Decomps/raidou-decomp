#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetBlur2Para(f32 rotate, f32 scale, s32 blend)
{
    blur2target.rotation = rotate;
    blur2target.scale = scale;
    blur2target.blend = blend;
}
