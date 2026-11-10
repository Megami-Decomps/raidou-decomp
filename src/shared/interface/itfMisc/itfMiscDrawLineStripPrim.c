#include "shared/interface/itfMisc/decls.h"

/* Emit indexed RGBA/XYZ2 records with the line-strip primitive. */
void itfMiscDrawLineStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count)));
    sdf2DMakePrimPacketHeader(packet, ITF_LINE_STRIP_PRIMITIVE_BITS, 2, 0x51, count);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < count; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
}
