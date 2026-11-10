#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolyRingExp2PartRotate(effPCPFlashPolyRingExp2_tag *pFlashPoly, s32 number)
{
    effPCPFlashPolyRingExp2Part_tag *part;

    part = &pFlashPoly->pPart[number];
    part->rotate += SDF_FRAME_STEP(pFlashPoly->angularStep);
}
