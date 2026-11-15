#ifndef SBTLWORK_H
#define SBTLWORK_H

#include "common.h"
#include "fileManager.h"
#include "kernel.h"
#include "sdf_types.h"

struct EffectSlotSet;

/* Pointed to by SBTL_WORK.panel at 0x628. */
typedef struct SBTL_PANEL {
    u8 pad00[8];
    struct EffectSlotSet *resources;
    u8 pad0C[0x14];
    u32 flg;
} SBTL_PANEL;

/* Battle main work, pointed to by wk_sbtl. Only fields read so far are named. */
typedef struct SBTL_WORK {
    u8 pad000[0x150];
    s32 mapNo[2];
    u8 pad158[4];
    u32 startFlg;
    u8 pad160[0x198 - 0x160];
    fileRequest_t *panelPack;
    s32 panelPackPos;
    s32 panelPackLen;
    u8 pad1A4[8];
    dds3ProcessID_t *process;
    u8 pad1B0[0x14];
    u32 flg;
    u32 flg2;
    u8 pad1CC[4];
    s32 targetUnitID;
    u8 pad1D4[4];
    s32 stageNo;
    struct SBTLUNIT_LISTWORK *unitList;
    u8 pad1E0[0x400 - 0x1E0];
    s32 *unitEffect;
    u8 pad404[0x628 - 0x404];
    SBTL_PANEL *panel;
} SBTL_WORK;

extern SBTL_WORK *wk_sbtl;

/* Devil table entry returned by sbtlGetDevilData_sbtlSub. */
typedef struct SBTL_DEVIL_DATA {
    u32 flag;
    u8 field04;
    u8 pad05;
    s16 motionId;
    u8 pad08[2];
    s16 field0A;
    s16 aisyoId;
    u8 field0E;
    u8 field0F;
    u8 pad10[4];
    s16 field14;
    u8 pad16[2];
    u32 stat18;
    u32 stat1C;
    u32 stat20;
    u32 stat24;
    u16 resist[4];
    s16 param[9];
    s16 field42;
    u8 pad44[0x60 - 0x44];
    f32 field60;
    f32 field64;
    u8 pad68[2];
    s16 huumaMesNo;
} SBTL_DEVIL_DATA;

typedef struct SBTL_PLAYER_GAUGE {
    s32 val[16];
} SBTL_PLAYER_GAUGE;

typedef struct SBTL_PLAYER_BLOCK {
    s32 word[4];
    s16 half[4];
} SBTL_PLAYER_BLOCK;

/* Player data, 0xFA0 bytes. Only the fields initialised so far are named. */
typedef struct SBTL_PLAYER_DATA {
    sdfVector4_t vec[2];
    s32 val20;
    s32 val24;
    s32 val28;
    s32 val2C;
    u8 pad30[4];
    s32 val34;
    u8 pad38[4];
    u8 val3C;
    u8 pad3D[0x48 - 0x3D];
    s32 val48;
    SBTL_PLAYER_BLOCK block[2];
    u8 pad7C[0x104 - 0x7C];
    SBTL_PLAYER_GAUGE gauge;
    u8 pad144[2];
    s16 val146;
    u8 pad148[4];
    s32 val14C;
    s32 val150;
    s32 val154;
    u8 pad158[0x238 - 0x158];
    u16 val238;
    u8 pad23A[0xF9C - 0x23A];
    u8 valF9C;
    u8 padF9D[3];
} SBTL_PLAYER_DATA;

/* Battle start parameters, copied to main_data by sbtlSbattle. */
typedef struct SBTL_START_DATA {
    u8 pad00[0x28];
    s32 val28;
    s32 mode;
    s16 val30;
    u8 pad32[2];
} SBTL_START_DATA;

/* One loaded SB_PACK resource, indexed by the pack entry id. */
typedef struct SBTL_PACK_SLOT {
    void *data;
    struct sdfMemHandle_t *handle;
    void *unk8;
    void *unkC;
} SBTL_PACK_SLOT;

SBTL_PLAYER_DATA *sbtlGetPlayerData(void);

/* One battle stage's row (0x100 bytes) in the table sbtlData100 points at, indexed by SBTL_WORK.stageNo. */
typedef struct SBTL_STAGE_DATA {
    s8 mapNo[2];    /* 0x00: copied to SBTL_WORK.mapNo */
    s8 form;        /* 0x02: mapMapSetForm, 0 for none */
    s8 encPoint;    /* 0x03: mapEncPointGet point */
    u8 pad04[2];
    s16 bgmId;      /* 0x06 */
    u8 pad08[0xF8];
} SBTL_STAGE_DATA;

extern SBTL_STAGE_DATA *sbtlData100;

#endif
