#include "common.h"

/* Declarations from smt3-decomp src/smt3/pb/pbGame.c */
#include "common.h"

#include "sdfFrame.h"

#include "sdk_libgcc.h"

#include "pbRecord.h"

#include "kernel.h"

#include "libsdf.h"

#include "scr.h"

#include "model.h"

#include "draw3d.h"

#include "ps2asm.h"

extern u32 dds3GetKernelCount(void);

typedef struct pbProcHandle_t pbProcHandle_t;

typedef struct { s32 value; } pbPanelValue_t;

typedef struct { s32 code, unk04, timer; } pbErrorWork_t;

typedef struct pbGroupDB_t pbGroupDB_t;

typedef struct
{
    s32 flg;
    s16 x, y;
    s32 dir;
    u8 dmy[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyChar_t;

typedef struct { s16 number, label; } pbStageEntry_t;

typedef struct { u16 blk[20][20]; } pbMapBlock_t;

typedef struct { u8 unk00[0x10]; pbMapBlock_t map; } pbMapArea_t;

typedef struct
{
    s32 pcharNum;
    u8 unk004[8];
    pbMapArea_t area;
    s32 charIndex;
    void *baseTex;
    void *tex344, *tex348, *tex34C;
    sdfMemHandle_t *mapData;
    u8 *giveupScript;
    s16 level, stage;
    u8 unk35C[4];
    struct pbProcHandle_t *menuProc;
    struct pbProcHandle_t *helpProc;
    u8 reserved368[4];
    s32 drawCount;
} pbMapWork_t;

typedef struct { u8 unk00[4]; u16 group; } pbGroupEntry_t;

struct pbGroupDB_t { u32 count; pbGroupEntry_t *entry[1]; };

typedef struct
{
    s32 flg;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbExit_t;

typedef struct
{
    f32 scale;
    u8 unk04[0xC];
    f32 eye[4], target[4];
} pbViewCamWork_t;

typedef struct { u8 unk00[8]; s32 modelIndex; } pbPanelModel_t;

typedef struct
{
    u16 kind;
    s16 x, y;
    u16 outline_num;
    u8 reserved08[8];
    f32 outlineVertices[8][4];
} pbBlockEntry_t;

typedef struct
{
    s32 value;
    u16 grpID;
    u16 num;
    u8 unk08[8];
    u16 kind;
    s16 cellX, cellY;
    u8 unk16[0x8A];
    sdfMemHandle_t *blockMem;
    /* Unrecovered bytes between the allocation handle and locked block pointer. */
    u8 blockStorageGap[0xC];
    pbBlockEntry_t *blk;
    union {
        sdfVector4_t rotation;
        struct { f32 rotationX, rot, rotationZ; u8 reservedC0[4]; };
    };
    f32 x, y, z;
    u8 reservedD0[4];
    u32 colors[8];
    u8 reservedF4[0xC];
} pbGroupData_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbGroupData_t *block; } pbRoundBlock_t;

typedef struct { s16 dir; u16 step; f32 speed; s32 unk08; } pbMoveCharWork_t;

typedef struct { s32 dir; f32 angle; } pbRoundCamWork_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { s16 x, y; } pbGoalPos_t;

typedef struct { u8 unk00[8]; pbGoalPos_t pos; } pbGoal_t;

typedef struct
{
    s16 clearBit;
    s16 unk02;
    pbStageEntry_t stage[1];
} pbLevel_t;

/* Common panel work header: its constructor installs process and data links. */
struct pbProcHandle_t
{
    dds3ProcessID_t *id;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyChar_t *chara;
            pbRecord_t *record; pbExit_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; pbMoveCharWork_t *moveChar;
            pbRoundCamWork_t *roundCam; } data;
};

extern f32 cameraAngle, cameraRadius, cameraDegree, cameraDegreeInit;

extern dds3ProcessID_t *scr_msg;

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern pbLevel_t *pbLevelTable[];

extern pbMapWork_t *pbMapWork;

extern pbGoal_t *goal;

extern pbProcHandle_t *record;

extern void sdfMatFreeDelayed(sdfMat_t *aMat);

extern void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern void pbGotoNewStage(void);

typedef void (*pbCharacterFunc_t)(void);

extern pbCharacterFunc_t pbCharacterTable[];

extern pbProcHandle_t *blockDB;

extern u32 current_char;

extern f32 pbDirVectorTable[];

extern s32 pbPopRecord(pbProcHandle_t *p, pbProcHandle_t **c, pbProcHandle_t *blkDB);

extern pbProcHandle_t *pbStartErrorDraw(pbProcHandle_t *parent, s32 eCode, s32 frame);

extern void *sdfChipAllocDirect(u32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern s32 ClearProc(dds3ProcessID_t *id);

extern void DeleteClearProc(dds3ProcessID_t *id);

extern void rsetOtOfs(s32 otofs);

extern void rsetAlpha(s32 mode);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, u32, void *);

extern void *memset(void *a, s32 b, size_t c);

extern s32 pb_draw_map_cursor_Process(dds3ProcessID_t *id);

extern void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id);

