#ifndef AW_XATKPAD_H
#define AW_XATKPAD_H

#include "common.h"

struct AW_WORK_t;

s32 sbtlResetColor(u32 color);
#ifdef VERSION_RAIDOU2
void cmpSetColor(s32 rgba);
s32 sbtlResetColorSub(u32 color);
#endif
void sbtlResetNSTOP(void);

#endif
