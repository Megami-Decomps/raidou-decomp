/* Declarations of smt3-decomp src/smt3/script/scrCommonCommand.c, for its bodies in this directory. */
#ifndef SHARED_SCRIPT_SCRCOMMONCOMMAND_H
#define SHARED_SCRIPT_SCRCOMMONCOMMAND_H

#include "common.h"

#include "sdk_libgcc.h"

#include "kernel.h"

#include "scr.h"

#include "ps2asm.h"

#include "sdfFrame.h"

static inline f32 scrFabsf(f32 x)
{
    f32 r;
    r = sdfFabs(x);
    return r;
}

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    s32 w;
} scrVecW_t;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} scrVec4_t;

extern s8 dds3PadBitTable[];

extern char scrRunNoProcessMsg[];

#include "sdk_libc.h"

extern void datDebugPrintf(const char *fmt, ...);

extern s32 scrGetIntPara(s32 n);

extern f32 scrGetFloatPara(s32 n);

extern char *scrGetStrPara(s32 n);

extern void scrSetIntReturnValue(s32 i);

extern s32 scrGetCommandTimer(void);

extern s32 scrGetLabelCodeAddress(s32 labelindex);

extern void scrSetCodePointer(s32 cp);

extern scrProcessWork_s *scrGetScriptHandle(void);

extern dds3ProcessID_t *scrRunScriptEx(s32 priority, void *header, void *typeTable, void *procTable,
                          void *labelTable, void *code, void *messageData, void *stringData,
                          s32 startProc);

extern s32 evtBitCheck(s32 no);

extern void evtBitOn(s32 no);

extern void evtBitOff(s32 no);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern void dds3PadVibrationSet(s32 port, s32 motor, s32 level);

extern void dds3SetScreenFovy(f32 fovy);

extern s32 dds3ExistProcess(dds3ProcessID_t *id);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern u32 dds3GetProcessRunTime(dds3ProcessID_t *id);

extern void dds3SetQuake(s32 frame, s32 range);

extern void dds3StopQuake(void);

extern void dds3SetLightColor(s32 frame, s32 no, void *col);

extern void dds3SetLightDir(s32 frame, s32 no, void *dir);

extern void dds3SetAmbientColor(s32 frame, void *col);

extern void dds3SetFogColor(s32 frame, f32 *col);

extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);

extern void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias);

extern void dds3SetBlur(s32 frame, s32 alpha, s32 scale);

extern void dds3SetMultiBlurPara(f32 rotate, f32 scale, u32 blend);

extern void dds3SetMultiBlurRGBA(s32 rgba);

extern void dds3SetMultiBlurArea(s32 area, s32 posx, s32 posy);

extern void dds3MultiBlurSwap(s32 frame);

extern void dds3MultiBlurOn(s32 frame);

extern void dds3MultiBlurOff(s32 frame);

extern void dds3SetDistBlurPara(s32 number, s32 width, s32 intval, f32 speed, f32 rotate, f32 distor,
                                s32 blend);

extern void dds3SetDistBlurRGBA(s32 rgba);

extern void dds3SetDistBlurArea(s32 area, s32 posx, s32 posy);

extern void dds3DistBlurSwap(s32 frame);

extern void dds3DistBlurOn(s32 frame);

extern void dds3DistBlurOff(s32 frame);

extern void dds3SetRipBlurPara(s32 number, f32 speed, f32 intval, f32 rotate, f32 distor, f32 distor_s,
                               s32 blend);

extern void dds3SetRipBlurRGBA(s32 rgba);

extern void dds3SetRipBlurArea(s32 area, s32 posx, s32 posy);

extern void dds3RipBlurSwap(s32 frame);

extern void dds3RipBlurOn(s32 frame);

extern void dds3RipBlurOff(s32 frame);

extern void dds3SetBlur2Para(f32 rotate, f32 scale, u32 blend);

extern void dds3SetBlur2RGBA(s32 rgba);

extern void dds3SetBlur2Area(s32 posx, s32 posy);

extern void dds3Blur2Swap(s32 frame);

extern void dds3Blur2On(s32 frame);

extern void dds3Blur2Off(s32 frame);

extern void dds3SetFilter2Para(s32 blend);

extern void dds3SetFilter2RGBA(s32 rgba);

