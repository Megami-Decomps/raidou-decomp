#include "common.h"
#include "draw.h"
#include "draw3d.h"
#include "kernel_draw.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdf_Decomp.h"
#include "sdf2D.h"
#include "sdfPacketBuff.h"
#include "sdfDraw.h"
#include "sdf3DPrim.h"
#include "sdfMat.h"
#include "sdk_libc.h"
#include "dds3KernelDraw_0010A750.h"
#include "sdfDevCons.h"
#include "sdfPrim.h"
#include "dds3KernelDraw.h"

/* One A+D register command with its standard DMA/GIF header. */
typedef struct
{
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t write;
} dds3GsCommand_t;

/* Each GIF reglist loop contains an RGBA register followed by an XYZ register. */
typedef struct
{
    u64 low;
    u64 high;
} dds3GouraudRegister_t;

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 color;
} dds3PrintControl_t;

/* Two-context Z-clear packets are 0xB0 bytes, two buffers per state. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGifTag_t testGif;
    sdfGrTagStdReg_t test;
    sdfGifTag_t clearGif;
    sdfGrTagStdReg_t colorA;
    sdfGrTagStdReg_t positionA;
    sdfGrTagStdReg_t colorB;
    sdfGrTagStdReg_t positionB;
    sdfGifTag_t blendGif;
    sdfGrTagStdReg_t blend[2];
} dds3ZClearPacket_t;

/* The fade sprite packet: a four-register state block then a textureless
 * sprite, 0xB0 bytes, one per frame buffer. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGifTag_t stateGif;
    sdfGrTagStdReg_t state[4];
    sdfGifTag_t spriteGif;
    sdfGrTagStdReg_t sprite[4];
} dds3FadePacket_t;

typedef struct dds3Filter2Param {
    /* 0x00 */ union { u8 rgba[4]; u32 color; };
    /* 0x04 */ s32 blend;
    /* 0x08 */ s32 rect[4];
} dds3Filter2Param_t;

typedef struct
{
    f32 fogMax;
    f32 fogMin;
    f32 fogNear;
    f32 fogFar;
} dds3FogRange_t;

typedef struct dds3MFilterParam {
    /* 0x00 */ s32 area[3];
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ s32 rect[4];
} dds3MFilterParam_t;

typedef struct dds3Blur2Param {
    /* 0x00 */ union { u8 rgba[4]; u32 color; };
    /* 0x04 */ s32 blend;
    /* 0x08 */ f32 rotation;
    /* 0x0C */ f32 scale;
    /* 0x10 */ s32 area[2];
    /* 0x18 */ s32 rect[4];
} dds3Blur2Param_t;

typedef struct dds3RipBlurBody {
    /* 0x00 */ s32 count;
    /* 0x04 */ f32 phaseStep;
    /* 0x08 */ f32 spacing;
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ f32 uvAngle;
    /* 0x18 */ f32 uvAmplitude;
    /* 0x1C */ f32 angleStep;
    /* 0x20 */ s32 x;
    /* 0x24 */ s32 y;
    /* 0x28 */ s32 area;
} dds3RipBlurBody_t __attribute__((aligned(8)));

typedef struct dds3RipBlurParam {
    /* 0x00 */ dds3RipBlurBody_t ripple;
    /* 0x2C */ u32 unk2C;
    /* 0x30 */ dds3Blur2Param_t blur2buf;
} dds3RipBlurParam_t;

typedef struct dds3MultiBlurParam {
    /* 0x00 */ s32 extent;
    /* 0x04 */ union { u8 rgba[4]; u32 color; };
    /* 0x08 */ s32 blend;
    /* 0x0C */ f32 rotation;
    /* 0x10 */ f32 scale;
    /* 0x14 */ s32 x;
    /* 0x18 */ s32 y;
    /* 0x1C */ s32 rect[4];
} dds3MultiBlurParam_t;

typedef struct dds3DistBlurParam {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 delaySpread;
    /* 0x08 */ f32 angleStep;
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ f32 uvAngle;
    /* 0x18 */ f32 uvAmplitude;
    /* 0x1C */ s32 x;
    /* 0x20 */ s32 y;
    /* 0x24 */ s32 spread;
    /* 0x28 */ s32 size;
} dds3DistBlurParam_t;

