#include "common.h"

/* Declarations from smt3-decomp src/smt3/code_0022C578.c */
#include "common.h"

#include "facility.h"

#include "result2.h"

#include "camp.h"

#include "kernel.h"

#include "fpu.h"

#include "libsdf.h"

#include "newdata.h"

#include "ps2asm.h"

#include "event.h"

#include "draw3d.h"

#include "kernel_draw.h"

#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

typedef struct frFontHandle_t frFontHandle_t;

typedef struct frFontRequest_t frFontRequest_t;

extern sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *packet, s32 prim, s32 count, s64 registers, s32 loops);

extern char fclCombineCalcName[];

extern s8 gProcessStat_0;

extern cmpSkillList_t *cmpSkillCursor;

extern void *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                              void *drawExit, void *deleteExit, void *p);

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

extern s32 datGetMaxHp(datUnitWork_t *work);

extern s32 datGetMaxMp(datUnitWork_t *work);

extern s32 dds3GetProcessStat(void *id);

extern s8 trmMode;

extern s8 gSelMsgPos;

extern s8 gMsgInvFlag;

extern s32 fldWmapEnabled;

extern u8 cmpLoadState;

extern datUnitWork_t *cmbBirthPanel;

extern u16 rstHeartsMasterPhase;

extern u8 rstSkillActFlag;

extern s32 D_0054FEBC;

extern s8 cmpStatusModelFlag;

extern s8 cmpStatusModelLoadState;

extern mdlObj_t *cmpEncycWork;

extern u16 cmpStatusModelId;

extern u32 fclDbgPID;

extern s32 campStatusActive;

typedef struct
{
    /* 0x0000 */ u8 unk_00;
    /* 0x0001 */ s8 unk_01;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_04;
    /* 0x0005 */ u8 unk_0005[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 unk_18;
    /* 0x0019 */ u8 unk_0019[3];
    /* 0x001C */ s32 reserved001C;
    /* 0x0020 */ s32 entryStep;
    /* 0x0024 */ s32 targetPhase;
    /* 0x0028 */ u8 reserved0028[4];
    /* 0x002C */ s32 fadeAlpha;
    /* 0x0030 */ s8 unk_30;
    /* 0x0031 */ s8 unk_31;
    /* 0x0032 */ u8 unk_0032[0x112E];
    /* 0x1160 */ s32 unk_1160;
    /* 0x1164 */ s32 unk_1164;
    /* 0x1168 */ s32 unk_1168;
    /* 0x116C */ s32 unk_116C;
    /* 0x1170 */ s32 unk_1170;
    /* 0x1174 */ s32 unk_1174;
} campInfoWork_t;

extern fclEncyc_instance_tag *g_pinst;

extern campInfoWork_t *campInfoWork;

extern s8 cmpParamGaugeState[2];

extern s8 cmpStatusModelReady;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[0x13];
    /* 0x14 */ void *allocation;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} cmpInfoWindow_t;

typedef struct
{
    u8 flags;
    u8 unk_01[0x1B];
    s32 unk_1C;
} cmbWork_t;

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_0001[0x13];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x44B0];
    /* 0x44C8 */ void *unk_44C8;
    /* 0x44CC */ s32 unk_44CC;
    /* 0x44D0 */ s32 unk_44D0;
} rcvWork_t;

extern rcvWork_t *shutdownfunc;

extern s8 gProcessStat_3;

extern cmpInfoWindow_t *cmpInfoWindow;

extern cmbWork_t *shpWork;

extern saddr_t mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);

extern void sdfMemFreeDirect(void *aHandle);

extern s32 gMsgID;

extern s32 gSelMsgNo;

typedef struct
{
    /* 0x00 */ s8 id;
    /* 0x01 */ s8 type;
    /* 0x02 */ u8 unk_02[2];
    /* 0x04 */ s32 color;
} cmpHeartsIconTableEntry_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ u8 unk_05[0xF];
    /* 0x14 */ void *allocation;
    union {
        /* 0x18 */ u16 slot[16];
        struct {
            u16 reserved0018[10];
            /* 0x2C */ s32 fadeAlpha;
            /* 0x30 */ s8 unk_30;
            /* 0x31 */ s8 unk_31;
            /* 0x32 */ u8 unk_32[6];
        };
    };
    /* 0x38 */ s8 slotCount;
    /* 0x39 */ s8 slotCursor;
} campStatusPanel_t;

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_01[3];
    /* 0x0004 */ s8 unk_04;
    /* 0x0005 */ u8 unk_05[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 state;
    /* 0x0019 */ u8 unk_19[2];
    /* 0x001B */ u8 unk_1B;
    /* 0x001C */ u8 unk_1C[0x18];
    /* 0x0034 */ s32 unk_34;
    /* 0x0038 */ s32 unk_38;
    /* 0x003C */ u8 unk_3C[0x2A8];
} cmpPanelEff_t;

extern cmpHeartsIconTableEntry_t cmpHeartsIconTable[];

extern campStatusPanel_t *campStatusPanel;

extern s32 cmpGaugeTimers[6];

extern void campUpdateEffectStep(void);

extern s32 func_00232198(void);

extern cmpPanelEff_t *cmpPanelEffList;

extern void cmpDrawSpriteFlag(u8 id, s32 flag, s32 arg);

extern void cmpDrawSprite(s32 id, s32 flag, s32 arg);

extern void cmpDrawSprite2(s32 id, s32 flag, s32 arg);

extern void cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h);

extern void cmpDrawBoxG(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *colors, s32 g, s32 h);

extern s32 sdfDelayedResrcCheck(void);

typedef struct
{
    /* 0x00 */ u16 count;
    /* 0x02 */ s8 PartyCnt;
    /* 0x03 */ s8 StockCnt;
    /* 0x04 */ s8 InvalidCnt;
    /* 0x05 */ s8 StockIdx[0x10];
    /* 0x15 */ s8 ListIdx[0x10];
    /* 0x25 */ u8 unk_25[0x31];
} cmpLocalStock_t;

/* The stock work the cursor helpers take: two 0x56-byte rows at the top and a
 * pair of 0x103C-byte records at 0xC0, both selected by the same index. */
typedef struct
{
    /* 0x0000 */ cmpLocalStock_t stock[2];
    /* 0x00AC */ u8 unk_00AC[0x14];
    /* 0x00C0 */ cmpStockInfo_t info[2];
} cmpStockWork_t;

typedef struct
{
    /* 0x0000 */ cmpCursorInfo_t cursor;
    /* 0x0038 */ u8 unk_0038[0x1004];
} cmpListPage_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002;
    /* 0x0003 */ s8 previousPhase;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 stockMessagePhase;
    /* 0x0019 */ u8 unk_0019[0x103F];
    /* 0x1058 */ cmpListPage_t page[3];
    /* 0x410C */ u8 unk_410C[0x22];
    /* 0x412E */ s16 itemCursor;
    /* 0x4130 */ u8 itemScroll;
    /* 0x4131 */ u8 unk_4131[0x2053];
    /* 0x6184 */ cmpLocalStock_t stock[2];
    /* 0x6230 */ u8 unk_6230[0x2078];
    /* 0x82A8 */ u8 drawPhase;
    /* 0x82A9 */ u8 unk_82A9[3];
    /* 0x82AC */ cmpLocalStock_t drawStock;
    /* 0x8302 */ u8 unk_8302;
    /* 0x8303 */ s8 unk_8303;
    /* 0x8304 */ u16 itemId[0xA0];
    /* 0x8444 */ u8 itemMode;
    /* 0x8445 */ u8 statusPhase;
    /* 0x8446 */ u8 reserved8446[2];
    /* 0x8448 */ u8 crossPanel[0x20];
    /* 0x8468 */ s32 unk_8468;
    /* 0x846C */ u8 unk_846C[0x34];
    /* 0x84A0 */ s32 effectStep;
    /* 0x84A4 */ void *dhProcess;
    /* 0x84A8 */ s32 RootTimer;
    /* 0x84AC */ s32 fadeStep;
    /* 0x84B0 */ s32 stockAlpha[16];
    /* 0x84F0 */ s8 stockStep[16];
    /* 0x8500 */ s32 stockDelay[16];
} campWork_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 flags2;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u8 unk_04[4];
} datItemEntry_t;

/* datSkillTable viewed from +0x20, which is where retail folds the base for
 * cmpChkSkillBad: it forms datSkillTable + 0x20 and indexes from there, so
 * &entry->badtype off the newdata.h declaration emits a different address.
 * kind and id are the IDB's badtype (+0x24) and basstatus (+0x26). */
typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ u8 kind;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u16 id;
    /* 0x08 */ u8 unk_08[0x14];
} datSkillBadWindow_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x20];
    /* 0x20 */ datSkillBadWindow_t bad;
} datSkillBadEntry_t;

extern datSkillBadEntry_t datSkillBadEntries[] FAKEMATCH_ASM_NAME("datSkillTable");

extern campWork_t *campWork;

extern datItemEntry_t datItemTable[];

extern f32 aViewEye[4];

extern f32 aViewTarget[4];

extern void cmpBuildStockNameList(cmpLocalStock_t *stock);

extern void cmpClearCursorSlots(cmpCursorInfo_t *cur);

extern void cmpInitStockCursorPos(void);

extern s32 cmpStartSkillTokusyuMsg(u16 skill);

extern void cmpModelInitCore2(void);

extern campStatusPanel_t *cmpStatusPhase;

extern s8 cmpStatusWindowHidden;

extern s8 cmpModelInitFlag;

extern u16 cmpStatusGroup;

extern s32 cmpModelHandle;

extern fclRibbon_t *g_pdebug;

extern void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                           const u32 *colors, s32 surfaceIndex);

extern s32 cmpGetCursorSlotValue(s8 index, cmpCursorSlot_t *slot);

extern f32 dds3ScreenParam[];

extern s32 cmpStatusModelResult;

extern s8 cmpParamGaugeColors[];

extern char *cmpParamNames[];

extern s8 dds3PadButtonState[];

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s8 shpProcessStat;

extern s8 gProcessStat_1;

extern s8 gProcessStat_2;

typedef struct
{
    /* 0x00 */ s8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s8 unk_02;
    /* 0x03 */ u8 unk_03[2];
    /* 0x05 */ s8 unk_05;
    /* 0x06 */ u8 unk_06[6];
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ void *allocation;
    /* 0x18 */ void *owner;
} cmpSeqInfo_t;

extern void cmpUpdateCursorScroll(cmpCursorInfo_t *cursor, s8 mode);

extern void cmpSetInfoWindowHidden(void);

extern u8 *GBWK_0;

extern u8 *GBWK_1;

extern u8 *GBWK_3;

#include "sdk_libc.h"

extern void sdfTexFree(s32 aHandle);

typedef struct
{
    /* 0x00000 */ u8 unk_00000;
    /* 0x00001 */ s8 unk_00001;
    /* 0x00002 */ u8 unk_00002;
    /* 0x00003 */ u8 unk_00003;
    /* 0x00004 */ s8 unk_00004;
    /* 0x00005 */ s8 unk_00005;
    /* 0x00006 */ u8 unk_00006[2];
    /* 0x00008 */ s32 unk_00008;
    /* 0x0000C */ u8 unk_0000C[0x10];
    /* 0x0001C */ cmpCursorInfo_t combineCursor;
    /* 0x00054 */ u8 unk_00054[0x40BC];
    /* 0x04110 */ cmbStockSlot_t stockSlot[3];
    /* 0x0411C */ u8 birthPanel[0x123EE];
    /* 0x1650A */ s8 unk_1650A;
    /* 0x1650B */ u8 unk_1650B[5];
    /* 0x16510 */ s32 script;
    /* 0x16514 */ s32 scriptArg;
    /* 0x16518 */ u8 unk_16518[4];
    /* 0x1651C */ void *explainData;
    /* 0x16520 */ u8 unk_16520[0x2C];
    /* 0x1654C */ u32 *unk_1654C[5];
    /* 0x16560 */ u8 unk_16560[0x70];
    /* 0x165D0 */ s32 smoke[3];
    /* 0x165DC */ u8 unk_165DC[0x1064];
    /* 0x17640 */ cmpCursorInfo_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ s32 unk_1A71C;
    /* 0x1A720 */ fclRibbon_t *ribbon;
} cmbGbWork_t;

extern s8 cmpGetStockInfoIndex(cmpLocalStock_t *stock, s8 mode, s8 flag);

extern s32 cmpStatSetTimer(s32 kind, s8 index);

extern sdfGrOt_t sdfGrOtTable[85];

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x1300];
    /* 0x1300 */ u8 frameTex[0x8E0];
} cmpDrawBank_t;

extern cmpDrawBank_t dds3DrawEnv[];

extern u32 dds3GetFrameID(void);

extern void sdfGrSetFrameMaskRGB(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfDrawTagLinkRefGif(void *aTag, addr_t aPacket);

extern void cmpReCalcStockCursorPos(s8 index);

typedef struct
{
    /* 0x00 */ u8 motionId;
    /* 0x01 */ u8 unk_01[7];
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 evo[3];
    /* 0x18 */ u8 unk_18[4];
    /* 0x1C */ f32 pos[3];
    /* 0x28 */ u8 unk_28[4];
} cmpModelParam_t;

extern cmpModelParam_t cmpModelParamTable[];

extern s32 dds3FadeCheck(void);

extern s8 gProcessStat_4;

extern s8 campExitState;

extern void cmpResetCursorInfo(cmpCursorInfo_t *cursor);

extern void campDrawText(s32 id, s32 *colors);

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

extern void mdlSetRotate(void *aHandle);

extern saddr_t PlayingNo;

extern s32 fclChkMessage(void);

extern void cmpReturnParamDH(void);

extern s32 cmpUnloadStatusModel(void);

typedef s32 (*cmpSeqFunc_t)(void);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern char shpProcName[];

extern char ragCalcProcName[];

extern char rcvCalcProcName[];

extern char rstProcName[];

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u8 unk_08;
    /* 0x09 */ u8 unk_09;
    /* 0x0A */ u8 unk_0A;
    /* 0x0B */ u8 unk_0B;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
} cmpDHWindow_t;

typedef struct
{
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 w;
} cmpVec4_t;

extern cmpVec4_t cmpStatusModelPos;

extern cmpVec4_t cmpStatusModelBasePos;

extern void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

extern u32 fclBlendColor(u32 color, u32 mask, u32 alpha);

extern void cmpUpdatePanelEffSlots(void);

typedef struct
{
    /* 0x0000 */ void *allocation;
    /* 0x0004 */ s32 unk_0004;
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ s32 unk_000C;
    /* 0x0010 */ u8 unk_0010[0xEC];
    /* 0x00FC */ s32 unk_00FC;
    /* 0x0100 */ s32 unk_0100;
    /* 0x0104 */ s32 unk_0104;
    /* 0x0108 */ s32 unk_0108;
    /* 0x010C */ s32 unk_010C;
    /* 0x0110 */ s32 unk_0110;
    /* 0x0114 */ s32 unk_0114;
    /* 0x0118 */ s32 unk_0118;
} stfUiWork_t;

extern stfUiWork_t *oStaff_UI;

extern s8 gMsgFlag;

extern void cmpRemoveStockDevilEntry(void);

extern void cmpInitDrawList(void);

typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
} cmpMsgEntry_t;

extern cmpMsgEntry_t cmpItemMsgTable[];

extern cmpMsgEntry_t cmpSkillMsgTable[];

extern campStatusPanel_t *campDhUpdateWork;

extern s8 cmpGetHeartsIconId(s8 index);

extern void cmpInitCursorRange(void *cur, u8 count, u8 visible, saddr_t layout);

extern void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);

extern void cmpModelFreeResource(void);

extern void cmpResetDrawEnv(void);

extern void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor);

extern void cmpUpdateTerminate(cmpInfoWindow_t *window);

extern s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);

extern void func_0023B500(s32 value);

extern s32 cmpGetPressedButton(s32 mask);

extern s32 cmpLoadStatusModel(void);

extern void cmpPlayCursorSE(u32 button);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

extern void cmpUpdateStatusModelLoad(u16 id);

extern s32 func_001C7E38(frFontHandle_t *frq, s8 mode, u32 arg);

extern s32 D_00345D88[];

#define D_00345D9C ((void **)(D_00345D88 + 5))

extern fclSpriteTbl_t fclSpriteTable[];

extern char shpFmtCount7[];

extern void cmpUpdateListSelection(fclPanelPos_t *pos, u8 *list, s32 unused,
                                   s8 mode, s32 alpha, s32 ot);

extern s32 cmpModelSetup(u16 a, s32 b);

extern s32 cmpChkPadPressUpDown(void);

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

extern char rcvNumFormat[];

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

extern char trmProcessNameTrmFmt[];

extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                               const char *text, frFontHandle_t *previous);

extern s32 func_001C7E38(frFontHandle_t *frq, s8 mode, u32 arg);

extern void fmProcTrigSequence(s32 mode);

void fclDrawSpriteSize(s32 x, s32 y, s32 d, s32 w, s32 h, s32 *color, s32 id, s32 idx, s32 *texList, fclSpriteTbl_t *table, s32 OtNo);

/* Called above its definition, where ee-gcc assumed int. */
s32 cmpMoveCursor(cmpCursorInfo_t *cur, s8 dir);

/* Called above its definition, where ee-gcc assumed int. */
s32 cmpMoveCursor2(cmpCursorInfo_t *cursor, s8 dir);

s32 fclDestroyMessage(void);

void fclLoadFileAsync(const char *name, void *buf);

void fclSetSelMessagePos(s8 SelPos);

s32 fclLoadTexture(char *pFileName);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(void *aHandle);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(void *aHandle);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

/* Called above its definition, where ee-gcc assumed int. */
u16 cmpGetStatusModelId(void);

extern s32 cmpModelSetup(u16 a, s32 b);

extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);

extern char sndPathPrefix[];

extern s8 sndInitDone;

extern s32 D_0054FD20;

extern s32 D_0054FD24;

void rstSetResultWork(void *work);

void rstClearResultWork(void);

extern const char cmpCampTmxPath[];

void cmpTexLoad(void);