extern char pbDrawMapCursorProcName[];

extern const char pbViewCameraProcName[];

extern char pbBlockDbProcName[], pbDrawMapbaseProcName[];

extern s32 pb_proc_block_DB_Process(dds3ProcessID_t *id);

extern void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id);

extern s32 viewModeCamera(dds3ProcessID_t *id);

extern s32 moveChar(dds3ProcessID_t *id);

extern void deleteMoveChar(dds3ProcessID_t *id);

extern const char pbMoveCharProcName[], pbMoveBlockProcName[], pbGameProcName[], pbMoveCameraProcName[];

extern s32 roundBlock(dds3ProcessID_t *id);

extern u8 degree_mode;

extern s8 dds3PadButtonState[];

extern u8 pbGameKeyState;

extern u32 RotateBit(u32 val, s32 loop);

extern void pbRoundCamera(s32 dir);

extern s16 pbMyCharPosX, pbMyCharPosY;

extern sdfVector4_t pbSavedViewCamera[2];

extern void pbSetMapCursorFlag(pbProcHandle_t *p, s32 flag);

extern void pbSetGameCamera4(s16 x, s16 y);

extern void pbRotateXCamera(f32 angle);

extern void pbRotateYCamera(f32 angle);

extern u8 pbGetKeyAction(u8 press);

extern void dds3SetQuake(s32 frame, s32 range);

extern void pbDeleteGroupEntry(pbProcHandle_t *db, u32 grpID);

extern void pbRebuildOutline(pbGroupData_t *p);

extern pbProcHandle_t *pbStartMoveBlock(pbProcHandle_t *parent, pbGroupData_t *grp, s32 dir);

typedef struct { s16 x, y; } pbKeyEntry_t;

extern s16 pbKeyTable[][2];

extern void pbGetMyCharcterPos2(pbProcHandle_t *p, s16 *x, s16 *y);

extern void pbMoveMyCharcter(pbProcHandle_t *p, f32 x, f32 y);

extern s32 game(dds3ProcessID_t *id);

extern s32 roundAutoCamera(dds3ProcessID_t *id);

extern u8 pbGetKeyMode(void);

extern const char pbGameDebugOwnerName[];

extern void deleteGame(dds3ProcessID_t *id);

extern void pbLoadSystemData(s32 num);

extern pbProcHandle_t *pbStartBlockDataBase(pbProcHandle_t *parent);

extern pbProcHandle_t *pbInitMapBaseDraw(pbProcHandle_t *parent, s32 sw);

extern pbProcHandle_t *pbInitExitDraw(pbProcHandle_t *parent);

extern pbProcHandle_t *pbStartRecord(pbProcHandle_t *parent);

extern pbProcHandle_t *pbStartDrawMapCursor(pbProcHandle_t *parent);

extern pbProcHandle_t *bmap, *mapCursor;

extern s32 D_0054F5F0;

extern pbMapBlock_t pbMapSnapshot;

extern s32 moveBlock(dds3ProcessID_t *id);

extern void deleteMoveBlock(dds3ProcessID_t *id);

extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern u32 pbAddRecord_flor(pbProcHandle_t *p, pbGroupData_t *grp);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frame);

extern u32 pbAddRecord_blk(pbProcHandle_t *p, pbGroupData_t *grp);

extern void deleteViewModeCamera(dds3ProcessID_t *id);

extern const char pbClearProcName[];

extern char pbDrawBgProcName[], pbLoadMapDataProcName[];

extern void updateMotion(void);

extern void pbFreeGroupHandles(pbBlockDB_t *db);

extern void pbFreeGroupEntry(pbGroupData_t *entry);

extern void fclClearFlag(s8 flag);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern void setRoundBlockData(pbGroupData_t *grp, s32 dir);

extern void func_001D2D00(pbProcHandle_t *panel, s32 arg);

extern pbProcHandle_t *mychar[];

extern pbProcHandle_t *moveB;

extern pbProcHandle_t *roundCamera;

extern const char pbRootProcName[];

extern u32 pbPushRecord(pbProcHandle_t *p);

extern u32 pbMyCharacterDispStat(pbProcHandle_t *p);

extern dds3ProcessID_t *pbStartGiveupConfirm(void);

extern s8 pbCheckPopRecord(pbProcHandle_t *p);

extern s8 pbCheckPreviousRecord(void);

extern u8 pbRotLKeyAct(void);

extern u8 pbRotRKeyAct(void);

extern pbProcHandle_t *moveViewCamera;

