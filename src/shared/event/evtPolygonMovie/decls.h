/* Declarations of smt3-decomp src/smt3/event/evtPolygonMovie.c, for its bodies in this directory. */
#ifndef SHARED_EVENT_EVTPOLYGONMOVIE_H
#define SHARED_EVENT_EVTPOLYGONMOVIE_H

#include "common.h"

#include "kernel.h"

#include "libsdf.h"

#include "effect.h"

#include "event.h"

#include "evtPolygonMovie.h"

#include "ps2asm.h"

#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

void *dds3GetProcessData(dds3ProcessID_t *id);

void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, char *name);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void evtPlaySe(s32 eventno, s32 seno);

extern void evtSetBokasi(s32 sw);

extern void dds3SetQuake(s32 frame, s32 range);

extern void dds3StopQuake(void);

extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);

extern void evtSetSkyAlpha(s32 frame, s32 a);

void evtFreePolygonMovieWorkData(evtPolygonMovieWork_t *wdata);

extern dds3Basic_t *dds3GetActiveScene(void);

extern void sdfChipFreeDirect(void *aPtr);

extern void itfMesMngRelease(s32 id);

extern void fileSyncAll(void);

extern void fileHandleFree(void *fileHandle);

extern void sdfMemFreeDirect(void *aHandle);

extern void FrameRun(evtPolygonMovieWork_t *wdata);

#include "sdk_libc.h"

extern f32 evtSceneLightParam[12];

extern s32 strcmp(const char *a, const char *b);

extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *effdata, f32 *pos, f32 *rot);

extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(void *data, f32 *pos, f32 *rot);

extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata);

extern void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 s);

extern void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, s32 cutno);

extern void evtEffectLoopControl(dds3Basic_t *eff, s32 sw);

extern void dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1, dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);

dds3Basic_t *SearchBasicFromNameIndex_evtPolygonMovie(evtPolygonMovieWork_t *work, s32 nameIndex);

s32 AddEffectTableMG12(s32 now, evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f);

extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);

extern void dds3EffectObjectPresetMG12(dds3Basic_t *eff);

extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);

extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index);

extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);

extern dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *pObjectBasic);

extern void evtPlayEventBgm(s32 eventNo, s32 bgm);

extern void evtFadeInEventBgm(s32 eventNo, s32 bgm);

extern void evtStopEventBgm(s32 eventNo, s32 bgm);

extern void evtTransEventBgm(s32 eventNo, s32 bgm);

extern void p2sdBgmVolDown(void);

extern void p2sdBgmVolUp(void);

extern void p2sdStopAllBgm(void);

typedef struct fileRequest fileRequest_t;

extern s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);

extern sdfMemHandle_t *fileGetMemHandle(void *fileHandle);

extern void *fileLoad(const char *filename);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

evtPolygonMovieWork_t *evtGetPolygonMovieWorkData1(evtPolygonMovieWork_t *wdata, void *pm1data, void *pm2data);

saddr_t evtPolygonMovieProcess1(dds3ProcessID_t *id);

typedef struct pmdCameraTable_t {
    s32 NameIndex, Reserve1, Reserve2, Reserve3;
} pmdCameraTable_t;

extern s32 dds3GetMapNo(dds3Basic_t *scene);

extern s32 evtLoadArea_FromMemory(s32 fld, s32 area, saddr_t pF1, saddr_t pF2, const void *pTB, s32 pBF);

extern void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);

extern s32 evtSetRegistUnit(s32 major, s32 minor);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *scene, u32 serial, s32 type);

extern evtUnit_t *evtGetUnitHandle(s32 serial);

extern void mdlSmoothSingleMotion(mdlObj_t *model, s32 group, s32 motion, f32 before, f32 smooth);

extern dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic);

extern void dds3MoverBasicSetPath(dds3Basic_t *mover, dds3Basic_t *path);

extern void dds3MoverBasicMakePath(dds3Basic_t *mover);

extern void sdfMotionSeek(sdfMotionCtrlr_t *motion, f32 frame);

extern u32 dds3DrawFlag;

extern char evtPolygonMovieStartMsg[];

extern char evtPolygonMovieMapMsg[];

void evtPolygonMovieShutdown(dds3ProcessID_t *id);

void SetPMFileName(s32 eventno, s32 cutno, char *pm1file, char *pm2file);

evtPolygonMovieWork_t *InitPolygonMovieWorkData(void);

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern void dds3SetLightColor(s32 frame, s32 no, f32 *col);

extern void dds3SetLightDir(s32 frame, s32 no, f32 *dir);

extern void dds3SetAmbientColor(s32 frame, f32 *col);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);

