#include "common.h"
#include "sdk_libgcc.h"
#include "libsdf.h"
#include "effect.h"
#include "sdfFrame.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "sdfChip.h"
#include "billManager.h"
#include "kernel_draw.h"
#include "dds3KernelMain.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "effBlur_Filter.h"
#include "effPacket.h"
#include "sdf2D.h"
#include "sdfMath.h"
#include "dds3KernelDraw.h"
#include "sdf_packet_list.h"
#include "code_r1_0021CE60.h"

extern u8 dds3EffectBase[];
void dds3EffectBlur(const dds3Effect_Blur_t *source);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(u8 *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(u8 *name);
extern u32 dds3NullDrawProc(void);

/* The frame buffers the blur samples from; only the mask source is named. */
typedef struct effBlurDrawBank_t {
    u8 pad0000[0x1300];
    u8 frameMaskSource[0x8E0];
} effBlurDrawBank_t;

void dds3EffectBlurSetRect(dds3Effect_PartialMultiBlurParam_t *work);
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam);
void dds3EffectPartialMultiBlurSetParam(effBlurTemplate_t *hPartialMultiBlur, const dds3Effect_PartialMultiBlurParam_t *pParam);
void dds3EffectDistBlurPartInit(effBlurScatterWork_t *pParam, effBlurScatterSlot_t *pPart);
effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam);
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *hDistortionBlur, dds3Effect_RippleBlurParam_t *pParam);
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart);
void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur);
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam);
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam);

#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "sdf_movie_types.h"

void effDrawBlurSource(const dds3Effect_Blur_t *source, struct sdfTexHandle_t *resource, u8 fixedPointCoordinates);

typedef struct {
    u8 pad00[0x124];
    u16 kind; /* 0x124: kind 0x109 selects separate slots for the two resources */
} BlurActor;

typedef dds3Effect_Blur_t BlurSource;

struct sdfDrawTag_t;

struct sdfGrRefPacketHeader_t;

/* Queue blend setup and both rectangle packets, then finish with the filter draw. */

void dds3EffectBlurDrawAlphaMulti(sdfDrawTag_t *list, const dds3Effect_Blur_t *source, u8 fixedPointCoordinates) {
    sdfGrPacketHeader_t *packet;

    packet = effPkt2DAllocTriStrip(1, 0x200);
    effEctBlurDrawAlphaMultiSub(source, effPkt2DGetPrimPtr(packet), fixedPointCoordinates);
    sdfDrawTagLink(list, EE_ADDR(packet));
    packet = effPkt2DAllocTriStrip(1, 0);
    dds3CallCos(source, effPkt2DGetPrimPtr(packet), fixedPointCoordinates);
    sdfDrawTagLink(list, EE_ADDR(packet));
}

void dds3EffectBlurDrawFrameMaskReset(sdfDrawTag_t *list) {
    sdfGrRefGifPacketHeader_t *packet;
    u32 frame;
    sdfGrOt_t *ot;

    packet = EE_ADDR_AS(sdfGrRefGifPacketHeader_t *, sdfGrAllocPacketBuff(0x40));
    frame = dds3GetFrameID();
    sdfGrSetFrameMaskReset((const u8 *)&dds3DrawEnv[frame].stdEnv, packet, 1);
    sdfDrawTagLinkRefGif(list, EE_ADDR(packet));
    ot = &effData3;
    ot->regFunc(ot, list);
}

void effCallNullDrawProc(const dds3Effect_Blur_t *source, sdfTexHandle_t *texture, u8 fixedPointCoordinates) {
    sdfDrawTag_t *list;

    if (dds3NullDrawProc() != 0) {
        return;
    }
    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    effCallGrAllocPacketBuff(list, source->blend, texture);
    dds3EffectBlurDrawAlphaMulti(list, source, fixedPointCoordinates);
    dds3EffectBlurDrawFrameMaskReset(list);
}

void dds3EffectBlurSetRect(dds3Effect_PartialMultiBlurParam_t *work) {
    s32 x = work->Blur.center[0] + 0x100;
    s32 y = work->Blur.center[1] + 0xE0;
    s32 extent = work->area;

    work->Blur.xy[0][0] = x - extent;
    work->Blur.xy[0][1] = y - extent;
    work->Blur.xy[1][0] = x + extent;
    work->Blur.xy[1][1] = y + extent;
    dds3EffectBlur(&work->Blur);
}

