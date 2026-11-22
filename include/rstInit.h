#ifndef RSTINIT_H
#define RSTINIT_H

#include "common.h"
#include "kernel.h"
#include "sdatUnit.h"
#include "code_r1_001CC1D0.h"

struct sprHandle_t;
struct gohData_t;
struct rstWork_t;

/* The result screen background: one sprite resource set. */
typedef struct rstBG_t {
    struct sprHandle_t *pSpr;
    s32 frame; /* blink frame, wraps after the last key */
} rstBG_t;

#ifdef VERSION_RAIDOU2
/* The item a stock devil gives on the result screen. */
typedef struct rstGift_t {
    s32 item;
    s32 nums;
    s32 unk08;
} rstGift_t;

/* Work of the rstInit process. Partial layout. */
typedef struct rstInitWork_t {
    u32 unk00;
    u32 flags; /* 0x04: bit 1 starts the result screen */
    jmsSeqHandle_t panel;                                  /* 0x08 */
    s32 seqUnit;                                           /* 0x54 */
    u8 unk58[0xC0 - 0x58];
    s32 exp;                                               /* 0xC0: Raidou's reward */
    s32 exCyuusei;                                         /* 0xC4: loyalty reward, 0 for none */
    s32 stockExp[20];                                      /* 0xC8 */
    s32 stockCyuusei[20];                                  /* 0x118 */
    u8 unk168[0x178 - 0x168];
    u32 unk178;                                            /* 0x178: bit 0 plays the level up sound */
    u8 unk17C[0x2C0 - 0x17C];
    SBTL_PLAYER_DATA player;                               /* 0x2C0 */
    u8 pad19A0[0x2566 - 0x2C0 - sizeof(SBTL_PLAYER_DATA)];
    u8 stockMax;                                           /* 0x2566: copy of DDS3_GBWK+0x4246 */
    u8 pad2567[0x2C58 - 0x2567];
    s32 playerExp;                                         /* 0x2C58: Raidou's experience after the battle */
    u8 pad2C5C[0x5560 - 0x2C5C];
    SBTL_PLAYER_DATA player2;                              /* 0x5560: compared with DDS3_GBWK by rstCalcProcess */
    u8 pad7800[0x7EB0 - 0x5560 - sizeof(SBTL_PLAYER_DATA)];
    rstGift_t gift[20];                                    /* 0x7EB0 */
    s32 rankUp;                                            /* 0x7FA0 */
    void *hResultSpr;                                      /* 0x7FA4 */
    void *hBgSpr;                                          /* 0x7FA8 */
    u8 unk7FAC[4];
    rstBG_t *bg;                                           /* 0x7FB0 */
    s32 aLevelUpAnimQue[(0x8054 - 0x7FB4) / 4];            /* 0x7FB4 */
    s32 fadeCount; /* 0x8054 */
    s32 unk8058;
    u8 unk805C[0x8068 - 0x805C];
    dds3ProcessID_t *countPID[3]; /* 0x8068: experience count-up processes */
} rstInitWork_t;

extern dds3ProcessID_t *ProcessStart_rstInitData;
extern jmsSeqUnit_t rstEndCalcProcessData;
extern jmsSeqUnit_t rstBtlEndWaitCalcProcessData;
s32 rstCallGetProcessStat(void);
dds3ProcessFunc_t rstLvUpWaitCalcProcess(dds3ProcessID_t *PID);
s32 rstCallGetGlobalEncountNo(rstInitWork_t *work);
/* HD rstRankUpKudaAddNumTbl: tubes added at each summoner rank. */
extern const s8 rstCalcRankUpTable[];
s32 rstCalcRankUp(rstInitWork_t *pData);
void rstDevilGiftCalc(rstInitWork_t *pData);
SBTL_PLAYER_DATA *rstGetPlayerUnit(void);
void rstResultExpGauge(rstInitWork_t *pData, s32 X, s32 Y, s32 AlphaRate, s32 CurrentExp, s32 CurrentLevelExp,
                       s32 NextLevelExp, s32 *pPartsList, struct sprHandle_t *pSpr);
void rstResultLevelUpAnim(s32 X, s32 Y, s32 AlphaRate, s32 Type, s32 *pQue, struct sprHandle_t *pSpr);
void rstCallBlendColor2(rstInitWork_t *pData, s32 X, s32 Y, s32 AlphaRate, s32 Rate, struct sprHandle_t *pSpr);
void rstResultPlayerLevel(s32 X, s32 Y, s32 AlphaRate, rstInitWork_t *pData, struct sprHandle_t *pSpr);
void rstCallPlaySE(rstInitWork_t *pData, s32 type, s32 x, s32 y, struct sprHandle_t *pSpr);
#endif
/* Nonzero when the return to the map cross-fades. */
extern s32 rstData5;
extern s32 rstData6;
extern s32 rstLoadMapResourceData;
extern s32 rstLoadMapResourceData2;

rstBG_t *InitBgCore(struct sprHandle_t *pSpr);
rstBG_t *InitBG(void);
void rstCallCalcAlphaRate(rstBG_t *bg, s32 alpha, s32 ot);
void rstDestroyBG(rstBG_t *bg);
s32 ProcessCheck_rstInit(void);
void ProcessEnd_rstInit(void);
struct rstInitWork_t *Init_rstInit(struct rstWork_t *src);
dds3ProcessFunc_t rstResultStartWaitProcess(dds3ProcessID_t *PID);
s32 Destroy_rstInit(dds3ProcessID_t *PID);
s32 rstProcessStartRstInitSub(void);
void mapRetuenFromNakamaDead(void);
dds3ProcessID_t *ProcessStart_rstInit(void);
s32 rstCheckFlag(s32 Flag);
s32 rstCheckRankUp(void);
s32 rstGetGiftItem(s32 StockIndex);
s32 rstGetGiftItemNums(s32 StockIndex);
s32 rstGetGlobalEncountNo(void);
s32 rstLoadMapResource(s32 RetType);
struct rstInitWork_t;
s32 rstMapReturnChkXFade(struct rstInitWork_t *pWork);
s32 rstResultFadeCheck(void);
dds3ProcessFunc_t rstResultLoadWaitProcess(dds3ProcessID_t *PID);
void rstResultStart(void);
void rstSetEncountDlcID(s32 EncountNo);
void rstSetFlag(s32 Flag);

extern const char rstStrResult[];
extern const char rstStrRsHaikeiSpr[];
extern const char rstStrResultSpr[];

#endif
