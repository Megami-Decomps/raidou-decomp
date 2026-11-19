#ifndef CODE_0038AD80_H
#define CODE_0038AD80_H

#include "common.h"
#include "enums.h"

struct tagCMP_WORK;
void mapAutoMapDraw3(void);
s32 cmpSetupCmpItemSub(CMP_SPR a_no);
void End_cmpItem(void);
struct tagCMP_WORK *cmpGetStockPID(void);
dds3ProcessID_t *cmpPartyDrawFuncSub(void);
extern const char ProcessStart_cmpMapStrCampAutomapMain[];
extern const char ProcessStart_cmpMapStrCampAutomapDraw[];

#endif
