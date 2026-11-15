#include "common.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"

extern const char evtFrameVarName[];

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

s32 FrameVarProcess(dds3ProcessID_t *id);
dds3ProcessID_t *evtInitFrameVar(void);
extern const char evtFrameVarName[24];
extern const char evtFrameHeadSoftenBias[16];

dds3ProcessID_t *evtInitFrameVar(void)
{
    return dds3StartProcess(evtFrameVarName, 0x2AF9, 1, 1, FrameVarProcess, NULL, NULL);
}

const char evtFrameVarName[24] = "FrameVar";

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", evtFrameHeadSoftenBias);

