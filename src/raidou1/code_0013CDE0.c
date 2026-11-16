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
#include "fileManager.h"
#include "dds3KernelCore.h"
#include "sdf3D.h"
#include "EventBit.h"
#include "sdfTex.h"
#include "effManager.h"
#include "dds3KernelDraw.h"
#include "sdfVu0.h"

void mapBgSetScrollReset(void) {
    dds3ScreenParam.height = 448.0f;
    sdf3DScreenAdjust = 0;
    sdf3DScreenAdjustOffsetX = 0.0f;
    sdf3DScreenAdjustOffsetY = 0.0f;
    sdf3DScreenAdjustScaleX = 1.0f;
    sdf3DScreenAdjustScaleY = 1.0f;
    dds3SetScreenFovy(0.628f);
}

INCLUDE_ASM(const s32, "code_0013CDE0", map_fldPoint3To2);

INCLUDE_ASM(const s32, "code_0013CDE0", map_fldPoint3To2O);

void map_fldPoint3To2O2(f32 x3, f32 y3, f32 z3, f32 *x2, f32 *y2) {
    sdfVector4_t scale = {mapData73 * 512.0f, 224.0f, 1.0f, 1.0f};
    sdfVector4_t point;
    sdfVector4_t screen;

    point.x = x3;
    point.y = y3;
    point.z = z3;
    point.w = 1.0f;
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR(&point);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&screen);
    *x2 = screen.x;
    *y2 = screen.y;
}

INCLUDE_ASM(const s32, "code_0013CDE0", map_fldPoint3To2O3);

/* NON_MATCHING: 159 of 173 words match.  */
void fldBgZoomTest(s32 mode) {
    if (mode == 2) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
    } else if (mode == 3) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 30);
    } else if (mode == 4) {
        mapZoomClear();
    } else if (mode == 5) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 0);
    } else if (mode == 6) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.7f, 30);
    } else if (mode == 7) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 3.0f, 30);
    } else if (mode == 8) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 80.0f, fldGlobalBase.focusZ, 1.5f, 9);
    } else if (mode == 9) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 1);
    } else if (mode == 0) {
        if (tzoom_event != 0) {
            tzoom_event = 1;
            tzoom_event_time = 30;
            tzoom_event_cnt = 30;
            mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
            FAKEMATCH_ASM_LIVE(mode);
        } else {
            mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
            FAKEMATCH_ASM_LIVE(mode);
        }
    } else {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 30);
        FAKEMATCH_ASM_LIVE(mode);
    }
}

INCLUDE_ASM(const s32, "code_0013CDE0", fldBgZoomTest2);

INCLUDE_ASM(const s32, "code_0013CDE0", mapZoomSetGoal);

