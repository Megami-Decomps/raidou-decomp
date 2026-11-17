#ifndef CODE_002929C8_H
#define CODE_002929C8_H

#include "common.h"
#include "sBtlUnit.h"

struct SBTL_PANEL;

/* One summon-menu step (HD SummonMenuFuncDelegate); takes &SBTL_WORK.mode on the PS2. */
typedef s32 (*sbtlSummonMenuFunc_t)(struct SBTLUNIT_WORK *wk, s16 *mode);

/* Step tables per SBTL_WORK.field436 (HD menutype) 1..7, indexed by field440 (HD sgstat[0]). */
extern sbtlSummonMenuFunc_t sbtlRefSbtl51Data[];
extern sbtlSummonMenuFunc_t sbtlRefSbtl51Data2[];
extern sbtlSummonMenuFunc_t sbtlRefSbtl51Data3[];
extern sbtlSummonMenuFunc_t sbtlRefSbtl51Data4[];
extern sbtlSummonMenuFunc_t sbtlRefSbtl51Data5[];
extern sbtlSummonMenuFunc_t sbtlRefSbtl51Data6[];
extern sbtlSummonMenuFunc_t sbtlRefSbtl51Data7[];

s32 sbtlLeaf77(struct SBTL_PANEL *panel);
s32 sbtlLeaf78(void);
void sbtlSummonMenuProc(struct SBTLUNIT_WORK *wk);
u32 sbtlCallRefSbtl51(void);

#endif
