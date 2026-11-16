#ifndef BILLMANAGER_H
#define BILLMANAGER_H

#include "common.h"
#include "billboard.h"
#include "sdf_resource.h"

billHandle_t *dds3BillboardCopy(const billHandle_t *hBillboard);
billHandle_t *dds3BillboardCreate(s32 index, addr_t data);
void dds3BillboardDelete(billHandle_t *hData);
void dds3BillboardDraw(billHandle_t *hData);
sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number);
void dds3BillboardLinkOt(void);
void dds3BillboardSetAnimColorAdvMode(billHandle_t *hBillboard);
void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame);
void dds3BillboardSetAnimKey2(billHandle_t *hBillboard, u32 frame);
void dds3BillboardSetAnimNumber(billHandle_t *hBillboard, u32 number);
void dds3BillboardSetDrawType(billHandle_t *hBillboard, s32 type);
void dds3BillboardSetMode(billHandle_t *hBillboard, s32 mode);
void dds3BillboardSetPosition(void *hBillboard, const void *pPos);
void dds3BillboardSetXYOffset(billHandle_t *hBillboard, float x, float y);

#include "sdf_types.h"


billHandle_t *dds3BillboardGeneral(s32 number);
struct billData_t;
void dds3BillAnimFreeP2A(struct billData_t *hBillAnimRes);
u32 dds3Billboard_P2AColorMul(u32 color0, u32 color1);
void dds3MultiBillboardPktLink(billRenderPair_t *hResource);
void dds3MultiBillboardTagLink(billHandle_t *obj, billRenderPair_t *node);
f32 dds3BillboardGetDirectionRotate(const sdfVector4_t *pPos, const sdfVector4_t *pVec);
f32 dds3BillboardGetDirectionScale(const sdfVector4_t *pPos, const sdfVector4_t *pVec);
s32 dds3BillboardGetAnimFrame(const billHandle_t *hBillboard);

#endif
