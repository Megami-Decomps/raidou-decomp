#ifndef SBTLTUBESUPPORT_H
#define SBTLTUBESUPPORT_H

#include "common.h"
#include "enums.h"

struct SBTLUNIT_WORK;

void nwdLeaf5(void);
s32 sbtlCallBtlGetSkilltbl16(struct SBTLUNIT_WORK *tgtwk, SKLID sklid);

#endif
