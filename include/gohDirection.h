#ifndef GOHDIRECTION_H
#define GOHDIRECTION_H

#include "common.h"
#include "kernel.h"

struct dds3ModelHandle_t;
struct gohData_t;

dds3ProcessFunc_t Direction2CalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t Direction2DrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohDrawProcess(dds3ProcessID_t *PID);
s32 gohEnd2Init(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohEndCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohEndDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohEndUpdateProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohEquipSwordCalcProcess(dds3ProcessID_t *PID);
s32 gohIsCombineFlag(void);
void gohModelFree(struct gohData_t *pData);
void gohModelLightSet(char *pUnitPbName, struct dds3ModelHandle_t *Model);
void gohModelLoad(struct gohData_t *pData);
s32 gohResultInit(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohResultUpdateProcess(dds3ProcessID_t *PID);
void gohSetModelScale(s32 DevilID, struct dds3ModelHandle_t *Model);
dds3ProcessFunc_t gohUpdateProcess(dds3ProcessID_t *PID);

#endif
