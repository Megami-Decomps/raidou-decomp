#include "common.h"
#include "dds3ObjectBase.h"
#include "dds3obj.h"
#include "ps2asm.h"
#include "fldFileResolver.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "sdfQuat.h"
#include "sdk_libm.h"
#include "sdfMemory.h"
#include "dds3KernelCore.h"
#include "mdlManager.h"

INCLUDE_ASM(const s32, "code_001416C0", fldPlayerMotReset);

INCLUDE_ASM(const s32, "code_001416C0", fldPlayerMotReset2);

void fldPlayerMotReset3(void) {
    s16 motion = playerModel->current.h.motionIndex;

    if (fldGlobalBase.nakamaMode == 0) {
        fldLeaf18(motion, 0, 15.0f);
    } else {
        NakamaMotion(motion, 0, 15.0f);
    }
}

INCLUDE_ASM(const s32, "code_001416C0", fldNakamaMotReset2);

s32 fldNakamaMotReset3(void) {
    if (summonDvBasic != NULL) {
        s16 motion = summonDvModel->current.h.motionIndex;

        if (fldGlobalBase.nakamaMode == 0) {
            NakamaMotion(motion, 0, 15.0f);
        }
    }
#ifdef FIX_BUGS
    return 0;
#endif
}

s32 fldAiboMotReset(void) {
    if (aiboBasic != NULL) {
        if (fldGlobalBase.nakamaMode == 0) {
            aiboModel->first->speed = 0.5f;
            mdlSmoothLoopMotion(aiboModel, 0, 2, 15.0f, 15.0f);
        }
    }
#ifdef FIX_BUGS
    return 0;
#endif
}

INCLUDE_ASM(const s32, "code_001416C0", fldPlayerForceRotStart);

INCLUDE_ASM(const s32, "code_001416C0", sndSeStatus_sndAt3);

void fldPlayerPosRot(void) {
    sdfVector4_t pos;
    sdfVector4_t old;
    sdfVector4_t rot = { 0.0f, 0.0f, 0.0f, 1.0f };
    sdfVector4_t axis = { 0.0f, 1.0f, 0.0f, 1.0f };
    dds3Basic_t *basic = playerBasic;
    dds3PoseSet_t *pose;
    f32 diff;
    f32 step;

    if (basic == NULL) {
        return;
    }
    if (fldGlobalBase.wordD4 == 1) {
        diff = R360_LR(fldGlobalBase.floatC0, fldGlobalBase.floatC4);
        step = diff / 10.0f;
        if (step < 0.0f) {
            step = -step;
        }
        if (step < 1.0f) {
            step = 1.0f;
        }
        if ((0.0f <= diff && diff <= step) || (diff <= 0.0f && -step <= diff)) {
            fldGlobalBase.wordD4 = 0;
            diff = fldGlobalBase.floatC4;
        } else if (diff < 0.0f) {
            diff = fldGlobalBase.floatC0 - step;
        } else {
            diff = fldGlobalBase.floatC0 + step;
        }
        fldGlobalBase.floatC0 = diff;
        SDF_VU0_LOADVECTOR(&axis);
        FromRotation(fldGlobalBase.floatC0 * 3.14f / 180.0f);
        SDF_VU0_STOREVECTOR_NOMEM(&rot);
        dds3SetRotPoseSet(basic, &rot);
    } else {
        f32 angle;

        dds3GetRotPoseSet(basic);
        angle = EulerAngleAxisY() * 180.0f / 3.14f;
        fldGlobalBase.floatC0 = angle;
        fldGlobalBase.floatC4 = angle;
    }
    dds3GetPosPoseSet(basic);
    SDF_VU0_STOREVECTOR(&pos);
    fldGlobalBase.floatA8 = fldGlobalBase.focusX;
    fldGlobalBase.floatAC = fldGlobalBase.focusY;
    fldGlobalBase.floatB0 = fldGlobalBase.focusZ;
    fldGlobalBase.focusX = pos.x;
    fldGlobalBase.focusY = pos.y;
    fldGlobalBase.focusZ = pos.z;
    dds3OnPoseSetFlag(basic->poseset, 1);
    dds3OnPoseSetFlag(basic->poseset, 8);
    fldSetCameraObjectTransitionFlag();
    pose = basic->poseset;
    SDF_VU0_COPYVECTOR(&old, &pose->pos2);
    if (fabsf(old.x - pos.x) < 1.0f) {
        pose->pos2.x = pos.x;
    } else {
        pose->pos2.x = old.x + (pos.x - old.x) / 3.0f;
    }
    if (fabsf(old.y - pos.y) < 1.0f) {
        pose->pos2.y = pos.y;
    } else {
        pose->pos2.y = old.y + (pos.y - old.y) / 3.0f;
    }
    if (fabsf(old.z - pos.z) < 1.0f) {
        pose->pos2.z = pos.z;
    } else {
        pose->pos2.z = old.z + (pos.z - old.z) / 3.0f;
    }
    pose->pos2.w = 1.0f;
    ppos01Point.pos.x = pose->pos2.x;
    ppos01Point.pos.y = pose->pos2.y;
    ppos01Point.pos.z = pose->pos2.z;
    ppos01Point.pos.w = pose->pos2.w;
    ppos01Point.rot.x = pose->rot.x;
    ppos01Point.rot.y = pose->rot.y;
    ppos01Point.rot.z = pose->rot.z;
    ppos01Point.rot.w = pose->rot.w;
}

INCLUDE_ASM(const s32, "code_001416C0", fldPlayerForceSetPosSpd);

INCLUDE_ASM(const s32, "code_001416C0", fldHirukoYukaMovetaskSub);

