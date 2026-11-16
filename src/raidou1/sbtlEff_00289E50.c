#include "common.h"
#include "dds3Basic.h"
#include "dds3obj.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
#include "sBtlSound.h"
#include "sBtlScript.h"
#include "sbtlDMG.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlItem.h"
#include "sdatUnit.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "code_r1_0021CE60.h"
#include "sdfDelayedResrc.h"
#include "libsdf.h"
#include "fileManager.h"
#include "sdfFile.h"
#include "dds3Randomize.h"
#include "sdfSound.h"
#include "sbtlSub.h"
#include "sbtlSub_0027ED08.h"
#include "sdfQuat.h"
#include "sBtlSkill.h"
#include "sBtlAI.h"
#include "sBtlDvltk.h"
#include "sbtlActAI.h"
#include "evtUnitManager.h"
#include "dds3UnitObjectBasic.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "dds3KernelMain.h"
#include "fld.h"
#include "sBtlPlayer.h"
#include "dds3PadManager.h"
#include "scrScriptProcess.h"
#include "itfMesManager.h"
#include "sBtlTalk.h"
#include "sdfVu0.h"
#include "newdata.h"
#include "code_r1_0027CBE8.h"
#include "sBtlPanel.h"
#include "AW_XAtkPad.h"
#include "EventBit.h"
#include "fldProcess.h"
#include "sdfMath.h"

s32 OnDataLoadProc(dds3ProcessID_t *id) {
    s32 i;

    if (!sbtlCallStartRain()) {
        return 0;
    }
    if (!sbtlBattleEventSb()) {
        return 0;
    }
    for (i = 0; i < 44; i++) {
        if (sbtl_effonfilecheck[i] == 2) {
            btlLoadSbPackData[i].unkC = Create_effPartsList(btlLoadSbPackData[i].data);
        }
    }
    wk_sbtl->flg |= 4;
    return -1;
}

void sbtlLeaf75(dds3ProcessID_t *id) {
}

/* NON_MATCHING: 23 of 81 words match. The table copy picks v1/a3 for source and destination where retail uses a3/t0, which shifts every later register. */
#ifdef NON_MATCHING
s32 sbtlFunc(s32 id, void *pData) {
    s32 table[20] = { 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308 };

    id -= 4;
    if (id < 20) {
        if (id < 12) {
            if (sbtlCallBtlSearchEffectData2(table[id], 12, pData)) {
                return 1;
            }
            return 0;
        }
        if (!sbtlCallBtlSearchEffectData2(table[id], 12, pData)) {
            return 0;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlFunc);
#endif

s32 sbtlLoadEffectData_Decode(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 match;
    s32 exist;

    exist = wk != NULL;
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL) {
            continue;
        }
        if ((u16)(unit->mode - 1) < 2) {
            continue;
        }
        match = 0;
        if (exist != 1 || wk->field1A0 == unit->field1A0) {
            match = 1;
        }
        if (match != 1) {
            continue;
        }
        if (unit->field414 & 0x80) {
            if (!sbtlCallBtlLoadEffectDataIDMask()) {
                return 0;
            }
        } else if (unit->field414 & 2) {
            if (!sbtlCallBtlGetSkilltbl10(unit)) {
                return 0;
            }
        } else if (!sbtlLoadEffectData_Nakama(unit)) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlFuncTable);

s32 sbtlBattleSBPACK(void) {
    fileRequest_t *pac;

    pac = pacLoad("/battle/SB_PACK3.LB");
    sbtlData83->pacHandle = pac;
    sbtlData83->setflg = 0;
    sbtlData83->dataCnt = 0;
    pacSync(pac);
    return 1;
}

s32 sbtlLoadEndCheck_PlayerOnMemData(void) {
    if (sbtlData83->setflg == 0) {
        return pacCheckEnd(sbtlData83->pacHandle) != 0;
    }
    return 1;
}

s32 sbtlLoadPlayerOnMemData(void) {
    sdfPacDecodeResrc_t *resrc;

    for (resrc = sbtlData83->pacHandle->pac.resrcHead; resrc != NULL; resrc = resrc->next) {
        if (sbtlData83->dataCnt == 8) {
            while (!sbtlLoadAttackHitData_pData(0, resrc->data)) {
            }
        } else {
            while (!sbtlBtlLoadPlayerOnMemDataSub(sbtlData83->dataCnt, resrc->data)) {
            }
        }
        sdfMemFreeDirect(resrc->resrc);
        sbtlData83->dataCnt++;
    }
    pacFileHandleFree_(sbtlData83->pacHandle);
    sbtlData83->setflg = 1;
    return 1;
}
