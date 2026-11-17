#ifndef LMAP_H
#define LMAP_H

#include "common.h"
#include "kernel.h"
#include "sdf_types.h"

struct sprHandle_t;
struct tagLMAP_CHR_WORK;
struct tagLMAP_EFFGRID_CB;
struct tagLMAP_FLDWND_WORK;
struct tagLMAP_LIST_WORK;
struct tagLMAP_LOCK_WORK;
struct tagLMP_MODEL_WK;

/* Local map work; only the state flags byte is placed so far. */
typedef struct lmapWork_t {
    u8 pad0000[0x3150];
    u8 flags; /* 0x3150 */
} lmapWork_t;

/* One 0x30-byte entry of the DH window table. */
typedef struct lmapDHWindow_t {
    u8 unk00[0x20];
    s32 unk20;
    s32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    s8 unk2B;
    s8 unk2C;
    u8 unk2D;
    u8 unk2E[2];
} lmapDHWindow_t;

extern lmapWork_t *DDS3_GBWK2;

dds3ProcessFunc_t ActWnd_Draw(dds3ProcessID_t *proc);
dds3ProcessFunc_t ActWnd_DrawProcess(dds3ProcessID_t *proc);
void ActWnd_Init(dds3ProcessID_t *proc);
void ActWnd_Initialize(dds3ProcessID_t *proc);
dds3ProcessFunc_t ActWnd_Main(dds3ProcessID_t *proc);
void ActWnd_Reset(dds3ProcessID_t *proc);
dds3ProcessID_t *ActWnd_Start(void);
f32 ChrRouteCalcPadParam(s32 pt0, s32 pt1);
s16 Chr_GetNearPortTableNo(struct tagLMAP_CHR_WORK *chrwk);
s32 Chr_SetPlayer(struct tagLMAP_CHR_WORK *wk, s16 pointNo);
s8 Chr_UpdateMove(struct tagLMAP_CHR_WORK *chrwk, struct tagLMAP_LOCK_WORK *lockwk);
void Chr_UpdatePadRandom(struct tagLMAP_CHR_WORK *wk);
s8 Chr_UpdatePadRoute(struct tagLMAP_CHR_WORK *chrwk);
s8 Chr_UpdatePadTrain(struct tagLMAP_CHR_WORK *chrwk);
s8 CtrlActionWindow(void);
s32 EffGrid_CheckWait(dds3ProcessID_t *proc);
dds3ProcessFunc_t EffGrid_Draw(dds3ProcessID_t *proc);
void EffGrid_DrawCB(struct tagLMAP_EFFGRID_CB *p_cb);
void EffGrid_DrawCBAll(dds3ProcessID_t *proc);
dds3ProcessFunc_t EffGrid_DrawProcess(dds3ProcessID_t *proc);
void EffGrid_Init(dds3ProcessID_t *proc);
void EffGrid_InitCB(struct tagLMAP_EFFGRID_CB *p_cb);
void EffGrid_InitCBAll(dds3ProcessID_t *proc);
void EffGrid_Initialize(dds3ProcessID_t *proc);
dds3ProcessFunc_t EffGrid_Process(dds3ProcessID_t *proc);
void EffGrid_SetCB(struct tagLMAP_EFFGRID_CB *p_cb, s32 no, s8 type);
void EffGrid_SetCBAll(dds3ProcessID_t *proc);
void EffGrid_Shutdown(dds3ProcessID_t *proc);
dds3ProcessID_t *EffGrid_Start(void);
void EffGrid_UpdateCB(struct tagLMAP_EFFGRID_CB *p_cb, s32 no, s8 type);
void EffGrid_UpdateCBAll(dds3ProcessID_t *proc);
void EffGrid_UpdateCBHold(struct tagLMAP_EFFGRID_CB *p_cb, s32 no);
void EffGrid_UpdateCBMove(struct tagLMAP_EFFGRID_CB *p_cb);
dds3ProcessID_t *EffWave_Start(void);
void FldWnd_CallStart(struct tagLMAP_FLDWND_WORK *wk);
dds3ProcessFunc_t FldWnd_DrawProcess(dds3ProcessID_t *PID);
void FldWnd_DrawThumbnail(struct tagLMAP_FLDWND_WORK *wk);
void FldWnd_LinkSprite(struct sprHandle_t *a_hSpr);
dds3ProcessFunc_t FldWnd_Process(dds3ProcessID_t *PID);
s32 EffGrid_Main(dds3ProcessID_t *PID);
void lmapLMAPPAC(struct tagLMAP_FLDWND_WORK *wk);
void FldWnd_ProcessStart(void);
void FldWnd_Start(sdfVector4_t *pos1, const sdfVector4_t *pos0);
void ListCallProcEnd(struct tagLMAP_LIST_WORK *wk);
s8 ListCheckProcEnd(struct tagLMAP_LIST_WORK *wk);
s32 ListLinkSprite(struct sprHandle_t *a_hSpr);
s32 ListMakeListStation(struct tagLMAP_LIST_WORK *wk);
void LoadFile(void);
s32 Lock_SetPointChr(struct tagLMAP_LOCK_WORK *lockwk, struct tagLMAP_CHR_WORK *chrwk, s32 lockFlg);
void M_InitCamera(void);
void M_InitScene_lmap(void);
s32 M_SetLockPointNpcCB(s8 charaNo, s32 lockFlg);
void M_UpdateKaya(struct tagLMAP_CHR_WORK *kaya);
s16 MapDat_GetRouteDat(s32 list_no, s32 type);
void MdlUpdate(struct tagLMP_MODEL_WK *wk);
s32 NpcDelete(void);
void Release_lmap(void);
s32 Start_lmap(s8 aFieldNo);
void Util_CalcLength(s16 *a_pDat, s16 a_no, s16 a_type);
s32 Util_GetPadAnalog(sdfVector4_t *p_pad, f32 *p_length, f32 min);
s8 _Chr_UpdatePadRoute(struct tagLMAP_CHR_WORK *chrwk);
void _FldWnd_CallEnd(struct tagLMAP_FLDWND_WORK *wk);
void cmpSetStockVisible(s32 a_bDraw);
s8 dmCallBitCheck(s16 a_no);
s32 getLeaf3(s8 flag);
s16 dmCallLMAPPOINTNO(s32 no);
dds3ProcessFunc_t lmapCallEffGridSetCBAll(dds3ProcessID_t *proc);
void lmapCallTexFreeDelayed2(void);
void mselNmeCamera(void);
extern dds3ProcessID_t *g_mainPID_LmpList;
extern dds3ProcessID_t *g_mainPID_FldWnd;

/* Partial layout of the field window process work. */
struct tagLMAP_FLDWND_WORK {
    struct sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    u8 unk08[0xE9];
    u8 flags; /* bit 1: stock visible. */
    u8 unkF2[2];
};

/* Partial layout of the lmap list process work. */
typedef struct lmapLmpListWork_t {
    struct sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    u8 unk08[0x34];
    u8 flags;
    u8 step;
    u8 pad3E[2];
    s32 unk40;
    s32 count;
    u8 unk48[4];
} lmapLmpListWork_t;

extern const char M_InitScene_lmapData[];
extern const char ListProcessStart_lmapStrLmapListMain[];
extern const char ListProcessStart_lmapStrLmapListDraw[];
extern const char FldWnd_ProcessStartStrLmapFldWndMainProc[];
extern const char FldWnd_ProcessStartStrLmapFldWndDrawProc[];

#endif
