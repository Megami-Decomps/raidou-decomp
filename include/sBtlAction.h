#ifndef SBTLACTION_H
#define SBTLACTION_H

#include "common.h"

struct SBTLUNIT_WORK;
struct sdatUnitWork_t;

s32 Proc_sbtlAction(struct SBTLUNIT_WORK *wk);
void sbtlHuumaParamUp(struct sdatUnitWork_t *unit);

#endif
