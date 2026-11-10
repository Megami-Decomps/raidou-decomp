#include "shared/effect/effPCPScatter/decls.h"

f32 *dds3WhirlwindPolyListGetVertex(dds3WhirlwindPolyList_tag *hPoly, s32 index) {
    return hPoly->points + index * hPoly->vectorsPerParticle * 4;
}
