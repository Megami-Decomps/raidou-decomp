#include "common.h"
#include "sdk_libc.h"
#include "sdfDraw.h"
#include "sdfTex.h"
#include "sdfMath.h"
#include "sdfPacketBuff.h"
#include "sdfBillBoard.h"

#include "include_asm.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "ps2asm.h"
#include "fpu.h"
#include "fldFileResolver_00132080.h"
#include "code_r1_001DED50.h"

/* Serialized entry offsets are relative to BillData.base. */
/* BillAnimationEntry.flags bits; distinct from entryModeFlags and billHandle_t.modeFlags. */
enum {
    BILL_ANIMATION_FLAG_FRAME_COLORS = 0x00000001,
    BILL_ANIMATION_FLAG_PACKET_LIST_2 = 0x00000002,
    BILL_ANIMATION_FLAG_PACKET_LIST_3 = 0x00000004,
    BILL_ANIMATION_FLAG_PACKET_LIST_MASK = 0x00000006,
    BILL_ANIMATION_FLAG_STOP_AT_END = 0x00000010,
    BILL_ANIMATION_FLAG_PLURAL_ENTRIES = 0x10000000
};

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

/* Return packed 64-bit TEX0, TEX1, and CLAMP states from the primary GS packet. */

typedef struct SdfGsTexturePacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite sampling;
    SdfGsRegisterWrite texture;
    SdfGsRegisterWrite clamp;
} SdfGsTexturePacket;

typedef char SdfGsTexturePacket_size_must_be_0x50[
    (sizeof(SdfGsTexturePacket) == 0x50) ? 1 : -1];

/* Caller-built composite draw description; 0x68 bytes. */
typedef struct effCompositeGsDescriptor_t {
    f32 position[4];
    f32 scaleX;
    f32 scaleY;
    u8 unk18[8];
    f32 angle;
    u32 color;
    u32 blendMode;
    BillTextureQuad primaryUv;
    BillTextureQuad secondaryUv;
    sdfTexHandle_t *primaryTexture;
    u64 primaryClamp;
    sdfTexHandle_t *secondaryTexture;
    u8 unk5C[4];
    u64 secondaryClamp;
} effCompositeGsDescriptor_t;

typedef char effCompositeGsDescriptor_size_must_be_0x68[
    (sizeof(effCompositeGsDescriptor_t) == 0x68) ? 1 : -1];

/* Producer-built input shared with the generated-texture packet providers.
 * Bytes 0x18..0x1F remain opaque; the provider does not read them. */
typedef struct effGeneratedTextureDescriptor_t {
    f32 position[4];
    f32 scaleX;
    f32 scaleY;
    u8 opaque18[8];
    f32 angle;
    u32 color;
    struct sdfTexHandle_t *texture;
} effGeneratedTextureDescriptor_t;

typedef char EffGeneratedTextureDescriptorSizeAssert[
    (sizeof(effGeneratedTextureDescriptor_t) == 0x2C) ? 1 : -1];

#define BILL_VARIANT_MASK 0xFFFF

#define EFF_PACKET_LIST_BYTES 0x20

void effDrawGeneratedTextureQuad(sdfDrawTag_t *packet, effGeneratedTextureDescriptor_t *source);
void effDrawCompositeTextureQuad(sdfDrawTag_t *packet, effCompositeGsDescriptor_t *source);
extern f32 aViewTarget[4], aViewEye[4];
extern u32 D_0034E090[],D_0034E0A0[];
void effDrawGeneratedTextureQuad(sdfDrawTag_t *list, effGeneratedTextureDescriptor_t *source);
void effSubmitGeneratedTexturePacket(sdfGrOt_t *surface, effGeneratedTextureDescriptor_t *source);
void effDrawCompositeTextureQuad(sdfDrawTag_t *list, effCompositeGsDescriptor_t *source);
void effSubmitCompositeGsPacket(sdfGrOt_t *surface, effCompositeGsDescriptor_t *source);

