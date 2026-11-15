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

struct dds3Billboard_Basic_t;

struct dds3Billboard_Basic_t *dds3BillboardGeneral_dds3Billboard(s32 number);
f32 dds3BillboardGetDirectionRotate(const sdfVector4_t *pPos, const sdfVector4_t *pVec);

#endif
