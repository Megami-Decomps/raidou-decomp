#include "common.h"
#include "ps2asm.h"
#include "sdf_lighting.h"
#include "sdf_resource.h"
#include "eeregs.h"
#include "sdk_libc.h"
#include "sdfPacketBuff.h"
#include "sdfDraw.h"
#include "sdfMat.h"

/* One frame of transformed matrix rows, normalized lighting rows and VIF codes. */
typedef struct sdf3DTmPacketData_t {
    u128 matrix[4];
    u128 scaledRows[3];
    u32 tag[4];
} sdf3DTmPacketData_t;

#define SDF_VIF_ITOP_LIGHTING 0x04000010

#define SDF_VIF_MSCAL_COMMAND 0x14000000

extern u8 sdf3DPrimIdentityTm[];
extern vu8 sdfFrameId;
extern f32 sdfFaceDivSoften;
extern f32 sdfFaceDivBias;
extern u32 sdf3DPrimDefaultFaceIndex[];


/* Native 0x2C VIF primitive request, shared by its quad/triangle producers; the
 * layout of libsdf.h's sdf3DPrimPolyParm_t, read here as raw words.
 * Attribute buffers carry serialized words; coordinate floats retain their bits.
 * Native callers supply a positive vertex count and aligned 16-byte position/normal rows. */
typedef struct sdf3DPrimPolyWords_t {
    s16 faceCount;
    s16 pointCount;
    u16 attr;
    u16 attrInhibit;
    u32 color;
    const void *faceIndex;
    const void *vertex;
    const void *normal;
    const void *uv;
    const void *multiUv;
    const void *vertexColor;
    void *(*allocator)(s32);
    f32 zBias;
} sdf3DPrimPolyWords_t;

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag,
                             s32 (*aAllocator)(s32));

void sdf3DPrimSetTmPacket(sdf3DTmPacketData_t *aPacket);

extern void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32));

extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32));

u32 *sdf3DPrimMakePoly(sdf3DPrimPolyWords_t *aParm);

void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag,
                             s32 (*aAllocator)(s32));

void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32));

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32));

void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag,
                             s32 (*aAllocator)(s32)) {
    u64 *referencePacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    referencePacket = EE_ADDR_AS(u64 *, aAllocator(0x20));
    referencePacket[0] = SDF_DMA_TAG(EE_ADDR(sdf3DPrimIdentityTm), 0x30000008);
    referencePacket[1] = 0x6C07C000ULL << 32;
    QUAD_ZERO(*(u128 *)&referencePacket[2]);
    sdfDrawTagLinkRef(aTag, EE_ADDR(referencePacket));
}

/* vu0 routine: store the resident matrix and inverse-column-length scaled rows, then VIF ITOP/MSCAL. */
void sdf3DPrimSetTmPacket(sdf3DTmPacketData_t *aPacket) {
    VU0_STORE_MATRIX_AND_UNIT_ROWS(aPacket);
    aPacket->tag[0] = SDF_VIF_ITOP_LIGHTING;
    aPacket->tag[1] = SDF_VIF_MSCAL_COMMAND;
    aPacket->tag[2] = 0;
    aPacket->tag[3] = 0;
}

void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32)) {
    u64 *dmaPacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    dmaPacket = EE_ADDR_AS(u64 *, aAllocator(0x90));
    dmaPacket[0] = SDF_DMA_TAG(EE_ADDR(dmaPacket + 2), 0x20000008);
    dmaPacket[1] = 0x6C07C000ULL << 32;
    sdf3DPrimSetTmPacket((sdf3DTmPacketData_t *)(dmaPacket + 2));
    sdfDrawTagLink(aTag, EE_ADDR(dmaPacket));
}

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32)) {
    u64 *referencePacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    sdfMatUpdatePacket(aMat, (s8)sdfFrameId);
    referencePacket = EE_ADDR_AS(u64 *, aAllocator(0x20));
    sdfMatMakePacket(aMat, (sdfDmaTag_t *)referencePacket, (s8)sdfFrameId);
    QUAD_ZERO(*(u128 *)&referencePacket[2]);
    sdfDrawTagLinkRef(aTag, EE_ADDR(referencePacket));
}

