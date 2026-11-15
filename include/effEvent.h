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
effEveBEDCut_t *effEveBEDCutCreate(struct effBTL_t *hEveBED, u16 CutNum, const void *pParams);
void effEveBEDCutDelete(effEveBEDCut_t *hEveBEDCut);
void effEveBEDCutSetParams(effEveBEDCut_t *hEveBEDCut, const void *pParams);
void effEveBEDCutSetScale(effEveBEDCut_t *hEveBEDCut, f32 scale);
void effEveBEDCutDraw(effEveBEDCut_t *hEveBEDCut);

void Eve_PartialMultiFilterSetDefTex(void);
void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *pParam);
void Eve_RippleBlurSetDefTex(void);

#endif
