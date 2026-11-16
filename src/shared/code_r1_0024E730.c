#include "common.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "fclMisc.h"
#include "rstInit.h"
#include "sBtlUnit.h"
#include "fld.h"
#include "sBtlScene.h"

#ifdef VERSION_RAIDOU1
s32 SetTestCamera(void)
{
    sdfVector4_t up = { 0.0f, 1.0f, 0.0f, 1.0f };
    sdfVector4_t eye;
    sdfVector4_t target;
    dds3Basic_t *camera;

    memset(&eye, 0, sizeof(eye));
    eye.w = 1.0f;
    target = (sdfVector4_t){ 0.0f, 0.0f, 10.0f, 1.0f };
    camera = dds3AddCameraObjectBasicTypeLookAt(dds3AllocObjectId(), &eye, &target, &up);
    dds3SetUnitObjectId(camera, "sbtl_Cam");
    dds3GetRadiusPoseSet(camera, 10.0f);
    dds3ObjectBaseCreateMover(camera);
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
    dds3ObjectBaseSetPadControl(camera, NULL);
    wk_sbtl->flg |= 1;
    return 1;
}
#else
INCLUDE_RODATA(const s32, "code_r1_0024E730", SetTestCameraTable);

INCLUDE_RODATA(const s32, "code_r1_0024E730", SetTestCameraTable2);

INCLUDE_ASM(const s32, "code_r1_0024E730", SetTestCamera);
#endif
