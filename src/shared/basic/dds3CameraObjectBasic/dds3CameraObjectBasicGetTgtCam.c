#include "shared/basic/dds3CameraObjectBasic/decls.h"

/* vu0 routine: refresh the rotated vectors and copy target/eye to the caller. */
void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *pBasic, sdfVector4_t *tgtpos, sdfVector4_t *campos)
{
    dds3CameraObjectBasicWorkData_t *work = pBasic->workdata;
    dds3PoseSet_t *pose = pBasic->poseset;

    SDF_VU0_LOADVECTOR(&pose->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&work->vertical);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&work->upVector);
    if (work->mode == 1) {
        SDF_VU0_LOADVECTOR(&work->eyeVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&pose->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&work->targetPos);
    }
    tgtpos->x = work->targetPos.x;
    tgtpos->y = work->targetPos.y;
    tgtpos->z = work->targetPos.z;
    tgtpos->w = work->targetPos.w;
    campos->x = pose->pos.x;
    campos->y = pose->pos.y;
    campos->z = pose->pos.z;
    campos->w = pose->pos.w;
}
