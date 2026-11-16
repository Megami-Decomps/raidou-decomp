#ifndef FLDENC_H
#define FLDENC_H

#include "common.h"
#include "libsdf.h"
#include "effect.h"
#include "kernel.h"

void encInitProcess(void);
void encLoadTexture(void);
void fldCallSetOt5(void);
void encCallPrimAllocDrawTag(void);
void encCallPrimAllocDrawTag2(s32 alpha);
s32 encLoad(void);
void encMode(s32 mode);
void encStop(void);
void fldFreeRunMapData(void);

extern effBlurTemplate_t *hPartialMultiBlur;
extern s32 gencMode;
extern s32 gencStageInnAlp;
extern s32 encDrawState;
extern s32 gencOn;
extern sdfTexHandle_t *fldData42;
extern f32 fldCallSetOt5Data;
extern f32 fldCallSetOt5Data2;
extern char fldEncDrawName[];
extern dds3Effect_PartialMultiBlurParam_t encBlurParam;
s32 encDrawProc(dds3ProcessID_t *aProcId);

#endif
