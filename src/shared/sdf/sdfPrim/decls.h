/* Declarations of smt3-decomp src/smt3/sdf/sdfPrim.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFPRIM_H
#define SHARED_SDF_SDFPRIM_H

#include "common.h"

#include "sdf_types.h"

#include "sdf_resource.h"

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

extern u8 sdfGrBufferSlot;

extern u16 sdfReqDrawDelaySleepCount;

extern u32 sdfDrawFrameCount;

extern volatile u8 sdfDrawSyncFlag;

struct sdfPrimRectElem_t;

struct sdfPrimF3Elem_t;

struct sdfPrimF4Elem_t;

struct sdfPrimBoxLineElem_t;

struct sdfPrimTransElem_t;

struct sdfPrimTexflushPacket_t;

struct sdfPreDrawHook_t;

struct sdfPrimGt3Elem_t;

struct sdfPrimGt4Elem_t;

struct sdfPrimRectPacket_t;

struct sdfPrimSpriteElem_t;

struct sdfPrimFt3Elem_t;

struct sdfPrimFt4Elem_t;

struct sdfPrimG3Elem_t;

struct sdfPrimG4Elem_t;

struct sdfPrimBitBltElem_t;

struct sdfPrimF3Packet_t;

struct sdfPrimF4Packet_t;

struct sdfPrimBoxLinePacket_t;

struct sdfPrimTransPacket_t;

struct sdfPrimGt3Packet_t;

struct sdfPrimGt4Packet_t;

struct sdfPrimSpritePacket_t;

struct sdfPrimFt3Packet_t;

struct sdfPrimFt4Packet_t;

struct sdfPrimG3Packet_t;

struct sdfPrimG4Packet_t;

struct sdfPrimBitBltPacket_t;

struct sdfGsMemSlice_t;

struct sdfGrFrameSet_t;

struct sdfDrawTag_t;

struct sdfMemHandle_t;

struct sdfPrimFrameMovePacket_t;

struct sdfPreDrawTag_t;

/* The frame worker passes the node and queued buffer index. The registered
 * handlers project the node into their distinct complete packet owners. */
typedef void (*SdfPacketPatchCallback)(void *, s32);

/* GIF REGLIST stream: PRIM, RGBAQ and two packed XYZ2 vertices. */
typedef struct sdfPrimRectElem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[2];
} sdfPrimRectElem_t;

/* GIF stream for one flat-color triangle: tag/register list, PRIM, RGBAQ,
 * and the three XYZ2 values. */
typedef struct sdfPrimF3Elem_t {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[3];
} sdfPrimF3Elem_t;

/* GIF stream for four flat-color vertices: PRIM, RGBAQ and four XYZ2 values. */
typedef struct sdfPrimF4Elem_t {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[4];
} sdfPrimF4Elem_t;

/* GIF stream for a closed rectangle: its first XYZ2 value is repeated last. */
typedef struct sdfPrimBoxLineElem_t {
    u64 giftag;
    u64 gifRegisterList;
    u64 prim;
    u64 rgbaq;
    u64 xyz2[5];
} sdfPrimBoxLineElem_t;

#define SDF_PACKET_LIST_BYTES 0x20

#define SDF_GIF_ONE_REGISTER_WORD 0x10000000

#define SDF_GS_DEFAULT_ALPHA 0x44

#define SDF_GS_ALPHA_TEST 0x717FB

#define SDF_ALPHA_DMA_QWORDS 3

#define SDF_VIF_DIRECT_THREE_WORD 0x50000003

#define SDF_VIF_FLUSHE_HALFWORD 0x1000

#define SDF_GIF_TWO_AD_LOOPS_EOP 0x8002

extern volatile s8 sdfReqDrawDelayCount;

/* TEST and ALPHA form the two A+D writes shared by the blend builders. */
typedef struct sdfPrimTransElem_t {
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t alpha;
} sdfPrimTransElem_t;

/* A single A+D write follows the texture-flush DMA/VIF/GIF header. */
typedef struct sdfPrimTexflushPacket_t {
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t texflush;
} sdfPrimTexflushPacket_t;

/* Pre-draw hook: the engine's record continues with two option words, which
 * each owner reads under its own name. */
