#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphereExpPartRotate(effPCPFlashPolySphereExp_tag *pFlashPoly, s32 number, void *pLook)
{
    effPCPFlashPolySphereExpPart_tag *part;

    part = &pFlashPoly->parts[number];
    part->look_r += SDF_FRAME_STEP(part->rotate);
}
