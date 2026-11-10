#include "shared/effect/effPCPNeedle/decls.h"

void dds3NeedleModelDraw(dds3NeedleModel_tag *hNModel)
{
    f32 matrix[16] __attribute__((aligned(16)));
    void *packet;
    dds3NeedleModelPart_tag *entry;
    u32 entryIndex;
    u32 remaining;
    u32 perPrim;

    packet = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    sdf3DPrimLinkMat(packet, hNModel->pMat, 0);
    SDF_VU0_STOREIDENTITY(matrix);
    matrix[0] = hNModel->scale[0];
    matrix[5] = hNModel->scale[1];
    matrix[10] = hNModel->scale[2];
    SDF_VU0_LOADMATRIX(matrix);
    SDF_VU0_LOADMATRIX_B(hNModel->matrix);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX(matrix);
    dds3NeedleModelPrimParam.parameters = NULL;
    entry = hNModel->pPart;
    for (entryIndex = 0; entryIndex < hNModel->number; entryIndex++, entry++) {
        perPrim = 3;
        matrix[12] = entry->position[0];
        matrix[13] = entry->position[1];
        matrix[14] = entry->position[2];
        SDF_VU0_LOADMATRIX(matrix);
        sdf3DPrimLinkTm(packet, 0);
        if (hNModel->hVNMem == NULL) {
            remaining = 12;
            dds3NeedleModelPrimParam.colors = dds3NeedleModelDefaultColors;
            dds3NeedleModelPrimParam.positions = dds3NeedleModelDefaultPoints;
            dds3NeedleModelPrimParam.normals = NULL;
        } else {
            dds3NeedleModelPrimParam.positions = hNModel->positions;
            dds3NeedleModelPrimParam.colors = hNModel->pVC;
            dds3NeedleModelPrimParam.normals = hNModel->normals;
            remaining = hNModel->point;
        }
        dds3NeedleModelPrimParam.color = entry->abgr;
        dds3NeedleModelPrimParam.primCount = 16;
        dds3NeedleModelPrimParam.vertexCount = 48;
        while (remaining >= 48) {
            sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(&dds3NeedleModelPrimParam)));
            if (hNModel->hVNMem == NULL) {
                dds3NeedleModelPrimParam.colors += 48;
                dds3NeedleModelPrimParam.positions += 48;
            } else {
                dds3NeedleModelPrimParam.colors += 48;
                dds3NeedleModelPrimParam.positions += 48;
                dds3NeedleModelPrimParam.normals += 48;
            }
            remaining -= 48;
        }
        if (remaining >= 3) {
            dds3NeedleModelPrimParam.primCount = remaining / perPrim;
            dds3NeedleModelPrimParam.vertexCount = remaining;
            sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(&dds3NeedleModelPrimParam)));
        }
    }
    dds3NeedleModelDrawSurfaces[hNModel->blend]->append(dds3NeedleModelDrawSurfaces[hNModel->blend], packet);
}
