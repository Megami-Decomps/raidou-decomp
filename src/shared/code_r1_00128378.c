#include "common.h"
#include "evtMoon.h"
#include "dds3ObjectBase.h"
#include "dds3MatterObjectBasic.h"
#include "fldProcess.h"
#include "fldFileResolver.h"
#include "evtStage.h"
#include "fld.h"

#include "dds3obj.h"
#include "field.h"
#include "kernel.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "basic.h"
#include "ps2asm.h"
#include "fpu.h"
#include "dds3UnitObjectBasic.h"
#include "dds3WorldBasic.h"
#include "code_r1_00128378.h"
#include "fldDraw.h"
#include "mapData.h"
#include "sdfMemory.h"
#include "sdfTex.h"
#include "sdfQuat.h"
#include "EventBit.h"
#include "fldMain.h"
#include "dds3SceneBasic.h"
#include "mdlManager.h"
#include "datCalc.h"
#include "dat.h"

/* Cursor advancement returns whether cursorIndex refers to a linked entry. */
typedef u32 Dds3BasicArrayCursorResult;

enum {
    DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED = 0,
    DDS3_BASIC_ARRAY_CURSOR_HAS_ENTRY = 1
};

void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *node);
dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *object);
u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *object);

/* Return the borrowed secondary world node, or NULL when no world is active. */

/* Object data word 8 stores a kind-dependent 32-bit payload. */

/* Set the callback on an object's slot-1 handler; returns whether it exists. */

dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind);

/* Compatible redeclarations keep the broad object header unchanged. */

extern char D_0039FCA0[];
extern dds3Basic_t *fldPlayerObject;
extern u32 fldReleaseUnitPadData[];

struct dds3Basic_t;

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz);
void fldToggleWorldNodeState(s64 clearMode);
extern u64 D_003BAB70[], D_003BAB78[], D_003BAB80[], D_003BAB88[], D_003BAB90[];
extern u64 D_003BAB98[], D_003BABA0[], D_003BABA8[], D_003BABB0[];

typedef struct fldVec4_t {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} fldVec4_t;

void fldReleaseUnitPad(void);

/* Area-state consumption markers are separate from shared game-state flags. */
enum {
    FIELD_TRANSITION_CONSUMED_FLAG = 2,
    FIELD_SECONDARY_SCENE_CONSUMED_FLAG = 4,
    FIELD_PRIMARY_SCENE_CONSUMED_FLAG = 8
};

enum {
    FIELD_ENCOUNTER_ROLL_ENTRY_COUNT = 16
};

#define FIELD_ENCOUNTER_PERCENT_RANGE 100U

extern char D_0039FCA0[], D_0039FCB0[];


extern char D_0039FD30[], D_0039FD40[];
extern char D_0039FD30[], D_0039FD40[];

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

s32 mapGetCircleCircleH(f32 x1, f32 y1, f32 r1, f32 x2, f32 y2, f32 r2, f32 *x3, f32 *y3) {
    f32 dx;
    f32 dy;
    f32 r;
    f32 len;

    dx = x1 - x2;
    dy = y1 - y2;
    r = r1 + r2;
    len = fsqrtf(dx * dx + dy * dy);
    if (len <= r) {
        dx /= len;
        dy /= len;
        *x3 = x2 + dx * r;
        *y3 = y2 + dy * r;
        return 1;
    }
    *x3 = x1;
    *y3 = y1;
    return 0;
}

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetEyeTgtRot);

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallAtan2);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetEyeTgt_Rot_OfsPos);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetTaniVector);

/* For kind-6 objects in mode 4, set mode 3 or clear it to 0.
 * status covers the count, payload mode word and cursor result. */
void fldToggleWorldNodeState(s64 clearMode) {
    dds3BasicArrayHandle_t *valueChain;
    s32 status;
    dds3Basic_t *worldObject;

    valueChain = dds3SceneBasicGetList(dds3GetControlScene(), 6);
    status = dds3GetBasicArrayCount(valueChain);
    if (status == 0) {
        return;
    }
    dds3SetFirstBasicArray(valueChain);
    do {
        worldObject = dds3GetBasicArray(valueChain);
        status = dds3MatterObjectBasicGetAlpMode(worldObject);
        if (status == 4) {
            if (clearMode == 0) {
                dds3MatterObjectBasicSetAlpMode(worldObject, 3);
            }
            else {
                dds3MatterObjectBasicSetAlpMode(worldObject, 0);
            }
        }
        status = dds3SetNextBasicArray(valueChain);
    } while (status != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED);
    dds3FreeBasicArrayHandle(valueChain);
}

