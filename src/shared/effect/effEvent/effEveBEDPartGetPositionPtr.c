#include "shared/effect/effEvent/decls.h"

/* vu0 routine: place one part relative to the record's position and facing. */
void effEveBEDPartGetPositionPtr(effEveBEDParams_t *pParams, effBTLAimParams_t *pPosPtr) {
    sdfVector4_t out;
    sdfVector4_t offset;
    sdfVector4_t position;
    s32 range = pPosPtr->rangeOverride;
    u8 dir = pPosPtr->directionMode;
    u8 mode = pPosPtr->aimMode;
    f32 radius;
    f32 half;
    f32 y;
    f32 zero;

    if (range == 0) {
        radius = pParams->r;
    } else {
        radius = (f32)range;
    }
    half = pParams->h * 0.5f;
    SDF_VU0_COPYVECTOR(&position, pParams);
    if (mode == 5) {
        if (dir == 8 || dir == 0xA) {
            y = -1.0f;
            if (range != 0) {
                y = -radius;
            }
        } else {
            y = -1.0f;
        }
    } else {
        y = position.y - effEveBEDModeScale[mode] * half;
        if (dir == 8 || dir == 0xA) {
            if (range != 0) {
                y -= radius;
            }
        }
    }
    if (dir == 9 || mode == 4) {
        if (half < radius) {
            offset.z = -radius;
        } else {
            offset.z = -half;
        }
        zero = 0.0f;
        offset.y = zero;
        offset.x = zero;
        position.y = y;
        LookAtMatrix(&position, aViewEye, aViewDown);
        ReverseMatrix();
        SDF_VU0_LOADVECTOR(&offset);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&position);
        SDF_VU0_ADDVECTOR();
        return;
    }
    if (dir == 8 || dir == 0xA) {
        out.x = position.x;
        out.y = y;
        out.z = position.z;
    } else {
        offset.x = 0.0f;
        offset.z = 1.0f;
        offset.y = 0.0f;
        SDF_VU0_LOADVECTOR(pParams->rot);
        ToMatrix();
        SetRotateMatrixY_B(effEveBEDDirAngle[dir]);
        MultMatrixNoArg();
        SDF_VU0_LOADVECTOR(&offset);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(&offset);
        out.x = position.x + radius * offset.x;
        out.y = y + radius * offset.y;
        out.z = position.z + radius * offset.z;
    }
    SDF_VU0_LOADVECTOR(&out);
}
