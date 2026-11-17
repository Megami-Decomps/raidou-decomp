#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"
#include "sdfDraw.h"
#include "sdfMath.h"
#include "sdf2D.h"
#include "sdfPacketBuff.h"
#include "dds3GlobalWork.h"
#include "kernel_draw.h"
#include "sdfDevCons.h"
#include "dds3PadManager.h"

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
extern pbColor_t itfTestOverlayColor;
extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;
extern s32 D_00551EA8, pbDebugPanel3State;
extern s32 itfTestOverlayPhaseStep;
extern pbColorRow_t itfOverlayColorEditorRows[4];
extern u8 pbDebugPrintPanel3Data;
extern char pbDebugPrintPanel3Data2[];
extern void *dds3FontReqAllocDrawTag(void);

#include "sdk_libc.h"

void pbDrawRotatedPrim(s32 otIndex);
extern char pbDebugPanel3Title[];
extern char pbDebugPanel3CursorMark[];
extern char pbDebugPanel3RowFormat[];
extern char pbDebugChannelFormat[];
typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

typedef struct
{
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t reg[2];
} pbRegPacket_t;

extern sdfGrOt_t sdfGrOtTable[];
extern sdfVector4_t aViewEye, aViewTarget;
u64 *pbAllocPrimPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1);
void *pbMakeSpritePacket(u64 data, s32 context);
void *pbMakePrimPacket(u64 data, s32 context);
void pbDrawRotatedPrim(s32 surfaceIndex);
extern char pbDebugPanel3Title[8];
extern char pbDebugPanel3CursorMark[8];
extern char pbDebugPanel3RowFormat[8];
extern char pbDebugChannelFormat[8];
extern s32 (*pbMapCallbacks[2])(s16 x, s16 y, s32 mode);

