#include "common.h"
#include "raidou_alloc.h"
#include "sdfFrame.h"
#include "basic.h"
#include "dds3obj.h"
#include "model.h"
#include "ps2asm.h"
#include "sdfChip.h"
#include "sdk_libc.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3ObjectBase.h"
#include "dds3MatterObjectBasic.h"
#include "fldDraw.h"
#include "fldProcess.h"

dds3Basic_t *dds3AddMatterObjectBasic(s32 serialID, f32 *pos, f32 *rot);
void dds3DeleteMatterObjectBasic(dds3Basic_t *aPtr);
void dds3MatterObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);
void dds3MatterObjectBasicSetHojiID(dds3Basic_t *aPtr, u32 hojiID);
u32 dds3MatterObjectBasicGetHojiID(u32 resID);
void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *aPtr, u32 alpmode);
u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *aPtr);
s32 dds3MatterObjectBasicConstructor(dds3Basic_t *aPtr);
void dds3MatterObjectBasicDestructor(dds3Basic_t *aPtr);

dds3Basic_t *dds3AddMatterObjectBasic(s32 serialID, f32 *pos, f32 *rot) {
    sdfVector4_t zero;
    dds3Basic_t *basic;

    memset(&zero, 0, sizeof(zero));
    zero.w = 1.0f;
    basic = dds3AddSceneBasicData(BASIC_TYPE_MATTEROBJECT);
    basic->serialID = serialID;
    dds3ObjectBaseCreateMover(basic);
    dds3SetRotPoseSet(basic, (const sdfVector4_t *)rot);
    dds3SetPosPoseSet(basic, &zero);
    dds3SetRadiusPoseSet(basic->poseset);
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[0] = pos[0];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[1] = pos[1];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[2] = pos[2];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[3] = pos[3];
    return basic;
}

void dds3DeleteMatterObjectBasic(dds3Basic_t *aPtr) {
    dds3DeleteSceneBasicData(aPtr);
}

void dds3MatterObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->OtLinkIdx = idx;
}

void dds3MatterObjectBasicSetHojiID(dds3Basic_t *aPtr, u32 hojiID) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->hojiID = hojiID;
}

u32 dds3MatterObjectBasicGetHojiID(u32 resID) {
    dds3Basic_t *scene;
    dds3Basic_t *basic;
    dds3MatterObjectBasicWorkData_t *work;

    scene = dds3GetControlScene();
    basic = dds3SearchSceneBasic(scene, resID, BASIC_TYPE_MATTEROBJECT);
    work = basic->workdata;
    return work->hojiID;
}

void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *aPtr, u32 alpmode) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->alpmode = alpmode;
}

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *aPtr) {
    return ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->alpmode;
}

INCLUDE_ASM(const s32, "basic/dds3MatterObjectBasic", dds3Leaf28);

s32 dds3MatterObjectBasicConstructor(dds3Basic_t *aPtr) {
    dds3MatterObjectBasicWorkData_t *work;
    void *allocation;

    dds3InitPoseSet(aPtr);
    allocation = sdfChipAllocDirect(sizeof(dds3MatterObjectBasicWorkData_t));
    aPtr->workdata = allocation;
    memset(allocation, 0, sizeof(dds3MatterObjectBasicWorkData_t));
    work = aPtr->workdata;
    work->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(aPtr);
    work->hojiID = 0;
    work->alpmode = 0;
    work->OtLinkIdx = -1;
    work->base_pos[0] = 0.0f;
    work->base_pos[1] = 0.0f;
    work->base_pos[2] = 0.0f;
    work->base_pos[3] = 1.0f;
    work->move_pos[0] = 0.0f;
    work->move_pos[1] = 0.0f;
    work->move_pos[2] = 0.0f;
    work->move_pos[3] = 1.0f;
    dds3ObjectBaseOnFlag(aPtr, 0xE2);
    return 1;
}

void dds3MatterObjectBasicDestructor(dds3Basic_t *aPtr) {
    dds3MatterObjectBasicWorkData_t *work;

    dds3FreePoseSet(aPtr);
    work = aPtr->workdata;
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "basic/dds3MatterObjectBasic", MainProcedure);

/* NON_MATCHING: 9 of 123 words match.  */
#ifdef NON_MATCHING
s32 dds3MatterObjectBasicDrawProcedure(dds3Basic_t *basic)
{
    dds3MatterObjectBasicWorkData_t *work;
    u32 alpmode;
    s32 otIndex;
    u32 color;
    s32 result;

    result = 1;
    if (effObjOpacityPassEnabled) {
        work = basic->workdata;
        if (work->OtLinkIdx == -1) {
            rsetOtOfs(44);
            rsetTest(1, 5, 0x60, 1, 0, 0, 1, 2);
            dds3ObjectBaseModelLinkOt(&ModelOt[0], basic);
            rsetOtOfs(44);
            rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
            return 1;
        }
        rsetOtOfs(42);
        rsetTest(1, 5, 0x60, 1, 0, 0, 1, 2);
        alpmode = work->alpmode;
        if (alpmode < 5) {
            if (alpmode >= 3) {
                if (basic->color == 0x80808080) {
                    dds3ObjectBaseModelLinkOt(&ModelOt[work->OtLinkIdx], basic);
                    goto link;
                }
                if (!dds3ObjectBaseCheckFlag(playerBasic, 1)) {
                    dds3ObjectBaseModelLinkOt(&ModelOt[10], basic);
                    goto link;
                }
                color = basic->color;
                basic->color = 0x80808080;
                dds3ObjectBaseModelLinkOt(&ModelOt[work->OtLinkIdx], basic);
                basic->color = color;
            }
        }
link:
        dds3ObjectBaseModelLinkOt(&ModelOt[work->OtLinkIdx], basic);
        rsetOtOfs(42);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "basic/dds3MatterObjectBasic", dds3MatterObjectBasicDrawProcedure);
#endif

INCLUDE_SDATA(const s32, "basic/dds3MatterObjectBasic", effObjOpacityPassEnabled);