void effDrawGeneratedTextureQuad(sdfDrawTag_t *list, effGeneratedTextureDescriptor_t *source) {
    f32 matrix[16] __attribute__((aligned(16)));
    BillTextureQuad uv;
    f32 corners[4][2];
    SdfGsTexturePacket *texturePacket;
    u64 *packet;
    s16 width, height;
    f32 halfWidth, halfHeight;
    f32 cosine, sine;
    f32 cornerX, cornerY;

    texturePacket = (SdfGsTexturePacket *)sdfGrAllocPacketBuff(sizeof(SdfGsTexturePacket));
    texturePacket->header.dmaTag = 4;
    texturePacket->header.vifCommands = 0x5000000410000000ULL;
    texturePacket->header.gifTag = 0x1000000000008003ULL;
    texturePacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    texturePacket->sampling.value = sdfTexGetTex1Reg(source->texture);
    texturePacket->sampling.registerId = SDF_GS_TEX1_1;
    texturePacket->texture.value = sdfTexGetTex0Reg(source->texture);
    texturePacket->texture.registerId = SDF_GS_TEX0_1;
    texturePacket->clamp.value = sdfTexGetClampReg(source->texture);
    texturePacket->clamp.registerId = SDF_GS_CLAMP_1;
    sdfDrawTagLink(list, (u32)texturePacket);

    EE_MMI_UNIT_MATRIX(matrix);
    packet = (u64 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket((u8 *)packet, matrix);
    sdfDrawTagLink(list, (u32)packet);

    width = (u16)source->texture->width << 4;
    height = (u16)source->texture->height << 4;
    halfWidth = (f32)(width >> 5) * 0.0625f * source->scaleX;
    halfHeight = (f32)(height >> 5) * 0.0625f * source->scaleY;
    uv.components[0] = 0;
    uv.components[1] = 0;
    uv.components[2] = width;
    uv.components[3] = 0;
    uv.components[4] = width;
    uv.components[5] = height;
    uv.components[6] = 0;
    uv.components[7] = height;
    if (source->angle == 0.0f) {
        corners[0][0] = -halfWidth;
        corners[0][1] = -halfHeight;
        corners[1][0] = halfWidth;
        corners[1][1] = -halfHeight;
        corners[2][0] = halfWidth;
        corners[2][1] = halfHeight;
        corners[3][0] = -halfWidth;
        corners[3][1] = halfHeight;
    } else {
        cosine = sdfCos(source->angle);
        sine = sdfSin(source->angle);
        cornerX = -halfWidth;
        cornerY = -halfHeight;
        corners[0][0] = cornerX * cosine - cornerY * sine;
        corners[0][1] = cornerX * sine + cornerY * cosine;
        cornerX = halfWidth;
        cornerY = -halfHeight;
        corners[1][0] = cornerX * cosine - cornerY * sine;
        corners[1][1] = cornerX * sine + cornerY * cosine;
        cornerX = halfWidth;
        cornerY = halfHeight;
        corners[2][0] = cornerX * cosine - cornerY * sine;
        corners[2][1] = cornerX * sine + cornerY * cosine;
        cornerX = -halfWidth;
        cornerY = halfHeight;
        corners[3][0] = cornerX * cosine - cornerY * sine;
        corners[3][1] = cornerX * sine + cornerY * cosine;
    }

    packet = (u64 *)sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1((u8 *)packet, source->position, source->color, &uv, &corners[0][0]);
    sdfDrawTagLink(list, (u32)packet);
}

/* Allocate/init a packet list, generate its texture payload, then append it to the draw surface. */
void effSubmitGeneratedTexturePacket(sdfGrOt_t *surface, effGeneratedTextureDescriptor_t *source) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    effDrawGeneratedTextureQuad(packetList, source);
    surface->regFunc(surface, packetList);
}

INCLUDE_ASM(const s32, "code_r1_001DED50", effDrawCompositeTextureQuad);

/* Allocate/init a packet list, generate its composite GS payload, then append it to the draw surface. */
void effSubmitCompositeGsPacket(sdfGrOt_t *surface, effCompositeGsDescriptor_t *source) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    effDrawCompositeTextureQuad(packetList, source);
    surface->regFunc(surface, packetList);
}