extern s32 D_00345D88[5];

void camp_22C610(void);

void func_0022C658(void);

/* The final add shares the preceding slot publication. Each arm captures the
 * current work root after its callback; derive the list only for its consumer. */
extern void cmbClearStockEntry(cmbUnit_t *unit);

extern s32 cmbAddDevilToStock(u16 devilId, cmbUnit_t *list, u8 count);

extern void fclEncycDebugRegist(void);

extern void evtBitOn(s32 bit);

void campDebugRegistAll(void);

void dds3ClearStockTable(void);

void cmpInitLocalStock(void);

void cmpInitDrawList(void);

void cmpInitStockTable(void);

extern u8 itfMesEmbedded_345160[];

extern char *cmpCursorCells[];

extern char *cmpPage0Cells[];

extern char *cmpPage1Cells[];

extern char *cmpPage2Cells[];

/* Four-byte cursor-flag storage in small BSS. */
extern u8 cmpPage2CursorFlags[4];

extern void cmpInitSeqInfo(cmpSeqInfo_t *work, cmpSeqInfo_t *parent);

extern s8 fclInitMessage(void *data);

extern void cmpSetCursorFlag(cmpCursorInfo_t *cursor, saddr_t flags);

extern void cmpDebugDumpCount(void);

extern void camp_H_cmpGlobalWork_t__ctor(void);

void cmpinit(void);

extern void rstSetPanelEffFlag(void);

void camp_update_Shutdown(void);

extern char cmpCalcProcName[];

extern const char campDrawProcName[];

extern const char campUpdateProcName[];

extern void cmpinit();

extern s32 cmpCalcSequence(void);

extern s32 camp_draw_Process(void);

extern s32 camp_update_Process(void);

extern void *cmpStartProcessPanelEff(s32 parent);

void cmpProcessStart(void);

extern char cmpCalcProcName[];

extern const char campDrawProcName[];

extern const char campUpdateProcName[];

void campKillProcesses(void);

s32 campCheckExitState(void);

void cmpSetLoadState(u8 state);

u8 cmpGetLoadState(void);

extern s32 cmpFindFreeStockSlot(s8 index, s8 room);

extern void datAddItem(s32 id, s32 num);

extern s32 datCheckItem(s32 id, s32 num);

s32 cmpDestroyItem(s8 all);

datUnitWork_t *cmpGetSelectSrcStock(void);

s8 cmpGetSelectedStockIndex(void);

datUnitWork_t *cmpGetSelectDstStock(void);

u16 cmpGetSelectItemID(void);

/* Encoded player name and glyph count written by frInitNameData. */
typedef struct {
    u8 reserved00[0x6604];
    u8 nameBytes[0x50];
    u8 nameLength;
} cmpSavedNameView_t;

extern char *text2[];

extern void fclSetMessageVar_20743088(s8 index, saddr_t value);

void cmpSetItSkMessageVar(datUnitWork_t *unit);

extern s32 cmpGetSkillTargetArea(u16 skillId);

s32 cmpGetItSkErrSelFlag(u16 skillId);

s32 cmpFindSkillMsgIndex(s32 which, u16 id);

s32 cmpFindItemMsgIndex(s32 which, u16 id);

void cmpCreateLocalItem(u8 mode);

extern s32 fclStartMessage(s32 message);

extern s32 cmpClearButtonGuide(s32 skill, u32 select);

extern s32 cmpStartSkillMsg(u16 skill, s32 errorFlags);

extern s32 cmpGetKugatatiMsg(void);

extern s32 fldEsutoMaChk(void);

extern s32 fldRiberaMaChk(void);

extern s32 fldRifutoMaChk(void);

extern s32 fldLightMaChk(void);

s32 cmpStartSkillTokusyuMsg(u16 skill);

s32 cmpStartItemTokusyuMsg(u16 item);

extern s32 cmpClearButtonGuide(s32 skill, u32 select);

s32 cmpStartSkillMsg(u16 skill, s32 errorFlags);

extern s32 cmpFindKugatatiMsgEntry(datUnitWork_t *unit);

extern s32 cmbChkDevilEvo(datUnitWork_t *unit);

s32 cmpGetKugatatiMsg(void);

extern s32 datGetItemSkillNo(s32 itemid);

extern s32 cmpStartItemMsg(u16 item, s32 errorFlags);

extern s32 cmpUseItemTokusyu(u16 item, saddr_t target, saddr_t user);

extern void cmpRecover(u16 skill, datUnitWork_t *stock, datUnitWork_t *target);

extern void cmpSetStockRecoverEff(s32 effect, s32 skill, cmpLocalStock_t *stock);

extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);

s32 cmpUseItem(void);

u16 cmpGetSelectSkillID(void);

extern s32 cmpAddCost(u16 skill, datUnitWork_t *unit);

extern s32 cmpGetSkillEntry(u16 skill, datUnitWork_t *unit);

extern s32 cmpStartSkillMsg(u16 skill, s32 errorFlags);

extern s32 cmpUseSkillTokusyu(u16 skill, datUnitWork_t *user, saddr_t target);

s32 cmpUseSkill(void);

s32 cmpChkDoNotUseSkill(u16 skill);

/* Called above its definition, where ee-gcc assumed int. */
s8 cmpStartStockMsg(s8 flag);

s32 cmpItemIsStockFull(u16 id);

void cmpApplyStockRecover(void);

extern void cmpClearStockFlag(datUnitWork_t *unit);

extern void datClearCommMemory(datUnitWork_t *work);

extern void datAllClearCommMemory(void);

void cmpApplyStockRemove(void);

extern s32 func_0022E168(u16 devilId);

void cmpRemoveStockDevilEntry(void);

s32 func_0022E168(u16 devilId);

void cmp_22E258(void *work);

s32 cmpInitSkillCursor(datUnitWork_t *unit);

void cmpInitStockCursorPos(void);

/* Target-specific allocation aid for the real stock-count accumulator; this
 * does not imply a hardware register requirement or original source spelling.
 * The binding ends before each call. See docs/matching/code_0022C578.md. */
extern void cmpChangeCursorListNums(s32 count, u8 *cursor);

void cmpReCalcStockCursorPos(s8 index);

void cmpInitRootCursorPos(void);

void cmpInitSelectCursor(s32 index);

s8 camp_H_cmpLocalStock_t__ctor(void);

s8 cmpStartStockMsg(s8 flag);

void cmpCalcRoot(void);

void cmpSetSkillTSL(u16 skill);

void cmpSetItemTSL(u16 itemId);

extern void cmpSetAfterCrossFunc(void *panel, s32 x, s32 y, void *func,
                                 void *data, s32 value, s32 alpha);

extern void cmpSetCrossGeneralVal(u8 *work, s8 value);

extern void cmpDrawSkillList(s32 *position, cmpCursorInfo_t *cursor,
                             datUnitWork_t *unit, s8 mode, s32 alpha);

extern const char campDrawProcName[16];

extern const char campUpdateProcName[16];

void cmpCalcSkill(void);

extern char *D_00330914[];

extern char cmpFmtNameCount[];

extern char *pStr[];

extern s32 cmpItemHasFlag(u16 item, s8 which);

s32 cmpCalcDestroyItem(void);

s32 cmpSetPartyDSL(void);

s32 cmpCalcStockControl(void);

extern void cmpSetAfterCrossFunc(void *panel, s32 x, s32 y, void *func,
                                 void *data, s32 value, s32 alpha);

extern void cmpSetCrossGeneralVal(u8 *work, s8 value);

extern s32 fclStartMessage(s32 message);

extern s32 fclStartSelMessage(s32 message);

extern void fclSetMessageInvalid(s8 mode);

s32 cmpCalcParty(void);

extern void dds3ConfigProcessStart(s32 Mode);

void cmpCalcConfig(void);

void cmpCalcLoad(void);

extern s32 fclChkMessage(void);

extern void cmpCreateLocalItem(u8 mode);

extern void cmpChangeCursorListNums(s32 count, u8 *cursor);

void cmpUpdateList(void);

/* The cursor initializer only mutates slots; its return register is unused. */
extern void cmpSetIniStockRecoverEff(u8 *work);

s32 cmpCalcSequence(void);

/* The gradient fan passes three fixed-point X and Y coordinates here.
 * Each vertex receives a separate distortion scale; X uses twice the Y scale. */
void nmeMakeGradColors(s32 *xs, s32 *ys, s32 delta0, s32 delta1, s32 delta2);

extern const s32 nmeQuadPanelCoords[];

extern void fclDrawPanelSprite2(s32 id, s32 ot);

void nmeDrawQuadPanel(s32 color0, s32 color1, s32 alpha, s32 ot);

extern const char cmpTextWhatWillYouDiscard[24] __attribute__((aligned(16)));

extern const char cmpTextWhoWillYouPartWith[24];

extern const char cmpTextWhoWillYouReturn[24];

extern const char cmpTextWhoWillYouSummon[24];

extern const char cmpTextFromWhomWillYouExtract[32];

extern const char cmpTextWhichWillYouUnequip[24];

extern const char cmpTextWhichWillYouIngest[24];

extern const char cmpTextSPITEMS[16];

extern const char cmpTextUseWhatItem[16];

extern const char cmpTextWhoWillUseIt[24];

extern const char cmpTextWhoWillUseASkill[24];

extern const char cmpTextWhatWillYouUse[24];

extern const char cmpTextMAINMENU[16];

extern const char cmpTextNoDemonsToExtractFrom[32];

extern const char cmpTextSummoned[16];

extern const s32 nmeQuadPanelCoords[12];

extern const u32 nmeGradBaseColors[10];

extern const f32 nmeGradWeights[6];

void nmeDrawBlendQuad(s32 alpha, s32 ot);

void nmeDrawBox(s32 color, s32 unused, s32 flag);

/* Native callers pass full argument words. Narrow only at the rendering
 * consumer, retaining this provider's byte and halfword conversions. */
void nmeDrawSizedSprite(s32 id, s32 idx, s32 *texs, fclSpriteTbl_t *tbl, s32 color, s32 size,
                        s32 ot);

s32 nmeLerp(s32 amplitude, s32 frame, s32 period);

s32 nmeLerp2(s32 scale, s32 value, s32 range);

extern void nmeDrawGradQuad(s32 x, s32 alpha, s32 ot);

extern void nmeDrawQuadPanel(s32 color0, s32 color1, s32 alpha, s32 ot);

void nmeDrawFrame(s32 type, s32 frame);

void nmeDrawBlendText(s32 sprite, s32 page, sdfTexHandle_t **textures,
                      fclSpriteTbl_t *table, u32 color, s32 frame, s32 ot);

extern frFontRequest_t *frwGetHelpFRQ(s32 x, s32 y, s32 priority, s8 bank, u16 index, s8 mode);

extern void frSetFontColor(frFontHandle_t *request, u32 color);

void cmpDrawHelp(fclPanelPos_t *pos, u16 index, s8 bank, s8 skipBackground, s32 alpha, u32 ot);

extern const char *cmpMenuTextTable[];

extern s32 cmpStartProcessInfo(s32 text, s32 flag);

void cmp_2304C0(s8 index);

void cmpDisableInfoWindow(void);

extern const char *cmpMenuStrings[] FAKEMATCH_ASM_NAME("cmpMenuTextTable");

extern void cmpSetSkillListRows(s32 rows);

extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *colors, s32 page,
                             s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 ot);

extern u32 frGetStrWidth2(frFontHandle_t *font);

extern void frSetDrawPosition(frFontHandle_t *font, u32 x, u32 y);

void campDrawText(s32 id, s32 *colors);

extern s32 cmpGetSkillTargetArea(u16 skillId);

void campSkillTargetIsSelf(u16 skillId, s8 value);

void campDrawStockRow(s8 mode);

void campDrawSetPhase(void);

void campDrawStockCursor(void);

extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *colors, s32 page,
                             s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 ot);

extern frFontRequest_t *frwGetNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                         const char *text, frFontRequest_t *previous);

extern char cmpFmtMaka[];

void campDrawNumber(void);

extern void fclDrawSpriteFull(s32 x, s32 y, s32 d, s32 w, s32 h, u8 mode, s32 *color, s32 id,
                              u16 spriteId, s32 *texList, fclSpriteTbl_t *table, s32 OtNo);

void campDrawFade(void);

/* The first sprite ID uses a localized live-value constraint; see the
 * matching note for the paired ordinary and nonvolatile controls. */
struct campCrossPanel_t;

extern void campRunPanelCallback(struct campCrossPanel_t *panel);

extern void campDrawSkillPanel(void);

extern void campDrawInfoPanel(void);

extern void campDrawSeq3(s8 phase);

extern void campDrawStockRow(s8 mode);

extern void campDrawNumber(void);

extern void fclDrawPanelSprite(s32 id, s32 ot);

extern void cmpFindPanelEffSlot(s32 ot);

s32 camp_draw_Process(void);

extern u32 dds3DebugSign;

extern void func_00242A70(s32 key);

s32 campDebugCheckPad(s8 key);

extern void *cmpStartProcessDH(cmpSeqInfo_t *parent);

void cmpUpdateRoot(void);

void cmpUpdateItem(void);

extern void cmpSetCrossGeneralVal(u8 *work, s8 value);

void cmpUpdateParty(void);

void campUpdateStockCursor(void);

extern s32 cmpChkDoNotUseSkill(u16 skill);

/* Exact-type view of the existing provider, deliberately hiding its earlier
 * definition. In paired complete-owner builds, the direct call moves the
 * common cursor-work load into the button-one branch delay slot; this view
 * preserves retail's no-op there and load in the default exit's delay slot.
 * The provider and ABI are unchanged. This is a target-specific visibility
 * aid, not evidence of original source spelling. */
extern s32 cmpChkDoNotUseSkillView(u16 skill) FAKEMATCH_ASM_NAME("cmpChkDoNotUseSkill");

void cmpUpdateSkillSelect(void);

extern u8 cmpClearStatusFlags(void);

extern void *cmpStartProcessStatus(s8 arg, cmpSeqInfo_t *parent);

extern void cmpSetStatusWindowHidden(void);

extern void cmpMoveStockCursor(s8 index, s8 direction, u8 *stock);

void cmpUpdateProcessStatus(void);

void cmpRemoveStockDevil(void);

extern s32 cmpSetStockRemoveEff(u8 *work);

extern s32 fclChkSelMessage(void);

extern s8 fclGetSelMessagePos(void);

extern s8 fclEndMessage(void);

void campWaitSelMessage(void);

void campCloseInfoWindow(void);

extern s32 fclChkSelMessage(void);

extern s8 fclGetSelMessagePos(void);

extern s8 fclEndMessage(void);

void cmpUpdateDestroyItemConfirm(void);

extern s32 checkConfigState(void);

void campShowStock(void);

void cmpUpdateLoad(void);

void campUpdateEffectStep(void);

s32 func_00232198(void);

s32 campUpdateEffects(void);

extern void fclUpdateMessage(void);

extern s32 cmpUpdateSeqInfo(cmbGbWork_t *work, s8 value);

s32 camp_update_Process(void);

s32 cmpStockEffIsActive(u8 *work);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x14];
    /* 0x14 */ void *allocation;
    /* 0x18 */ s8 unk_18;
    /* 0x19 */ u8 unk_19[3];
    /* 0x1C */ cmpLocalStock_t *stock;
} cmpStockSeqInfo_t;

void *cmpAllocStockProcess(cmpLocalStock_t *pNameList, s32 pParent);

void camp_update_stock_Shutdown(void);

extern s32 camp_calc_stock_Process(s32 id);

extern s32 camp_draw_stock_Process(s32 id);

extern s32 camp_update_stock_Process(dds3ProcessID_t *id);

void *cmpStartProcessStock(cmpLocalStock_t *stock, s32 value);

void campHideStatusWindow(void);

void cmpSetStockVisible(s8 visible);

extern cmpInfoWindow_t *campCalcStockWindow;

s32 camp_calc_stock_Process(s32 id);

extern void campDrawStockSlots(s8 *indices, s32 active, s8 *flags, cmpLocalStock_t *stock);

void campDrawStockPanel(cmpLocalStock_t *stock, s32 mode);

extern cmpInfoWindow_t *campDrawStockWindow;

s32 camp_draw_stock_Process(s32 id);

extern cmpStockSeqInfo_t *cmpStockUpdateState;

extern void cmpUpdateCursorSlots(cmpCursorInfo_t *cursor);

s32 camp_update_stock_Process(dds3ProcessID_t *id);

void cmpSetDHEntry(s8 id);

extern s32 cmpHeartsIconIsVisible(s8 index);

s32 datCountRegisteredHearts(void);

s32 cmpHeartsIconIsVisible(s8 id);

void cmpInitHeartsIconTable(campInfoWork_t *work);

s8 cmpGetHeartsIconId(s8 index);

s8 cmpGetHeartsIconType(s8 index);

s32 cmpGetHeartsIconColor(s8 index);

s32 campDhCheckReady(void);

s32 campDhIconIsReady(void);

extern sdfTexHandle_t *sdfTexSet(TMX_HEADER_t *aTex);

void campDhCheckTextures(void);

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x116C];
    /* 0x116C */ s32 texHandle[3];
} cmpDhWork_t;

s32 cmpLoadDHTextures(cmpDhWork_t *work);

extern const char campDhFreeTexMsg[];

void campDhFreeTexture(campInfoWork_t *work);

extern void campDhFreeTexture(campInfoWork_t *work);

extern void sdfMemFreePtrDirect(void *aPtr);

extern const char campDhFreeDataMsg[];

void campDhFreeResources(campInfoWork_t *work);

s32 func_00232C08(void);

extern char *pstr[];

extern char *pstr[];

s32 cmpSetHeartsSequenceRemove(void);

void cmpInitDHWindow(u8 *base);

extern void cmpInitSeqInfo(cmpSeqInfo_t *work, cmpSeqInfo_t *parent);

extern void cmpInitHeartsIcon(campInfoWork_t *work);

extern s8 fclGetCurrentSelMessagePos(void);

campInfoWork_t *cmpinitDH(cmpSeqInfo_t *parent);

