#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtSky.c */
#include "common.h"

#include "sdfFrame.h"

#include "kernel.h"

extern char evtSkyProcName[];

extern u16 skyanime;

extern u16 skyanow;

extern u16 skyamax;

extern s16 skyatarget;

extern s16 skyabase_par;

extern s32 fldbokasi;

extern s16 fldSkyGetAlphaOfs(void);

extern void fldSkySetAlphaOfs(s32 alp);

extern void fldSkySetAlpha(s32 alp);

extern void fldSetFilterAnime(s32 anime);

extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);

extern void fldSkyDrawFilter(void);

extern void fldDrawFrameSprite(void);

extern void rsetOtOfs(s32 otofs);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

void evtUpdateSkyAlpha(void);

s32 evtSkyProcess1(dds3ProcessID_t *id);

void evtSkyShutdown(dds3ProcessID_t *id);

void evtSetBokasi(s32 sw);

void evtSetSkyAlpha(s32 frame, s32 a);

u16 evtCheckSkyAlphaAnime(void);

void evtUpdateSkyAlpha(void);

s32 evtSkyProcess1(dds3ProcessID_t *id);

void evtSkyShutdown(dds3ProcessID_t *id);

void evtExitSky(void);

dds3ProcessID_t *evtInitSky(void);
/* End of declarations from src/smt3/event/evtSky.c */

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

INCLUDE_ASM(const s32, "event/evtSky", func_001BB7E8);

INCLUDE_ASM(const s32, "event/evtSky", func_001BB838);

void evtExitSky(void)
{
    dds3ProcessID_t *id;

    id = dds3SearchProcessName(evtSkyProcName);
    if (id != NULL) {
        dds3KillProcess(id, 1);
    }
}

INCLUDE_ASM(const s32, "event/evtSky", evtInitSky);
INCLUDE_SDATA(const s32, "event/evtSky", D_0046C810);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C820);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C828);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C830);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C838);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C840);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C848);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C850);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C858);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C860);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C868);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C870);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C878);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C880);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C888);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C890);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C898);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C8A0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C8A8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C8B0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C8B8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C8C0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C8C8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0046C8D0);

