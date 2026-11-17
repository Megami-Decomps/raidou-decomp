#include "common.h"
#include "sdfDevCons.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "effect.h"
#include "kernel.h"
#include "kernel_draw.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "dds3GlobalWork.h"
#include "effMath.h"
#include "effPacket.h"
#include "sdf3DPrim.h"
#include "sdfVu0.h"
#include "sdf_Decomp.h"
#include "sdfMath.h"
#include "sdk_libc.h"
#include "sdfMat.h"

/* Screen pixel pair in the debug overlay's own 1/16 pixel grid. */
typedef struct
{
    s32 x;
    s32 y;
} effDebugPoint_t;

/* Matrix view for the VU0 matrix load macros' memory operand. */
typedef struct
{
    f32 rows[4][4];
} effDebugMatrix_t;

extern sdfMat_t *effDebugSelectCursorMat;
extern f32 effDebugSelectCursorAngle;
extern sdfVector4_t effDebugTriPositions[4];
extern u32 effDebugTriColors[4];

void effDebugPointDraw(sdfVector4_t *pPos)
{
    sdfVector4_t screen;
    effDebugPoint_t point;
    void *tag;
    sdfGrOt_t *ot;

    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    SDF_VU0_LOADVECTOR(pPos);
    effScreenOffset();
    SDF_VU0_STOREVECTOR_MEMOPS(screen);
    point.x = (s32)screen.x - 0x700;
    point.y = ((s32)screen.y << 1) - 0xF20;
    sdfDrawTagLink(tag, (addr_t)dds3PrimFrameRect(point.x * 16 + DDS3_SCREEN_OFFX, point.y * 8 + DDS3_SCREEN_OFFY,
                                          0xFF0000, 0x20, 0x10, 0x60008080, 0x60008080));
    ot = &sdfGrOtTable[DDS3_OT_DEBUG];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

void effDebugPointDraw2(sdfVector4_t *pPos, u32 color)
{
    sdfVector4_t screen;
    effDebugPoint_t point;
    void *tag;
    sdfGrOt_t *ot;

    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    SDF_VU0_LOADVECTOR(pPos);
    effScreenOffset();
    SDF_VU0_STOREVECTOR_MEMOPS(screen);
    point.x = (s32)screen.x - 0x700;
    point.y = ((s32)screen.y << 1) - 0xF20;
    sdfDrawTagLink(tag, (addr_t)dds3PrimFrameRect(point.x * 16 + DDS3_SCREEN_OFFX, point.y * 8 + DDS3_SCREEN_OFFY,
                                          0xFF0000, 0x20, 0x10, color, color));
    ot = &sdfGrOtTable[DDS3_OT_DEBUG];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

void effDebugLineDraw(sdfVector4_t *pPos0, sdfVector4_t *pPos1)
{
    sdfVector4_t screen[2];
    effDebugPoint_t point[2];
    void *tag;
    sdfGrOt_t *ot;

    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    SDF_VU0_LOADVECTOR(pPos0);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&screen[0]);
    SDF_VU0_LOADVECTOR(pPos1);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&screen[1]);
    point[0].x = (s32)screen[0].x - 0x700;
    point[0].y = ((s32)screen[0].y << 1) - 0xF20;
    point[1].x = (s32)screen[1].x - 0x700;
    point[1].y = ((s32)screen[1].y << 1) - 0xF20;
    sdfDrawTagLink(tag, (addr_t)dds3PrimLine(point[0].x * 16 + DDS3_SCREEN_OFFX, point[0].y * 8 + DDS3_SCREEN_OFFY,
                                     0xFF0000, 0x60008080,
                                     point[1].x * 16 + DDS3_SCREEN_OFFX, point[1].y * 8 + DDS3_SCREEN_OFFY,
                                     0xFF0000, 0x60008080, 0));
    ot = &sdfGrOtTable[DDS3_OT_DEBUG];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

void effDebugLineDraw2(sdfVector4_t *from, sdfVector4_t *to, u32 mode)
{
    sdfVector4_t screen[2];
    effDebugPoint_t point[2];
    void *tag;
    sdfGrOt_t *ot;

    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    SDF_VU0_LOADVECTOR(from);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&screen[0]);
    SDF_VU0_LOADVECTOR(to);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&screen[1]);
    point[0].x = (s32)screen[0].x - 0x700;
    point[0].y = ((s32)screen[0].y << 1) - 0xF20;
    point[1].x = (s32)screen[1].x - 0x700;
    point[1].y = ((s32)screen[1].y << 1) - 0xF20;
    sdfDrawTagLink(tag, (addr_t)dds3PrimLine(point[0].x * 16 + DDS3_SCREEN_OFFX, point[0].y * 8 + DDS3_SCREEN_OFFY,
                                     0xFF0000, mode,
                                     point[1].x * 16 + DDS3_SCREEN_OFFX, point[1].y * 8 + DDS3_SCREEN_OFFY,
                                     0xFF0000, mode, 0));
    ot = &sdfGrOtTable[DDS3_OT_DEBUG];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