extern void campDhFreeResources(campInfoWork_t *work);

void camp_update_dh_Shutdown(void);

extern campInfoWork_t *cmpinitDH(cmpSeqInfo_t *parent);

extern s32 camp_calc_dh_Process(dds3ProcessID_t *id);

extern s32 camp_draw_dh_Process();

extern s32 camp_update_dh_Process();

extern const char campDhFreeTexMsg[24];

extern const char campDhFreeDataMsg[24];

void *cmpStartProcessDH(cmpSeqInfo_t *parent);

s32 campInfoWorkExists(void);

void campHideInfoWindow(void);

extern s32 cmpAddQuePanelEff(s32 x, s32 y, s32 z, u16 width, u16 height, u8 step,
                             u8 delay, s32 color0, s32 color1, u8 kind);

extern void rstCreateBeforeSkillList(s8 heartsId, datUnitWork_t *stock, s8 levelOffset,
                                     rstSkillInfo_t *info);

void cmpSetStatusStartEff(void);

extern campInfoWork_t *volatile campDhCalcStateLive FAKEMATCH_ASM_NAME("campDhCalcState");

extern campWork_t *volatile campWorkLive FAKEMATCH_ASM_NAME("campWork");

extern void cmpSetStatusStartEff(void);

extern void cmpModelInit(void);

void campDhDrawBox(s32 alpha);

extern void cmpDrawBoxGDefault(s32 x, s32 y, s32 z, s32 width, s32 height,
                              s32 *colors, s32 ot);

void campDhDrawGaugeBars(s32 unusedAlpha);

void fclCalcArrowCursorOffset(s8 index, s32 *x, s32 *y);

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[0x1B];
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 entryStep;
    /* 0x24 */ s32 targetPhase;
    /* 0x28 */ u8 reserved28[4];
    /* 0x2C */ s32 fadeAlpha;
    /* 0x30 */ s8 unk_30;
    /* 0x31 */ s8 iconIndex;
    /* 0x32 */ u8 reserved32[0x1146];
    /* 0x1178 */ s32 cursorAlpha[64];
} campDhWork_t;

extern campDhWork_t *campDhDrawWork;

void fclGetArrowCursorTarget(s32 x, s32 y, s32 z, u32 color, s32 extra);

extern s32 datDevilIsRegistered(u8 devilId);

void fclDrawArrowCursorIcon(s32 x, s32 y, s32 z, s8 index, s32 extra);

extern void fclDrawSpriteSizeAlphaFilter(s32 x, s32 y, s32 d, s32 w, s32 h, u8 mode,
    s32 *color, s32 id, f32 angle, u16 idx, s32 *texs, fclSpriteTbl_t *tbl, s32 ot);

void fclDrawArrowCursorBox(s32 x, s32 y, s32 z, s32 w, s32 h, u32 color,
                          s32 id, s32 mode, s32 alpha, f32 angle);

typedef struct
{
    /* 0x00 */ s8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ u8 unk_02[2];
    /* 0x04 */ u32 color;
    /* 0x08 */ s8 mode;
    /* 0x09 */ s8 direction;
    /* 0x0A */ u8 unk_0A[2];
    /* 0x0C */ s32 directionTimer;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ f32 unk_1C;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
} campDhEntry_t;

void fclGetArrowCursorEntry(s32 x, s32 y, s32 z, s8 index, s32 extra);

extern campDhWork_t *volatile campDhBackgroundWork FAKEMATCH_ASM_NAME("campDhDrawWork");

void fclDrawArrowCursorLabel(s32 x, s32 y, s32 z, s32 index, s32 *color, s32 alpha);

void fclDrawArrowCursor(s32 x, s32 y, s32 z, s32 extra);

void campDhDrawPanel(s32 x, s32 y, s32 unused, s32 alpha);

extern const u32 campDhQuadGaugeCoords[];

extern const u32 campDhQuadGaugeColors[];

void campDhDrawQuadGauge(s32 ratio);

void campDhDrawHelpTextAlt(s32 x, s32 y, s32 z, s32 alpha);

/* The captured font X coordinate uses its native saved register. At the
 * draw-position consumer, the short-lived font argument binding preserves
 * native argument ordering; $4 is not live across any preceding call.
 * These SMT3-only allocation/scheduling aids emit no instructions and do
 * not establish historical source spelling. The ordinary control swaps
 * four coordinate words; coordinate-only control rotates three arguments. */
extern u8 frEnableMode(s32 flag);

extern u8 frDisableMode(u8 flag);

extern frFontHandle_t *frLinkFontString(u8 *text, s8 style, s8 color, s8 type,
    frFontHandle_t *previous);

extern void frSetFontSpace(frFontHandle_t *font, s32 space);

extern void frSetDrawPosition(frFontHandle_t *font, u32 x, u32 y);

extern void frSetPriority(frFontHandle_t *font, u32 priority);

extern void frSetMessageStepY(u32 step);

extern void frSetMessageDefStepY(void);

/* EE prefix-initializer view: the canonical provider takes fclPanel_t*,
 * but its complete native body only stores X/Y/Z/W/H/Col at offsets
 * 0x00/04/08/0C/0E/10. It performs no reads, calls, pointer escape or
 * full-object operation. Explicit casts at this owner's calls retain the
 * existing 0x20-byte prefix storage; they do not assert ISO C compatibility
 * between fclPanelPos_t and the full panel type. */
extern void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work,
    fclPanel_t *pPanelPos);

void campDhDrawHelpText(s32 x, s32 y, s32 z, s32 alpha, s8 index);

extern const u32 campDhGaugePalette[];

extern char campDhFmtAmount[];

extern s8 cmpGetParamGaugeColor(s8 index);

extern s32 cmbGetHeartsParam(s8 devilId, s8 index);

extern frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 priority,
                                               u32 color, const char *text,
                                               frFontHandle_t *previous);

typedef struct
{
    /* 0x00 */ s32 visible;
    /* 0x04 */ s32 alpha;
    /* 0x08 */ s8 step;
    /* 0x09 */ s8 id;
    /* 0x0A */ u8 pad[2];
    /* 0x0C */ s32 fade;
    /* 0x10 */ s32 fadeStep;
} campDhIconGauge_t;

extern void campDhDrawHelpText(s32 x, s32 y, s32 z, s32 alpha, s8 index);

extern void campDhDrawHeartsGauge(campDhIconGauge_t *gauge, s8 arg);

void campDhDrawHeartsGauge(campDhIconGauge_t *gauge, s8 arg);

void campDhDrawHeartsIcon(s32 a, s32 b, s32 c, s8 d, s32 e);

extern void campDhDrawBoxSprite(s32 alpha);

extern void campDhDrawPanel(s32 x, s32 y, s32 unused, s32 alpha);

void campDhDrawIcon(void);

extern s32 cmbGetHeartsParam(s8 devilId, s8 index);

void campDhDrawHearts(void);

extern void campDhDrawIcon(void);

extern const u32 campDhQuadGaugeCoords[6];

extern const u32 campDhQuadGaugeColors[4];

extern const u32 campDhGaugePalette[6];

void campDhApplyEntry(s32 button);

s32 campDhFindEntry(void);

/* The Hearts panel keeps the pre-selection equipment byte and a complete
 * player unit snapshot, restored after the confirmation message. */
typedef struct
{
    u8 unk_00[0x32];
    s8 equipHearts;
    u8 unk_33;
    datUnitWork_t unit;
} campDhSavedUnit_t;

extern void datEquipHearts(s32 HeartsID);

void cmpReturnParamDH(void);

void campDhEquipHearts(void);

/* Exact-type, same-symbol view withholds the earlier provider definition.
 * The direct call leaves three default-tail words different; this conservative
 * visibility boundary restores the native delay slots without changing the
 * provider or call target. This is a reconstruction aid, not historical source. */
extern void campDhHideInfoWindowView(void) FAKEMATCH_ASM_NAME("campHideInfoWindow");

void campDhUpdateCursor(void);

s32 cmpUnloadStatusModel(void);

extern s8 cmpGetStatusModelFlag(void);

extern s32 fclStartSelMessage(s32 message);

extern void fclSetMessageInvalid(s8 mode);

extern s32 fclStartMessage(s32 message);

void cmpUpdateHeartsAttachStatus(void);

/* campDhUpdateWork, as this unit's animation and icon code reads it. The two
 * were separate structs whose 0x28-byte prefixes agreed, so they are one here.
 * A third view, campStatusPanel_t *campDhUpdateWork, is declared above: the
 * alias is what lets both spellings of the one object coexist, and unifying
 * them is type recovery on the real allocation, not a rename. */
typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x1C];
    /* 0x001C */ s32 unk_1C;
    /* 0x0020 */ s32 unk_20;
    /* 0x0024 */ s32 unk_24;
    /* 0x0028 */ s32 unk_28;
    /* 0x002C */ u8 unk_002C[0x114C];
    /* 0x1178 */ s32 iconTimer[64];
} campDhTimerWork_t;

extern campDhTimerWork_t *campDhTimerWork FAKEMATCH_ASM_NAME("campDhUpdateWork");

void cmpStepHeartsAttachCursor(s32 step);

extern s32 fclChkSelMessage(void);

extern s8 fclGetSelMessagePos(void);

extern s8 fclEndMessage(void);

extern void fclSetMessageInvalid(s8 mode);

extern s32 fclStartMessage(s32 message);

extern void cmpStepHeartsAttachCursor(s32 step);

void cmpUpdateHeartsAttachConfirm(void);

extern s32 fclChkSelMessage(void);

extern s8 fclGetSelMessagePos(void);

extern s8 fclEndMessage(void);

extern void fclSetMessageInvalid(s8 mode);

extern s32 fclStartMessage(s32 message);

extern void datRemoveHearts(void);

void cmpUpdateHeartsRemoveConfirm(void);

s32 campDhWaitMessageEnd(void);

void campDhAdvanceAnim(void);

void campDhFadeAlpha(void);

void campDhUpdateIcons(void);

extern u32 dds3GetRandA(void *state);

void campDhRollRatio(campDhEntry_t *entry);

void campDhGetEntryRatio(campDhEntry_t *entry);

typedef struct
{
    u8 unk_0000[0x1278];
    campDhEntry_t entries[25];
} campDhEntryWork_t;

extern campDhEntryWork_t *campDhEntryWork FAKEMATCH_ASM_NAME("campDhUpdateWork");

void campDhScanEntries(void);

void campDhFindVisibleIcon(void);

extern void cmpSetSkillListRows(s32 rows);

void campDrawSkillPanel(void);

void cmbSetBirthPanel(datUnitWork_t *panel);

void rstSetHeartsMasterPhase(void);

void rstClearHeartsMasterPhase(void);

u8 rst_235DA0(void);

void rstSetSkillActFlag(u8 flag);

void rstClearFlag(s8 index);

s8 func_00235DC8(s8 index);

extern s32 cmpStatusAnimCount[];

extern s32 cmpStatTimer;

s32 cmpStatSetTimer(s32 kind, s8 index);

void func_00235E40(s8 index);

s32 func_00235E60(void);

void cmpGetGaugeBlinkAlpha(s32 frame, s8 state, u32 *colors);

s32 cmpGetPanelAlphaOrDefault(u8 *alpha, s32 color);

extern s32 cmpStatTimer;

extern void cmpGetGaugeBlinkAlpha(s32 frame, s8 state, u32 *colors);

extern void fclDrawSpriteFilter(s32 x, s32 y, s32 z, u8 filter, s32 *colors,
                                s32 page, s32 spriteId, s32 *texList,
                                fclSpriteTbl_t *table, s32 ot);

extern frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 priority,
                                               u32 color, const char *text,
                                               frFontHandle_t *previous);

extern char cmpFmtDec2[];

void rstDrawNumber(s32 x, s32 y, s8 number);

extern char cmpFmtValMax[];

extern void fclDrawSpriteRect(s32 x, s32 y, s32 z, s32 width, s32 height,
                              s32 *colors, s32 page, s32 spriteId, s32 *texList,
                              fclSpriteTbl_t *table, s32 ot);

extern void cmpDrawStatusIconRow(s32 x, s32 y, s32 z, s32 color,
                                 datUnitWork_t *unit, s32 flags, s32 ot);

void cmpDrawStatusIcons(s32 x, s32 y, s32 *colors, s32 mode, datUnitWork_t *unit);

extern char *datExplainTable[];

extern u32 strlen(const char *text);

#include "font.h"

extern s32 datGetPlayerLNDLevel(datUnitWork_t *unit);

extern u16 datGetLevelForDraw(datUnitWork_t *unit);

extern s32 datAdvanceDevilLevel(datUnitWork_t *unit);

extern frFontHandle_t *frGetNameFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

extern datDevilTableEntry_t datDevilTable[];

extern char *datExplainTable[];

extern char cmpFmtExpNext[];

extern s16 cmpHeartsMasterPhase FAKEMATCH_ASM_NAME("rstHeartsMasterPhase");

extern datUnitWork_t *volatile cmpStatusBirthLive FAKEMATCH_ASM_NAME("cmbBirthPanel");

extern void cmpDrawStatusBadge(s32 x, s32 y, s32 *colors, u16 statusFlags, s32 unused);

extern s32 cmbGetHeartsParamEx(s8 devilId, s8 index, s32 registered);

extern s32 cmbGetParam(datUnitWork_t *unit, s32 index);

extern volatile s32 cmpGaugeAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");

extern s32 cmbGetParam(datUnitWork_t *unit, s32 index);

extern void cmpDrawParamGauge(s32 x, s32 y, s32 *colors, s32 rowHeight,
                              s8 index, s8 bonus, s8 mode, datUnitWork_t *unit);

extern void fclDrawSpriteAlpha(s32 x, s32 y, s32 z, u8 mode, s32 *colors,
                                s32 page, s32 spriteId, s32 *texList,
                                fclSpriteTbl_t *table, s32 ot);

extern datDevilTableEntry_t datDevilTable[];

extern char *datExplainTable[];

extern s16 datGetDevilParam(u8 devilId);

void cmpDrawStatusValue(s32 x, s32 y, s32 *colors, s32 mode, datUnitWork_t *unit);

void cmpDrawStatusBars(s32 x, s32 y, u8 *alpha);

extern volatile s32 cmpStatusAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");

extern s32 cmpFindSkillEntry(u16 skillId);

extern char *text1[];

extern volatile s32 cmpSkillAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");

void cmpDrawStatusCom(datUnitWork_t *a, s8 b, s8 c, s8 d);

void cmpDrawStatusComEx(datUnitWork_t *a, u8 alpha, s8 b, s8 c, s8 *d, s8 e, s32 color, s8 f, s8 g, s8 h);

extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);

extern void cmbCopyDefaultDevilToStock(u16 devilId, datUnitWork_t *dst);

void cmpCalcTargetDevil(datUnitWork_t *src, datUnitWork_t *dst);

extern s32 cmpStatusAnimCount[];

void cmpAdvanceStatusAnim(void);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x10];
} cmpModelLink_t;

extern cmpModelLink_t sdfModelOtTable[];

extern void cmpDrawStatusRow(s32, s32, s32 *, s32, datUnitWork_t *);

extern void cmpDrawParamPanel(s32, s32, s32 *, s8 *, datUnitWork_t *, s8, s8, s8);

extern void cmpDrawStatusValue(s32, s32, s32 *, s32, datUnitWork_t *);

extern u16 itfMesMngGetStatManager(void);

extern void rstCreateBeforeSkillList(s8, datUnitWork_t *, s8, rstSkillInfo_t *);

extern void cmpDrawSkill(s32, s32, s32 *, datUnitWork_t *, rstSkillInfo_t *, s8, s8,
                         s32, s8, s32);

extern s32 cmpDrawStatusModel(void *);

void cmpDrawStatusComEx2(datUnitWork_t *unit, s32 x, s32 y, u8 alpha,
                         s8 kind, s8 detail, s8 *delta, s8 mode, u32 color,
                         s8 show, s8 option, s8 extra, s8 full);

extern char *pStr[];

extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                               const char *text, frFontHandle_t *previous);

void cmpDrawItemListHeader(s32 x, s32 y, s32 c, u16 index);

/* Called before its definition without a parameter prototype. */
extern void cmpDrawScrollThumb();

void cmpDrawItemListScrollBar(s32 alpha, cmpCursorInfo_t *cursor);

void campDrawInfoPanel(void);

/* This exact-type view withholds the new same-unit body from the dispatcher.
 * The paired direct call changes its branch/epilogue layout; the linker target
 * and truthful provider remain unchanged. This is not an original-source claim. */
extern void campDrawTextView(s32 id, s32 *colors) FAKEMATCH_ASM_NAME("campDrawText");

void campDrawSeq3(s8 phase);

extern char *text1[];

void cmpDrawSkillListHeader(s32 x, s32 y, s32 c, u16 index);

u8 cmpGetSkillIcon(u16 skill);

u16 cmpGetSkillCost(u16 skill, u16 *work);

extern const u8 D_0053FEB8[];

extern char cmpFmtSkillCost[];

void cmpDrawSkillListScrollBar(s32 a, cmpCursorInfo_t *cursor);

extern s32 cmpCursorBlinkColor(s32 timer);

extern s32 cmpCursorFadeChannel(s32 color, s32 timer, s32 rate);

extern void cmpDrawListFrame(s32 x, s32 y, s32 unusedZ, s32 rowHeight,
                             s8 rowCount, s32 alpha, s32 ot);

extern void cmpSetCursorSlot(s8 index, s8 state, u8 param, cmpCursorSlot_t *slot);

extern s32 cmpAllocEffectSlot(s32 color, s8 selected, s8 disabled,
                              cmpCursorSlot_t *slot);

void cmpExpandColor(u64 *dst, u32 color);

extern sdfDrawTag_t cmpViewTag[2];

extern sdf3DInitPacket_t cmpViewInitPacket[2];

extern sdfDrawTag_t cmpLightTag[2];

extern sdf3DLightParallelPacket_t cmpLightPacket[2];

extern sdf3DViewportPacket_t cmpViewportPacket[2];

extern sdfMatrix4_t sdfViewMatrix;