typedef struct sdfPreDrawHook_t {
    struct sdfPreDrawHook_t *next;
    SdfPacketPatchCallback predrawFunc;
    u32 option0;
    u32 option1;
} sdfPreDrawHook_t;

/* PRIM followed by 3 individually textured and colored vertices. */
typedef struct sdfPrimGt3Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 uv0;
    u64 rgbaq0;
    u64 xy0;
    u64 uv1;
    u64 rgbaq1;
    u64 xy1;
    u64 uv2;
    u64 rgbaq2;
    u64 xy2;
} sdfPrimGt3Elem_t;

/* PRIM followed by 4 individually textured and colored vertices. */
typedef struct sdfPrimGt4Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 uv0;
    u64 rgbaq0;
    u64 xy0;
    u64 uv1;
    u64 rgbaq1;
    u64 xy1;
    u64 uv2;
    u64 rgbaq2;
    u64 xy2;
    u64 uv3;
    u64 rgbaq3;
    u64 xy3;
} sdfPrimGt4Elem_t;

/* DMA/VIF header followed by the complete two-vertex GIF payload. */
typedef struct sdfPrimRectPacket_t {
    /* The chain tag's two data words carry the VIF codes that upload the elem. */
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimRectElem_t elem;
} sdfPrimRectPacket_t;

typedef struct sdfPrimSpriteElem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    u64 uv0;
    u64 xy0;
    u64 uv1;
    u64 xy1;
} sdfPrimSpriteElem_t;

/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct sdfPrimFt3Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 rgbaq;
    u64 uv0;
    u64 xy0;
    u64 uv1;
    u64 xy1;
    u64 uv2;
    u64 xy2;
} sdfPrimFt3Elem_t;

/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct sdfPrimFt4Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 rgbaq;
    u64 uv0;
    u64 xy0;
    u64 uv1;
    u64 xy1;
    u64 uv2;
    u64 xy2;
    u64 uv3;
    u64 xy3;
} sdfPrimFt4Elem_t;

/* PRIM followed by three independently colored vertices. */
typedef struct sdfPrimG3Elem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq0;
    u64 xy0;
    u64 rgbaq1;
    u64 xy1;
    u64 rgbaq2;
    u64 xy2;
} sdfPrimG3Elem_t;

/* PRIM followed by four independently colored vertices. */
typedef struct sdfPrimG4Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 rgbaq0;
    u64 xy0;
    u64 rgbaq1;
    u64 xy1;
    u64 rgbaq2;
    u64 xy2;
    u64 rgbaq3;
    u64 xy3;
} sdfPrimG4Elem_t;

/* The four A+D writes controlling a GS image transfer. */
typedef struct sdfPrimBitBltElem_t {
    sdfGrTagStdReg_t bitbltbuf;
    sdfGrTagStdReg_t trxpos;
    sdfGrTagStdReg_t trxreg;
    sdfGrTagStdReg_t trxdir;
} sdfPrimBitBltElem_t;

/* The caller prepends DMA/VIF words to the triangle GIF stream. Allocation
 * reserves 0x50 bytes; the final qword is not written by these helpers. */
typedef struct sdfPrimF3Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimF3Elem_t elem;
    u64 unwrittenAllocationWord;
} sdfPrimF3Packet_t;

/* The allocation contains the DMA/VIF prefix and the entire quad GIF stream. */
typedef struct sdfPrimF4Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimF4Elem_t elem;
} sdfPrimF4Packet_t;

/* The caller prepends DMA/VIF words. The final allocated qword is not written. */
typedef struct sdfPrimBoxLinePacket_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimBoxLineElem_t line;
    u64 unwrittenAllocationWord;
} sdfPrimBoxLinePacket_t;

extern struct sdfMemHandle_t *sdfGrPacketBuffHandle;

/* Fixed packets allocated by the paired blend and composite-quad renderers. */
typedef struct sdfPrimTransPacket_t {
    sdfGrPacketHeader_t header;
    sdfPrimTransElem_t elem;
} sdfPrimTransPacket_t;

typedef struct sdfPrimGt3Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimGt3Elem_t elem;
} sdfPrimGt3Packet_t;