/* Clone a blur template into a fresh allocation. */
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam) {
    effBlurTemplate_t *dst = sdfChipAllocDirect(sizeof(effBlurTemplate_t));

    dst->texture = dds3BillboardGetGeneralTex(2);
    memcpy(&dst->body, pParam, sizeof(*pParam));
    return dst;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBlur_Filter.c:322, match, same function 1.00 (words 5/5, calls 1.00, shape 1.00) */
void effStopSub(effBlurTemplate_t *hPartialMultiBlur) {
    sdfChipFreeDirect(hPartialMultiBlur);
}

void effCallNullDrawProc2(effBlurTemplate_t *work) {
    s32 x;
    s32 y;
    s32 extent;

    if (dds3NullDrawProc() != 0) {
        return;
    }
    x = work->body.Blur.center[0] + 0x100;
    y = work->body.Blur.center[1] + 0xE0;
    extent = work->body.area;
    work->body.Blur.xy[0][0] = x - extent;
    work->body.Blur.xy[0][1] = y - extent;
    work->body.Blur.xy[1][0] = x + extent;
    work->body.Blur.xy[1][1] = y + extent;
    effCallNullDrawProc(&work->body.Blur, work->texture, 0);
}

/* GS coordinates use sixteenth-pixel X and half-height Y; extent is halved for Y. */
void effDrawBlurFixedPointRectangle(dds3Effect_PartialMultiBlur_t *owner) {
    s32 centerX, centerY, halfExtent;

    if (dds3NullDrawProc() == 0) {
        centerX = owner->body.Blur.center[0] + 0x1000;
        centerY = (owner->body.Blur.center[1] + 0xE00) >> 1;
        halfExtent = owner->body.area;
        owner->body.Blur.xy[0][0] = centerX - halfExtent;
        owner->body.Blur.xy[1][0] = centerX + halfExtent;
        halfExtent >>= 1;
        owner->body.Blur.xy[0][1] = centerY - halfExtent;
        owner->body.Blur.xy[1][1] = centerY + halfExtent;
        effDrawBlurSource(&owner->body.Blur, owner->texture, 1);
    }
}

/* Copy only the serialized parameters, leaving the owned work tail intact. */
void dds3EffectPartialMultiBlurSetParam(effBlurTemplate_t *hPartialMultiBlur, const dds3Effect_PartialMultiBlurParam_t *pParam) {
    hPartialMultiBlur->body = *pParam;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effLeaf56);

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBlur_Filter.c:473, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void dds3EffectDistortionBlurSetTex(effBlurScaleWork_t *work) {
    work->texture = dds3BillboardGetGeneralTex(2);
}

void dds3EffectDistBlurPartInit(effBlurScatterWork_t *pParam, effBlurScatterSlot_t *pPart) {
    dds3Effect_Blur_t *quad = &pPart->quad;
    f32 spread;
    s32 halfSize;
    s32 centerX;
    s32 centerY;
    pPart->delay = SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % (pParam->params.interval + 1));
    pPart->angle = -3.14159265f;
    quad->rotate = pParam->params.rotate;
    quad->abgr = pParam->params.abgr;
    quad->blend = pParam->params.blend;
    spread = pParam->params.area;
    halfSize = pParam->params.width;
    quad->center[0] = pParam->params.center[0] +
        (s32)(spread * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f));
    halfSize >>= 1;
    quad->center[1] = pParam->params.center[1] +
        (s32)(spread * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f));
    centerX = quad->center[0] + 256;
    quad->xy[0][0] = centerX - halfSize;
    quad->xy[1][0] = centerX + halfSize;
    centerY = quad->center[1] + 224;
    quad->xy[0][1] = centerY - halfSize;
    quad->xy[1][1] = centerY + halfSize;
}

effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam)
{
    struct sdfMemHandle_t *allocation;
    effBlurScatterWork_t *work;
    effBlurScatterSlot_t *slot;
    s32 slotIndex;

    allocation = sdfMemAllocDirect(sizeof(effBlurScatterWork_t) + 100 * sizeof(effBlurScatterSlot_t));
    work = EE_ADDR_AS(effBlurScatterWork_t *, sdfMemLock(allocation));
    work->params = *pParam;
    work->allocation = allocation;
    work->slots = (effBlurScatterSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(2);
    slot = work->slots;
    for (slotIndex = 0; slotIndex < 100; slotIndex++, slot++) {
        dds3EffectDistBlurPartInit(work, slot);
        slot->angle = 3.14159265f + 1.0f;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBlur_Filter.c:407, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
void effPCPDistortionBlurExpDeleteSub(effBlurScatterWork_t *hDistortionBlur) {
    sdfMemFreeDirect(hDistortionBlur->allocation);
}

void effBlurStepScatterSlotsAndDraw(effBlurScatterWork_t *work) {
    sdfDrawTag_t *list;
    effBlurScatterSlot_t *slot;
    s32 count;
    f64 level;
    f64 angle;

    if (dds3NullDrawProc() == 0) {
        list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(list);
        effCallGrAllocPacketBuff(list, work->params.blend, work->texture);
        slot = work->slots;
        if (work->params.number > 0) {
            count = work->params.number;
            do {
                if (slot->delay == 0) {
                    if (slot->angle > 3.1415926f) {
                        dds3EffectDistBlurPartInit(work, slot);
                    }
                    slot->quad.scale = work->params.distortion * sdfSin(slot->angle) + 1.0f;
                    level = fptodp((f32)(work->params.abgr >> 24));
                    angle = fptodp(slot->angle);
                    if (dpcmp(angle, 0.0) < 0) {
                        angle = dpsub(0.0, angle);
                    }
                    level = dpmul(level, dpsub(3.1415926f, angle));
                    level = dpmul(level, 0.31830988f);
                    slot->quad.abgr = (slot->quad.abgr & 0xFFFFFF) | (dptoul(level) << 24);
                    dds3EffectBlurDrawAlphaMulti(list, &slot->quad, 0);
                    if ((effGetFlags() & 1) == 0) {
                        slot->angle += work->params.speed;
                    }
                } else if ((effGetFlags() & 1) == 0) {
                    slot->delay--;
                }
                count--;
                slot++;
            } while (count != 0);
        }
        dds3EffectBlurDrawFrameMaskReset(list);
    }
}

/* Update scale parameters but preserve the allocated destination slot count. */
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *hDistortionBlur, dds3Effect_RippleBlurParam_t *pParam) {
    u32 count = hDistortionBlur->params.number;

    hDistortionBlur->params = *pParam;
    hDistortionBlur->params.number = count;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effLeaf57);

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBlur_Filter.c:355, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void dds3EffectPartialMultiBlurSetTexDef(effBlurScatterWork_t *hPartialMultiBlur) {
    hPartialMultiBlur->texture = dds3BillboardGetGeneralTex(2);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectRipBlurPartAreaSet);

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart) {
    dds3Effect_Blur_t *quad = &pPart->quad;

    pPart->phase = 0.0f;
    pPart->angle = 0.0f;
    quad->abgr = pParam->params.abgr;
    quad->blend = pParam->params.blend;
    quad->rotate = pParam->params.rotate;
    quad->center[0] = pParam->params.center[0];
    quad->center[1] = pParam->params.center[1];
}

/* Reset every slot, then stagger its initial phase by -spacing * index. */
void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur) {
    effBlurScaleSlot_t *slot = hRippleBlur->slots;
    s32 count = hRippleBlur->params.number;
    s32 i;

    for (i = 0; i < count; i++, slot++) {
        dds3EffectRipBlurPartInit(hRippleBlur, slot);
        slot->phase = -(hRippleBlur->params.interval * (f32)i);
    }
}

