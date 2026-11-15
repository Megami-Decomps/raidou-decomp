#ifndef CMPMAP_H
#define CMPMAP_H

#include "common.h"
#include "kernel.h"

dds3ProcessFunc_t DrawProcess(dds3ProcessID_t *PID);
s32 ProcessCheck_cmpMap(void);
void ProcessEnd_cmpMap(void);
dds3ProcessFunc_t fldCallGetProcessData3(dds3ProcessID_t *PID);

#endif
