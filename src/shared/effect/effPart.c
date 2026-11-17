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
void dds3Leaf34(void *, void *);
void dds3P2APolygonSetRGBA(void *, u32);
void dds3Leaf35(void *, void *);
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
void effLeaf202(void *, void *);
void effLeaf203(void *, u32);
void *effPCPHama01Create(void *);
void effPCPHama01Draw(void *);
void effPCPHama01Delete(void *);
void *effPCPHama01Copy(void *);
void effLeaf62(void *, void *);
void effLeaf63(void *, f32);
void effLeaf64(void *, u32);
void *effPCPHama02Create(void *);
void effPCPHama02Draw(void *);
void effPCPHama02Delete(void *);
void *effPCPHama02Copy(void *);
void effLeaf65(void *, void *);
void effLeaf66(void *, f32);
void effLeaf67(void *, u32);
void *effPCPHamaonCreate(void *);
void effPCPHamaonDraw(void *);
void effPCPHamaonDelete(void *);
void *effPCPHamaonCopy(void *);
void effLeaf68(void *, void *);
void effLeaf69(void *, f32);
void effLeaf70(void *, u32);
void *effPCPMudoCreate(void *);
void effPCPMudoDraw(void *);
void effPCPMudoDelete(void *);
void *effPCPMudoCopy(void *);
void effLeaf74(void *, void *);
void effLeaf75(void *, f32);
void effLeaf76(void *, u32);
void *effPCPHwave01Create(void *);
void effPCPHwave01Draw(void *);
void effPCPHwave01Delete(void *);
void *effPCPHwave01Copy(void *);
void effLeaf77(void *, void *);
void effLeaf78(void *, u32);
void *effPCP_ThunderPolyLowCreate(void *);
void effPCP_ThunderPolyLowDraw(void *);
void effPCP_ThunderPolyLowDelete(void *);
void effPCP_ThunderPolySetPosition(void *, void *);
void effLeaf201(void *, u32);
void *effPCPZiodin01Create(void *);
void effPCPZiodin01Draw(void *);
void effPCPZiodin01Delete(void *);
void *effPCPZiodin01Copy(void *);
void effLeaf79(void *, void *);
void effLeaf81(void *, u32);
void *effPCPMultiBlurExpCreate(void *);
void effPCPMultiBlurExpDraw(void *);
void effBTLSys_Encount_Delete2(void *);
void effLeaf109(void *, void *);
void effLeaf110(void *, u32);
void effLeaf111(void *, f32);
void *effPCPMHanma01Create(void *);
void effPCPMHanma01Draw(void *);
void effPCPMHanma01Delete(void *);
void *effPCPMHanma01Copy(void *);
void effLeaf71(void *, void *);
void effLeaf73(void *, u32);
void *effPCPMZiodin01Create(void *);
void effPCPMZiodin01Draw(void *);
void effPCPMZiodin01Delete(void *);
void *effPCPMZiodin01Copy(void *);
void effLeaf91(void *, void *);
void effLeaf93(void *, u32);
void *effPCPSparkleSphereCreate(void *);
void effPCPSparkleSphereDraw(void *);
void effBTLSys_Nego_Kyusyu_Delete(void *);
void effLeaf59(void *, void *);
void effLeaf60(void *, u32);
void effLeaf61(void *, f32);
void *effPCPFlashPolySphereCreate(void *);
void effPCPFlashPolySphereDraw(void *);
void effPCPFlashPolySphereDelete(void *);
void effLeaf212(void *, void *);
void effLeaf214(void *, f32);
void effPCPFlashPolySphereSetColor(void *, u32);
void *effPCPFlashPolySphereCreatePCF(void *);
void *effPCPFlashPolySphereCopy(void *);
void *effPCPFlashPolyRingCreatePCF(void *);
void effPCPFlashPolyRingDraw(void *);
void effPCPFlashPolyRingDelete(void *);
void *effPCPFlashPolyRingCopy(void *);
void effLeaf215(void *, void *);
void effLeaf217(void *, f32);
void effPCPFlashPolyRingSetColor(void *, u32);
void *effPCPFlashPolyPipeCreatePCF(void *);
void effPCPFlashPolyPipeDraw(void *);
void effPCPFlashPolyPipeDelete(void *);
void *effPCPFlashPolyPipeCopy(void *);
void effLeaf218(void *, void *);
void effLeaf220(void *, f32);
void effPCPFlashPolyPipeSetColor(void *, u32);
void *effPCPFlashPolySphere2CreatePCF(void *);
void effPCPFlashPolySphere2Draw(void *);
void effPCPFlashPolySphere2Delete(void *);
void *effPCPFlashPolySphere2Copy(void *);
void effLeaf209(void *, void *);
void effLeaf211(void *, f32);
void effPCPFlashPolySphere2SetColor(void *, u32);
void *effPCPFlashPolyRingExpCreatePCF(void *);
void effPCPFlashPolyRingExpHDraw(void *);
void effPCPFlashPolyRingExpDelete(void *);
void *effPCPFlashPolyRingExpCopy(void *);
void effLeaf221(void *, void *);
void effLeaf223(void *, f32);
void effPCPFlashPolyRingExpHSetColor(void *, u32);
void *effPCPMZiodin02Create(void *);
void effPCPMZiodin02Draw(void *);
void effPCPMZiodin02Delete(void *);
void *effPCPMZiodin02Copy(void *);
void effLeaf94(void *, void *);
void effLeaf96(void *, u32);
void *effPCPZiodin02Create(void *);
void effPCPZiodin02Draw(void *);
void effPCPZiodin02Delete(void *);
void *effPCPZiodin02Copy(void *);
void effLeaf82(void *, void *);
void effLeaf84(void *, u32);
void *effPCPZiodin03Create(void *);
void effPCPZiodin03Draw(void *);
void effPCPZiodin03Delete(void *);
void *effPCPZiodin03Copy(void *);
void effLeaf85(void *, void *);
void effLeaf87(void *, u32);
void *effPCPZiodin04Create(void *);
void effPCPZiodin04Draw(void *);
void effPCPZiodin04Delete(void *);
void *effPCPZiodin04Copy(void *);
void effLeaf88(void *, void *);
void effLeaf90(void *, u32);
void *effPCPShockWave01Create(void *);
void effPCPShockWave01Draw(void *);
void effBTLSys_Nego_Kaihuku_Delete2(void *);
void *effPCPShockWave01Copy(void *);
void effLeaf97(void *, void *);
void effLeaf99(void *, u32);
void *effPCPHouden01Create(void *);
void effPCPHouden01Draw(void *);
void effBTLSys_Nego_Kaihuku_Delete(void *);
void *effPCPHouden01Copy(void *);
void effLeaf103(void *, void *);
void effLeaf105(void *, u32);
void *effPCPMultiFilterExpCreate(void *);
void effPCPMultiFilterExpDraw(void *);
void effBTLSys_Encount_Delete(void *);
void effLeaf112(void *, void *);
void effLeaf113(void *, u32);
void effLeaf114(void *, f32);
void *effPCPBlinkMultiBlurCreate(void *);
void effPCPBlinkMultiBlurDraw(void *);
void effPCPBlinkMultiBlurDelete(void *);
void effLeaf106(void *, void *);
void effLeaf107(void *, u32);
void effLeaf108(void *, f32);
void *effPCPMultiFilterExpCreatePCF(void *);
void *effPCPMultiFilterExpCopy(void *);
void *effPCPShockWave02Create(void *);
void effPCPShockWave02Draw(void *);
void effPCPShockWave02Delete(void *);
void *effPCPShockWave02Copy(void *);
void effLeaf100(void *, void *);
void effLeaf102(void *, u32);
void *effPCPFilterExpCreatePCF(void *);
void effPCPFilterExpDraw(void *);
void effPCPFilterExpDelete(void *);
void *effPCPFilterExpCopy(void *);
void effLeaf118(void *, u32);
void *effPCPBlurExpCreatePCF(void *);
void effPCPBlurExpDraw(void *);
void effPCPBlurExpDelete(void *);
void *effPCPBlurExpCopy(void *);
void effLeaf119(void *, u32);
void *effPCPDistortionBlurExpCreatePCF(void *);
void effPCPDistortionBlurExpDraw(void *);
void effPCPDistortionBlurExpDelete(void *);
void *effPCPRippleBlurExpCopy(void *);
void effLeaf120(void *, void *);
void effLeaf121(void *, u32);
void *effPCPMultiBlurExp2CreatePCF(void *);
void effPCPMultiBlurExp2Draw(void *);
void effPCPMultiBlurExp2Delete(void *);
void *effPCPMultiBlurExp2Copy(void *);
void effLeaf115(void *, void *);
void effLeaf116(void *, u32);
void *effPCPMegidora01Create(void *);
void effPCPMegidora01Draw(void *);
void effPCPMegidora01Delete(void *);
void *effPCPMegidora01Copy(void *);
void effLeaf124(void *, void *);
void effLeaf126(void *, u32);
void *effPCPMegidoraon01Create(void *);
void effPCPMegidoraon01Draw(void *);
void effPCPMegidoraon01Delete(void *);
void *effPCPMegidoraon01Copy(void *);
void effLeaf127(void *, void *);
void effLeaf129(void *, u32);
void *effPCPBreathCreatePCF(void *);
void effLeaf130(void *);
void effPCPBreathDelete(void *);
void *effPCPBreathCopy(void *);
void effLeaf131(void *, void *);
void effPCPBreathSetRotateMatrix(void *, void *);
void effLeaf132(void *, u32);
void *effPCPHamaCardSpiralCreatePCF(void *);
void effPCPHamaCardSpiralDraw(void *);
void effPCPHamaCardSpiralDelete(void *);
void *effPCPHamaCardSpiralCopy(void *);
void effLeaf244(void *, void *);
void effPCPHamaCardSpiralSetScale(void *, f32);
void effPCPHamaCardSpiralSetColor(void *, u32);
void *effPCPHamaCardSphereCreatePCF(void *);
void effPCPHamaCardSphereDraw(void *);
void effPCPHamaCardSphereDelete(void *);
void *effPCPHamaCardSphereCopy(void *);
void effLeaf246(void *, void *);
void effLeaf248(void *, f32);
void effPCPHamaCardSphereSetColor(void *, u32);
void *effPCPAgiraoFireCreatePCF(void *);
void effPCPAgiraoFireDraw(void *);
void effPCPAgiraoFireDelete(void *);
void *effPCPAgiraoFireCopy(void *);
void effLeaf133(void *, void *);
void effLeaf134(void *, u32);
void *effPCPBufuIceCreate(void *);
void effPCPBufuIceDraw(void *);
void effPCPBufuIceDelete(void *);
void *effPCPBufuIceCopy(void *);
void effLeaf135(void *, void *);
void effLeaf137(void *, f32);
void effLeaf138(void *, void *);
void effLeaf136(void *, u32);
void *effPCPBufudnIceCreate(void *);
void effPCPBufudnIceDraw(void *);
void effCallEffectPartDelete(void *);
void *effPCPBufudnIceCopy(void *);
void effLeaf139(void *, void *);
void effLeaf141(void *, f32);
void effLeaf142(void *, void *);
void effLeaf140(void *, u32);
void *effPCPBufuraIceCreate(void *);
void effPCPBufuraIceDraw(void *);
void effCallEffectPartDelete2(void *);
void *effPCPBufuraIceCopy(void *);
void effLeaf143(void *, void *);
void effLeaf145(void *, f32);
void effLeaf146(void *, void *);
void effLeaf144(void *, u32);
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
void effLeaf264(void *);
void effPCPNeedleSpiralDelete(void *);
void *effPCPNeedleSpiralCopy(void *);
void effLeaf265(void *, void *);
void effPCPNeedleSpiralSetColor(void *, u32);
void *effCallChipAllocDirect2(void *);
void effLeaf163(void *);
void effPCPEyeThunderDelete(void *);
void *effLeaf151(void *);
void effLeaf164(void *, void *);
void effLeaf165(void *, u32);
void *effCallChipAllocDirect3(void *);
void *effLeaf152(void *);
void *effCallChipAllocDirect4(void *);
void *effLeaf153(void *);
void *effCallChipAllocDirect5(void *);
void *effLeaf154(void *);
void *effCallChipAllocDirect6(void *);
void *effLeaf155(void *);
void *effCallChipAllocDirect7(void *);
void *effLeaf156(void *);
void *effCallChipAllocDirect8(void *);
void *effLeaf157(void *);
void *effCallChipAllocDirect9(void *);
void *effLeaf158(void *);
void *effCallChipAllocDirect10(void *);
void *effLeaf159(void *);
void *effCallChipAllocDirect11(void *);
void *effLeaf160(void *);
void *effPCPRippleBlurExpCreatePCF(void *);
void effPCPRippleBlurExpDraw(void *);
void effPCPRippleBlurExpDelete(void *);
void *effPCPDistortionBlurExpCopy(void *);
void effLeaf122(void *, void *);
void effLeaf123(void *, u32);
void *effPCPThunderSphereExpCreatePCF(void *);
void effPCPThunderSphereExpDraw(void *);
void effPCPThunderSphereExpDelete(void *);
void *effPCPThunderSphereExpCopy(void *);
void effLeaf166(void *, void *);
void effLeaf168(void *, f32);
void effLeaf167(void *, u32);
void *effPCPRingPolyExpCreatePCF(void *);
void effPCPRingPolyExpDraw(void *);
void effPCPRingPolyExpDelete(void *);
void *effPCPRingPolyExpCopy(void *);
void effLeaf169(void *, void *);
void effLeaf170(void *, f32);
void effPCPRingPolyExp2SetRotateMatrix(void *, void *);
void effLeaf171(void *, u32);
void effPCPRingPolyExpSetDirection(void *, void *);
void *effPCPWhirlwindPolyCreatePCF(void *);
void effPCPWhirlwindPolyDraw(void *);
void effPCPWhirlwindPolyDelete(void *);
void *effPCPWhirlwindPolyCopy(void *);
void effLeaf251(void *, void *);
void effLeaf252(void *, f32);
void effPCPWhirlwind3PolySetRotateMatrix(void *, void *);
void effPCPWhirlwindPolySetColor(void *, u32);
void *effCallChipAllocDirect12(void *);
void *effLeaf161(void *);
void *effCallChipAllocDirect13(void *);
void *effLeaf162(void *);
void *effPCPRadiateThunderCreatePCF(void *);
void effPCPRadiateThunderDraw(void *);
void effPCPRadiateThunderDelete(void *);
void *effPCPRadiateThunderCopy(void *);
void effLeaf175(void *, void *);
void effLeaf176(void *, f32);
void effLeaf177(void *, u32);
void *effPCPWwSpherePolyCreatePCF(void *);
void effPCPWwSpherePolyDraw(void *);
void effPCPWwSpherePolyDelete(void *);
void *effPCPWwSpherePolyCopy(void *);
void effLeaf260(void *, void *);
void effLeaf261(void *, f32);
void effPCPWwSpherePolySetRotateMatrix(void *, void *);
void effPCPWwSpherePolySetColor(void *, u32);
void *effPCPFlashPolySphereExpCreatePCF(void *);
void effPCPFlashPolySphereExpDraw(void *);
void effPCPFlashPolySphereExpDelete(void *);
void *effPCPFlashPolySphereExpCopy(void *);
void effLeaf224(void *, void *);
void effLeaf226(void *, f32);
void effPCPFlashPolySphereExpSetColor(void *, u32);
void *effPCPFlashPolySphere2ExpCreatePCF(void *);
void effPCPFlashPolySphere2ExpDraw(void *);
void effPCPFlashPolySphere2ExpDelete(void *);
void *effPCPFlashPolySphere2ExpCopy(void *);
void effLeaf227(void *, void *);
void effLeaf229(void *, f32);
void effPCPFlashPolySphere2ExpSetColor(void *, u32);
void *effPCPFlashPolySphere3CreatePCF(void *);
void effPCPFlashPolySphere3Draw(void *);
void effPCPFlashPolySphere3Delete(void *);
void *effPCPFlashPolySphere3Copy(void *);
void effLeaf230(void *, void *);
void effLeaf232(void *, f32);
void effPCPFlashPolySphere3SetColor(void *, u32);
void *effPCPFlashPolyRingExp2CreatePCF(void *);
void effPCPFlashPolyRingExp2Draw(void *);
void effFunc9(void *);
void *effPCPFlashPolyRingExp2Copy(void *);
void effLeaf233(void *, void *);
void effLeaf235(void *, f32);
void effPCPFlashPolyRingExp2SetColor(void *, u32);
void *effPCPWhirlwind2PolyCreatePCF(void *);
void effPCPWhirlwind2PolyDraw(void *);
void effPCPWhirlwind2PolyDelete(void *);
void *effPCPWhirlwind2PolyCopy(void *);
void effLeaf254(void *, void *);
void effLeaf255(void *, f32);
void effPCPWhirlwind2PolySetRotateMatrix(void *, void *);
void effPCPWhirlwind2PolySetColor(void *, u32);
void *effPCPMakkaCreat(void *);
void effLeaf178(void *);
void effPCPMakkaDelete(void *);
void *effPCPMakkaCopy(void *);
void effLeaf179(void *, void *);
void effLeaf181(void *, f32);
void effLeaf180(void *, u32);
void *effPCPRingPolyExp2CreatePCF(void *);
void effPCPRingPolyExp2Draw(void *);
void effPCPRingPolyExp2Delete(void *);
void *effPCPRingPolyExp2Copy(void *);
void effLeaf172(void *, void *);
void effLeaf173(void *, f32);
void effLeaf174(void *, u32);
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
void effLeaf182(void *, void *);
void effLeaf183(void *, f32);
void effLeaf184(void *, u32);
void *effAPPoint4CreatePCF(void *);
void effAPPoint4Draw(void *);
void effAPPoint4Delete(void *);
void *effAPPoint4Copy(void *);
void *effPCPRadiateTBCreatePCF(void *);
void effPCPRadiateTBDraw(void *);
void effPCPRadiateTBDelete(void *);
void *effPCPRadiateTBCopy(void *);
void effLeaf185(void *, void *);
void effLeaf186(void *, u32);
void *effPCPWhirlwind3PolyCreatePCF(void *);
void effPCPWhirlwind3PolyDraw(void *);
void effPCPWhirlwind3PolyDelete(void *);
void *effPCPWhirlwind3PolyCopy(void *);
void effLeaf257(void *, void *);
void effLeaf258(void *, f32);
void effCallMultMatrixNoArg(void *, void *);
void effPCPWhirlwind3PolySetColor(void *, u32);
void *effPCPThunderSpherePolyCreatePCF(void *);
void *effPCPThunderSpherePolyCopy(void *);
void *effCallEffectPCPDataGetData(void *);
void effCallPCPThunderSphereLowRotate(void *);
void effPCPFlashPolyRingExp2Delete(void *);
void *effLeaf195(void *);
void effLeaf196(void *, void *);
void effLeaf198(void *, f32);
void effLeaf197(void *, u32);
void *effPCPSpiralBEDCreatePCF(void *);
void effPCPSpiralBEDDraw(void *);
void effPCPSpiralBEDDelete(void *);
void *effPCPSpiralBEDCopy(void *);
void effLeaf187(void *, void *);
void effLeaf188(void *, f32);
void effLeaf189(void *, u32);
void *effPCPAgiFireballSpCreatePCF(void *);
void *effPCPAgiFireballSpCopy(void *);
void *effPCPAgiFireballSp2CreatePCF(void *);
void *effPCPAgiFireballSp2Copy(void *);
void *effPCPBezierBillCreatePCF(void *);
void effLeaf206(void *);
void effPCPBezierBillDelete(void *);
void *effPCPBezierBillCopy(void *);
void effLeaf207(void *, void *);
void effLeaf208(void *, u32);
void *effPCPCrackCreatePCF(void *);
void effPCPCrackDraw(void *);
void effPCPCrackDelete(void *);
void *effPCPCrackCopy(void *);
void effLeaf286(void *, void *);
void effLeaf287(void *, u32);
void *effPCPHelperBEDCreatePCF(void *);
void effPCPHelperBEDDraw(void *);
void effPCPHelperBEDDelete(void *);
void *effPCPHelperBEDCopy(void *);
void effLeaf190(void *, void *);
void effLeaf191(void *, f32);
void effLeaf192(void *, u32);
void *effPCPFlashPolyCircleCreatePCF(void *);
void effPCPFlashPolyCircleDraw(void *);
void effFunc10(void *);
void *effPCPFlashPolyCircleCopy(void *);
void effLeaf236(void *, void *);
void effLeaf238(void *, f32);
void effPCPFlashPolyCircleSetColor(void *, u32);
void *effPCPFlashPolyRingExpHCreatePCF(void *);
void effPCPFlashPolyRingExpDraw(void *);
void effCallFlashPolyListHDelete(void *);
void *effPCPFlashPolyRingExpHCopy(void *);
void effLeaf239(void *, void *);
void effLeaf241(void *, f32);
void effPCPFlashPolyRingExpHSetRotateMatrix(void *, void *);
void effPCPFlashPolyRingExpSetColor(void *, u32);
void *effPCPBossBaelCreatePCF(void *);
void effLeaf288(void *);
void effPCPBossBaelDelete(void *);
void *effPCPBossBaelCopy(void *);
void effLeaf289(void *, void *);
void effLeaf290(void *, u32);
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
void effLeaf336(void *);
void effPCPBossMETATOBeamDelete(void *);
void *effPCPBossMETATOBeamCopy(void *);
void effLeaf337(void *, void *);
void dds3Leaf42(void *, u32);

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
    { dds3P2APolygonCreate, dds3P2APolygonDraw, dds3P2APolygonDelete, dds3P2APolygonCopy, dds3P2APolygonSetPosition, dds3P2APolygonSetScale, dds3Leaf34, dds3P2APolygonSetRGBA, dds3Leaf35, NULL },
    { (void *(*)(void *))dds3EffectPCPCreate, (void (*)(void *))dds3EffectPCPDraw, (void (*)(void *))dds3EffectPCPDelete, (void *(*)(void *))dds3EffectPCPCopy, dds3EffectPCPSetPosition, dds3EffectPCPSetScale, dds3EffectPCPSetRotateMatrix, dds3EffectPCPSetColor, dds3EffectPCPSetDirection, dds3EffectPCPSetArea },
    { Create_effPartsList, effPartsDraw, effPartsDelete, effPartsCreateFinish, effPartsPositionSet, effPartsScaleSet, effPartsRotateMatrixSet, ColorSet, NULL, NULL },
};

