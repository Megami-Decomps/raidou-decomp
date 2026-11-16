#include "common.h"
#include "sdfFrame.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "evtSky.h"
#include "dds3KernelDraw.h"
#include "dds3KernelMain.h"
#include "fldDraw.h"

extern char evtSkyProcName[];
extern u16 skyanime;
extern u16 skyanow;
extern u16 skyamax;
extern s16 skyatarget;
extern s16 skyabase_par;
extern s16 fldSkyGetAlphaOfs(void);
extern void fldSkySetAlphaOfs(s32 alp);
void evtUpdateSkyAlpha(void);
void evtSetSkyAlpha(s32 frame, s32 a);
u16 evtCheckSkyAlphaAnime(void);
void evtExitSky(void);
extern char evtSkyProcName[8];

void evtSetSkyAlpha(s32 frame, s32 a)
{
    s16 current;

    current = fldSkyGetAlphaOfs();
    if (current != a) {
        if (frame == 0) {
            fldSkySetAlphaOfs(a);
            skyanime = 0;
        } else {
            skyamax = SDF_FRAMES(frame);
            skyatarget = current;
            skyabase_par = a;
            skyanime = 1;
            skyanow = 0;
        }
    }
}

u16 evtCheckSkyAlphaAnime(void)
{
    return skyanime;
}

void evtUpdateSkyAlpha(void)
{
    if (skyanime != 0) {
        skyanow++;
        fldSkySetAlphaOfs(skyatarget +
                          (s32)((f32)(skyabase_par - skyatarget) * ((f32)skyanow / (f32)skyamax)));
        if (skyanow >= skyamax) {
            skyanime = 0;
        }
    }
}

s32 evtSkyProcess1(dds3ProcessID_t *id)
{
    evtUpdateSkyAlpha();
    if (fldbokasi != 0) {
#ifdef VERSION_RAIDOU1
        rsetOtOfs(103);
#else
        rsetOtOfs(104);
#endif
        if (dds3BgSetScrollSub() == 0) {
            fldDrawFrameSprite();
        }
    }
    return 0;
}

void evtSkyShutdown(dds3ProcessID_t *id)
{
    skyanime = 0;
    fldbokasi = 0;
    dds3FilterSet(0, 1, 0);
}

void evtExitSky(void)
{
    dds3ProcessID_t *id;

    id = dds3SearchProcessName(evtSkyProcName);
    if (id != NULL) {
        dds3KillProcess(id, 1);
    }
}

dds3ProcessID_t *evtInitSky(void)
{
    fldSetFilterAnime(0);
    dds3FilterSetAlpha(0x80);
    dds3FilterSetAlphaOfs(0);
    dds3FilterSet(0, 1, 0);
    return dds3StartProcess(evtSkyProcName, 0x2B0C, 1, 1, evtSkyProcess1, (dds3ProcessFunc_t)evtSkyShutdown, NULL);
}
INCLUDE_SDATA(const s32, "event/evtSky", evtSkyProcName);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry2);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry3);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry4);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry5);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry6);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry7);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry8);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry9);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry10);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry11);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry12);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry13);

INCLUDE_SDATA(const s32, "event/evtSky", evtGROUPMAXData2Entry);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry14);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry15);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry16);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry17);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry18);

INCLUDE_SDATA(const s32, "event/evtSky", evtViewerDrawShadowConfigRowDataEntry);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry19);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry20);

INCLUDE_SDATA(const s32, "event/evtSky", evtFrameListHeadDataEntry21);

