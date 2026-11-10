#include "shared/effect/effSpline/decls.h"

void dds3BuildSplinePoints(f32 *tangents, f32 *points, u32 count) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(count * sizeof(f32));
    f32 *rhs = (f32 *)sdfMemLock(handle);
    f32 (*pts)[3] = (f32 (*)[3])points;
    s32 component;
    s32 i;

    for (component = 0; component < 3; component++) {
        rhs[0] = (pts[1][component] - pts[0][component]) * 3.0f;
        rhs[count - 1] = (pts[count - 1][component] - pts[count - 2][component]) * 3.0f;
        for (i = 1; i <= (s32)count - 2; i++) {
            rhs[i] = ((pts[i][component] - pts[i - 1][component]) +
                      (pts[i + 1][component] - pts[i][component])) * 3.0f;
        }
        SolveTridiagonal(tangents, rhs, count);
        tangents += count;
    }
    sdfMemFreeDirect(handle);
}
