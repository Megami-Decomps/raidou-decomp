#include "shared/kernel/dds3KernelDraw/decls.h"

/* vu0 routine: stage an immediate or interpolated ambient color. */
void dds3SetAmbientColor(s32 frame, f32 *col)
{
    u32 baseColorWords[4];
    u32 targetColorWords[4];
    u32 packedColor;

    if (frame == 0) {
        dds3DrawFlag &= ~0x100;
        SDF_VU0_LOADVECTOR(col);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&dds3LightParam.ambient);
    } else {
        ambientmax = SDF_FRAMES(frame);
        ambientnow = 0;
        dds3DrawFlag |= 0x100;
        SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
        MMI_RGBA_PACK_128(packedColor);
        baseColorWords[0] = packedColor;
        ambientbase = packedColor;
        SDF_VU0_LOADVECTOR(col);
        SDF_VU0_VECTOR_W_SET_ONE();
        MMI_RGBA_PACK_128(packedColor);
        targetColorWords[0] = packedColor;
        ambienttarget = packedColor;
    }
}
