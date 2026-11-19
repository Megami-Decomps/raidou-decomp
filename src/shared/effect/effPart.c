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

#ifdef VERSION_RAIDOU1
extern dds3EffectFunc_t dds3EffectFuncTable[8];
#else
extern dds3EffectFunc_t dds3EffectFuncTable[7];
#endif
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
#ifdef VERSION_RAIDOU1
extern dds3EffectFunc_t EffectPCPFuncTable[98];
#else
extern dds3EffectFunc_t EffectPCPFuncTable[106];
#endif

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

void effAPPoint4DrawSub(effPartHandle_t *hEffPart, void *pPos)
{
    void (*handler)(void *, void *) = dds3EffectFuncTable[hEffPart->type].setPosition;

    if (handler != NULL) {
        handler(hEffPart->pHandle, pPos);
    }
}

void effBTLCutSetScaleSub(effPartHandle_t *hEffPart, f32 scale)
{
    void (*handler)(void *, f32) = dds3EffectFuncTable[hEffPart->type].setScale;

    if (handler != NULL) {
        handler(hEffPart->pHandle, scale);
    }
}

void effPCPHama01PartInitSub(effPartHandle_t *hEffPart, void *pMatrix)
{
    void (*handler)(void *, void *) = dds3EffectFuncTable[hEffPart->type].setRotateMatrix;

    if (handler != NULL) {
        handler(hEffPart->pHandle, pMatrix);
    }
}

void effAPPoint4PartInitSub(effPartHandle_t *hEffPart, u32 color)
{
    void (*handler)(void *, u32) = dds3EffectFuncTable[hEffPart->type].setColor;

    if (handler != NULL) {
        handler(hEffPart->pHandle, color);
    }
}

void dds3EffectPartSetDirection(effPartHandle_t *hEffPart, void *pDir)
{
    void (*handler)(void *, void *) = dds3EffectFuncTable[hEffPart->type].setDirection;

    if (handler != NULL) {
        handler(hEffPart->pHandle, pDir);
    }
}

void dds3EffectPartSetArea(effPartHandle_t *hEffPart, f32 area)
{
    void (*handler)(void *, f32) = dds3EffectFuncTable[hEffPart->type].setArea;

    if (handler != NULL) {
        handler(hEffPart->pHandle, area);
    }
}

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

void dds3EffectPartModelDraw(void *hData) {
    mdlLink(hData, sdfModelOtTable);
}

void dds3EffectPartModelDelete(void *arg) {
    mdlFree(arg);
}

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

void dds3EffectPartModelSetColor(void *arg0, u32 arg1) {
    mdlSetColor(arg0, arg1);
}

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

void dds3EffectPCPSetPosition(void *hData, void *pPos)
{
    dds3EffectPCP_t *hPCP = hData;
    void (*handler)(void *, void *) = EffectPCPFuncTable[hPCP->kateg].setPosition;

    if (handler != NULL) {
        handler(hPCP->pHandle, pPos);
    }
}

void dds3EffectPCPSetScale(void *hData, f32 scale)
{
    dds3EffectPCP_t *hPCP = hData;
    void (*handler)(void *, f32) = EffectPCPFuncTable[hPCP->kateg].setScale;

    if (handler != NULL) {
        handler(hPCP->pHandle, scale);
    }
}

void dds3EffectPCPSetRotateMatrix(void *hData, void *pMatrix)
{
    dds3EffectPCP_t *hPCP = hData;
    void (*handler)(void *, void *) = EffectPCPFuncTable[hPCP->kateg].setRotateMatrix;

    if (handler != NULL) {
        handler(hPCP->pHandle, pMatrix);
    }
}

void dds3EffectPCPSetDirection(void *hData, void *pDir)
{
    dds3EffectPCP_t *hPCP = hData;
    void (*handler)(void *, void *) = EffectPCPFuncTable[hPCP->kateg].setDirection;

    if (handler != NULL) {
        handler(hPCP->pHandle, pDir);
    }
}

void dds3EffectPCPSetColor(void *hData, u32 color)
{
    dds3EffectPCP_t *hPCP = hData;
    void (*handler)(void *, u32) = EffectPCPFuncTable[hPCP->kateg].setColor;

    if (handler != NULL) {
        handler(hPCP->pHandle, color);
    }
}

