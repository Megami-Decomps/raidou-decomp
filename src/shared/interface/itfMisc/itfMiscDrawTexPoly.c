#include "shared/interface/itfMisc/decls.h"

/* Emit a textured triangle fan: each four-float input supplies S/T/Q only.
 * The fourth float is skipped, and the context word is shifted without masking. */
void itfMiscDrawTexPoly(sdfIVector2_t *pvertex, f32 *pstq, itfColor_t *pcolor, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, ITF_QUAD_VERTEX_COUNT)));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_TEXTURED_FAN_PRIMITIVE_BITS, 3, 0x512, ITF_QUAD_VERTEX_COUNT);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        f32 *uvDst = (f32 *)dst;

        uvDst[0] = pstq[0];
        uvDst[1] = pstq[1];
        uvDst[2] = pstq[2];
        pstq += 4;
        dst += 2;
        dst[0] = (u64)pcolor->components[0] | ((u64)pcolor->components[1] << 32);
        dst[1] = (u64)pcolor->components[2] | ((u64)pcolor->components[3] << 32);
        pcolor++;
        dst += 2;
        dst[1] = (u64)z;
        dst[0] = (u64)(u32)(pvertex->x + ITF_GS_X_BIAS) | ((u64)(pvertex->y + ITF_GS_Y_BIAS) << 32);
        pvertex++;
        dst += 2;
    }
    sdfDrawTagLink(pdrawtag, EE_ADDR(packet));
}
