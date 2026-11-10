#include "shared/interface/itfMisc/decls.h"

/* Emit a colored textured sprite without binding a texture in this function. */
void itfMiscDrawSpritePrim2(sdfIVector2_t *pos, itfColor_t *uv, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1)));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_COLORED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = pos[0].x + ITF_GS_X_BIAS;
    y0 = pos[0].y + ITF_GS_Y_BIAS;
    x1 = pos[1].x + ITF_GS_X_BIAS;
    y1 = pos[1].y + ITF_GS_Y_BIAS;
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
    dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)z;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)z;
    sdfDrawTagLink(pdrawtag, EE_ADDR(packet));
}
