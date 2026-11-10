#include "shared/effect/effPCPMisc/decls.h"

/* vu0 routine: initialize both transforms with the libvu0 identity primitive. */
dds3RingPolyExpList_tag *dds3RingPolyExpListCreate(u32 face) {
    u32 count = face * 4 + 4;
    dds3RingPolyExpList_tag *node = sdfChipAllocDirect(sizeof(dds3RingPolyExpList_tag));
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * 20);
    f32 *points = EE_ADDR_AS(void *, sdfMemLock(allocation));

    memset(points, 0, count * 20);
    node->pVertex = points;
    node->blend = 2;
    node->pVC = (u32 *)(points + count * 4);
    node->vertexCount = count;
    node->color = 0x80808080;
    node->hMem = allocation;
    node->scale = 1.0f;
    node->pMat = sdfMatNew();
    sdfMatSetConstant((struct sdfMat_t *)node->pMat, 1.0f);
    SDF_VU0_STOREIDENTITY(node->rot);
    SDF_VU0_STOREIDENTITY(node->matrix);
    memset(&dds3RingPolyExpListPolyParam, 0, sizeof(dds3RingPolyExpListPolyParam));
    dds3RingPolyExpListPolyParam.primitive = 0x4000;
    return node;
}
