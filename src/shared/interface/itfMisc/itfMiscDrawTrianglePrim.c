#include "shared/interface/itfMisc/decls.h"

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfMiscDrawTrianglePrim(sdfIVector2_t *pvertex, itfColor_t *pcolor, s32 x, s32 y, u32 z, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_TRIANGLE_VERTEX_COUNT)));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_PRIMITIVE_BITS, 2, 0x51, ITF_TRIANGLE_VERTEX_COUNT);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < ITF_TRIANGLE_VERTEX_COUNT; i++) {
        dst[0] = (u64)pcolor->components[0] | ((u64)pcolor->components[1] << 32);
        dst[1] = (u64)pcolor->components[2] | ((u64)pcolor->components[3] << 32);
        pcolor++;
        dst += 2;
        dst[1] = (u64)z;
        dst[0] = (u64)(u32)(pvertex->x + x + ITF_GS_X_BIAS) | ((u64)(pvertex->y + y + ITF_GS_Y_BIAS) << 32);
        pvertex++;
        dst += 2;
    }
    sdfDrawTagLink(pdrawtag, EE_ADDR(packet));
}