#ifdef VERSION_RAIDOU1
s32 pbDebugPrintPanel3(void) {
    sdfDevPrintCB_t packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y = 0x7A20;
    s16 previousSelection;
    s32 previousRate;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        EE_ADDR(dds3PrimFrameRect(0x8950, 0x79A8, 0xFEFFFF, 0x5A0, 0x210, 0x60000000, 0x40806020)));
    sdfDevSetPrintCB(&packet, 0x8980, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, EE_ADDR(sdfDevPrintfEx(&packet, pbDebugPanel3Title)));
    if (pbDebugPanel3X.editing == 0) {
        sdfDevSetPrintCB(&packet, 0x8980, pbDebugPanel3X.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        pbDebugPanel3X.blink++;
        if (pbDebugPanel3X.blink < 32 || (pbDebugPanel3X.blink & 31) < 18) {
            sdfDrawTagLink(list, EE_ADDR(sdfDevPrintfEx(&packet, pbDebugPanel3CursorMark)));
        }
    }
    for (row = 0; row != 4; row++) {
        sdfDevSetPrintCB(&packet, 0x8980, y, 0xFF0000, itfOverlayColorEditorRows[row].highlight);
        sdfDrawTagLink(list,
            EE_ADDR(sdfDevPrintfEx(&packet, pbDebugPanel3RowFormat, itfOverlayColorEditorRows[row].title)));
        highlight = 0;
        if (pbDebugPanel3X.editing != 0 && row == pbDebugPanel3X.selection) {
            highlight = 6;
        }
        sdfDevSetPrintCB(&packet, 0x8C80, y, 0xFF0000, highlight);
        if (row < 4) {
            sdfDrawTagLink(list,
                EE_ADDR(sdfDevPrintfEx(&packet, pbDebugChannelFormat, itfTestOverlayColor.channels[row])));
        } else {
            sdfDrawTagLink(list,
                EE_ADDR(sdfDevPrintfEx(&packet, pbDebugPrintPanel3Data2, itfTestOverlayPhaseStep)));
        }
        y += 0x60;
    }
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    pbDebugPanel3State += itfTestOverlayPhaseStep;
    pbDrawRotatedPrim(102);
    if (dds3PadButtonState[0x32] < 0) {
        pbDebugPrintPanel3Data ^= 1;
    }
    if (pbDebugPanel3X.editing == 0) {
        previousSelection = pbDebugPanel3X.selection;
        if (dds3PadButtonState[0x36] & 2) {
            pbDebugPanel3X.selection--;
            if (pbDebugPanel3X.selection < 0) {
                pbDebugPanel3X.selection = 3;
            }
        }
        if (dds3PadButtonState[0x37] & 2) {
            pbDebugPanel3X.selection++;
            if (pbDebugPanel3X.selection >= 4) {
                pbDebugPanel3X.selection = 0;
            }
        }
        if (previousSelection != pbDebugPanel3X.selection) {
            pbDebugPanel3X.blink = 0;
        }
        if (dds3PadButtonState[0x33] < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
        if (dds3PadButtonState[0x31] & 2) {
            return -1;
        }
    } else {
        if (pbDebugPanel3X.selection < 4) {
            if (dds3PadButtonState[0x34] & 2) {
                itfTestOverlayColor.channels[pbDebugPanel3X.selection] -= 10;
                if (itfTestOverlayColor.channels[pbDebugPanel3X.selection] < 0) {
                    itfTestOverlayColor.channels[pbDebugPanel3X.selection] = 0;
                }
            }
            if (dds3PadButtonState[0x35] & 2) {
                itfTestOverlayColor.channels[pbDebugPanel3X.selection] += 10;
                if (itfTestOverlayColor.channels[pbDebugPanel3X.selection] > 255) {
                    itfTestOverlayColor.channels[pbDebugPanel3X.selection] = 255;
                }
            }
        } else {
            previousRate = itfTestOverlayPhaseStep;
            if (dds3PadButtonState[0x34] & 2) {
                itfTestOverlayPhaseStep -= 0x100;
                if (itfTestOverlayPhaseStep < 0) {
                    itfTestOverlayPhaseStep = 0;
                }
            }
            if (dds3PadButtonState[0x35] & 2) {
                itfTestOverlayPhaseStep += 0x100;
                if (itfTestOverlayPhaseStep > 0x800) {
                    itfTestOverlayPhaseStep = 0x800;
                }
            }
            if (previousRate != itfTestOverlayPhaseStep) {
                pbDebugPanel3State = 0;
            }
        }
        if (dds3PadButtonState[0x31] < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "interface/itfTestMesManager", pbDebugPrintPanel3);
#endif

u64 *pbAllocPrimPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = 0x14D;
    packet[5] = color0 | ((u64)0xFE00 << 46);
    packet[7] = color0 | ((u64)0xFE00 << 46);
    packet[9] = color1 | ((u64)0xFE00 << 46);
    packet[11] = color1 | ((u64)0xFE00 << 46);
    packet[6] = (u64)((x & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[8] = (u64)(((x + width) & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[10] = (u64)(((x + width) & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[12] = (u64)((x & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[13] = packet[14] = packet[15] = 0;
    return packet;
}

void *pbMakeSpritePacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = EE_ADDR_AS(pbRegPacket_t *, sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30));
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x48 : 0x47;
    return packet;
}

void *pbMakePrimPacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = EE_ADDR_AS(pbRegPacket_t *, sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30));
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x43 : 0x42;
    return packet;
}

void pbDrawRotatedPrim(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    pbColorByte_t *component;
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    f32 phase;

    phase = (f32)(pbDebugPanel3State % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSin(phase) + 1.0f) * 0.5f * itfTestOverlayColor.alpha);
    component = itfTestOverlayColor.channelBytes;
    for (i = 0; i != 3; i++, component++) {
        color |= component->value << (i * 8);
    }
    color |= (u32)alpha << 24;
    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list, EE_ADDR(pbMakeSpritePacket(0x33001, 0)));
    sdfDrawTagLink(list, EE_ADDR(pbMakePrimPacket(6, 0)));
    sdfDrawTagLink(list, EE_ADDR(pbAllocPrimPacket(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color)));
    surface = &sdfGrOtTable[surfaceIndex];
    ((sdfGrOtRegFunc_t)surface->regFunc)(surface, list);
}
INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbDebugPrintPanel3Data);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbDebugPanel3Title);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbDebugPanel3CursorMark);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbDebugPanel3RowFormat);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbDebugChannelFormat);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbDebugPrintPanel3Data2);

