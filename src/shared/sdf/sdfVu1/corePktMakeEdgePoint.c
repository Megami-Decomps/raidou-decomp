#include "shared/sdf/sdfVu1/decls.h"

/* vu0 routine: interpolate a clip vertex between two source rows, COP2 macro math only */
void corePktMakeEdgePoint(sdfCorePacketPointBuff_tag *out, sdfCorePacketPointBuff_tag *first, sdfCorePacketPointBuff_tag *second) {
    out->sourceA = first;
    out->sourceB = second;
    VU0_INTERPOLATE_CLIP_VERTEX(out, first, second);
}
