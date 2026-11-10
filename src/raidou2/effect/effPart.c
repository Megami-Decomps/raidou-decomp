#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effPart.c */
#include "common.h"

#include "motion.h"

#include "model.h"

#include "event.h"

#include "nb.h"

#include "effBattle.h"

#include "ps2asm.h"

extern sdfVector4_t effPartModelPosInit;

extern sdfVector4_t effPartModelRotInit;

extern sdfVector4_t effPartModelScaleInit;

typedef struct
{
    u16 type;
    void *pHandle;
} effPartHandle_t;

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

void mdlSetPosition(mdlObj_t *aHandle);

void mdlSetRotate(mdlObj_t *aHandle);

void mdlSetScale(mdlObj_t *aHandle);

void mdlSetColor(mdlObj_t *aHandle, u32 aColor);

void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern dds3EffectFunc_t dds3EffectFuncTable[7];

extern void *sdfModelOtTable[4];

extern s32 effPartModelSerial;

extern s32 effBTLCutCount;

/* Header of a packed model an effect part carries, followed by the pack data. */
typedef struct
{
    u32 size;
    u8 reserve[0xC];
    u8 data[1];
} dds3EffectPartModelData_t;

void *dds3BillboardCreate(s32 kind, addr_t data);

void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);

void mdlLink(mdlObj_t *aHandle, void **aModelOt);

void mdlFree(mdlObj_t *aHandle);

extern dds3EffectFunc_t EffectPCPFuncTable[];

extern void **effPartPCPDataTable[];

void *sdfChipAllocDirect(s32 aSize);

void sdfChipFreeDirect(void *aPtr);

void dds3BillboardSetScale(void *hBillboard, f32 x, f32 y);

s32 mdlGetMajorNum(mdlObj_t *aHandle);

s32 mdlGetMinorNum(mdlObj_t *aHandle);

mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor);

void dds3EffectPartModelInit(mdlObj_t *hModel);

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);

extern u32 effBTLPointLightProcess;

extern s8 effBTLFieldColorOn;

extern s8 D_0054F0F1;

extern const char *effBTLJyocyuEffFilePath;

extern u8 effBTLDebugCameraAllEdit;

extern u8 effBTLDebugCameraAllTable;

extern u8 D_0054F1C3;

extern s32 effBTLPartyIndex;

extern s8 effBTLUnitLightOn;

extern char effBedDirName[];

void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);

u16 dds3EffectPartGetType(effPartHandle_t *hEffPart);

void dds3EffectPartDelete(effPartHandle_t *hEffPart);

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);

void dds3EffectPartDraw(effPartHandle_t *hEffPart);

void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, sdfVector4_t *pPos);

void dds3EffectPartSetScale(effPartHandle_t *hEffPart, f32 scale);

void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr);

void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);

u16 dds3EffectPartGetType(effPartHandle_t *hEffPart);

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);

void dds3EffectPartDelete(effPartHandle_t *hEffPart);

void dds3EffectPartDraw(effPartHandle_t *hEffPart);

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);

void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, sdfVector4_t *pPos);

void dds3EffectPartSetScale(effPartHandle_t *hEffPart, f32 scale);

void dds3EffectPartSetRotateMatrix(effPartHandle_t *hEffPart, void *pMatrix);

void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr);

void dds3EffectPartSetDirection(effPartHandle_t *hEffPart, void *pMatrix);

void dds3EffectPartSetArea(effPartHandle_t *hEffPart, f32 area);

void *dds3EffectPartTMXBillboardCreate(u32 data);

void *dds3EffectPartP2ABillboardCreate(u32 data);

void dds3EffectPartBillboardScale(void *hBillboard, f32 scale);

void dds3EffectPartModelInit(mdlObj_t *hModel);

mdlObj_t *dds3EffectPartModelCreate(dds3EffectPartModelData_t *pData);

void dds3EffectPartModelDraw(mdlObj_t *hData);