extern void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag);

extern void *evtGetUnitHandle2(dds3Basic_t *unit);

extern void evtLinkLightObjectBasic(void *h, dds3Basic_t *light, s32 frame);

extern void evtUnlinkLightObjectBasic(void *h, s32 frame);

void evtHokanSceneLight(s32 hokan, f32 ratio, f32 *key1, f32 *key2, f32 *d);

f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2);

extern s16 dds3ScreenBlurAlpha;

extern s16 dds3ScreenBlurScale;

extern u8 dds3ScreenBlurFlag;

extern u32 dds3LightParam[];

extern pmdSceneFogTable_t evtSceneFogParam;

extern dds3Effect_Blur_t evtBlur2Param;

extern dds3Effect_PartialMultiBlurParam_t evtMultBlurParam;

extern dds3Effect_DistortionBlurParam_t evtDistBlurParam;

extern dds3Effect_RippleBlurParam_t evtRipBlurParam;

extern dds3Effect_PartialMultiFilterParam_t evtMultFilterParam;

extern dds3Effect_Filter_t evtFilterParam;

extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);

extern void Eve_BlurSetParam(dds3Effect_Blur_t *pParam);

extern void Eve_BlurON(void);

extern void Eve_BlurOFF(void);

extern void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *pParam);

extern void Eve_PartialMultiBlurON(void);

extern void Eve_PartialMultiBlurOFF(void);

extern void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *pParam);

extern void Eve_DistortionBlurON(void);

extern void Eve_DistortionBlurOFF(void);

extern void Eve_RippleBlurSetParam(dds3Effect_RippleBlurParam_t *pParam);

extern void Eve_RippleBlurON(void);

extern void Eve_RippleBlurOFF(void);

extern void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *pParam);

extern void Eve_PartialMultiFilterON(void);

extern void Eve_PartialMultiFilterOFF(void);

extern void Eve_FilterSetParam(dds3Effect_Filter_t *pParam);

extern void Eve_FilterON(void);

extern void Eve_FilterOFF(void);

void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d);

void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *key1, dds3Effect_Blur_t *key2, dds3Effect_Blur_t *d);

void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d);

void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *key1, dds3Effect_DistortionBlurParam_t *key2, dds3Effect_DistortionBlurParam_t *d);

void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d);

void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d);

void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d);

s32 AddEffectTable(s32 now, evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, char *effname, char *targetname, s32 posindex, s32 cutno);

f32 GetKeyFrameRatio(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void DelEffectTable_evtPolygonMovie(evtPolygonMovieWork_t *wdata, s32 no);

void evtRunUnitMode(dds3Basic_t *unit, s32 mode);

typedef void (*evtFrameFunc_t)(evtPolygonMovieWork_t *, pmdFrameTable_t *, pmdFrameTable_t *, s32);

extern evtFrameFunc_t nowcamera[];

extern void dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

extern void dds3SetScreenFovy(f32 fovy);

extern saddr_t evtCameraFrameState;

extern f32 nowfovy;

extern s32 evtCameraLastFrame;

extern void *sdfChipAllocDirect(u32 aSize);

extern void *memset(void *a, s32 b, size_t c);

/* vu0 routine: blend two RGBA8888 colours by t */
u32 evtHokanColor(s32 hokan, f32 ratio, u32 c1, u32 c2);

extern f32 evtHokanFogParam[4];

extern u32 evtSceneFogDefaultColor[4];

extern dds3Effect_Blur_t evtBlur2DefaultParam;

extern dds3Effect_PartialMultiBlurParam_t evtMultBlurDefaultParam;

extern dds3Effect_Filter_t evtFilterDefaultParam;

extern dds3Effect_PartialMultiFilterParam_t evtMultFilterDefaultParam;

extern dds3Effect_DistortionBlurParam_t evtDistBlurDefaultParam;

extern dds3Effect_RippleBlurParam_t evtRipBlurDefaultParam;

typedef struct polyMovieClip_t {
    u8 opaque_00[8];
    f32 duration;
    f32 position;
} polyMovieClip_t;

typedef struct polyMovieState_t {
    u8 opaque_00[4];
    polyMovieClip_t *clip;
} polyMovieState_t;

typedef struct polyMovieObject_t {
    u8 opaque_00[0x18];
    polyMovieState_t *state;
} polyMovieObject_t;

s32 evtStopMover(polyMovieObject_t *mover, s32 sw, s32 baseframe, s32 nowframe);

typedef struct evtScaledValue_t {
    u32 word00;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u8 pad18[4];
} evtScaledValue_t;

void dds3PathBasicControlSetRatio(evtScaledValue_t *pControl, f32 ratio);

void dds3PathBasicPauseOn(evtScaledValue_t *value);

void dds3PathBasicPauseOff(evtScaledValue_t *pControl);

extern f32 evtSceneLightDefaultColor[4];

extern f32 evtSceneLightDefaultDir[4];

extern f32 evtSceneLightDefaultAmbient[4];

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

sdfMemHandle_t *evtAllocMovieWork(pmdHeader_t **out);

extern void *memcpy(void *a, const void *b, size_t c);

static inline void UnpackRGBAWordFloat(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, unit);
}