/* Allocate an owner and slot array from only the serialized parameter prefix. */
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam) {
    s32 count = pParam->number;
    void *allocation = sdfMemAllocDirect(count * 0x30 + 0x38);
    effBlurScaleWork_t *work = EE_ADDR_AS(effBlurScaleWork_t *, sdfMemLock(allocation));
    effBlurScaleSlot_t *slot;
    s32 i = 0;

    /* Copy through size, leaving the source handle, allocation and slots owned
       by this new work rather than reading a live-work tail from src. */
    memcpy(work, pParam, 0x2C);
    work->allocation = allocation;
    work->slots = (effBlurScaleSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(3);
    slot = work->slots;
    while (i < count) {
        dds3EffectRipBlurPartInit(work, slot);
        slot->phase = -(work->params.interval * (f32)i);
        i++;
        slot++;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBlur_Filter.c:539, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
void effPCPRippleBlurExpDeleteSub(effBlurScaleWork_t *hRippleBlur) {
    sdfMemFreeDirect(hRippleBlur->allocation);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", effBlurStepScaleSlotsAndDraw);

void effBlurBuildSamplingQuad(const dds3Effect_Blur_t *source, effBlurPrimTriStrip_t *quad, u8 fixedPointCoordinates) {
    f32 phase = source->rotate * 0.017453292519943295f;
    f32 amplitude = source->scale;
    f32 dx = sdfSin(phase) * amplitude;
    f32 dy = sdfCos(phase) * amplitude;

    quad->r = ((u8 *)&source->abgr)[0];
    quad->g = ((u8 *)&source->abgr)[1];
    quad->b = ((u8 *)&source->abgr)[2];
    quad->a = ((u8 *)&source->abgr)[3];
    quad->a = quad->a * effBlurGetBlurValid();
    if (fixedPointCoordinates == 0) {
        quad->vtx[0].x = (source->xy[0][0] << 4) + 0x7000;
        quad->vtx[0].y = (source->xy[0][1] << 3) + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = (source->xy[1][1] << 3) + 0x7900;
        quad->vtx[2].x = (source->xy[1][0] << 4) + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
        quad->vtx[0].s = (source->xy[0][0] + dx) * (1.0f / 512.0f);
        quad->vtx[0].t = (source->xy[0][1] + dy) * (1.0f / 512.0f);
        quad->vtx[1].s = quad->vtx[0].s;
        quad->vtx[1].t = (source->xy[1][1] + dy) * (1.0f / 512.0f);
        quad->vtx[2].s = (source->xy[1][0] + dx) * (1.0f / 512.0f);
        quad->vtx[2].t = quad->vtx[0].t;
        quad->vtx[3].s = quad->vtx[2].s;
        quad->vtx[3].t = quad->vtx[1].t;
    } else {
        quad->vtx[0].x = source->xy[0][0] + 0x7000;
        quad->vtx[0].y = source->xy[0][1] + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = source->xy[1][1] + 0x7900;
        quad->vtx[2].x = source->xy[1][0] + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
        quad->vtx[0].s = (source->xy[0][0] + dx) * (1.0f / 8192.0f);
        quad->vtx[0].t = (source->xy[0][1] + dy) * (1.0f / 4096.0f);
        quad->vtx[1].s = quad->vtx[0].s;
        quad->vtx[1].t = (source->xy[1][1] + dy) * (1.0f / 4096.0f);
        quad->vtx[2].s = (source->xy[1][0] + dx) * (1.0f / 8192.0f);
        quad->vtx[2].t = quad->vtx[0].t;
        quad->vtx[3].s = quad->vtx[2].s;
        quad->vtx[3].t = quad->vtx[1].t;
    }
    quad->vtx[0].z = 0;
    quad->vtx[0].adc = 0;
    quad->vtx[1].z = 0;
    quad->vtx[1].adc = 0;
    quad->vtx[2].z = 0;
    quad->vtx[2].adc = 0;
    quad->vtx[3].z = 0;
    quad->vtx[3].adc = 0;
}

void effBlurDrawFramebufferQuad(const dds3Effect_Blur_t *source) {
    sdfDrawTag_t *list;
    sdfGrRefPacketHeader_t *tag;
    u64 *samplingPacket;
    u64 *textureAlphaPacket;
    u64 *blendPacket;
    u64 *clampPacket;
    sdfGrPacketHeader_t *drawPacket;

    if (dds3NullDrawProc() == 0) {
        list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(list);
        tag = EE_ADDR_AS(sdfGrRefPacketHeader_t *, sdfGrAllocPacketBuff(0x20));
        sdfGrLinkDispFrameTex(list, EE_ADDR(&dds3DrawEnv[dds3GetFrameID()].stdEnv), tag);
        samplingPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x30));
        samplingPacket[0] = 2;
        samplingPacket[1] = 0x5000000210000000ULL;
        samplingPacket[2] = 0x1000000000008001ULL;
        samplingPacket[3] = 0xE;
        samplingPacket[4] = 0x61;
        samplingPacket[5] = 0x14;
        sdfDrawTagLink(list, EE_ADDR(samplingPacket));
        textureAlphaPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
        textureAlphaPacket[0] = 3;
        textureAlphaPacket[1] = 0x5000000310000000ULL;
        textureAlphaPacket[2] = 0x1000000000008002ULL;
        textureAlphaPacket[3] = 0xE;
        textureAlphaPacket[4] = 0x8000000080ULL;
        textureAlphaPacket[5] = 0x3B;
        textureAlphaPacket[6] = 0;
        textureAlphaPacket[7] = 0x3F;
        sdfDrawTagLink(list, EE_ADDR(textureAlphaPacket));
        blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
        blendPacket[0] = 3;
        blendPacket[1] = 0x5000000310000000ULL;
        blendPacket[2] = 0x1000000000008002ULL;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = source->blend;
        blendPacket[7] = 0x42;
        sdfDrawTagLink(list, EE_ADDR(blendPacket));
        clampPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x30));
        clampPacket[0] = 2;
        clampPacket[1] = 0x5000000210000000ULL;
        clampPacket[2] = 0x1000000000008001ULL;
        clampPacket[3] = 0xE;
        if (dds3BistaCheck() != 0) {
            clampPacket[4] = ((s64)(((447 - dds3Data33) >> 1) - 1) << 34) |
                             (((s64)(dds3DrawBistaData2 + 1) >> 1) << 24) | 9;
        } else {
            clampPacket[4] = 0x37C00000009ULL;
        }
        clampPacket[5] = 8;
        sdfDrawTagLink(list, EE_ADDR(clampPacket));
        drawPacket = effPkt2DAllocTriStrip(1, 0);
        effBlurBuildSamplingQuad(source, effPkt2DGetPrimPtr(drawPacket), 0);
        sdfDrawTagLink(list, EE_ADDR(drawPacket));
        effData3.regFunc(&effData3, list);
    }
}