void dds3EffectPartModelDelete(mdlObj_t *hData);

mdlObj_t *dds3EffectPartModelCopy(void *hData);

void dds3EffectPartModelSetPosition(mdlObj_t *hData, sdfVector4_t *pPos);

void dds3EffectPartModelSetScale(void *hData, f32 scale);

void dds3EffectPartModelSetRotateMatrix(mdlObj_t *hData, sdfMatrix4_t *pMatrix);

void dds3EffectPartModelSetColor(mdlObj_t *hData, u32 abgr);

dds3EffectPCP_t *dds3EffectPCPCreate(dds3EffectPCPData_t *hPCPData);

void dds3EffectPCPDelete(dds3EffectPCP_t *hData);

void dds3EffectPCPDraw(dds3EffectPCP_t *hData);

dds3EffectPCP_t *dds3EffectPCPCopy(dds3EffectPCP_t *hPCPSrc);

void dds3EffectPCPSetPosition(dds3EffectPCP_t *hData, void *pPos);

void dds3EffectPCPSetScale(dds3EffectPCP_t *hData, f32 scale);

void dds3EffectPCPSetRotateMatrix(dds3EffectPCP_t *hData, void *pMatrix);

void dds3EffectPCPSetDirection(dds3EffectPCP_t *hData, void *pMatrix);

void dds3EffectPCPSetColor(dds3EffectPCP_t *hData, u32 abgr);

void dds3EffectPCPSetArea(dds3EffectPCP_t *hData, f32 area);

u32 dds3EffectPCPDataGetKateg(dds3EffectPCPData_t *hPCPData);

u32 dds3EffectPCPDataGetParam(dds3EffectPCPData_t *hPCPData);

void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number);

u32 dds3EffectPCPDataGetSize(dds3EffectPCPData_t *hPCPData, u32 number);

u32 dds3EffectPCPDataGetType(dds3EffectPCPData_t *hPCPData, u32 number);

effPartHandle_t *dds3EffectPCPDataPartCreate(dds3EffectPCPData_t *hPCPData, u32 number);

extern void *effPCPThunderSpherePolyDataList[];

extern void *effPCPThunderPolyDataList[];

extern void *effPCPThunderRadiatePolyDataList[];

extern void *effPCPMultiBlurExpDataList[];

extern void *effPCPSparkleSphereDataList[];

extern void *effPCPFlashPolySphereDataList[];

extern void *effPCPMultiFilterExpDataList[];

extern void *effPCPBlinkMultiBlurDataList[];

void *dds3EffectLoad(void *);

void dds3EffectDraw(void *);

void dds3EffectDelete(void *);

void *dds3EffectCopy(void *);

void dds3EffectSetPosition(void *, void *);

void dds3EffectSetScale(void *, f32);

void dds3EffectSetRotateMatrix(void *, void *);

void dds3EffectSetRGBA(void *, u32);

void dds3BillboardDraw(void *);

void dds3BillboardDelete(void *);

void *dds3BillboardCopy(void *);

void dds3BillboardSetPosition(void *, void *);

void dds3BillboardSetRGBA(void *, u32);

void *dds3P2APolygonCreate(void *);

void dds3P2APolygonDraw(void *);

void dds3P2APolygonDelete(void *);

void *dds3P2APolygonCopy(void *);

void dds3P2APolygonSetPosition(void *, void *);

void dds3P2APolygonSetScale(void *, f32);

void dds3P2APolygonSetRotateMatrix(void *, void *);

void dds3P2APolygonSetRGBA(void *, u32);

void dds3P2APolygonSetDirection(void *, void *);

void *effPCPThunderSpherePolyCreate(void *);

void effPCPThunderSpherePolyDraw(void *);

void effPCPThunderSpherePolyDelete(void *);

void effPCPThunderSpherePolySetPosition(void *, void *);

void effPCPThunderSpherePolySetScale(void *, f32);

