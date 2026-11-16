#include "common.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "mjn.h"
#include "dds3AreaBasic.h"
#include "sdk_libc.h"
#include "effManager.h"
#include "sdf_resource.h"
#include "mapData.h"
#include "mselGlobal.h"
#include "fclMisc.h"

void M_InitScene_mjn(void)
{
    dds3Basic_t *scene = dds3AddScene();

    dds3SetControlScene(scene);
    dds3SetActiveScene(scene);
    dds3AddAreaBasic(dds3SerializeID(), "nme_area");
}

INCLUDE_ASM(const s32, "code_r1_002F93D8", mselCallGetControlScene);

void mselNmeCamera(void)
{
    sdfVector4_t up = { 0.0f, 1.0f, 0.0f, 1.0f };
    sdfVector4_t position = { 0.0f, 0.0f, -512.0f, 1.0f };
    sdfVector4_t target;
    dds3Basic_t *camera;

    memset(&target, 0, sizeof(target));
    target.w = 1.0f;
    camera = dds3AddCameraObjectBasicTypeLookAt(dds3SerializeID(), &position, &target, &up);
    dds3SetUnitObjectId(camera, "nme_camera");
    dds3CameraObjectBasicSetFovy(camera, 0.43633229f);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), camera);
}

void mselNameNEFLASH(void)
{
    sdfVector4_t position = { -192.0f, -192.0f, 0.0f, 1.0f };

    if (fldTitleMiniWork4 == NULL) {
        fldTitleMiniWork4 = sdfDevLoadFileEx("/name/NE_FLASH.EPL", &mselNameNEFLASHData, NULL);
        fldTitleMiniTex3 = dds3EffectLoadEPL((void *)mselNameNEFLASHData);
        fclInitDstEff2Sub((struct plistHandle_t *)fldTitleMiniTex3->instance);
        dds3EffectReset(fldTitleMiniTex3);
        dds3EffectSetPosition(fldTitleMiniTex3, &position);
        dds3EffectSetLoop_dds3Effect(fldTitleMiniTex3, 1);
        dds3EffectSetScale(fldTitleMiniTex3, 1.0f);
    }
}
INCLUDE_RODATA(const s32, "code_r1_002F93D8", fldFontNameEntDataStr);

INCLUDE_SDATA(const s32, "code_r1_002F93D8", fldTitleMiniWork4);

INCLUDE_SDATA(const s32, "code_r1_002F93D8", mselNameNEFLASHData);

INCLUDE_SDATA(const s32, "code_r1_002F93D8", fldTitleMiniTex3);

