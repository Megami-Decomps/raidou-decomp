/* Declarations of smt3-decomp src/smt3/sdf/sdfDraw.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFDRAW_H
#define SHARED_SDF_SDFDRAW_H

#include "common.h"

#include "eeregs.h"

#include "libdma.h"

#include "sdfGsMemIO.h"

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

/* Values written to the PRIM and TEST registers by the centered-bounds packet. */
enum {
    SDF_GS_PRIMITIVE_SPRITE = 0x06,
    SDF_GS_CENTERED_VIEW_BOUNDS_TEST = 0x30003
};

extern u8 sdfFrameId;

#define SDF_DMA_TAG_REF_BYTE 0x30

#define SDF_DMA_CNT_ONE_WORD 0x10000001

#define SDF_VIF_DIRECT_ONE_FLUSHE 0x5000000110000000ULL

#define SDF_GIF_PACKED_AD_BITS 0x1000000000008000ULL

#define SDF_VIF_DIRECT_ONE_NOP 0x5000000100000000ULL

#define SDF_GS_DEFAULT_TEXA 0x4000000080ULL

#define SDF_GS_SCENE_TEST 0x517FB

#define SDF_TEXTURE_SCENE_PAYLOAD_QWORDS 0x15

#define SDF_GRAPH_DEPTH_BUFFER_INDEX 2

extern u8 sdfGrBufferSlot;

extern u16 sdfReqDrawDelaySleepCount;

extern u32 sdfDrawFrameCount;

extern u8 sdfDrawImageTrans;

extern volatile u8 sdfDrawSyncFlag;

extern u32 sdfGetRunTimeCountBase(void);

extern u32 sdfGetRunTimeCount(u32 aBaseTime);

extern void sdfWaitVblank(s32 count);

struct sdfPrimTransElem_t;

struct sdfGrRefPacketHeader_t;

struct sdfPreDrawHook_t;

struct sdfGrDrawFramePacket_t;

struct sdfGrClearPacket_t;

struct sdfGrRefGifPacketHeader_t;

struct sdfGrDrawFrame_t;

struct sdfGrDrawEnv_t;

struct sdfGsMemSlice_t;

struct sdfGrFrameSet_t;

struct sdfDrawTag_t;

struct sdfGrOt_t;

struct sdfGrFramePacket_t;

struct SdfDmaTag;

struct sdfDrawReqRec_t;

struct sdfPreDrawTag_t;

struct sdfPreDrawChain_t;

/* The frame worker passes the node and queued buffer index. The registered
 * handlers project the node into their distinct complete packet owners. */
typedef void (*SdfPacketPatchCallback)(void *, s32);

#define SDF_DMA_TAG_NEXT_BYTE 0x20

#define SDF_DMA_TAG_CALL_BYTE 0x50

#define SDF_DMA_TAG_END_BYTE 0x70

#define SDF_DMA_QWC_MASK 0xFFFF

#define SDF_QWORD_BYTES 0x10

#define SDF_DMA_EXTENDED_TAIL_OFFSET 0x30

#define SDF_DMA_REFERENCE_NODE_BYTES 0x20

#define SDF_DMA_TAG_REF_WORD 0x30000000

#define SDF_DMA_TAG_NEXT_WORD 0x20000000

#define SDF_VIF_DIRECT_WORD 0x50000000

#define SDF_VIF_FLUSHE_WORD 0x10000000

#define SDF_GIF_ONE_REGISTER_WORD 0x10000000

#define SDF_GIF_EOP_BIT 0x8000

#define SDF_GS_CENTER_BIAS 0x1000

#define SDF_GS_FIELD_OFFSET_STEP 8

#define SDF_GS_DEFAULT_ALPHA 0x44

#define SDF_SCENE_DRAW_PAYLOAD_QWORDS 5

extern volatile s8 sdfReqDrawDelayCount;

/* TEST and ALPHA form the two A+D writes shared by the blend builders. */
typedef struct sdfPrimTransElem_t {
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t alpha;
} sdfPrimTransElem_t;

/* REF tag and VIF commands followed by the trailing chain tag (0x20).
 * The complete trailing quadword is cleared before its NEXT header is patched. */
typedef struct sdfGrRefPacketHeader_t {
    u64 dmaTag;
    u64 vifCommands;
    s128 nextTag;
} sdfGrRefPacketHeader_t;

/* Pre-draw hook: the engine's record continues with two option words, which
 * each owner reads under its own name. */
typedef struct sdfPreDrawHook_t {
    struct sdfPreDrawHook_t *next;
    SdfPacketPatchCallback predrawFunc;
} sdfPreDrawHook_t;