#ifdef VERSION_RAIDOU1
void mapAM_MaskClear(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 80; i++) {
        for (j = 0; j < 32; j++) {
            DDS3_GBWK->automapmask[i][j] = 0;
        }
    }
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 32; j++) {
            DDS3_GBWK->automapmask2[i][j] = 0;
        }
    }
}
#else
void NewGameInit(void)
{
    memset(DDS3_GBWK->mesLog, 0, sizeof(DDS3_GBWK->mesLog));
}
#endif

#ifdef VERSION_RAIDOU1
void mapAM_MaskSetCurrent(s32 map, s32 floor) {
    s32 i;

    if (map == 0) {
        map = fldGlobalBase.amSBlock;
    }
    if (map == 4) {
        if (floor == 4) {
            floor = 0;
        }
        if (floor == 5) {
            floor = 0;
        }
    }
    if (map == 8 && floor == 4) {
        floor = 0;
    }
    if (map == 10 && floor == 5) {
        floor = 3;
    }
    if (map == 15) {
        if (floor == 1) {
            floor = 0;
        }
        if (floor == 2) {
            floor = 0;
        }
    }
    if (map == 25) {
        if (floor == 1) {
            floor = 0;
        }
        if (floor == 2) {
            floor = 0;
        }
        if (floor == 3) {
            floor = 0;
        }
    }
    for (i = 0; i < 80; i++) {
        if (mapProcSequenceSubData[i][0] == map && mapProcSequenceSubData[i][1] == floor) {
            fldGlobalBase.automapMaskCurrentIdx = i;
            return;
        }
    }
    for (i = 0; i < 16; i++) {
        if (mapProcSequenceSubData2[i][0] == map && mapProcSequenceSubData2[i][1] == floor) {
            fldGlobalBase.automapMaskCurrentIdx = i + 80;
            return;
        }
    }
    fldGlobalBase.automapMaskCurrentIdx = -1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", mapAM_MaskSetCurrent);

INCLUDE_ASM(const s32, "code_r1_00128378", mapAM_MaskOpen_ID);
#endif

#ifdef VERSION_RAIDOU1
void mapAM_MaskOpen_ID(s32 map, s32 floor, s32 id) {
    s32 idx;

    if (map < 32 && fldGlobalBase.automapMaskCurrentIdx != -1) {
        idx = fldGlobalBase.automapMaskCurrentIdx;
        if (idx < 80) {
            DDS3_GBWK->automapmask[idx][id / 8] |= 1 << (id - (id / 8) * 8);
        } else {
            DDS3_GBWK->automapmask2[idx - 80][id / 8] |= 1 << (id - (id / 8) * 8);
        }
    }
}

void mapRefDDS3GBWK3(s32 map, s32 floor, s32 id) {
    if (map < 32 && fldGlobalBase.automapMaskCurrentIdx != -1) {
        if (fldGlobalBase.automapMaskCurrentIdx < 80) {
            DDS3_GBWK->automapmask[fldGlobalBase.automapMaskCurrentIdx][id / 8] &= ~(1 << (id - (id / 8) * 8));
        } else {
            DDS3_GBWK->automapmask2[fldGlobalBase.automapMaskCurrentIdx - 80][id / 8] &= ~(1 << (id - (id / 8) * 8));
        }
    }
}
/* NON_MATCHING: 36 of 43 words match.  */
#ifdef NON_MATCHING
s32 mapAM_MaskChk_ID(s32 map, s32 floor, s32 id) {
    s32 idx;
    s32 byte;
    s32 bit;
    u8 bits;
    s32 one;

    if (map >= 32) {
        return 1;
    }
    idx = fldGlobalBase.automapMaskCurrentIdx;
    if (idx == -1) {
        return 1;
    }
    if (idx < 80) {
        one = 1;
        byte = id / 8;
        bits = DDS3_GBWK->automapmask[idx][byte];
        bit = id - byte * 8;
    } else {
        one = 1;
        byte = id / 8;
        bits = DDS3_GBWK->automapmask2[idx - 80][byte];
        bit = id - byte * 8;
    }
    return (bits & (one << bit)) != 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", mapAM_MaskChk_ID);
#endif

#else
INCLUDE_ASM(const s32, "code_r1_00128378", mapRefDDS3GBWK3);
INCLUDE_ASM(const s32, "code_r1_00128378", mapAM_MaskChk_ID);

/* body: matched in raidou @ 780f3f57: src/shared/code_r1_00128378.c:287, found, same function 0.96 (words 107/108, calls 1.00, shape 0.88) */
#endif
#ifdef VERSION_RAIDOU1
void mapCallBitCheck(void) {
    s32 i;

    fldGlobalBase.initScript = NULL;
    fldGlobalBase.startScript = NULL;
    for (i = 0; i < 96; i++) {
        if (mapCallBitCheckData[i].map != fldGlobalBase.amSBlock) {
            continue;
        }
        if (mapCallBitCheckData[i].floor != fldGlobalBase.nameU) {
            continue;
        }
        if (mapCallBitCheckData[i].bit != 0) {
            if (mapCallBitCheckData[i].bitOn == 0) {
                if (evtBitCheck(mapCallBitCheckData[i].bit)) {
                    continue;
                }
            } else if (!evtBitCheck(mapCallBitCheckData[i].bit)) {
                continue;
            }
        }
        if (mapCallBitCheckData[i].bit2 != 0) {
            if (mapCallBitCheckData[i].bit2On == 0) {
                if (evtBitCheck(mapCallBitCheckData[i].bit2)) {
                    continue;
                }
            } else if (!evtBitCheck(mapCallBitCheckData[i].bit2)) {
                continue;
            }
        }
        if (mapCallBitCheckData[i].kind == 0) {
            if (fldGlobalBase.initScript != NULL) {
                continue;
            }
            fldGlobalBase.initScript = mapCallBitCheckData[i].name;
        } else {
            if (fldGlobalBase.startScript != NULL) {
                continue;
            }
            fldGlobalBase.startScript = mapCallBitCheckData[i].name;
            if (mapCallBitCheckData[i].once == 0) {
                fldGlobalBase.word2D4 = 1;
            } else {
                fldGlobalBase.word2D4 = 0;
            }
        }
    }
}

void mapRndEncParamSet(void) {
    if (playerBasic != NULL) {
        fldGlobalBase.rndEncMode = 1;
        fldGlobalBase.rndEncFieldID = fldGlobalBase.amSBlock;
        fldGlobalBase.rndEncFloorID = fldGlobalBase.nameU;
        fldGlobalBase.rndEncX = fldGlobalBase.focusX;
        fldGlobalBase.rndEncY = fldGlobalBase.focusY;
        fldGlobalBase.rndEncZ = fldGlobalBase.focusZ;
    }
}

dds3Basic_t *mapGetPlayersBasic(s32 type) {
    switch (type) {
        case 0:
            return playerBasic;
        case 1:
            if (fldGlobalBase.nakamaMode == 0) {
                return playerBasic;
            }
            return raidoBasic;
        case 2:
            fldGlobalBase.aiboRunStop = 1;
            return aiboBasic;
        case 3:
            if (DDS3_GBWK->mapSummonID == 0) {
                return NULL;
            }
            return summonDvBasic;
        default:
            return NULL;
    }
}

INCLUDE_ASM(const s32, "code_r1_00128378", mapSetPlayersVisible);
#else
void mapCallBitCheck(void) {
    s32 i;

    fldGlobalBase.initScript = NULL;
    fldGlobalBase.startScript = NULL;
    for (i = 0; i < MAPCALLBITCHECK_NUM; i++) {
        if (mapCallBitCheckData[i].map != fldGlobalBase.amSBlock) {
            continue;
        }
        if (mapCallBitCheckData[i].floor != fldGlobalBase.nameU) {
            continue;
        }
        if (mapCallBitCheckData[i].bit != 0) {
            if (mapCallBitCheckData[i].bitOn == 0) {
                if (evtBitCheck(mapCallBitCheckData[i].bit)) {
                    continue;
                }
            } else if (!evtBitCheck(mapCallBitCheckData[i].bit)) {
                continue;
            }
        }
        if (mapCallBitCheckData[i].bit2 != 0) {
            if (mapCallBitCheckData[i].bit2On == 0) {
                if (evtBitCheck(mapCallBitCheckData[i].bit2)) {
                    continue;
                }
            } else if (!evtBitCheck(mapCallBitCheckData[i].bit2)) {
                continue;
            }
        }
        if (mapCallBitCheckData[i].kind == 0) {
            if (fldGlobalBase.initScript != NULL) {
                continue;
            }
            fldGlobalBase.initScript = mapCallBitCheckData[i].name;
        } else {
            if (fldGlobalBase.startScript != NULL) {
                continue;
            }
            fldGlobalBase.startScript = mapCallBitCheckData[i].name;
            if (mapCallBitCheckData[i].once == 0) {
                fldGlobalBase.word2D4 = 1;
            } else {
                fldGlobalBase.word2D4 = 0;
            }
        }
    }
}

void mapRndEncParamSet(void) {
    if (playerBasic != NULL) {
        fldGlobalBase.rndEncMode = 1;
        fldGlobalBase.rndEncFieldID = fldGlobalBase.amSBlock;
        fldGlobalBase.rndEncFloorID = fldGlobalBase.nameU;
        fldGlobalBase.rndEncX = fldGlobalBase.focusX;
        fldGlobalBase.rndEncY = fldGlobalBase.focusY;
        fldGlobalBase.rndEncZ = fldGlobalBase.focusZ;
    }
}

dds3Basic_t *mapGetPlayersBasic(s32 type) {
    switch (type) {
        case 0:
            return playerBasic;
        case 1:
            if (fldGlobalBase.nakamaMode == 0) {
                return playerBasic;
            }
            return raidoBasic;
        case 2:
            fldGlobalBase.aiboRunStop = 1;
            return aiboBasic;
        case 3:
            if (DDS3_GBWK->mapSummonID == 0) {
                return NULL;
            }
            return summonDvBasic;
        default:
            return NULL;
    }
}

INCLUDE_ASM(const s32, "code_r1_00128378", mapSetPlayersVisible);
#endif

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 33 of 63 words match.  */
#ifdef NON_MATCHING
s32 mapCheckDying(s32 mode) {
    s32 index;
    s32 status;

    if (mode == 0) {
        if (fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 2) {
            return 0;
        }
        if (datCheckDying(&DDS3_GBWK->playerData.unit)) {
            return 1;
        }
        status = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.unit);
        return status == 6 || status == 11;
    }
    index = datCallGetStockDevilID(DDS3_GBWK->mapSummonID);
    if (index == -1) {
        return 0;
    }
    if (datCheckDying(&DDS3_GBWK->playerData.stock[index])) {
        return 1;
    }
    status = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[index]);
    if (status == 6 || status == 11) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", mapCheckDying);
#endif
#else
INCLUDE_ASM(const s32, "code_r1_00128378", mapCallProcessCheck);
#endif

s32 mapGetRaidouSerialID(void) {
    if (!fldGlobalBase.nakamaMode) {
        return playerBasic->serialID;
    }
    return raidoBasic->serialID;
}

s32 mapGetAiboSerialID(void) {
    if (aiboBasic != NULL && dds3ObjectBaseCheckFlag(aiboBasic, 1)) {
        return aiboBasic->serialID;
    }
    return -1;
}

s32 mapGetAiboSerialID2(void) {
    if (aiboBasic == NULL) {
        return -1;
    }
    return aiboBasic->serialID;
}

s32 mapGetNakamaSerialID(void) {
    if (summonDvBasic != NULL && dds3ObjectBaseCheckFlag(summonDvBasic, 1)) {
        return summonDvBasic->serialID;
    }
    return -1;
}

s32 mapGetNakamaSerialID2(void) {
    if (summonDvBasic == NULL) {
        return -1;
    }
    return summonDvBasic->serialID;
}

void effBTLFieldColorDraw(void) {
    if (aiboBasic != NULL) {
        mapAiboFoceSet(aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.y, aiboBasic->poseset->pos.z);
    }
}

INCLUDE_ASM(const s32, "code_r1_00128378", effRefDvBasic2);

void mapNakamaOikakeBuffReset2(void) {
    if (summonDvBasic == NULL && playerBasic != NULL) {
        mapSummonDvFoceSet(playerBasic->poseset->pos.x, playerBasic->poseset->pos.y, playerBasic->poseset->pos.z);
    }
}

INCLUDE_RODATA(const s32, "code_r1_00128378", mapGetEyeTgtRotTable);

void mapRaidouBackUpPosReset(void) {
    f32 axis[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

    if (fldGlobalBase.nakamaMode) {
        SDF_VU0_LOADVECTOR(axis);
#ifdef VERSION_RAIDOU1
        fldGlobalBase.word190 = fldGlobalBase.amSBlock;
        fldGlobalBase.word194 = fldGlobalBase.nameU;
        fldGlobalBase.float184 = raidoBasic->poseset->pos.x;
        fldGlobalBase.float188 = raidoBasic->poseset->pos.y;
        fldGlobalBase.float18C = raidoBasic->poseset->pos.z;
#else
        fldGlobalBase.word1C8 = fldGlobalBase.amSBlock;
        fldGlobalBase.word1CC = fldGlobalBase.nameU;
        fldGlobalBase.float1BC = raidoBasic->poseset->pos.x;
        fldGlobalBase.float1C0 = raidoBasic->poseset->pos.y;
        fldGlobalBase.float1C4 = raidoBasic->poseset->pos.z;
#endif
        dds3GetRotPoseSet(raidoBasic);
#ifdef VERSION_RAIDOU1
        fldGlobalBase.float198 = (EulerAngleAxisY() * 180.0f) / 3.14f;
#else
        fldGlobalBase.float1D0 = (EulerAngleAxisY() * 180.0f) / 3.14f;
#endif
    } else {
        SDF_VU0_LOADVECTOR(axis);
        fldGlobalBase.focusX = playerBasic->poseset->pos.x;
        fldGlobalBase.focusY = playerBasic->poseset->pos.y;
        fldGlobalBase.focusZ = playerBasic->poseset->pos.z;
        dds3GetRotPoseSet(playerBasic);
        fldGlobalBase.floatC4 = fldGlobalBase.floatC0 = (EulerAngleAxisY() * 180.0f) / 3.14f;
        mapHamidasiCheck();
        fldPlayerForceSetPos_NoHokan(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
    }
}

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallGetRotPoseSet);

INCLUDE_ASM(const s32, "code_r1_00128378", mapCallGetRotPoseSet2);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCtnGetSub);

INCLUDE_ASM(const s32, "code_r1_00128378", fldSetSceneParamBattleReturnSub);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "code_r1_00128378", tipsProcessCheck);

