#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtDebug.c */
#include "common.h"

#include "scr.h"

#include "basic.h"

#include "newdata.h"

#include "kernel.h"

#include "libsdf.h"

typedef void (*evtOtRegFunc_t)(sdfGrOt_t *, sdfDrawTag_t *);

extern f32 gMoonCnt;

extern dds3ProcessID_t *moonprocess_id;

extern s32 evtDebugState;

extern s32 evtSceneDispWork;

extern u32 dds3BackColor;

extern u8 dds3PadButtonState[];

extern sdfGrOt_t sdfGrOtTable[];

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *, s32, s32, s32, s32);

extern void *sdfDevPrintf(u32 aX, u32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

#include "sdk_libc.h"

extern s32 sdfFileCheckExist(char *aFilename);

extern void scrAllReleaseScript(void);

extern void evtFreeStage(void);

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 start, s32 shutdown,
                                         dds3ProcessFunc_t process, dds3ProcessFunc_t finish, void *data);

extern s32 dds3KillProcessName(char *name, s32 shutdown);

extern void evtBitAllOff(void);

extern void evtDebugPrintCamera(void);

saddr_t EventDebugProcess1(dds3ProcessID_t *id);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *scene);

extern dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *scene, dds3Basic_t *camera);

extern f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *camera);

extern void dds3SetScreenFovy(f32 fovy);

extern void *dds3FontReqAllocDrawTag(void);

extern const char evtDebugCameraNameFmt[];

extern const char *evtDebugDefaultCameraName;

extern u32 evtDebugCameraPage;

extern sdfGrOt_t sdfDebugGrOt;

void evtDebugPrintCamera(void);

/* After launching a test script, keep the test task idle and return 0. */
s32 evtDebugCameraStep(dds3ProcessID_t *id);

/* Return the same update callback installed by evtInitEventTest. */
void *evtGetDebugProcess(void);

saddr_t EventDebugProcess1(dds3ProcessID_t *id);

/* Test-task teardown destroys the named script processes. */
void EventDebugShutdown(dds3ProcessID_t *id);

void evtInitEventTest(void);

void evtChkEndProcess(void);

extern const char evtSceneDispNameCamera[16];

extern const char evtSceneDispNameMatter[16];

extern const char evtSceneDispNameEffect[16];

extern const char evtSceneDispNameStatic[16];

void func_001EBC90(void);
/* End of declarations from src/smt3/event/evtDebug.c */

INCLUDE_ASM(const s32, "event/evtDebug", func_r1_001AEBC0);

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

INCLUDE_RODATA(const s32, "event/evtDebug", D_r1_0044E9D8);

INCLUDE_RODATA(const s32, "event/evtDebug", D_r1_0044E9E8);

INCLUDE_RODATA(const s32, "event/evtDebug", D_r1_0044E9F8);

INCLUDE_RODATA(const s32, "event/evtDebug", D_r1_0044EA08);

INCLUDE_RODATA(const s32, "event/evtDebug", D_r1_0044EA18);

INCLUDE_ASM(const s32, "event/evtDebug", func_r1_001AEC60);
INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C628);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C62C);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C630);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C634);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C638);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C640);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C648);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C650);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C658);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C660);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C668);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C670);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C678);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C680);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C688);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C690);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C698);

INCLUDE_SDATA(const s32, "event/evtDebug", D_r1_0046C6A0);