/* Four A+D writes define one GS drawing context, without a DMA/GIF header. */
typedef struct sdfGrDrawFramePacket_t {
    sdfGrTagStdReg_t frame;
    sdfGrTagStdReg_t zbuf;
    sdfGrTagStdReg_t xyoffset;
    sdfGrTagStdReg_t scissor;
} sdfGrDrawFramePacket_t;

/* A+D writes that draw the centered viewport bounds as a sprite. */
typedef struct sdfGrClearPacket_t {
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t primitive;
    sdfGrTagStdReg_t rgbaq;
    sdfGrTagStdReg_t xyz2[2];
} sdfGrClearPacket_t;

/* Shared A+D state for GS primitive modes, color clamping, dithering and TEXA. */
typedef struct sdfGrRefGifPacketHeader_t {
    sdfGrTagStdReg_t dmatag;
    sdfGrTagStdReg_t giftag;
    sdfGrTagStdReg_t reftag;
    sdfGrTagStdReg_t linktag;
} sdfGrRefGifPacketHeader_t;

/* Accessed output span of the common scene-context builder. */
typedef struct sdfGrDrawFrame_t {
    sdfGrPacketHeader_t header;
    sdfGrDrawFramePacket_t packet;
} sdfGrDrawFrame_t;

/* Complete DMA/GIF owner used by scene builders and held-texture draw banks. */
typedef struct sdfGrDrawEnv_t {
    sdfGrPacketHeader_t header;
    sdfGrRefGifPacketHeader_t defParm;
    sdfGrDrawFramePacket_t frame1;
    sdfGrDrawFramePacket_t frame2;
    sdfGrClearPacket_t clear;
    sdfGrTagStdReg_t test1;
    sdfGrTagStdReg_t alpha1;
    sdfGrTagStdReg_t test2;
    sdfGrTagStdReg_t alpha2;
} sdfGrDrawEnv_t;

/* Graph target: two color buffers followed by the auxiliary/depth buffer (0x14). */
typedef struct sdfGrFrameSet_t {
    s16 width;
    s16 h;
    s16 height;
    u8 bufferFormat;
    u8 auxiliaryFormat;
    sdfGsMemSlice_t *buffers[3];
} sdfGrFrameSet_t;

/* Draw-surface pool entry (0x20); the SDK initializes and flushes this owner. */
typedef struct sdfGrOt_t {
    struct sdfGrOt_t *next; /* 0x00 */
    sdfDrawTag_t *tagHead;     /* 0x04 */
    sdfDrawTag_t *tagTail;     /* 0x08 */
    u32 noSortTag;
    void (*regFunc)(struct sdfGrOt_t *, sdfDrawTag_t *);          /* 0x10 */
    s32 (*callbackFunc)(struct sdfGrOt_t *, s32, sdfDrawTag_t *); /* 0x14 */
    u32 data;
    u32 pad;
} sdfGrOt_t;

/* Frame/depth builder state: one DMA/GIF header and two A+D register writes. */
typedef struct sdfGrFramePacket_t {
    u8 pad00[4];
    void (*prepare)(struct sdfGrFramePacket_t *, s32 bufferIndex);
    u8 pad08[8];
    sdfGrPacketHeader_t packetHeader;
    sdfGrTagStdReg_t frame;
    sdfGrTagStdReg_t zbuff;
    sdfGrFrameSet_t *frameSet;
    u32 frameMask;
    s32 zbuffMask;
    s32 context;
} sdfGrFramePacket_t;

/* Packet-list code views the same 16-byte row as QWC/ID/ADDR/VIF commands. */
typedef struct SdfDmaTag {
    u16 quadwordCount;
    u8 pad02;
    u8 kind;
    u32 address;
    u64 vifCommands;
} QWORD_ALIGNED SdfDmaTag;

void sdfDrawTagLink2(struct sdfDrawTag_t *aTag, addr_t aHeadPacket,
                          addr_t aTailPacket);

void sdfDrawTagLinkRef(struct sdfDrawTag_t *aTag, addr_t aPacket);

/* One 0x10-byte entry in the three-slot frame packet submission ring. */
typedef struct sdfDrawReqRec_t {
    struct sdfDrawTag_t *drawTag;
    sdfPreDrawHook_t *preDraw;
    u8 drawCount;
    u8 queuedBufferIndex;
    u8 bufferIndex;
    u8 bufferSlotIndex;
    u8 pad0C[4];
} sdfDrawReqRec_t;

