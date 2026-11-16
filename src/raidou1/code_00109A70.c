#include "common.h"
#include "libsdf.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "sdf2D.h"
#include "sdfPrim.h"
#include "sdfPacketBuff.h"
#include "sdfDraw.h"
#include "dds3KernelMain.h"

void eglCallDLinkTexPacket(f32 x, f32 y, f32 width, f32 height, s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                           u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;

    list = dds3CallGrAllocPacketBuff9();
    sdf2DLinkTexPacket(list, texture, 0);
    sdfPrimLinkGt4(list, 0x40,
        (s32)(x * 16.0f) + 0x7000, (s32)(y * 8.0f) + 0x7900,
        (u << 4), (v << 4), color0,
        (s32)(x * 16.0f + width * 16.0f) + 0x7000, (s32)(y * 8.0f) + 0x7900,
        (u << 4) + (textureWidth << 4), (v << 4), color1,
        (s32)(x * 16.0f) + 0x7000, (s32)(y * 8.0f + height * 8.0f) + 0x7900,
        (u << 4), (v << 4) + (textureHeight << 4), color3,
        (s32)(x * 16.0f + width * 16.0f) + 0x7000, (s32)(y * 8.0f + height * 8.0f) + 0x7900,
        (u << 4) + (textureWidth << 4), (v << 4) + (textureHeight << 4), color2,
        -1, NULL);
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->regFunc(surface, list);
}

void eglCallDLinkTexPacket2(f32 x, f32 y, f32 width, f32 height, f32 u, f32 v, f32 textureWidth, f32 textureHeight,
                            u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;

    list = dds3CallGrAllocPacketBuff9();
    sdf2DLinkTexPacket(list, texture, 0);
    sdfPrimLinkGt4(list, 0x40,
        (s32)(x * 16.0f) + 0x7000, (s32)(y * 8.0f) + 0x7900,
        (s32)(u * 16.0f), (s32)(v * 16.0f), color0,
        (s32)(x * 16.0f + width * 16.0f) + 0x7000, (s32)(y * 8.0f) + 0x7900,
        (s32)((u + textureWidth) * 16.0f), (s32)(v * 16.0f), color1,
        (s32)(x * 16.0f) + 0x7000, (s32)(y * 8.0f + height * 8.0f) + 0x7900,
        (s32)(u * 16.0f), (s32)((v + textureHeight) * 16.0f), color3,
        (s32)(x * 16.0f + width * 16.0f) + 0x7000, (s32)(y * 8.0f + height * 8.0f) + 0x7900,
        (s32)((u + textureWidth) * 16.0f), (s32)((v + textureHeight) * 16.0f), color2,
        -1, NULL);
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->regFunc(surface, list);
}

INCLUDE_ASM(const s32, "code_00109A70", evtSubmitTexturedGradientQuad);

void LinkSprite4F(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, s32 u0, s32 v0, s32 u1, s32 v1,
                  s32 u2, s32 v2, s32 u3, s32 v3, s32 z, u32 c0, u32 c1, u32 c2, u32 c3, sdfTexHandle_t *th)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;

    list = dds3CallGrAllocPacketBuff9();
    sdf2DLinkTexPacket(list, th, 0);
    sdfPrimLinkGt4(list, 0x40,
        (s32)(x0 * 16.0f) + 0x7000, (s32)(y0 * 8.0f) + 0x7900, u0 * 16, v0 * 16, c0,
        (s32)(x1 * 16.0f) + 0x7000, (s32)(y1 * 8.0f) + 0x7900, u1 * 16, v1 * 16, c1,
        (s32)(x2 * 16.0f) + 0x7000, (s32)(y2 * 8.0f) + 0x7900, u2 * 16, v2 * 16, c3,
        (s32)(x3 * 16.0f) + 0x7000, (s32)(y3 * 8.0f) + 0x7900, u3 * 16, v3 * 16, c2,
        z >> 4, NULL);
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->regFunc(surface, list);
}

INCLUDE_ASM(const s32, "code_00109A70", evtSubmitTexturedGradientQuadPoints);

/* NON_MATCHING: 214 of 220 words match.  */
#ifdef NON_MATCHING
void eglLinkBoxZ(s32 x, s32 y, s32 w, s32 h, u32 depth, s32 color0, s32 color1, s32 color2, s32 color3) {
    s32 coords[8];
    sdfGrPacketHeader_t *packet;
    sdfDrawTag_t *command;
    u64 *dst;
    s32 i;
    s32 *pos;
    sdfGrOt_t *descriptor;
    u32 r0, g0, b0, a0, r1, g1, b1, a1, r3, g3, b3, a3, r2, g2, b2, a2;


    switch (dds3LinkBoxZSub()) {
    case 0:
        x *= 2;
        y *= 2;
        w *= 2;
        h *= 2;
        break;
    case 1:
        x *= 2;
        y *= 2;
        w *= 2;
        h *= 2;
        x -= 512;
        break;
    case 2:
        x *= 2;
        y *= 2;
        w *= 2;
        h *= 2;
        y -= 448;
        break;
    case 3:
        x *= 2;
        y *= 2;
        w *= 2;
        h *= 2;
        x -= 512;
        y -= 448;
        break;
    }

    r0 = color0 & 0xFF;
    g0 = (color0 >> 8) & 0xFF;
    b0 = (color0 >> 16) & 0xFF;
    a0 = (u32)color0 >> 24;
    r1 = color1 & 0xFF;
    g1 = (color1 >> 8) & 0xFF;
    b1 = (color1 >> 16) & 0xFF;
    a1 = (u32)color1 >> 24;
    r2 = color2 & 0xFF;
    g2 = (color2 >> 8) & 0xFF;
    b2 = (color2 >> 16) & 0xFF;
    a2 = (u32)color2 >> 24;
    r3 = color3 & 0xFF;
    g3 = (color3 >> 8) & 0xFF;
    b3 = (color3 >> 16) & 0xFF;
    a3 = (u32)color3 >> 24;

    coords[0] = x * 16;
    coords[1] = y * 8;
    coords[2] = (x + w) * 16;
    coords[3] = y * 8;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 8;
    coords[6] = x * 16;
    coords[7] = (y + h) * 8;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4)));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = depth;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    descriptor->regFunc(descriptor, command);
}
#else
INCLUDE_ASM(const s32, "code_00109A70", eglLinkBoxZ);
#endif
