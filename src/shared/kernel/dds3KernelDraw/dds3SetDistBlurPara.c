#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetDistBlurPara(s32 number, s32 width, s32 intval, f32 rotate,
                         f32 distor, f32 blend, s32 speed)
{
    if (number >= 101)
{
        distblurovercnt++;
        number = 100;
    }
    distblurtarget.count = number;
    distblurtarget.size = width;
    distblurtarget.delaySpread = intval;
    distblurtarget.angleStep = rotate;
    distblurtarget.uvAngle = distor;
    distblurtarget.uvAmplitude = blend;
    distblurtarget.blend = speed;
}