INCLUDE_ASM(const s32, "code_r1_00128378", tipsGetSkilZeniSagasiCheckSub);

INCLUDE_ASM(const s32, "code_r1_00128378", tipsGetSkilZeniSagasiCheckSub2);

INCLUDE_ASM(const s32, "code_r1_00128378", tipsGetSkilTakaraSagasiCheckSub);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetSkilTakaraSagasiCheckSub);

INCLUDE_ASM(const s32, "code_r1_00128378", mapGetSkilTakaraSagasiCheckSub2);

INCLUDE_ASM(const s32, "code_r1_00128378", mapTakaraChkOpen);

INCLUDE_ASM(const s32, "code_r1_00128378", mapLeaf);

INCLUDE_ASM(const s32, "code_r1_00128378", fldGbLocationSavePointStore);

INCLUDE_ASM(const s32, "code_r1_00128378", mapChkTakaraEX);

#endif
INCLUDE_RODATA(const s32, "code_r1_00128378", mapCallGetRotPoseSetTable);

INCLUDE_RODATA(const s32, "code_r1_00128378", mapCallGetRotPoseSet2Table);

#ifdef VERSION_RAIDOU1
void fldFirstInit(void) {
    sdfMemHandle_t *file;
    TMX_HEADER_t *tmx;

    tmx = NULL;
    fldDrawInit();
    mapCutFileLoad();
    fldCallFptodp();
    fldGlobalBase.word30 = 0;
    fldGlobalBase.texture2B0 = NULL;
    fldGlobalBase.texSlot[0].texture = NULL;
    fldGlobalBase.texSlot[0].file = NULL;
    sdfRandomize(&fldRandomBuf, 123456);
    fldCallMemset4();
    fldAllTblPacLoad();
    fldAllTblLoad();
    file = sdfDevLoadFileEx("/map/all/shadow01.tmx", (u32 *)&tmx, NULL);
    fldGlobalBase.texture220 = sdfTexSet(tmx);
    sdfMemFreeDelayedDirect(file);
    fldData9 = dds3CallMatNew2(fldGlobalBase.texture220);
    mapEffEplInit();
    mefMistFastCreate();
}

