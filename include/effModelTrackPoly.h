#ifndef EFFMODELTRACKPOLY_H
#define EFFMODELTRACKPOLY_H

#include "common.h"
#include "model.h"

typedef struct
{
    mdlObj_t *hModel;
    s32 mark0;
    s32 mark1;
    f32 frame_s;
    f32 frame_e;
    u32 record;
    s32 face;
    u32 split;
    u32 blend;
    u32 gradientColors[4];
} Eff_ModelTrackPolyParam_t;

typedef struct effMTPolyListHandle_t effMTPolyListHandle_t;

effMTPolyListHandle_t *Eff_ModelTrackPolyListCreate(Eff_ModelTrackPolyParam_t *pParam, u32 number);
void Eff_ModelTrackPolyListDelete(effMTPolyListHandle_t *list);
void Eff_ModelTrackPolyListReset(effMTPolyListHandle_t *hMTPolyList, s32 index);
void Eff_ModelTrackPolyListDrawOnly(effMTPolyListHandle_t *hMTPolyList);

#endif
