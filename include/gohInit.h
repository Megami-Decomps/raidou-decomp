#ifndef GOHINIT_H
#define GOHINIT_H

#include "common.h"
#include "kernel.h"

struct gohData_t;

dds3ProcessFunc_t Destroy_gohInit(dds3ProcessID_t *PID);
void ModelFree(struct gohData_t *pData);
s32 ModelLoad_gohInit(struct gohData_t *pData);
dds3ProcessFunc_t TalkSelDrawProcess(dds3ProcessID_t *PID);
void campGohmadenCalc(void);
void campKillProcesses(void);
s32 gohBgmLoad(struct gohData_t *pData);
s32 gohBgmPlay(struct gohData_t *pData);
s32 gohBgmStop(struct gohData_t *pData);
void gohCallMemFreeDirect2(struct gohData_t *_data);
s32 gohCheckLoadBgModel(struct gohData_t *pData);
s32 gohCheckModelLoad(s32 ModelTableIndex);
void gohFreeBgModel(struct gohData_t *pData);
dds3ProcessFunc_t gohLoad2ndProcess(dds3ProcessID_t *PID);
s32 gohLoadBgModel(dds3ProcessID_t *PID, struct gohData_t *pData);
s32 gohScrCmd_PresentItem(void);
void gohStartEtcProcess(dds3ProcessID_t *PID);
s32 jmsGetCursorPosID(void);
s32 gohProcessCheck(void);
void gohFunc(void);

extern const char cmpCalcProcName2[];
extern const char campDrawProcName2[];
extern const char campUpdateProcName2[];
void gohShopDebug(void);

#endif