static inline u32 PackRGBAWord(f32 unit) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], unit);
    return rgba[0];
}

/* One asm location for every arm's w set, so jump2 can cross-jump them. */
static inline void SetBlendVecW(f32 w)
{
    SDF_VU0_SETVECTOR_W(w);
}

void UnitFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void CameraFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

s32 QuakeFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

s32 FadeFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void SeFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

s32 BgmFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void BlurFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

dds3Basic_t *AddEffect(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f);

s32 AddEffectTable(s32 now, evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, char *effname, char *targetname, s32 posindex, s32 cutno);

s32 AddEffectTableMG12(s32 now, evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f);

void DelEffectTable_evtPolygonMovie(evtPolygonMovieWork_t *wdata, s32 no);

void EffectFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

dds3Basic_t *SearchBasicFromNameIndex_evtPolygonMovie(evtPolygonMovieWork_t *work, s32 nameIndex);

void MG12FrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void BEDFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

s32 LightFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void SkyFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

s32 BokasiFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

f32 GetKeyFrameRatio(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void SLightFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void SFogFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void Blur2FrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void MBlurFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void DBlurFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void RBlurFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void MFilterFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void FilterFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);

void FrameRun(evtPolygonMovieWork_t *wdata);

s32 evtPolygonMovieProcess2(dds3ProcessID_t *id);

void CheckLoad_evtPolygonMovie(evtPolygonMovieWork_t *work);

s32 evtCheckPolygonMovieRun(dds3ProcessID_t *id);

saddr_t evtPolygonMovieProcess1(dds3ProcessID_t *id);

void evtPolygonMovieShutdown(dds3ProcessID_t *id);

f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2);

s32 evtHokanInt(s32 hokan, f32 ratio, s32 i1, s32 i2);

u32 evtHokanUInt(s32 hokan, f32 ratio, u32 i1, u32 i2);

/* vu0 routine: blend two RGBA8888 colours by t */
u32 evtHokanColor(s32 hokan, f32 ratio, u32 c1, u32 c2);

/* vu0 routine: blend three rows of vectors a and b by t into out (missing input = default rows) */
void evtHokanSceneLight(s32 hokan, f32 ratio, f32 *key1, f32 *key2, f32 *d);

/* vu0 routine: blend a vector and an RGBA8888 colour by t (missing input =
 * default vector/colour) */
void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d);

void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *key1, dds3Effect_Blur_t *key2, dds3Effect_Blur_t *d);

void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d);

void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d);

void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d);

void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d);

void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *key1, dds3Effect_DistortionBlurParam_t *key2, dds3Effect_DistortionBlurParam_t *d);

void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d);

void evtRunUnitMode(dds3Basic_t *unit, s32 mode);

void evtSetPolygonMovieFlag(dds3ProcessID_t *id, s32 flag);

void evtResetPolygonMovieFlag(dds3ProcessID_t *id, s32 flag);

u8 evtCheckPolygonMovieFlag(dds3ProcessID_t *id, u32 flag);

/* Scale the clip to the elapsed fraction of its duration, then apply or undo it. */
s32 evtStopMover(polyMovieObject_t *mover, s32 sw, s32 baseframe, s32 nowframe);

evtPolygonMovieWork_t *InitPolygonMovieWorkData(void);

evtPolygonMovieWork_t *evtGetPolygonMovieWorkData1(evtPolygonMovieWork_t *wdata, void *pm1data, void *pm2data);

void evtFreePolygonMovieWorkData(evtPolygonMovieWork_t *wdata);

/* Allocate a resource block holding a fresh "PMD2" header; returns the handle. */
sdfMemHandle_t *evtAllocMovieWork(pmdHeader_t **out);

void SetPMFileName(s32 eventno, s32 cutno, char *pm1file, char *pm2file);

evtPolygonMovieWork_t *evtGetPolygonMovieWorkData2(s32 eventno, s32 cutno, s32 type);

dds3ProcessID_t *evtStartPolygonMovie2(s32 priority, s32 eventno, s32 cutno);

#endif