/* Links the blend state and a quad of one colour between two corners. */
void dds3EffectFilter(dds3Effect_Filter_t *source) {
    sdfDrawTag_t *list;
    u64 *blendPacket;
    u64 *drawPacket;
    u64 color;
    u32 topLeft;
    u32 bottomLeft;
    u32 topRight;
    u32 bottomRight;
    s32 x[4];
    s32 y[4];

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = source->blend;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, EE_ADDR(blendPacket));

    drawPacket = EE_ADDR_AS(u64 *, effPkt2DAllocF4(0));
    color = source->abgr | 0x3F80000000000000ULL;
    x[0] = (source->xy[0][0] << 4) + 0x7000;
    x[1] = (source->xy[1][0] << 4) + 0x7000;
    y[0] = (source->xy[0][1] << 3) + 0x7900;
    y[1] = (source->xy[1][1] << 3) + 0x7900;
    topLeft = (u16)x[0] | ((u32)(u16)y[0] << 16);
    bottomLeft = (u16)x[0] | ((u32)(u16)y[1] << 16);
    topRight = (u16)x[1] | ((u32)(u16)y[0] << 16);
    bottomRight = (u16)x[1] | ((u32)(u16)y[1] << 16);
    drawPacket[5] = color;
    drawPacket[7] = color;
    drawPacket[9] = color;
    drawPacket[11] = color;
    drawPacket[6] = 0xFF000000000000ULL | topLeft;
    drawPacket[8] = 0xFF000000000000ULL | bottomLeft;
    drawPacket[10] = 0xFF000000000000ULL | topRight;
    drawPacket[12] = 0xFF000000000000ULL | bottomRight;
    sdfDrawTagLink(list, EE_ADDR(drawPacket));
    dds3Data9.regFunc(&dds3Data9, list);
}

