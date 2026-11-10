#include "shared/effect/polyManager/decls.h"

dds3Polygon_t *dds3PolygonCreate(s32 face, s16 type, s16 attr) {
    s32 vertexCount;
    s32 bytes;
    sdfMemHandle_t *allocation;
    u8 *buffer;
    dds3Polygon_t *record;
    sdfMat_t *asset;

    if (type & 8) vertexCount = face + 2;
    else vertexCount = face * 3;
    bytes = vertexCount * sizeof(u128);
    if (type & 1) bytes += vertexCount * sizeof(record->texcoords[0]);
    if (type & 2) bytes += vertexCount * sizeof(u128);
    if (type & 4) bytes += vertexCount * sizeof(u32);
    allocation = sdfMemAllocDirect(bytes + sizeof(*record));
    buffer = EE_ADDR_AS(u8 *, sdfMemLock(allocation));
    record = (dds3Polygon_t *)(buffer + bytes);
    memset(record, 0, sizeof(*record));
    record->pVertex = (u128 *)buffer;
    /* Allocation and layout reuse one byte extent; DDS1 cc1probe:
     * sandbox_ClusterX/ctor_reentry_F4_probe/decisions.log:16561-16660.
     * The saved span ties allocation at 1250, below vertex count at 3500;
     * a separate span instead has priority 3846 and takes the count's home. */
    bytes = vertexCount * sizeof(u128);
    buffer += bytes;
    if (type & 1) {
        record->texcoords = (u32 (*)[2])buffer;
        buffer += vertexCount * sizeof(record->texcoords[0]);
    }
    if (type & 2) record->pNormal = (u128 *)buffer;
    if (type & 4) record->pVertexColor = (u32 *)buffer;
    record->flags = type;
    record->capacity = vertexCount;
    record->hMem = allocation;
    record->drawCount = 0;
    memset(&record->parm, 0, sizeof(record->parm));
    record->parm.color = 0x80808080;
    if (!(attr & 1)) record->parm.attr = 0x4000;
    asset = sdfMatNew();
    record->pMat = asset;
    if (!(attr & 2)) sdfMatSetConstant(asset, 1.0f);
    return record;
}
