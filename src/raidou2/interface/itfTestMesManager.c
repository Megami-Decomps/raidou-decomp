#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"
#include "sdfDraw.h"
#include "sdfMath.h"

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
extern void *sdfGrAllocPacketBuff(u32 aSize);
extern void *sdf2DMakeStdPacket(void *aPacket, u32 aLeng);
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

INCLUDE_ASM(const s32, "interface/itfTestMesManager", pbDebugPrintPanel3);

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

    packet = sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30);
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x48 : 0x47;
    return packet;
}

void *pbMakePrimPacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30);
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

