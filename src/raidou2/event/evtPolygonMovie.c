#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtPolygonMovie.c */
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

extern const char evtPolygonMovieStartMsg[];

extern const char evtPolygonMovieMapMsg[];

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

extern const char evtPolygonMovieStartMsg[40];

extern const char evtPolygonMovieMapMsg[16];

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

extern saddr_t evtCameraFrameState;

extern f32 nowfovy;

extern s32 evtCameraLastFrame;
/* End of declarations from src/smt3/event/evtPolygonMovie.c */

/* Declarations from dds-decomp src/dds1/event/evtPolygonMovie.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_evtFrameFunc_t 1

#define SMT3_DEFINES_evtScaledValue_t 1

#define SMT3_DEFINES_fileRequest_t 1

#define SMT3_DEFINES_pmdCameraTable_t 1

#define SMT3_DEFINES_polyMovieClip_t 1

#define SMT3_DEFINES_polyMovieObject_t 1

#define SMT3_DEFINES_polyMovieState_t 1

#include "dds/dds3_task_lifecycle.h"

#include "dds/itf.h"

#include "dds/sdf_file_existence_api.h"

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

/* Return a retained texture handle after releasing the temporary asset allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *path);

SdfFileExistenceResult sdfFileCheckExist(const char *path);

#include "dds/sdk_ps2.h"

#include "dds/common.h"

#include "dds/sdf_file_existence_api.h"

#include "dds/dds3_object_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/eff.h"

#include "dds/eff_event_draw.h"

#include "dds/sdf_chip.h"

#include "dds/file_request_api.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/sdf_heap_api.h"

#include "dds/ee_mmi.h"

#include "dds/pcp_vu0.h"

#include "dds/fld.h"

#include "dds/evt_polygon_movie.h"

#include "dds/evt_polygon_movie_load_api.h"

#include "dds/evt_unit.h"

#include "dds/mdl.h"

#include "dds/dds3obj.h"

#include "dds/itf.h"

#include "dds/itf_mes_resource.h"

#include "dds/sdf_float_counter.h"

#include "dds/evt_blend_vec_color.h"

#include "dds/mnu_title_sound.h"

extern f32 D_00368540[4];

extern f32 D_00368550[4];

extern f32 D_00368560[4];

extern dds3Effect_Blur_t D_00368590;

extern dds3Effect_PartialMultiBlurParam_t evtMovieDefaultBlurTemplate;

extern dds3Effect_Filter_t evtMovieDefaultSolidRect;

extern dds3Effect_PartialMultiFilterParam_t evtMovieDefaultResourceRect;

extern dds3Effect_DistortionBlurParam_t evtMovieDefaultBlurScatter;

extern dds3Effect_RippleBlurParam_t evtMovieDefaultBlurScale;

extern dds3Effect_Blur_t D_003686A0;

extern ItfMesWindowIndex itfMesMngInitialize(ItfMesSub *messageSub);

extern void mnuStopTitleVoicePlayback(void);

#include "dds/sdf_debug_api.h"

extern f32 D_00368570[4];

extern u32 D_00368580[4];

/* One asm location for every arm's w set, so jump2 can cross-jump them. */
static inline void evtSetBlendVecW(f32 w)
{
    VU0_SCALAR_OP_CLOBBER(w, "vmulx.w vf10, vf0, vf2x");
}

void evtBlendParamsG(s32 enable, f32 t, const dds3Effect_Blur_t *a, const dds3Effect_Blur_t *b, dds3Effect_Blur_t *out);

void evtBlendParamsH(s32 enable, f32 t, const EvtBlendKey *a, const EvtBlendKey *b, EvtBlendKey *out);

void evtPolygonMovieSetObjectMode(dds3Basic_t *obj, EvtPolygonMovieObjectMode mode,
                                  u32 setFlags, u32 clearFlags);

/* Apply a caller-provided mask to the viewer's movie-work flags. */
void evtPolygonMovieSetFlagBits(dds3ProcessID_t *task, EvtPolygonMovieWorkFlags flags);

void evtPolygonMovieClearFlagBits(dds3ProcessID_t *task, EvtPolygonMovieWorkFlags flags);

/* Store elapsed frames, bounded below by zero and above by clip duration. */
void evtPolygonMovieClampTime(dds3Basic_t *movie, s32 unused, s32 start, s32 end);

PolyMovieWork *evtPolygonMovieInitWork(PolyMovieWork *work, PmdHeader *mainHeader, PmdHeader *sub, PmdHeader *sub2);

void evtPolygonMovieFreeWork(PolyMovieWork *work);

#include "dds/common.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_heap_api.h"

#include "dds/evt_viewer.h"

#include "dds/event.h"

