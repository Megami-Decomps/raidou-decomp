#ifndef RSTINIT_H
#define RSTINIT_H

#include "common.h"
#include "kernel.h"

void Destroy_rstInit(dds3ProcessID_t *PID);
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

#endif