void effDebugStringDraw(s32 x, s32 y, s32 commandIndex, const char *format) {
    sdfDrawTag_t *task = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *scene;

    sdfDrawTagInit(task);
    sdfDrawTagLink(task, (addr_t)sdfDevPrintf((x << 4) + DDS3_SCREEN_OFFX, (y << 3) + DDS3_SCREEN_OFFY, 0xFF0000, commandIndex, format));
    scene = &sdfGrOtTable[DDS3_OT_DEBUG];
    ((void (*)(sdfGrOt_t *, void *))scene->regFunc)(scene, task);
}

void effDebugWireframeSphereDraw(const f32 *center, u32 color, f32 radius) {
    f32 point[4] __attribute__((aligned(16)));
    f32 matrix[4][4] __attribute__((aligned(16)));
    sdfVector4_t vertices[40];
    u32 colors[40];
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    sdfVector4_t *out;
    f32 latitude;
    f32 longitude;
    f32 ringRadius;
    f32 ringY;
    u32 pass;
    u32 ring = 0;
    u32 segment;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    do {
        colors[ring++] = color;
    } while (ring < 40);

    SDF_VU0_STOREIDENTITY(matrix);
    for (pass = 0; pass < 2; pass++) {
        latitude = 0.0f;
        for (ring = 0; ring < 14; ring++) {
            latitude += 0.20943949f;
            out = vertices;
            longitude = 0.0f;
            ringY = radius * sdfCos(latitude);
            ringRadius = radius * sdfSin(latitude);
            SDF_VU0_LOADMATRIX(matrix);
            SDF_VU0_LOADVECTOR_B( center);
            point[0] = ringRadius;
            point[1] = ringY;
            point[2] = 0.0f;
            SDF_VU0_LOADVECTOR( point);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();

            for (segment = 0; segment < 20; segment++) {
                SDF_VU0_STOREVECTOR(out);
                out++;
                longitude += 0.31415924429893494f;
                point[0] = sdfCos(longitude) * ringRadius;
                point[1] = ringY;
                point[2] = sdfSin(longitude) * ringRadius;
                SDF_VU0_LOADVECTOR( point);
                SDF_VU0_VECTOR_W_SET_ZERO();
                SDF_VU0_APPLYMATRIX();
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(out);
                out++;
            }
            sdfDrawTagLink(list, (addr_t)effDebugDrawWireModel(vertices, colors, 40, 0x80));
        }
        SetRotateMatrixX(1.5707963f);
        SDF_VU0_LOADMATRIX_B_MEMOPS(matrix);
        MultMatrixNoArg();
        SDF_VU0_STOREMATRIX_NOMEM(matrix);
    }
    surface = &sdfGrOtTable[DDS3_OT_DEBUG_WIRE];
    ((void (*)(sdfGrOt_t *, void *))surface->regFunc)(surface, list);
}

void effDebugSelectCursorDraw(f32 *position) {
    sdfMat_t *asset = effDebugSelectCursorMat;
    sdfDrawTag_t *list;
    sdf3DPrimPolyParm_t packet;
    sdfGrOt_t *surface;

    if (asset == NULL) {
        asset = sdfMatNew();
        effDebugSelectCursorMat = asset;
        sdfMatSetConstant(asset, 1.0f);
    }

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    SetRotateMatrixY(effDebugSelectCursorAngle);
    effDebugSelectCursorAngle += 0.10471974313259125f;
    SDF_VU0_LOADVECTOR( position);
    VU0_ADD_VF31_VF10_XYZ();
    sdf3DPrimLinkTm(list, NULL);
    memset(&packet, 0, sizeof(packet));
    packet.attr = 0x4000;
    effDebugTriPositions[2].z = 0.0f;
    packet.vertexColor = effDebugTriColors;
    packet.color = 0x80808080;

    effDebugTriPositions[0].x = 0.0f;
    effDebugTriPositions[0].y = 0.0f;
    effDebugTriPositions[0].z = 0.0f;
    effDebugTriPositions[1].x = 15.0f;
    effDebugTriPositions[1].y = -30.0f;
    effDebugTriPositions[1].z = 0.0f;
    effDebugTriPositions[2].x = -15.0f;
    effDebugTriPositions[2].y = -30.0f;

    packet.faceCount = 1;
    packet.pointCount = 3;
    packet.vertex = effDebugTriPositions;
    packet.uv = NULL;
    sdf3DPrimLinkMat(list, effDebugSelectCursorMat, NULL);
    sdfDrawTagLink(list, (addr_t)effPkt3DPrimMakePoly(&packet));
    surface = &sdfGrOtTable[44];
    ((void (*)(sdfGrOt_t *, void *))surface->regFunc)(surface, list);
}

