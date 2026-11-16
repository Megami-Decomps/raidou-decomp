#ifndef RSTINIT_H
#define RSTINIT_H

#include "common.h"
#include "kernel.h"

struct sprHandle_t;

/* The result screen background: one sprite resource set. */
typedef struct rstBG_t {
    struct sprHandle_t *pSpr;
    u32 unk4;
} rstBG_t;

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
s32 rstMapReturnChkXFade(void *pData);
s32 rstResultFadeCheck(void);
dds3ProcessFunc_t rstResultLoadWaitProcess(dds3ProcessID_t *PID);
void rstResultStart(void);
void rstSetEncountDlcID(s32 EncountNo);
void rstSetFlag(s32 Flag);

extern const char rstStrResult[];
extern const char rstStrRsHaikeiSpr[];

#endif
