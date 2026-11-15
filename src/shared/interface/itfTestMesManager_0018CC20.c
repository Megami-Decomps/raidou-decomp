#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"
#include "itfMisc.h"
#include "sdfTex.h"

typedef struct { u8 unk00[8]; f32 degrees; u8 unk0C[4]; } pbFovWork_t;

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
extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;
extern s32 D_00551EA8, pbDebugPanel3State;

#include "sdk_libc.h"

typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

typedef struct
{
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t reg[2];
} pbRegPacket_t;

extern char *TestMsgReloadTextureData[];
extern u8 dds3PadButtonState[];
extern s32 testMsgTexIndex;
extern void *testMsgTexture;
extern void itfDebugPrintMark(void);
extern sdfVector4_t aViewEye, aViewTarget;
void TestMsgReloadTexture(void);
s32 TestMsgMngC_Process(dds3ProcessID_t *id);
void itfDebugPrintMark(void);
extern s32 (*pbMapCallbacks[2])(s16 x, s16 y, s32 mode);

INCLUDE_ASM(const s32, "interface/itfTestMesManager_0018CC20", itfDebugMesMngInitialize);

void TestMsgReloadTexture(void)
{
    if (testMsgTexture != NULL) {
        sdfTexFreeDelayed(testMsgTexture);
        testMsgTexture = NULL;
    }
    testMsgTexIndex = (testMsgTexIndex + 1) & 3;
    if (testMsgTexIndex != 3) {
        testMsgTexture = itfMiscLoadTexture(TestMsgReloadTextureData[testMsgTexIndex]);
    }
}

s32 TestMsgMngC_Process(dds3ProcessID_t *id)
{
    if ((s8)dds3PadButtonState[0x20] < 0) {
        TestMsgReloadTexture();
    }
    return 0;
}

INCLUDE_ASM(const s32, "interface/itfTestMesManager_0018CC20", TestMsgMngD_Process);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018CC20", TestMsgReloadTextureDataStrFontSampleBg02Tmx);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018CC20", TestMsgReloadTextureDataStrFontSampleBg01Tmx);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager_0018CC20", TestMsgReloadTextureDataStrFontSampleBg00Tmx);

void itfDebugPrintMark(void) {
    printf("********* AAAA ********\n");
}
s32 testMsgTexIndex = 3;

void *testMsgTexture = NULL;
