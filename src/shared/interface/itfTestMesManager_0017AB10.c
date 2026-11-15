#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

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

s32 pbCalcFovDegrees(f32 *, f32, f32, f32, f32);
extern char pbDebugBlurOnText[];
extern char pbDebugBlurOffText[];
extern char pbDebugPanel2CursorMark[];
extern char pbDebugPanel2RowFormat[];
extern char pbDebugBlurValueFormat[];
extern char pbDebugBlurOffsetFormat[];
typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

typedef struct
{
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t reg[2];
} pbRegPacket_t;

extern u8 dds3PadButtonState[];
extern sdfVector4_t aViewEye, aViewTarget;
s32 pbCalcFovDegrees(f32 *value, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep);
extern char pbDebugBlurOnText[8];
extern char pbDebugBlurOffText[8];
extern char pbDebugPanel2CursorMark[8];
extern char pbDebugPanel2RowFormat[8];
extern char pbDebugBlurValueFormat[8];
extern char pbDebugBlurOffsetFormat[8];
extern s32 (*pbMapCallbacks[2])(s16 x, s16 y, s32 mode);

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