extern f32 aViewEye[4], aViewTarget[4];

/* Three rows initialized by pbLoadTextures. */
extern f32 pbGameCameraParam[3][4];

/* Three target-offset rows. */
extern f32 pbCameraVec[3][4];

extern void pbSetGameCamera1(void);

extern void pbSetGameCamera2(void);

extern void pbPreviousRecord(void);

extern void *pbFindGroupEntry(pbGroupDB_t *db, u32 group);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

s32 getCharacterNum(void);

void pbNextCharacter(void);

void updateMotion(void);

extern s32 pbFindProcHandle(s16 px, s16 py);

extern s8 goalHitCheck(s16 x, s16 y);

extern u32 pushCheck(s32 direction, u32 grpID, pbGroupData_t **pGroup);

s32 roundCheck(s32 direction, u32 grpID, pbGroupData_t **pGroup);

s32 hitCheck(s32 direction, pbGroupData_t **pGroup);

extern void *pbSearchGroupDB(pbProcHandle_t *p, u32 grpID);

extern s32 pbFindProcHandle(s16 px, s16 py);

u32 pushCheck(s32 direction, u32 grpID, pbGroupData_t **pGroup);

s32 roundDirCheck(s32 direction, u32 groupID, u32 *stepCount);

s8 hallCheck(pbGroupData_t *grp);

s32 pbFindProcHandle(s16 px, s16 py);

s8 goalHitCheck(s16 x, s16 y);

s32 gameKey0(void);

s32 gameKey1(void);

u32 RotateBit(u32 val, s32 loop);

s32 gameKey2(void);

extern const char pbRootProcName[16];

extern const char pbGameDebugOwnerName[24];

void deleteGame(dds3ProcessID_t *id);

/* Initializer fields in the work allocation; the map starts at +0x1C. */
typedef struct
{
    s32 pcharNum;
    u8 pcharReg[4];
    s16 pcharPos[4][2];
    s16 exitX, exitY;
    pbMapBlock_t map;
} pbMapWorkInitView_t;

extern s32 pbMoveCharCooldown;

extern u32 pbMoveCharList;

extern u32 pbMapIsValid(void);

extern void dds3FadeIn2(s32 frame);

extern void pbSetViewRotation(f32 angle, f32 radius, f32 degree);

extern void pbSetExitPos(pbProcHandle_t *panel, s32 x, s32 y);

extern void pbSetExitDisp(pbProcHandle_t *panel, s8 sw);

extern pbProcHandle_t *pbInitMyCharacterDraw(pbProcHandle_t *parent, u32 color, u32 motion);

extern void pbSetPanelValue(pbProcHandle_t *panel, s8 value);

extern void pbGetCellY(pbProcHandle_t *panel, s16 cx, s16 cy);

extern void pbClearRecord(pbProcHandle_t *panel);

extern void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern s32 fclSetWorkValue(s8 index, s32 value);

extern s16 GetStageScriptLabel(void);

extern void pbInitGameCamera(void);

void pbInitGame(pbProcHandle_t *panel, dds3ProcessID_t *id);

s32 moveChar(dds3ProcessID_t *id);

extern void pbMoveMyCharcter2(pbProcHandle_t *p, s16 x, s16 y);

extern void pbSetPanelValue(pbProcHandle_t *panel, s8 value);

extern pbProcHandle_t *pbStartClear(pbProcHandle_t *parent);

extern void pbClearRecord(pbProcHandle_t *panel);

extern void pbNextCharacter(void);

extern void func_001D2D00(pbProcHandle_t *panel, s32 arg);

extern s32 getCharacterNum(void);

extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 timer);

extern pbProcHandle_t *pbGetMoveChar(void);

extern pbProcHandle_t *moveC;

extern u32 pbMoveCharList;

extern pbMapWork_t *pbMapWork;

extern pbProcHandle_t *record;

void deleteMoveChar(dds3ProcessID_t *id);

void setMoveBlockData(pbGroupData_t *grp, s32 dir);

void rotXY(s16 *x, s16 *y, s32 dir);

void setRoundBlockData(pbGroupData_t *grp, s32 dir);

s32 roundBlock(dds3ProcessID_t *id);

void deleteRoundBlock(dds3ProcessID_t *id);

s32 roundAutoCamera(dds3ProcessID_t *id);

void deleteRoundAutoCamera(dds3ProcessID_t *id);

s32 viewModeCamera(dds3ProcessID_t *id);

void deleteViewModeCamera(dds3ProcessID_t *id);

s32 ClearProc(dds3ProcessID_t *id);

void DeleteClearProc(dds3ProcessID_t *id);

void CalcDegreeMode(void);

s16 GetStageNumber(void);

s16 GetStageScriptLabel(void);

s16 GetAllStageClearBit(void);

pbProcHandle_t *pbStartGame(pbProcHandle_t *parent);