extern u16 mulblurnow; extern u16 mulblurmax; extern u16 mulblurabase;
extern u16 mulbluratarget; extern u16 mulblurswapnow; extern u16 mulblurswapmax;
extern u16 distblurnow; extern u16 distblurmax; extern u16 distblurabase;
extern u16 distbluratarget; extern u16 distblurswapnow; extern u16 distblurswapmax;
extern u16 ripblurnow; extern u16 ripblurmax; extern u16 ripblurabase;
extern u16 ripbluratarget; extern u16 ripblurswapnow; extern u16 ripblurswapmax;
extern u16 fogparanow; extern u16 fogparamax;
extern f32 sdfFaceDivSoften; extern f32 sdfFaceDivBias;
extern u16 sbnow; extern u16 sbmax;
extern f32 softenbase; extern f32 softentarget;
extern f32 biasbase; extern f32 biastarget;
extern sdfMat_t *eglMaterial;
sdfMat_t *sdfMatNew(void);
void *memcpy(void *a, const void *b, size_t c);
extern u32 egl_OT_Offset;

/* The quake's two axis gains are copied in by value, which is the unaligned
 * eight-byte block move at the entry. */
typedef struct
{
    f32 axis[2];
} dds3QuakeGain_t;

extern sdfGrOt_t sdfGrOtTable[];
extern u64 eglPolyStripWord[];
extern sdfVector4_t eglLineVerts[];
extern u32 eglLineColors[];
extern u16 ambientnow, ambientmax;
extern u32 ambientbase, ambienttarget;
extern u16 fogrgbnow, fogrgbmax;
extern u32 fogrgbbase, fogrgbtarget;
extern dds3MultiBlurParam_t key2;

/* SDK float-input color conversion, shared by the six light/ambient/fog inputs. */
static inline void dds3UnpackLightColor(u32 color, f32 unit) {
    u32 rgba[4] __attribute__((aligned(16)));
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

void eglLinkTri(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 rgba0, s32 rgba1, s32 rgba2);
void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 color0, u32 color1, u32 color2, u32 color3);
void eglLinkTexPoly4(s32 color0, f32 x0, f32 y0, f32 z0, s32 color1, f32 x1, f32 y1, f32 z1, s32 color2, f32 x2, f32 y2, f32 z2, s32 color3, f32 x3, f32 y3, f32 z3, sdfTexHandle_t *texture, f32 u0, f32 v0, f32 u1, f32 v1);
void eglLinkLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 color0, u32 color1);

#include "event.h"
#include "scr.h"
#include "sdf_packet_alloc.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "kernel.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "font.h"
#include "effect.h"

enum {
    EVT_PACKET_LIST_BYTES = 0x20,
    EVT_GS_COMMAND_BYTES = 0x30,
    EVT_GS_TEST_PRIMARY = 0x47,
    EVT_GS_TEST_SECONDARY = 0x48,
    EVT_GS_ALPHA_PRIMARY = 0x42,
    EVT_FIXED_ALPHA_INTERPOLATION = 0x64,
    EVT_QUAD_DEFAULT_DEPTH = 0xFFFFFF,
    EVT_FRAME_REFERENCE_PACKET_BYTES = 0x40,
    EVT_FRAME_DRAW_RECORD_BYTES = 0x1F40,
    EVT_BACKGROUND_BLEND_FLAG = 0x100,
    EVT_DRAW_COLOR_BLEND_FLAG = 0x200,
    EVT_DRAW_VECTOR_BLEND_FLAG = 0x400,
    EVT_SAVED_VECTOR_BLEND_FLAG = 0x4000,
    EVT_QUAD_DESCRIPTOR_BYTES = 0x2C,
    EVT_QUAD_VERTEX_COUNT = 4,
    EVT_SELECTION_ALLOCATION_BYTES = 0x28,
    EVT_DEV_CONSOLE_BUFFER_BYTES = 0x200,
    BF_FLW0_MAGIC = 0x30574C46
};

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

