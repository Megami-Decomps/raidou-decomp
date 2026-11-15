#include "common.h"
#include "sBtlEff.h"
#include "sBtlUnit.h"
#include "sBtlSkill.h"
#include "sbattle.h"
#include "sbtlWork.h"
#include "EventBit.h"
#include "fileManager.h"
#include "sdk_libc.h"
#include "newdata.h"
#include "sBtlEncount.h"
#include "sBtlCalc.h"
#include "sbtlSub.h"
#include "fld.h"
#include "ps2asm.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_0027CBE8.h"
#include "sdfQuat.h"

/* NON_MATCHING: 18 of 404 words match. Register allocation shifts, extern GBWK not gp-relative. */
#ifdef NON_MATCHING
struct SBTLUNIT_WORK *sbtlCallMemset21(s32 devilId, s32 point, s32 arg, s32 mode)
{
    struct SBTLUNIT_WORK *unit;
    SBTL_DEVIL_DATA *devil;
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *sub;
    sdfVector4_t identity;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
    s32 i;
    SBTLUNIT_MODEL *model;

    memset(&identity, 0, sizeof(identity));
    identity.w = 1.0f;
    if (devilId == 125 && DDS3_GBWK->partyNum > 0) {
        if (DDS3_GBWK->party[0].devilId == 45) {
            devilId = 126;
        } else {
            for (i = 1; i < DDS3_GBWK->partyNum; i++) {
                if (DDS3_GBWK->party[i].devilId == 45) {
                    devilId = 126;
                    break;
                }
            }
        }
    }
    if (devilId == 127 && DDS3_GBWK->partyNum > 0) {
        if (DDS3_GBWK->party[0].devilId == 47) {
            devilId = 128;
        } else {
            for (i = 1; i < DDS3_GBWK->partyNum; i++) {
                if (DDS3_GBWK->party[i].devilId == 47) {
                    devilId = 128;
                    break;
                }
            }
        }
    }
    devil = sbtlGetDevilData_sbtlSub(devilId);
    visual = sbtlGetMotionVisualData(2, devil->motionId);
    sub = sbtlSetUnitSub(2, devil->motionId, 0);
    unit = sbtlSetSummon(1, devilId, arg);
    unit->devilID = devilId;
    unit->devilParam1 = devil->field04;
    unit->devilParam0 = devil->field0E;
    unit->field404 = devil->aisyoId;
    unit->field400 = devil->field0A;
    unit->aisyo = *sbtlGetAisyotbl(devil->aisyoId);
    unit->aisyoBase = unit->aisyo;
    unit->flg |= devil->flag;
    unit->resist[0] = devil->resist[0];
    unit->stat1C0 = devil->stat18;
    unit->resist[1] = devil->resist[1];
    unit->stat1D8 = devil->stat18;
    unit->resist[2] = devil->resist[2];
    unit->stat1DC = devil->stat1C;
    unit->resist[3] = devil->resist[3];
    unit->stat1C8 = devil->stat20;
    unit->stat1E0 = devil->stat20;
    unit->stat1E4 = devil->stat24;
    for (i = 0; i < 9; i++) {
        unit->param[i] = devil->param[i];
    }
    sbtlCheckAutoSkill_Enemy(unit);
    sbtlCheckBattlePara(unit);
    unit->devilParam2 = devil->field0F;
    if (point == 100) {
        unit->pos.x = 0.0f;
        unit->pos.y = 0.0f;
        unit->pos.z = 0.0f;
        unit->rot.x = 0.0f;
        unit->rot.y = 0.0f;
        unit->rot.z = 0.0f;
        unit->rot.w = 1.0f;
        VU0_LOAD_VF(vf10, &unit->rot);
        VU0_STORE_VF(vf10, &unit->rotBase);
        sbtlSetRotateDeg(unit);
    } else {
        mapEncPointGet(point, &x, &y, &z, &r);
        unit->pos.x = x;
        unit->pos.y = y;
        unit->pos.z = z;
        FromEuler(0.0f, r * 0.017453292f, 0.0f);
        VU0_STORE_VF(vf10, &unit->rot);
        VU0_LOAD_VF(vf10, &identity);
        VU0_STORE_VF(vf10, &unit->rotBase);
        unit->angleY = r;
    }
    if (unit->flg & 0x80000) {
        VU0_LOAD_VF(vf10, &unit->pos);
        VU0_STORE_VF(vf10, &unit->posSaved);
    }
    model = unit->model;
    model->flags |= 0x1000;
    if (sbtlCheckScriptData(unit->devilID)) {
        unit->model->flags &= ~0x1000;
    }
    if (unit->flg & 0x8000000) {
        unit->field3B6 = devil->field42;
    } else {
        unit->field3B6 = 0;
    }
    unit->zero18 = 0;
    unit->field414 = 2;
    unit->field424 = 0;
    model = unit->model;
    unit->visualA = visual->field04;
    model->field0FC = 0;
    model->field100 = unit->field1A0;
    unit->field428 = visual->field08;
    model->flags = (model->flags | 0x160) & ~0x80;
    VU0_STORE_VF(vf10, &model->vec20);
    VU0_STORE_VF(vf10, &unit->model->vec30);
    model = unit->model;
    model->field12C = devil->field64;
    model->field130 = devil->field60;
    if (unit->stat1C0 == 0) {
        unit->stat1C0 = 50;
        unit->stat1C4 = 50;
    }
    unit->visual[0] = visual->scale;
    unit->visual[1] = visual->scale;
    unit->visual[2] = visual->scale;
    unit->visual[3] = visual->scale;
    unit->visualKind = visual->kind;
    model->field11C = visual->field10;
    unit->actMax = devil->field14;
    model->field120 = sub->field04;
    model->field124 = devil->field60;
    model->field128 = devil->field64;
    sbtlSetactcounter(unit);
    unit->actCounter = GetIntervalRand(0, unit->actMax);
    if (visual->field30 > 0) {
        sbtlGetDevilData(unit, visual->field32, visual->field30);
        unit->flg |= 0x1000;
    }
    sbtlRefSbtl18(unit, mode);
    if (mode != 2) {
        if (mode == 1) {
            sbtlCallMemset17(wk_sbtl->unitEffect, unit, 14, 0, &unit->pos.x, 0x80808080, 0);
        } else if (unit->flg & 0x100000) {
            sbtlCallMemset17(wk_sbtl->unitEffect, unit, 5, 0, &unit->pos.x, 0x80808080, 0);
        } else {
            sbtlCallMemset17(wk_sbtl->unitEffect, unit, 51, 0, &unit->pos.x, 0x80808080, 0);
        }
    }
    return unit;
}
#else
INCLUDE_ASM(const s32, "code_r1_00297EF0", sbtlCallMemset21);
#endif
