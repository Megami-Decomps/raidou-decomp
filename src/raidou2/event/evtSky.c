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

extern u16 skyanime;

extern s32 fldbokasi;

extern char evtSkyProcName[8];

extern char evtFrameNumFmt[8];

extern char D_0054F918[8];

extern char D_0054F920[8];

extern char D_0054F928[8];

extern char D_0054F930[8];

extern char D_0054F938[8];

extern char D_0054F940[8];

extern char D_0054F948[8];

extern char D_0054F950[8];

extern char D_0054F958[8];

extern char D_0054F960[8];

extern char D_0054F968[8];

extern char D_0054F970[8];

extern char D_0054F978[8];

extern char D_0054F980[8];

extern char D_0054F988[8];

extern char D_0054F990[8];

extern s32 evtMenuDisp_cnt;

extern char evtScrollUpMark[8];

extern char evtScrollDownMark[8];

extern char evtFmtIndentInt[8];

extern char evtFmtStr[8];

extern char evtFmtSpaceStr[8];

extern char evtFmtNameExt[8];

extern char evtFmtChar[8];

extern char evtLabelEvent[8];

extern char evtFmtEventNo[8];

extern char evtLabelCut[8];

extern char evtFmtCutNo[8];

extern char evtLabelName[8];

extern char D_0054FA00[8];

extern char D_0054FA08[8];

extern char D_0054FA10[8];

extern char D_0054FA18[8];

extern char D_0054FA20[8];

extern char D_0054FA28[8];

extern char D_0054FA30[8];

extern char D_0054FA38[8];

extern char evtFmtInt4[8];

extern char evtStrEllipsis[8];

extern char evtStrOn[8];

extern char evtStrOff[8];

extern char evtBlendNameNormal[8];

extern char evtBlendNameAdd[8];

extern char evtBlendNameSub[8];

extern char evtFmtFloat1[8];

extern char evtFmtInt[8];

extern char evtFmtName11[8];

extern char evtFmtPlayerNo[8];

extern char evtFmtIndentStr[8];

extern char evtMsgSaveOpenError[8];

extern char evtPfsDevice[8];

extern char evtPackNameFormat[8];
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

INCLUDE_ASM(const s32, "event/evtSky", func_001D18F0);

INCLUDE_ASM(const s32, "event/evtSky", func_001D1940);

void evtExitSky(void)
{
    dds3ProcessID_t *id;

    id = dds3SearchProcessName(evtSkyProcName);
    if (id != NULL) {
        dds3KillProcess(id, 1);
    }
}

INCLUDE_ASM(const s32, "event/evtSky", evtInitSky);
INCLUDE_SDATA(const s32, "event/evtSky", D_004DFA80);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFA90);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFA98);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAA0);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAA8);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAB0);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAB8);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAC0);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAC8);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAD0);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAD8);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAE0);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAE8);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAF0);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFAF8);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB00);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB08);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB10);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB18);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB20);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB28);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB30);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB38);

INCLUDE_SDATA(const s32, "event/evtSky", D_004DFB40);