#include "dds/kwln.h"

#include "dds/sdf.h"

#include "dds/evt_world.h"

#include "dds/evt_picture.h"

#include "dds/evt_polygon_movie.h"

#include "dds/file.h"

#include "dds/file_request_api.h"

#include "dds/dds3_task_lifecycle.h"

extern char D_003BBF78[];

/* Build the 0x20-byte PMD3 resource header, copy it into a fresh allocation and
 * hand the retained address back through `out`. */
sdfMemHandle_t *evtPolygonMovieSynthesizePmd3Header(PmdHeader **out);

s32 evtFormatPolygonMoviePaths(s32 eventId, s32 sceneId, char *path1, char *path2, char *path3);

extern const char evtMovieBeforePrimaryLoadMessage[31];

extern const char evtMoviePrimaryLoadedMessage[31];

extern const char evtMovieTertiaryLoadedMessage[26];

extern const char evtMovieResourcesReadyMessage[31];

extern const char evtMovieWorkInitializedMessage[31];

extern u32 D_003BA8EC;

extern void evtEventViewerDestroyActiveAlternate(dds3ProcessID_t *task);

extern void evtEventViewerInitCampFontResource(dds_evtViewerInfo_t *viewer);

extern char D_003ADDB0[];

/* Create the event viewer task at `priority` for `eventId`/`sceneId` and request its movie files. */
dds3ProcessID_t *evtViewerCreateTask(u32 priority, s32 eventId, s32 sceneId);

/* Flag operations act on the scheduler task's user-value context. */
void evtPictureDrawOn(dds3ProcessID_t *task);

void evtPictureDrawOff(dds3ProcessID_t *task);

void evtKillPictureProcess(dds3ProcessID_t *task);

/* Allocate the picture task's flag and texture state. */
EvtPictureWork *evtAllocateContext(void);

void LoadTexture(EvtPictureWork *pictureWork, const char *path);
/* End of declarations from src/dds1/event/evtPolygonMovie.c */

f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2)
{
    if (hokan) {
        return f1 * (1.0f - ratio) + f2 * ratio;
    }
    return f1;
}

s32 evtHokanInt(s32 hokan, f32 ratio, s32 i1, s32 i2)
{
    s32 result = i1;

    if (hokan) {
        result = (s32)((f32)i1 * (1.0f - ratio) + (f32)i2 * ratio);
    }
    return result;
}

u32 evtHokanUInt(s32 hokan, f32 ratio, u32 i1, u32 i2)
{
    if (hokan) {
        return (u32)((f32)i1 * (1.0f - ratio) + (f32)i2 * ratio);
    }
    return i1;
}

/* vu0 routine: blend two RGBA8888 colours by t */
u32 evtHokanColor(s32 hokan, f32 ratio, u32 c1, u32 c2)
{
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;

    if (hokan) {
        color1[0] = c1;
        MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        color2[0] = c2;
        MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(ratio);
        SDF_VU0_ADDVECTOR();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        blended[0] = packed;
        return blended[0];
    }
    return c1;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001B8A30);

/* vu0 routine: blend a vector and an RGBA8888 colour by t (missing input =
 * default vector/colour) */
