#ifndef TITLEMAIN_H
#define TITLEMAIN_H

#include "common.h"
#include "kernel.h"

struct titleData_t;

dds3ProcessFunc_t TitleAdvertiseCalcProcess(dds3ProcessID_t *PID);
s32 TitleAdvertiseInit(dds3ProcessID_t *PID);
dds3ProcessFunc_t TitleAtlusLogoCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t TitleAtlusLogoDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t TitleConfigCalcProcess(dds3ProcessID_t *PID);
s32 TitleConfigDestroy(dds3ProcessID_t *PID);
s32 TitleDLCInit(dds3ProcessID_t *PID);
void TitleDestroy(dds3ProcessID_t *PID);
dds3ProcessFunc_t TitleEndWaitDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t TitleFirstCalcProcess3(dds3ProcessID_t *PID);
void TitleGoMain(struct titleData_t *pData);
s32 TitleProcessCheck(void);
void TitleProcessEnd(void);
void fldCallElvReset(s32 iMode);
dds3ProcessFunc_t rstCallGetProcessData32(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstCallGetProcessData33(dds3ProcessID_t *PID);
s32 rstCallGetProcessData34(dds3ProcessID_t *PID);

#endif