void mapCutFileLoad(void) {
    playerBasic = NULL;
    cameraBasic = NULL;
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldReturnSavePointPositionRestore);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallChkTakaraEX);

INCLUDE_ASM(const s32, "code_r1_00128378", sdfMcPollMkdir);

INCLUDE_ASM(const s32, "code_r1_00128378", sdfLoadMapFromMemorySub);

void sbtlRefDDS3GBWK(void) {
    DDS3_GBWK->unk48FC = 0;
}

void sbtlCountClear(void)
{
    sbtlCheckEndEscape();
    fldPlayerAFree();
}

INCLUDE_ASM(const s32, "code_r1_00128378", sbtlTitleNewGameStartSub);

INCLUDE_ASM(const s32, "code_r1_00128378", fclCombineDraw);

/* body: matched in raidou @ 780f3f57: src/shared/code_r1_00128378.c:491, found, same function 0.94 (words 51/49, calls 0.89, shape 0.98) */
void fclMapAllShadow(void) {
    sdfMemHandle_t *file;
    TMX_HEADER_t *tmx;

    tmx = NULL;
    fldDrawInit();
    mapCutFileLoad();
    fldCallFptodp();
    fldGlobalBase.word30 = 0;
    fldGlobalBase.texture2B0 = NULL;
    fldGlobalBase.texSlot[0].texture = NULL;
    fldGlobalBase.texSlot[0].file = NULL;
    sdfRandomize(&fldRandomBuf, 123456);
    fldCallMemset4();
    fldAllTblPacLoad();
    fldAllTblLoad();
    file = sdfDevLoadFileEx("/map/all/shadow01.tmx", (u32 *)&tmx, NULL);
    fldGlobalBase.texture220 = sdfTexSet(tmx);
    sdfMemFreeDelayedDirect(file);
    fldData9 = dds3CallMatNew2(fldGlobalBase.texture220);
    mapEffEplInit();
    nbCameraInit();
    mefMistFastCreate();
}