/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct sdfPreDrawTag_t {
    u32 next;
    sdfPreDrawHook_t *head;
    sdfPreDrawHook_t *tail;
    u32 pad;
} sdfPreDrawTag_t;

/* Frame submission joins these 0x10-byte linked owners through head/tail. */
typedef struct sdfPreDrawChain_t {
    sdfPreDrawTag_t *tagHead;
    sdfPreDrawTag_t *tail;
} sdfPreDrawChain_t;

typedef struct sdfTexPacket_t {
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t tex1;
    sdfGrTagStdReg_t tex0;
    sdfGrTagStdReg_t clamp;
} sdfTexPacket_t;

/* Linked draw environment, including its two frame-mask writes and textures. */
typedef struct sdfGrStdDrawEnv_t {
    sdfPreDrawHook_t link;
    sdfGrFrameSet_t *view;
    u8 unk0C[4];
    sdfGrPacketHeader_t header;
    sdfGrRefGifPacketHeader_t defParm;
    sdfGrDrawFramePacket_t frame1;
    sdfGrDrawFramePacket_t frame2;
    sdfGrClearPacket_t clear;
    sdfGrTagStdReg_t test1;
    sdfGrTagStdReg_t alpha1;
    sdfGrTagStdReg_t test2;
    sdfGrTagStdReg_t alpha2;
    sdfGrTagStdReg_t frameContextOne;
    sdfGrTagStdReg_t frameContextTwo;
    sdfTexPacket_t texturePackets[2];
} sdfGrStdDrawEnv_t;

void sdfGrStdDrawEnvPredraw(sdfGrStdDrawEnv_t *aPre, s32 aEvenOdd);

void sdfGrSetRefGifPacket(sdfGrRefGifPacketHeader_t *aPacket, addr_t aRegs, s32 aNumRegs);

void sdfGrSetDrawDefParmPacket(sdfGrRefGifPacketHeader_t *aDef);

extern s32 EIntr(void);

extern s32 DIntr(void);

extern s32 sdfDrawReqQueueIndex;

extern sdfDrawReqRec_t sdfGrDrawState[];

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

void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, addr_t aPacket);

void sdfDrawTagLinkCall(sdfDrawTag_t *aTag, addr_t aPacket);

void sdfGrOtFuncLinkToHead(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

void sdfGrOtNormalReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

s32 sdfGrOtNormalCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData);

void sdfGrOtInit(sdfGrOt_t *aOt, s32 aNumOt);

sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *ex);

SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *p, SdfDmaTag *ex);

void sdfGrJoinDrawTag(sdfDrawTag_t *aFore, sdfDrawTag_t *aBack);

void sdfGrInitExPacket(sdfDrawTag_t *p);

sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *aOt);

void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook);

void sdfPreDrawChainLink(sdfPreDrawChain_t *aChain, sdfPreDrawTag_t *aTag);

void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords);

void sdfGrSetDrawFramePacket(sdfGrDrawFramePacket_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                  s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat,
                  s32 aHalfOffset, s32 aContext);

void sdfGrSetClearPacket(sdfGrClearPacket_t *aClear, s32 aW, s32 aH, s32 aPixelFormat, s32 aZBuffFormat);

void sdfGrSetDrawFrame(sdfGrDrawFrame_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                           s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat, s32 aContext);

void sdfGrLinkStdDrawEnv(sdfDrawTag_t *aTag, sdfPreDrawTag_t *aPreDrawTag, sdfPreDrawHook_t *aEnv);

void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, addr_t aEnv, sdfGrRefPacketHeader_t *aPkt);

void sdfGrLinkDrawFrameTex(sdfDrawTag_t *aTag, u32 aEnv, sdfGrRefPacketHeader_t *aPkt);

void sdfGrFramePacketPredraw(sdfGrFramePacket_t *aPre, s32 aEvenOdd);

void sdfGrSetFramePacket(sdfGrFramePacket_t *aFrame, sdfGrFrameSet_t *aFrameSet, u32 aFrameMask, s32 aZBuffMask, s32 aContext);

void sdfGrReqDrawDelay(s32 count);

void sdfGrInitDraw(void);

void sdfDrawTagInit(sdfDrawTag_t *a);

void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

void sdfDrawTagLink2(sdfDrawTag_t *aTag, addr_t aHeadPacket, addr_t aTailPacket);

void sdfDrawTagLinkRef(sdfDrawTag_t *aTag, addr_t aPacket);

void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, addr_t aPacket);

void sdfDrawTagLinkCall(sdfDrawTag_t *aTag, addr_t aPacket);

void sdfGrOtFuncLinkToHead(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

void sdfGrOtNormalReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

s32 sdfGrOtNormalCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData);

