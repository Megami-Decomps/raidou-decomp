#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolyRingPartRotate(effPCPFlashPolyRing_tag *pFlashPoly, s32 number)
{
    effPCPFlashPolyRingExp2Part_tag *part;

    part = &pFlashPoly->parts[number];
    part->rotate += SDF_FRAME_STEP(pFlashPoly->angularStep);
}
