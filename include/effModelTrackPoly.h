#ifndef EFFMODELTRACKPOLY_H
#define EFFMODELTRACKPOLY_H

#include "common.h"
#include "model.h"
#include "sdfMat.h"
#include "sdfMemory.h"

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

typedef struct dds3HermitianTrackPoly_tag {
    u32 blend;
    u32 abgr;
    s32 point;
    s32 count;
    s32 current;
    s32 split;
    u128 *pVertex;
    u32 *pVC;
    sdfMat_t *pMat;
    sdfMemHandle_t *hMem;
} dds3HermitianTrackPoly_tag;

/* Thirteen copied words, followed by an update counter and owned history. */
typedef struct {
    struct mdlObj_t *model;
    s32 idA;
    s32 idB;
    f32 startFrame;
    f32 endFrame;
    u32 sampleInterval;
    s32 historyLength;
    u32 unk1C;
    u32 drawKind;
    u32 gradientColors[4];
} EffTrackPolyParams;

typedef struct effTrackPolyWork_t {
    EffTrackPolyParams params;
    u32 updateCount;
    dds3HermitianTrackPoly_tag *data;
} effTrackPolyWork_t;

typedef char EffTrackPolyParams_size_must_be_0x34[(sizeof(EffTrackPolyParams) == 0x34) ? 1 : -1];

typedef char effTrackPolyWork_size_must_be_0x3C[(sizeof(effTrackPolyWork_t) == 0x3C) ? 1 : -1];

dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 face, s32 split);
void dds3HermitianTrackPolySetVC(dds3HermitianTrackPoly_tag *hPoly, u32 *pABGR);
void dds3HermitianTrackPolySetBlend(dds3HermitianTrackPoly_tag *hPoly, u32 blend);
void dds3HermitianTrackPolyReset(dds3HermitianTrackPoly_tag *hPoly);

typedef struct effMTPolyListHandle_t effMTPolyListHandle_t;

effMTPolyListHandle_t *Eff_ModelTrackPolyListCreate(Eff_ModelTrackPolyParam_t *pParam, u32 number);
void Eff_ModelTrackPolyListDelete(effMTPolyListHandle_t *list);
void Eff_ModelTrackPolyListReset(effMTPolyListHandle_t *hMTPolyList, s32 index);
void Eff_ModelTrackPolyListDrawOnly(effMTPolyListHandle_t *hMTPolyList);

#endif