extern dds3LightState_t dds3LightParam;

extern sdfVector4_t dds3AmbientColor;

extern f32 aViewDown[4];

extern void LookAtMatrix(f32 *, f32 *, f32 *);

extern void sdf3DMakeScreenParam(sdf3DScreenParam_t *);

extern void sdfDrawTagInit(sdfDrawTag_t *);

extern void sdf3DLinkInitPacket(sdfDrawTag_t *, sdf3DInitPacket_t *);

extern void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *, sdf3DScreenParam_t *, sdfMatrix4_t *);

extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t);

void cmpDrawSetup3DView(void);

void cmpDrawSpriteFlag(u8 id, s32 flag, s32 ot);

void func_00239C08(u8 id, s32 arg);

void cmpDrawSprite(s32 id, s32 flag, s32 ot);

void fclDrawPanelSprite(s32 id, s32 arg);

void cmpDrawSprite2(s32 id, s32 flag, s32 ot);

void fclDrawPanelSprite2(s32 id, s32 arg);

void cmpDrawSpriteIndexed(s32 id, s32 ot);

void cmpDrawSpriteIndexed2(s32 id, s32 ot);

void func_00239F88(s32 *work, s32 value);

void uiDrawUniformRgbRange(const u32 *xs, const u32 *ys, u32 z, u32 color, u32 ot);

void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

void cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h);

void cmpDrawBoxF(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

void cmpDrawBoxGDefault(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *f, s32 g);

void cmp_23A360(s32 x, s32 y, s32 z, s32 w, s32 h, s32 g, s32 col);

void cmpDrawPanelEffCell(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);

void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                    const u32 *colors, s32 surfaceIndex);

void cmpDraw3DPoly(const f32 *first, const f32 *second, const f32 *third,
                   const u32 *colors, s32 unused, sdfMat_t *asset, s32 surfaceIndex);

void cmpDisableRGB(s32 ot);

void cmpAllocRefGifTag(s32 ot);

void cmp_23A8B0(s32 arg);

/* This target-specific constraint completes the real sprite ID before the
 * tail-call argument copy/restores. Ordinary and nonvolatile forms retain a
 * four-word scheduling difference. It emits no instructions, is not a memory
 * barrier, and does not establish the original source spelling. */
void cmp_23A918(s32 arg);

void func_0023A958(f32 *pos);

void func_0023A980(f32 *pos);

extern void mdlSetPosition(void *aHandle);

void func_0023A9A8(f32 *pos, void *model);

void func_0023A9F0(f32 *rot, void *model);

extern void mdlLink(void *aHandle, cmpModelLink_t *aModelOt);

void cmp_23AA28(f32 *a, f32 *rot, void *model, s32 index);

void func_0023AA88(s32 value);

s32 func_0023AA90(void);

cmpSeqFunc_t cmp_23AAC0(s32 value);

cmpSeqFunc_t cmp_23AAF8(s32 value);

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 color;
} cmpPrintControl_t;

extern void sdfDevSetPrintCB(cmpPrintControl_t *cb, s32 x, s32 y, s32 z, u32 attr);

extern void *sdfDevPrintfEx(cmpPrintControl_t *cb, const char *str, ...);

void cmpDrawDebugPrint(s32 x, s32 y, s32 z, u32 attr, const char *str, s32 ot);

extern void *dds3PrimFrameRect(s32 x, s32 y, s32 c, s32 w, s32 h, s32 f, s32 g);

void cmpDrawFrameRect(s32 x, s32 y, s32 c, s32 w, s32 h, s32 f, s32 g, s32 ot);

s8 cmpGetStatusModelFlag(void);

void cmpSetStatusModelFlag(s8 flag);

s8 cmpGetStatusModelLoadState(void);

void cmpSetModelReloadFlag(void);

void cmpClearStatusModelLoadState(void);

s32 func_0023AD30(void);

extern void cmpInitSeqInfo(cmpSeqInfo_t *pSeq, cmpSeqInfo_t *pParent);

extern void cmpModelInit(void);

campStatusPanel_t *cmpInitStatus(s8 arg, cmpSeqInfo_t *pParent);

void camp_update_status_Shutdown(void);

extern campStatusPanel_t *cmpInitStatus(s8 arg, cmpSeqInfo_t *pParent);

extern s32 camp_calc_status_Process(s32 id);

extern s32 camp_draw_status_Process(s32 id);

extern s32 camp_update_status_Process(s32 id);

extern const u8 D_0053FEB8[8] __attribute__((section(".rodata")));

void *cmpStartProcessStatus(s8 arg, cmpSeqInfo_t *pParent);

void cmpSetStatusWindowHidden(void);

extern void campFreeStatusModel(void);

s32 camp_calc_status_Process(s32 id);

s32 camp_draw_status_Process(s32 id);

void campFreeStatusModel(void);

void cmpUpdateStatusModelLoad(u16 id);

extern s32 campStatusIsActive(void);

s32 camp_update_status_Process(s32 id);

void *cmpAllocInfoProcess(void *param, s32 pParent);

void camp_update_info_Shutdown(void);

extern s32 camp_calc_info_Process(s32 id);

extern s32 camp_draw_info_Process(s32 id);

extern s32 camp_update_info_Process(s32 id);

s32 cmpStartProcessInfo(s32 text, s32 flag);

void cmpSetInfoWindowHidden(void);

void func_0023B500(s32 value);

s32 camp_calc_info_Process(s32 id);

extern void campDrawInfoText(s32 a, s32 b, s32 color, s32 d, s32 e, s32 f, s32 g);

s32 camp_draw_info_Process(s32 id);

s32 camp_update_info_Process(s32 id);

extern void cmpSetAfterCrossFunc(void *panel, s32 b, s32 c, void *func, void *data, s32 f,
                                 s32 g);

extern void campInitPanelPos(void *panel, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

void camp_H_cmpGlobalWork_t__ctor(void);

void cmpSetCrossGeneralVal(u8 *work, s8 value);

extern void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

void campInitPanelPos(void *panel, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

void cmpSetAfterCrossFunc(void *panel, s32 x, s32 y, void *func, void *data, s32 value, s32 alpha);

typedef struct campCrossPanel_t
{
    /* 0x00 */ void (*func)(void *, void *, s32, s32, s32, s32);
    /* 0x04 */ u8 pos[0x14];
    /* 0x18 */ cmpPadStopWork_t *cursor;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 alpha;
    /* 0x24 */ s8 flag;
    /* 0x25 */ u8 unk_25[7];
} campCrossPanel_t;

void campRunPanelCallback(campCrossPanel_t *panel);

void fclInitCursorWindow(void *work, void *data, u8 count);

void fclSetCursorList(void *work, s32 mode);

s32 cmpCursorBlinkColor(s32 timer);

s32 cmpCursorFadeChannel(s32 color, s32 timer, s32 rate);

typedef struct {
    cmpCursorInfo_t cursor;
    u8 reserved38[4];
    cmpCursorSlot_t slot[256][2];
} cmpCursorExtended_t;

extern void cmpSetCursorSlot(s8 index, s8 state, u8 param, cmpCursorSlot_t *slot);

extern s32 cmpAllocEffectSlot(s32 color, s8 selected, s8 disabled,
                              cmpCursorSlot_t *slot);

u32 cmpDrawListItemBox(cmpCursorInfo_t *cursor, s32 item, s32 row,
                       s32 mode, s32 alpha);

extern u32 cmpDrawListItemBox(cmpCursorInfo_t *cursor, s32 item, s32 row,
                              s32 mode, s32 alpha);

/* Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
void cmpDrawListItemText(s32 x, s32 y, s32 priority, u8 rowHeight,
                         cmpCursorInfo_t *cursor, s32 mode, s32 alpha, s32 ot);

s32 cmpGetBlinkOffset(s32 kind, cmpPanelEff_t *eff);

extern void cmpScaleColorByPhase(s8 phase, u32 *colors, s32 scale);

void cmpDrawListRowBox(s32 x, s32 y, s32 z, u16 w, u16 h2, u16 h, s32 c0, s32 c1, s32 c2,
                       s8 phase, s32 scale, s32 ot);

extern void cmpDrawListRows(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color,
                           s8 mode, s32 scale, s8 phase, s8 layout,
                           cmpCursorInfo_t *cursor, cmpListRowDraw_t draw, s32 ot);

void cmp_23C918(fclPanelPos_t *pos, s32 full, s32 total, s32 color1, s32 color2, s32 flag);

void camp_23C9C0(fclPanelPos_t *pos, s32 width, s32 gap,
                  s32 first, s32 second, s32 third, s32 total,
                  s32 color1, s32 color2, s32 color3, s32 color4, s32 flag);

void camp_23CAF8(s32 x, s32 y, s32 a, s32 b, s32 color, s32 g);

void campDrawInfoText(s32 x, s32 y, s32 z, s32 h, s32 e, s32 color, s32 g);

void camp_23CC70(s32 x, s32 y, s32 z, s32 h, s32 n, s32 f, s32 g, s32 col);

void camp_23CDB8(s32 x, s32 y, s32 z, s32 h, s32 n, s32 f, s32 g, s32 col);

extern frFontHandle_t *frGetCNameFRQ(s8 target, s8 style, frFontHandle_t *chain,
                                     s32 firstOption);

extern void frSetDrawPosition(frFontHandle_t *font, u32 x, u32 y);

extern void frSetPriority(frFontHandle_t *font, u32 priority);

extern void frSetFontColor(frFontHandle_t *font, u32 color);

void cmpDrawStatusBadge(s32 x, s32 y, s32 *colors, u16 statusFlags, s32 unused);

extern char cmpFmtHpMp[];

extern char cmpFmtLevel[];

extern u16 datGetLevelForDraw(datUnitWork_t *work);

void campDrawStatusRow2(s32 inputX, s32 y, u32 color, datUnitWork_t *unit);

extern void fclDrawSpriteRect(s32 x, s32 y, s32 d, s32 w, s32 h, s32 *color,
                              s32 id, s32 idx, s32 *texList,
                              fclSpriteTbl_t *table, s32 ot);

void campDrawStatusRow3(s32 x, s32 y, s32 color, datUnitWork_t *unit);

s32 campGetCursorColorSel(s32 color, cmpCursorSlot_t *slot);

s32 campGetCursorColorAlt(cmpCursorSlot_t *slot);

s32 campGetCursorColorAlt2(s32 color, cmpCursorSlot_t *slot);

s32 campGetStockAlpha(s8 index, s8 row);

s32 campCalcStockScale(s8 index, s8 row);

s32 campDrawStockCell(s32 color, s8 index, s8 row);

extern s8 cmpGetCursorSlotState(s8 index, cmpCursorSlot_t *slot);

extern void campDrawStockStatus(s32 x, s32 y, s32 z, s32 color,
                                s8 cursorMode, s8 detailMode,
                                datUnitWork_t *unit, cmpCursorSlot_t *cursor,
                                s32 alpha);

s32 campDrawStockLabel(s32 x, s32 y, s32 z, s8 slot, s8 row,
                       cmpCursorSlot_t *cursor, s8 cursorMode, s32 alpha);

s32 cmpStockHasRoom(cmpStockWork_t *work, s8 index);

s32 campDrawStockIcon(s8 a, s8 b, s8 *rows, s8 *cols, s8 force);

s32 cmpFindFreeStockSlot(s8 index, s8 room);

void cmpScaleColorByPhase(s8 phase, u32 *colors, s32 scale);

extern void campDrawStockHighlight(s32 x, s32 y, s32 c, s32 d, s32 e, s32 f, s8 g, s32 h,
                                   s32 ot);

s32 campCheckStockFree(s32 x, s32 y, s8 index, s8 room, s8 known, s32 f, s8 g);

typedef struct
{
    /* 0x00 */ s32 pos;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 start;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 target;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 width;
    /* 0x1C */ u8 reflected;
    /* 0x20 */ s32 total;
    /* 0x24 */ s32 left;
    /* 0x28 */ s32 unk_28;
} cmpPanelEffAnim_t;

void cmpRandomizePanelEff(s32 width, s32 height, s32 kind, cmpPanelEffAnim_t *anim);

typedef struct
{
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 step;
    /* 0x02 */ u8 delay;
    /* 0x03 */ u8 kind;
    /* 0x04 */ u32 color4;
    /* 0x08 */ u32 color8;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ u16 width;
    /* 0x1A */ u16 height;
    /* 0x1C */ s32 frame;
    /* 0x20 */ s32 value;
    /* 0x24 */ cmpPanelEffAnim_t anim[16];
} cmpPanelEffState_t;

void cmpDrawPanelEffBar(cmpPanelEffState_t *state, s32 ot);

void cmpUpdatePanelEffEntry(cmpPanelEffState_t *state);

s32 func_0023ECF0(s32 index);

typedef struct
{
    /* 0x0000 */ u8 unk_0000[8];
    /* 0x0008 */ u8 state;
    /* 0x0009 */ u8 step;
    /* 0x000A */ u8 delay;
    /* 0x000B */ u8 kind;
    /* 0x000C */ u8 unk_000C[0x2D8];
} cmpPanelEffView_t;

s32 func_0023ED18(s8 kind, s32 value);

s32 cmpSetPanelEffParam(s32 index, s32 value);

s32 func_0023EDB0(s32 index);

s8 rstGetPanelEffState(s32 index);

extern cmpPanelEff_t *volatile cmpPanelEffLive FAKEMATCH_ASM_NAME("cmpPanelEffList");

extern void cmpRandomizePanelEff(s32 width, s32 height, s32 kind,
                                cmpPanelEffAnim_t *anim);

extern void cmpAddPanelEffEntry(s32 index, s32 x, s32 y, s32 z, u16 width, u16 height,
                                u8 step, u8 delay, s32 color4, s32 color8, u8 kind);

s32 cmpAddQuePanelEff(s32 a1, s32 a2, s32 a3, u16 a4, u16 a5, u8 a6, u8 a7, s32 a8, s32 a9, u8 a10);

void cmpDrawPanelEffRing(cmpPanelEffState_t *state, s32 ot);

extern void cmpDrawPanelEffBox(void *entry, s32 arg);

extern void cmpDrawPanelEffRing(cmpPanelEffState_t *entry, s32 arg);

extern void cmpDrawPanelEffQuad(cmpPanelEffState_t *entry, s32 arg);

extern const s32 cmpPanelEffCoords[12];

void cmpFindPanelEffSlot(s32 arg);

void cmpUpdatePanelEffSlots(void);

s32 camp_update_paneleff_Process(void);

void cmpClearPanelEffSlots(cmpPanelEff_t *list);

cmpPanelEff_t *cmpAllocPanelEffWork(s32 value);

void camp_update_paneleff_Shutdown(void);

extern cmpPanelEff_t *cmpAllocPanelEffWork(s32 value);

void *cmpStartProcessPanelEff(s32 parent);

void rstSetPanelEffFlag(void);

void rstSetStatusModelReady(s32 ready);

mdlObj_t *cmpGetEncycWork(void);

u16 cmpGetStatusModelId(void);

extern s32 mdlGetNumOfCut(mdlObj_t *work, s32 index);

void cmp_23F8F0(u16 id, s32 delta);

void func_0023F9F8(u16 id, f32 delta);

void func_0023FA98(u16 id, s32 x, s32 y, s32 z);

void func_0023FB10(u16 id, s32 x, s32 y, s32 z);

u8 cmpGetModelMotionId(u16 id);

f32 cmpGetModelScale(u16 id);

void rstGetEvoModelParam(u16 id, f32 *out);

void cmpGetModelPosRot(u16 id, f32 *out);

extern void func_0023FCC0(void);

void cmpModelInitCore2(void);

extern const f32 cmpDefaultLightColor[];

extern const u32 cmpDefaultLightDir[];

extern const u32 cmpDefaultAmbientColor[];

extern const u32 cmpDefaultFogColor[];

extern f32 D_00345F60[];

extern void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias);

extern void dds3SetLightColor(s32 frame, s32 number, f32 *color);

extern void dds3SetLightDir(s32 frame, s32 number, f32 *direction);

extern void dds3SetAmbientColor(s32 frame, f32 *color);

extern void dds3SetFogColor(s32 frame, f32 *color);

extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 nearPlane, f32 farPlane);

extern void sdf3DSetCamera(sdf3DScreenParam_t *screenParam, void *camera);

void func_0023FCC0(void);

void cmpModelInit(void);

void cmpResetDrawEnv(void);

extern void mdlFree(mdlObj_t *aHandle);

extern void cmpFreeBadEffect(void);

extern const char cmpModelFreeMsg[];

void cmpModelFreeResource(void);

s32 cmpPrepareStatusModel(s32 group, s32 id, s32 blocking);

typedef struct
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 scl;
    void *hModel;
} cmpBadEffectParams_t;

extern datUnitVisual_t datModelParamTable[];

void cmpCreateBadEffect(u16 id, void *model, u8 *buffer);

extern void *hCAMPBadEffect;

extern u8 D_00349300[];

extern const char cmpBadEffReadyMsg[];

extern void *effCAMPBadCreate(u8 *work, s32 kind);

s32 cmpBadEffSetup(u16 id, u32 flag);

extern void *hCAMPBadEffect;

extern void effCAMPBadDelete(void *hCAMPBadEffect);

extern const char cmpBadEffFreeMsg[];

void cmpFreeBadEffect(void);

extern const char cmpModelPrepareMsg[];

void rstPrepareStatusModel(s32 group, s32 id, s32 blocking);

s32 cmpModelSetup(u16 id, s32 blocking);

extern const char cmpModelReadyMsg[];

extern mdlObj_t *mdlLoad(u32 group, u32 id);

extern void cmpGetStatusModelMotion();

s32 cmpLoadStatusModel(void);

#include "motion.h"

extern void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern datUnitVisual_t datModelParamTable[];

extern void mdlSetScale(void *aHandle);

f32 cmpGetStatusModelScale(void *model, s32 flag);

void cmpResetStatusModelPos(void);

void cmpSetStatusModelPose(mdlObj_t *model);

void cmpSetModelRotate(void *model);

extern f32 dds3QuakeOffset[];

