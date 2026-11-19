#ifndef RSTINIT_H
#define RSTINIT_H

#include "common.h"
#include "kernel.h"
#include "sdatUnit.h"
#include "code_r1_001CC1D0.h"

struct sprHandle_t;
struct gohData_t;

/* The result screen background: one sprite resource set. */
typedef struct rstBG_t {
    struct sprHandle_t *pSpr;
    u32 unk4;
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
    u8 unk58[0x2C0 - 0x58];
    SBTL_PLAYER_DATA player;                               /* 0x2C0 */
    u8 pad19A0[0x5560 - 0x2C0 - sizeof(SBTL_PLAYER_DATA)];
    SBTL_PLAYER_DATA player2;                              /* 0x5560: compared with DDS3_GBWK by rstCalcProcess */
    u8 pad7800[0x7EB0 - 0x5560 - sizeof(SBTL_PLAYER_DATA)];
    rstGift_t gift[20];                                    /* 0x7EB0 */
    s32 rankUp;                                            /* 0x7FA0 */
} rstInitWork_t;

extern dds3ProcessID_t *ProcessStart_rstInitData;
extern jmsSeqUnit_t rstEndCalcProcessData;
s32 rstCallGetGlobalEncountNo(rstInitWork_t *work);
SBTL_PLAYER_DATA *rstGetPlayerUnit(void);
#endif
/* Nonzero when the return to the map cross-fades. */
extern s32 rstData5;

rstBG_t *InitBgCore(struct sprHandle_t *pSpr);
rstBG_t *InitBG(void);
void rstCallCalcAlphaRate(rstBG_t *bg, s32 alpha, s32 ot);
void rstDestroyBG(rstBG_t *bg);
s32 ProcessCheck_rstInit(void);
void ProcessEnd_rstInit(void);
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

#endif