void effResourceQuadDraw(effResourceRectDrawParams_t *params, sdfTexHandle_t *resource, u8 gsCoordinates) {
    sdfDrawTag_t *list;
    u64 *packet;
    sdfGrPacketHeader_t *drawPacket;
    effBlurPrimTriStrip_t *quad;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    packet = (u64 *)sdfGrAllocPacketBuff(0x40);
    packet[0] = 3;
    packet[1] = 0x5000000310000000ULL;
    packet[2] = 0x1000000000008002ULL;
    packet[3] = 14;
    packet[4] = 200705;
    packet[5] = 72;
    packet[6] = params->blendControl;
    packet[7] = 67;
    sdfDrawTagLink(list, (u32)packet);
    sdf2DLinkTexPacket(list, resource, 1);
    drawPacket = effPkt2DAllocTriStrip(1, 0x200);
    quad = effPkt2DGetPrimPtr(drawPacket);
    quad->r = params->color[0];
    quad->g = params->color[1];
    quad->b = params->color[2];
    quad->a = params->color[3];
    if (gsCoordinates == 0) {
        quad->vtx[0].x = (params->bounds.left << 4) + 0x7000;
        quad->vtx[0].y = (params->bounds.top << 3) + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = (params->bounds.bottom << 3) + 0x7900;
        quad->vtx[2].x = (params->bounds.right << 4) + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
    } else {
        quad->vtx[0].x = params->bounds.left + 0x7000;
        quad->vtx[0].y = params->bounds.top + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = params->bounds.bottom + 0x7900;
        quad->vtx[2].x = params->bounds.right + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
    }
    quad->vtx[0].s = 0.0f;
    quad->vtx[0].t = 0.0f;
    quad->vtx[1].s = 0.0f;
    quad->vtx[1].t = 1.0f;
    quad->vtx[2].s = 1.0f;
    quad->vtx[2].t = 0.0f;
    quad->vtx[3].s = 1.0f;
    quad->vtx[3].t = 1.0f;
    quad->vtx[0].z = 0;
    quad->vtx[0].adc = 0;
    quad->vtx[1].z = 0;
    quad->vtx[1].adc = 0;
    quad->vtx[2].z = 0;
    quad->vtx[2].adc = 0;
    quad->vtx[3].z = 0;
    quad->vtx[3].adc = 0;
    sdfDrawTagLink(list, (u32)drawPacket);
    dds3Data9.regFunc(&dds3Data9, list);
}

/* Clone rectangle parameters and select a fresh source handle. */
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam) {
    dds3Effect_PartialMultiFilter_t *dst = sdfChipAllocDirect(sizeof(dds3Effect_PartialMultiFilter_t));

    dst->hAlphaTex = dds3BillboardGetGeneralTex(0);
    memcpy(&dst->param, pParam, sizeof(*pParam));
    return dst;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBlur_Filter.c:713, match, same function 1.00 (words 5/5, calls 1.00, shape 1.00) */
void effBTLSysEncountDeleteSub(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter) {
    sdfChipFreeDirect(hPartialMultiFilter);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBlur_Filter.c:717, near, same function 0.98 (words 27/26, calls 1.00, shape 0.98) */
void effPCPMultiFilterExpDrawSub(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter) {
    s32 half = (s32)((f32)hPartialMultiFilter->param.area * 1.4f);
    s32 x = hPartialMultiFilter->param.center[0] + 0x100;
    s32 y = hPartialMultiFilter->param.center[1] + 0xE0;

    hPartialMultiFilter->param.Filter.bounds.left = x - half;
    hPartialMultiFilter->param.Filter.bounds.top = y - half;
    hPartialMultiFilter->param.Filter.bounds.right = x + half;
    hPartialMultiFilter->param.Filter.bounds.bottom = y + half;
    effResourceQuadDraw(&hPartialMultiFilter->param.Filter, hPartialMultiFilter->hAlphaTex, 0);
}

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTestBed);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3Str012402D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrDamageD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrMltestD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrBtest0D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTest2D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTestD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTestTmx2);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrWw02Tmx);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSmoke08Tmx);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSmoke07Tmx);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSmokeTmx);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSpview3P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSpview2P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSpviewP2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrMulti00P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrStage12P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrTestP2A2);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrMakatora020P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRing01D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRball01D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRball02D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrBball01D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki21BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki20BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki11BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki10BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki01BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki00BD3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki01D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrNiiki00D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrHouden0P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrDikira20P2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSse3D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSse2D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSse1D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSse0D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSsf3D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSsf2D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSsf1D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSsf0D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRingf2D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRingf1D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrRingf0D3P);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrSltfireP2A);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", dds3StrHcTestPb);