/* The allocated draw state's float at +0x1C is initialized to 1. */
typedef struct evtDrawState_t {
    u8 pad00[0x1C];
    f32 value1C;
} evtDrawState_t;

typedef struct dds3ResourceNode_t {
    s32 unk0;
    struct dds3ResourceNode_t *next;
    s32 *ready;
} dds3ResourceNode_t;

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct evtGsCommand_t {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} evtGsCommand_t;

/* Native rectangle emitter: x/y/width/height, explicit depth, then TL/TR/BR/BL colors. */
void evtSubmitGradientRectAtDepth(s32 x, s32 y, s32 w, s32 h, u32 depth, s32 color0, s32 color1, s32 color2, s32 color3);
void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A750", eglLinkBox);

/* Submit the four-corner gradient rectangle with the native default depth.
   The inserted 0xFFFFFF is depth, not a white color. */
void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor) {
    evtSubmitGradientRectAtDepth(x, y, width, height, EVT_QUAD_DEFAULT_DEPTH, topLeftColor, topRightColor, bottomRightColor, bottomLeftColor);
}

void dds3CallGrAllocPacketBuff5(s32 c0, s32 c1, s32 c2, s32 c3, f32 x, f32 y, f32 w, f32 h)
{
    s32 coords[4][2];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    dds3GouraudRegister_t *registers;
    s32 i;
    sdfGrOt_t *descriptor;
    u32 r0, g0, b0, a0;
    u32 r1, g1, b1, a1;
    u32 b3, r2, g2, b2, a2;
    u32 r3, g3, a3;

    r0 = c0 & 0xFF;
    g0 = (c0 >> 8) & 0xFF;
    b0 = (c0 >> 16) & 0xFF;
    a0 = (u32)c0 >> 24;
    r1 = c1 & 0xFF;
    g1 = (c1 >> 8) & 0xFF;
    b1 = (c1 >> 16) & 0xFF;
    a1 = (u32)c1 >> 24;
    r2 = c2 & 0xFF;
    g2 = (c2 >> 8) & 0xFF;
    b2 = (c2 >> 16) & 0xFF;
    a2 = (u32)c2 >> 24;
    r3 = c3 & 0xFF;
    g3 = (c3 >> 8) & 0xFF;
    b3 = (c3 >> 16) & 0xFF;
    a3 = (u32)c3 >> 24;
    coords[0][0] = x * 16.0f;
    coords[0][1] = y * 8.0f;
    coords[1][0] = (x + w) * 16.0f;
    coords[1][1] = y * 8.0f;
    coords[2][0] = (x + w) * 16.0f;
    coords[2][1] = (y + h) * 8.0f;
    coords[3][0] = x * 16.0f;
    coords[3][1] = (y + h) * 8.0f;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    registers = (dds3GouraudRegister_t *)sdf2DGetPrimPtr(packet);
    for (i = 0; i < 4; i++) {
        if (i == 0) {
            registers->low = (u64)r0 | ((u64)g0 << 32);
            registers->high = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            registers->low = (u64)r1 | ((u64)g1 << 32);
            registers->high = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            registers->low = (u64)r3 | ((u64)g3 << 32);
            registers->high = (u64)b3 | ((u64)a3 << 32);
        } else {
            registers->low = (u64)r2 | ((u64)g2 << 32);
            registers->high = (u64)b2 | ((u64)a2 << 32);
        }
        registers++;
        registers->high = 0xFFFFFF;
        registers->low = (u64)(u32)(coords[i][0] + 0x7000) |
            ((u64)(coords[i][1] + 0x7900) << 32);
        registers++;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void eglLinkTri(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 rgba0, s32 rgba1, s32 rgba2)
{
    s32 coords[3][2];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    dds3GouraudRegister_t *registers;
    s32 i;
    sdfGrOt_t *descriptor;

    u32 r0 = rgba0 & 0xFF;
    u32 g0 = (rgba0 >> 8) & 0xFF;
    u32 b0 = (rgba0 >> 16) & 0xFF;
    u32 a0 = (u32)rgba0 >> 24;
    u32 r1 = rgba1 & 0xFF;
    u32 g1 = (rgba1 >> 8) & 0xFF;
    u32 b1 = (rgba1 >> 16) & 0xFF;
    u32 a1 = (u32)rgba1 >> 24;
    u32 r2 = rgba2 & 0xFF;
    u32 g2 = (rgba2 >> 8) & 0xFF;
    u32 b2 = (rgba2 >> 16) & 0xFF;
    u32 a2 = (u32)rgba2 >> 24;
    coords[0][0] = x0 * 16;
    coords[0][1] = y0 * 8;
    coords[1][0] = x1 * 16;
    coords[1][1] = y1 * 8;
    coords[2][0] = x2 * 16;
    coords[2][1] = y2 * 8;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 3));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 3);
    registers = (dds3GouraudRegister_t *)sdf2DGetPrimPtr(packet);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            registers->low = (u64)r0 | ((u64)g0 << 32);
            registers->high = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            registers->low = (u64)r1 | ((u64)g1 << 32);
            registers->high = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            registers->low = (u64)r2 | ((u64)g2 << 32);
            registers->high = (u64)b2 | ((u64)a2 << 32);
        }
        registers++;
        registers->high = 0xFFFFFF;
        registers->low = (u64)(u32)(coords[i][0] + 0x7000) |
            ((u64)(coords[i][1] + 0x7900) << 32);
        registers++;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A750", dds3CallGrAllocPacketBuff);