void mapZoomClear(void) {
    mapData75 = 1.0f;
    mapData76 = 1.0f;
    mapZoomClearData = 0.0f;
    mapData77 = 0;
    mapData78 = 0;
    mapZoomSetGoalData11 = 0.0f;
    mapZoomSetGoalData12 = 0.0f;
    mapZoomSetGoalData13 = 512.0f;
    mapZoomSetGoalData14 = 448.0f;
    mapZoomSetGoalData15 = 0.0f;
    mapZoomSetGoalData16 = 0.0f;
    mapZoomSetGoalData17 = 0.0f;
    mapZoomSetGoalData18 = 0.0f;
    mapData79 = 512.0f;
    mapData80 = 448.0f;
    mapZoomSetGoalData19 = 0.0f;
    mapZoomSetGoalData20 = 0.0f;
    mapZoomSetGoalData21 = 0.0f;
    mapZoomSetGoalData22 = 0.0f;
    mapZoomSetGoalData23 = 512.0f;
    mapZoomSetGoalData24 = 448.0f;
    mapZoomSetGoalData25 = 0.0f;
    mapZoomSetGoalData26 = 0.0f;
    mapData81 = 0.0f;
    mapData82 = 0.0f;
    mapData83 = 0.0f;
    mapData84 = 0.0f;
    mapData85 = 0.0f;
    mapData86 = 0.0f;
    Zoom = 1.0f;
    mapData73 = 1.0f;
    mapZoomSetGoalData9 = 1.0f;
    mapZoomSetGoalData10 = 1.0f;
    mapData74 = 0;
    mapBgSetScrollData4 = 0;
    tzoom_event = 0;
    mapZoomSetGoalData = mapData59 = mapZoomSetGoalData6 = mapZoomClearData;
    mapZoomSetGoalData2 = mapZoomSetGoalData4 = mapZoomSetGoalData7 = mapZoomClearData;
    mapZoomSetGoalData3 = mapZoomSetGoalData5 = mapZoomSetGoalData8 = mapZoomClearData;
    mapZoomSetGoalData27 = mapZoomClearData;
    mapZoomSetGoalData28 = mapZoomClearData;
    mapZoomSetGoalData29 = mapZoomClearData;
    mapZoomSetGoalData30 = mapZoomClearData;
    mapData87 = mapZoomClearData;
    mapData88 = mapZoomClearData;
    mapBgSetScrollData7 = mapZoomClearData;
    mapBgSetScrollData8 = mapZoomClearData;
    mapZoomSetGoalData31 = mapZoomClearData;
    mapZoomSetGoalData32 = mapZoomClearData;
    mapData89 = mapZoomClearData;
    mapData90 = mapZoomClearData;
    mapBgSetScrollData9 = mapZoomClearData;
    mapBgSetScrollData10 = mapZoomClearData;
    mapBgSetScrollData11 = mapZoomClearData;
    mapBgSetScrollData12 = mapZoomClearData;
    mapZoomSetGoalData33 = mapZoomClearData;
    mapZoomSetGoalData34 = mapZoomClearData;
    mapData91 = mapZoomClearData;
    mapData92 = mapZoomClearData;
    mapBgSetScrollData13 = mapZoomClearData;
    mapBgSetScrollData14 = mapZoomClearData;
    mapBgSetScrollData15 = mapZoomClearData;
    mapBgSetScrollData16 = mapZoomClearData;
    fscroll_mode = 0;
    mapData61 = 0;
    mapBgSetScrollData3 = 0;
    mapData62 = mapData65 = mapData68 = mapZoomClearData;
    mapData63 = mapData66 = mapData69 = mapZoomClearData;
    mapData64 = mapData67 = mapData70 = mapZoomClearData;
}

/* NON_MATCHING: 10 of 32 words match.  */
#ifdef NON_MATCHING
void mapBgFScrollOn(f32 x, f32 y, f32 z, s32 hokan) {
    if (hokan == 0) {
        mapData62 = x;
        fscroll_mode = 1;
        mapData64 = z;
        mapData61 = 0;
        mapBgSetScrollData3 = 0;
        mapData68 = x;
        mapData65 = x;
        mapData69 = y;
        mapData66 = y;
        mapData70 = z;
        mapData67 = z;
        mapData63 = y;
    } else {
        fscroll_mode = 1;
        mapData62 = mapData59;
        mapData63 = mapZoomSetGoalData4;
        mapData64 = mapZoomSetGoalData5;
        mapData68 = x;
        mapData69 = y;
        mapData70 = z;
        mapData61 = hokan;
        mapData65 = x;
        mapData66 = y;
        mapData67 = z;
        mapBgSetScrollData3 = hokan;
    }
}
#else
INCLUDE_ASM(const s32, "code_0013CDE0", mapBgFScrollOn);
#endif

/* NON_MATCHING: 24 of 32 words match.  */
#ifdef NON_MATCHING
void sndSePlay_sndAt3(s32 hokan) {
    f32 zero;

    if (hokan == 0) {
        fscroll_mode = 0;
        zero = 0.0f;
        mapData61 = 0;
        mapBgSetScrollData3 = 0;
        mapData68 = zero;
        mapData65 = zero;
        mapData62 = zero;
        mapData69 = zero;
        mapData66 = zero;
        mapData63 = zero;
        mapData70 = zero;
        mapData67 = zero;
        mapData64 = zero;
    } else {
        fscroll_mode = 2;
        mapBgSetScrollData3 = hokan;
        mapData62 = mapData59;
        mapData63 = mapZoomSetGoalData4;
        mapData64 = mapZoomSetGoalData5;
        mapData61 = hokan;
        mapData65 = mapData59;
        mapData66 = mapZoomSetGoalData4;
        mapData67 = mapZoomSetGoalData5;
        mapData68 = 0.0f;
        mapData69 = 0.0f;
        mapData70 = 0.0f;
    }
}
#else
INCLUDE_ASM(const s32, "code_0013CDE0", sndSePlay_sndAt3);
#endif

