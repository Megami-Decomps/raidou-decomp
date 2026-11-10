#include "shared/effect/effPCPScatter/decls.h"

u32 dds3WhirlwindPolyListGetABGR(dds3WhirlwindPolyList_tag *hPoly, s32 index) {
    return hPoly->colors[index];
}
