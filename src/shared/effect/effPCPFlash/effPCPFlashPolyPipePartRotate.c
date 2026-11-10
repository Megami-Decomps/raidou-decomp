#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolyPipePartRotate(effPCPFlashPolyPipe_tag *pFlashPoly, s32 number)
{
    effPCPFlashPolyPipePart_tag *part;

    part = &pFlashPoly->parts[number];
    part->rotate += SDF_FRAME_STEP(pFlashPoly->increment);
}
