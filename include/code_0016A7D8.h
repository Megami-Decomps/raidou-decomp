#ifndef CODE_0016A7D8_H
#define CODE_0016A7D8_H

#include "common.h"
#include "sdf_resource.h"

extern s32 am_top_x;
extern s32 am_top_z;
extern s32 am_map;
extern s32 am_floor;
extern f32 mapCallSetOt2Data7;
extern f32 mapCallSetOt2Data8;
extern f32 mapCallSetOt2Data9;
extern f32 fldData34;
extern f32 fldData35;
extern s32 fldData36;
extern s32 mapCallSetOt2Data10;
extern s32 draw_flags;
/* Bounding box (min x, min z, max x, max z) of the visible cells mapAutoMapUpdateLimit counted. */
extern s32 am_min_x;
extern s32 am_min_z;
extern s32 am_max_x;
extern s32 am_max_z;
/* {4, 9, 59, 49}; mapAutoMapCalcPosLimit centres the z view on [1]..[3]. */
extern s16 limit_ofs[4];
extern struct sdfTexHandle_t *effQueuedResourceNameRecord4;

s32 fldFindSceneEntryData(s32 id);
s32 mapAutoMapUpdateLimit(s32 floor);
s32 fldDrawTopSub(void);
void mapAutoMapCalcPosLimit(s32 dx, s32 dz);
void fldMapAllAmap(void);
void mapFunc10(void);

#endif
