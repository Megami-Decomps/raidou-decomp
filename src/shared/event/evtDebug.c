#include "common.h"
#include "scr.h"
#include "basic.h"
#include "newdata.h"
#include "kernel.h"
#include "libsdf.h"
#include "evtStage.h"
#include "dds3KernelCore.h"
#include "evtDebug.h"

typedef void (*evtOtRegFunc_t)(sdfGrOt_t *, sdfDrawTag_t *);
extern s32 evtSceneDispWork;
extern u32 dds3BackColor;

#include "sdk_libc.h"



saddr_t EventDebugProcess1(dds3ProcessID_t *id);

/* Test-task teardown destroys the named script processes. */
void EventDebugShutdown(dds3ProcessID_t *id);
void evtInitEventTest(void);
void evtChkEndProcess(void);
extern const char evtSceneDispNameCamera[16];
extern const char evtSceneDispNameMatter[16];
extern const char evtSceneDispNameEffect[16];
extern const char evtSceneDispNameStatic[16];

INCLUDE_ASM(const s32, "event/evtDebug", evtDebugCameraStep);

void evtInitEventTest(void)
{
    dds3BackColor = 0x80000000;
    dds3StartProcess("EventTest", 0x2AF9, 1, 1,
                    (dds3ProcessFunc_t)EventDebugProcess1,
                    (dds3ProcessFunc_t)EventDebugShutdown, NULL);
}

void evtChkEndProcess(void)
{
    dds3KillProcessName("EventTest", 1);
    dds3KillProcessName("PolygonMovie", 0);
    evtFreeStage();
}

INCLUDE_RODATA(const s32, "event/evtDebug", evtSceneDispNameCamera);

INCLUDE_RODATA(const s32, "event/evtDebug", evtSceneDispNameMatter);

INCLUDE_RODATA(const s32, "event/evtDebug", evtSceneDispNameEffect);

INCLUDE_RODATA(const s32, "event/evtDebug", evtSceneDispNameStatic);

INCLUDE_RODATA(const s32, "event/evtDebug", evtSceneDispRows);

INCLUDE_ASM(const s32, "event/evtDebug", evtSceneDispProcess);
#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtDebug", evtDebugCameraStepData);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispWork);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispTimer);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispState);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry2);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry3);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry4);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry5);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry6);

INCLUDE_SDATA(const s32, "event/evtDebug", nbCameraStateNameEvent);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry7);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry8);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry9);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry10);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry11);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispCountFmt);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispSerialFmt);

#endif
#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "event/evtDebug", evtDebugCameraStepData);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispWork);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispTimer);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispState);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry2);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry3);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry4);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry5);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry6);

INCLUDE_SDATA(const s32, "event/evtDebug", nbCameraStateNameEvent);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry7);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry8);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry9);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry10);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispRowsEntry11);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispCountFmt);

INCLUDE_SDATA(const s32, "event/evtDebug", evtSceneDispSerialFmt);

#endif