void effPCPThunderSpherePolySetColor(void *, u32);

void *effPCP_ThunderPolyCreate(void *);

void effPCP_ThunderPolyDraw(void *);

void effPCP_ThunderPolyDelete(void *);

void effPCP_ThunderPolySetPosition(void *, void *);

void effPCP_ThunderPolySetColor(void *, u32);

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

void effPCP_ThunderPolyLowSetPosition(void *, void *);

void effPCP_ThunderPolyLowSetColor(void *, u32);

void *effPCPZiodin01Create(void *);

void effPCPZiodin01Draw(void *);

void effPCPZiodin01Delete(void *);

void *effPCPZiodin01Copy(void *);

void effPCPZiodin01SetPosition(void *, void *);

void effPCPZiodin01SetColor(void *, u32);

void *effPCPMultiBlurExpCreate(void *);

void effPCPMultiBlurExpDraw(void *);

void effPCPMultiBlurExpDelete(void *);

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

void effPCPSparkleSphereDelete(void *);

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

void effPCPFlashPolyRingExpDraw(void *);

void effPCPFlashPolyRingExpDelete(void *);

void *effPCPFlashPolyRingExpCopy(void *);

void effPCPFlashPolyRingExpSetPosition(void *, void *);

void effPCPFlashPolyRingExpSetScale(void *, f32);

void effPCPFlashPolyRingExpSetColor(void *, u32);

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

void effPCPShockWave01Delete(void *);

void *effPCPShockWave01Copy(void *);

void effPCPShockWave01SetPosition(void *, void *);

void effPCPShockWave01SetColor(void *, u32);

void *effPCPHouden01Create(void *);

void effPCPHouden01Draw(void *);

void effPCPHouden01Delete(void *);

void *effPCPHouden01Copy(void *);

void effPCPHouden01SetPosition(void *, void *);

void effPCPHouden01SetColor(void *, u32);

void *effPCPMultiFilterExpCreate(void *);

void effPCPMultiFilterExpDraw(void *);

void effPCPMultiFilterExpDelete(void *);

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

void *effPCPDistortionBlurExpCopy(void *);

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

void *effPCPBufuraIceCreate(void *);

void effPCPBufuraIceDraw(void *);

void effPCPBufuraIceDelete(void *);

void *effPCPBufuraIceCopy(void *);

void effPCPBufuraIceSetPosition(void *, void *);

void effPCPBufuraIceSetScale(void *, f32);

void effPCPBufuraIceSetRotateMatrix(void *, void *);

void effPCPBufuraIceSetColor(void *, u32);

void *effPCPBufudnIceCreate(void *);

void effPCPBufudnIceDraw(void *);

void effPCPBufudnIceDelete(void *);

void *effPCPBufudnIceCopy(void *);

void effPCPBufudnIceSetPosition(void *, void *);

void effPCPBufudnIceSetScale(void *, f32);

void effPCPBufudnIceSetRotateMatrix(void *, void *);

void effPCPBufudnIceSetColor(void *, u32);

void *effPCPBufuLightCreate(void *);

void effPCPBufuLightDraw(void *);

void effPCPBufuLightDelete(void *);

void *effPCPBufuLightCopy(void *);

void *effPCPAgiFireballCreatePCF(void *);

void effPCPAgiFireballDraw(void *);

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

void *effPCPEyeThunderCreate00_0(void *);

void effPCPEyeThunderDraw(void *);

void effPCPEyeThunderDelete(void *);

void *effPCPEyeThunderCopy00_0(void *);

void effPCPEyeThunderSetPosition(void *, void *);

void effPCPEyeThunderSetColor(void *, u32);

void *effPCPEyeThunderCreate00_1(void *);

void *effPCPEyeThunderCopy00_1(void *);

void *effPCPEyeThunderCreate01_0(void *);

void *effPCPEyeThunderCopy01_0(void *);

void *effPCPEyeThunderCreate01_1(void *);

