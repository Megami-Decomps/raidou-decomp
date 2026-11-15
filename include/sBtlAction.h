#ifndef SBTLACTION_H
#define SBTLACTION_H

#include "common.h"

struct SBTLUNIT_WORK;

s32 Proc_sbtlAction(struct SBTLUNIT_WORK *wk);
s32 sbtlSetHangekiFlag(struct SBTLUNIT_WORK *wk);

#endif
