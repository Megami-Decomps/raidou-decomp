#include "common.h"
#include "dds3CameraObjectBasic.h"
#include "dds3Basic.h"
#include "ps2asm.h"
#include "dds3KernelDraw.h"

INCLUDE_ASM(const s32, "dds3CameraObjectBasic_0011A520", dds3CameraObjectBasicGetMode);

INCLUDE_ASM(const s32, "dds3CameraObjectBasic_0011A520", dds3CameraObjectBasicAlterMode);

void dds3CameraObjectBasicSetTgtCam(dds3Basic_t *pBasic, const sdfVector4_t *tgtpos, const sdfVector4_t *campos) {
    dds3CameraObjectBasicGetCameraTM(pBasic, tgtpos);
    dds3SetPosPoseSet_dds3Basic(pBasic, campos);
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

dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *pBasic) {
    return ((dds3UnitObjectBasicWorkData_t *)pBasic->workdata)->pObjectBaseControlWorkData;
}
