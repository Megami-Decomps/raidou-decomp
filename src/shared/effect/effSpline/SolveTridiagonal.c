#include "shared/effect/effSpline/decls.h"

void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count) {
    s32 byteCount = count * sizeof(f32);
    void *handle = sdfMemAllocDirect(byteCount);
    f32 *upper;
    s32 i;
    f32 denominator;
    f32 *upperEnd;
    f32 *solutionEnd;
    f32 *rhsEnd;

    count -= 2;
    upper = (f32 *)sdfMemLock(handle);
    denominator = 2.0f;

    solution[0] = rhs[0] / denominator;
    for (i = 1; i <= count; i++) {
        upper[i] = 1.0f / denominator;
        denominator = 4.0f - upper[i];
        solution[i] = (rhs[i] - solution[i - 1]) / denominator;
    }
    upperEnd = (f32 *)((u8 *)upper + byteCount);
    solutionEnd = (f32 *)((u8 *)solution + byteCount);
    rhsEnd = (f32 *)((u8 *)rhs + byteCount);
    upperEnd[-1] = 1.0f / denominator;
    denominator = 2.0f - upperEnd[-1];
    solutionEnd[-1] = (rhsEnd[-1] - solutionEnd[-2]) / denominator;
    for (i = count; i >= 0; i--) {
        solution[i] -= upper[i + 1] * solution[i + 1];
    }
    sdfMemFreeDirect(handle);
}
