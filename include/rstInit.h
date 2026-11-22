#ifndef RSTINIT_H
#define RSTINIT_H

#include "common.h"
#include "kernel.h"
#include "sdatUnit.h"
#include "code_0034F3B8.h"
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

/* One of rstCallPlaySE's four entries. Partial layout. */
typedef struct rstPlaySE_t {
    s32 unk00;
    u8 unk04[0x24 - 0x4];
} rstPlaySE_t;

/* Work of the rstInit process. Partial layout. */
typedef struct rstInitWork_t {
    sdfMemHandle_t *hMem; /* 0x00: the handle this work was locked from */
    u32 flags; /* 0x04: bit 1 starts the result screen */
    jmsSeqHandle_t panel;                                  /* 0x08 */
    s32 seqUnit;                                           /* 0x54 */
    rstWork_t reward;                                      /* 0x58 */
    rstWork_t reward2;                                     /* 0x188: copy of reward made by Init_rstInit */
    u8 unk2B8[4];
    s32 unk2BC;
    SBTL_PLAYER_DATA player;                               /* 0x2C0 */
    SBTL_PLAYER_DATA player1;                              /* 0x2C10 */
    SBTL_PLAYER_DATA player2;                              /* 0x5560: compared with DDS3_GBWK by rstCalcProcess */
    rstGift_t gift[20];                                    /* 0x7EB0 */
    s32 rankUp;                                            /* 0x7FA0 */
    void *hResultSpr;                                      /* 0x7FA4 */
    void *hBgSpr;                                          /* 0x7FA8 */
    u8 unk7FAC[4];
    rstBG_t *bg;                                           /* 0x7FB0 */
    s32 aLevelUpAnimQue[5][8];                             /* 0x7FB4 */
    s32 fadeCount; /* 0x8054 */
    s32 unk8058;
    u8 unk805C[0x8068 - 0x805C];
    dds3ProcessID_t *countPID[3]; /* 0x8068: experience count-up processes */
    rstPlaySE_t playSE[4];        /* 0x8074 */
} rstInitWork_t;

SIZE_CHECK(rstInitWorkSizeCheck, rstInitWork_t, 0x8104);

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
void rstEndNext(rstInitWork_t *pData);
void rstPreCalcUnit(rstInitWork_t *pData);
s32 rstResultCyuuseiMaxAnim(s32 X, s32 Y, s32 AlphaRate, s32 *pQue, struct sprHandle_t *pSpr);
void rstCallBlendColor3(s32 X, s32 Y, s32 AlphaRate, rstInitWork_t *pData, struct sprHandle_t *pSpr, s32 StockIndex,
                        s32 Type);
/* Initial values of rstCallBlendColor3's locals: the experience gauge parts and the level up sprite scale. */
typedef struct rstExpGaugeParts_t {
    s32 parts[6];
} rstExpGaugeParts_t;
typedef struct rstAnimScale_t {
    f32 scale[2];
} rstAnimScale_t;
extern const rstExpGaugeParts_t rstResultNakamaLevelTable;
extern const rstAnimScale_t rstCallBlendColor3Data4[];
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
void Destroy_rstInit(dds3ProcessID_t *PID);
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
