#include "shared/kernel/dds3KernelDraw/decls.h"

/* vu0 routine: stage an immediate or interpolated packed color. */
void dds3SetFogColor(s32 frame, f32 *col)
{
    u32 targetColorWords[4];
    u32 packedTargetColor;

    SDF_VU0_LOADVECTOR(col);
    SDF_VU0_VECTOR_W_SET_ZERO();
    MMI_RGBA_PACK_255(packedTargetColor);
    targetColorWords[0] = packedTargetColor;
    if (frame == 0) {
        dds3DrawFlag &= ~0x200;
        dds3LightParam.fog.color = targetColorWords[0];
    } else {
        dds3DrawFlag |= 0x200;
        fogrgbmax = SDF_FRAMES(frame);
        fogrgbtarget = targetColorWords[0];
        fogrgbbase = dds3LightParam.fog.color;
        fogrgbnow = 0;
    }
}
