#ifndef CODE_003648C0_H
#define CODE_003648C0_H

#include "common.h"
#include "model.h"
#include "evtUnitManager.h"

struct tagCMP_WORK;
extern struct mdlObj_t *cmpDrawStatusModel_cmpModelData;
s32 cmpMenuFilter(struct tagCMP_WORK *wk);
void mapFunc5(void);
void mapFunc6(void);
void cmpStockListMake(dds3ProcessID_t *PID);
dds3ProcessID_t *cmpStockListProcessStart(void);
extern const char ProcessStart_cmpStatusStrCampStatusMain[];
extern const char ProcessStart_cmpStatusStrCampStatusDraw[];
s32 cmpCallGetProcessData11(dds3ProcessID_t *id);
s32 cmpCallGetProcessData12(dds3ProcessID_t *id);

#endif