pbProcHandle_t *pbStartMoveChar(pbProcHandle_t *parent, s32 dir, u32 scl);

pbProcHandle_t *pbStartMoveBlock(pbProcHandle_t *parent, pbGroupData_t *grp, s32 dir);

pbProcHandle_t *pbStartRoundBlock(pbProcHandle_t *parent, pbGroupData_t *grp, s32 dir);

pbProcHandle_t *pbStartAutoRoundCamera(pbProcHandle_t *parent, s32 dir);

pbProcHandle_t *pbStartViewModeCamera(pbProcHandle_t *parent);

void pbRoundCamera(s32 dir);

void pbSetGameCamera0(void);

extern void pbGetMyCharcterPos(pbProcHandle_t *character, sdfVector4_t *position);

void pbSetGameCamera1(void);

void pbSetGameCamera2(void);

extern s32 pbGetMyCharcterRot(pbProcHandle_t *character);

void pbSetGameCamera3(void);

void pbSetGameCamera4(s16 x, s16 y);

void pbInitGameCamera(void);

s8 pbCheckPreviousRecord(void);

void pbPreviousRecord(void);

pbProcHandle_t *pbStartClear(pbProcHandle_t *parent);

dds3ProcessID_t *pbStartGiveupConfirm(void);

void pbGetCameraParam(f32 *out);

void func_001D8EA0(f32 angle);

void pbSetCameraRadius(f32 radius);

void pbSetCameraDegree(f32 degree);

void pbSetCameraDegreeInit(f32 degree);

dds3ProcessID_t *pbGetMessageScriptProc(void);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 timer);

extern void evtBitOn(s32 no);

#include "sdk_libc.h"

extern pbProcHandle_t *pbGetMoveChar(void);

extern const char pbGameProcName[16];

extern const char pbMoveCharProcName[24];

extern const char pbMoveBlockProcName[24];

extern const char pbMoveCameraProcName[24];

extern const char pbViewCameraProcName[32];

extern const char pbClearProcName[16];

void pbGotoNewStage(void);

void pbRoundStageNext(s16 *plevel, s16 *pstage);

void pbRoundStagePrev(s16 *plevel, s16 *pstage);

void *pbFindGroupEntry(pbGroupDB_t *db, u32 group);

extern void pbRebuildOutline(pbGroupData_t *group);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);

extern void *sdfMemLock(sdfMemHandle_t *handle);

void pbAddGroupEntry(pbBlockDB_t *db);

void pbFreeGroupEntry(pbGroupData_t *entry);

void pbFreeGroupHandles(pbBlockDB_t *db);

u8 pbGetBlockAt(s16 x, s16 y, s16 grp);

typedef struct { sdfVector4_t v[4]; } pbMatrix_t;

extern void pbAddGroupEntry(pbBlockDB_t *db);

extern s32 sdfGrAllocPacketBuff(s32 size);

extern void sdfDrawTagInit(sdfDrawTag_t *tag);

extern void sdf3DPrimLinkMat(sdfDrawTag_t *tag, sdfMat_t *material, s32 (*allocator)(s32));

extern void pbSetPrimMatrix(sdfVector4_t *rotation, f32 *position, void *tag);

extern u8 pbFindBlockSlot(s16 x, s16 y, s32 mode);

extern s32 pbDrawMapPolyTransformed(sdfVector4_t *position, u8 slot, u32 kind,
                                   sdfDrawTag_t *tag);

extern void SetRotateMatrixXYZ(const sdfVector3_t *rotation);

extern void StoreMatrix(void *matrix);

extern void LoadMatrix(const void *matrix);

extern sdfGrPacketHeader_t *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors,
                                               s32 count, u32 flags);

extern void sdfDrawTagLink(sdfDrawTag_t *tag, addr_t packet);

extern sdfGrOt_t sdfGrOtTable[85];

s32 pb_proc_block_DB_Process(dds3ProcessID_t *id);

void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id);

extern const s32 pbMapCursorOffsetTable[4][4];

extern s32 pbMapCursorBaseTable[4][4], pbMapCursorUvTable[4][4], pbMapCursorColorTable[4][4];

extern f32 pbMapCursorWork;

extern sdfGrOt_t D_002F67F8;

extern f32 sdfSin(f32 angle);

extern void itfMiscDrawSpritePrim(void *vertices, void *uv, void *color, s32 z,
                                  sdfTexHandle_t *texture, s32 context, sdfDrawTag_t *tag);

s32 pb_draw_map_cursor_Process(dds3ProcessID_t *id);

void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id);

extern const s32 pbMapCursorOffsetTable[4][4];
/* End of declarations from src/smt3/pb/pbGame.c */

s8 pbCheckPreviousRecord(void) {
    s8 ret = pbCheckPopRecord(record);
    return ret;
}
