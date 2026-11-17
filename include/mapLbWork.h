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
} mapLbSprite_t;

typedef struct mapLbSpriteBox_t {
    s16 pos[8];     /* x0 y0 x1 y1 x2 y2 x3 y3 */
    s32 tex;        /* 0: not drawn */
    u8 unk14[4];
} mapLbSpriteBox_t;

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
extern mapLbSpriteBox_t *mapSKYBGEDITData4;
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
extern f32 mapData74;
extern s32 mapBgSetScrollData4;
extern f32 mapData75;
extern f32 mapData76;
extern f32 mapZoomClearData;
extern s32 mapData77;
extern s32 mapData78;
extern f32 mapZoomSetGoalData11;
extern f32 mapZoomSetGoalData12;
extern f32 mapZoomSetGoalData13;
extern f32 mapZoomSetGoalData14;
extern f32 mapZoomSetGoalData15;
extern f32 mapZoomSetGoalData16;
extern f32 mapZoomSetGoalData17;
extern f32 mapZoomSetGoalData18;
extern f32 mapData79;
extern f32 mapData80;
extern f32 mapZoomSetGoalData19;
extern f32 mapZoomSetGoalData20;
extern f32 mapZoomSetGoalData21;
extern f32 mapZoomSetGoalData22;
extern f32 mapZoomSetGoalData23;
extern f32 mapZoomSetGoalData24;
extern f32 mapZoomSetGoalData25;
extern f32 mapZoomSetGoalData26;
extern f32 mapData81;
extern f32 mapData82;
extern f32 mapData83;
extern f32 mapData84;
extern f32 mapData85;
extern f32 mapData86;
extern f32 mapZoomSetGoalData27;
extern f32 mapZoomSetGoalData28;
extern f32 mapZoomSetGoalData29;
extern f32 mapZoomSetGoalData30;
extern f32 mapData87;
extern f32 mapData88;
extern f32 mapBgSetScrollData7;
extern f32 mapBgSetScrollData8;
extern f32 mapZoomSetGoalData31;
extern f32 mapZoomSetGoalData32;
extern f32 mapData89;
extern f32 mapData90;
extern f32 mapBgSetScrollData9;
extern f32 mapBgSetScrollData10;
extern f32 mapBgSetScrollData11;
extern f32 mapBgSetScrollData12;
extern f32 mapZoomSetGoalData33;
extern f32 mapZoomSetGoalData34;
extern f32 mapData91;
extern f32 mapData92;
extern f32 mapBgSetScrollData5;
extern f32 mapBgSetScrollData6;
extern s32 mapBgSetScrollData17;
extern s32 mapBgSetScrollData18;
extern f32 mapBgSetScrollData19;
extern f32 mapData93;
extern f32 mapData94;
extern f32 mapData95;
extern f32 mapData96;
extern f32 mapSKYBGEDITData;
extern f32 mapSKYBGEDITData2;
extern f32 mapSKYBGEDITData3;
extern s32 mapWork4;
extern u16 QuakeFrame;
extern f32 mapBgSetScrollData13;
extern f32 mapBgSetScrollData14;
extern f32 mapBgSetScrollData15;
extern f32 mapBgSetScrollData16;
extern f32 mapZoomSetGoalData35;

void mapSKYBGEDIT(void);
extern struct sdfTexHandle_t *fldTitleMiniTex2;
extern struct sdfTexHandle_t *fldTitleMiniWork2;
void fldCallMemFreeDelayedDirect4(void);
void fldCallGrAllocPacketBuff2(u8 *image);
void fldCallGrAllocPacketBuff3(u8 *image);
void fldCallGrAllocPacketBuff4(u8 *image);
void fldCallGrAllocPacketBuff5(u8 *image);

#endif /* MAPLBWORK_H */
