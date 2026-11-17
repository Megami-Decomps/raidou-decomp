#ifndef EFFEVENT_H
#define EFFEVENT_H

#include "common.h"
#include "effect.h"

struct effBTL_t;

typedef struct effEveBEDParams_t {
    f32 center[4];
    f32 rot[4];
    f32 r;
    f32 h;
    f32 scale;
    u32 abgr;
} effEveBEDParams_t;

typedef struct effEveBEDCut_t effEveBEDCut_t;

struct effBTL_t *effEveBEDCreate(struct effBTL_t *pData);
void effEveBEDDelete(struct effBTL_t *hEveBED);
s8 effCallMemcpy3(void);
effEveBEDCut_t *effEveBEDCutCreate(struct effBTL_t *hEveBED, u16 CutNum, const void *pParams);
void effEveBEDCutDelete(effEveBEDCut_t *hEveBEDCut);
void effEveBEDCutSetParams(effEveBEDCut_t *hEveBEDCut, const void *pParams);
void effEveBEDCutSetScale(effEveBEDCut_t *hEveBEDCut, f32 scale);
void effEveBEDCutDraw(effEveBEDCut_t *hEveBEDCut);

void Eve_PartialMultiFilterSetDefTex(void);
void Eve_PartialMultiFilterSetTex(sdfTexHandle_t *hTex);
void Eve_RippleBlurSetTex(sdfTexHandle_t *hTex);
void Eve_PartialMultiBlurSetTex(sdfTexHandle_t *hTex);
void Eve_DistortionBlurSetTex(sdfTexHandle_t *hTex);
void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *pParam);
void Eve_RippleBlurSetDefTex(void);
void Eve_PartialMultiBlurSetDefTex(void);
void Eve_DistortionBlurSetDefTex(void);
void Eve_PartialMultiBlurON(void);
void Eve_PartialMultiBlurOFF(void);
void Eve_PartialMultiFilterON(void);
void Eve_PartialMultiFilterOFF(void);
void Eve_DistortionBlurON(void);
void Eve_DistortionBlurOFF(void);
void Eve_RippleBlurON(void);
void Eve_RippleBlurOFF(void);
void Eve_FilterON(void);
void Eve_FilterOFF(void);
void Eve_BlurOFF(void);
void effEnableFramebufferQuad(void);
void effDisableFramebufferQuad(void);
effScreenDrawParams_t *BlurEditorGetParams(void);
effBlurTemplate_t *effMultiBlurSwapSub(void);
effBlurScatterWork_t *effDistBlurSwapSub(void);
effBlurScaleWork_t *Eve_RippleBlurGetParams(void);
effScreenDrawParams_t *effGetCh73Params(void);
dds3Effect_Filter_t *effFilter2SwapSub(void);
dds3Effect_PartialMultiFilter_t *effMFilterSwapSub(void);
dds3Effect_PartialMultiBlurParam_t *mblurGetParam(void);
dds3Effect_DistortionBlurParam_t *dblurGetParam(void);
dds3Effect_Blur_t *effGetLoadDescD(void);
dds3Effect_PartialMultiFilterParam_t *dblurGetParam2(void);
dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void);
dds3Effect_RippleBlurParam_t *rblurGetParam(void);

#include "effManager.h"


/* Show flags, one byte each in sdata. */
extern s8 Eve_BlurShow;
extern s8 Eve_PartialMultiBlurShow;
extern s8 Eve_DistortionBlurShow;
extern s8 EffectDrawData2;
extern s8 Eve_FilterShow;
extern s8 Eve_PartialMultiFilterShow;
extern s8 Eve_RippleBlurShow;
extern effScreenDrawParams_t Eve_Blur;
extern dds3Effect_Filter_t Eve_Filter;

void BlurSetParam2(dds3Effect_Blur_t *pParam);
void EffectDraw(void);
void Eve_FilterSetParam(dds3Effect_Filter_t *pParam);
void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *pParam);
void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *pParam);
void Eve_RippleBlurSetParam(void *pParam);
void effCopyFramebufferQuadParameters(const dds3Effect_Blur_t *source);
void effBTLSys_Nego_Stone_Delete2(effEveBEDCut_t *hEveBEDCut);

#endif