void *effPCPEyeThunderCopy01_1(void *);

void *effPCPEyeThunderCreate02_0(void *);

void *effPCPEyeThunderCopy02_0(void *);

void *effPCPEyeThunderCreate02_1(void *);

void *effPCPEyeThunderCopy02_1(void *);

void *effPCPEyeThunderCreate03_0(void *);

void *effPCPEyeThunderCopy03_0(void *);

void *effPCPEyeThunderCreate03_1(void *);

void *effPCPEyeThunderCopy03_1(void *);

void *effPCPEyeThunderCreate04_0(void *);

void *effPCPEyeThunderCopy04_0(void *);

void *effPCPEyeThunderCreate04_1(void *);

void *effPCPEyeThunderCopy04_1(void *);

void *effPCPRippleBlurExpCreatePCF(void *);

void effPCPRippleBlurExpDraw(void *);

void effPCPRippleBlurExpDelete(void *);

void *effPCPRippleBlurExpCopy(void *);

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

void effPCPRingPolyExpSetRotateMatrix(void *, void *);

void effPCPRingPolyExpSetColor(void *, u32);

void effPCPRingPolyExpSetDirection(void *, void *);

void *effPCPWhirlwindPolyCreatePCF(void *);

void effPCPWhirlwindPolyDraw(void *);

void effPCPWhirlwindPolyDelete(void *);

void *effPCPWhirlwindPolyCopy(void *);

void effPCPWhirlwindPolySetPosition(void *, void *);

void effPCPWhirlwindPolySetScale(void *, f32);

void effPCPWhirlwindPolySetRotateMatrix(void *, void *);

void effPCPWhirlwindPolySetColor(void *, u32);

void *effPCPEyeThunderCreate05_0(void *);

void *effPCPEyeThunderCopy05_0(void *);

void *effPCPEyeThunderCreate05_1(void *);

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

void effPCPFlashPolyRingExp2Delete(void *);

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

void effPCPAgiFireball2SetPosition(void *, void *);

void effPCPAgiFireball2SetRotateMatrix(void *, void *);

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

void effPCPWhirlwind3PolySetRotateMatrix(void *, void *);

void effPCPWhirlwind3PolySetColor(void *, u32);

void *effPCPThunderSpherePolyCreatePCF(void *);

void *effPCPThunderSpherePolyCopy(void *);

void *effPCPThunderSpherePolyLowCreate(void *);

void effPCPThunderSpherePolyLowDraw(void *);

void effPCPThunderSpherePolyLowDelete(void *);

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

void effPCPFlashPolyCircleDelete(void *);

void *effPCPFlashPolyCircleCopy(void *);

void effPCPFlashPolyCircleSetPosition(void *, void *);

void effPCPFlashPolyCircleSetScale(void *, f32);

void effPCPFlashPolyCircleSetColor(void *, u32);

void *effPCPFlashPolyRingExpHCreatePCF(void *);

void effPCPFlashPolyRingExpHDraw(void *);

void effPCPFlashPolyRingExpHDelete(void *);

void *effPCPFlashPolyRingExpHCopy(void *);

void effPCPFlashPolyRingExpHSetPosition(void *, void *);

void effPCPFlashPolyRingExpHSetScale(void *, f32);

void effPCPFlashPolyRingExpHSetRotateMatrix(void *, void *);

void effPCPFlashPolyRingExpHSetColor(void *, u32);

void *effPCPBossBaelCreatePCF(void *);

void effPCPBossBaelDraw(void *);

void effPCPBossBaelDelete(void *);

void *effPCPBossBaelCopy(void *);

void effPCPBossBaelSetPosition(void *, void *);

void effPCPBossBaelSetColor(void *, u32);

void *effPCPBossDengekiCreatePCF(void *);

void effPCPBossDengekiDraw(void *);

void effPCPBossDengekiDelete(void *);

void *effPCPBossDengekiCopy(void *);

