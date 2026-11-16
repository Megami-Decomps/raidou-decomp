#ifndef CODE_0015D490_H
#define CODE_0015D490_H

#include "common.h"

struct sprHandle_t;
struct effHandle_t;

/* One looping field sound effect: kankyo (ambient) slots and moving-object slots. */
typedef struct mapSoundSe_t {
    s32 active;
    s32 se;
    s32 vol;
    s32 update;
    s32 state; /* playing flag for moving slots, repeat countdown for kankyo slots */
} mapSoundSe_t;

/* A party member sound effect played after a delay in frames. */
typedef struct mapSeDelay_t {
    s16 timer;
    s16 vol;
} mapSeDelay_t;

/* Bad-status effects drawn on the player (mapData117) and the summoned devil (mapCallMemset2Data). */
extern struct effHandle_t *mapData117;
extern s32 mapData118;
extern struct effHandle_t *mapCallMemset2Data;
extern s32 mapData120;
extern s32 mapData121;
extern u8 *mapCallEnableModeData2[];
extern f32 mapIfDrawESData2;
extern char fldTitleTaskName[];
extern s32 fldBgmSmgAddr;
extern s32 fldBgmLoadStep;
extern s32 p2sdData2;
extern s32 mapData129;
extern s32 mapSoundKankyoTaskSubData;
extern s32 mapData131;
extern s32 mapSeWallHitData;
extern s32 takara_openanm_objid;
extern s32 fldCallSetOt4Data;
extern s32 fldCallSetOt4Data2;
extern s32 p2sdSoundSmgLoadChkSubData[];
extern s16 p2sdCallBitCheckData[];
extern struct sprHandle_t *fldDrwZBuf2;
/* Two 33x33 grids of small random offsets; fldCallGetRandA3 fills the first 32x32. */
extern f32 fldWork7[33][33];
extern f32 fldCallSetOtWork[33][33];
extern mapSoundSe_t mapFileLoadSub8Work[8];
extern mapSoundSe_t mapSoundKankyoSeSetWork[64];
extern mapSeDelay_t datWork7[16];

s32 fldIsMapTitleActive(void);
s32 sbtlGetRndEncCnt(void);
s32 sbtlGetRndEncMax(s32 area);
s32 p2sdSoundSmgLoadChkSub(s32 map);
void p2sdVolume2(s32 se, s32 vol);
void p2sdPlaySE_Vol_Loop(s32 se);
void mapSoundKankyoTaskSub(void);
void mapCallPlaySEVolLoop(void);
void mapCallPlaySEVolLoop2(void);
void mapSeNakamaDelayTask(void);
void mapCallMemset2(void);
void mapIfDrawES(void);
void mapCallEnableMode(s32 map, s32 floor, s32 cut);

extern const char DecisionFeedInStrMapAll[];
extern const char DecisionFeedInStrMapMeiSpr[];
extern const char mapStrTakaraOpenanmObjid[];

#endif
