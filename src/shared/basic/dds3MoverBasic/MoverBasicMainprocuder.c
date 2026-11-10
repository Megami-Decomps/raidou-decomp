#include "shared/basic/dds3MoverBasic/decls.h"

/* Paths supply absolute channels; a primary callback supplies relative pose. */
s32 MoverBasicMainprocuder(dds3Basic_t *pBasic)
{
    sdfVector4_t pathVector;
    fldResourceLightPath_t light;
    dds3PoseSet_t relativePose;
    dds3MoverBasicWorkData_t *work = pBasic->workdata;
    dds3Basic_t *object = work->pObjectBasic;
    dds3PoseSet_t *pose = object->poseset;
    dds3BasicControlProc_t callback;
    dds3CameraObjectBasicWorkData_t *camera;
    f32 fovy;

    if (work->pPathControl != NULL) {
        if (work->pPathControl->flag & DDS3_PATHBASIC_FLAG_POS) {
            dds3PathBasicControlGetPos(work->pPathControl);
            SDF_VU0_STOREVECTOR(&pathVector);
            dds3SetPosPoseSet(object, &pathVector);
            if (object->type >> 24 == BASIC_TYPE_MATTEROBJECT) {
                SDF_VU0_COPYVECTOR(&((dds3MatterPathPosition_t *)object->workdata)->position, &pathVector);
            }
        }
        if (work->pPathControl->flag & DDS3_PATHBASIC_FLAG_ROT) {
            dds3PathBasicControlGetRot(work->pPathControl);
            SDF_VU0_STOREVECTOR(&pathVector);
            dds3SetRotPoseSet(object, &pathVector);
        }
        if (work->pPathControl->flag & DDS3_PATHBASIC_FLAG_FOVY) {
            if (object->type >> 24 == BASIC_TYPE_CAMERAOBJECT) {
                fovy = dds3PathBasicControlGetFovy(work->pPathControl);
                camera = object->workdata;
                camera->fovy = fovy;
                camera->flag |= 1;
            }
        }
        if (work->pPathControl->flag & DDS3_PATHBASIC_FLAG_LIGHT) {
            if (object->type >> 24 == BASIC_TYPE_LIGHTOBJECT) {
                dds3PathBasicControlNextStep(work->pPathControl, &light);
                dds3SetLightObjectPathResoruce(object, &light);
            }
        }
        dds3PathBasicControlIsFinished(work->pPathControl);
    } else {
        callback = work->pFunc;
        pose->frameFall = 0;
        if (callback != NULL) {
            dds3BackupPoseSet(&relativePose);
            if (callback(&relativePose, object) == 1) {
                dds3GetSclPoseSetMCR(object, &relativePose.scl);
                dds3AddRotPoseSet(object, &relativePose.rot);
                dds3GetPosPoseSetMCR(object, &relativePose.pos);
            }
        }
    }
    return 1;
}
