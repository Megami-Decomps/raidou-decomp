#ifndef RSTRESULT_H
#define RSTRESULT_H

#include "common.h"
#include "kernel.h"

s32 rstBtlEndWaitInit(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstEndUpdateProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstLvUpWaitCalcProcess(dds3ProcessID_t *PID);

#endif
