#include "common.h"
#include "sBtlUnit.h"
#include "sbtlSub.h"
#include "ps2asm.h"
#include "sdfVu0.h"
#include "sdfMath.h"

INCLUDE_ASM(const s32, "code_00288220", sbtlBtlCheckSkillCancelSub);

INCLUDE_ASM(const s32, "code_00288220", sbtlStartDeadEvent);

INCLUDE_ASM(const s32, "code_00288220", sbtlGetPointLast);

s32 GetUnitHelperMatrix(struct SBTLUNIT_WORK *wk, SBTLUNIT_MARK mark)
{
    sdfVector4_t vec;
    f32 angle;

    if (wk->unit.devilId == 627) {
        if (mark == 300) {
            sbtlGetUnitHelperPos(wk, 101);
            SDF_VU0_LOADVECTOR_B(&wk->model->scrPos3);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(&vec);
            angle = sdfAtan2(vec.x, vec.y);
            SDF_VU0_LOADIDENTITY();
            SetRotateMatrixZ(-angle);
            return 1;
        }
        if (mark == 302) {
            sbtlGetUnitHelperPos(wk, 102);
            SDF_VU0_LOADVECTOR_B(&wk->model->scrPos3);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(&vec);
            angle = sdfAtan2(vec.x, vec.y);
            SDF_VU0_LOADIDENTITY();
            SetRotateMatrixZ(-angle);
            return 1;
        }
        if (mark == 301) {
            SDF_VU0_LOADIDENTITY();
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00288220", sbtlCheckEffectCancel);

INCLUDE_ASM(const s32, "code_00288220", sbtlRefSbtl38);

INCLUDE_ASM(const s32, "code_00288220", sbtlSetSkillTarget);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf8);

INCLUDE_ASM(const s32, "code_00288220", Support_RunWaitProcess);

INCLUDE_ASM(const s32, "code_00288220", Support_RunWarpProcess);

INCLUDE_ASM(const s32, "code_00288220", sbtlSupport_BattleStop);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallEncGetPointNums);

INCLUDE_ASM(const s32, "code_00288220", Support_RunDownProcess);

INCLUDE_ASM(const s32, "code_00288220", mapFunc12);

INCLUDE_ASM(const s32, "code_00288220", Support_GetNakamaData);

INCLUDE_ASM(const s32, "code_00288220", mapEncGetPointNums);

INCLUDE_ASM(const s32, "code_00288220", Support_SetAisyo);

INCLUDE_ASM(const s32, "code_00288220", sbtlSupport_StopAction);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallBtlSetMotionSBtlUnit);

INCLUDE_ASM(const s32, "code_00288220", Support_MovePos2);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallBtlGetAITBL);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallGetIntPara31);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallEncGetPointNums4);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallSetIntReturnValue9);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallGetIntPara);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf9);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallEncGetPointNums2);

INCLUDE_ASM(const s32, "code_00288220", sbtlBtlCommandSBTL);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallBtlDeath);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallGetIntPara32);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallBtlCheckDamageTarget7);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallGetIntPara33);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallGetIntPara34);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallGetIntPara35);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallGetIntPara36);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallSetIntReturnValue10);

INCLUDE_ASM(const s32, "code_00288220", sbtlCallSetIntReturnValue11);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf10);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf11);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf12);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf13);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf14);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf15);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf16);

INCLUDE_ASM(const s32, "code_00288220", sbtlLeaf17);
