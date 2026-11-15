#ifndef POLYMANAGER_H
#define POLYMANAGER_H

#include "common.h"
#include "effect.h"

void dds3PolygonLinkOt(void);
void dds3TrackPolyLinkOt(void);
void dds3TrackPolyColorSet(dds3TrackPoly_t *hTrackPoly, s32 number, u32 color);
dds3TrackPoly_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);
void dds3TrackPolyDelete(dds3TrackPoly_t *hTrackPoly);
void dds3TrackPolyDraw(dds3TrackPoly_t *node);
void dds3TrackPolyListInit(dds3TrackPoly_t *hTrackPoly, s32 number);
void dds3TrackPolySet(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pV);
void dds3TrackPolySet2(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pV);
void dds3TrackPolyVC3Set_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);
void dds3TrackPolyVC5Set_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);
void dds3TrackPolyVCRefresh_ParType1(dds3TrackPoly_t *hTrackPoly, s32 index);
void dds3TrackPolyVCRefresh_ParType2(dds3TrackPoly_t *hTrackPoly, s32 index);
void dds3TrackPolyVCSet(dds3TrackPoly_t *hTrackPoly, s32 type, u32 abgr_in, u32 abgr_out);

#endif
