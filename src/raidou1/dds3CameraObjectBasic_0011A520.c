#include "common.h"
#include "basic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3Basic.h"
#include "ps2asm.h"
#include "dds3KernelDraw.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3CameraObjectBasic.c:264, match, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
u32 dds3CameraObjectBasicGetMode(dds3Basic_t *pBasic)
{
    dds3CameraObjectBasicWorkData_t *work = pBasic->workdata;

    return work->mode;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/basic/dds3CameraObjectBasic.c:271, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
void dds3CameraObjectBasicAlterMode(dds3Basic_t *basic, u32 mode)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    if (work->mode != mode) {
        work->mode = mode;
    }
}

void dds3CameraObjectBasicSetTgtCam(dds3Basic_t *pBasic, const sdfVector4_t *tgtpos, const sdfVector4_t *campos) {
    dds3CameraObjectBasicGetCameraTM(pBasic, tgtpos);
    dds3SetPosPoseSet(pBasic, campos);
    ((void (*)(dds3Basic_t *))pBasic->basicfunction->mainprocuder)(pBasic);
}

s32 dds3CameraObjectBasicFourceCalc(dds3Basic_t *pBasic) {
    dds3PoseSet_t *pose;
    dds3CameraObjectBasicWorkData_t *work;

    pose = pBasic->poseset;
    work = pBasic->workdata;
    MakeCamera(pBasic);
    dds3SetRadiusPoseSet(pose);
    SDF_VU0_COPYVECTOR(&aViewEye, &pose->pos);
    SDF_VU0_COPYVECTOR(&aViewTarget, &work->targetPos);
    SDF_VU0_COPYVECTOR(&aViewDown, &work->upVector);
    if (work->flag & 1) {
        dds3SetScreenFovy(work->fovy);
        work->flag &= ~1;
    }
    return 1;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3UnitObjectBasic.c:61, found, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *pBasic) {
    return ((dds3UnitObjectBasicWorkData_t *)pBasic->workdata)->pObjectBaseControlWorkData;
}
