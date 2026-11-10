#include "shared/effect/polyManager/decls.h"

void dds3PolygonTriangleSet(dds3Polygon_t *hPolygon, const f32 *center) {
    s32 count = hPolygon->drawCount;
    f32 point[4] __attribute__((aligned(16)));
    f32 (*positions)[4];
    u16 flags;

    SDF_VU0_COPYVECTOR(point, center);
    positions = (f32 (*)[4])(((u32)count << 4) + EE_ADDR(hPolygon->pVertex));
    SDF_VU0_COPYVECTOR(positions[0], point);
    point[1] += 5.0f;
    SDF_VU0_COPYVECTOR(positions[1], point);
    point[2] += 5.0f;
    SDF_VU0_COPYVECTOR(positions[2], point);
    flags = hPolygon->flags;
    if (flags & 2) {
        f32 (*normals)[4] = (f32 (*)[4])(((u32)count << 4) + EE_ADDR(hPolygon->pNormal));
        SDF_VU0_COPYVECTOR(normals[0], dds3PolygonTriangleParam);
        SDF_VU0_COPYVECTOR(normals[1], dds3PolygonTriangleParam);
        SDF_VU0_COPYVECTOR(normals[2], dds3PolygonTriangleParam);
    }
    if (flags & 4) {
        u32 *colors = (u32 *)(((u32)count << 2) + EE_ADDR(hPolygon->pVertexColor));
        colors[0] = 0x80000080;
        colors[1] = 0x80000080;
        colors[2] = 0x80000080;
        count = hPolygon->drawCount;
    }
    hPolygon->drawCount = count + 3;
}
