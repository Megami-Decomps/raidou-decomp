#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySpherePartRotate(effPCPFlashPolySphere_tag *pFlashPoly, s32 number, void *pLook)
{
    effPCPFlashPolySpherePart_tag *part = &pFlashPoly->pPart[number];
    sdfVector4_t position;

    position.x = part->position[0];
    position.y = part->position[1];
    position.z = part->position[2];
    SetRotateMatrixAxis(SDF_FRAME_STEP(part->rotate), (const sdfVector3_t *)pLook);
    SDF_VU0_LOADVECTOR(&position);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(&position);
    part->position[0] = position.x;
    part->position[1] = position.y;
    part->position[2] = position.z;
}