void sdfGrOtInit(sdfGrOt_t *aOt, s32 aNumOt);

sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *ex);

SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *p, SdfDmaTag *ex);

void sdfGrJoinDrawTag(sdfDrawTag_t *aFore, sdfDrawTag_t *aBack);

void sdfGrInitExPacket(sdfDrawTag_t *p);

sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *aOt);

void sdfPreDrawTagInit(sdfPreDrawTag_t *aTag);

void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook);

void sdfPreDrawChainInit(sdfPreDrawChain_t *aChain);

void sdfPreDrawChainLink(sdfPreDrawChain_t *aChain, sdfPreDrawTag_t *aTag);

void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords);

void sdfGrSetRefGifPacket(sdfGrRefGifPacketHeader_t *aPacket, addr_t aRegs, s32 aNumRegs);

void sdfGrSetDrawFramePacket(sdfGrDrawFramePacket_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                  s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat,
                  s32 aHalfOffset, s32 aContext);

void sdfGrSetClearPacket(sdfGrClearPacket_t *aClear, s32 aW, s32 aH, s32 aPixelFormat, s32 aZBuffFormat);

void sdfGrSetDrawDefParmPacket(sdfGrRefGifPacketHeader_t *aDef);

void sdfGrSetDrawFrame(sdfGrDrawFrame_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                           s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat, s32 aContext);

void sdfGrSetDrawEnv(sdfGrDrawEnv_t *aEnv, sdfGrFrameSet_t *aFrameSet, s32 aEvenOdd);

extern void sdfTexSetPacket(sdfTexPacket_t *packet, s32 width, s32 height, s32 gsAddr,
                            s32 pixelFormat, s32 clutAddr, s32 clutFormat, s32 colorComp,
                            s32 mipmapCount, s32 mipmapParm, s32 wrapMode, s32 context);

void sdfGrSetStdDrawEnv(sdfGrStdDrawEnv_t *aEnv, sdfGrFrameSet_t *aFrameSet);

void sdfGrLinkStdDrawEnv(sdfDrawTag_t *aTag, sdfPreDrawTag_t *aPreDrawTag, sdfPreDrawHook_t *aEnv);

void sdfGrSetFrameMaskRGB(const u8 *aEnv, sdfGrRefGifPacketHeader_t *aHdr, s32 aContext);

void sdfGrSetFrameMaskReset(const u8 *aEnv, sdfGrRefGifPacketHeader_t *aHdr, s32 aContext);

void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, addr_t aEnv, sdfGrRefPacketHeader_t *aPkt);

void sdfGrLinkDrawFrameTex(sdfDrawTag_t *aTag, u32 aEnv, sdfGrRefPacketHeader_t *aPkt);

void sdfGrFramePacketPredraw(sdfGrFramePacket_t *aPre, s32 aEvenOdd);

void sdfGrSetFramePacket(sdfGrFramePacket_t *aFrame, sdfGrFrameSet_t *aFrameSet, u32 aFrameMask, s32 aZBuffMask, s32 aContext);

void sdfGrReqDraw(sdfDrawTag_t *aTag, sdfPreDrawChain_t *aPreDraw);

void sdfGrReqDrawDelay(s32 count);

void sdfGrInitDraw(void);

/* The draw worker publishes this to the kernel process, which clears it. */
extern volatile u8 sdfDrawPublishFlag;

extern volatile u8 sdfDrawFrameFlag;

extern volatile u8 sdfDispThreadFlag;

extern u8 sdfFrameEvenOdd;

extern u8 sdfGrDrawFrameId;

extern u8 sdfGrDrawFirstFlag;

extern u32 sdfGrDrawFrameTagHead;

extern s32 dds3GsThreadId;

extern s32 SignalSema(s32 sema);

extern s32 SleepThread(void);

extern void FlushCache(s32 mode);

extern sceDmaChan *sceDmaGetChan(u32 channel);

extern void sceDmaSend(sceDmaChan *channel, void *tag);

extern void sceDmaRecvN(sceDmaChan *channel, void *data, u32 qwc);

extern u64 sceGsGetIMR(void);

extern u64 sceGsPutIMR(u64 mask);

extern void sdfDeliverDrawStart(s32 frameId, s32 bufferId);

extern void sdfDeliverDrawEnd(s32 mode, s32 frameId, s32 halfOffset);

extern void sdfImageSetTrans(void);

extern void sdfDevConsDrawAll(void);

extern void sdfDelayedResrcProc(void);

void sdfGrDrawFrame(void);

void sdfDrawSync(void);

#endif
