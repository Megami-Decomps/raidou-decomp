#include "shared/interface/itfMisc/decls.h"

/* Emit an untextured two-corner GS sprite. */
void itfMiscDrawSpritePlanePrim(sdfIVector2_t *pos, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 1)));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_SPRITE_PRIMITIVE_BITS, 3, 0x551, 1);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
    dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
    dst += 2;
    dst[1] = (u64)z;
    dst[0] = (u64)(u32)(pos[0].x + ITF_GS_X_BIAS) | ((u64)(pos[0].y + ITF_GS_Y_BIAS) << 32);
    dst += 2;
    dst[1] = (u64)z;
    dst[0] = (u64)(u32)(pos[1].x + ITF_GS_X_BIAS) | ((u64)(pos[1].y + ITF_GS_Y_BIAS) << 32);
    sdfDrawTagLink(pdrawtag, EE_ADDR(packet));
}
