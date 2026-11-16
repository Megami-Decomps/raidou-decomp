#ifndef DDS3KERNELDRAW_H
#define DDS3KERNELDRAW_H

#include "common.h"

void dds3Blur2Off(s32 frame);
void dds3Blur2On(s32 frame);
void dds3Blur2Swap(s32 frame);
void dds3DistBlurOff(s32 frame);
void dds3DistBlurOn(s32 frame);
void dds3DistBlurSwap(s32 frame);
void dds3Filter2Off(s32 frame);
void dds3Filter2On(s32 frame);
void dds3Filter2Swap(s32 frame);
void dds3MFilterOff(s32 frame);
void dds3MFilterOn(s32 frame);
void dds3MFilterSwap(s32 frame);
void dds3MultiBlurOff(s32 frame);
void dds3MultiBlurOn(s32 frame);
void dds3MultiBlurSwap(s32 frame);
void dds3RipBlurOff(s32 frame);
void dds3RipBlurOn(s32 frame);
void dds3RipBlurSwap(s32 frame);
void dds3SetAmbientColor(s32 frame, f32 *col);
void dds3SetBlur(s32 frame, s32 alpha, s32 scale);
void dds3SetBlur2Area(s32 posx, s32 posy);
void dds3SetBlur2Para(f32 rotate, f32 scale, s32 blend);
void dds3SetBlur2RGBA(u32 rgba);
void dds3SetDistBlurArea(s32 area, s32 posx, s32 posy);
void dds3SetDistBlurPara(s32 number, s32 width, s32 intval, f32 rotate, f32 distor, f32 blend, s32 speed);
void dds3SetDistBlurRGBA(u32 rgba);
void dds3SetFilter2Para(s32 blend);
void dds3SetFilter2RGBA(u32 rgba);
void dds3SetFogColor(s32 frame, f32 *col);
void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);
void dds3SetLightColor(s32 frame, s32 no, f32 *col);
void dds3SetLightDir(s32 frame, s32 no, f32 *dir);
void dds3SetMFilterArea(s32 area, s32 posx, s32 posy);
void dds3SetMFilterPara(s32 blend);
void dds3SetMFilterRGBA(u32 rgba);
void dds3SetMultiBlurArea(s32 area, s32 posx, s32 posy);
void dds3SetMultiBlurPara(f32 rotate, f32 scale, s32 blend);
void dds3SetMultiBlurRGBA(u32 rgba);
void dds3SetQuake(s32 frame, s32 range);
void dds3SetRipBlurArea(s32 area, s32 posx, s32 posy);
void dds3SetRipBlurPara(s32 number, f32 intval, f32 rotate, f32 distor, f32 distor_s, f32 blend, s32 speed);
void dds3SetRipBlurRGBA(u32 rgba);
void dds3SetScreenFovy(f32 fovy);
void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias);
void dds3StopQuake(void);

#include "enums.h"

struct SDF_GS_TEST;

s32 dds3BistaCheck(void);
void dds3BistaClear(void);
void dds3BistaIn(u32 frame);
void dds3BistaOut(u32 frame);
void dds3BistaOutEx(u32 frame, u32 btm_h);
void dds3BistaUpdate(void);
void dds3ChangeBistaMode(BISTA_MODE bistamode);
void dds3DrawBista(void);
void dds3FilterCalc(void);
void dds3FilterClear(void);
void dds3FilterDraw(void);
s32 dds3FilterGetAlpha(void);
s32 dds3FilterGetAlphaOfs(void);
void dds3FilterSet(s32 idx, s32 mode, s32 hokan);
void dds3FilterSetAlpha(s32 alp);
void dds3FilterSetAlphaOfs(s32 alp);
void dds3InitDrawEnv(void);
void dds3RunQuake(void);
void eglLinkAlpha(s32 mode);
void eglSetOt(u32 ot);
void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
s32 sbtlResetJump(void);

extern u32 dds3BackColor;
extern u32 dds3DrawFlag;

extern s16 dds3DrawBistaData2;
extern s16 dds3Data33;
u32 dds3NullDrawProc(void);

struct sdfTexHandle_t *dds3GetRenderTexture(void);

#endif