void setCngCameraDir(void) {
    mapBgSetScrollData20 = 1;
}

INCLUDE_ASM(const s32, "code_0013CDE0", mapBgSetScroll);

INCLUDE_ASM(const s32, "code_0013CDE0", mapLeaf11);

void mapBgFree2(void) {
    fldCallMemFreeDelayedDirect4();
    mapCallMemFreeDirect();
    if (fldGlobalBase.word30 != NULL) {
        sdfTexFreeDelayed(fldGlobalBase.word30);
        fldGlobalBase.word44 = -1;
        fldGlobalBase.word30 = NULL;
    }
    mapBgSetScrollReset();
    dds3FilterClear();
    fldGlobalBase.hitMatterIndex = 0;
    fldGlobalBase.cutIdxPrev = 0;
    fldGlobalBase.word4C = 0;
}

void mapAutomapFldClr(s32 field) {
    if (fldGlobalBase.word4C == 1) {
        fldGlobalBase.word4C = 2;
    }
    fldJmsQuestMapLoad_fld(fldGlobalBase.amSBlock, field);
    mapCallFilterSet(field);
    if (fldGlobalBase.word30 != NULL) {
        sdfTexFreeDelayed(fldGlobalBase.word30);
        fldGlobalBase.word44 = -1;
        fldGlobalBase.word30 = NULL;
    }
    fldGlobalBase.word44 = field;
    fldGlobalBase.word30 = NULL;
}

void mapCutSetSub(s32 cut) {
    if (fldGlobalBase.word1DC == 1) {
        if (cut == 27 || cut == 33) {
            fldGlobalBase.cutIdx = 33;
            return;
        }
        fldGlobalBase.word1DC = 0;
    }
    fldGlobalBase.cutIdx = cut;
}

INCLUDE_ASM(const s32, "code_0013CDE0", mapCutSetTblID2);

/* NON_MATCHING: 4 of 34 words match.  */
void mapCutBgLoad(void) {
    s32 cut;

    cut = fldGlobalBase.cutIdx;
    if (cut < 127 && cut != 0 && bg_tex_load_last != cut) {
        bg_tex_load_rec = -1;
        bg_lay_load_rec = -1;
        if (mapData5[fldGlobalBase.amSBlock][cut] != 0) {
            mapAutomapFldClr(cut);
            bg_tex_load_last = fldGlobalBase.cutIdx;
        } else {
            mapBgFree2();
            FAKEMATCH_ASM_LIVE(cut);
        }
    }
}

INCLUDE_ASM(const s32, "code_0013CDE0", mapCutBgLoad_NB);

void fldAllTblPacLoad(void) {
    fileRequest_t *pac;
    sdfPacDecodeResrc_t *node;
    s32 i;
    sdfMemHandle_t *handle;
    void *top;

    i = 0;
    pac = pacLoad(fldStrMapAllAlltblLb);
    pacSync(pac);
    for (node = pac->pac.resrcHead; node != NULL; node = node->next, i++) {
        switch (i) {
        case 2:
            handle = node->resrc;
            top = sdfMemGetPtr(handle);
            memcpy(fldWork6, top, sdfMemGetSize(handle));
            sdfMemFreeDirect(node->resrc);
            break;
        case 3:
            handle = node->resrc;
            top = sdfMemGetPtr(handle);
            memcpy(LightDataTbl, top, sdfMemGetSize(handle));
            sdfMemFreeDirect(node->resrc);
            break;
        case 0:
            handle = node->resrc;
            top = sdfMemGetPtr(handle);
            memcpy(mapEXFloorData, top, sdfMemGetSize(handle));
            sdfMemFreeDirect(node->resrc);
            break;
        case 1:
            handle = node->resrc;
            top = sdfMemGetPtr(handle);
            memcpy(fldAllTblPacLoadData, top, sdfMemGetSize(handle));
            sdfMemFreeDirect(node->resrc);
            break;
        default:
            sdfMemFreeDirect(node->resrc);
            break;
        }
    }
    pacFileHandleFree_(pac);
}

INCLUDE_RODATA(const s32, "code_0013CDE0", fldStrMapAllAlltblLb);