void effPCPBossDengekiSetColor(void *, u32);

void *effPCPAnalyzeBEDCreatePCF(void *);

void effPCPAnalyzeBEDDraw(void *);

void effPCPAnalyzeBEDDelete(void *);

void *effPCPAnalyzeBEDCopy(void *);

void effPCPAnalyzeBEDSetScale(void *, f32);

void effPCPAnalyzeBEDSetColor(void *, u32);

void *effPCPBossThunderboltCreatePCF(void *);

void effPCPBossThunderboltDraw(void *);

void effPCPBossThunderboltDelete(void *);

void *effPCPBossThunderboltCopy(void *);

void effPCPBossThunderboltSetPosition(void *, void *);

void effPCPBossThunderboltSetColor(void *, u32);

void *effPCPBossKAGUInvokeCreatePCF(void *);

void effPCPBossKAGUInvokeDraw(void *);

void effPCPBossKAGUInvokeDelete(void *);

void *effPCPBossKAGUInvokeCopy(void *);

void effPCPBossKAGUInvokeSetColor(void *, u32);

void *effPCPBossKAGUAttackCreatePCF(void *);

void effPCPBossKAGUAttackDraw(void *);

void effPCPBossKAGUAttackDelete(void *);

void *effPCPBossKAGUAttackCopy(void *);

void effPCPBossKAGUAttackSetColor(void *, u32);

void *effPCPBossKAGUThunderboltCreatePCF(void *);

void effPCPBossKAGUThunderboltDraw(void *);

void effPCPBossKAGUThunderboltDelete(void *);

void *effPCPBossKAGUThunderboltCopy(void *);

void effPCPBossKAGUThunderboltSetPosition(void *, void *);

void effPCPBossKAGUThunderboltSetColor(void *, u32);

void *effPCPBossKAGU2AttackCreatePCF(void *);

void effPCPBossKAGU2AttackDraw(void *);

void effPCPBossKAGU2AttackDelete(void *);

void *effPCPBossKAGU2AttackCopy(void *);

void effPCPBossKAGU2AttackSetColor(void *, u32);

void effPCPBossKAGU2ThunderboltDraw(void *);

void *effPCPFLEACHBlurCreatePCF(void *);

void effPCPFLEACHBlurDraw(void *);

void effPCPFLEACHBlurDelete(void *);

void *effPCPFLEACHBlurCopy(void *);

void effPCPFLEACHBlurSetColor(void *, u32);

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

void *effPCPScreenCrackModelCreatePCF(void *);

void effPCPScreenCrackModelDraw(void *);

void effPCPScreenCrackModelDelete(void *);

void *effPCPScreenCrackModelCopy(void *);

void effPCPScreenCrackModelSetColor(void *, u32);

void *effPCPMuzzleFlashCreatePCF(void *);

void effPCPMuzzleFlashDraw(void *);

void effPCPMuzzleFlashDelete(void *);

void *effPCPMuzzleFlashCopy(void *);

void effPCPMuzzleFlashSetPosition(void *, void *);

void effPCPMuzzleFlashSetScale(void *, f32);

void effPCPMuzzleFlashSetRotateMatrix(void *, void *);

void effPCPMuzzleFlashSetColor(void *, u32);

void *effPCPDANTE_Thunder_CreatePCF(void *);

void effPCPDANTE_Thunder_DrawSub(void *);

void effPCPDANTE_Thunder_Delete(void *);

void *effPCPDANTE_Thunder_Copy(void *);

void effPCPDANTE_Thunder_SetPosition(void *, void *);

void effPCPDANTE_Thunder_SetRotateMatrix(void *, void *);

void effPCPDANTE_Thunder_SetColor(void *, u32);

void *effPCPDANTE_Damage_CreatePCF(void *);

void effPCPDANTE_Damage_Run(void *);

void effPCPDANTE_Damage_Delete(void *);

