#include "common.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "sdf2D.h"
#include "sdfPrim.h"

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

void evtSubmitTexturedGradientQuad(s32 x, s32 y, s32 width, s32 height, s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                                   s32 z, u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;

    list = dds3CallGrAllocPacketBuff9();
    sdf2DLinkTexPacket(list, texture, 0);
    sdfPrimLinkGt4(list, 0x40,
        (x << 4) + 0x7000, (y << 3) + 0x7900,
        (u << 4), (v << 4), color0,
        (x << 4) + (width << 4) + 0x7000, (y << 3) + 0x7900,
        (u << 4) + (textureWidth << 4), (v << 4), color1,
        (x << 4) + 0x7000, (y << 3) + (height << 3) + 0x7900,
        (u << 4), (v << 4) + (textureHeight << 4), color3,
        (x << 4) + (width << 4) + 0x7000, (y << 3) + (height << 3) + 0x7900,
        (u << 4) + (textureWidth << 4), (v << 4) + (textureHeight << 4), color2,
        z >> 4, NULL);
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->regFunc(surface, list);
}

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

INCLUDE_ASM(const s32, "code_00109BE8", evtSubmitTexturedGradientQuadPoints);

INCLUDE_ASM(const s32, "code_00109BE8", eglLinkBoxZ);
