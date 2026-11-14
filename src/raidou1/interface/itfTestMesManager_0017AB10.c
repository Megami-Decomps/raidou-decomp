#include "common.h"

/* Declarations from smt3-decomp src/smt3/interface/itfTestMesManager.c */
#include "common.h"

#include "kernel.h"

#include "libsdf.h"

#include "scr.h"

#include "model.h"

#include "draw3d.h"

#include "ps2asm.h"

typedef struct { u8 unk00[8]; f32 degrees; u8 unk0C[4]; } pbFovWork_t;

extern pbFovWork_t pbDebugFovWork;

typedef struct { s32 x, y, z; u32 style; } pbPrintState_t;

typedef struct
{
    s16 editing, selection;
    u32 blink;
    u8 unk08[8];
} pbPanelEditor_t;

typedef struct { const char *title; s32 highlight; } pbColorRow_t;

typedef struct { u8 value; u8 unk01[3]; } pbColorByte_t;

typedef union
{
    struct { s32 red, green, blue, alpha; };
    s32 channels[4];
    pbColorByte_t channelBytes[4];
} pbColor_t;

extern pbPanelEditor_t pbDebugPanel2Pos, pbDebugPanel3X;

extern pbColorRow_t D_00326C20[6];

extern pbColor_t itfTestOverlayColor;

extern sdf3DScreenParam_t dds3ScreenParam;

extern const char *D_00326C08[6];

extern u8 dds3ScreenBlurFlag;

extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;

extern s32 dds3ScreenBlurOfsX[2];

extern s32 D_00551EA8, pbDebugPanel3State;

extern u8 D_0054F584;

extern void *dds3FontReqAllocDrawTag(void);

extern void *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t);

extern void sdfDevSetPrintCB(pbPrintState_t *, s32, s32, s32, s32);

extern void *sdfDevPrintfEx(void *, const char *, ...);

#include "sdk_libc.h"

extern f32 sdfSin(f32 aRadius);

s32 pbCalcFovDegrees(f32 *, f32, f32, f32, f32);

void pbDrawRotatedPrim(s32 otIndex);

extern char pbDebugBlurOnText[];

extern char pbDebugBlurOffText[];

extern char pbDebugPanel2CursorMark[];

extern char pbDebugPanel2RowFormat[];

extern char pbDebugBlurValueFormat[];

extern char pbDebugBlurOffsetFormat[];

extern char pbDebugPanel3Title[];

extern char pbDebugPanel3CursorMark[];

extern char pbDebugPanel3RowFormat[];

extern char pbDebugChannelFormat[];

extern char D_0054F5A8[];

typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

typedef struct
{
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t reg[2];
} pbRegPacket_t;

extern char *TestMsgReloadTextureData[];

extern s32 D_00326BD8[4];

extern s32 D_00326BE8[4];

extern s32 D_00326BF8[4];

extern u8 scrEmbedded_3267A8[];

extern sdfGrOt_t sdfGrOtTable[];

extern u8 dds3PadButtonState[];

extern u32 dds3BackColor;

extern s32 testMsgTexIndex;

extern void *testMsgTexture;

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern void itfMesMngSetKutiCallback(s32 id, void *func);

extern void itfDebugPrintMark(void);

extern void sdfTexFreeDelayed(void *aHandle);

extern void *itfMiscLoadTexture(char *filename);

extern void *sdfGrAllocPacketBuff(u32 aSize);

extern void *sdf2DMakeStdPacket(void *aPacket, u32 aLeng);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);

extern void itfMiscDrawSpritePrim(s32 *pos, s32 *uv, s32 *color, s32 z, void *texhandle, s32 context,
                                  sdfDrawTag_t *pdrawtag);

extern sdfVector4_t aViewEye, aViewTarget;

void TestMsgReloadTexture(void);

s32 TestMsgMngC_Process(dds3ProcessID_t *id);

s32 TestMsgMngD_Process(dds3ProcessID_t *id);

void itfDebugMesMngInitialize(void);

void TestMsgReloadTexture(void);

s32 TestMsgMngC_Process(dds3ProcessID_t *id);

s32 TestMsgMngD_Process(dds3ProcessID_t *id);

extern const char D_00535BD0[24];

extern const char D_00535BE8[24];

extern const char D_00535C00[24];

void itfDebugPrintMark(void);

s32 pbDebugPrintPanel(void);

s32 pbCalcFovDegrees(f32 *value, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep);

/* Draw the blur settings and handle selection, editing and cancellation. */
s32 pbDebugPrintPanel2(void);

/* Draw the four color channels and handle selection and value editing. */
s32 pbDebugPrintPanel3(void);

u64 *pbAllocPrimPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1);

void *pbMakeSpritePacket(u64 data, s32 context);

void *pbMakePrimPacket(u64 data, s32 context);

void pbDrawRotatedPrim(s32 surfaceIndex);

void func_001D2638(f32 *delta);

