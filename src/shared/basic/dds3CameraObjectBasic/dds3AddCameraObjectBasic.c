#include "shared/basic/dds3CameraObjectBasic/decls.h"

dds3Basic_t *dds3AddCameraObjectBasic(u32 serialID, f32 fovy, const sdfVector4_t *position,
                                     const sdfVector4_t *eye, const sdfVector4_t *up, s32 mode)
{
    dds3Basic_t *basic = dds3AllocCameraObjectBasic(serialID);
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    work->fovy = fovy;
    work->mode = mode;
    dds3SetPosPoseSet(basic, position);
    dds3SetRadiusPoseSet(basic->poseset);
    SDF_VU0_COPYVECTOR(&work->vertical, up);
    SDF_VU0_COPYVECTOR(&work->eyeVector, eye);
    if (mode == 0) {
        SDF_VU0_COPYVECTOR(&work->targetPos, eye);
    }
    MakeCamera(basic);
    return basic;
}