void *effPCPDANTE_Damage_Copy(void *);

void effPCPDANTE_Damage_SetColor(void *, u32);

void *effPCP_PBCamera_CreatePCF(void *);

void effPCP_PBCamera_Run(void *);

void effPCP_PBCamera_Delete(void *);

void *effPCP_PBCamera_Copy(void *);

void effPCP_PBCamera_SetPosition(void *, void *);

void effPCP_PBCamera_SetRotateMatrix(void *, void *);

void *effPCPDANTE_ShowTime_CreatePCF(void *);

void effPCPDANTE_ShowTime_Draw(void *);

void effPCPDANTE_ShowTime_Delete(void *);

void *effPCPDANTE_ShowTime_Copy(void *);

void effPCPDANTE_ShowTime_SetPosition(void *, void *);

void *effPCP_BED_CreatePCF(void *);

void effPCP_BED_Draw(void *);

void effPCP_BED_Delete(void *);

void *effPCP_BED_Copy(void *);

void effPCP_BED_SetPosition(void *, void *);

void effPCP_BED_SetRotateMatrix(void *, void *);

void effPCP_BED_SetColor(void *, u32);

void *effPCP_HBED2_CreatePCF(void *);

void effPCP_HBED2_Draw(void *);

void effPCP_HBED2_Delete(void *);

void *effPCP_HBED2_Copy(void *);

void effPCP_HBED2_SetPosition(void *, void *);

void effPCP_HBED2_SetScale(void *, f32);

void effPCP_HBED2_SetRotateMatrix(void *, void *);

void effPCP_HBED2_SetColor(void *, u32);

extern void **effPartPCPDataTable[28];

extern dds3EffectFunc_t dds3EffectFuncTable[7];

extern sdfVector4_t effPartModelPosInit __attribute__((aligned(16)));

extern sdfVector4_t effPartModelRotInit __attribute__((aligned(16)));

extern sdfVector4_t effPartModelScaleInit __attribute__((aligned(16)));

/* Indexed by PCP category. The IDB names entries from 1 on, as effPCP*Table. */
extern dds3EffectFunc_t EffectPCPFuncTable[106];
/* End of declarations from src/smt3/effect/effPart.c */

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

INCLUDE_ASM(const s32, "effect/effPart", func_00210DF8);

INCLUDE_ASM(const s32, "effect/effPart", func_00210E38);

INCLUDE_ASM(const s32, "effect/effPart", func_00210E78);

INCLUDE_ASM(const s32, "effect/effPart", func_00210EB8);

INCLUDE_ASM(const s32, "effect/effPart", func_00210EF8);

INCLUDE_ASM(const s32, "effect/effPart", func_00210F38);

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

INCLUDE_ASM(const s32, "effect/effPart", func_002110D8);

INCLUDE_ASM(const s32, "effect/effPart", func_002110F8);

INCLUDE_ASM(const s32, "effect/effPart", func_00211110);

INCLUDE_ASM(const s32, "effect/effPart", func_00211168);

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

INCLUDE_ASM(const s32, "effect/effPart", func_002111F0);

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

INCLUDE_ASM(const s32, "effect/effPart", func_002113E0);

INCLUDE_ASM(const s32, "effect/effPart", func_00211420);

INCLUDE_ASM(const s32, "effect/effPart", func_00211460);

INCLUDE_ASM(const s32, "effect/effPart", func_002114A0);

INCLUDE_ASM(const s32, "effect/effPart", func_002114E0);

INCLUDE_ASM(const s32, "effect/effPart", func_00211520);

INCLUDE_ASM(const s32, "effect/effPart", func_00211560);

INCLUDE_ASM(const s32, "effect/effPart", func_00211568);

void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return (u8 *)hPCPData + part->offset;
}

INCLUDE_ASM(const s32, "effect/effPart", func_00211588);

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

INCLUDE_SDATA(const s32, "effect/effPart", D_004E17D4);

