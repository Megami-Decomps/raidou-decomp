#include "shared/effect/effPCPBezier/decls.h"

void dds3BezierTrackPolyCreate(dds3BezierTrackPoly_tag *face, u32 *split) {
    f32 t = 0.0f;
    u32 count = face->point / 3;
    u32 alphaCount = count >> 1;
    f32 step = 1.0f / count;
    f32 alpha = 0.0f;
    f32 alphaStep = 1.0f / alphaCount;
    u32 *colors = face->pVC;
    u32 innerStart = split[0] & 0xFFFFFF;
    u32 innerEnd = split[2] & 0xFFFFFF;
    u32 outerStart = split[1];
    u32 outerEnd = split[3];
    u32 i = 0;

    if (count != 0) {
        do {
            u32 alphaMask = (u32)(alpha * 2147483648.0f) & 0xFF000000;

            colors[0] = effColorBlend(outerEnd, outerStart, t);
            colors[1] = effColorBlend(innerEnd, innerStart, t) | alphaMask;
            colors[2] = colors[0];
            t += step;
            if (i < alphaCount) {
                alpha += alphaStep;
            }
            i++;
            colors += 3;
        } while (i < count);
    }
}
