#ifndef JMSMAIN_H
#define JMSMAIN_H

#include "common.h"
#include "kernel.h"

dds3ProcessFunc_t EndProcess_gohMain(dds3ProcessID_t *PID);
s32 ScrControl(dds3ProcessID_t *PID);

#endif
