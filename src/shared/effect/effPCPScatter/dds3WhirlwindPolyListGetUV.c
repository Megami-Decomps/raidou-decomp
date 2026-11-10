#include "shared/effect/effPCPScatter/decls.h"

f32 *dds3WhirlwindPolyListGetUV(dds3WhirlwindPolyList_tag *hPoly, s32 index) {
    return hPoly->uv + index * hPoly->vectorsPerParticle * 2;
}
