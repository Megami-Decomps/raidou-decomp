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

/* NON_MATCHING: 24 of 404 words match. Register allocation shifts, extern GBWK not gp-relative. */
#ifdef NON_MATCHING
struct SBTLUNIT_WORK *SetEncountData_Sub_sbtlEncount(s32 devilId, s32 point, s32 arg, s32 mode)
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
    if (devilId == 125 && DDS3_GBWK->playerData.partyNum > 0) {
        if (DDS3_GBWK->playerData.stock[0].devilId == 45) {
            devilId = 126;
        } else {
            for (i = 1; i < DDS3_GBWK->playerData.partyNum; i++) {
                if (DDS3_GBWK->playerData.stock[i].devilId == 45) {
                    devilId = 126;
                    break;
                }
            }
        }
    }
    if (devilId == 127 && DDS3_GBWK->playerData.partyNum > 0) {
        if (DDS3_GBWK->playerData.stock[0].devilId == 47) {
            devilId = 128;
        } else {
            for (i = 1; i < DDS3_GBWK->playerData.partyNum; i++) {
                if (DDS3_GBWK->playerData.stock[i].devilId == 47) {
                    devilId = 128;
                    break;
                }
            }
        }
    }
    devil = sbtlGetDevilData_sbtlSub(devilId);
    visual = sbtlGetVisualData(2, devil->motionId);
    sub = sbtlGetMotionVisualData(2, devil->motionId, 0);
    unit = sbtlSetSummon(1, devilId, arg, 0);
    unit->unit.devilId = devilId;
    unit->unit.level = devil->level;
    unit->unit.raceId = devil->field0E;
    unit->field404 = devil->aisyoId;
    unit->field400 = devil->field0A;
    unit->unit.aisyo = *sbtlGetAisyotbl(devil->aisyoId);
    unit->aisyoBase = unit->unit.aisyo;
    unit->flg |= devil->flag;
    unit->unit.backup.str = devil->resist[0];
    unit->unit.now.hp = devil->stat18;
    unit->unit.backup.vit = devil->resist[1];
    unit->unit.backup.hp = devil->stat18;
    unit->unit.backup.tec = devil->resist[2];
    unit->unit.backup.maxhp = devil->stat1C;
    unit->unit.backup.luck = devil->resist[3];
    unit->unit.now.mp = devil->stat20;
    unit->unit.backup.mp = devil->stat20;
    unit->unit.backup.maxmp = devil->stat24;
    for (i = 0; i < 9; i++) {
        unit->haveskill[i + 2] = devil->param[i];
    }
    sbtlCheckAutoSkill_Enemy(unit);
    sbtlCheckBattlePara(unit);
    unit->unit.seikaku = devil->field0F;
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
        FromEuler(0.0f, r * 0.017453293f, 0.0f);
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
    if (sbtlCheckScriptData(unit->unit.devilId)) {
        unit->model->flags &= ~0x1000;
    }
    if (unit->flg & 0x8000000) {
        unit->motion_setno = devil->field42;
    } else {
        unit->motion_setno = 0;
    }
    unit->mv_spd.z = 0;
    unit->field414 = 2;
    unit->field424 = 0;
    model = unit->model;
    unit->max_mvspd.z = visual->field04;
    model->field0FC = 0;
    model->field100 = unit->field1A0;
    unit->field428 = visual->field08;
    model->flags = (model->flags | 0x160) & ~0x80;
    VU0_STORE_VF(vf10, &model->vec20);
    VU0_STORE_VF(vf10, &unit->model->vec30);
    model = unit->model;
    model->field12C = devil->field64;
    model->field130 = devil->field60;
    if (unit->unit.now.hp == 0) {
        unit->unit.now.hp = 50;
        unit->unit.now.maxhp = 50;
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
            sbtlCreateEff(wk_sbtl->effList, unit, 14, 0, &unit->pos.x, 0x80808080, 0);
        } else if (unit->flg & 0x100000) {
            sbtlCreateEff(wk_sbtl->effList, unit, 5, 0, &unit->pos.x, 0x80808080, 0);
        } else {
            sbtlCreateEff(wk_sbtl->effList, unit, 51, 0, &unit->pos.x, 0x80808080, 0);
        }
    }
    return unit;
}
#else
INCLUDE_ASM(const s32, "code_r1_00297EF0", SetEncountData_Sub_sbtlEncount);
#endif