void cmpSetStatusModelParam(void);

extern void cmpSetStatusModelPose(mdlObj_t *model);

extern void cmpSetModelRotate(void *model);

extern void cmpSetStatusModelParam(void);

extern void effCAMPBadDraw(void *hCAMPBadEffect);

s32 cmpDrawStatusModel(void *scene);

extern dds3GlobalWork_t *volatile cmpRebuildGlobalWork FAKEMATCH_ASM_NAME("DDS3_GBWK");

void cmpRemoveStockSlot(s8 index);

void cmpCompactStockList(s8 devilId);

void cmpClearStockFlag(datUnitWork_t *work);

void cmpSetStockFlag(datUnitWork_t *work);

void cmpSetMaxHPMP(datUnitWork_t *unit);

s8 cmpGetParamGaugeColor(s8 index);

extern const char cmpDebugIdFmt[];

extern s32 datExecSkill(s32 skill, datUnitWork_t *user, datUnitWork_t *target);

void cmpExecRecover(u16 skill, datUnitWork_t *user, datUnitWork_t *target);

s32 cmp_240C10(void);

u16 cmpChkSkillBad(u16 skill);

s32 cmpGetSkillEntry(u16 id, datUnitWork_t *unit);

s32 cmpAddCost(u16 skill, datUnitWork_t *unit);

s32 cmpGetSkillTargetArea(u16 skillId);

extern s32 cmbGetParamBase(datUnitWork_t *unit, s32 index);

extern const f32 cmpDefaultLightColor[4];

extern const u32 cmpDefaultLightDir[4];

extern const u32 cmpDefaultAmbientColor[4];

extern const u32 cmpDefaultFogColor[4];

extern const char cmpModelFreeMsg[24];

extern const char cmpBadEffReadyMsg[20];

extern const u32 D_00540054 __attribute__((section(".rodata")));

extern const char cmpBadEffFreeMsg[24];

extern const char cmpModelPrepareMsg[32];

extern const char cmpModelReadyMsg[32];

extern const char cmpTextVitality[16];

extern const char cmpTextIntelligence[16];

extern const char cmpTextStrength[16];

extern const char cmpDebugIdFmt[16];

s32 cmpUseItemKou(u16 item, datUnitWork_t *unit);

s32 cmpUseItemTokusyu(u16 item, saddr_t target, saddr_t user);

extern void fldEsutoMaStart(void);

extern void fldRiberaMaStart(void);

extern void fldRifutoMaStart(void);

extern void fldLightMaStart(void);

s32 cmpUseSkillTokusyu(u16 skill, datUnitWork_t *user, saddr_t unused);

void cmpRecover(u16 skill, datUnitWork_t *user, datUnitWork_t *target);

char *cmpGetParamName(s8 index);

s32 cmpGetPressedButton(s32 mask);

void func_002413B8(u32 button, s8 mode);

/* This UI entry fixes the shared cursor-sound helper's mode at zero. */
void cmpPlayCursorSE(u32 button);

s32 cmpChkPadPressUpDown(void);

s32 cmpGetPadUpDown(void);

s32 func_002415D8(void);

s32 cmpAllocEffectSlot(s32 color, s8 a, s8 b, cmpCursorSlot_t *slot);

void cmpClearCursorSlots(cmpCursorInfo_t *cur);

s32 cmpGetCursorSlotValue(s8 index, cmpCursorSlot_t *slot);

s8 cmpGetCursorSlotState(s8 index, cmpCursorSlot_t *slot);

/* The offset and update pointer have real, separate native address lifetimes.
 * Keep their two short-lived registers; neither crosses a call. The empty
 * value-base operand preserves the conditional address phase, not memory order.
 * These are local matching aids, not evidence of historical source spelling. */
void cmpUpdateCursorSlots(cmpCursorInfo_t *cursor);

void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);

void cmpInitCursorRange(void *cursor, u8 index, u8 rows, saddr_t flag);

void cmpUpdateCursorScroll(cmpCursorInfo_t *cursor, s8 mode);

void cmpSetCursorFlag(cmpCursorInfo_t *cur, saddr_t flag);

void cmp_241A10(cmpCursorInfo_t *cursor);

s32 cmp_241A88(cmpCursorInfo_t *cursor, s32 index);

void cmpResetCursorPos(s32 value, cmpCursorInfo_t *cursor);

void cmpSetCursorRow(s32 row, cmpCursorInfo_t *cursor);

void cmpResetCursorInfo(cmpCursorInfo_t *cursor);

s32 rcvRootHasItems(rcvRootWork_t *work);

s32 cmpCursorAtFirstEnabled(cmpCursorInfo_t *cursor);

s32 cmpCursorAtLastEnabled(cmpCursorInfo_t *cursor);

void cmpAdvanceCursorPhase(cmpCursorInfo_t *cursor);

s32 cmpMoveCursor(cmpCursorInfo_t *cur, s8 dir);

s32 cmpGetCursorMoveMode(cmpCursorInfo_t *cursor);

s32 cmpMoveCursor2(cmpCursorInfo_t *cursor, s8 dir);

void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor);

s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

s8 cmpGetCursorRow(cmpCursorInfo_t *cursor);

extern s32 cmpMoveCursor2(cmpCursorInfo_t *cursor, s8 dir);

void cmpMoveStockCursor(s8 index, s8 dir, u8 *base);

void func_00242320(s8 value, u8 *work);

/* The alternate mask value feeds the final availability test. This empty
 * operand preserves the native branch-shaped selection in ee-gcc. It emits
 * no instructions and is a local reconstruction
 * aid, not evidence of the original source spelling. */
s32 cmpCursorSlotIsSelectable(datUnitWork_t *unit, u16 inputMask, s8 flag);

s8 cmpGetStockInfoIndex(cmpLocalStock_t *stock, s8 index, s8 flag);

s8 cmpGetStockCount(void);

void cmpSortStockList(void);

s8 func_002426F0(void);

s8 func_00242710(u8 id);

void cmpDebugDumpCount(void);

void cmpInitSeqInfo(cmpSeqInfo_t *pSeq, cmpSeqInfo_t *pParent);

s32 cmpChkSeqInfo(cmbGbWork_t *work);

s32 cmpUpdateSeqInfo(cmbGbWork_t *work, s8 value);

void cmpUpdateTerminate(cmpInfoWindow_t *window);

void cmp_242918(s32 id, u8 bits);

void cmp_242958(s32 id, u8 bits);

u8 cmp_242998(s32 id);

extern datSkill_t attr[];

s32 func_002429C0(void);

s32 cmpItemHasFlag(u16 item, s8 which);

void func_00242A70(s32 key);

s32 campStatusIsActive(void);

extern const char D_00540128[16];

extern const char D_00540138[16];

extern const char D_00540148[16];

extern const char D_00540158[16];

extern const char D_00540168[16];

extern const char D_00540178[16];

extern const char D_00540188[16];

extern const char D_00540198[16];

extern const char cmpTextProtegonist[16];

extern const char D_005401B8[16];

extern const char cmpTextSakahagi[16];

extern const char D_005401D8[16];

extern const char D_005401E8[16];

extern const char D_005401F8[16];

extern const char D_00540208[16];

extern const char D_00540218[16];

extern const char D_00540228[16];

extern const char D_00540238[16];

extern const char D_00540248[16];

extern const char D_00540258[16];

extern const char D_00540268[16];

extern const char D_00540278[16];

extern const char D_00540288[16];

extern const char D_00540298[16];

extern const char D_005402A8[16];

extern const char D_005402B8[16];

extern const char D_005402C8[16];

extern const char D_005402D8[16];

extern const char D_005402E8[16];

extern const char D_005402F8[16];

extern const char D_00540308[16];

extern const char D_00540318[16];

extern const char D_00540328[16];

extern const char D_00540338[16];

extern const char D_00540348[16];

extern const char D_00540358[16];

extern const char D_00540368[16];

extern const char D_00540378[16];

extern const char D_00540388[16];

extern const char D_00540398[16];

extern const char D_005403A8[16];

extern const char D_005403B8[16];

extern const char D_005403C8[16];

extern const char D_005403D8[16];

extern const char D_005403E8[16];

extern const char D_005403F8[16];

extern const char D_00540408[16];

extern const char D_00540418[16];

extern const char D_00540428[16];

extern const char D_00540438[16];

extern const char D_00540448[16];

extern const char D_00540458[16];

extern const char D_00540468[16];

extern const char D_00540478[16];

extern const char D_00540488[16];

extern const char D_00540498[16];

extern const char D_005404A8[16];

extern const char D_005404B8[16];

extern const char D_005404C8[16];

extern const char D_005404D8[16];

extern const char D_005404E8[16];

extern const char D_005404F8[16];

extern const char D_00540508[16];

extern const char D_00540518[16];

extern const char D_00540528[16];

extern const char D_00540538[16];

extern const char D_00540548[16];

extern const char D_00540558[16];

extern const char D_00540568[16];

extern const char D_00540578[16];

extern const char D_00540588[16];

extern const char D_00540598[16];

extern const char D_005405A8[16];

extern const char D_005405B8[16];

extern const char D_005405C8[16];

extern const char D_005405D8[32];

extern const char D_005405F8[24];

extern const char D_00540610[24];

extern const char D_00540628[24];

extern const char D_00540640[16];

extern const char D_00540650[24];

extern const char D_00540668[40];

extern const char D_00540690[24];

extern const char D_005406A8[40];

extern const char D_005406D0[32];

extern const char D_005406F0[24];

extern const char D_00540708[16];

extern const char D_00540718[32];

extern const char D_00540738[32];

extern const char D_00540758[24];

extern const char D_00540770[40];

extern const char D_00540798[16];

extern const char D_005407A8[24];

extern const char D_005407C0[24];

extern const char D_005407D8[16];

extern const char D_005407E8[16];

extern const char D_005407F8[48];

extern const char D_00540828[40];

extern const char D_00540850[56];

extern const char D_00540888[48];

extern const char D_005408B8[48];

extern const char D_005408E8[32];

extern const char D_00540908[40];

extern const char D_00540930[40];

extern const char D_00540958[32];

extern const char D_00540978[32];

extern const char D_00540998[32];

extern const char D_005409B8[48];

extern const char D_005409E8[40];

extern const char D_00540A10[40];

extern const char D_00540A38[24];

extern const char D_00540A50[16];

extern const char D_00540A60[40];

extern const char D_00540A88[24];

extern const char D_00540AA0[40];

extern const char D_00540AC8[32];

extern const char D_00540AE8[32];

extern const char D_00540B08[24];

extern const char D_00540B20[24];

extern const char D_00540B38[40];

extern const char D_00540B60[24];

extern const char D_00540B78[40];

extern const char D_00540BA0[40];

extern const char D_00540BC8[32];

extern const char D_00540BE8[32];

extern const char D_00540C08[24];

extern const char D_00540C20[24];

extern const char D_00540C38[24];

extern const char D_00540C50[48];

extern const char D_00540C80[24];

extern const char D_00540C98[32];

extern const char D_00540CB8[24];

extern const char D_00540CD0[32];

extern const char D_00540CF0[40];

extern const char D_00540D18[24];

extern const char D_00540D30[40];

extern const char D_00540D58[24];

extern const char D_00540D70[40];

extern const char D_00540D98[40];

extern const char D_00540DC0[40];

extern const char D_00540DE8[40];

extern const char D_00540E10[24];

extern const char D_00540E28[48];

extern const char D_00540E58[32];

extern const char D_00540E78[32];

extern const char D_00540E98[40];

extern const char D_00540EC0[40];

extern const char D_00540EE8[32];

extern const char D_00540F08[40];

extern const char D_00540F30[32];

extern const char D_00540F50[32];

extern const char D_00540F70[24];

extern const char D_00540F88[24];

extern const char D_00540FA0[32];

extern const char D_00540FC0[24];

extern const char D_00540FD8[16];

extern const char D_00540FE8[32];

extern const char D_00541008[48];

extern const char D_00541038[40];

extern const char D_00541060[40];

extern const char D_00541088[24];

extern const char D_005410A0[24];

extern const char D_005410B8[24];

extern const char D_005410D0[24];

extern const char D_005410E8[40];

extern const char D_00541110[32];

extern const char D_00541130[24];

extern const char D_00541148[40];

extern const char D_00541170[24];

extern const char D_00541188[24];

extern const char D_005411A0[32];

extern const char D_005411C0[24];

extern const char D_005411D8[24];

extern const char D_005411F0[40];

extern const char D_00541218[24];

extern const char D_00541230[24];

extern const char D_00541248[32];

extern const char D_00541268[24];

extern const char D_00541280[32];

extern const char D_005412A0[32];

extern const char D_005412C0[24];

extern const char D_005412D8[32];

extern const char D_005412F8[32];

extern const char D_00541318[24];

extern const char D_00541330[40];

extern const char D_00541358[40];

extern const char D_00541380[32];

extern const char D_005413A0[32];

extern const char D_005413C0[32];

extern const char D_005413E0[40];

extern const char D_00541408[40];

extern const char D_00541430[24];

extern const char D_00541448[24];

extern const char D_00541460[24];

extern const char D_00541478[32];

extern const char D_00541498[32];

extern const char D_005414B8[32];

extern const char D_005414D8[32];

extern const char D_005414F8[32];

extern const char D_00541518[40];

extern const char D_00541540[24];

extern const char D_00541558[16];

extern const char D_00541568[24];

extern const char D_00541580[32];

extern const char D_005415A0[24];

extern const char D_005415B8[24];

extern const char D_005415D0[32];

extern const char D_005415F0[24];

extern const char D_00541608[24];

extern const char D_00541620[24];

extern const char D_00541638[16];

extern const char D_00541648[24];

extern const char D_00541660[24];

extern const char D_00541678[32];

extern const char D_00541698[24];

extern const char D_005416B0[32];

extern const char D_005416D0[32];

extern const char D_005416F0[32];

extern const char D_00541710[40];

extern const char D_00541738[40];

extern const char D_00541760[32];

extern const char D_00541780[32];

extern const char D_005417A0[32];

extern const char D_005417C0[32];

extern const char D_005417E0[40];

extern const char D_00541808[32];

extern const char D_00541828[40];

extern const char D_00541850[40];

extern const char D_00541878[40];

extern const char D_005418A0[40];

extern const char D_005418C8[24];

extern const char D_005418E0[40];

extern const char D_00541908[32];

extern const char D_00541928[40];

extern const char D_00541950[32];

extern const char D_00541970[32];

extern const char D_00541990[24];

extern const char D_005419A8[40];

extern const char D_005419D0[32];

extern const char D_005419F0[24];

extern const char D_00541A08[40];

extern const char D_00541A30[40];

extern const char D_00541A58[32];

extern const char D_00541A78[32];

extern const char D_00541A98[32];

extern const char D_00541AB8[40];

extern const char D_00541AE0[40];

extern const char D_00541B08[40];

extern const char D_00541B30[40];

extern const char D_00541B58[32];

extern const char D_00541B78[24];

extern const char D_00541B90[40];

extern const char D_00541BB8[40];

extern const char D_00541BE0[16];

extern const char cmpTextStarKey[16];

extern const char cmpTextAfterlifeBell[16];

extern const char cmpTextBlackVisor[16];

extern const char cmpTextDeathstone[16];

extern const char cmpTextMoonKey[16];

extern const char cmpTextKnowledge[16];

extern const char cmpTextSovereignty[16];

extern const char cmpTextFoundation[16];

extern const char cmpTextEternity[16];

extern const char cmpTextGodliness[16];

extern const char cmpTextCompassion[16];

extern const char cmpTextNetherstone[16];

extern const char cmpTextEarthstone[16];

extern const char cmpTextHeavenstone[16];

extern const char cmpTextParkKey[16];

extern const char cmpTextLordSSword[16];

extern const char cmpTextKimonStone[16];

extern const char cmpTextHimorogi[16];

extern const char cmpTextUmugiStone[16];

extern const char cmpText1000YenBill[16];

extern const char cmpTextRedKila[16];

extern const char cmpTextGreenKila[16];

extern const char cmpTextYellowKila[16];

extern const char cmpTextWhiteKila[16];

extern const char cmpTextAnnexPass[16];

extern const char cmpTextAquamarine[16];

extern const char cmpTextTurquoise[16];

extern const char cmpTextAmethyst[16];

extern const char cmpTextSapphire[16];

extern const char cmpTextMasakados[16];

extern const char cmpTextKamurogi[16];

extern const char cmpTextMurakumo[16];

extern const char cmpTextAnathema[16];

extern const char cmpTextNarukami[16];

extern const char cmpTextShiranui[16];

extern const char cmpTextIyomante[16];

extern const char cmpTextWadatsumi[16];

extern const char cmpTextMarogareh[16];

extern const char cmpTextLuckyTicket[16];

extern const char cmpTextDiviningWater[16];

extern const char cmpTextSmokeBall[16];

extern const char cmpTextLightBall[16];

extern const char cmpTextFloatBall[16];

extern const char cmpTextAttractPipe[16];

extern const char cmpTextRepulseBell[16];

extern const char cmpTextSpyglass[16];

extern const char cmpTextSoulReturn[16];

extern const char cmpTextBlessedFan[16];

extern const char cmpTextLuIncense[16];

extern const char cmpTextAgIncense[16];

extern const char cmpTextViIncense[16];

extern const char cmpTextMaIncense[16];

extern const char cmpTextStIncense[16];

extern const char cmpTextTetrajaRock[16];

extern const char cmpTextDekajaRock[16];

extern const char cmpTextAttackMirror[16];

extern const char cmpTextMagicMirror[16];

extern const char cmpTextPotOfDeath[16];

extern const char cmpTextMakajamRock[16];

extern const char cmpTextWagtailPlume[16];

extern const char cmpTextPoisonArrow[16];

extern const char cmpTextMegidolaRock[16];

extern const char cmpTextMegidoRock[16];

extern const char cmpTextMamudoRock[16];

extern const char cmpTextMahamaRock[16];

extern const char cmpTextMazanRock[16];

extern const char cmpTextMazioRock[16];

extern const char cmpTextMabufuRock[16];

