#ifndef MAPLBWORK_H
#define MAPLBWORK_H

#include "common.h"
#include "libsdf.h"

typedef struct mapLbHead_t {
    u8 unk00[8];
    s16 w;
    s16 h;
    s32 entrySize;  /* bit 31 set: a sub block with per-cell sizes follows the entries */
    s16 mask[16];   /* one bit per column, per row */
} mapLbHead_t;

typedef struct mapLbWork_t {
    mapLbHead_t *head;
    sdfMemHandle_t *handle;
    u8 *entry[81];
    mapLbHead_t *sub;
    u8 *cell[81];
} mapLbWork_t;

typedef struct mapLbSprite_t {
    s16 pos[8];     /* x0 y0 x1 y1 x2 y2 x3 y3, scaled by Zoom */
    s16 uv[8];
    s32 tex;        /* 0: not drawn */
    u8 unk24[4];
} mapLbSprite_t;

typedef struct mapLbIndex_t {
    s32 count;      /* sprites at mapSKYBGEDITData4 */
    u32 sectionOfs;
} mapLbIndex_t;

typedef struct mapLbSection_t {
    u32 ofs[100];
    u32 count[100];
} mapLbSection_t;

extern sdfMemHandle_t *fldJmsQuestMapLoad_fldData;
extern u8 *fldJmsQuestMapLoad_fldData2;
extern mapLbWork_t evtStageTestState2;
extern s32 fldData23;
extern s8 fldData[];
extern mapLbIndex_t *fldData21;
extern mapLbSprite_t *mapSKYBGEDITData4;
extern mapLbSection_t *fldCallSetOt3Data;
extern mapLbSprite_t *fldData5[];
extern s32 fldData22;
extern f32 Zoom;
extern f32 bg_scroll_last_x;
extern f32 mapData71;
extern f32 mapData72;
extern f32 mapData73;
extern f32 mapZoomSetGoalData9;
extern f32 mapZoomSetGoalData10;
extern s32 mapData74;
extern s32 mapBgSetScrollData4;
extern f32 mapData75;
extern f32 mapData76;
extern s32 mapZoomClearData;
extern s32 mapData77;
extern s32 mapData78;

void mapSKYBGEDIT(void);
extern void *fldTitleMiniTex2;

#endif /* MAPLBWORK_H */
