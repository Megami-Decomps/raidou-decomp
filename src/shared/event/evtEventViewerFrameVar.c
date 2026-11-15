#include "common.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"
#include "dds3KernelCore.h"

extern const char evtFrameVarName[];


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