u32 *sdf3DPrimMakePoly(sdf3DPrimPolyWords_t *aParm) {
    s32 stripWords = aParm->faceCount;
    s32 count = aParm->pointCount;
    s32 headerWords = 18;
    s32 wordsPerVertex = 3;
    u32 primitive = aParm->attr | 0x68;
    u32 format = 0x60;
    s32 bytes;
    void *(*allocator)(s32);
    u32 *packet;
    u32 *cursor;
    const u128 *vector;
    const u32 *coordinates;
    const u32 *secondCoordinates;
    s32 i;
    u32 countCode;

    if (aParm->normal != NULL) {
        format = 0xE0;
        headerWords = 19;
        wordsPerVertex = 6;
    }
    if (aParm->uv != NULL) {
        format |= 0x100;
        primitive |= 0x10;
        headerWords++;
        wordsPerVertex += 2;
        if (aParm->multiUv != NULL) {
            primitive |= 0x1000;
            wordsPerVertex += 2;
        }
    }
    if (aParm->vertexColor != NULL) {
        format |= 0x200;
        primitive |= 0x800;
        headerWords++;
        wordsPerVertex++;
    }
    bytes = (headerWords + stripWords + wordsPerVertex * count) * 4;
    bytes = (bytes + 15) & ~15;
    allocator = aParm->allocator;
    if (allocator == NULL) allocator = sdfGrAllocPacketBuff;
    packet = allocator(bytes);
    *(u64 *)packet = (u64)(u16)((bytes >> 4) - 1) | 0x20000000ULL;
    packet[2] = 0x6102C000;
    packet[3] = (~aParm->attrInhibit) & 0xFFFF;
    packet[4] = 0x6E01C002;
    packet[5] = aParm->color;
    packet[6] = 0x6003C003;
    cursor = packet + 7;
    *cursor++ = (u32)aParm->zBias;
    *cursor++ = (u32)sdfFaceDivSoften;
    *cursor++ = (u32)sdfFaceDivBias;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)stripWords | ((u32)(u16)count << 16);
    *cursor++ = (primitive & 0xFFFF) | (format << 16);
    *cursor++ = (stripWords << 16) | 0x6E00C001;
    if (aParm->faceIndex == NULL) {
        memcpy(cursor, sdf3DPrimDefaultFaceIndex, stripWords * 4);
    } else {
        memcpy(cursor, aParm->faceIndex, stripWords * 4);
    }
    cursor += stripWords;
    stripWords++;
    countCode = count << 16;
    *cursor++ = countCode | stripWords | 0x6800C000;
    vector = aParm->vertex;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, *vector);
        cursor += 3;
        vector++;
        i++;
    } while (i != count);
    stripWords += count;
    vector = aParm->normal;
    if (vector != NULL) {
        *cursor++ = countCode | stripWords | 0x6800C000;
        i = 0;
        do {
            MMI_STORE_VEC3_VALUE(cursor, *vector);
            cursor += 3;
            vector++;
            i++;
        } while (i != count);
        stripWords += count;
    }
    coordinates = aParm->uv;
    if (coordinates != NULL) {
        secondCoordinates = aParm->multiUv;
        if (secondCoordinates == NULL) {
            *cursor++ = countCode | stripWords | 0x6400C000;
            memcpy(cursor, coordinates, count * 8);
            cursor += count * 2;
        } else {
            *cursor++ = countCode | stripWords | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *coordinates++;
                *cursor++ = *coordinates++;
                *cursor++ = *secondCoordinates++;
                *cursor++ = *secondCoordinates++;
                i++;
            } while (i != count);
        }
        stripWords += count;
    }
    if (aParm->vertexColor != NULL) {
        *cursor++ = countCode | stripWords | 0x6E00C000;
        memcpy(cursor, aParm->vertexColor, count * 4);
        cursor += count;
    }
    *cursor = 0x1400000C;
    while (EE_ADDR(++cursor) & 15) {
        *cursor = 0;
    }
    return packet;
}
