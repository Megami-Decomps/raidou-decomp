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

extern pbColor_t D_00326C50;

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

extern char *D_00326BC8[];

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

INCLUDE_ASM(const s32, "interface/itfTestMesManager_0018F738", pbDebugPrintPanel);

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

INCLUDE_ASM(const s32, "interface/itfTestMesManager_0018F738", func_0018F9D0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEB20);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEB30);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEB40);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEB50);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEB60);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEB70);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEB80);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEB90);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEBA0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEBB0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEBC0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEBD0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEBE0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEBF0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC00);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC10);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC20);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC30);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC40);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC50);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC60);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC70);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC80);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEC90);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AECA0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AECB0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AECC0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AECD0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AECE0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AECF0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED00);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED10);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED20);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED30);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED40);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED50);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED60);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED70);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED80);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AED90);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEDA0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEDB0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEDC0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEDD0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEDE0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEDF0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE00);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE10);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE20);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE30);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE40);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE50);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE60);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE70);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE80);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEE90);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEEA0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEEB0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEEC0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEED0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEEE0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEEF0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF00);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF10);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF20);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF30);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF40);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF50);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF60);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF70);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF80);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEF90);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEFA0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEFB0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEFC0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEFD0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEFE0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AEFF0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AF000);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018F738", D_004AF010);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF240);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF248);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF250);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF258);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF260);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF268);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF270);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF278);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF280);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF288);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager_0018F738", D_004DF290);