void mapCutFileLoad(void) {
    playerBasic = NULL;
    cameraBasic = NULL;
}
#endif

#ifdef VERSION_RAIDOU1
void mapSetSceneParam(fldSceneParam_t *param, s32 field, s32 floor, s32 point) {
    param->flag = 0;
    param->loadField = field;
    param->loadFloor = floor;
    param->loadPoint = point;
    param->loadMode = 0;
    memset(param->FoceScript, 0, 0x10);
    fldRefTexLoadLast();
    fldGlobalBase.cutIdx = 0;
    fldGlobalBase.cutIdxPrev = 0;
    fldGlobalBase.cutState = 0;
    fldGlobalBase.playerSeTable = 0;
    fldGlobalBase.hitMatterIndex = 0;
    fldGlobalBase.focusX = 0.0f;
    fldGlobalBase.focusY = 0.0f;
    fldGlobalBase.focusZ = 0.0f;
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", mapSetSceneParam);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", mapSetSceneParam_BattleReturn);

void fldElvReset(fldSceneParam_t *param, s32 field) {
    param->flag = 0;
    param->loadField = field;
    param->loadFloor = -1;
    param->loadPoint = -1;
    param->loadMode = 0;
    fldRefTexLoadLast();
}

#ifdef VERSION_RAIDOU1
void fldReadyScene(void) {
    evtLoadArea(gfldSceneParam.loadField, gfldSceneParam.loadFloor);
    if (fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 2) {
        fldGlobalBase.nakamaTaskOff = 1;
    } else {
        fldGlobalBase.nakamaTaskOff = 0;
    }
    fldSetUnit();
    if (!evtBitCheck(184) || evtBitCheck(206)) {
        if (fldGlobalBase.amSBlock == 1 && fldGlobalBase.nameU == 0 && (!evtBitCheck(177) || evtBitCheck(179))) {
            mapCtnLoad();
        }
        if (fldGlobalBase.amSBlock == 2 && (fldGlobalBase.nameU == 0 || fldGlobalBase.nameU == 2)) {
            mapCtnLoad();
        }
        if (fldGlobalBase.amSBlock == 3) {
            if (fldGlobalBase.nameU == 0) {
                mapCtnLoad();
            }
            if (fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 1 && (!evtBitCheck(65) || evtBitCheck(96))) {
                mapCtnLoad();
            }
        }
        if (fldGlobalBase.amSBlock == 4 && fldGlobalBase.nameU == 0) {
            mapCtnLoad();
        }
        if (fldGlobalBase.amSBlock == 7 && fldGlobalBase.nameU == 0 && (!evtBitCheck(131) || evtBitCheck(144))) {
            mapCtnLoad();
        }
    }
    fldRegistUnitPad();
    fldSetUnitFollowCamera();
    dds3SetActiveUnitSceneBasic(dds3GetControlScene(), playerBasic);
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldCallLoadArea);
#endif

