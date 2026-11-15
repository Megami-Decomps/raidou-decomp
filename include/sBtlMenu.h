#ifndef SBTLMENU_H
#define SBTLMENU_H

#include "common.h"
#include "kernel.h"

struct SBTLUNIT_WORK;

void ProcessStart_sbtlMenu(void);
s32 cmpUpdateSword(void);
struct SBTLUNIT_WORK *fldCallMenuProcessCheck(void);
s32 sbtlCallMemFreeDelayedDirect(void);
void sbtlCampSubSpr(void);
void sbtlCmndCntST(void);
dds3ProcessFunc_t sbtlDrawProcess(dds3ProcessID_t *PID);
s32 sbtlFuncKatana(void);
s32 sbtlIsMenuStartScript(void);
void sbtlSetMenuImage(dds3ProcessID_t *PID);

#endif