void fldPlayerForceSetPos_NoHokan(f32 x, f32 y, f32 z) {
    dds3PoseSet_t *pose;

    if (playerBasic != NULL) {
        fldGlobalBase.focusX = x;
        fldGlobalBase.focusY = y;
        fldGlobalBase.focusZ = z;
        playerBasic->poseset->pos.x = x;
        playerBasic->poseset->pos.y = y;
        playerBasic->poseset->pos.z = z;
        dds3OnPoseSetFlag(playerBasic->poseset, 1);
        dds3OnPoseSetFlag(playerBasic->poseset, 8);
        pose = playerBasic->poseset;
        SDF_VU0_COPYVECTOR(&pose->pos2, &pose->pos);
        ppos01Point.pos.x = pose->pos2.x;
        ppos01Point.pos.y = pose->pos2.y;
        ppos01Point.pos.z = pose->pos2.z;
        ppos01Point.pos.w = pose->pos2.w;
        ppos01Point.rot.x = pose->rot.x;
        ppos01Point.rot.y = pose->rot.y;
        ppos01Point.rot.z = pose->rot.z;
        ppos01Point.rot.w = pose->rot.w;
    }
}

INCLUDE_ASM(const s32, "code_001416C0", fldPlayerForceSetRot_NoHokan);

INCLUDE_ASM(const s32, "code_001416C0", fldCallMemset);

INCLUDE_ASM(const s32, "code_001416C0", fldPlayerForceSetPos);

INCLUDE_ASM(const s32, "code_001416C0", fldCallSin3);

void fldProcSequenceSub3(void) {
    fldAreaWork_t *area;

    if (playerModel != NULL) {
        area = &fldGlobalBase;
        if (area->nakamaMode == 0 && playerModel->current.h.motionIndex == 1) {
            area->wordDC++;
        } else {
            area->wordDC = 0;
        }
    }
}

INCLUDE_ASM(const s32, "code_001416C0", fldCallFptodp);

void fldCallMemFreeDirect(void) {
    if (fldRunMapData != NULL) {
        sdfMemFreeDirect(fldRunMapData);
    }
    fldRunMapData = NULL;
    fldRunMapCellData = NULL;
    pFroMapHead = 0;
    fldRunMapScale = 0;
    fldData24 = 0;
}

void mapCallMemAllocDirect(s32 w, s32 h) {
    s32 x;
    s32 y;

    fldRunMapData = sdfMemAllocDirect(w * h * 8);
    fldRunMapCellData = sdfMemGetPtr(fldRunMapData);
    pFroMapHead = w;
    fldRunMapScale = h;
    for (x = 0; x < w; x++) {
        for (y = 0; y < h; y++) {
            fldRunMapCellData[y * w + x] = -1;
        }
    }
    fldData24 = fldGlobalBase.nameU;
}

void mapFreeMapDataSub(void) {
    fldCallMemFreeDirect();
}

void mapSetAreaRefreshSub(s32 ix, s32 iy, s32 type, s32 bit) {
    u64 one = 1;

    if (type < 64 && ix < pFroMapHead && iy < fldRunMapScale) {
        if (bit == 0) {
            fldRunMapCellData[iy * pFroMapHead + ix] &= ~(one << type);
        } else {
            fldRunMapCellData[iy * pFroMapHead + ix] |= one << type;
        }
    }
}

INCLUDE_ASM(const s32, "code_001416C0", mapBigClipChkF);

INCLUDE_ASM(const s32, "code_001416C0", mapLeaf10);

INCLUDE_ASM(const s32, "code_001416C0", mapCalcLen);

INCLUDE_ASM(const s32, "code_001416C0", mapSeVolCalc);

s32 BgProcSequence(dds3ProcessID_t *id) {
    if (dds3SearchProcessName(BgProcSequenceStrFldProcSequence) == NULL) {
        mapCutSet_From_Idx();
    }
    if (bg_tex_load_rec != -1 && bg_tex_load_last != bg_tex_load_rec) {
        mapZoomClear();
        mapAutomapFldClr(bg_tex_load_rec);
        bg_tex_load_last = bg_tex_load_rec;
        if (fldGlobalBase.word28C != 0) {
            mapCallBitCheck5(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
        }
        mapBgSetScroll(bg_scroll_last_x, mapData71, mapData72, 0);
    }
    if (bg_lay_load_rec != -1 && bg_lay_load_last != bg_lay_load_rec) {
        mapZoomClear();
        bg_lay_load_last = bg_lay_load_rec;
    }
    bg_tex_load_rec = -1;
    bg_lay_load_rec = -1;
    return 0;
}

INCLUDE_ASM(const s32, "code_001416C0", fldProcSequence2);

void fldBgTaskInit(void) {
    dds3StartProcess(fldStrBgProcSequence, 1013, 0, 0, BgProcSequence, NULL, NULL);
    dds3StartProcess(fldStrBgProcDraw, 11014, 0, 0, fldProcSequence2, NULL, NULL);
}

INCLUDE_ASM(const s32, "code_001416C0", fldBgTexClear);

void fldRefTexLoadLast(void) {
    bg_tex_load_last = -1;
    bg_lay_load_last = -1;
}

INCLUDE_ASM(const s32, "code_001416C0", fldMapMapBg);

void mapCallMemFreeDirect(void) {
    if (fldJmsQuestMapLoad_fldData != NULL) {
        sdfMemFreeDirect(fldJmsQuestMapLoad_fldData);
    }
    fldJmsQuestMapLoad_fldData = NULL;
    fldJmsQuestMapLoad_fldData2 = NULL;
}