#ifdef VERSION_RAIDOU1
s32 PlayerGetMdlLoadID(char *path) {
    s32 type;

    if (evtBitCheck(161) && !evtBitCheck(163)) {
        if (fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 2) {
            fldGlobalBase.wordE0 = 3;
            type = 7;
        } else {
            fldGlobalBase.wordE0 = 2;
            type = 6;
        }
    } else if (fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 2) {
        type = 8;
        fldGlobalBase.wordE0 = 1;
    } else {
        type = 0;
        fldGlobalBase.wordE0 = 0;
    }
    if (path != NULL) {
        switch (type) {
        case 0:
            sprintf(path, "/model/field/000.LB");
            break;
        case 6:
            sprintf(path, "/model/field/000_j.LB");
            break;
        case 7:
            sprintf(path, "/model/field/naked_f_j.LB");
            break;
        case 8:
            sprintf(path, "/model/field/naked_f.LB");
            break;
        }
    }
    return type;
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", PlayerGetMdlLoadID);
#endif

void fldPlayerALoad(void) {
    char path[32];

    if (fldPlayerAnimData == NULL) {
        PlayerGetMdlLoadID(path);
        fldPlayerAnimData = sdfDevLoadFileEx(path, (u32 *)&gfldPlayerPtr, &gfldPlayerSizeBk);
    }
}

s32 fldPlayerALoad_OnDataCheck(void) {
    return fldPlayerAnimData != NULL;
}

void fldPlayerALoadStart(void) {
    char path[32];

    if (fldPlayerAnimData == NULL) {
        fldPlayerAnimData = NULL;
        PlayerGetMdlLoadID(path);
        gMapCash_pacHandle = pacLoad(path);
    }
}

s32 fldPlayerALoadEndCheck(void) {
    sdfPacDecodeResrc_t *resrc;
    s32 n = 0;

    if (fldPlayerAnimData != NULL) {
        return 1;
    }
    if (pacCheckEnd(gMapCash_pacHandle) == 0) {
        return 0;
    }
    for (resrc = gMapCash_pacHandle->pac.resrcHead; resrc != NULL; resrc = resrc->next, n++) {
        if (n == 0) {
            fldPlayerAnimData = resrc->resrc;
            gfldPlayerPtr = resrc->data;
            gfldPlayerSizeBk = sdfMemGetSize(fldPlayerAnimData);
        }
        else {
            sdfMemFreeDirect(resrc->resrc);
        }
    }
    pacFileHandleFree_(gMapCash_pacHandle);
    return 1;
}

void fldPlayerAFree(void) {
    if (fldPlayerAnimData != NULL) {
        sdfMemFreeDirect(fldPlayerAnimData);
        fldPlayerAnimData = NULL;
    }
}

#ifdef VERSION_RAIDOU1
void fldJDataLoad(void) {
    void *src;
    void *dst;

    fldPlayerALoad();
    fldJDataLoadData2 = gfldPlayerSizeBk;
    effQueuedResourceNameRecord3 = sdfMemAllocDirect(gfldPlayerSizeBk);
    src = sdfMemGetPtr(fldPlayerAnimData);
    dst = sdfMemGetPtr(effQueuedResourceNameRecord3);
    memcpy(dst, src, fldJDataLoadData2);
    gfldPlayerPtr = dst;
    mdlPackDecode(2, PlayerGetMdlLoadID(NULL), 0x101, gfldPlayerPtr, fldJDataLoadData2);
    sdfMemFreeDelayedDirect(effQueuedResourceNameRecord3);
    effQueuedResourceNameRecord3 = NULL;
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldCallPlayerALoad);
#endif

void fldCallMemFreeDelayedDirect(void) {
    if (effQueuedResourceNameRecord3 != NULL) {
        sdfMemFreeDelayedDirect(effQueuedResourceNameRecord3);
        effQueuedResourceNameRecord3 = NULL;
    }
    fldPlayerAFree();
}

void fldCallMemFreeDelayedDirect2(void) {
    if (fldData11 != NULL) {
        sdfMemFreeDelayedDirect(fldData11);
        fldData11 = NULL;
        nbResultFreeTexturesData = NULL;
        nbResultFreeTexturesData2 = 0;
    }
    fldNakamaCashLoad_EndCheckData = 0;
}

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_Start);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_Resrc);