extern const sdfVector4Unaligned_t pbMyCharScale;

extern const char pbDrawMyCharacterProcName[24];

extern s32 testMsgTexIndex;

extern void *testMsgTexture;

extern char D_0054F518[8];

extern char D_0054F520[8];

extern char D_0054F528[8];

extern char pbDebugBlurOnText[8];

extern char pbDebugBlurOffText[8];

extern char pbDebugPanel2CursorMark[8];

extern char pbDebugPanel2RowFormat[8];

extern char pbDebugBlurValueFormat[8];

extern char pbDebugBlurOffsetFormat[8];

extern char D_0054F560[8];

extern char D_0054F568[8];

extern char D_0054F570[8];

extern char D_0054F578[8];

extern char D_0054F580[4];

extern u8 D_0054F584;

extern char pbDebugPanel3Title[8];

extern char pbDebugPanel3CursorMark[8];

extern char pbDebugPanel3RowFormat[8];

extern char pbDebugChannelFormat[8];

extern char D_0054F5A8[8];

extern s32 func_001D35F0(s16 x, s16 y, s32 mode);

extern u8 pbFindBlockSlot(s16 x, s16 y, s32 mode);

extern s32 (*pbMapCallbacks[2])(s16 x, s16 y, s32 mode);

extern s8 pbNeighborOffsets[4][2];

extern char pbProcNameFormat[4];

extern void *pbMapWork;

extern s32 D_0054F5C8;

extern s32 D_0054F5CC;

extern u8 pbMenuItemCount[2];

extern s32 D_0054F5D4;

extern u32 pbMoveCharList;

extern void *blockDB;

extern void *bmap;

extern void *goal;

extern void *record;

extern void *mapCursor;

extern s32 D_0054F5F0;

extern u8 degree_mode;

extern u8 pbGameKeyState;

extern f32 pbMapCursorWork;

extern char pbMCardInitFormat[8];

extern char pbMCardNewline[8];

extern void *g_pdebug_0;

extern char pbDebugMenuCursorMark[8];

extern char pbDebugFloatFormat[8];
/* End of declarations from src/smt3/interface/itfTestMesManager.c */

INCLUDE_ASM(const s32, "interface/itfTestMesManager_0017AB10", pbDebugPrintPanel);

s32 pbCalcFovDegrees(f32 *value, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep)
{
    f32 current = *value;
    s32 changed = 0;

    if ((s8)dds3PadButtonState[0x35] < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (dds3PadButtonState[0x35] & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if ((s8)dds3PadButtonState[0x34] < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (dds3PadButtonState[0x34] & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if ((s8)dds3PadButtonState[0x3A] < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (dds3PadButtonState[0x3A] & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if ((s8)dds3PadButtonState[0x38] < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (dds3PadButtonState[0x38] & 2) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
                changed = 1;
            }
        }
    }
    if (changed != 0) {
        *value = current;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "interface/itfTestMesManager_0017AB10", pbDebugPrintPanel2);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrEffectEdit);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrPointLight);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrFieldLight);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrFldFilter);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrCut23Fix);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrEnemyParty);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStr1Shot);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrCylinderF);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrCylinderC);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllHide);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrCamera);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrBgm);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAi);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrEnemyComm);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllSelect);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllSkill);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllDead);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllCrticl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllHit);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllSake);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllHansya);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrAllKyusyu);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrBad);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrPress);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrDamage);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrChange);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", pbStrDebugMenu);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTestTlp);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrAgOutEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTestP2A);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrMhbfdinEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTest3Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTest2Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTest1Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTest0Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrShoukanEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBhudainAEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBfInEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBigDamYEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrAgsword33Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrAgsword32Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrAgsword31Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrLightaEp);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrLightaEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrLightEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrAgiTobaEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrRevtinaEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTest02Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrMudshotEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTestEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrStopHelperPb);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrYuka02Fix02Pb);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrTestTmx);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrNoooEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrKaitenEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrLaunch00Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrAgiball01Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrAgiball00Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBRange00Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrFuDevilEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrFuRaidohEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrFuYukaEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrB12Hit03Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrB12Hit02Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrB12Hit01Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrB12Hit00Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBuhmain00Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBlood02Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBlood00Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrDamYuka2Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrDamYuka1Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrDamYukaEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrEffTesEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrKudanEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrMhmudoEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrConbAuraEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBad04Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrBad01Epl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrDamagMubEpl);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0017AB10", dds3StrDamagMuaEpl);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", pbDebugBlurOnText);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", pbDebugBlurOffText);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", pbDebugPanel2CursorMark);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", pbDebugPanel2RowFormat);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", pbDebugBlurValueFormat);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", pbDebugBlurOffsetFormat);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", itfOverlayColorEditorRowsEntry);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", itfOverlayColorEditorRowsEntry2);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", itfOverlayColorEditorRowsEntry3);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", itfOverlayColorEditorRowsEntry4);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0017AB10", itfOverlayColorEditorRowsEntry5);