extern void dds3Filter2Swap(s32 frame);

extern void dds3Filter2On(s32 frame);

extern void dds3Filter2Off(s32 frame);

extern void dds3SetMFilterPara(s32 blend);

extern void dds3SetMFilterRGBA(s32 rgba);

extern void dds3SetMFilterArea(s32 area, s32 posx, s32 posy);

extern void dds3MFilterSwap(s32 frame);

extern void dds3MFilterOn(s32 frame);

extern void dds3MFilterOff(s32 frame);

s32 scrCommand_KEY(void);

s32 scrCommand_BIT_CHK(void);

s32 scrCommand_BIT_ON(void);

s32 scrCommand_BIT_OFF(void);

s32 scrCommand_RND(void);

s32 scrCommand_SYNC(void);

s32 scrCommand_WAIT(void);

s32 scrCommand_PUT(void);

s32 scrCommand_PUTS(void);

s32 scrCommand_FADE_IN(void);

s32 scrCommand_FADE_OUT(void);

s32 scrCommand_SEL_JMP(void);

s32 scrCommand_PAD_TRIG(void);

s32 scrCommand_PAD_PRESS(void);

s32 scrCommand_PAD_ACT(void);

s32 scrCommand_SET_FOVY(void);

s32 scrCommand_SCR_RUN(void);

s32 scrCommand_SCR_KILL(void);

s32 scrCommand_SCR_WAIT(void);

s32 scrCommand_SCR_STAT(void);

s32 scrCommand_SCR_TIME(void);

s32 scrCommand_QUAKE_ON(void);

s32 scrCommand_QUAKE_OFF(void);

s32 scrCommand_SCENE_LIGHT_RGB(void);

s32 scrCommand_SCENE_LIGHT_DIR(void);

s32 scrCommand_SCENE_AMBIENT(void);

s32 scrCommand_SCENE_FOG_RGB(void);

s32 scrCommand_SCENE_FOG_PARA(void);

s32 scrCommand_SOFTEN_BIAS(void);

s32 scrCommand_BLUR(void);

s32 scrCommand_MUL_BLUR_PARA(void);

s32 scrCommand_MUL_BLUR_RGBA(void);

s32 scrCommand_MUL_BLUR_AREA(void);

s32 scrCommand_MUL_BLUR_SWAP(void);

s32 scrCommand_MUL_BLUR_ON(void);

s32 scrCommand_MUL_BLUR_OFF(void);

s32 scrCommand_DIST_BLUR_PARA(void);

s32 scrCommand_DIST_BLUR_RGBA(void);

s32 scrCommand_DIST_BLUR_AREA(void);

s32 scrCommand_DIST_BLUR_SWAP(void);

s32 scrCommand_DIST_BLUR_ON(void);

s32 scrCommand_DIST_BLUR_OFF(void);

s32 scrCommand_RIP_BLUR_PARA(void);

s32 scrCommand_RIP_BLUR_RGBA(void);

s32 scrCommand_RIP_BLUR_AREA(void);

s32 scrCommand_RIP_BLUR_SWAP(void);

s32 scrCommand_RIP_BLUR_ON(void);

s32 scrCommand_RIP_BLUR_OFF(void);

s32 scrCommand_BLUR2_PARA(void);

s32 scrCommand_BLUR2_RGBA(void);

s32 scrCommand_BLUR2_AREA(void);

s32 scrCommand_BLUR2_SWAP(void);

s32 scrCommand_BLUR2_ON(void);

s32 scrCommand_BLUR2_OFF(void);

s32 scrCommand_FILTER2_PARA(void);

s32 scrCommand_FILTER2_RGBA(void);

s32 scrCommand_FILTER2_SWAP(void);

s32 scrCommand_FILTER2_ON(void);

s32 scrCommand_FILTER2_OFF(void);

s32 scrCommand_MFILTER_PARA(void);

s32 scrCommand_MFILTER_RGBA(void);

s32 scrCommand_MFILTER_AREA(void);

s32 scrCommand_MFILTER_SWAP(void);

s32 scrCommand_MFILTER_ON(void);

s32 scrCommand_MFILTER_OFF(void);

s32 scrCommand_BLUR_ALLOFF(void);

s32 scrCommand_FILTER_ALLOFF(void);

#endif
