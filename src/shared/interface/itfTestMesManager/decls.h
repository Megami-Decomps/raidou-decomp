/* Declarations of smt3-decomp src/smt3/interface/itfTestMesManager.c, for its bodies in this directory. */
#ifndef SHARED_INTERFACE_ITFTESTMESMANAGER_H
#define SHARED_INTERFACE_ITFTESTMESMANAGER_H

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

#endif