extern const char cmpTextMaragiRock[16];

extern const char cmpTextDisStone[16];

extern const char cmpTextDisMute[16];

extern const char cmpTextDisCharm[16];

extern const char cmpTextDisStun[16];

extern const char cmpTextDisPoison[16];

extern const char cmpTextSacredWater[16];

extern const char cmpTextBalmOfRising[16];

extern const char cmpTextRevivalBead[16];

extern const char cmpTextChakraElixir[16];

extern const char cmpTextBeadOfLife[16];

extern const char cmpTextSomaDroplet[16];

extern const char cmpTextGreatChakra[16];

extern const char cmpTextChakraPot[16];

extern const char cmpTextChakraDrop[16];

extern const char cmpTextBeadChain[16];

extern const char cmpTextLifeStone[16];

extern const char cmpTextMedicine[16];

extern const char cmpTextMuscleDrink[16];

extern const char D_00542160[16];

extern const char cmpTextCallEvil[16];

extern const char cmpTextCallSouls[16];

extern const char cmpTextCallSoldier[16];

extern const char cmpTextCallAngel[16];

extern const char cmpTextJiveTalk[16];

extern const char cmpTextWineParty[16];

extern const char cmpTextMaidenPlea[16];

extern const char cmpTextIntimidate[16];

extern const char cmpTextPersuade[16];

extern const char cmpTextGonnection[16];

extern const char cmpTextArbitration[16];

extern const char cmpTextStoneHunt[16];

extern const char D_00542230[16];

extern const char D_00542240[16];

extern const char D_00542250[16];

extern const char D_00542260[16];

extern const char D_00542270[16];

extern const char D_00542280[16];

extern const char D_00542290[16];

extern const char D_005422A0[16];

extern const char D_005422B0[16];

extern const char D_005422C0[16];

extern const char D_005422D0[16];

extern const char D_005422E0[16];

extern const char D_005422F0[16];

extern const char D_00542300[16];

extern const char D_00542310[16];

extern const char D_00542320[16];

extern const char D_00542330[16];

extern const char D_00542340[16];

extern const char D_00542350[16];

extern const char D_00542360[16];

extern const char D_00542370[16];

extern const char D_00542380[16];

extern const char D_00542390[16];

extern const char D_005423A0[16];

extern const char D_005423B0[16];

extern const char D_005423C0[16];

extern const char D_005423D0[16];

extern const char D_005423E0[16];

extern const char D_005423F0[16];

extern const char D_00542400[16];

extern const char D_00542410[16];

extern const char D_00542420[16];

extern const char D_00542430[16];

extern const char D_00542440[16];

extern const char D_00542450[16];

extern const char D_00542460[16];

extern const char D_00542470[16];

extern const char D_00542480[16];

extern const char D_00542490[16];

extern const char D_005424A0[16];

extern const char D_005424B0[16];

extern const char D_005424C0[16];

extern const char D_005424D0[16];

extern const char D_005424E0[16];

extern const char D_005424F0[16];

extern const char D_00542500[16];

extern const char D_00542510[16];

extern const char D_00542520[16];

extern const char D_00542530[16];

extern const char D_00542540[16];

extern const char D_00542550[16];

extern const char D_00542560[16];

extern const char D_00542570[16];

extern const char D_00542580[16];

extern const char D_00542590[16];

extern const char D_005425A0[16];

extern const char D_005425B0[16];

extern const char D_005425C0[16];

extern const char D_005425D0[16];

extern const char D_005425E0[16];

extern const char D_005425F0[16];

extern const char D_00542600[16];

extern const char D_00542610[16];

extern const char D_00542620[16];

extern const char D_00542630[16];

extern const char D_00542640[16];

extern const char D_00542650[16];

extern const char D_00542660[16];

extern const char D_00542670[16];

extern const char D_00542680[16];

extern const char D_00542690[16];

extern const char D_005426A0[16];

extern const char D_005426B0[16];

extern const char D_005426C0[16];

extern const char D_005426D0[16];

extern const char D_005426E0[16];

extern const char D_005426F0[16];

extern const char D_00542700[16];

extern const char D_00542710[16];

extern const char D_00542720[16];

extern const char D_00542730[16];

extern const char D_00542740[16];

extern const char D_00542750[16];

extern const char D_00542760[16];

extern const char D_00542770[16];

extern const char D_00542780[16];

extern const char D_00542790[16];

extern const char D_005427A0[16];

extern const char D_005427B0[16];

extern const char D_005427C0[16];

extern const char D_005427D0[16];

extern const char D_005427E0[16];

extern const char D_005427F0[16];

extern const char D_00542800[16];

extern const char D_00542810[16];

extern const char D_00542820[16];

extern const char D_00542830[16];

extern const char D_00542840[16];

extern const char D_00542850[16];

extern const char D_00542860[16];

extern const char cmpTextLaughter[16];

extern const char cmpTextDarkHowl[16];

extern const char cmpTextGathering[16];

extern const char cmpTextBeckonCall[16];

extern const char cmpTextInfiniteLight[16];

extern const char cmpTextDragonEye[16];

extern const char cmpTextBeastEye[16];

extern const char cmpTextIntoxicate[16];

extern const char cmpTextPanicVoice[16];

extern const char cmpTextSexyGaze[16];

extern const char cmpTextSonicWave[16];

extern const char cmpTextEternalRest[16];

extern const char cmpTextBindingCry[16];

extern const char cmpTextStunGaze[16];

extern const char cmpTextSolNiger[16];

extern const char cmpTextDismalTune[16];

extern const char cmpTextDebilitate[16];

extern const char cmpTextFogBreath[16];

extern const char cmpTextToxicCloud[16];

extern const char cmpTextBaelSBane[16];

extern const char cmpTextEvilGaze[16];

extern const char cmpTextMuteGaze[16];

extern const char cmpTextStoneGaze[16];

extern const char cmpTextHellGaze[16];

extern const char cmpTextRadiance[16];

extern const char cmpTextStarlight[16];

extern const char cmpTextVioletFlash[16];

extern const char cmpTextLifeDrain[16];

extern const char cmpTextManaDrain[16];

extern const char cmpTextDeathtouch[16];

extern const char cmpTextHolyWrath[16];

extern const char cmpTextThunderclap[16];

extern const char cmpTextWetWind[16];

extern const char cmpTextWindCutter[16];

extern const char cmpTextWingBuffet[16];

extern const char cmpTextBoltStorm[16];

extern const char cmpTextGlacialBlast[16];

extern const char cmpTextIceBreath[16];

extern const char cmpTextRagnarok[16];

extern const char cmpTextProminence[16];

extern const char cmpTextHellfire[16];

extern const char cmpTextFireBreath[16];

extern const char cmpTextTentacle[16];

extern const char cmpTextExtraHit[16];

extern const char cmpTextRetaliate[16];

extern const char cmpTextGaeaRage[16];

extern const char cmpTextMagmaAxis[16];

extern const char cmpTextSpiralViper[16];

extern const char cmpTextEvilMelody[16];

extern const char cmpTextHolyMelody[16];

extern const char D_00542B90[16];

extern const char D_00542BA0[16];

extern const char D_00542BB0[16];

extern const char D_00542BC0[16];

extern const char D_00542BD0[16];

extern const char D_00542BE0[16];

extern const char D_00542BF0[16];

extern const char D_00542C00[16];

extern const char D_00542C10[16];

extern const char D_00542C20[16];

extern const char D_00542C30[16];

extern const char D_00542C40[16];

extern const char D_00542C50[16];

extern const char D_00542C60[16];

extern const char D_00542C70[16];

extern const char D_00542C80[16];

extern const char D_00542C90[16];

extern const char D_00542CA0[16];

extern const char D_00542CB0[16];

extern const char D_00542CC0[16];

extern const char D_00542CD0[16];

extern const char D_00542CE0[16];

extern const char D_00542CF0[16];

extern const char D_00542D00[16];

extern const char D_00542D10[16];

extern const char D_00542D20[16];

extern const char D_00542D30[16];

extern const char D_00542D40[16];

extern const char D_00542D50[16];

extern const char D_00542D60[16];

extern const char D_00542D70[16];

extern const char D_00542D80[16];

extern const char D_00542D90[16];

extern const char D_00542DA0[16];

extern const char D_00542DB0[16];

extern const char D_00542DC0[16];

extern const char D_00542DD0[16];

extern const char D_00542DE0[16];

extern const char D_00542DF0[16];

extern const char D_00542E00[16];

extern const char D_00542E10[16];

extern const char D_00542E20[16];

extern const char D_00542E30[16];

extern const char D_00542E40[16];

extern const char D_00542E50[16];

extern const char D_00542E60[16];

extern const char D_00542E70[16];

extern const char D_00542E80[16];

extern const char D_00542E90[16];

extern const char D_00542EA0[16];

extern const char D_00542EB0[16];

extern const char D_00542EC0[16];

extern const char D_00542ED0[16];

extern const char D_00542EE0[16];

extern const char D_00542EF0[16];

extern const char D_00542F00[16];

extern const char D_00542F10[16];

extern const char D_00542F20[16];

extern const char D_00542F30[16];

extern const char D_00542F40[16];

extern const char D_00542F50[16];

extern const char D_00542F60[16];

extern const char D_00542F70[16];

extern const char D_00542F80[16];

extern const char D_00542F90[16];

extern const char D_00542FA0[16];

extern const char D_00542FB0[16];

extern const char D_00542FC0[16];

extern const char D_00542FD0[16];

extern const char D_00542FE0[16];

extern const char D_00542FF0[16];

extern const char D_00543000[24];

extern const char D_00543018[16];

extern const char D_00543028[24];

extern const char D_00543040[16];

extern const char D_00543050[16];

extern const char D_00543060[16];

extern const char D_00543070[16];

extern const char D_00543080[24];

extern const char D_00543098[16];

extern const char D_005430A8[24];

extern campWork_t *campWork;

extern s8 campExitState;

extern char cmpTextLoad[8];

extern char cmpTextConfig[8];

extern char cmpTextStatus[8];

extern char cmpTextParty[8];

extern char cmpTextSkill[8];

extern char cmpTextItem[8];

extern char cmpTextGems[8];

extern char cmpTextDiscard[8];

extern char cmpTextUse[8];

extern char cmpTextSummon[8];

extern char cmpTextUnequip[8];

extern char cmpTextEquip[8];

extern char cmpCalcProcName[8];

extern char cmpFmtNameCount[8];

extern char cmpTextGEMS[8];

extern char cmpTextITEM[8];

extern char D_0054FE58[8];

extern char D_0054FE60[8];

extern char D_0054FE68[8];

extern char cmpFmtMaka[4];

extern campStatusPanel_t *campStatusPanel;

extern campInfoWork_t *campInfoWork;

extern char campDhFmtAmount[8];

extern s32 cmpStatTimer;

extern u16 rstHeartsMasterPhase;

extern datUnitWork_t *cmbBirthPanel;

extern char cmpFmtDec2[8];

extern char cmpFmtValMax[8];

extern char cmpFmtExpNext[8];

extern const char cmpFmtItemQty[8];

extern char cmpFmtSkillCost[4];

extern s32 D_0054FEBC;

extern campStatusPanel_t *cmpStatusPhase;

extern s8 cmpStatusWindowHidden;

extern s8 cmpStatusModelLoadState;

extern cmpInfoWindow_t *cmpInfoWindow;

extern char cmpFmtHpMp[8];

extern char cmpFmtLevel[4];

extern cmpPanelEff_t *cmpPanelEffList;

extern s32 cmpStatusModelResult;

extern mdlObj_t *cmpEncycWork;

extern void *hCAMPBadEffect;

extern s8 cmpStatusModelReady;

extern s8 cmpModelInitFlag;

extern u16 cmpStatusGroup;

extern u16 cmpStatusModelId;

extern s32 cmpModelHandle;

extern char cmpTextLuck[8];

extern char cmpTextAgility[8];

extern char cmpTextMagic[8];

extern s32 campStatusActive;

extern s32 cmbRaceTable;

extern s32 nbEncountMaxBase;

extern char D_0054FF20[8];

extern char D_0054FF28[8];

extern char D_0054FF30[8];

extern char D_0054FF38[8];

extern char D_0054FF40[8];

extern char D_0054FF48[8];

extern char D_0054FF50[8];

extern char D_0054FF58[8];

extern char D_0054FF60[8];

extern char D_0054FF68[8];

extern char D_0054FF70[8];

extern char D_0054FF78[8];

extern char D_0054FF80[8];

extern char D_0054FF88[8];

extern char D_0054FF90[8];

extern char D_0054FF98[8];

extern char D_0054FFA0[8];

extern char D_0054FFA8[8];

extern char D_0054FFB0[8];

extern char D_0054FFB8[8];

extern char D_0054FFC0[8];

extern char D_0054FFC8[8];

extern char D_0054FFD0[8];

extern char D_0054FFD8[8];

extern char D_0054FFE0[8];

extern char D_0054FFE8[8];

extern char D_0054FFF0[8];

extern char D_0054FFF8[8];

extern char D_00550000[8];

extern char D_00550008[8];

extern char D_00550010[8];

extern char D_00550018[8];

extern char D_00550020[8];

extern char D_00550028[8];

extern char D_00550030[8];

extern char D_00550038[8];

extern char D_00550040[8];

extern char D_00550048[8];

extern char D_00550050[8];

extern char D_00550058[8];

extern char D_00550060[8];

extern char D_00550068[8];

extern char D_00550070[8];

extern char D_00550078[8];

extern char D_00550080[8];

extern char D_00550088[8];

extern char D_00550090[8];

extern char D_00550098[8];

extern char D_005500A0[8];

extern char D_005500A8[8];

extern char D_005500B0[8];

extern char D_005500B8[8];

extern char D_005500C0[8];

extern char D_005500C8[8];

extern char D_005500D0[8];

extern char D_005500D8[8];

extern char D_005500E0[8];

extern char D_005500E8[8];

extern char D_005500F0[8];

extern char D_005500F8[8];

extern char D_00550100[8];

extern char D_00550108[8];

extern char D_00550110[8];

extern char D_00550118[8];

extern char D_00550120[8];

extern char D_00550128[8];

extern char D_00550130[8];

extern char D_00550138[8];

extern char D_00550140[8];

extern char D_00550148[8];

extern char D_00550150[8];

extern char D_00550158[8];

extern char D_00550160[8];

extern char D_00550168[8];

extern char D_00550170[8];

extern char D_00550178[8];

extern char D_00550180[8];

extern char D_00550188[8];

extern char D_00550190[8];

extern char D_00550198[8];

extern char D_005501A0[8];

extern char D_005501A8[8];

extern char D_005501B0[8];

extern char D_005501B8[8];

extern char D_005501C0[8];

extern char D_005501C8[8];

extern char D_005501D0[8];

extern char D_005501D8[8];

extern char D_005501E0[8];

extern char D_005501E8[8];

extern char D_005501F0[8];

extern char D_005501F8[8];

extern char D_00550200[8];

extern char D_00550208[8];

extern char D_00550210[8];

extern char D_00550218[8];

extern char D_00550220[8];

extern char D_00550228[8];

extern char D_00550230[8];

extern char D_00550238[8];

extern char D_00550240[8];

extern char D_00550248[8];

extern char D_00550250[8];

extern char D_00550258[8];

extern char D_00550260[8];

extern char D_00550268[8];

extern char D_00550270[8];

extern char D_00550278[8];

extern char D_00550280[8];

extern char D_00550288[8];

extern char D_00550290[8];

extern char D_00550298[8];

extern char D_005502A0[8];

extern char D_005502A8[8];

extern char D_005502B0[8];

extern char D_005502B8[8];

extern char D_005502C0[8];

extern char D_005502C8[8];

extern char D_005502D0[8];

extern char D_005502D8[8];

extern char D_005502E0[8];

extern char D_005502E8[8];

extern char D_005502F0[8];

extern char D_005502F8[8];

extern char D_00550300[8];

extern char D_00550308[8];

extern char D_00550310[8];

extern char D_00550318[8];

extern char D_00550320[8];

extern char D_00550328[8];

extern char D_00550330[8];

extern char D_00550338[8];

extern char D_00550340[8];

extern char D_00550348[8];

extern char D_00550350[8];

extern char D_00550358[8];

extern char D_00550360[8];

extern char D_00550368[8];

extern char D_00550370[8];

extern char D_00550378[8];

extern char D_00550380[8];

extern char D_00550388[8];

extern char D_00550390[8];

extern char D_00550398[8];

extern char D_005503A0[8];

extern char D_005503A8[8];

extern char D_005503B0[8];

extern char D_005503B8[8];

extern char D_005503C0[8];

extern char D_005503C8[8];

extern char D_005503D0[8];

extern char D_005503D8[8];

extern char D_005503E0[8];

extern char D_005503E8[8];

extern char D_005503F0[8];

extern char D_005503F8[8];

extern char D_00550400[8];

extern char D_00550408[8];

extern char D_00550410[8];

extern char D_00550418[8];

extern char D_00550420[8];

extern char D_00550428[8];

extern char D_00550430[8];

extern char D_00550438[8];

extern char D_00550440[8];

extern char D_00550448[8];

extern char D_00550450[8];

extern char D_00550458[8];

extern char D_00550460[8];

extern char D_00550468[8];

extern char D_00550470[8];

extern char D_00550478[8];

extern char D_00550480[8];

extern char D_00550488[8];

extern char D_00550490[8];

extern char D_00550498[8];

extern char D_005504A0[8];

extern char D_005504A8[8];

extern char D_005504B0[8];

extern char D_005504B8[8];

extern char D_005504C0[8];

extern char D_005504C8[8];

extern char D_005504D0[8];

extern char D_005504D8[8];

extern char D_005504E0[8];

extern char D_005504E8[8];

extern char D_005504F0[8];

extern char D_005504F8[8];

extern char D_00550500[8];

extern char D_00550508[8];

extern char D_00550510[8];

extern char D_00550518[8];

extern char D_00550520[8];

extern char D_00550528[8];

extern char D_00550530[8];

extern char D_00550538[8];

extern char D_00550540[8];

extern char D_00550548[8];