void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d)
{

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 != NULL) {
        SDF_VU0_SETVECTOR_X(key1->vec[0]);
        SDF_VU0_SETVECTOR_Y(key1->vec[1]);
        SDF_VU0_SETVECTOR_Z(key1->vec[2]);
        SetBlendVecW(key1->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (key2 != NULL) {
        SDF_VU0_SETVECTOR_X(key2->vec[0]);
        SDF_VU0_SETVECTOR_Y(key2->vec[1]);
        SDF_VU0_SETVECTOR_Z(key2->vec[2]);
        SetBlendVecW(key2->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_GETVECTOR_X(d->vec[0]);
    SDF_VU0_GETVECTOR_Y(d->vec[1]);
    SDF_VU0_GETVECTOR_Z(d->vec[2]);
    SDF_VU0_GETVECTOR_W(d->vec[3]);
    if (key1 != NULL) {
        UnpackRGBAWordFloat(key1->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(evtSceneFogDefaultColor[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (key2 != NULL) {
        UnpackRGBAWordFloat(key2->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(evtSceneFogDefaultColor[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    d->color = PackRGBAWord(128.0f);
}

void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *key1, dds3Effect_Blur_t *key2, dds3Effect_Blur_t *d)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtBlur2DefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtBlur2DefaultParam;
    }
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->scale = evtHokanFloat(hokan, ratio, key1->scale, key2->scale);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        d->center[positionOrCornerIndex] = evtHokanInt(hokan, ratio, key1->center[positionOrCornerIndex], key2->center[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->xy[positionOrCornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->xy[positionOrCornerIndex][coordinateIndex], key2->xy[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtMultBlurDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtMultBlurDefaultParam;
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    d->Blur.abgr = evtHokanColor(hokan, ratio, key1->Blur.abgr, key2->Blur.abgr);
    d->Blur.blend = key1->Blur.blend;
    d->Blur.rotate = evtHokanFloat(hokan, ratio, key1->Blur.rotate, key2->Blur.rotate);
    d->Blur.scale = evtHokanFloat(hokan, ratio, key1->Blur.scale, key2->Blur.scale);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        d->Blur.center[positionOrCornerIndex] = evtHokanInt(hokan, ratio, key1->Blur.center[positionOrCornerIndex], key2->Blur.center[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->Blur.xy[positionOrCornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->Blur.xy[positionOrCornerIndex][coordinateIndex], key2->Blur.xy[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d)
{
    s32 cornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtFilterDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtFilterDefaultParam;
    }
    for (cornerIndex = 0; cornerIndex < 2; cornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->xy[cornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->xy[cornerIndex][coordinateIndex], key2->xy[cornerIndex][coordinateIndex]);
        }
    }
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
}

void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d)
{
    s32 centerCoordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtMultFilterDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtMultFilterDefaultParam;
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    for (centerCoordinateIndex = 0; centerCoordinateIndex < 2; centerCoordinateIndex++) {
        d->center[centerCoordinateIndex] = evtHokanInt(
            hokan, ratio, key1->center[centerCoordinateIndex], key2->center[centerCoordinateIndex]);
    }
    d->Filter.rgba = evtHokanColor(hokan, ratio, key1->Filter.rgba, key2->Filter.rgba);
    d->Filter.blendControl = key1->Filter.blendControl;
}

void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *key1, dds3Effect_DistortionBlurParam_t *key2, dds3Effect_DistortionBlurParam_t *d)
{
    s32 positionAxisIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtDistBlurDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtDistBlurDefaultParam;
    }
    d->number = evtHokanUInt(hokan, ratio, key1->number, key2->number);
    d->interval = evtHokanUInt(hokan, ratio, key1->interval, key2->interval);
    d->speed = evtHokanFloat(hokan, ratio, key1->speed, key2->speed);
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->distortion = evtHokanFloat(hokan, ratio, key1->distortion, key2->distortion);
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        d->center[positionAxisIndex] = evtHokanInt(hokan, ratio, key1->center[positionAxisIndex], key2->center[positionAxisIndex]);
    }
    d->width = evtHokanUInt(hokan, ratio, key1->width, key2->width);
}

void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d)
{
    s32 positionAxisIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtRipBlurDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtRipBlurDefaultParam;
    }
    d->number = evtHokanUInt(hokan, ratio, key1->number, key2->number);
    d->speed = evtHokanFloat(hokan, ratio, key1->speed, key2->speed);
    d->interval = evtHokanFloat(hokan, ratio, key1->interval, key2->interval);
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->power = evtHokanFloat(hokan, ratio, key1->power, key2->power);
    d->frequency = evtHokanFloat(hokan, ratio, key1->frequency, key2->frequency);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        d->center[positionAxisIndex] = evtHokanInt(hokan, ratio, key1->center[positionAxisIndex], key2->center[positionAxisIndex]);
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtBlendParamsG);

void evtBlendParamsH(s32 enable, f32 t, const EvtBlendKey *a, const EvtBlendKey *b, EvtBlendKey *out)
{
    s32 colorChannelIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    out->x = evtHokanInt(enable, t, a->x, b->x);
    out->w[0] = evtHokanInt(enable, t, a->w[0], b->w[0]);
    out->w[1] = evtHokanInt(enable, t, a->w[1], b->w[1]);
    out->w[2] = evtHokanInt(enable, t, a->w[2], b->w[2]);
    out->w[3] = evtHokanInt(enable, t, a->w[3], b->w[3]);
    out->flagWord = a->flagWord;
    for (colorChannelIndex = 0; colorChannelIndex < 3; colorChannelIndex++) {
        out->y[colorChannelIndex] = evtHokanInt(enable, t, a->y[colorChannelIndex], b->y[colorChannelIndex]);
        out->z[colorChannelIndex] = evtHokanInt(enable, t, a->z[colorChannelIndex], b->z[colorChannelIndex]);
    }
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001B99E0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001B9B90);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001B9BC8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001B9C00);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001B9CA8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001B9D00);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001B9D78);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BA008);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BA378);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BA418);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BA498);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BA598);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_r1_0044FD18);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BA640);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BA740);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BA7E0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_r1_001BAAF0);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", D_r1_0046C7F0);

