#ifndef RSTGROWTH_H
#define RSTGROWTH_H

#include "common.h"
#include "kernel.h"

struct tagRST_GROWTH_WORK;

void rstCallProcEnd(struct tagRST_GROWTH_WORK *wk);
dds3ProcessID_t *rstCmpItemGrowthStart(void);
dds3ProcessID_t *rstGetGiftMsgPID(void);
void rstProcessEnd(void);
s32 rstSetFlagPlayBgm(void);

#endif