void effDebugWireframeCylinderDraw(const f32 *center, u32 color, f32 radius, f32 height) {
    sdfVector4_t point; /* The line provider consumes xyz; preserve the untouched w lane. */
    sdfVector4_t vertices[40];
    u32 colors[40];
    sdfDrawTag_t *list;
    sdfVector4_t *out;
    sdfGrOt_t *surface;
    u32 ring;
    u32 segment;
    f32 y;
    f32 heightStep;
    f32 angleStep;
    f32 angle;
    f32 oppositeAngle;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    for (ring = 0; ring < 40; ring++) {
        colors[ring] = color;
    }
    heightStep = height / 19.0f;
    angleStep = 0.31415924429893494f;
    y = -height * 0.5f;
    for (ring = 0; ring < 20; ring++) {
        angle = 0.0f;
        out = vertices;
        point.x = radius;
        point.y = y;
        point.z = 0.0f;
        SDF_VU0_LOADVECTOR_B( center);
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        for (segment = 0; segment < 20; segment++) {
            SDF_VU0_STOREVECTOR(out);
            out++;
            angle += angleStep;
            point.x = sdfCos(angle) * radius;
            point.y = y;
            point.z = sdfSin(angle) * radius;
            SDF_VU0_LOADVECTOR( &point);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(out);
            out++;
        }
        sdfDrawTagLink(list, (addr_t)effDebugDrawWireModel(vertices, colors, 40, 0x80));
        y += heightStep;
    }

    out = vertices;
    SDF_VU0_LOADVECTOR_B( center);
    angle = 0.0f;
    for (segment = 0; segment < 20; segment++) {
        point.x = sdfCos(angle) * radius;
        point.y = -height * 0.5f;
        point.z = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        point.x = sdfCos(angle) * radius;
        point.y = height * 0.5f;
        point.z = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, (addr_t)effDebugDrawWireModel(vertices, colors, 40, 0x80));

    out = vertices;
    SDF_VU0_LOADVECTOR_B( center);
    angle = 0.0f;
    for (segment = 0; segment < 10; segment++) {
        point.x = sdfCos(angle) * radius;
        point.y = -height * 0.5f;
        point.z = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        oppositeAngle = angle + 3.1415926f;
        point.x = sdfCos(oppositeAngle) * radius;
        point.y = -height * 0.5f;
        point.z = sdfSin(oppositeAngle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, (addr_t)effDebugDrawWireModel(vertices, colors, 20, 0x80));

    out = vertices;
    SDF_VU0_LOADVECTOR_B( center);
    angle = 0.0f;
    for (segment = 0; segment < 10; segment++) {
        point.x = sdfCos(angle) * radius;
        point.y = height * 0.5f;
        point.z = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        oppositeAngle = angle + 3.1415926f;
        point.x = sdfCos(oppositeAngle) * radius;
        point.y = height * 0.5f;
        point.z = sdfSin(oppositeAngle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, (addr_t)effDebugDrawWireModel(vertices, colors, 20, 0x80));
    surface = &sdfGrOtTable[DDS3_OT_DEBUG_WIRE];
    ((void (*)(sdfGrOt_t *, void *))surface->regFunc)(surface, list);
}

void effDebugWindowDraw(s32 x, s32 y, s32 width, s32 height, s32 fillColor, s32 borderColor) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *scene;

    sdfDrawTagInit(list);
    sdfDrawTagLink(list, (addr_t)dds3PrimFrameRect(x * 0x10 + DDS3_SCREEN_OFFX, y * 8 + DDS3_SCREEN_OFFY, 0xFF0000, width * 0x10, height * 8, fillColor, borderColor));
    scene = &sdfGrOtTable[DDS3_OT_DEBUG];
    ((void (*)(sdfGrOt_t *, void *))scene->regFunc)(scene, list);
}
INCLUDE_SDATA(const s32, "effect/effDebug", effDebugSelectCursorMat);

INCLUDE_SDATA(const s32, "effect/effDebug", effDebugSelectCursorAngle);

INCLUDE_SDATA(const s32, "effect/effDebug", dds3EffectFileNameTableEntry);

