#include "common.h"
#include "motion.h"
#include "model.h"
#include "event.h"
#include "nb.h"
#include "effBattle.h"
#include "ps2asm.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "billManager.h"
#include "effPart.h"

extern sdfVector4_t effPartModelPosInit;
extern sdfVector4_t effPartModelRotInit;
extern sdfVector4_t effPartModelScaleInit;

typedef struct
{
    u32 kateg;
    u32 param;
    void *pHandle;
} dds3EffectPCP_t;

typedef struct
{
    void *(*create)(void *);
    void (*draw)(void *);
    void (*destroy)(void *);
    void *(*copy)(void *);
    void (*setPosition)(void *, void *);
    void (*setScale)(void *, f32);
    void (*setRotateMatrix)(void *, void *);
    void (*setColor)(void *, u32);
    void (*setDirection)(void *, void *);
    void (*setArea)(void *, f32);
} dds3EffectFunc_t;

extern dds3EffectFunc_t dds3EffectFuncTable[7];
extern s32 effPartModelSerial;

/* Header of a packed model an effect part carries, followed by the pack data. */
typedef struct
{
    u32 size;
    u8 reserve[0xC];
    u8 data[1];
} dds3EffectPartModelData_t;

extern dds3EffectFunc_t EffectPCPFuncTable[];
extern void **effPartPCPDataTable[];
void dds3BillboardSetScale(void *hBillboard, f32 x, f32 y);
void dds3EffectPartModelInit(mdlObj_t *hModel);
void *dds3EffectPartTMXBillboardCreate(u32 data);
void *dds3EffectPartP2ABillboardCreate(u32 data);
void dds3EffectPartBillboardScale(void *hBillboard, f32 scale);
mdlObj_t *dds3EffectPartModelCreate(dds3EffectPartModelData_t *pData);
void dds3EffectPartModelSetScale(void *hData, f32 scale);
void dds3EffectPartModelSetRotateMatrix(mdlObj_t *hData, sdfMatrix4_t *pMatrix);
dds3EffectPCP_t *dds3EffectPCPCreate(dds3EffectPCPData_t *hPCPData);
void dds3EffectPCPDelete(dds3EffectPCP_t *hData);
void dds3EffectPCPDraw(dds3EffectPCP_t *hData);
dds3EffectPCP_t *dds3EffectPCPCopy(dds3EffectPCP_t *hPCPSrc);
effPartHandle_t *dds3EffectPCPDataPartCreate(dds3EffectPCPData_t *hPCPData, u32 number);
extern void **effPartPCPDataTable[28];
extern sdfVector4_t effPartModelPosInit __attribute__((aligned(16)));
extern sdfVector4_t effPartModelRotInit __attribute__((aligned(16)));
extern sdfVector4_t effPartModelScaleInit __attribute__((aligned(16)));

/* Indexed by PCP category. The IDB names entries from 1 on, as effPCP*Table. */
extern dds3EffectFunc_t EffectPCPFuncTable[106];

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = type;
    hEffPart->pHandle = dds3EffectFuncTable[type].create(pData);
    return hEffPart;
}

void dds3EffectPartDelete(effPartHandle_t *hEffPart)
{
    dds3EffectFuncTable[hEffPart->type].destroy(hEffPart->pHandle);
    sdfChipFreeDirect(hEffPart);
}

void dds3EffectPartDraw(effPartHandle_t *hEffPart)
{
    dds3EffectFuncTable[hEffPart->type].draw(hEffPart->pHandle);
}

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = hEffPartSrc->type;
    hEffPart->pHandle = dds3EffectFuncTable[hEffPartSrc->type].copy(hEffPartSrc->pHandle);
    return hEffPart;
}

INCLUDE_ASM(const s32, "effect/effPart", effAPPoint4DrawSub);

INCLUDE_ASM(const s32, "effect/effPart", effBTLCutSetScaleSub);

INCLUDE_ASM(const s32, "effect/effPart", effPCPHama01PartInitSub);

INCLUDE_ASM(const s32, "effect/effPart", effAPPoint4PartInitSub);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPartSetDirection);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPartSetArea);

void *dds3EffectPartTMXBillboardCreate(u32 data)
{
    return dds3BillboardCreate(0, data);
}

void *dds3EffectPartP2ABillboardCreate(u32 data)
{
    return dds3BillboardCreate(1, data);
}

void dds3EffectPartBillboardScale(void *hBillboard, f32 scale)
{
    dds3BillboardSetScale(hBillboard, scale, scale);
}