typedef struct sdfPrimGt4Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimGt4Elem_t elem;
    u64 unwrittenTail; /* Final allocated word is not written by the builder. */
} sdfPrimGt4Packet_t;

typedef struct sdfPrimSpritePacket_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimSpriteElem_t spr;
} sdfPrimSpritePacket_t;

typedef struct sdfPrimFt3Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimFt3Elem_t elem;
} sdfPrimFt3Packet_t;

typedef struct sdfPrimFt4Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimFt4Elem_t elem;
} sdfPrimFt4Packet_t;

typedef struct sdfPrimG3Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimG3Elem_t elem;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} sdfPrimG3Packet_t;

typedef struct sdfPrimG4Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimG4Elem_t elem;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} sdfPrimG4Packet_t;

/* DMA/VIF and GIF header followed by the complete image-transfer register block. */
typedef struct sdfPrimBitBltPacket_t {
    sdfGrPacketHeader_t header;
    sdfPrimBitBltElem_t bitblt;
} sdfPrimBitBltPacket_t;

/* Graph target: two color buffers followed by the auxiliary/depth buffer (0x14). */
typedef struct sdfGrFrameSet_t {
    s16 width;
    s16 h;
    s16 height;
    u8 bufferFormat;
    u8 auxiliaryFormat;
    sdfGsMemSlice_t *buffers[3];
} sdfGrFrameSet_t;

/* Metadata node plus the extended local-to-local image-copy payload (0x70). */
typedef struct sdfPrimFrameMovePacket_t {
    sdfPreDrawHook_t worker; /* option0 selects the source resource for the buffer */
    sdfPrimBitBltPacket_t packet;
} sdfPrimFrameMovePacket_t;

/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct sdfPreDrawTag_t {
    u32 next;
    sdfPreDrawHook_t *head;
    sdfPreDrawHook_t *tail;
    u32 pad;
} sdfPreDrawTag_t;

extern s32 sdfGrPacketBuffSize;

extern s32 sdfGrPacketBuffBase[2];

extern s32 sdfGrPacketBuffPtr;

extern s32 sdfGrPacketBuffLimit;

extern s32 sdfDelayedResrcSemaId;

extern s32 sdfDrawReqQueueIndex;

extern sdfGrFrameSet_t sdfGrFrameSet;

typedef struct effDebugPolyParams_t {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} effDebugPolyParams_t;

s32 sdfGrAllocPacketBuff(s32 aSize);

void sdfDrawTagInit(sdfDrawTag_t *a);

void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook);

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32));

void sdfPrimLinkSimplePacket(s32 aTag, void (*aFunc)(s32), s32 aPacketSize, s32 (*aAllocator)(s32));

void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSemiTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSemiTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetAddTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSubTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSubTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSubTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetTexflushElem(sdfGrTagStdReg_t *aElem);

void sdfPrimSetTexflushPacket(sdfPrimTexflushPacket_t *aPkt);

void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *aElem, u32 aDstGsAddr, s32 aDstBuffWidth,
                  s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                  u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                  s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode);

void sdfPrimSetBitBltPacket(sdfPrimBitBltPacket_t *aPacket, u32 aDstGsAddr, s32 aDstBuffWidth,
                                    s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                                    u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                                    s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode);

void sdfPrimLinkBitBltPacket(s32 aTag, u32 aDstGsAddr, s32 aDstBuffWidth,
                             s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                             u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                             s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                             s32 aDirMode, s32 (*aAllocator)(s32));

void sdfPrimFrameMovePredraw(sdfPrimFrameMovePacket_t *aPre, s32 aEvenOdd);

void sdfPrimLinkFrameMovePacket(sdfDrawTag_t *aDrawTag, sdfPreDrawTag_t *aPreDrawTag,
                   sdfGsMemSlice_t *aBuffHandle, s32 aDstX, s32 aDstY,
                   s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                   s32 aFrameSw, s32 (*aAllocator)(s32));

void sdfPrimSetSpriteElem(sdfPrimSpriteElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ);

