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

extern SBTL_AISYO_TBL *aisyo_tbldata;

/* A unit's work: the player and each stock devil, and embedded in SBTLUNIT_WORK. The raidou2 tail is partial. */
typedef struct sdatUnitWork_t {
    /* 0x00 */ u32 flag;
    /* 0x04 */ s8 raceId;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 devilId;
    /* 0x08 */ s8 level;
    /* 0x09 */ s8 seikaku;
    /* 0x0A */ s8 dummy0;
    /* 0x0B */ u8 pad0B;
    /* 0x0C */ s16 chuuseiHigh;
    /* 0x0E */ s16 chuusei;
    /* 0x10 */ s16 vice; /* HD vice, spent by sbtlDecVice */
    /* 0x12 */ u8 pad12[0x2];
    /* 0x14 */ s32 exp;
    /* 0x18 */ sdatParaWork_t now;
    /* 0x30 */ sdatParaWork_t backup;
    /* 0x48 */ sdatItemParaWork_t pitem;
    /* 0x60 */ sdatParaWork_t pbtl;
    /* 0x78 */ sdatParaWork_t huuma;
#ifdef VERSION_RAIDOU2
    /* 0x90 */ s32 haveskill[17];
    /* 0xD4 */ s32 battleskill[4];
    /* 0xE4 */ s8 autoskill[17]; /* set for a haveskill entry the unit repeats on its own */
    /* 0xF5 */ u8 padF5[0xF8 - 0xF5];
    /* 0xF8 */ u32 badStatus; /* sbtlGetBadStatus reads it */
    /* 0xFC */ u8 padFC[0x4];
    /* 0x100 */ s16 autost_cnt;
    /* 0x102 */ s16 autost_cnt2; /* frames between auto skill heals */
    /* 0x104 */ u32 itemstatus;
    /* 0x108 */ s16 itemst_cnt;
    /* 0x10A */ s16 itemst_cnt2;
    /* 0x10C */ SBTL_AISYO_TBL aisyo;
    /* 0x14C */ s16 field14C;
    /* 0x14E */ u8 pad14E[0x178 - 0x14E];
#else
    /* 0x90 */ s32 haveskill[10];
    /* 0xB8 */ u32 badStatus; /* sbtlGetBadStatus reads it */
    /* 0xBC */ s16 badst_cnt;  /* turns left on the current bad status */
    /* 0xBE */ s16 badst_cnt2; /* frame counter for the status's periodic effect */
    /* 0xC0 */ u32 koufunst; /* HD koufunst */
    /* 0xC4 */ s16 autost_cnt;
    /* 0xC6 */ s16 autost_cnt2; /* frames between auto skill heals */
    /* 0xC8 */ u32 itemstatus;
    /* 0xCC */ s16 itemst_cnt;
    /* 0xCE */ s16 itemst_cnt2;
    /* 0xD0 */ SBTL_AISYO_TBL aisyo;
#endif
} sdatUnitWork_t;

#ifdef VERSION_RAIDOU2
SIZE_CHECK(sdatUnitWorkSizeCheck, sdatUnitWork_t, 0x178);
#else
SIZE_CHECK(sdatUnitWorkSizeCheck, sdatUnitWork_t, 0x110);
#endif

typedef sdatUnitWork_t datPartyDevil_t;

#define SDAT_STOCK_NUM 12
#define SDAT_ITEM_NUM 160

#ifdef VERSION_RAIDOU2
/* Player data at DDS3_GBWK+0x1FA0. Partial raidou2 layout: it ends at the last stock devil, the rest is unread. */
typedef struct SBTL_PLAYER_DATA {
    /* 0x000 */ sdfVector4_t vec[2];
    /* 0x020 */ s32 mag;
    /* 0x024 */ s32 val24;
    /* 0x028 */ s32 maka;
    /* 0x02C */ s32 val2C;
    /* 0x030 */ u8 pad30[4];
    /* 0x034 */ sdatUnitWork_t unit;
    /* 0x1AC */ u8 pad1AC[0x2];
    /* 0x1AE */ s16 sword; /* datRefDDS3GBWK12 equips it */
    /* 0x1B0 */ u8 pad1B0[0x1B8 - 0x1B0];
    /* 0x1B8 */ s32 hgunnum;
    /* 0x1BC */ s32 bulletno;
    /* 0x1C0 */ u8 pad1C0[0x200 - 0x1C0];
    /* 0x200 */ s16 item[SDAT_ITEM_NUM]; /* stock per item id; R2's item count is not established */
    /* 0x340 */ u8 pad340[0x540 - 0x340];
    /* 0x540 */ sdatUnitWork_t stock[SDAT_STOCK_NUM];
} SBTL_PLAYER_DATA;

SIZE_CHECK(SBTL_PLAYER_DATASizeCheck, SBTL_PLAYER_DATA, 0x16E0);
#else
/* Player data, 0xFA0 bytes at DDS3_GBWK+0x1CA0. Only the fields used so far are named. */
typedef struct SBTL_PLAYER_DATA {
    /* 0x000 */ sdfVector4_t vec[2];
    /* 0x020 */ s32 mag;
    /* 0x024 */ s32 val24;
    /* 0x028 */ s32 maka;
    /* 0x02C */ s32 val2C;
    /* 0x030 */ u8 pad30[4];
    /* 0x034 */ sdatUnitWork_t unit;
    /* 0x144 */ u8 pad144[2];
    /* 0x146 */ s16 val146;
    /* 0x148 */ s16 swordAtk;
    /* 0x14A */ u8 pad14A[2];
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

#endif