INCLUDE_ASM(const s32, "code_r1_00128378", nbResultFreeTextures);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_Block);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_RndEncRetStart);

INCLUDE_ASM(const s32, "code_r1_00128378", fldNakamaCashLoad_EndCheck);

s32 fldSetUnitSub(void) {
    return fldData11 != NULL;
}

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallMemFreeDelayedDirect3);

INCLUDE_ASM(const s32, "code_r1_00128378", fldModelNpcLb);

INCLUDE_ASM(const s32, "code_r1_00128378", fldAiboCashLoad_Resrc);

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallPackDecode);

INCLUDE_ASM(const s32, "code_r1_00128378", fldAiboCashLoad_Block);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_00128378", fldAiboCashLoad_RndEncRetStart);
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldRefBasic);
#endif

INCLUDE_ASM(const s32, "code_r1_00128378", fldAiboCashLoad_EndCheck);

s32 fldSetUnitSub2(void)
{
    return fldData12 != NULL;
}

#ifdef VERSION_RAIDOU1
void cmpModelFreeResource(void) {
    if (playerBasic != NULL) {
        mapCtnFree();
        cameraBasic = NULL;
        playerBasic = NULL;
        playerModel = NULL;
        raidoBasic = NULL;
        raidoModel = NULL;
        aiboBasic = NULL;
        aiboModel = NULL;
        summonDvBasic = NULL;
        summonDvModel = NULL;
        fldCallMemFreeDelayedDirect();
        fldCallMemFreeDelayedDirect2();
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00128378", fldLoadMapResourceSub);
#endif

#ifdef VERSION_RAIDOU1
void fldAllShutDown(void) {
    cmpModelFreeResource();
    evtFreeStage();
    fldProcShutDown();
}
#else
s32 fldLoadMapResourceSub2(void)
{
    return mapCallLoadCheckMidi() != 0;
}

INCLUDE_ASM(const s32, "code_r1_00128378", fldCallCtnFree);

INCLUDE_ASM(const s32, "code_r1_00128378", fldAllShutDown);
#endif

void fldRegistUnitPad(void) {
    if (playerBasic != NULL) {
        fldEncountSetFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(playerBasic, fldLeaf6);
    }
    fldGlobalBase.noEncountCnt = 4;
    fldGlobalBase.noInpPlCnt = 4;
    fldGlobalBase.ifDrawOn = 1;
    mapWaitCbScreenFadeBSub(0);
    fldGlobalBase.aiboRunStop = 0;
}

void fldReleaseUnitPad(void) {
    if (fldPlayerObject != 0) {
        fldEncountClearFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(fldPlayerObject, 0);
    }
    fldReleaseUnitPadData[0] = 4;
}
INCLUDE_SDATA(const s32, "code_r1_00128378", playerModel);

INCLUDE_SDATA(const s32, "code_r1_00128378", raidoBasic);

INCLUDE_SDATA(const s32, "code_r1_00128378", raidoModel);

INCLUDE_SDATA(const s32, "code_r1_00128378", aiboBasic);

INCLUDE_SDATA(const s32, "code_r1_00128378", aiboModel);

INCLUDE_SDATA(const s32, "code_r1_00128378", summonDvBasic);

INCLUDE_SDATA(const s32, "code_r1_00128378", summonDvModel);

INCLUDE_SDATA(const s32, "code_r1_00128378", cameraBasic);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData9);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "code_r1_00128378", mapSetSceneParam_BattleReturnData);
#else
INCLUDE_SDATA(const s32, "code_r1_00128378", fldReturnSavePointPositionRestoreData);

INCLUDE_SDATA(const s32, "code_r1_00128378", nbData21);

INCLUDE_SDATA(const s32, "code_r1_00128378", Process_WarpIdx);
#endif

INCLUDE_SDATA(const s32, "code_r1_00128378", fldPlayerAnimData);

INCLUDE_SDATA(const s32, "code_r1_00128378", effQueuedResourceNameRecord3);

INCLUDE_SDATA(const s32, "code_r1_00128378", gfldPlayerPtr);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldJDataLoadData2);

INCLUDE_SDATA(const s32, "code_r1_00128378", gfldPlayerSizeBk);

INCLUDE_SDATA(const s32, "code_r1_00128378", gMapCash_pacHandle);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData11);

INCLUDE_SDATA(const s32, "code_r1_00128378", nbResultFreeTexturesData);

INCLUDE_SDATA(const s32, "code_r1_00128378", nbResultFreeTexturesData2);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldNakamaCashLoad_ResrcData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldNakamaCashLoad_EndCheckData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData12);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldCallPackDecodeData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldCallPackDecodeData2);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldAiboCashLoad_ResrcData);

INCLUDE_SDATA(const s32, "code_r1_00128378", fldData13);