#endif
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A750", eglLinkMaru2D);

void fldDrwZLoad(void)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *ot;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    ot = &sdfGrOtTable[egl_OT_Offset];
    ot->regFunc(ot, list);
}

extern sdfMat_t *eglMaterial;

void dds3CallGrAllocPacketBuff6(s32 x0, s32 y0, s32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2,
                         f32 col0, f32 col1, f32 col2) {
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t vertices[3];
    u32 colors[3];
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    sdf3DPrimLinkMat(command, eglMaterial, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.faceCount = 1;
    desc.pointCount = 3;
    desc.vertex = vertices;
    desc.vertexColor = colors;
    vertices[0].x = x1;
    vertices[0].y = y1;
    vertices[0].z = z1;
    vertices[1].x = x2;
    vertices[1].y = y2;
    vertices[1].z = z2;
    vertices[2].x = col0;
    vertices[2].y = col1;
    vertices[2].z = col2;
    colors[0] = x0;
    colors[1] = y0;
    colors[2] = z0;
    sdfDrawTagLink(command, EE_ADDR(sdf3DPrimMakePoly(&desc)));
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    descriptor->regFunc(descriptor, command);
}

/* Preserve native vertex order 0,2,3,1; W components are not initialized. */
void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 color0, u32 color1, u32 color2, u32 color3)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    void *list;
    sdfGrOt_t *surface;

    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, eglMaterial, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = eglPolyStripWord[0];
    sdfDrawTagLink(list, EE_ADDR(sdf3DPrimMakePoly(&desc)));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelDraw_0010A750.c:402, near, same function 0.96 (words 113/116, calls 0.92, shape 0.99) */
void dds3CallGrAllocPacketBuff7(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 color0, u32 color1, u32 color2, u32 color3)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    void *list;
    sdfGrOt_t *surface;

    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkMat(list, eglMaterial, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = eglPolyStripWord[0];
    sdfDrawTagLink(list, EE_ADDR(sdf3DPrimMakePoly(&desc)));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

/* Preserve native vertex order 0,2,3,1; W components are not initialized. */
void eglLinkTexPoly4(s32 color0, f32 x0, f32 y0, f32 z0, s32 color1, f32 x1, f32 y1, f32 z1, s32 color2, f32 x2, f32 y2, f32 z2, s32 color3, f32 x3, f32 y3, f32 z3, sdfTexHandle_t *texture, f32 u0, f32 v0, f32 u1, f32 v1)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    sdfVector2_t uvs[4];
    sdfMat_t *asset;
    void *list;
    sdfGrOt_t *surface;

    asset = sdfMatNew();
    sdfMatSetTex(asset, texture);
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, asset, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    desc.uv = uvs;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = eglPolyStripWord[0];
    uvs[0].x = u0;
    uvs[0].y = v0;
    uvs[1].x = u0;
    uvs[1].y = v1;
    uvs[2].x = u1;
    uvs[2].y = v1;
    uvs[3].x = u1;
    uvs[3].y = v0;
    sdfDrawTagLink(list, EE_ADDR(sdf3DPrimMakePoly(&desc)));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    sdfMatFreeDelayed(asset);
}

