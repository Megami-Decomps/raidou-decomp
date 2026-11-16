#ifndef SBTLSCRIPT_H
#define SBTLSCRIPT_H

#include "common.h"
#include "enums.h"

/* Effect placement entry; only the fields read so far are named. */
typedef struct EFFPDATA_WORK {
    u8 pad00[0x14];
    u8 iti;
    u8 pad15[0x3];
    s32 iti_no;
} EFFPDATA_WORK;
struct SBTLUNIT_WORK;

s32 GetUnitHelperMatrix(struct SBTLUNIT_WORK *wk, SBTLUNIT_MARK mark);
s32 Support_RunDownProcess(void);
s32 Support_RunWarpProcess(void);
s32 sbtlCheckEffectCancel(struct SBTLUNIT_WORK *wk, SKLID skillID, struct EFFPDATA_WORK *effpdwork);
s32 sbtlCheckEnshutuSummonUnit(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckSkillCancel(struct SBTLUNIT_WORK *wk, SKLID skillID);
s32 sbtlCreateAIScript(void);
s32 sbtlGetPointLast(struct SBTLUNIT_WORK *wk, SKLID skillID);
void sbtlStartDeadEvent(struct SBTLUNIT_WORK *wk);

#endif
