#ifndef SBTLTUBESUPPORT_H
#define SBTLTUBESUPPORT_H

#include "common.h"
#include "enums.h"

struct SBTLUNIT_WORK;

s32 NWD_GET_INTIMATE_RANK(void);
s32 sbtlCallBtlGetSkilltbl16(struct SBTLUNIT_WORK *tgtwk, SKLID sklid);

#endif