void dds3EffectPartModelInit(mdlObj_t *hModel)
{
    SDF_VU0_LOADVECTOR(&effPartModelPosInit);
    mdlSetPosition(hModel);
    SDF_VU0_LOADVECTOR(&effPartModelRotInit);
    mdlSetRotate(hModel);
    SDF_VU0_LOADVECTOR(&effPartModelScaleInit);
    mdlSetScale(hModel);
    mdlSetColor(hModel, 0x80808080);
    if (hModel->first != NULL) {
        mdlLoopMotion(hModel, 0, 0);
        hModel->first->speed = 1.0f;
    }
    hModel->flags &= ~1;
}

mdlObj_t *dds3EffectPartModelCreate(dds3EffectPartModelData_t *pData)
{
    mdlObj_t *hModel;

    mdlPackDecode(7, effPartModelSerial, 0x101, pData->data, pData->size);
    hModel = mdlLoad(7, effPartModelSerial);
    dds3EffectPartModelInit(hModel);
    effPartModelSerial++;
    return hModel;
}

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPartModelDraw);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPartModelDelete);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPartModelCopy);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPartModelSetPosition);

void dds3EffectPartModelSetScale(void *hData, f32 scale)
{
    sdfVector4_t v;

    v.x = v.y = v.z = scale;
    SDF_VU0_LOADVECTOR_MEMOPS(v);
    mdlSetScale(hData);
}

void dds3EffectPartModelSetRotateMatrix(mdlObj_t *hData, sdfMatrix4_t *pMatrix)
{
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_STORE_ROW0(&hData->inner->tm.m[0]);
    SDF_VU0_STORE_ROW1(&hData->inner->tm.m[4]);
    SDF_VU0_STORE_ROW2(&hData->inner->tm.m[8]);
}

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPartModelSetColor);

dds3EffectPCP_t *dds3EffectPCPCreate(dds3EffectPCPData_t *hPCPData)
{
    dds3EffectPCP_t *hPCP;

    hPCP = sdfChipAllocDirect(sizeof(dds3EffectPCP_t));
    hPCP->kateg = dds3EffectPCPDataGetKateg(hPCPData);
    hPCP->param = dds3EffectPCPDataGetParam(hPCPData);
    if (EffectPCPFuncTable[hPCP->kateg].copy == NULL) {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(effPartPCPDataTable[hPCP->kateg][hPCP->param]);
    } else {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(hPCPData);
    }
    return hPCP;
}

void dds3EffectPCPDelete(dds3EffectPCP_t *hData)
{
    EffectPCPFuncTable[hData->kateg].destroy(hData->pHandle);
    sdfChipFreeDirect(hData);
}

void dds3EffectPCPDraw(dds3EffectPCP_t *hData)
{
    EffectPCPFuncTable[hData->kateg].draw(hData->pHandle);
}

dds3EffectPCP_t *dds3EffectPCPCopy(dds3EffectPCP_t *hPCPSrc)
{
    dds3EffectPCP_t *hPCP;

    hPCP = sdfChipAllocDirect(sizeof(dds3EffectPCP_t));
    hPCP->kateg = hPCPSrc->kateg;
    hPCP->param = hPCPSrc->param;
    if (EffectPCPFuncTable[hPCP->kateg].copy == NULL) {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(effPartPCPDataTable[hPCP->kateg][hPCP->param]);
    } else {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].copy(hPCPSrc->pHandle);
    }
    return hPCP;
}

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPCPSetPosition);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPCPSetScale);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPCPSetRotateMatrix);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPCPSetDirection);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPCPSetColor);

INCLUDE_ASM(const s32, "effect/effPart", dds3EffectPCPSetArea);

INCLUDE_ASM(const s32, "effect/effPart", effEctPCPCreateSub);

INCLUDE_ASM(const s32, "effect/effPart", effEctPCPCreateSub2);

void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return (u8 *)hPCPData + part->offset;
}

INCLUDE_ASM(const s32, "effect/effPart", effLeaf58);

u32 dds3EffectPCPDataGetType(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return part->type;
}

effPartHandle_t *dds3EffectPCPDataPartCreate(dds3EffectPCPData_t *hPCPData, u32 number)
{
    u16 type;
    void *pData;

    type = (u16)dds3EffectPCPDataGetType(hPCPData, number);
    pData = dds3EffectPCPDataGetData(hPCPData, number);
    return dds3EffectPartCreate(type, pData);
}

s32 effPartModelSerial = 0;