sdfVector4_t effPartModelPosInit __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
sdfVector4_t effPartModelRotInit __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
sdfVector4_t effPartModelScaleInit __attribute__((aligned(16))) = { 1.0f, 1.0f, 1.0f, 0.0f };
dds3EffectFunc_t EffectPCPFuncTable[98] = {
    { effPCPThunderSpherePolyCreate, effPCPThunderSpherePolyLowDraw, effPCPFlashPolyCircleDelete, NULL, effPCPShockWave01DrawSub, effPCPThunderSphereExpDrawSub, NULL, effPCPShockWave01DrawSub2, NULL, NULL },
    { effPCP_ThunderPolyCreate, effPCP_ThunderPolyDraw, effPCP_ThunderPolyDelete, NULL, effPCP_ThunderPolyLowSetPosition, NULL, NULL, effPCPRadiateThunderDrawSub, NULL, NULL },
    { effPCP_ThunderRadiatePolyCreate, effPCP_ThunderRadiatePolyDraw, effPCP_ThunderRadiatePolyDelete, NULL, effLeaf202, NULL, NULL, effLeaf203, NULL, NULL },
    { effPCPHama01Create, effPCPHama01Draw, effPCPHama01Delete, effPCPHama01Copy, effLeaf62, effLeaf63, NULL, effLeaf64, NULL, NULL },
    { effPCPHama02Create, effPCPHama02Draw, effPCPHama02Delete, effPCPHama02Copy, effLeaf65, effLeaf66, NULL, effLeaf67, NULL, NULL },
    { effPCPHamaonCreate, effPCPHamaonDraw, effPCPHamaonDelete, effPCPHamaonCopy, effLeaf68, effLeaf69, NULL, effLeaf70, NULL, NULL },
    { effPCPMudoCreate, effPCPMudoDraw, effPCPMudoDelete, effPCPMudoCopy, effLeaf74, effLeaf75, NULL, effLeaf76, NULL, NULL },
    { effPCPHwave01Create, effPCPHwave01Draw, effPCPHwave01Delete, effPCPHwave01Copy, effLeaf77, NULL, NULL, effLeaf78, NULL, NULL },
    { effPCP_ThunderPolyLowCreate, effPCP_ThunderPolyLowDraw, effPCP_ThunderPolyLowDelete, NULL, effPCP_ThunderPolySetPosition, NULL, NULL, effLeaf201, NULL, NULL },
    { effPCPZiodin01Create, effPCPZiodin01Draw, effPCPZiodin01Delete, effPCPZiodin01Copy, effLeaf79, NULL, NULL, effLeaf81, NULL, NULL },
    { effPCPMultiBlurExpCreate, effPCPMultiBlurExpDraw, effBTLSys_Encount_Delete2, NULL, effLeaf109, NULL, NULL, effLeaf110, NULL, effLeaf111 },
    { effPCPMHanma01Create, effPCPMHanma01Draw, effPCPMHanma01Delete, effPCPMHanma01Copy, effLeaf71, NULL, NULL, effLeaf73, NULL, NULL },
    { effPCPMZiodin01Create, effPCPMZiodin01Draw, effPCPMZiodin01Delete, effPCPMZiodin01Copy, effLeaf91, NULL, NULL, effLeaf93, NULL, NULL },
    { effPCPSparkleSphereCreate, effPCPSparkleSphereDraw, effBTLSys_Nego_Kyusyu_Delete, NULL, effLeaf59, NULL, NULL, effLeaf60, NULL, effLeaf61 },
    { effPCPFlashPolySphereCreate, effPCPFlashPolySphereDraw, effPCPFlashPolySphereDelete, NULL, effLeaf212, effLeaf214, NULL, effPCPFlashPolySphereSetColor, NULL, NULL },
    { effPCPFlashPolySphereCreatePCF, effPCPFlashPolySphereDraw, effPCPFlashPolySphereDelete, effPCPFlashPolySphereCopy, effLeaf212, effLeaf214, NULL, effPCPFlashPolySphereSetColor, NULL, NULL },
    { effPCPFlashPolyRingCreatePCF, effPCPFlashPolyRingDraw, effPCPFlashPolyRingDelete, effPCPFlashPolyRingCopy, effLeaf215, effLeaf217, NULL, effPCPFlashPolyRingSetColor, NULL, NULL },
    { effPCPFlashPolyPipeCreatePCF, effPCPFlashPolyPipeDraw, effPCPFlashPolyPipeDelete, effPCPFlashPolyPipeCopy, effLeaf218, effLeaf220, NULL, effPCPFlashPolyPipeSetColor, NULL, NULL },
    { effPCPFlashPolySphere2CreatePCF, effPCPFlashPolySphere2Draw, effPCPFlashPolySphere2Delete, effPCPFlashPolySphere2Copy, effLeaf209, effLeaf211, NULL, effPCPFlashPolySphere2SetColor, NULL, NULL },
    { effPCPFlashPolyRingExpCreatePCF, effPCPFlashPolyRingExpHDraw, effPCPFlashPolyRingExpDelete, effPCPFlashPolyRingExpCopy, effLeaf221, effLeaf223, NULL, effPCPFlashPolyRingExpHSetColor, NULL, NULL },
    { effPCPMZiodin02Create, effPCPMZiodin02Draw, effPCPMZiodin02Delete, effPCPMZiodin02Copy, effLeaf94, NULL, NULL, effLeaf96, NULL, NULL },
    { effPCPZiodin02Create, effPCPZiodin02Draw, effPCPZiodin02Delete, effPCPZiodin02Copy, effLeaf82, NULL, NULL, effLeaf84, NULL, NULL },
    { effPCPZiodin03Create, effPCPZiodin03Draw, effPCPZiodin03Delete, effPCPZiodin03Copy, effLeaf85, NULL, NULL, effLeaf87, NULL, NULL },
    { effPCPZiodin04Create, effPCPZiodin04Draw, effPCPZiodin04Delete, effPCPZiodin04Copy, effLeaf88, NULL, NULL, effLeaf90, NULL, NULL },
    { effPCPShockWave01Create, effPCPShockWave01Draw, effBTLSys_Nego_Kaihuku_Delete2, effPCPShockWave01Copy, effLeaf97, NULL, NULL, effLeaf99, NULL, NULL },
    { effPCPHouden01Create, effPCPHouden01Draw, effBTLSys_Nego_Kaihuku_Delete, effPCPHouden01Copy, effLeaf103, NULL, NULL, effLeaf105, NULL, NULL },
    { effPCPMultiFilterExpCreate, effPCPMultiFilterExpDraw, effBTLSys_Encount_Delete, NULL, effLeaf112, NULL, NULL, effLeaf113, NULL, effLeaf114 },
    { effPCPBlinkMultiBlurCreate, effPCPBlinkMultiBlurDraw, effPCPBlinkMultiBlurDelete, NULL, effLeaf106, NULL, NULL, effLeaf107, NULL, effLeaf108 },
    { effPCPMultiFilterExpCreatePCF, effPCPMultiFilterExpDraw, effBTLSys_Encount_Delete, effPCPMultiFilterExpCopy, effLeaf112, NULL, NULL, effLeaf113, NULL, effLeaf114 },
    { effPCPShockWave02Create, effPCPShockWave02Draw, effPCPShockWave02Delete, effPCPShockWave02Copy, effLeaf100, NULL, NULL, effLeaf102, NULL, NULL },
    { effPCPFilterExpCreatePCF, effPCPFilterExpDraw, effPCPFilterExpDelete, effPCPFilterExpCopy, NULL, NULL, NULL, effLeaf118, NULL, NULL },
    { effPCPBlurExpCreatePCF, effPCPBlurExpDraw, effPCPBlurExpDelete, effPCPBlurExpCopy, NULL, NULL, NULL, effLeaf119, NULL, NULL },
    { effPCPDistortionBlurExpCreatePCF, effPCPDistortionBlurExpDraw, effPCPDistortionBlurExpDelete, effPCPRippleBlurExpCopy, effLeaf120, NULL, NULL, effLeaf121, NULL, NULL },
    { effPCPMultiBlurExp2CreatePCF, effPCPMultiBlurExp2Draw, effPCPMultiBlurExp2Delete, effPCPMultiBlurExp2Copy, effLeaf115, NULL, NULL, effLeaf116, NULL, NULL },
    { effPCPMegidora01Create, effPCPMegidora01Draw, effPCPMegidora01Delete, effPCPMegidora01Copy, effLeaf124, NULL, NULL, effLeaf126, NULL, NULL },
    { effPCPMegidoraon01Create, effPCPMegidoraon01Draw, effPCPMegidoraon01Delete, effPCPMegidoraon01Copy, effLeaf127, NULL, NULL, effLeaf129, NULL, NULL },
    { effPCPBreathCreatePCF, effLeaf130, effPCPBreathDelete, effPCPBreathCopy, effLeaf131, NULL, effPCPBreathSetRotateMatrix, effLeaf132, NULL, NULL },
    { effPCPHamaCardSpiralCreatePCF, effPCPHamaCardSpiralDraw, effPCPHamaCardSpiralDelete, effPCPHamaCardSpiralCopy, effLeaf244, effPCPHamaCardSpiralSetScale, NULL, effPCPHamaCardSpiralSetColor, NULL, NULL },
    { effPCPHamaCardSphereCreatePCF, effPCPHamaCardSphereDraw, effPCPHamaCardSphereDelete, effPCPHamaCardSphereCopy, effLeaf246, effLeaf248, NULL, effPCPHamaCardSphereSetColor, NULL, NULL },
    { effPCPAgiraoFireCreatePCF, effPCPAgiraoFireDraw, effPCPAgiraoFireDelete, effPCPAgiraoFireCopy, effLeaf133, NULL, NULL, effLeaf134, NULL, NULL },
    { effPCPBufuIceCreate, effPCPBufuIceDraw, effPCPBufuIceDelete, effPCPBufuIceCopy, effLeaf135, effLeaf137, effLeaf138, effLeaf136, NULL, NULL },
    { effPCPBufudnIceCreate, effPCPBufudnIceDraw, effCallEffectPartDelete, effPCPBufudnIceCopy, effLeaf139, effLeaf141, effLeaf142, effLeaf140, NULL, NULL },
    { effPCPBufuraIceCreate, effPCPBufuraIceDraw, effCallEffectPartDelete2, effPCPBufuraIceCopy, effLeaf143, effLeaf145, effLeaf146, effLeaf144, NULL, NULL },
    { effLeaf147, effLeaf150, effLeaf149, effLeaf148, NULL, NULL, NULL, NULL, NULL, NULL },
    { effPCPAgiFireballCreatePCF, effPCPAgiFireball2DrawSub, effPCPAgiFireballDelete, effPCPAgiFireballCopy, effPCPAgiFireballSetPosition, NULL, effPCPAgiFireballSetRotateMatrix, effPCPAgiFireballSetColor, NULL, NULL },
    { effPCPNeedleSpiralCreatePCF, effLeaf264, effPCPNeedleSpiralDelete, effPCPNeedleSpiralCopy, effLeaf265, NULL, NULL, effPCPNeedleSpiralSetColor, NULL, NULL },
    { effCallChipAllocDirect2, effLeaf163, effPCPEyeThunderDelete, effLeaf151, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect3, effLeaf163, effPCPEyeThunderDelete, effLeaf152, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect4, effLeaf163, effPCPEyeThunderDelete, effLeaf153, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect5, effLeaf163, effPCPEyeThunderDelete, effLeaf154, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect6, effLeaf163, effPCPEyeThunderDelete, effLeaf155, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect7, effLeaf163, effPCPEyeThunderDelete, effLeaf156, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect8, effLeaf163, effPCPEyeThunderDelete, effLeaf157, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect9, effLeaf163, effPCPEyeThunderDelete, effLeaf158, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect10, effLeaf163, effPCPEyeThunderDelete, effLeaf159, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect11, effLeaf163, effPCPEyeThunderDelete, effLeaf160, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effPCPRippleBlurExpCreatePCF, effPCPRippleBlurExpDraw, effPCPRippleBlurExpDelete, effPCPDistortionBlurExpCopy, effLeaf122, NULL, NULL, effLeaf123, NULL, NULL },
    { effPCPThunderSphereExpCreatePCF, effPCPThunderSphereExpDraw, effPCPThunderSphereExpDelete, effPCPThunderSphereExpCopy, effLeaf166, effLeaf168, NULL, effLeaf167, NULL, NULL },
    { effPCPRingPolyExpCreatePCF, effPCPRingPolyExpDraw, effPCPRingPolyExpDelete, effPCPRingPolyExpCopy, effLeaf169, effLeaf170, effPCPRingPolyExp2SetRotateMatrix, effLeaf171, effPCPRingPolyExpSetDirection, NULL },
    { effPCPWhirlwindPolyCreatePCF, effPCPWhirlwindPolyDraw, effPCPWhirlwindPolyDelete, effPCPWhirlwindPolyCopy, effLeaf251, effLeaf252, effPCPWhirlwind3PolySetRotateMatrix, effPCPWhirlwindPolySetColor, NULL, NULL },
    { effCallChipAllocDirect12, effLeaf163, effPCPEyeThunderDelete, effLeaf161, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effCallChipAllocDirect13, effLeaf163, effPCPEyeThunderDelete, effLeaf162, effLeaf164, NULL, NULL, effLeaf165, NULL, NULL },
    { effPCPRadiateThunderCreatePCF, effPCPRadiateThunderDraw, effPCPRadiateThunderDelete, effPCPRadiateThunderCopy, effLeaf175, effLeaf176, NULL, effLeaf177, NULL, NULL },
    { effPCPWwSpherePolyCreatePCF, effPCPWwSpherePolyDraw, effPCPWwSpherePolyDelete, effPCPWwSpherePolyCopy, effLeaf260, effLeaf261, effPCPWwSpherePolySetRotateMatrix, effPCPWwSpherePolySetColor, NULL, NULL },
    { effPCPFlashPolySphereExpCreatePCF, effPCPFlashPolySphereExpDraw, effPCPFlashPolySphereExpDelete, effPCPFlashPolySphereExpCopy, effLeaf224, effLeaf226, NULL, effPCPFlashPolySphereExpSetColor, NULL, NULL },
    { effPCPFlashPolySphere2ExpCreatePCF, effPCPFlashPolySphere2ExpDraw, effPCPFlashPolySphere2ExpDelete, effPCPFlashPolySphere2ExpCopy, effLeaf227, effLeaf229, NULL, effPCPFlashPolySphere2ExpSetColor, NULL, NULL },
    { effPCPFlashPolySphere3CreatePCF, effPCPFlashPolySphere3Draw, effPCPFlashPolySphere3Delete, effPCPFlashPolySphere3Copy, effLeaf230, effLeaf232, NULL, effPCPFlashPolySphere3SetColor, NULL, NULL },
    { effPCPFlashPolyRingExp2CreatePCF, effPCPFlashPolyRingExp2Draw, effFunc9, effPCPFlashPolyRingExp2Copy, effLeaf233, effLeaf235, NULL, effPCPFlashPolyRingExp2SetColor, NULL, NULL },
    { effPCPWhirlwind2PolyCreatePCF, effPCPWhirlwind2PolyDraw, effPCPWhirlwind2PolyDelete, effPCPWhirlwind2PolyCopy, effLeaf254, effLeaf255, effPCPWhirlwind2PolySetRotateMatrix, effPCPWhirlwind2PolySetColor, NULL, NULL },
    { effPCPMakkaCreat, effLeaf178, effPCPMakkaDelete, effPCPMakkaCopy, effLeaf179, effLeaf181, NULL, effLeaf180, NULL, NULL },
    { effPCPRingPolyExp2CreatePCF, effPCPRingPolyExp2Draw, effPCPRingPolyExp2Delete, effPCPRingPolyExp2Copy, effLeaf172, effLeaf173, NULL, effLeaf174, NULL, NULL },
    { effPCPAgiFireball2CreatePCF, effPCPAgiFireball2Draw, effPCPAgiFireball2Delete, effPCPAgiFireball2Copy, effPCPAgiFireball2SetRotateMatrix, NULL, effCallPCPAgiFireballSetRotateMatri, effPCPAgiFireball2SetColor, NULL, NULL },
    { effPCPRadiateBEDCreatePCF, effPCPRadiateBEDDraw, effPCPRadiateBEDDelete, effPCPRadiateBEDCopy, effLeaf182, effLeaf183, NULL, effLeaf184, NULL, NULL },
    { effAPPoint4CreatePCF, effAPPoint4Draw, effAPPoint4Delete, effAPPoint4Copy, NULL, NULL, NULL, NULL, NULL, NULL },
    { effPCPRadiateTBCreatePCF, effPCPRadiateTBDraw, effPCPRadiateTBDelete, effPCPRadiateTBCopy, effLeaf185, NULL, NULL, effLeaf186, NULL, NULL },
    { effPCPWhirlwind3PolyCreatePCF, effPCPWhirlwind3PolyDraw, effPCPWhirlwind3PolyDelete, effPCPWhirlwind3PolyCopy, effLeaf257, effLeaf258, effCallMultMatrixNoArg, effPCPWhirlwind3PolySetColor, NULL, NULL },
    { effPCPThunderSpherePolyCreatePCF, effPCPThunderSpherePolyLowDraw, effPCPFlashPolyCircleDelete, effPCPThunderSpherePolyCopy, effPCPShockWave01DrawSub, effPCPThunderSphereExpDrawSub, NULL, effPCPShockWave01DrawSub2, NULL, NULL },
    { effCallEffectPCPDataGetData, effCallPCPThunderSphereLowRotate, effPCPFlashPolyRingExp2Delete, effLeaf195, effLeaf196, effLeaf198, NULL, effLeaf197, NULL, NULL },
    { effPCPSpiralBEDCreatePCF, effPCPSpiralBEDDraw, effPCPSpiralBEDDelete, effPCPSpiralBEDCopy, effLeaf187, effLeaf188, NULL, effLeaf189, NULL, NULL },
    { effPCPAgiFireballSpCreatePCF, effPCPAgiFireball2DrawSub, effPCPAgiFireballDelete, effPCPAgiFireballSpCopy, effPCPAgiFireballSetPosition, NULL, effPCPAgiFireballSetRotateMatrix, effPCPAgiFireballSetColor, NULL, NULL },
    { effPCPAgiFireballSp2CreatePCF, effPCPAgiFireball2DrawSub, effPCPAgiFireballDelete, effPCPAgiFireballSp2Copy, effPCPAgiFireballSetPosition, NULL, effPCPAgiFireballSetRotateMatrix, effPCPAgiFireballSetColor, NULL, NULL },
    { effPCPBezierBillCreatePCF, effLeaf206, effPCPBezierBillDelete, effPCPBezierBillCopy, effLeaf207, NULL, NULL, effLeaf208, NULL, NULL },
    { effPCPCrackCreatePCF, effPCPCrackDraw, effPCPCrackDelete, effPCPCrackCopy, effLeaf286, NULL, NULL, effLeaf287, NULL, NULL },
    { effPCPHelperBEDCreatePCF, effPCPHelperBEDDraw, effPCPHelperBEDDelete, effPCPHelperBEDCopy, effLeaf190, effLeaf191, NULL, effLeaf192, NULL, NULL },
    { effPCPFlashPolyCircleCreatePCF, effPCPFlashPolyCircleDraw, effFunc10, effPCPFlashPolyCircleCopy, effLeaf236, effLeaf238, NULL, effPCPFlashPolyCircleSetColor, NULL, NULL },
    { effPCPFlashPolyRingExpHCreatePCF, effPCPFlashPolyRingExpDraw, effCallFlashPolyListHDelete, effPCPFlashPolyRingExpHCopy, effLeaf239, effLeaf241, effPCPFlashPolyRingExpHSetRotateMatrix, effPCPFlashPolyRingExpSetColor, NULL, NULL },
    { effPCPBossBaelCreatePCF, effLeaf288, effPCPBossBaelDelete, effPCPBossBaelCopy, effLeaf289, NULL, NULL, effLeaf290, NULL, NULL },
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
    { effPCPBossMETATOBeamCreatePCF, effLeaf336, effPCPBossMETATOBeamDelete, effPCPBossMETATOBeamCopy, effLeaf337, NULL, NULL, dds3Leaf42, NULL, NULL },
};
#endif
