#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far)
{
    if (frame == 0) {
        dds3DrawFlag &= ~0x400;
        dds3FogState.fogMax = max;
        dds3FogState.fogMin = min;
        dds3FogState.fogNear = near;
        dds3FogState.fogFar = far;
    } else {
        f32 oldMax = dds3FogState.fogMax;
        f32 oldMin = dds3FogState.fogMin;
        f32 oldNear = dds3FogState.fogNear;
        f32 oldFar = dds3FogState.fogFar;
        dds3DrawFlag |= 0x400;
        fogparamax = SDF_FRAMES(frame);
        dds3FogParam.fogMax = oldMax;
        dds3FogParam.fogMin = oldMin;
        dds3FogParam.fogNear = oldNear;
        dds3FogParam.fogFar = oldFar;
        dds3FogColor.fogMax = max;
        dds3FogColor.fogMin = min;
        dds3FogColor.fogNear = near;
        dds3FogColor.fogFar = far;
        fogparanow = 0;
    }
}
