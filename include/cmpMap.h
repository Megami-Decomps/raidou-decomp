#ifndef CMPMAP_H
#define CMPMAP_H

#include "common.h"
#include "kernel.h"

s32 DrawProcess(dds3ProcessID_t *PID);
s32 ProcessCheck_cmpMap(void);
void ProcessEnd_cmpMap(void);
s32 Process_cmpMap(dds3ProcessID_t *PID);

#endif