void dds3EffectPCPSetArea(void *hData, f32 area)
{
    dds3EffectPCP_t *hPCP = hData;
    void (*handler)(void *, f32) = EffectPCPFuncTable[hPCP->kateg].setArea;

    if (handler != NULL) {
        handler(hPCP->pHandle, area);
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPart.c:401, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 effEctPCPCreateSub(dds3EffectPCPData_t *hPCPData)
{
    return hPCPData->kateg;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPart.c:406, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 effEctPCPCreateSub2(dds3EffectPCPData_t *hPCPData)
{
    return hPCPData->param;
}

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

#ifdef VERSION_RAIDOU1
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry10;
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry11;
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry12;
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry13;
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry14;
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry15;
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry16;
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry17;
extern struct effPCPThunderSpherePolyParam_t effPartPCPDataTableEntry18;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry19;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry20;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry21;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry22;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry23;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry24;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry25;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry26;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry27;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry28;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry29;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry30;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry31;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry32;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry33;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry34;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry35;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry36;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry37;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry38;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry39;
extern struct effPCP_ThunderPolyParam_t effPartPCPDataTableEntry40;
extern struct effPCP_ThunderRadiatePolyParam_t effPartPCPDataTableEntry41;
extern struct effPCPBlinkMultiBlurParam_t effPartPCPDataTableEntry42;
extern struct effPCPBlinkMultiBlurParam_t effPartPCPDataTableEntry43;
extern struct effPCPBlinkMultiBlurParam_t effPartPCPDataTableEntry47;
extern struct effPCPSparkleSphereParam_t effPartPCPDataTableEntry9;
extern struct effPCPFlashPolySphereParam_t effPartPCPDataTableEntry44;
extern struct effPCPMultiFilterExpParam_t effPartPCPDataTableEntry45;
extern struct effPCPMultiFilterExpParam_t effPartPCPDataTableEntry46;

void *dds3EffectLoad(void *);
void dds3EffectDraw_dds3Effect(void *);
void dds3EffectDelete(void *);
void *dds3EffectCopy(void *);
void dds3EffectSetPosition(void *, void *);
void dds3EffectSetScale(void *, f32);
void dds3EffectSetRotateMatrix(void *, void *);
void effSetEffectColorSubSub(void *, u32);
void dds3PCPSparkleSphereDrawSub(void *, u32);
void dds3EffectPartModelDraw(void *);
void dds3EffectPartModelDelete(void *);
void *dds3EffectPartModelCopy(void *);
void dds3EffectPartModelSetPosition(void *, void *);
void dds3EffectPartModelSetColor(void *, u32);
void dds3EffectPCPSetPosition(void *, void *);
void dds3EffectPCPSetScale(void *, f32);
void dds3EffectPCPSetRotateMatrix(void *, void *);
void dds3EffectPCPSetColor(void *, u32);
void dds3EffectPCPSetArea(void *, f32);
void *dds3P2APolygonCreate(void *);
void dds3P2APolygonDraw(void *);
void dds3P2APolygonDelete(void *);
void *dds3P2APolygonCopy(void *);
void dds3P2APolygonSetPosition(void *, void *);
void dds3P2APolygonSetScale(void *, f32);
void dds3P2APolygonSetRotateMatrix(void *, void *);
void dds3P2APolygonSetRGBA(void *, u32);
void dds3P2APolygonSetDirection(void *, void *);
void dds3EffectPCPSetDirection(void *, void *);
void *Create_effPartsList(void *);
void effPartsDraw(void *);
void effPartsDelete(void *);
void *effPartsCreateFinish(void *);
void effPartsPositionSet(void *, void *);
void effPartsScaleSet(void *, f32);
void effPartsRotateMatrixSet(void *, void *);
void ColorSet(void *, u32);
void *effPCPThunderSpherePolyCreate(void *);
void effPCPThunderSpherePolyLowDraw(void *);
void effPCPFlashPolyCircleDelete(void *);
void effPCPShockWave01DrawSub(void *, void *);
void effPCPThunderSphereExpDrawSub(void *, f32);
void effPCPShockWave01DrawSub2(void *, u32);
void *effPCP_ThunderPolyCreate(void *);
void effPCP_ThunderPolyDraw(void *);
void effPCP_ThunderPolyDelete(void *);
void effPCP_ThunderPolyLowSetPosition(void *, void *);
void effPCPRadiateThunderDrawSub(void *, u32);
void *effPCP_ThunderRadiatePolyCreate(void *);
void effPCP_ThunderRadiatePolyDraw(void *);
void effPCP_ThunderRadiatePolyDelete(void *);
void effPCP_ThunderRadiatePolySetPosition(void *, void *);
void effPCP_ThunderRadiatePolySetColor(void *, u32);
void *effPCPHama01Create(void *);
void effPCPHama01Draw(void *);
void effPCPHama01Delete(void *);
void *effPCPHama01Copy(void *);
void effPCPHama01SetPosition(void *, void *);
void effPCPHama01SetScale(void *, f32);
void effPCPHama01SetColor(void *, u32);
void *effPCPHama02Create(void *);
void effPCPHama02Draw(void *);
void effPCPHama02Delete(void *);
void *effPCPHama02Copy(void *);
void effPCPHama02SetPosition(void *, void *);
void effPCPHama02SetScale(void *, f32);
void effPCPHama02SetColor(void *, u32);
void *effPCPHamaonCreate(void *);
void effPCPHamaonDraw(void *);
void effPCPHamaonDelete(void *);
void *effPCPHamaonCopy(void *);
void effPCPHamaonSetPosition(void *, void *);
void effPCPHamaonSetScale(void *, f32);
void effPCPHamaonSetColor(void *, u32);
void *effPCPMudoCreate(void *);
void effPCPMudoDraw(void *);
void effPCPMudoDelete(void *);
void *effPCPMudoCopy(void *);
void effPCPMudoSetPosition(void *, void *);
void effPCPMudoSetScale(void *, f32);
void effPCPMudoSetColor(void *, u32);
void *effPCPHwave01Create(void *);
void effPCPHwave01Draw(void *);
void effPCPHwave01Delete(void *);
void *effPCPHwave01Copy(void *);
void effPCPHwave01SetPosition(void *, void *);
void effPCPHwave01SetColor(void *, u32);
void *effPCP_ThunderPolyLowCreate(void *);
void effPCP_ThunderPolyLowDraw(void *);
void effPCP_ThunderPolyLowDelete(void *);
void effPCP_ThunderPolySetPosition(void *, void *);
void effPCP_ThunderPolyLowSetColor(void *, u32);
void *effPCPZiodin01Create(void *);
void effPCPZiodin01Draw(void *);
void effPCPZiodin01Delete(void *);
void *effPCPZiodin01Copy(void *);
void effPCPZiodin01SetPosition(void *, void *);
void effPCPZiodin01SetColor(void *, u32);
void *effPCPMultiBlurExpCreate(void *);
void effPCPMultiBlurExpDraw(void *);
void effBTLSys_Encount_Delete2(void *);
void effPCPMultiBlurExpSetPosition(void *, void *);
void effPCPMultiBlurExpSetColor(void *, u32);
void effPCPMultiBlurExpSetArea(void *, f32);
void *effPCPMHanma01Create(void *);
void effPCPMHanma01Draw(void *);
void effPCPMHanma01Delete(void *);
void *effPCPMHanma01Copy(void *);
void effPCPMHanma01SetPosition(void *, void *);
void effPCPMHanma01SetColor(void *, u32);
void *effPCPMZiodin01Create(void *);
void effPCPMZiodin01Draw(void *);
void effPCPMZiodin01Delete(void *);
void *effPCPMZiodin01Copy(void *);
void effPCPMZiodin01SetPosition(void *, void *);
void effPCPMZiodin01SetColor(void *, u32);
void *effPCPSparkleSphereCreate(void *);
void effPCPSparkleSphereDraw(void *);
void effBTLSys_Nego_Kyusyu_Delete(void *);
void effPCPSparkleSphereSetPosition(void *, void *);
void effPCPSparkleSphereSetColor(void *, u32);
void effPCPSparkleSphereSetArea(void *, f32);
void *effPCPFlashPolySphereCreate(void *);
void effPCPFlashPolySphereDraw(void *);
void effPCPFlashPolySphereDelete(void *);
void effPCPFlashPolySphereSetPosition(void *, void *);
void effPCPFlashPolySphereSetScale(void *, f32);
void effPCPFlashPolySphereSetColor(void *, u32);
void *effPCPFlashPolySphereCreatePCF(void *);
void *effPCPFlashPolySphereCopy(void *);
void *effPCPFlashPolyRingCreatePCF(void *);
void effPCPFlashPolyRingDraw(void *);
void effPCPFlashPolyRingDelete(void *);
void *effPCPFlashPolyRingCopy(void *);
void effPCPFlashPolyRingSetPosition(void *, void *);
void effPCPFlashPolyRingSetScale(void *, f32);
void effPCPFlashPolyRingSetColor(void *, u32);
void *effPCPFlashPolyPipeCreatePCF(void *);
void effPCPFlashPolyPipeDraw(void *);
void effPCPFlashPolyPipeDelete(void *);
void *effPCPFlashPolyPipeCopy(void *);
void effPCPFlashPolyPipeSetPosition(void *, void *);
void effPCPFlashPolyPipeSetScale(void *, f32);
void effPCPFlashPolyPipeSetColor(void *, u32);
void *effPCPFlashPolySphere2CreatePCF(void *);
void effPCPFlashPolySphere2Draw(void *);
void effPCPFlashPolySphere2Delete(void *);
void *effPCPFlashPolySphere2Copy(void *);
void effPCPFlashPolySphere2SetPosition(void *, void *);
void effPCPFlashPolySphere2SetScale(void *, f32);
void effPCPFlashPolySphere2SetColor(void *, u32);
void *effPCPFlashPolyRingExpCreatePCF(void *);
void effPCPFlashPolyRingExpHDraw(void *);
void effPCPFlashPolyRingExpDelete(void *);
void *effPCPFlashPolyRingExpCopy(void *);
void effPCPFlashPolyRingExpSetPosition(void *, void *);
void effPCPFlashPolyRingExpSetScale(void *, f32);
void effPCPFlashPolyRingExpHSetColor(void *, u32);
void *effPCPMZiodin02Create(void *);
void effPCPMZiodin02Draw(void *);
void effPCPMZiodin02Delete(void *);
void *effPCPMZiodin02Copy(void *);
void effPCPMZiodin02SetPosition(void *, void *);
void effPCPMZiodin02SetColor(void *, u32);
void *effPCPZiodin02Create(void *);
void effPCPZiodin02Draw(void *);
void effPCPZiodin02Delete(void *);
void *effPCPZiodin02Copy(void *);
void effPCPZiodin02SetPosition(void *, void *);
void effPCPZiodin02SetColor(void *, u32);
void *effPCPZiodin03Create(void *);
void effPCPZiodin03Draw(void *);
void effPCPZiodin03Delete(void *);
void *effPCPZiodin03Copy(void *);
void effPCPZiodin03SetPosition(void *, void *);
void effPCPZiodin03SetColor(void *, u32);
void *effPCPZiodin04Create(void *);
void effPCPZiodin04Draw(void *);
void effPCPZiodin04Delete(void *);
void *effPCPZiodin04Copy(void *);
void effPCPZiodin04SetPosition(void *, void *);
void effPCPZiodin04SetColor(void *, u32);
void *effPCPShockWave01Create(void *);
void effPCPShockWave01Draw(void *);
void effBTLSys_Nego_Kaihuku_Delete2(void *);
void *effPCPShockWave01Copy(void *);
void effPCPShockWave01SetPosition(void *, void *);
void effPCPShockWave01SetColor(void *, u32);
void *effPCPHouden01Create(void *);
void effPCPHouden01Draw(void *);
void effBTLSys_Nego_Kaihuku_Delete(void *);
void *effPCPHouden01Copy(void *);
void effPCPHouden01SetPosition(void *, void *);
void effPCPHouden01SetColor(void *, u32);
void *effPCPMultiFilterExpCreate(void *);
void effPCPMultiFilterExpDraw(void *);
void effBTLSys_Encount_Delete(void *);
void effPCPMultiFilterExpSetPosition(void *, void *);
void effPCPMultiFilterExpSetColor(void *, u32);
void effPCPMultiFilterExpSetArea(void *, f32);
void *effPCPBlinkMultiBlurCreate(void *);
void effPCPBlinkMultiBlurDraw(void *);
void effPCPBlinkMultiBlurDelete(void *);
void effPCPBlinkMultiBlurSetPosition(void *, void *);
void effPCPBlinkMultiBlurSetColor(void *, u32);
void effPCPBlinkMultiBlurSetArea(void *, f32);
void *effPCPMultiFilterExpCreatePCF(void *);
void *effPCPMultiFilterExpCopy(void *);
void *effPCPShockWave02Create(void *);
void effPCPShockWave02Draw(void *);
void effPCPShockWave02Delete(void *);
void *effPCPShockWave02Copy(void *);
void effPCPShockWave02SetPosition(void *, void *);
void effPCPShockWave02SetColor(void *, u32);
void *effPCPFilterExpCreatePCF(void *);
void effPCPFilterExpDraw(void *);
void effPCPFilterExpDelete(void *);
void *effPCPFilterExpCopy(void *);
void effPCPFilterExpSetColor(void *, u32);
void *effPCPBlurExpCreatePCF(void *);
void effPCPBlurExpDraw(void *);
void effPCPBlurExpDelete(void *);
void *effPCPBlurExpCopy(void *);
void effPCPBlurExpSetColor(void *, u32);
void *effPCPDistortionBlurExpCreatePCF(void *);
void effPCPDistortionBlurExpDraw(void *);
void effPCPDistortionBlurExpDelete(void *);
void *effPCPRippleBlurExpCopy(void *);
void effPCPDistortionBlurExpSetPosition(void *, void *);
void effPCPDistortionBlurExpSetColor(void *, u32);
void *effPCPMultiBlurExp2CreatePCF(void *);
void effPCPMultiBlurExp2Draw(void *);
void effPCPMultiBlurExp2Delete(void *);
void *effPCPMultiBlurExp2Copy(void *);
void effPCPMultiBlurExp2SetPosition(void *, void *);
void effPCPMultiBlurExp2SetColor(void *, u32);
void *effPCPMegidora01Create(void *);
void effPCPMegidora01Draw(void *);
void effPCPMegidora01Delete(void *);
void *effPCPMegidora01Copy(void *);
void effPCPMegidora01SetPosition(void *, void *);
void effPCPMegidora01SetColor(void *, u32);
void *effPCPMegidoraon01Create(void *);
void effPCPMegidoraon01Draw(void *);
void effPCPMegidoraon01Delete(void *);
void *effPCPMegidoraon01Copy(void *);
void effPCPMegidoraon01SetPosition(void *, void *);
void effPCPMegidoraon01SetColor(void *, u32);
void *effPCPBreathCreatePCF(void *);
void effPCPBreathDraw(void *);
void effPCPBreathDelete(void *);
void *effPCPBreathCopy(void *);
void effPCPBreathSetPosition(void *, void *);
void effPCPBreathSetRotateMatrix(void *, void *);
void effPCPBreathSetColor(void *, u32);
void *effPCPHamaCardSpiralCreatePCF(void *);
void effPCPHamaCardSpiralDraw(void *);
void effPCPHamaCardSpiralDelete(void *);
void *effPCPHamaCardSpiralCopy(void *);
void effPCPHamaCardSpiralSetPosition(void *, void *);
void effPCPHamaCardSpiralSetScale(void *, f32);
void effPCPHamaCardSpiralSetColor(void *, u32);
void *effPCPHamaCardSphereCreatePCF(void *);
void effPCPHamaCardSphereDraw(void *);
void effPCPHamaCardSphereDelete(void *);
void *effPCPHamaCardSphereCopy(void *);
void effPCPHamaCardSphereSetPosition(void *, void *);
void effPCPHamaCardSphereSetScale(void *, f32);
void effPCPHamaCardSphereSetColor(void *, u32);
void *effPCPAgiraoFireCreatePCF(void *);
void effPCPAgiraoFireDraw(void *);
void effPCPAgiraoFireDelete(void *);
void *effPCPAgiraoFireCopy(void *);
void effPCPAgiraoFireSetPosition(void *, void *);
void effPCPAgiraoFireSetColor(void *, u32);
void *effPCPBufuIceCreate(void *);
void effPCPBufuIceDraw(void *);
void effPCPBufuIceDelete(void *);
void *effPCPBufuIceCopy(void *);
void effPCPBufuIceSetPosition(void *, void *);
void effPCPBufuIceSetScale(void *, f32);
void effPCPBufuIceSetRotateMatrix(void *, void *);
void effPCPBufuIceSetColor(void *, u32);
void *effPCPBufudnIceCreate(void *);
void effPCPBufudnIceDraw(void *);
void effCallEffectPartDelete(void *);
void *effPCPBufudnIceCopy(void *);
void effPCPBufudnIceSetPosition(void *, void *);
void effPCPBufudnIceSetScale(void *, f32);
void effPCPBufudnIceSetRotateMatrix(void *, void *);
void effPCPBufudnIceSetColor(void *, u32);
void *effPCPBufuraIceCreate(void *);
void effPCPBufuraIceDraw(void *);
void effCallEffectPartDelete2(void *);
void *effPCPBufuraIceCopy(void *);
void effPCPBufuraIceSetPosition(void *, void *);
void effPCPBufuraIceSetScale(void *, f32);
void effPCPBufuraIceSetRotateMatrix(void *, void *);
void effPCPBufuraIceSetColor(void *, u32);
void *effLeaf147(void *);
void effLeaf150(void *);
void effLeaf149(void *);
void *effLeaf148(void *);
void *effPCPAgiFireballCreatePCF(void *);
void effPCPAgiFireball2DrawSub(void *);
void effPCPAgiFireballDelete(void *);
void *effPCPAgiFireballCopy(void *);
void effPCPAgiFireballSetPosition(void *, void *);
void effPCPAgiFireballSetRotateMatrix(void *, void *);
void effPCPAgiFireballSetColor(void *, u32);
void *effPCPNeedleSpiralCreatePCF(void *);
void effPCPNeedleSpiralDraw(void *);
void effPCPNeedleSpiralDelete(void *);
void *effPCPNeedleSpiralCopy(void *);
void effPCPNeedleSpiralSetPosition(void *, void *);
void effPCPNeedleSpiralSetColor(void *, u32);
void *effCallChipAllocDirect2(void *);
void effPCPEyeThunderDraw(void *);
void effPCPEyeThunderDelete(void *);
void *effPCPEyeThunderCopy00_0(void *);
void effPCPEyeThunderSetPosition(void *, void *);
void effPCPEyeThunderSetColor(void *, u32);
void *effCallChipAllocDirect3(void *);
void *effPCPEyeThunderCopy00_1(void *);
void *effCallChipAllocDirect4(void *);
void *effPCPEyeThunderCopy01_0(void *);
void *effCallChipAllocDirect5(void *);
void *effPCPEyeThunderCopy01_1(void *);
void *effCallChipAllocDirect6(void *);
void *effPCPEyeThunderCopy02_0(void *);
void *effCallChipAllocDirect7(void *);
void *effPCPEyeThunderCopy02_1(void *);
void *effCallChipAllocDirect8(void *);
void *effPCPEyeThunderCopy03_0(void *);
void *effCallChipAllocDirect9(void *);
void *effPCPEyeThunderCopy03_1(void *);
void *effCallChipAllocDirect10(void *);
void *effPCPEyeThunderCopy04_0(void *);
void *effCallChipAllocDirect11(void *);
void *effPCPEyeThunderCopy04_1(void *);
void *effPCPRippleBlurExpCreatePCF(void *);
void effPCPRippleBlurExpDraw(void *);
void effPCPRippleBlurExpDelete(void *);
void *effPCPDistortionBlurExpCopy(void *);
void effPCPRippleBlurExpSetPosition(void *, void *);
void effPCPRippleBlurExpSetColor(void *, u32);
void *effPCPThunderSphereExpCreatePCF(void *);
void effPCPThunderSphereExpDraw(void *);
void effPCPThunderSphereExpDelete(void *);
void *effPCPThunderSphereExpCopy(void *);
void effPCPThunderSphereExpSetPosition(void *, void *);
void effPCPThunderSphereExpSetScale(void *, f32);
void effPCPThunderSphereExpSetColor(void *, u32);
void *effPCPRingPolyExpCreatePCF(void *);
void effPCPRingPolyExpDraw(void *);
void effPCPRingPolyExpDelete(void *);
void *effPCPRingPolyExpCopy(void *);
void effPCPRingPolyExpSetPosition(void *, void *);
void effPCPRingPolyExpSetScale(void *, f32);
void effPCPRingPolyExp2SetRotateMatrix(void *, void *);
void effPCPRingPolyExpSetColor(void *, u32);
void effPCPRingPolyExpSetDirection(void *, void *);
void *effPCPWhirlwindPolyCreatePCF(void *);
void effPCPWhirlwindPolyDraw(void *);
void effPCPWhirlwindPolyDelete(void *);
void *effPCPWhirlwindPolyCopy(void *);
void effPCPWhirlwindPolySetPosition(void *, void *);
void effPCPWhirlwindPolySetScale(void *, f32);
void effPCPWhirlwind3PolySetRotateMatrix(void *, void *);
void effPCPWhirlwindPolySetColor(void *, u32);
void *effCallChipAllocDirect12(void *);
void *effPCPEyeThunderCopy05_0(void *);
void *effCallChipAllocDirect13(void *);
void *effPCPEyeThunderCopy05_1(void *);
void *effPCPRadiateThunderCreatePCF(void *);
void effPCPRadiateThunderDraw(void *);
void effPCPRadiateThunderDelete(void *);
void *effPCPRadiateThunderCopy(void *);
void effPCPRadiateThunderSetPosition(void *, void *);
void effPCPRadiateThunderSetScale(void *, f32);
void effPCPRadiateThunderSetColor(void *, u32);
void *effPCPWwSpherePolyCreatePCF(void *);
void effPCPWwSpherePolyDraw(void *);
void effPCPWwSpherePolyDelete(void *);
void *effPCPWwSpherePolyCopy(void *);
void effPCPWwSpherePolySetPosition(void *, void *);
void effPCPWwSpherePolySetScale(void *, f32);
void effPCPWwSpherePolySetRotateMatrix(void *, void *);
void effPCPWwSpherePolySetColor(void *, u32);
void *effPCPFlashPolySphereExpCreatePCF(void *);
void effPCPFlashPolySphereExpDraw(void *);
void effPCPFlashPolySphereExpDelete(void *);
void *effPCPFlashPolySphereExpCopy(void *);
void effPCPFlashPolySphereExpSetPosition(void *, void *);
void effPCPFlashPolySphereExpSetScale(void *, f32);
void effPCPFlashPolySphereExpSetColor(void *, u32);
void *effPCPFlashPolySphere2ExpCreatePCF(void *);
void effPCPFlashPolySphere2ExpDraw(void *);
void effPCPFlashPolySphere2ExpDelete(void *);
void *effPCPFlashPolySphere2ExpCopy(void *);
void effPCPFlashPolySphere2ExpSetPosition(void *, void *);
void effPCPFlashPolySphere2ExpSetScale(void *, f32);
void effPCPFlashPolySphere2ExpSetColor(void *, u32);
void *effPCPFlashPolySphere3CreatePCF(void *);
void effPCPFlashPolySphere3Draw(void *);
void effPCPFlashPolySphere3Delete(void *);
void *effPCPFlashPolySphere3Copy(void *);
void effPCPFlashPolySphere3SetPosition(void *, void *);
void effPCPFlashPolySphere3SetScale(void *, f32);
void effPCPFlashPolySphere3SetColor(void *, u32);
void *effPCPFlashPolyRingExp2CreatePCF(void *);
void effPCPFlashPolyRingExp2Draw(void *);
void effFunc9(void *);
void *effPCPFlashPolyRingExp2Copy(void *);
void effPCPFlashPolyRingExp2SetPosition(void *, void *);
void effPCPFlashPolyRingExp2SetScale(void *, f32);
void effPCPFlashPolyRingExp2SetColor(void *, u32);
void *effPCPWhirlwind2PolyCreatePCF(void *);
void effPCPWhirlwind2PolyDraw(void *);
void effPCPWhirlwind2PolyDelete(void *);
void *effPCPWhirlwind2PolyCopy(void *);
void effPCPWhirlwind2PolySetPosition(void *, void *);
void effPCPWhirlwind2PolySetScale(void *, f32);
void effPCPWhirlwind2PolySetRotateMatrix(void *, void *);
void effPCPWhirlwind2PolySetColor(void *, u32);
void *effPCPMakkaCreat(void *);
void effPCPMakkaDraw(void *);
void effPCPMakkaDelete(void *);
void *effPCPMakkaCopy(void *);
void effPCPMakkaSetPosition(void *, void *);
void effPCPMakkaSetScale(void *, f32);
void effPCPMakkaSetColor(void *, u32);
void *effPCPRingPolyExp2CreatePCF(void *);
void effPCPRingPolyExp2Draw(void *);
void effPCPRingPolyExp2Delete(void *);
void *effPCPRingPolyExp2Copy(void *);
void effPCPRingPolyExp2SetPosition(void *, void *);
void effPCPRingPolyExp2SetScale(void *, f32);
void effPCPRingPolyExp2SetColor(void *, u32);
void *effPCPAgiFireball2CreatePCF(void *);
void effPCPAgiFireball2Draw(void *);
void effPCPAgiFireball2Delete(void *);
void *effPCPAgiFireball2Copy(void *);
void effPCPAgiFireball2SetRotateMatrix(void *, void *);
void effCallPCPAgiFireballSetRotateMatri(void *, void *);
void effPCPAgiFireball2SetColor(void *, u32);
void *effPCPRadiateBEDCreatePCF(void *);
void effPCPRadiateBEDDraw(void *);
void effPCPRadiateBEDDelete(void *);
void *effPCPRadiateBEDCopy(void *);
void effPCPRadiateBEDSetPosition(void *, void *);
void effPCPRadiateBEDSetScale(void *, f32);
void effPCPRadiateBEDSetColor(void *, u32);
void *effAPPoint4CreatePCF(void *);
void effAPPoint4Draw(void *);
void effAPPoint4Delete(void *);
void *effAPPoint4Copy(void *);
void *effPCPRadiateTBCreatePCF(void *);
void effPCPRadiateTBDraw(void *);
void effPCPRadiateTBDelete(void *);
void *effPCPRadiateTBCopy(void *);
void effPCPRadiateTBSetPosition(void *, void *);
void effPCPRadiateTBSetColor(void *, u32);
void *effPCPWhirlwind3PolyCreatePCF(void *);
void effPCPWhirlwind3PolyDraw(void *);
void effPCPWhirlwind3PolyDelete(void *);
void *effPCPWhirlwind3PolyCopy(void *);
void effPCPWhirlwind3PolySetPosition(void *, void *);
void effPCPWhirlwind3PolySetScale(void *, f32);
void effCallMultMatrixNoArg(void *, void *);
void effPCPWhirlwind3PolySetColor(void *, u32);
void *effPCPThunderSpherePolyCreatePCF(void *);
void *effPCPThunderSpherePolyCopy(void *);
void *effCallEffectPCPDataGetData(void *);
void effCallPCPThunderSphereLowRotate(void *);
void effPCPFlashPolyRingExp2Delete(void *);
void *effPCPThunderSpherePolyLowCopy(void *);
void effPCPThunderSpherePolyLowSetPosition(void *, void *);
void effPCPThunderSpherePolyLowSetScale(void *, f32);
void effPCPThunderSpherePolyLowSetColor(void *, u32);
void *effPCPSpiralBEDCreatePCF(void *);
void effPCPSpiralBEDDraw(void *);
void effPCPSpiralBEDDelete(void *);
void *effPCPSpiralBEDCopy(void *);
void effPCPSpiralBEDSetPosition(void *, void *);
void effPCPSpiralBEDSetScale(void *, f32);
void effPCPSpiralBEDSetColor(void *, u32);
void *effPCPAgiFireballSpCreatePCF(void *);
void *effPCPAgiFireballSpCopy(void *);
void *effPCPAgiFireballSp2CreatePCF(void *);
void *effPCPAgiFireballSp2Copy(void *);
void *effPCPBezierBillCreatePCF(void *);
void effPCPBezierBillDraw(void *);
void effPCPBezierBillDelete(void *);
void *effPCPBezierBillCopy(void *);
void effPCPBezierBillSetPosition(void *, void *);
void effPCPBezierBillSetColor(void *, u32);
void *effPCPCrackCreatePCF(void *);
void effPCPCrackDraw(void *);
void effPCPCrackDelete(void *);
void *effPCPCrackCopy(void *);
void effPCPCrackSetPosition(void *, void *);
void effPCPCrackSetColor(void *, u32);
void *effPCPHelperBEDCreatePCF(void *);
void effPCPHelperBEDDraw(void *);
void effPCPHelperBEDDelete(void *);
void *effPCPHelperBEDCopy(void *);
void effPCPHelperBEDSetPosition(void *, void *);
void effPCPHelperBEDSetScale(void *, f32);
void effPCPHelperBEDSetColor(void *, u32);
void *effPCPFlashPolyCircleCreatePCF(void *);
void effPCPFlashPolyCircleDraw(void *);
void effFunc10(void *);
void *effPCPFlashPolyCircleCopy(void *);
void effPCPFlashPolyCircleSetPosition(void *, void *);
void effPCPFlashPolyCircleSetScale(void *, f32);
void effPCPFlashPolyCircleSetColor(void *, u32);
void *effPCPFlashPolyRingExpHCreatePCF(void *);
void effPCPFlashPolyRingExpDraw(void *);
void effCallFlashPolyListHDelete(void *);
void *effPCPFlashPolyRingExpHCopy(void *);
void effPCPFlashPolyRingExpHSetPosition(void *, void *);
void effPCPFlashPolyRingExpHSetScale(void *, f32);
void effPCPFlashPolyRingExpHSetRotateMatrix(void *, void *);
void effPCPFlashPolyRingExpSetColor(void *, u32);
void *effPCPBossBaelCreatePCF(void *);
void effPCPBossBaelDraw(void *);
void effPCPBossBaelDelete(void *);
void *effPCPBossBaelCopy(void *);
void effLeaf289(void *, void *);
void effPCPBossBaelSetColor(void *, u32);
void *effLeaf292(void *);
void effLeaf295(void *);
void effLeaf294(void *);
void *effLeaf293(void *);
void effLeaf296(void *, u32);
void *effPcpKind88CreateDisabled(void *);
void effPcpKind88DispatchNoOp(void *);
void effPcpKind88ReleaseNoOp(void *);
void *effPcpKind88DuplicateDisabled(void *);
void effPcpKind88ScaleNoOp(void *, f32);
void effPcpKind88ValueNoOp(void *, u32);
void *effLeaf298(void *);
void effLeaf301(void *);
void effLeaf300(void *);
void *effLeaf299(void *);
void effLeaf302(void *, void *);
void effLeaf303(void *, u32);
void *effLeaf305(void *);
void effLeaf308(void *);
void effLeaf307(void *);
void *effLeaf306(void *);
void effLeaf309(void *, u32);
void *effLeaf311(void *);
void effLeaf314(void *);
void effLeaf313(void *);
void *effLeaf312(void *);
void effLeaf315(void *, u32);
void *effLeaf317(void *);
void effLeaf320(void *);
void effLeaf319(void *);
void *effLeaf318(void *);
void effLeaf321(void *, void *);
void effLeaf322(void *, u32);
void *effLeaf324(void *);
void effLeaf327(void *);
void effLeaf326(void *);
void *effLeaf325(void *);
void effLeaf328(void *, u32);
void effLeaf329(void *);
void *effPcpKind95CreateDisabled(void *);
void effPcpKind95DispatchNoOp(void *);
void effPcpKind95ReleaseNoOp(void *);
void *effPcpKind95DuplicateDisabled(void *);
void effPcpKind95ValueNoOp(void *, u32);
void *effPCPCrack2CreatePCF(void *);
void effPCPCrack2Draw(void *);
void effPCPCrack2Delete(void *);
void *effPCPCrack2Copy(void *);
void effPCPCrack2SetPosition(void *, void *);
void effPCPCrack2SetColor(void *, u32);
void *effPCPBossMETATOBeamCreatePCF(void *);
void effPCPBossMETATOBeamDraw(void *);
void effPCPBossMETATOBeamDelete(void *);
void *effPCPBossMETATOBeamCopy(void *);
void effPCPBossMETATOBeamSetPosition(void *, void *);
void effPCPBossMETATOBeamSetColor(void *, u32);

void *effPCPThunderSpherePolyDataList[9] = {
    &effPartPCPDataTableEntry10,
    &effPartPCPDataTableEntry11,
    &effPartPCPDataTableEntry12,
    &effPartPCPDataTableEntry13,
    &effPartPCPDataTableEntry14,
    &effPartPCPDataTableEntry15,
    &effPartPCPDataTableEntry16,
    &effPartPCPDataTableEntry17,
    &effPartPCPDataTableEntry18,
};

void *effPCPThunderPolyDataList[22] = {
    &effPartPCPDataTableEntry19,
    &effPartPCPDataTableEntry20,
    &effPartPCPDataTableEntry21,
    &effPartPCPDataTableEntry22,
    &effPartPCPDataTableEntry23,
    &effPartPCPDataTableEntry24,
    &effPartPCPDataTableEntry25,
    &effPartPCPDataTableEntry26,
    &effPartPCPDataTableEntry27,
    &effPartPCPDataTableEntry28,
    &effPartPCPDataTableEntry29,
    &effPartPCPDataTableEntry30,
    &effPartPCPDataTableEntry31,
    &effPartPCPDataTableEntry32,
    &effPartPCPDataTableEntry33,
    &effPartPCPDataTableEntry34,
    &effPartPCPDataTableEntry35,
    &effPartPCPDataTableEntry36,
    &effPartPCPDataTableEntry37,
    &effPartPCPDataTableEntry38,
    &effPartPCPDataTableEntry39,
    &effPartPCPDataTableEntry40,
};

void *effPCPThunderRadiatePolyDataList[1] __attribute__((section(".data"))) = {
    &effPartPCPDataTableEntry41,
};

void *effPCPMultiBlurExpDataList[2] __attribute__((section(".data"))) = {
    &effPartPCPDataTableEntry42,
    &effPartPCPDataTableEntry43,
};

void *effPCPSparkleSphereDataList[1] __attribute__((section(".data"))) = {
    &effPartPCPDataTableEntry9,
};

void *effPCPFlashPolySphereDataList[1] __attribute__((section(".data"))) = {
    &effPartPCPDataTableEntry44,
};

void *effPCPMultiFilterExpDataList[2] __attribute__((section(".data"))) = {
    &effPartPCPDataTableEntry45,
    &effPartPCPDataTableEntry46,
};

void *effPCPBlinkMultiBlurDataList[1] __attribute__((section(".data"))) = {
    &effPartPCPDataTableEntry47,
};

void **effPartPCPDataTable[28] = {
    effPCPThunderSpherePolyDataList,
    effPCPThunderPolyDataList,
    effPCPThunderRadiatePolyDataList,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    effPCPMultiBlurExpDataList,
    NULL,
    NULL,
    effPCPSparkleSphereDataList,
    effPCPFlashPolySphereDataList,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    effPCPMultiFilterExpDataList,
    effPCPBlinkMultiBlurDataList,
};

dds3EffectFunc_t dds3EffectFuncTable[8] = {
    { dds3EffectLoad, dds3EffectDraw_dds3Effect, dds3EffectDelete, dds3EffectCopy, dds3EffectSetPosition, dds3EffectSetScale, dds3EffectSetRotateMatrix, effSetEffectColorSubSub, NULL, NULL },
    { (void *(*)(void *))dds3EffectPartP2ABillboardCreate, (void (*)(void *))dds3BillboardDraw, (void (*)(void *))dds3BillboardDelete, (void *(*)(void *))dds3BillboardCopy, (void (*)(void *, void *))dds3BillboardSetPosition, dds3EffectPartBillboardScale, NULL, dds3PCPSparkleSphereDrawSub, NULL, NULL },
    { (void *(*)(void *))dds3EffectPartTMXBillboardCreate, (void (*)(void *))dds3BillboardDraw, (void (*)(void *))dds3BillboardDelete, (void *(*)(void *))dds3BillboardCopy, (void (*)(void *, void *))dds3BillboardSetPosition, dds3EffectPartBillboardScale, NULL, dds3PCPSparkleSphereDrawSub, NULL, NULL },
    { (void *(*)(void *))dds3EffectPartModelCreate, dds3EffectPartModelDraw, dds3EffectPartModelDelete, dds3EffectPartModelCopy, dds3EffectPartModelSetPosition, dds3EffectPartModelSetScale, (void (*)(void *, void *))dds3EffectPartModelSetRotateMatrix, dds3EffectPartModelSetColor, NULL, NULL },
    { (void *(*)(void *))dds3EffectPCPCreate, (void (*)(void *))dds3EffectPCPDraw, (void (*)(void *))dds3EffectPCPDelete, (void *(*)(void *))dds3EffectPCPCopy, dds3EffectPCPSetPosition, dds3EffectPCPSetScale, dds3EffectPCPSetRotateMatrix, dds3EffectPCPSetColor, NULL, dds3EffectPCPSetArea },
    { dds3P2APolygonCreate, dds3P2APolygonDraw, dds3P2APolygonDelete, dds3P2APolygonCopy, dds3P2APolygonSetPosition, dds3P2APolygonSetScale, dds3P2APolygonSetRotateMatrix, dds3P2APolygonSetRGBA, dds3P2APolygonSetDirection, NULL },
    { (void *(*)(void *))dds3EffectPCPCreate, (void (*)(void *))dds3EffectPCPDraw, (void (*)(void *))dds3EffectPCPDelete, (void *(*)(void *))dds3EffectPCPCopy, dds3EffectPCPSetPosition, dds3EffectPCPSetScale, dds3EffectPCPSetRotateMatrix, dds3EffectPCPSetColor, dds3EffectPCPSetDirection, dds3EffectPCPSetArea },
    { Create_effPartsList, effPartsDraw, effPartsDelete, effPartsCreateFinish, effPartsPositionSet, effPartsScaleSet, effPartsRotateMatrixSet, ColorSet, NULL, NULL },
};

sdfVector4_t effPartModelPosInit __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
sdfVector4_t effPartModelRotInit __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
sdfVector4_t effPartModelScaleInit __attribute__((aligned(16))) = { 1.0f, 1.0f, 1.0f, 0.0f };
dds3EffectFunc_t EffectPCPFuncTable[98] = {
    { effPCPThunderSpherePolyCreate, effPCPThunderSpherePolyLowDraw, effPCPFlashPolyCircleDelete, NULL, effPCPShockWave01DrawSub, effPCPThunderSphereExpDrawSub, NULL, effPCPShockWave01DrawSub2, NULL, NULL },
    { effPCP_ThunderPolyCreate, effPCP_ThunderPolyDraw, effPCP_ThunderPolyDelete, NULL, effPCP_ThunderPolyLowSetPosition, NULL, NULL, effPCPRadiateThunderDrawSub, NULL, NULL },
    { effPCP_ThunderRadiatePolyCreate, effPCP_ThunderRadiatePolyDraw, effPCP_ThunderRadiatePolyDelete, NULL, effPCP_ThunderRadiatePolySetPosition, NULL, NULL, effPCP_ThunderRadiatePolySetColor, NULL, NULL },
    { effPCPHama01Create, effPCPHama01Draw, effPCPHama01Delete, effPCPHama01Copy, effPCPHama01SetPosition, effPCPHama01SetScale, NULL, effPCPHama01SetColor, NULL, NULL },
    { effPCPHama02Create, effPCPHama02Draw, effPCPHama02Delete, effPCPHama02Copy, effPCPHama02SetPosition, effPCPHama02SetScale, NULL, effPCPHama02SetColor, NULL, NULL },
    { effPCPHamaonCreate, effPCPHamaonDraw, effPCPHamaonDelete, effPCPHamaonCopy, effPCPHamaonSetPosition, effPCPHamaonSetScale, NULL, effPCPHamaonSetColor, NULL, NULL },
    { effPCPMudoCreate, effPCPMudoDraw, effPCPMudoDelete, effPCPMudoCopy, effPCPMudoSetPosition, effPCPMudoSetScale, NULL, effPCPMudoSetColor, NULL, NULL },
    { effPCPHwave01Create, effPCPHwave01Draw, effPCPHwave01Delete, effPCPHwave01Copy, effPCPHwave01SetPosition, NULL, NULL, effPCPHwave01SetColor, NULL, NULL },
    { effPCP_ThunderPolyLowCreate, effPCP_ThunderPolyLowDraw, effPCP_ThunderPolyLowDelete, NULL, effPCP_ThunderPolySetPosition, NULL, NULL, effPCP_ThunderPolyLowSetColor, NULL, NULL },
    { effPCPZiodin01Create, effPCPZiodin01Draw, effPCPZiodin01Delete, effPCPZiodin01Copy, effPCPZiodin01SetPosition, NULL, NULL, effPCPZiodin01SetColor, NULL, NULL },
    { effPCPMultiBlurExpCreate, effPCPMultiBlurExpDraw, effBTLSys_Encount_Delete2, NULL, effPCPMultiBlurExpSetPosition, NULL, NULL, effPCPMultiBlurExpSetColor, NULL, effPCPMultiBlurExpSetArea },
    { effPCPMHanma01Create, effPCPMHanma01Draw, effPCPMHanma01Delete, effPCPMHanma01Copy, effPCPMHanma01SetPosition, NULL, NULL, effPCPMHanma01SetColor, NULL, NULL },
    { effPCPMZiodin01Create, effPCPMZiodin01Draw, effPCPMZiodin01Delete, effPCPMZiodin01Copy, effPCPMZiodin01SetPosition, NULL, NULL, effPCPMZiodin01SetColor, NULL, NULL },
    { effPCPSparkleSphereCreate, effPCPSparkleSphereDraw, effBTLSys_Nego_Kyusyu_Delete, NULL, effPCPSparkleSphereSetPosition, NULL, NULL, effPCPSparkleSphereSetColor, NULL, effPCPSparkleSphereSetArea },
    { effPCPFlashPolySphereCreate, effPCPFlashPolySphereDraw, effPCPFlashPolySphereDelete, NULL, effPCPFlashPolySphereSetPosition, effPCPFlashPolySphereSetScale, NULL, effPCPFlashPolySphereSetColor, NULL, NULL },
    { effPCPFlashPolySphereCreatePCF, effPCPFlashPolySphereDraw, effPCPFlashPolySphereDelete, effPCPFlashPolySphereCopy, effPCPFlashPolySphereSetPosition, effPCPFlashPolySphereSetScale, NULL, effPCPFlashPolySphereSetColor, NULL, NULL },
    { effPCPFlashPolyRingCreatePCF, effPCPFlashPolyRingDraw, effPCPFlashPolyRingDelete, effPCPFlashPolyRingCopy, effPCPFlashPolyRingSetPosition, effPCPFlashPolyRingSetScale, NULL, effPCPFlashPolyRingSetColor, NULL, NULL },
    { effPCPFlashPolyPipeCreatePCF, effPCPFlashPolyPipeDraw, effPCPFlashPolyPipeDelete, effPCPFlashPolyPipeCopy, effPCPFlashPolyPipeSetPosition, effPCPFlashPolyPipeSetScale, NULL, effPCPFlashPolyPipeSetColor, NULL, NULL },
    { effPCPFlashPolySphere2CreatePCF, effPCPFlashPolySphere2Draw, effPCPFlashPolySphere2Delete, effPCPFlashPolySphere2Copy, effPCPFlashPolySphere2SetPosition, effPCPFlashPolySphere2SetScale, NULL, effPCPFlashPolySphere2SetColor, NULL, NULL },
    { effPCPFlashPolyRingExpCreatePCF, effPCPFlashPolyRingExpHDraw, effPCPFlashPolyRingExpDelete, effPCPFlashPolyRingExpCopy, effPCPFlashPolyRingExpSetPosition, effPCPFlashPolyRingExpSetScale, NULL, effPCPFlashPolyRingExpHSetColor, NULL, NULL },
    { effPCPMZiodin02Create, effPCPMZiodin02Draw, effPCPMZiodin02Delete, effPCPMZiodin02Copy, effPCPMZiodin02SetPosition, NULL, NULL, effPCPMZiodin02SetColor, NULL, NULL },
    { effPCPZiodin02Create, effPCPZiodin02Draw, effPCPZiodin02Delete, effPCPZiodin02Copy, effPCPZiodin02SetPosition, NULL, NULL, effPCPZiodin02SetColor, NULL, NULL },
    { effPCPZiodin03Create, effPCPZiodin03Draw, effPCPZiodin03Delete, effPCPZiodin03Copy, effPCPZiodin03SetPosition, NULL, NULL, effPCPZiodin03SetColor, NULL, NULL },
    { effPCPZiodin04Create, effPCPZiodin04Draw, effPCPZiodin04Delete, effPCPZiodin04Copy, effPCPZiodin04SetPosition, NULL, NULL, effPCPZiodin04SetColor, NULL, NULL },
    { effPCPShockWave01Create, effPCPShockWave01Draw, effBTLSys_Nego_Kaihuku_Delete2, effPCPShockWave01Copy, effPCPShockWave01SetPosition, NULL, NULL, effPCPShockWave01SetColor, NULL, NULL },
    { effPCPHouden01Create, effPCPHouden01Draw, effBTLSys_Nego_Kaihuku_Delete, effPCPHouden01Copy, effPCPHouden01SetPosition, NULL, NULL, effPCPHouden01SetColor, NULL, NULL },
    { effPCPMultiFilterExpCreate, effPCPMultiFilterExpDraw, effBTLSys_Encount_Delete, NULL, effPCPMultiFilterExpSetPosition, NULL, NULL, effPCPMultiFilterExpSetColor, NULL, effPCPMultiFilterExpSetArea },
    { effPCPBlinkMultiBlurCreate, effPCPBlinkMultiBlurDraw, effPCPBlinkMultiBlurDelete, NULL, effPCPBlinkMultiBlurSetPosition, NULL, NULL, effPCPBlinkMultiBlurSetColor, NULL, effPCPBlinkMultiBlurSetArea },
    { effPCPMultiFilterExpCreatePCF, effPCPMultiFilterExpDraw, effBTLSys_Encount_Delete, effPCPMultiFilterExpCopy, effPCPMultiFilterExpSetPosition, NULL, NULL, effPCPMultiFilterExpSetColor, NULL, effPCPMultiFilterExpSetArea },
    { effPCPShockWave02Create, effPCPShockWave02Draw, effPCPShockWave02Delete, effPCPShockWave02Copy, effPCPShockWave02SetPosition, NULL, NULL, effPCPShockWave02SetColor, NULL, NULL },
    { effPCPFilterExpCreatePCF, effPCPFilterExpDraw, effPCPFilterExpDelete, effPCPFilterExpCopy, NULL, NULL, NULL, effPCPFilterExpSetColor, NULL, NULL },
    { effPCPBlurExpCreatePCF, effPCPBlurExpDraw, effPCPBlurExpDelete, effPCPBlurExpCopy, NULL, NULL, NULL, effPCPBlurExpSetColor, NULL, NULL },
    { effPCPDistortionBlurExpCreatePCF, effPCPDistortionBlurExpDraw, effPCPDistortionBlurExpDelete, effPCPRippleBlurExpCopy, effPCPDistortionBlurExpSetPosition, NULL, NULL, effPCPDistortionBlurExpSetColor, NULL, NULL },
    { effPCPMultiBlurExp2CreatePCF, effPCPMultiBlurExp2Draw, effPCPMultiBlurExp2Delete, effPCPMultiBlurExp2Copy, effPCPMultiBlurExp2SetPosition, NULL, NULL, effPCPMultiBlurExp2SetColor, NULL, NULL },
    { effPCPMegidora01Create, effPCPMegidora01Draw, effPCPMegidora01Delete, effPCPMegidora01Copy, effPCPMegidora01SetPosition, NULL, NULL, effPCPMegidora01SetColor, NULL, NULL },
    { effPCPMegidoraon01Create, effPCPMegidoraon01Draw, effPCPMegidoraon01Delete, effPCPMegidoraon01Copy, effPCPMegidoraon01SetPosition, NULL, NULL, effPCPMegidoraon01SetColor, NULL, NULL },
    { effPCPBreathCreatePCF, effPCPBreathDraw, effPCPBreathDelete, effPCPBreathCopy, effPCPBreathSetPosition, NULL, effPCPBreathSetRotateMatrix, effPCPBreathSetColor, NULL, NULL },
    { effPCPHamaCardSpiralCreatePCF, effPCPHamaCardSpiralDraw, effPCPHamaCardSpiralDelete, effPCPHamaCardSpiralCopy, effPCPHamaCardSpiralSetPosition, effPCPHamaCardSpiralSetScale, NULL, effPCPHamaCardSpiralSetColor, NULL, NULL },
    { effPCPHamaCardSphereCreatePCF, effPCPHamaCardSphereDraw, effPCPHamaCardSphereDelete, effPCPHamaCardSphereCopy, effPCPHamaCardSphereSetPosition, effPCPHamaCardSphereSetScale, NULL, effPCPHamaCardSphereSetColor, NULL, NULL },
    { effPCPAgiraoFireCreatePCF, effPCPAgiraoFireDraw, effPCPAgiraoFireDelete, effPCPAgiraoFireCopy, effPCPAgiraoFireSetPosition, NULL, NULL, effPCPAgiraoFireSetColor, NULL, NULL },
    { effPCPBufuIceCreate, effPCPBufuIceDraw, effPCPBufuIceDelete, effPCPBufuIceCopy, effPCPBufuIceSetPosition, effPCPBufuIceSetScale, effPCPBufuIceSetRotateMatrix, effPCPBufuIceSetColor, NULL, NULL },
    { effPCPBufudnIceCreate, effPCPBufudnIceDraw, effCallEffectPartDelete, effPCPBufudnIceCopy, effPCPBufudnIceSetPosition, effPCPBufudnIceSetScale, effPCPBufudnIceSetRotateMatrix, effPCPBufudnIceSetColor, NULL, NULL },
    { effPCPBufuraIceCreate, effPCPBufuraIceDraw, effCallEffectPartDelete2, effPCPBufuraIceCopy, effPCPBufuraIceSetPosition, effPCPBufuraIceSetScale, effPCPBufuraIceSetRotateMatrix, effPCPBufuraIceSetColor, NULL, NULL },
    { effLeaf147, effLeaf150, effLeaf149, effLeaf148, NULL, NULL, NULL, NULL, NULL, NULL },
    { effPCPAgiFireballCreatePCF, effPCPAgiFireball2DrawSub, effPCPAgiFireballDelete, effPCPAgiFireballCopy, effPCPAgiFireballSetPosition, NULL, effPCPAgiFireballSetRotateMatrix, effPCPAgiFireballSetColor, NULL, NULL },
    { effPCPNeedleSpiralCreatePCF, effPCPNeedleSpiralDraw, effPCPNeedleSpiralDelete, effPCPNeedleSpiralCopy, effPCPNeedleSpiralSetPosition, NULL, NULL, effPCPNeedleSpiralSetColor, NULL, NULL },
    { effCallChipAllocDirect2, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy00_0, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect3, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy00_1, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect4, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy01_0, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect5, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy01_1, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect6, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy02_0, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect7, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy02_1, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect8, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy03_0, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect9, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy03_1, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect10, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy04_0, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect11, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy04_1, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effPCPRippleBlurExpCreatePCF, effPCPRippleBlurExpDraw, effPCPRippleBlurExpDelete, effPCPDistortionBlurExpCopy, effPCPRippleBlurExpSetPosition, NULL, NULL, effPCPRippleBlurExpSetColor, NULL, NULL },
    { effPCPThunderSphereExpCreatePCF, effPCPThunderSphereExpDraw, effPCPThunderSphereExpDelete, effPCPThunderSphereExpCopy, effPCPThunderSphereExpSetPosition, effPCPThunderSphereExpSetScale, NULL, effPCPThunderSphereExpSetColor, NULL, NULL },
    { effPCPRingPolyExpCreatePCF, effPCPRingPolyExpDraw, effPCPRingPolyExpDelete, effPCPRingPolyExpCopy, effPCPRingPolyExpSetPosition, effPCPRingPolyExpSetScale, effPCPRingPolyExp2SetRotateMatrix, effPCPRingPolyExpSetColor, effPCPRingPolyExpSetDirection, NULL },
    { effPCPWhirlwindPolyCreatePCF, effPCPWhirlwindPolyDraw, effPCPWhirlwindPolyDelete, effPCPWhirlwindPolyCopy, effPCPWhirlwindPolySetPosition, effPCPWhirlwindPolySetScale, effPCPWhirlwind3PolySetRotateMatrix, effPCPWhirlwindPolySetColor, NULL, NULL },
    { effCallChipAllocDirect12, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy05_0, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effCallChipAllocDirect13, effPCPEyeThunderDraw, effPCPEyeThunderDelete, effPCPEyeThunderCopy05_1, effPCPEyeThunderSetPosition, NULL, NULL, effPCPEyeThunderSetColor, NULL, NULL },
    { effPCPRadiateThunderCreatePCF, effPCPRadiateThunderDraw, effPCPRadiateThunderDelete, effPCPRadiateThunderCopy, effPCPRadiateThunderSetPosition, effPCPRadiateThunderSetScale, NULL, effPCPRadiateThunderSetColor, NULL, NULL },
    { effPCPWwSpherePolyCreatePCF, effPCPWwSpherePolyDraw, effPCPWwSpherePolyDelete, effPCPWwSpherePolyCopy, effPCPWwSpherePolySetPosition, effPCPWwSpherePolySetScale, effPCPWwSpherePolySetRotateMatrix, effPCPWwSpherePolySetColor, NULL, NULL },
    { effPCPFlashPolySphereExpCreatePCF, effPCPFlashPolySphereExpDraw, effPCPFlashPolySphereExpDelete, effPCPFlashPolySphereExpCopy, effPCPFlashPolySphereExpSetPosition, effPCPFlashPolySphereExpSetScale, NULL, effPCPFlashPolySphereExpSetColor, NULL, NULL },
    { effPCPFlashPolySphere2ExpCreatePCF, effPCPFlashPolySphere2ExpDraw, effPCPFlashPolySphere2ExpDelete, effPCPFlashPolySphere2ExpCopy, effPCPFlashPolySphere2ExpSetPosition, effPCPFlashPolySphere2ExpSetScale, NULL, effPCPFlashPolySphere2ExpSetColor, NULL, NULL },
    { effPCPFlashPolySphere3CreatePCF, effPCPFlashPolySphere3Draw, effPCPFlashPolySphere3Delete, effPCPFlashPolySphere3Copy, effPCPFlashPolySphere3SetPosition, effPCPFlashPolySphere3SetScale, NULL, effPCPFlashPolySphere3SetColor, NULL, NULL },
    { effPCPFlashPolyRingExp2CreatePCF, effPCPFlashPolyRingExp2Draw, effFunc9, effPCPFlashPolyRingExp2Copy, effPCPFlashPolyRingExp2SetPosition, effPCPFlashPolyRingExp2SetScale, NULL, effPCPFlashPolyRingExp2SetColor, NULL, NULL },
    { effPCPWhirlwind2PolyCreatePCF, effPCPWhirlwind2PolyDraw, effPCPWhirlwind2PolyDelete, effPCPWhirlwind2PolyCopy, effPCPWhirlwind2PolySetPosition, effPCPWhirlwind2PolySetScale, effPCPWhirlwind2PolySetRotateMatrix, effPCPWhirlwind2PolySetColor, NULL, NULL },
    { effPCPMakkaCreat, effPCPMakkaDraw, effPCPMakkaDelete, effPCPMakkaCopy, effPCPMakkaSetPosition, effPCPMakkaSetScale, NULL, effPCPMakkaSetColor, NULL, NULL },
    { effPCPRingPolyExp2CreatePCF, effPCPRingPolyExp2Draw, effPCPRingPolyExp2Delete, effPCPRingPolyExp2Copy, effPCPRingPolyExp2SetPosition, effPCPRingPolyExp2SetScale, NULL, effPCPRingPolyExp2SetColor, NULL, NULL },
    { effPCPAgiFireball2CreatePCF, effPCPAgiFireball2Draw, effPCPAgiFireball2Delete, effPCPAgiFireball2Copy, effPCPAgiFireball2SetRotateMatrix, NULL, effCallPCPAgiFireballSetRotateMatri, effPCPAgiFireball2SetColor, NULL, NULL },
    { effPCPRadiateBEDCreatePCF, effPCPRadiateBEDDraw, effPCPRadiateBEDDelete, effPCPRadiateBEDCopy, effPCPRadiateBEDSetPosition, effPCPRadiateBEDSetScale, NULL, effPCPRadiateBEDSetColor, NULL, NULL },
    { effAPPoint4CreatePCF, effAPPoint4Draw, effAPPoint4Delete, effAPPoint4Copy, NULL, NULL, NULL, NULL, NULL, NULL },
    { effPCPRadiateTBCreatePCF, effPCPRadiateTBDraw, effPCPRadiateTBDelete, effPCPRadiateTBCopy, effPCPRadiateTBSetPosition, NULL, NULL, effPCPRadiateTBSetColor, NULL, NULL },
    { effPCPWhirlwind3PolyCreatePCF, effPCPWhirlwind3PolyDraw, effPCPWhirlwind3PolyDelete, effPCPWhirlwind3PolyCopy, effPCPWhirlwind3PolySetPosition, effPCPWhirlwind3PolySetScale, effCallMultMatrixNoArg, effPCPWhirlwind3PolySetColor, NULL, NULL },
    { effPCPThunderSpherePolyCreatePCF, effPCPThunderSpherePolyLowDraw, effPCPFlashPolyCircleDelete, effPCPThunderSpherePolyCopy, effPCPShockWave01DrawSub, effPCPThunderSphereExpDrawSub, NULL, effPCPShockWave01DrawSub2, NULL, NULL },
    { effCallEffectPCPDataGetData, effCallPCPThunderSphereLowRotate, effPCPFlashPolyRingExp2Delete, effPCPThunderSpherePolyLowCopy, effPCPThunderSpherePolyLowSetPosition, effPCPThunderSpherePolyLowSetScale, NULL, effPCPThunderSpherePolyLowSetColor, NULL, NULL },
    { effPCPSpiralBEDCreatePCF, effPCPSpiralBEDDraw, effPCPSpiralBEDDelete, effPCPSpiralBEDCopy, effPCPSpiralBEDSetPosition, effPCPSpiralBEDSetScale, NULL, effPCPSpiralBEDSetColor, NULL, NULL },
    { effPCPAgiFireballSpCreatePCF, effPCPAgiFireball2DrawSub, effPCPAgiFireballDelete, effPCPAgiFireballSpCopy, effPCPAgiFireballSetPosition, NULL, effPCPAgiFireballSetRotateMatrix, effPCPAgiFireballSetColor, NULL, NULL },
    { effPCPAgiFireballSp2CreatePCF, effPCPAgiFireball2DrawSub, effPCPAgiFireballDelete, effPCPAgiFireballSp2Copy, effPCPAgiFireballSetPosition, NULL, effPCPAgiFireballSetRotateMatrix, effPCPAgiFireballSetColor, NULL, NULL },
    { effPCPBezierBillCreatePCF, effPCPBezierBillDraw, effPCPBezierBillDelete, effPCPBezierBillCopy, effPCPBezierBillSetPosition, NULL, NULL, effPCPBezierBillSetColor, NULL, NULL },
    { effPCPCrackCreatePCF, effPCPCrackDraw, effPCPCrackDelete, effPCPCrackCopy, effPCPCrackSetPosition, NULL, NULL, effPCPCrackSetColor, NULL, NULL },
    { effPCPHelperBEDCreatePCF, effPCPHelperBEDDraw, effPCPHelperBEDDelete, effPCPHelperBEDCopy, effPCPHelperBEDSetPosition, effPCPHelperBEDSetScale, NULL, effPCPHelperBEDSetColor, NULL, NULL },
    { effPCPFlashPolyCircleCreatePCF, effPCPFlashPolyCircleDraw, effFunc10, effPCPFlashPolyCircleCopy, effPCPFlashPolyCircleSetPosition, effPCPFlashPolyCircleSetScale, NULL, effPCPFlashPolyCircleSetColor, NULL, NULL },
    { effPCPFlashPolyRingExpHCreatePCF, effPCPFlashPolyRingExpDraw, effCallFlashPolyListHDelete, effPCPFlashPolyRingExpHCopy, effPCPFlashPolyRingExpHSetPosition, effPCPFlashPolyRingExpHSetScale, effPCPFlashPolyRingExpHSetRotateMatrix, effPCPFlashPolyRingExpSetColor, NULL, NULL },
    { effPCPBossBaelCreatePCF, effPCPBossBaelDraw, effPCPBossBaelDelete, effPCPBossBaelCopy, effLeaf289, NULL, NULL, effPCPBossBaelSetColor, NULL, NULL },
    { effLeaf292, effLeaf295, effLeaf294, effLeaf293, NULL, NULL, NULL, effLeaf296, NULL, NULL },
    { effPcpKind88CreateDisabled, effPcpKind88DispatchNoOp, effPcpKind88ReleaseNoOp, effPcpKind88DuplicateDisabled, NULL, effPcpKind88ScaleNoOp, NULL, effPcpKind88ValueNoOp, NULL, NULL },
    { effLeaf298, effLeaf301, effLeaf300, effLeaf299, effLeaf302, NULL, NULL, effLeaf303, NULL, NULL },
    { effLeaf305, effLeaf308, effLeaf307, effLeaf306, NULL, NULL, NULL, effLeaf309, NULL, NULL },
    { effLeaf311, effLeaf314, effLeaf313, effLeaf312, NULL, NULL, NULL, effLeaf315, NULL, NULL },
    { effLeaf317, effLeaf320, effLeaf319, effLeaf318, effLeaf321, NULL, NULL, effLeaf322, NULL, NULL },
    { effLeaf324, effLeaf327, effLeaf326, effLeaf325, NULL, NULL, NULL, effLeaf328, NULL, NULL },
    { effLeaf317, effLeaf329, effLeaf319, effLeaf318, effLeaf321, NULL, NULL, effLeaf322, NULL, NULL },
    { effPcpKind95CreateDisabled, effPcpKind95DispatchNoOp, effPcpKind95ReleaseNoOp, effPcpKind95DuplicateDisabled, NULL, NULL, NULL, effPcpKind95ValueNoOp, NULL, NULL },
    { effPCPCrack2CreatePCF, effPCPCrack2Draw, effPCPCrack2Delete, effPCPCrack2Copy, effPCPCrack2SetPosition, NULL, NULL, effPCPCrack2SetColor, NULL, NULL },
    { effPCPBossMETATOBeamCreatePCF, effPCPBossMETATOBeamDraw, effPCPBossMETATOBeamDelete, effPCPBossMETATOBeamCopy, effPCPBossMETATOBeamSetPosition, NULL, NULL, effPCPBossMETATOBeamSetColor, NULL, NULL },
};
#endif
