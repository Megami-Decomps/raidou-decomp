#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere3PartRotate(effPCPFlashPolySphere3_tag *pFlashPoly, s32 number, void *pLook)
{
    effPCPFlashPolySphere3Part_tag *part;

    part = &pFlashPoly->parts[number];
    part->look_r += SDF_FRAME_STEP(part->rotate);
}