extern char D_00550550[8];

extern char D_00550558[8];

extern char D_00550560[8];

extern char D_00550568[8];

extern char D_00550570[8];

extern char D_00550578[8];

extern char D_00550580[8];

extern char D_00550588[8];

extern char D_00550590[8];

extern char D_00550598[8];

extern char D_005505A0[8];

extern char D_005505A8[8];

extern char D_005505B0[8];

extern char D_005505B8[8];

extern char D_005505C0[8];

extern char D_005505C8[8];

extern char D_005505D0[8];

extern char D_005505D8[8];

extern char D_005505E0[8];

extern char D_005505E8[8];

extern char D_005505F0[8];

extern char D_005505F8[8];

extern char D_00550600[8];

extern char D_00550608[8];

extern char D_00550610[8];

extern char D_00550618[8];

extern char D_00550620[8];

extern char D_00550628[8];

extern char D_00550630[8];

extern char D_00550638[8];

extern char D_00550640[8];

extern char D_00550648[8];

extern char D_00550650[8];

extern char D_00550658[8];

extern char D_00550660[8];

extern char D_00550668[8];

extern char D_00550670[8];

extern char D_00550678[8];

extern char D_00550680[8];

extern char D_00550688[8];

extern char D_00550690[8];

extern char D_00550698[8];

extern char D_005506A0[8];

extern char D_005506A8[8];

extern char D_005506B0[8];

extern char D_005506B8[8];

extern char D_005506C0[8];

extern char D_005506C8[8];

extern char D_005506D0[8];

extern char D_005506D8[8];

extern char D_005506E0[8];

extern char D_005506E8[8];

extern char D_005506F0[8];

extern char D_005506F8[8];

extern char D_00550700[8];

extern char D_00550708[8];

extern char D_00550710[8];

extern char D_00550718[8];

extern char D_00550720[8];

extern char D_00550728[8];

extern char D_00550730[8];

extern char D_00550738[8];

extern char D_00550740[8];

extern char D_00550748[8];

extern char D_00550750[8];

extern char D_00550758[8];

extern char D_00550760[8];

extern char D_00550768[8];

extern char D_00550770[8];

extern char D_00550778[8];

extern char D_00550780[8];

extern char D_00550788[8];

extern char D_00550790[8];

extern char D_00550798[8];

extern char D_005507A0[8];

extern char D_005507A8[8];

extern char D_005507B0[8];

extern char D_005507B8[8];

extern char D_005507C0[8];

extern char D_005507C8[8];

extern char D_005507D0[8];

extern char D_005507D8[8];

extern char D_005507E0[8];

extern char D_005507E8[8];

extern char D_005507F0[8];

extern char D_005507F8[8];

extern char D_00550800[8];

extern char D_00550808[8];

extern char D_00550810[8];

extern char D_00550818[8];

extern char D_00550820[8];

extern char D_00550828[8];

extern char D_00550830[8];

extern char D_00550838[8];

extern char D_00550840[8];

extern char D_00550848[8];

extern char D_00550850[8];

extern char D_00550858[8];

extern char D_00550860[8];

extern char D_00550868[8];

extern char D_00550870[8];

extern char D_00550878[8];

extern char D_00550880[8];

extern char D_00550888[8];

extern char D_00550890[8];

extern char D_00550898[8];

extern char D_005508A0[8];

extern char D_005508A8[8];

extern char D_005508B0[8];

extern char D_005508B8[8];

extern char D_005508C0[8];

extern char D_005508C8[8];

extern char D_005508D0[8];

extern char D_005508D8[8];

extern char D_005508E0[8];

extern char D_005508E8[8];

extern char D_005508F0[8];

extern char D_005508F8[8];

extern char D_00550900[8];

extern char D_00550908[8];

extern char D_00550910[8];

extern char D_00550918[8];

extern char D_00550920[8];

extern char D_00550928[8];

extern char D_00550930[8];

extern char D_00550938[8];

extern char D_00550940[8];

extern char D_00550948[8];

extern char D_00550950[8];

extern char D_00550958[8];

extern char D_00550960[8];

extern char D_00550968[8];

extern char D_00550970[8];

extern char D_00550978[8];

extern char D_00550980[8];

extern char D_00550988[8];

extern char D_00550990[8];

extern char D_00550998[8];

extern char D_005509A0[8];

extern char D_005509A8[8];

extern char D_005509B0[8];

extern char D_005509B8[8];

extern char D_005509C0[8];

extern char D_005509C8[8];

extern char D_005509D0[8];

extern char D_005509D8[8];

extern char D_005509E0[8];

extern char D_005509E8[8];

extern char D_005509F0[8];

extern char D_005509F8[8];

extern char D_00550A00[8];

extern char D_00550A08[8];

extern char D_00550A10[8];

extern char D_00550A18[8];

extern char D_00550A20[8];

extern char D_00550A28[8];

extern char D_00550A30[8];

extern char D_00550A38[8];

extern char D_00550A40[8];

extern char D_00550A48[8];

extern char D_00550A50[8];

extern char D_00550A58[8];

extern char D_00550A60[8];

extern char D_00550A68[8];

extern char D_00550A70[8];

extern char D_00550A78[8];

extern char D_00550A80[8];

extern char D_00550A88[8];

extern char D_00550A90[8];

extern char D_00550A98[8];

extern char D_00550AA0[8];

extern char D_00550AA8[8];

extern char D_00550AB0[8];

extern char D_00550AB8[8];

extern char D_00550AC0[8];

extern char D_00550AC8[8];

extern char D_00550AD0[8];

extern char D_00550AD8[8];

extern char D_00550AE0[8];

extern char D_00550AE8[8];

extern char D_00550AF0[8];

extern char D_00550AF8[8];

extern char D_00550B00[8];

extern char D_00550B08[8];

extern char D_00550B10[8];

extern char D_00550B18[8];

extern char D_00550B20[8];

extern char D_00550B28[8];

extern char D_00550B30[8];

extern char D_00550B38[8];

extern char D_00550B40[8];

extern char D_00550B48[8];

extern char D_00550B50[8];

extern char D_00550B58[8];

extern char D_00550B60[8];

extern char D_00550B68[8];

extern char D_00550B70[8];

extern char D_00550B78[8];

extern char D_00550B80[8];

extern char D_00550B88[8];

extern char D_00550B90[8];

extern char D_00550B98[8];

extern char D_00550BA0[8];

extern char D_00550BA8[8];

extern char D_00550BB0[8];

extern char D_00550BB8[8];

extern char D_00550BC0[8];

extern char D_00550BC8[8];

extern char D_00550BD0[8];

extern char D_00550BD8[8];

extern char D_00550BE0[8];

extern char D_00550BE8[8];

extern char D_00550BF0[8];

extern char D_00550BF8[8];

extern char D_00550C00[8];

extern char D_00550C08[8];

extern char D_00550C10[8];

extern char D_00550C18[8];

extern char D_00550C20[8];

extern char D_00550C28[8];

extern char D_00550C30[8];

extern char D_00550C38[8];

extern char D_00550C40[8];

extern char D_00550C48[8];

extern char D_00550C50[8];

extern char D_00550C58[8];

extern char D_00550C60[8];

extern char D_00550C68[8];

extern char D_00550C70[8];

extern char D_00550C78[8];

extern char D_00550C80[8];

extern char D_00550C88[8];

extern char D_00550C90[8];

extern char D_00550C98[8];

extern char D_00550CA0[8];

extern char D_00550CA8[8];

extern char D_00550CB0[8];

extern char D_00550CB8[8];

extern char D_00550CC0[8];

extern char D_00550CC8[8];

extern char D_00550CD0[8];

extern char D_00550CD8[8];

extern char D_00550CE0[8];

extern char D_00550CE8[8];

extern char D_00550CF0[8];

extern char D_00550CF8[8];

extern char D_00550D00[8];

extern char D_00550D08[8];

extern char D_00550D10[8];

extern char D_00550D18[8];

extern char D_00550D20[8];

extern char D_00550D28[8];

extern char D_00550D30[8];

extern char D_00550D38[8];

extern char D_00550D40[8];

extern char D_00550D48[8];

extern char D_00550D50[8];

extern char D_00550D58[8];

extern char D_00550D60[8];

extern char D_00550D68[8];

extern char D_00550D70[8];

extern char D_00550D78[8];

extern char D_00550D80[8];

extern char D_00550D88[8];

extern char D_00550D90[8];

extern char D_00550D98[8];

extern char D_00550DA0[8];

extern char D_00550DA8[8];

extern char D_00550DB0[8];

extern char D_00550DB8[8];

extern char D_00550DC0[8];

extern char D_00550DC8[8];

extern char D_00550DD0[8];

extern char D_00550DD8[8];

extern char D_00550DE0[8];

extern char D_00550DE8[8];

extern char D_00550DF0[8];

extern char D_00550DF8[8];

extern char D_00550E00[8];

extern char D_00550E08[8];

extern char D_00550E10[8];

extern char D_00550E18[8];

extern char D_00550E20[8];

extern char D_00550E28[8];

extern char D_00550E30[8];

extern char D_00550E38[8];

extern char D_00550E40[8];

extern char D_00550E48[8];

extern char D_00550E50[8];

extern char D_00550E58[8];

extern char D_00550E60[8];

extern char D_00550E68[8];

extern char D_00550E70[8];

extern char D_00550E78[8];

extern char D_00550E80[8];

extern char D_00550E88[8];

extern char D_00550E90[8];

extern char D_00550E98[8];

extern char D_00550EA0[8];

extern char D_00550EA8[8];
/* End of declarations from src/smt3/code_0022C578.c */

/* Declarations from dds-decomp src/dds1/game/code_002BF790.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_campCrossPanel_t 1

#define SMT3_DEFINES_campDhEntryWork_t 1

#define SMT3_DEFINES_campDhEntry_t 1

#define SMT3_DEFINES_campDhIconGauge_t 1

#define SMT3_DEFINES_campDhSavedUnit_t 1

#define SMT3_DEFINES_campDhTimerWork_t 1

#define SMT3_DEFINES_campDhWork_t 1

#define SMT3_DEFINES_campInfoWork_t 1

#define SMT3_DEFINES_campStatusPanel_t 1

#define SMT3_DEFINES_campWork_t 1

#define SMT3_DEFINES_cmbGbWork_t 1

#define SMT3_DEFINES_cmbWork_t 1

#define SMT3_DEFINES_cmpBadEffectParams_t 1

#define SMT3_DEFINES_cmpCursorExtended_t 1

#define SMT3_DEFINES_cmpDHWindow_t 1

#define SMT3_DEFINES_cmpDhWork_t 1

#define SMT3_DEFINES_cmpDrawBank_t 1

#define SMT3_DEFINES_cmpHeartsIconTableEntry_t 1

#define SMT3_DEFINES_cmpInfoWindow_t 1

#define SMT3_DEFINES_cmpListPage_t 1

#define SMT3_DEFINES_cmpLocalStock_t 1

#define SMT3_DEFINES_cmpModelLink_t 1

#define SMT3_DEFINES_cmpModelParam_t 1

#define SMT3_DEFINES_cmpMsgEntry_t 1

#define SMT3_DEFINES_cmpPanelEffAnim_t 1

#define SMT3_DEFINES_cmpPanelEffState_t 1

#define SMT3_DEFINES_cmpPanelEffView_t 1

#define SMT3_DEFINES_cmpPanelEff_t 1

#define SMT3_DEFINES_cmpPrintControl_t 1

#define SMT3_DEFINES_cmpSavedNameView_t 1

#define SMT3_DEFINES_cmpSeqFunc_t 1

#define SMT3_DEFINES_cmpSeqInfo_t 1

#define SMT3_DEFINES_cmpStockSeqInfo_t 1

#define SMT3_DEFINES_cmpStockWork_t 1

#define SMT3_DEFINES_cmpVec4_t 1

#define SMT3_DEFINES_datItemEntry_t 1

#define SMT3_DEFINES_datSkillBadEntry_t 1

#define SMT3_DEFINES_datSkillBadWindow_t 1

#define SMT3_DEFINES_frFontHandle_t 1

#define SMT3_DEFINES_frFontRequest_t 1

#define SMT3_DEFINES_rcvWork_t 1

#define SMT3_DEFINES_stfUiWork_t 1

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/sdk_libc.h"

void *memset(void *dst, s32 value, u32 size);

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

/* Append standard packets to a DMA packet list. A null allocator selects the
   default packet allocator. Asset packets reference the selected draw entry. */
void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                              s32 (*allocatePacket)(s32));

/* Build the serialized VIF attribute stream and return its word cursor. */
u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

#include "dds/sdk_ps2.h"

#include "dds/sdf_gs_header.h"

#include "dds/sdf_gs_alpha_register_api.h"

#include "dds/sdf_gs_register_submit_api.h"

#include "dds/common.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_frame_draw_buffer.h"

#include "dds/sdf_cons_matrix_packet.h"

#include "dds/sdf_dma_packet_header.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_header.h"

#include "dds/fpu.h"

#include "dds/eff.h"

#include "dds/eff_slot_phase.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_records.h"

#include "dds/itf_grid_text.h"

#include "dds/itf_draw_grid.h"

#include "dds/sdf.h"

#include "dds/sdf_draw.h"

#include "dds/sdf_projection.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/sdf_packet_append.h"

#include "dds/grid_packed_vertex.h"

#include "dds/itf_grid_status_records.h"

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);

extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, void *);

/* Four words filled together; their corner/channel interpretation is unknown. */
typedef struct UiQuadWords {
    u32 unk00[4];
} UiQuadWords; // 0x10

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

#include "dds/sdf_packet_alloc.h"

extern void effDrawSpriteWithWorkCornerColors(s32 x, s32 y, s32 z, s32 flags, EffectSlotSet *set, s32 slot, BdWork *entry, s32 layer);

/* Resolve the indexed render entry before applying position, depth, and draw flags. */
void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z, s32 drawFlags, EffectSlotSet *resource, s32 resourceIndex, s32 surfaceIndex);

/* Update the indexed slot's description countdown and carry its active state forward. */
void itfUpdateGridSlotDescription(EffectSlotSet *resource, s32 resourceIndex);

/* Resolve an entry by key, falling back to the object's stored value. */
s32 itfGridLookupValueOrDefault(EffectSlotSet *resource, s32 resourceIndex);

void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height);

/* Store pixel bounds quantized to the widget's 16x8 grid, then copy all four words. */
void itfGridSetQuantizedBounds(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y,
                   s32 width, s32 height);

void itfGridSetBounds(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height);

/* Restore four saved palette words in the native packed work-record buffer. */
void itfGridCopyEntryQuad(s32 resource, s32 entryIndex);

/* Cache the actual resource owner and its signed render-slot index. */
void itfGridStorePosition(MenuGridSlot *slot, EffectSlotSet *resource, s32 resourceIndex);

EffSlotPhaseStep itfGridApplySqrtBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

/* Apply the ZOOM_01 easing to the adjustment bounds and fade their alpha. */
EffSlotPhaseStep itfGridApplyQuadraticZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

/* Shrink the grid bounds and apply the three-phase flush envelope to corner alpha. */
EffSlotPhaseStep itfGridApplyTimedBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

EffSlotPhaseStep itfGridApplyThresholdBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

EffSlotPhaseStep itfUpdateAngleAndGetCycleStep(BdWork *unused, BdWork *out, EffTimedState *timedState);

/* Blend each saved corner color toward the table's target color. */
EffSlotPhaseStep itfGridInterpolateCornerColorsToTarget(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

/* Apply linear ZOOM easing to the adjustment bounds and fade their alpha. */
EffSlotPhaseStep itfGridApplyLinearZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

EffSlotPhaseStep itfGridApplyLinearBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

extern sdfDrawTag_t D_003DFAB0[2];

extern DmaPacketHeader D_003DFAF0[2];

extern sdfDrawTag_t D_003DFB30[2];

extern sdf3DLightParallelPacket_t D_003DFB70[2];

extern ConsMatrixPacket D_003DFD30[2];

extern sdf3DLight_t *D_00324770[3];

extern f32 kwlnDefaultColorVector[4];

/* Rebuild the view matrix and both frame banks' matrix and lighting packet lists. */
void itfInitDoubleBufferedScenePackets(void);

void itfSetPrimaryFramebufferAlphaFlag(u8 value, u32 kind);

void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind);

void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind);

/* Fill all four words with value without assigning a corner or channel order. */
void uiFillQuadColorWords(UiQuadWords *quad, u32 colorWord);

extern void itfDrawColoredRectangle(u32, u32, u32, u32, u32, const u32 *, u32, u32);

void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);

void itfDrawColoredRectangle(u32 x, u32 y, u32 z, u32 width, u32 height, const u32 *colors, u32 gsContext, u32 surfaceIndex);

void uiDrawGradientColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, const u32 *vertexColors, u32 surfaceIndex);

/* Draw four frame edges; the bottom edge extends 16 units beyond the right side. */
void uiDrawFrameEdges(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 context);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

void uiDrawActiveSurfaceRegion(s32 surfaceIndex);

void sdfDispatchSurfaceWithPreparedTexturePacket(s32 surfaceIndex);

void uiDrawTexturedSurfaceAtFarDepth(s32 surface);
/* End of declarations from src/dds1/game/code_002BF790.c */

/* Resolve the indexed render entry before applying position, depth, and draw flags. */
void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z, s32 drawFlags, EffectSlotSet *resource, s32 resourceIndex, s32 surfaceIndex) {
    void *renderEntry = effGetSlotWorkOrOverride(resource, resourceIndex);
    effDrawSpriteWithWorkCornerColors(offsetX, offsetY, z, drawFlags, resource, resourceIndex, (BdWork *)renderEntry,
                  surfaceIndex);
}

