#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetRipBlurPara(s32 number, f32 intval, f32 rotate, f32 distor, f32 distor_s, f32 blend, s32 speed)
{
    if (number >= 0x29) {
        ripblurovercnt++;
        number = 0x28;
    }
    ripblurtarget.ripple.count = number;
    ripblurtarget.ripple.phaseStep = intval;
    ripblurtarget.ripple.spacing = rotate;
    ripblurtarget.ripple.uvAngle = distor;
    ripblurtarget.ripple.uvAmplitude = distor_s;
    ripblurtarget.ripple.angleStep = blend;
    ripblurtarget.ripple.blend = speed;
}
