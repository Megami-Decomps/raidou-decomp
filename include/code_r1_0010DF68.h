#ifndef CODE_R1_0010DF68_H
#define CODE_R1_0010DF68_H

#include "common.h"

struct dds3ProcessID_t;

void datDebugPrintf(const char *fmt, ...);
void datDEBUGCONSOLE(const char *fmt, ...);
s32 evtDrawResourceCountDebugTask(struct dds3ProcessID_t *task);
extern struct dds3ProcessID_t *evtToggleDebugTimeGraphTaskWork;
s32 getLmpMainProc(s32 mode);
void mjnTestFontCalc(void);

#endif