/* Update the indexed slot's description countdown and carry its active state forward. */
void itfUpdateGridSlotDescription(EffectSlotSet *resource, s32 resourceIndex) {
    BdWork *base = &resource->workEntries[resourceIndex];
    const u32 timedByteOffset = (resourceIndex + base->slotOffset) * sizeof(BdWork);
    BdWork *timed = (BdWork *)(timedByteOffset + (u32)resource->workEntries);
    BdWork *previous = (BdWork *)effGetSlotWorkOrOverride(resource, resourceIndex + base->slotOffset);
    BdWork *next;
    u32 advance = 0;

    if (timed->remainingDescriptionUpdates == 0) {
        timed->remainingDescriptionUpdates =
            resource->descriptions[resourceIndex + base->slotOffset].descriptionUpdateDelay;
        if ((u32)(resourceIndex + timed->slotOffset + 1) < resource->count) {
            const s32 nextFlags = resource->descriptions[resourceIndex + base->slotOffset + 1].flags &
                                  EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION;
            const u32 shouldAdvance = nextFlags > 0;
            advance = shouldAdvance;
        }
        if (advance == 1) {
            base->slotOffset++;
        } else {
            if (base->slotOffset == 0) {
                return;
            }
            base->slotOffset = 0;
        }
        effInitializeSlotWorkFromDescription(resource, resourceIndex + base->slotOffset, previous);
        next = (BdWork *)effGetSlotWorkOrOverride(resource, resourceIndex + base->slotOffset);
        next->states[0].flags = previous->states[0].flags;
        next->states[0].source = previous->states[0].source;
        next->states[0].value = previous->states[0].value;
    } else {
        timed->remainingDescriptionUpdates--;
    }
}

/* Resolve an entry by key, falling back to the object's stored value. */
s32 itfGridLookupValueOrDefault(EffectSlotSet *resource, s32 resourceIndex) {
    BdWork *entry = (BdWork *)effGetSlotWorkOrOverride(resource, resourceIndex);
    s32 result;

    if (entry->states[0].delay == 0) {
        itfUpdateGridSlotDescription(resource, resourceIndex);
    }
    result = (s32)effUpdateTimedStates(resource, (u32)resourceIndex, entry);
    if (result == 0) {
        result = resource->defaultValue;
    }
    return result;
}

void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height) {
    EffectSlotDescription *entry = &resource->descriptions[resourceIndex];
    void *record = effGetSlotWorkOrOverride(resource, resourceIndex);

    entry->bounds[0] = x >> 4;
    entry->bounds[1] = y >> 3;
    entry->bounds[2] = width >> 4;
    entry->bounds[3] = height >> 3;
    effInitializeSlotWorkFromDescription(resource, resourceIndex, record);
}

INCLUDE_ASM(const s32, "code_002CAFD8", itfGridSetQuantizedBounds);

INCLUDE_ASM(const s32, "code_002CAFD8", itfGridSetBounds);

INCLUDE_ASM(const s32, "code_002CAFD8", itfGridCopyEntryQuad);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CB3E0);

EffSlotPhaseStep itfGridApplySqrtBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)timedState->source->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 scaled = (s32)(fsqrtf((f32)delta) * (f32)timedState->value * (1.0f / 65536.0f));
        if (delta > 0) {
            dimensions[i] = delta - scaled * scaled;
        } else {
            dimensions[i] = scaled * scaled + delta;
        }
    }
    if (table->mirrored != 0) {
        factor = EFF_SLOT_PHASE_ONE - timedState->value;
    } else {
        factor = timedState->value;
    }
    sourceColor = rectangle->savedColors;
    destColor = geometry->cornerColors;
    for (i = 0; i < 4; i++, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        *destColor = (color & ~0xFF) | (alpha * factor / EFF_SLOT_PHASE_ONE);
    }
    return EFF_SLOT_PHASE_ONE / table->divisor;
}

/* Apply the ZOOM_01 easing to the adjustment bounds and fade their alpha. */
EffSlotPhaseStep itfGridApplyQuadraticZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 squares[2];
    s32 deltas[2];
    s32 previous[2];
    s32 scaledWidth;
    s32 scaledHeight;
    s32 widthAdjustment;
    s32 heightAdjustment;
    u32 *sourceColor;
    u32 *destColor;
    s32 colorMask;
    s32 fractionalMask;
    s32 i;

    table = (GridAngleTable *)timedState->source->status;
    colorMask = -0x100;
    fractionalMask = 0xFFFF;
    deltas[0] = table->divisor << 4;
    deltas[1] = (((table->mirrored << 12) / 640) * rectangle->sourceHeight) / rectangle->sourceWidth;
    previous[0] = out->geometry.bounds[0];
    previous[1] = out->geometry.bounds[1];
    scaledWidth = (s32)(fsqrtf((f32)deltas[0]) * (f32)timedState->value * (1.0f / 65536.0f));
    squares[0] = scaledWidth * scaledWidth;
    widthAdjustment = -((deltas[0] - squares[0]) / 2);
    scaledHeight = (s32)(fsqrtf((f32)deltas[1]) * (f32)timedState->value * (1.0f / 65536.0f));
    squares[1] = scaledHeight * scaledHeight;
    heightAdjustment = -((deltas[1] - squares[1]) / 2);

    out->geometry.bounds[0] = widthAdjustment;
    out->geometry.bounds[2] += (previous[0] - widthAdjustment) * 2;
    out->geometry.bounds[1] = heightAdjustment;
    out->geometry.bounds[3] += (previous[1] - heightAdjustment) * 2;

    sourceColor = rectangle->savedColors;
    destColor = out->geometry.cornerColors;
    i = 3;

    for (; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        s32 product = alpha * timedState->value;
        s32 negative = 0;

        if (product < 0) {
            negative++;
        }
        *destColor = (color & colorMask) | ((product + negative * fractionalMask) >> 16);
    }
    return EFF_SLOT_PHASE_ONE / table->cycleDivisor;
}

/* Shrink the grid bounds and apply the three-phase flush envelope to corner alpha. */
EffSlotPhaseStep itfGridApplyTimedBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridFlushTable *table;
    s32 phases[3];
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensionOut;
    s32 totalDuration;
    s32 numerator;
    s32 denominator;
    u32 *sourceColor;
    u32 *destColor;
    s32 i = 0;

    table = (GridFlushTable *)timedState->source->status;
    totalDuration = table->inDuration;
    totalDuration += table->outDuration;
    totalDuration += table->waitDuration;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    dimensionOut = geometry->bounds;
    {
        s32 fractionalMask = 0xFFFF;

        for (; i < 2; i++) {
            s32 delta = deltas[i];
            s32 magnitude = delta < 0 ? -delta : delta;
            s32 product = magnitude * timedState->value;
            s32 negative = 0;
            s32 scaled;

            if (product < 0) {
                negative++;
            }
            /* Signed fixed-point division rounds toward zero. */
            scaled = (product + negative * fractionalMask) >> 16;
            if (delta > 0) {
                dimensionOut[i] = delta - scaled;
            } else {
                dimensionOut[i] = delta + scaled;
            }
        }
    }

    phases[0] = (table->inDuration << 16) / totalDuration;
    phases[1] = (table->waitDuration << 16) / totalDuration;
    phases[2] = (table->outDuration << 16) / totalDuration;

    if (phases[1] + phases[2] < timedState->value) {
        numerator = phases[0] - (timedState->value - (phases[1] + phases[2]));
        denominator = phases[0];
    } else if (phases[2] < timedState->value) {
        destColor = geometry->cornerColors;
        sourceColor = rectangle->savedColors;
        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            *destColor = *sourceColor;
        }
        return EFF_SLOT_PHASE_ONE / totalDuration;
    } else {
        numerator = timedState->value;
        denominator = phases[2];
    }

    destColor = geometry->cornerColors;
    sourceColor = rectangle->savedColors;
    {
        s32 colorMask = -0x100;

        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            u32 color = *sourceColor;
            s32 lowByte = *(u8 *)sourceColor;

            *destColor = (color & colorMask) | (lowByte * numerator / denominator);
        }
    }
    return EFF_SLOT_PHASE_ONE / totalDuration;
}

EffSlotPhaseStep itfGridApplyThresholdBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)timedState->source->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 magnitude = delta < 0 ? -delta : delta;
        s32 amount = magnitude * timedState->value / EFF_SLOT_PHASE_ONE;
        if (delta > 0) {
            dimensions[i] = delta - amount;
        } else {
            dimensions[i] = delta + amount;
        }
    }
    factor = ((100 - table->mirrored) << 16) / 100;
    destColor = geometry->cornerColors;
    sourceColor = rectangle->savedColors;
    for (i = 0; i < 4; i++, destColor++, sourceColor++) {
        if (timedState->value < factor) {
            u32 color = *sourceColor;
            s32 alpha = *(u8 *)sourceColor;
            *destColor = (color & ~0xFF) | (alpha * timedState->value / factor);
        } else {
            *destColor = *sourceColor;
        }
    }
    return EFF_SLOT_PHASE_ONE / table->divisor;
}

INCLUDE_ASM(const s32, "code_002CAFD8", itfUpdateAngleAndGetCycleStep);

/* Blend each saved corner color toward the table's target color. */
EffSlotPhaseStep itfGridInterpolateCornerColorsToTarget(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridColorTargetTable *table = (GridColorTargetTable *)timedState->source->status;
    u32 *destColor = out->geometry.cornerColors;
    u32 *sourceColor = rectangle->savedColors;
    s32 i;

    for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = (color & 0xFF000000) >> 24;
        s32 blue = (color & 0xFF0000) >> 16;
        s32 green = (color & 0xFF00) >> 8;
        s32 red = *(u8 *)sourceColor;
        s32 alphaTarget = table->alpha;

        blue = (blue + (table->blue - blue) * timedState->value / EFF_SLOT_PHASE_ONE) & 0xFF;
        green = (green + (table->green - green) * timedState->value / EFF_SLOT_PHASE_ONE) & 0xFF;
        red = (red + (table->red - red) * timedState->value / EFF_SLOT_PHASE_ONE) & 0xFF;
        *destColor = ((alpha + (alphaTarget - alpha) * timedState->value / EFF_SLOT_PHASE_ONE) << 24) | (blue << 16)
            | (green << 8) | red;
    }
    return EFF_SLOT_PHASE_ONE / table->divisor;
}

/* Apply linear ZOOM easing to the adjustment bounds and fade their alpha. */
EffSlotPhaseStep itfGridApplyLinearZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 scaled[2];
    s32 deltas[2];
    s32 previous[2];
    s32 scaledWidth;
    s32 scaledHeight;
    s32 widthAdjustment;
    s32 heightAdjustment;
    s32 angle;
    u32 *sourceColor;
    u32 *destColor;
    s32 colorMask;
    s32 fractionalMask;
    s32 i;

    table = (GridAngleTable *)timedState->source->status;
    colorMask = -0x100;
    fractionalMask = 0xFFFF;
    deltas[0] = table->divisor << 4;
    deltas[1] = (((table->mirrored << 12) / 640) * rectangle->sourceHeight) / rectangle->sourceWidth;
    angle = timedState->value;
    previous[0] = out->geometry.bounds[0];
    previous[1] = out->geometry.bounds[1];
    scaledWidth = deltas[0] * angle;
    if (scaledWidth < 0) {
        scaledWidth += 0xFFFF;
    }
    scaled[0] = scaledWidth >> 16;
    widthAdjustment = -((deltas[0] - scaled[0]) / 2);
    out->geometry.bounds[0] = widthAdjustment;
    out->geometry.bounds[2] += (previous[0] - widthAdjustment) * 2;

    scaledHeight = deltas[1] * angle;
    if (scaledHeight < 0) {
        scaledHeight += 0xFFFF;
    }
    scaled[1] = scaledHeight >> 16;
    heightAdjustment = -((deltas[1] - scaled[1]) / 2);
    out->geometry.bounds[1] = heightAdjustment;
    out->geometry.bounds[3] += (previous[1] - heightAdjustment) * 2;

    sourceColor = rectangle->savedColors;
    destColor = out->geometry.cornerColors;
    i = 3;
    for (; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        s32 colorProduct = alpha * timedState->value;
        s32 negative = 0;

        if (colorProduct < 0) {
            negative++;
        }
        *destColor = (color & colorMask) | ((colorProduct + negative * fractionalMask) >> 16);
    }
    return EFF_SLOT_PHASE_ONE / table->cycleDivisor;
}

EffSlotPhaseStep itfGridApplyLinearBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)timedState->source->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 magnitude = delta < 0 ? -delta : delta;
        s32 amount = magnitude * timedState->value / EFF_SLOT_PHASE_ONE;
        if (delta > 0) {
            dimensions[i] = delta - amount;
        } else {
            dimensions[i] = delta + amount;
        }
    }
    if (table->mirrored != 0) {
        factor = EFF_SLOT_PHASE_ONE - timedState->value;
    } else {
        factor = timedState->value;
    }
    sourceColor = rectangle->savedColors;
    destColor = geometry->cornerColors;
    for (i = 0; i < 4; i++, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        *destColor = (color & ~0xFF) | (alpha * factor / EFF_SLOT_PHASE_ONE);
    }
    return EFF_SLOT_PHASE_ONE / table->divisor;
}

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CBE38);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC0E0);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC230);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC358);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC3F0);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC558);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC630);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC668);

void cmpDrawSpriteFlag(u8 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id != 0;
    if (flag == 0) {
        prim[1] = 0x4A;
    } else {
        prim[1] = 0x4B;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC898);

void cmpDrawSprite(s32 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id;
    if (flag == 0) {
        prim[1] = 0x47;
    } else {
        prim[1] = 0x48;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CC990);

void cmpDrawSprite2(s32 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id;
    if (flag == 0) {
        prim[1] = 0x42;
    } else {
        prim[1] = 0x43;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CCA88);

void cmpDrawSpriteIndexed(s32 id, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[1] = 0x49;
    prim[0] = id;
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpDrawSpriteIndexed2(s32 id, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[1] = 0x14;
    prim[0] = id;
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CCC18);

void uiDrawUniformRgbRange(const u32 *xs, const u32 *ys, u32 z, u32 color, u32 ot)
{
    u32 colors[3];

    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    cmpDrawTriG(xs, ys, z, colors, ot);
}

void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot)
{
    void *packet;
    struct { u64 colorChannels[2]; u64 packedXY; u64 depth; } *prim;
    sdfDrawTag_t *tag;
    sdfGrOt_t *otp;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(6, 1)));
    sdf2DMakePrimPacketHeader(packet, 0x14B, 6, 0x515151, 1);
    prim = sdf2DGetPrimPtr(packet);
    cmpExpandColor(prim[0].colorChannels, colors[0]);
    prim[0].packedXY = (xs[0] + 0x7000) | ((u64)(ys[0] + 0x7900) << 32);
    prim[0].depth = z;
    cmpExpandColor(prim[1].colorChannels, colors[1]);
    prim[1].packedXY = (xs[1] + 0x7000) | ((u64)(ys[1] + 0x7900) << 32);
    prim[1].depth = z;
    cmpExpandColor(prim[2].colorChannels, colors[2]);
    prim[2].packedXY = (xs[2] + 0x7000) | ((u64)(ys[2] + 0x7900) << 32);
    prim[2].depth = z;
    tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t)));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h)
{
    s32 colors[4];

    colors[3] = color;
    colors[1] = color;
    colors[2] = color;
    colors[0] = color;
    cmpDrawBoxG(a, b, c, d, e, colors, g, h);
}

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CCE18);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpDrawBoxG);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CCFC8);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CCFE8);

void cmpDrawPanelEffCell(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h)
{
    u32 pair[2];

    pair[1] = g;
    pair[0] = g;
    cmpDrawBoxLine(a, b, c, d, e, f, pair, h);
}

void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                    const u32 *colors, s32 surfaceIndex)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    void *packet;
    u64 *prim;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(4, 1)));
    sdf2DMakePrimPacketHeader(packet, 0x149, 4, 0x5151, 1);
    prim = sdf2DGetPrimPtr(packet);
    cmpExpandColor(prim, colors[0]);
    prim[2] = (x0 + 0x7000) | ((u64)(y0 + 0x7900) << 32);
    prim[3] = z0;
    cmpExpandColor(prim + 4, colors[1]);
    prim[6] = (x1 + 0x7000) | ((u64)(y1 + 0x7900) << 32);
    prim[7] = z1;
    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, EE_ADDR(packet));
    surface = &sdfGrOtTable[surfaceIndex];
    surface->regFunc(surface, list);
}

void cmpDraw3DPoly(const f32 *first, const f32 *second, const f32 *third,
                   const u32 *colors, s32 unused, sdfMat_t *asset, s32 surfaceIndex)
{
    sdf3DPrimPolyParm_t request;
    u32 vertexColors[3];
    f32 textureCoordinates[3][2];
    sdfVector4_t positions[3] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    u32 firstColor, secondColor, thirdColor;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t)));
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    if (asset != NULL) {
        sdf3DPrimLinkMat(list, asset, 0);
    }
    memset(&request, 0, sizeof(request));
    firstColor = colors[0];
    secondColor = colors[1];
    thirdColor = colors[2];
    request.faceCount = 1;
    request.pointCount = 3;
    request.color = firstColor;
    request.vertex = positions;
    request.normal = NULL;
    request.uv = NULL;
    vertexColors[0] = firstColor;
    vertexColors[1] = secondColor;
    vertexColors[2] = thirdColor;
    request.vertexColor = vertexColors;
    SDF_VU0_COPYVECTOR(&positions[0], first);
    SDF_VU0_COPYVECTOR(&positions[1], second);
    SDF_VU0_COPYVECTOR(&positions[2], third);
    textureCoordinates[0][0] = 0.0f;
    textureCoordinates[0][1] = 0.0f;
    textureCoordinates[1][0] = 0.0f;
    textureCoordinates[1][1] = 0.0f;
    textureCoordinates[2][0] = 0.0f;
    textureCoordinates[2][1] = 0.0f;
    sdfDrawTagLink(list, EE_ADDR(sdf3DPrimMakePoly(&request)));
    surface = &sdfGrOtTable[surfaceIndex];
    surface->regFunc(surface, list);
}

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CD3C0);

INCLUDE_ASM(const s32, "code_002CAFD8", func_002CD460);

void uiDrawTexturedSurfaceAtFarDepth(s32 surface) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, surface);
    uiDrawActiveSurfaceRegion(surface);
}
