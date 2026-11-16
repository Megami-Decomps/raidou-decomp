#ifndef SDATUNIT_H
#define SDATUNIT_H

#include "common.h"
#include "sdf_types.h"

/* One set of unit parameters. */
typedef struct sdatParaWork_t {
    s32 hp;
    s32 maxhp;
    s32 mp;
    s32 maxmp;
    s16 str;
    s16 vit;
    s16 tec;
    s16 luck;
} sdatParaWork_t;

/* Parameters from equipment: each stat holds two item bonuses, one per byte. */
typedef struct sdatItemParaWork_t {
    s32 hp;
    s32 maxhp;
    s32 mp;
    s32 maxmp;
    u8 str[2];
    u8 vit[2];
    u8 tec[2];
    u8 luck[2];
} sdatItemParaWork_t;

/* Affinity per attribute, 100 is neutral. */
typedef struct SBTL_AISYO_TBL {
    s32 word[16];
} SBTL_AISYO_TBL;

/* A unit's work: the player and each stock devil, and embedded in SBTLUNIT_WORK. */
typedef struct sdatUnitWork_t {
    /* 0x00 */ u32 flag;
    /* 0x04 */ s8 raceId;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 devilId;
    /* 0x08 */ s8 level;
    /* 0x09 */ u8 seikaku;
    /* 0x0A */ u8 pad0A[0xA];
    /* 0x14 */ s32 exp;
    /* 0x18 */ sdatParaWork_t now;
    /* 0x30 */ sdatParaWork_t backup;
    /* 0x48 */ sdatItemParaWork_t pitem;
    /* 0x60 */ sdatParaWork_t pbtl;
    /* 0x78 */ sdatParaWork_t huuma;
    /* 0x90 */ s32 haveskill[10];
    /* 0xB8 */ u32 badStatus; /* sbtlGetBadStatus reads it */
    /* 0xBC */ u8 padBC[0x14];
    /* 0xD0 */ SBTL_AISYO_TBL aisyo;
} sdatUnitWork_t;

typedef sdatUnitWork_t datPartyDevil_t;

#define SDAT_STOCK_NUM 12
#define SDAT_ITEM_NUM 160

/* Player data, 0xFA0 bytes at DDS3_GBWK+0x1CA0. Only the fields used so far are named. */
typedef struct SBTL_PLAYER_DATA {
    /* 0x000 */ sdfVector4_t vec[2];
    /* 0x020 */ s32 val20;
    /* 0x024 */ s32 val24;
    /* 0x028 */ s32 maka;
    /* 0x02C */ s32 val2C;
    /* 0x030 */ u8 pad30[4];
    /* 0x034 */ sdatUnitWork_t unit;
    /* 0x144 */ u8 pad144[2];
    /* 0x146 */ s16 val146;
    /* 0x148 */ u8 pad148[4];
    /* 0x14C */ s32 val14C;
    /* 0x150 */ s32 val150;
    /* 0x154 */ s32 val154;
    /* 0x158 */ u8 pad158[0x198 - 0x158];
    /* 0x198 */ s16 item[SDAT_ITEM_NUM]; /* stock per item id */
    /* 0x2D8 */ sdatUnitWork_t stock[SDAT_STOCK_NUM];
    /* 0xF98 */ u8 padF98[2];
    /* 0xF9A */ u16 unkF9A;
    /* 0xF9C */ s8 partyNum;
    /* 0xF9D */ u8 padF9D[3];
} SBTL_PLAYER_DATA;

#endif
