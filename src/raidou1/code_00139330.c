#include "common.h"
#include "dds3ObjectBase.h"

#include "basic.h"
#include "dds3obj.h"
#include "field.h"
#include "include_asm.h"
#include "kernel_draw.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "ps2asm.h"
#include "fpu.h"
#include "kernel.h"
#include "draw3d.h"
#include "scr.h"
#include "fldFileResolver_00132080.h"
#include "effPCPMisc_00224D10.h"
#include "fldFileResolver.h"
#include "datCalc.h"
#include "newdata.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "mdlManager.h"
#include "fldDbg.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "sdk_libm.h"
#include "sdfMemory.h"
#include "dds3KernelCore.h"

void fldPlayerMotReset(void) {
    s16 motion = playerModel->current.h.motionIndex;

    if (fldGlobalBase.nakamaMode == 0) {
        if (mapCheckDying(0) != 0) {
            fldLeaf18(motion, 2, 10.0f);
        } else {
            fldLeaf18(motion, 0, 10.0f);
        }
    } else {
        NakamaMotion(motion, mapCheckDying(1) != 0 ? 2 : 0, 10.0f);
    }
}

INCLUDE_ASM(const s32, "code_00139330", fldPlayerMotReset2);

void fldPlayerMotReset3(void) {
    s16 motion = playerModel->current.h.motionIndex;

    if (fldGlobalBase.nakamaMode == 0) {
        fldLeaf18(motion, 0, 15.0f);
    } else {
        NakamaMotion(motion, 0, 15.0f);
    }
}

INCLUDE_ASM(const s32, "code_00139330", fldNakamaMotReset2);

s32 fldNakamaMotReset3(void) {
    if (summonDvBasic != NULL) {
        s16 motion = summonDvModel->current.h.motionIndex;

        if (fldGlobalBase.nakamaMode == 0) {
            NakamaMotion(motion, 0, 15.0f);
        }
    }
}

s32 fldAiboMotReset(void) {
    if (aiboBasic != NULL) {
        if (fldGlobalBase.nakamaMode == 0) {
            aiboModel->first->speed = 0.5f;
            mdlSmoothLoopMotion(aiboModel, 0, 2, 15.0f, 15.0f);
        }
    }
}

INCLUDE_ASM(const s32, "code_00139330", fldPlayerForceRotStart);

INCLUDE_ASM(const s32, "code_00139330", sndSeStatus_sndAt3);

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

INCLUDE_ASM(const s32, "code_00139330", fldPlayerForceSetPosSpd);

INCLUDE_ASM(const s32, "code_00139330", fldHirukoYukaMovetaskSub);

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

INCLUDE_ASM(const s32, "code_00139330", fldPlayerForceSetRot_NoHokan);

INCLUDE_ASM(const s32, "code_00139330", fldCallSin3);

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

/* NON_MATCHING: 7 of 50 words match.  */
#ifdef NON_MATCHING
void fldCallFptodp(void) {
    s32 i;
    f32 angle;
    f32 s;
    f32 c;

    for (i = 0; i < 1000; i++) {
        angle = (f32)i * 0.00157;
        s = sdfSin(angle);
        c = sdfCos(angle);
        fldCallFptodpWork[(s32)(s * 100.0f)] = c;
    }
}
#else
INCLUDE_ASM(const s32, "code_00139330", fldCallFptodp);
#endif

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

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0013A350.c:1, found */
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

/* NON_MATCHING: 44 of 78 words match.  */
#ifdef NON_MATCHING
s32 mapBigClipChkF(s32 cut, f32 x, f32 y, f32 z) {
    s32 ix;
    s32 iz;

    if (cut >= 64) {
        return 1;
    }
    if (fldGlobalBase.amSBlock == 8 && fldGlobalBase.nameU == 0 && (cut == 27 || cut == 32) && z < 1700.0f) {
        return 0;
    }
    if (fldGlobalBase.amSBlock == 9 && fldGlobalBase.nameU == 0 && cut == 6 && z < 2300.0f) {
        return 0;
    }
    fldSetPlayerTargetData(&ix, &iz, x, z);
    ix /= 5;
    iz /= 5;
    if (ix < pFroMapHead && iz < fldRunMapScale) {
        return (fldRunMapCellData[iz * pFroMapHead + ix] & ((u64)1 << cut)) != 0;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00139330", mapBigClipChkF);
#endif

INCLUDE_ASM(const s32, "code_00139330", mapLeaf10);

INCLUDE_ASM(const s32, "code_00139330", mapCalcLen);

INCLUDE_ASM(const s32, "code_00139330", mapSeVolCalc);

s32 BgProcSequence(dds3ProcessID_t *id) {
    if (dds3SearchProcessName(BgProcSequenceStrFldProcSequence) == NULL) {
        mapCutSet_From_Idx();
    }
    if (bg_tex_load_rec != -1 && bg_tex_load_last != bg_tex_load_rec) {
        mapZoomClear();
        mapAutomapFldClr(bg_tex_load_rec);
        bg_tex_load_last = bg_tex_load_rec;
        if (fldGlobalBase.word248 != 0) {
            mapSoundKankyoSeSet(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
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

s32 fldFunc(dds3ProcessID_t *id) {
    mapSKYBGEDIT();
    return 0;
}

void fldBgTaskInit(void) {
    dds3StartProcess(fldStrBgProcSequence, 1013, 0, 0, BgProcSequence, NULL, NULL);
    dds3StartProcess(fldStrBgProcDraw, 11014, 0, 0, fldFunc, NULL, NULL);
}

/* NON_MATCHING: 27 of 35 words match.  */
#ifdef NON_MATCHING
void fldBgTexClear(void) {
    mapBgFree2();
    mapCallMemFreeDirect();
    fldFreeMapData();
    bg_tex_load_last = -1;
    bg_lay_load_last = -1;
    bg_tex_load_rec = -1;
    bg_lay_load_rec = -1;
    mapData73 = 1.0f;
    fldGlobalBase.cutIdx = 0;
    fldGlobalBase.cutIdxPrev = 0;
    fldGlobalBase.cutState = 0;
    fldGlobalBase.hitMatterIndex = 0;
    mapZoomSetGoalData9 = 1.0f;
    mapZoomSetGoalData10 = 1.0f;
    mapData74 = 0;
    mapBgSetScrollData4 = 0;
    mapData75 = 1.0f;
    mapData76 = 1.0f;
    mapZoomClearData = 0;
    mapData77 = 0;
    mapData78 = 0;
    Zoom = 1.0f;
}
#else
INCLUDE_ASM(const s32, "code_00139330", fldBgTexClear);
#endif

void fldRefTexLoadLast(void) {
    bg_tex_load_last = -1;
    bg_lay_load_last = -1;
}

INCLUDE_RODATA(const s32, "code_00139330", fldPlayerForceSetRot_NoHokanTable);

INCLUDE_RODATA(const s32, "code_00139330", fldCallFptodpTable);

INCLUDE_RODATA(const s32, "code_00139330", BgProcSequenceStrFldProcSequence);

INCLUDE_RODATA(const s32, "code_00139330", fldStrBgProcSequence);

INCLUDE_RODATA(const s32, "code_00139330", fldStrBgProcDraw);

INCLUDE_ASM(const s32, "code_00139330", fldMapMapBg);

void mapCallMemFreeDirect(void) {
    if (fldJmsQuestMapLoad_fldData != NULL) {
        sdfMemFreeDirect(fldJmsQuestMapLoad_fldData);
    }
    fldJmsQuestMapLoad_fldData = NULL;
    fldJmsQuestMapLoad_fldData2 = NULL;
}
