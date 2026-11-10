#include "shared/basic/dds3CameraObjectBasic/decls.h"

/* Rotate the local vectors and cache their world-space look-at matrix. */
void MakeCamera(dds3Basic_t *pBasic)
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
    LookAtMatrix(&work->targetPos, &pose->pos, &work->upVector);
    SDF_VU0_STOREMATRIX(&work->cameraTM);
}