void sdfPrimLinkSprite(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetF3Elem(sdfPrimF3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aZ);

void sdfPrimLinkF3(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                   s32 aY1, s32 aX2, s32 aY2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetF4Elem(sdfPrimF4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ);

void sdfPrimLinkF4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetG3Elem(sdfPrimG3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ);

void sdfPrimLinkG3(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetG4Elem(sdfPrimG4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aX3, s32 aY3, s32 aColor3, s32 aZ);

void sdfPrimLinkG4(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1,
                   s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aX3,
                   s32 aY3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetFt3Elem(sdfPrimFt3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aZ);

void sdfPrimLinkFt3(s32 aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetFt4Elem(sdfPrimFt4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2, s32 aV2,
                   s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ);

void sdfPrimLinkFt4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ,
                   s32 (*aAllocator)(s32));

void sdfPrimSetGt3Elem(sdfPrimGt3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aColor2, s32 aZ);

void sdfPrimLinkGt3(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aZ,
                   s32 (*aAllocator)(s32));

void sdfPrimSetGt4Elem(sdfPrimGt4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2, s32 aU2,
                   s32 aV2, s32 aColor2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aColor3, s32 aZ);

void sdfPrimLinkGt4(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aX3, s32 aY3,
                   s32 aU3, s32 aV3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetRectElem(sdfPrimRectElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkRect(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetLineElem(sdfPrimRectElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetBoxLineElem(sdfPrimBoxLineElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkBoxLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32));

void sdfPrimLinkSimplePacket(s32 aTag, void (*aFunc)(s32), s32 aPacketSize, s32 (*aAllocator)(s32));

void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSemiTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSemiTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetAddTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSubTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSubTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSubTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetTexflushElem(sdfGrTagStdReg_t *aElem);

void sdfPrimSetTexflushPacket(sdfPrimTexflushPacket_t *aPkt);

void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *aElem, u32 aDstGsAddr, s32 aDstBuffWidth,
                  s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                  u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                  s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode);

void sdfPrimSetBitBltPacket(sdfPrimBitBltPacket_t *aPacket, u32 aDstGsAddr, s32 aDstBuffWidth,
                                    s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                                    u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                                    s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode);

void sdfPrimLinkBitBltPacket(s32 aTag, u32 aDstGsAddr, s32 aDstBuffWidth,
                             s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                             u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                             s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                             s32 aDirMode, s32 (*aAllocator)(s32));

void sdfPrimFrameMovePredraw(sdfPrimFrameMovePacket_t *aPre, s32 aEvenOdd);

void sdfPrimLinkFrameMovePacket(sdfDrawTag_t *aDrawTag, sdfPreDrawTag_t *aPreDrawTag,
                   sdfGsMemSlice_t *aBuffHandle, s32 aDstX, s32 aDstY,
                   s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                   s32 aFrameSw, s32 (*aAllocator)(s32));

void sdfPrimSetSpriteElem(sdfPrimSpriteElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ);

void sdfPrimLinkSprite(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetF3Elem(sdfPrimF3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aZ);

void sdfPrimLinkF3(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                   s32 aY1, s32 aX2, s32 aY2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetF4Elem(sdfPrimF4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ);

void sdfPrimLinkF4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetG3Elem(sdfPrimG3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ);

void sdfPrimLinkG3(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetG4Elem(sdfPrimG4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aX3, s32 aY3, s32 aColor3, s32 aZ);

void sdfPrimLinkG4(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1,
                   s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aX3,
                   s32 aY3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetFt3Elem(sdfPrimFt3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aZ);

void sdfPrimLinkFt3(s32 aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetFt4Elem(sdfPrimFt4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2, s32 aV2,
                   s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ);

void sdfPrimLinkFt4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ,
                   s32 (*aAllocator)(s32));

void sdfPrimSetGt3Elem(sdfPrimGt3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aColor2, s32 aZ);

void sdfPrimLinkGt3(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aZ,
                   s32 (*aAllocator)(s32));

void sdfPrimSetGt4Elem(sdfPrimGt4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2, s32 aU2,
                   s32 aV2, s32 aColor2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aColor3, s32 aZ);

void sdfPrimLinkGt4(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aX3, s32 aY3,
                   s32 aU3, s32 aV3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetRectElem(sdfPrimRectElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkRect(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetLineElem(sdfPrimRectElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetBoxLineElem(sdfPrimBoxLineElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkBoxLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

#endif