/* NON_MATCHING: 131 of 137 words match.  */
#ifdef NON_MATCHING
/* body: matched in raidou @ 88f0d54: src/shared/kernel/dds3KernelDraw_0010A750.c:447, near, same function 0.97 (words 137/140, calls 0.95, shape 0.99) */
void dds3CallMatNew(s32 color0, f32 x0, f32 y0, f32 z0, s32 color1, f32 x1, f32 y1, f32 z1, s32 color2, f32 x2, f32 y2, f32 z2, s32 color3, f32 x3, f32 y3, f32 z3, sdfTexHandle_t *texture, f32 u0, f32 v0, f32 u1, f32 v1)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    sdfVector2_t uvs[4];
    sdfMat_t *asset;
    void *list;
    sdfGrOt_t *surface;

    asset = sdfMatNew();
    sdfMatSetTex(asset, texture);
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkMat(list, asset, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    desc.uv = uvs;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = eglPolyStripWord[0];
    uvs[0].x = u0;
    uvs[0].y = v0;
    uvs[1].x = u0;
    uvs[1].y = v1;
    uvs[2].x = u1;
    uvs[2].y = v1;
    uvs[3].x = u1;
    uvs[3].y = v0;
    sdfDrawTagLink(list, EE_ADDR(sdf3DPrimMakePoly(&desc)));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    sdfMatFreeDelayed(asset);
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A750", dds3CallMatNew);
#endif

sdfMat_t *dds3CallMatNew2(sdfTexHandle_t *tex)
{
    sdfMat_t *mat;

    mat = sdfMatNew();
    sdfMatSetTex(mat, tex);
    return mat;
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010BAF0.c:1, match */
void dds3FileLoadSub(struct sdfMat_t *file) {
    sdfMatFreeDelayed(file);
}

/* NON_MATCHING: 115 of 130 words match.  */
#ifdef NON_MATCHING
void dds3CallGrAllocPacketBuff8(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3,
                                f32 z3, u32 color0, u32 color1, u32 color2, u32 color3, sdfMat_t *mat, f32 u0,
                                f32 v0, f32 u1, f32 v1)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    sdfVector2_t uvs[4];
    void *list;
    sdfGrOt_t *surface;

    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkMat(list, mat, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    desc.uv = uvs;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = eglPolyStripWord[0];
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    uvs[0].x = u0;
    uvs[0].y = v0;
    uvs[1].x = u0;
    uvs[1].y = v1;
    uvs[2].x = u1;
    uvs[2].y = v1;
    uvs[3].x = u1;
    uvs[3].y = v0;
    sdfDrawTagLink(list, EE_ADDR(sdf3DPrimMakePoly(&desc)));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A750", dds3CallGrAllocPacketBuff8);
#endif

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A750", dds3CallGrAllocPacketBuff2);

#endif
void eglLinkLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 color0, u32 color1)
{
    void *list;
    eglLineVerts[0].x = x0;
    eglLineVerts[0].y = y0;
    eglLineVerts[0].z = z0;
    eglLineVerts[1].x = x1;
    eglLineVerts[1].y = y1;
    eglLineVerts[1].z = z1;
    eglLineColors[1] = color1;
    eglLineColors[0] = color0;
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, EE_ADDR(effDebugDrawWireModel(eglLineVerts, eglLineColors, 2, 0x80)));
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    }
}

void eglLinkPrint(s32 x, s32 y, s32 col, const char *str)
{
    sdfDevPrintCB_t printCB;
    sdfDrawTag_t *list;
    sdfPrimTransPacket_t *packet;

    list = dds3CallGrAllocPacketBuff9();
    packet = sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, packet);
    sdfDevSetPrintCB(&printCB, x * 16 + 0x7000, y * 8 + 0x7900, 0x0FFFFF80, col);
    sdfDrawTagLink(list, sdfDevPrintfEx(&printCB, str));
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A750", eglLinkKanji);
