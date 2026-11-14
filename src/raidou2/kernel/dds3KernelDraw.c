#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3KernelDraw.c */
#include "common.h"

#include "draw.h"

#include "draw3d.h"

#include "kernel_draw.h"

#include "ps2asm.h"

#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

/* One A+D register command with its standard DMA/GIF header. */
typedef struct
{
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t write;
} dds3GsCommand_t;

/* Each GIF reglist loop contains an RGBA register followed by an XYZ register. */
typedef struct
{
    u64 low;
    u64 high;
} dds3GouraudRegister_t;

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 color;
} dds3PrintControl_t;

/* Two-context Z-clear packets are 0xB0 bytes, two buffers per state. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGifTag_t testGif;
    sdfGrTagStdReg_t test;
    sdfGifTag_t clearGif;
    sdfGrTagStdReg_t colorA;
    sdfGrTagStdReg_t positionA;
    sdfGrTagStdReg_t colorB;
    sdfGrTagStdReg_t positionB;
    sdfGifTag_t blendGif;
    sdfGrTagStdReg_t blend[2];
} dds3ZClearPacket_t;

extern u32 dds3DrawFlag;

extern u8 fadergba[4];

/* The fade sprite packet: a four-register state block then a textureless
 * sprite, 0xB0 bytes, one per frame buffer. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGifTag_t stateGif;
    sdfGrTagStdReg_t state[4];
    sdfGifTag_t spriteGif;
    sdfGrTagStdReg_t sprite[4];
} dds3FadePacket_t;

extern dds3FadePacket_t dds3FadePacket[2];

extern sdfFrameU16_t fadenow;

extern sdfFrameU16_t fademax;

typedef struct dds3Filter2Param {
    /* 0x00 */ union { u8 rgba[4]; u32 color; };
    /* 0x04 */ s32 blend;
    /* 0x08 */ s32 rect[4];
} dds3Filter2Param_t;

typedef struct
{
    f32 fogMax;
    f32 fogMin;
    f32 fogNear;
    f32 fogFar;
} dds3FogRange_t;

extern dds3LightState_t dds3LightParam;

extern sdf3DFogParam_t dds3FogState;

extern dds3Filter2Param_t dds3Filter2Buf;

extern dds3Filter2Param_t filter2target;

extern u16 filter2now;

extern u16 filter2max;

extern u16 filter2abase;

extern u16 filter2atarget;

extern u16 filter2swapnow;

extern u16 filter2swapmax;

typedef struct dds3MFilterParam {
    /* 0x00 */ s32 area[3];
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ s32 rect[4];
} dds3MFilterParam_t;

typedef struct dds3Blur2Param {
    /* 0x00 */ union { u8 rgba[4]; u32 color; };
    /* 0x04 */ s32 blend;
    /* 0x08 */ f32 rotation;
    /* 0x0C */ f32 scale;
    /* 0x10 */ s32 area[2];
    /* 0x18 */ s32 rect[4];
} dds3Blur2Param_t;

extern dds3MFilterParam_t dds3MFilterBuf;

extern dds3MFilterParam_t mfiltertarget;

extern u16 mfilternow;

extern u16 mfiltermax;

extern u16 mfilterabase;

extern u16 mfilteratarget;

extern u16 mfilterswapnow;

extern u16 mfilterswapmax;

typedef struct dds3RipBlurBody {
    /* 0x00 */ s32 count;
    /* 0x04 */ f32 phaseStep;
    /* 0x08 */ f32 spacing;
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ f32 uvAngle;
    /* 0x18 */ f32 uvAmplitude;
    /* 0x1C */ f32 angleStep;
    /* 0x20 */ s32 x;
    /* 0x24 */ s32 y;
    /* 0x28 */ s32 area;
} dds3RipBlurBody_t __attribute__((aligned(8)));

typedef struct dds3RipBlurParam {
    /* 0x00 */ dds3RipBlurBody_t ripple;
    /* 0x2C */ u32 unk2C;
    /* 0x30 */ dds3Blur2Param_t blur2buf;
} dds3RipBlurParam_t;

extern dds3RipBlurParam_t ripblurtarget;

typedef struct dds3MultiBlurParam {
    /* 0x00 */ s32 extent;
    /* 0x04 */ union { u8 rgba[4]; u32 color; };
    /* 0x08 */ s32 blend;
    /* 0x0C */ f32 rotation;
    /* 0x10 */ f32 scale;
    /* 0x14 */ s32 x;
    /* 0x18 */ s32 y;
    /* 0x1C */ s32 rect[4];
} dds3MultiBlurParam_t;

typedef struct dds3DistBlurParam {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 delaySpread;
    /* 0x08 */ f32 angleStep;
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ f32 uvAngle;
    /* 0x18 */ f32 uvAmplitude;
    /* 0x1C */ s32 x;
    /* 0x20 */ s32 y;
    /* 0x24 */ s32 spread;
    /* 0x28 */ s32 size;
} dds3DistBlurParam_t;

extern dds3MultiBlurParam_t dds3MultiBlurBuf;

extern dds3MultiBlurParam_t mulblurtarget;

extern u16 mulblurnow; extern u16 mulblurmax; extern u16 mulblurabase;

extern u16 mulbluratarget; extern u16 mulblurswapnow; extern u16 mulblurswapmax;

extern dds3DistBlurParam_t dds3DistBlurBuf;

extern dds3DistBlurParam_t distblurtarget;

extern u16 distblurnow; extern u16 distblurmax; extern u16 distblurabase;

extern u16 distbluratarget; extern u16 distblurswapnow; extern u16 distblurswapmax;

extern u32 distblurovercnt;

extern dds3RipBlurBody_t dds3RipBlurBuf;

extern u16 ripblurnow; extern u16 ripblurmax; extern u16 ripblurabase;

extern u16 ripbluratarget; extern u16 ripblurswapnow; extern u16 ripblurswapmax;

extern u32 ripblurovercnt;

s32 *Eve_PartialMultiBlurGetParams(void);

void Eve_PartialMultiBlurSetParam(void *pParam);

void Eve_PartialMultiBlurON(void);

void Eve_PartialMultiBlurOFF(void);

s32 *Eve_DistortionBlurGetParams(void);

void Eve_DistortionBlurSetParam(void *pParam);

void Eve_DistortionBlurON(void);

void Eve_DistortionBlurOFF(void);

s32 *Eve_RippleBlurGetParams(void);

void Eve_RippleBlurSetParam(void *pParam);

void Eve_RippleBlurON(void);

void Eve_RippleBlurOFF(void);

extern dds3Blur2Param_t blur2target;

extern u16 blur2now;

extern u16 blur2max;

extern u16 blur2abase;

extern u16 blur2atarget;

extern u16 blur2swapnow;

extern u16 blur2swapmax;

extern s16 dds3ScreenBlurAlpha;

extern s16 dds3ScreenBlurScale;

extern u8 dds3ScreenBlurFlag;

extern u16 blurnow;

extern u16 blurmax;

extern s16 bluralphabase;

extern s16 blurscalebase;

extern s16 bluralphatarget;

extern s16 blurscaletarget;

s32 *Eve_PartialMultiFilterGetParams(void);

void Eve_PartialMultiFilterSetParam(void *pParam);

void Eve_PartialMultiFilterON(void);

void Eve_PartialMultiFilterOFF(void);

s32 *Eve_BlurGetParam(void);

void Eve_BlurSetParam(void *pParam);

void Eve_BlurON(void);

void Eve_BlurOFF(void);

void setdraw_xy(s32 *xy);

extern dds3FogRange_t dds3FogParam;

extern dds3FogRange_t dds3FogColor;

extern u16 fogparanow; extern u16 fogparamax;

extern f32 sdfFaceDivSoften; extern f32 sdfFaceDivBias;

extern u16 sbnow; extern u16 sbmax;

extern f32 softenbase; extern f32 softentarget;

extern f32 biasbase; extern f32 biastarget;

extern u32 dds3RenderTex;

extern sdfGsMemSlice_t *dds3RenderTexHandle;

extern sdfTexHandle_t *dds3RenderTexture;

extern sdfMat_t *eglMaterial;

void sdfTexFree(sdfTexHandle_t *aHandle);

sdfMat_t *sdfMatNew(void);

void *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, const char *pStr, void *pFRQ);

void frDrawFont(void *pFRQ);

void frDestroyFontRequestAfter(void *pFRQ);

void *Eve_FilterGetParams(void);

void Eve_FilterSetParam(void *pParam);

void Eve_FilterON(void);

void Eve_FilterOFF(void);

void *memcpy(void *a, const void *b, size_t c);

void dds3FadeClear(void);

s32 dds3FadeCheck(void);

extern sdf3DScreenParam_t dds3ScreenParam;

extern sdf3DScreenParam_t dds3ScreenParam2;

extern u8 dds3DrawProcessFlag;

extern u16 dds3QuakeFlag;

extern s16 dds3QuakeFrame;

extern u16 dds3QuakeXRange;

extern u16 dds3QuakeYRange;

extern u32 egl_OT_Offset;

extern f32 dds3QuakeOffset[4];

/* The quake's two axis gains are copied in by value, which is the unaligned
 * eight-byte block move at the entry. */
typedef struct
{
    f32 axis[2];
} dds3QuakeGain_t;

extern dds3QuakeGain_t dds3QuakeGainTable[];

f32 dds3GetRandFloatA(s32 name);

void sdfGrSetScreen(s32 aMode);

void sdf3DMakeScreenParam(sdf3DScreenParam_t *aParam);

void dds3InitDrawEnvSub(s32 i);

void dds3InitZClearDrawTag(void);

void sdf3DLinkInitPacket(sdfDrawTag_t *aTag, sdf3DInitPacket_t *aPacket);

void sdfPreDrawTagInit(sdfPreDrawTag_t *aTag);

void sdfGrSetStdDrawEnv(dds3StdDrawEnv_t *aEnv, sdfGrFrameSet_t *aFrameSet);

void sdfGrLinkStdDrawEnv(sdfDrawTag_t *aTag, sdfPreDrawTag_t *aPreDrawTag, dds3StdDrawEnv_t *aEnv);

extern sdfGrFrameSet_t sdfGrFrameSet;

extern sdfMatrix4_t aProjMatrix;

extern dds3LightState_t dds3LightParam2;

extern sdfVector4_t dds3AmbientColor;

extern sdfVector4_t dds3AmbientColor2;

extern u32 dds3BackColor;

void eglInit(void);

extern sdfGrOt_t sdfGrOtTable[];

extern u64 eglPolyStripWord[];

extern sdfVector4_t eglLineVerts[];

extern u32 eglLineColors[];

extern sdf3DScreenParam_t dds3RenderScreenParam;

extern sdf3DLight_t *dds3RenderLightParallel[];

extern sdfVector4_t dds3RenderAmbientColor;

extern sdfMatrix4_t sdfViewMatrix;

extern sdfMatrix4_t dds3RenderViewportMatrix;

extern sdfGsMemSlice_t *dds3ZBuffHandles[];

extern sdfGrFrameSet_t dds3RenderTexId;

extern dds3DrawBank_t dds3DrawEnv[];

extern dds3ZClearPacket_t dds3ZClearDrawTag[3][2];

extern u16 dds3RenderTexWidth;

extern u16 dds3RenderTexHeight;

extern u32 dds3RenderDrawEnv;

extern u32 dds3DebugSign;

extern u16 lightcolornow[4];

extern u16 lightcolormax[4];

extern u16 lightdirnow[4];

extern u16 lightdirmax[3];

extern u32 dds3LightColor[];

extern u32 dds3LightTargetColor[];

extern f32 dds3LightArcFrom[][4];

extern f32 dds3LightArcTo[][4];

extern u16 ambientnow, ambientmax;

extern u32 ambientbase, ambienttarget;

extern u16 fogrgbnow, fogrgbmax;

extern u32 fogrgbbase, fogrgbtarget;

extern dds3MultiBlurParam_t key2;

extern dds3DistBlurParam_t dds3DistBlurCurrent;

extern dds3RipBlurParam_t dds3BlurState;

extern dds3Blur2Param_t dds3Blur2State;

extern dds3Filter2Param_t dds3BlurParam;

extern dds3MFilterParam_t dds3MFilterCurrent;

void *memset(void *a, s32 b, size_t c);

f32 fabsf(f32 value);

u32 dds3GetFrameID(void);

void dds3FreeRenderTexture(void);

sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

void *sdfGrAllocPacketBuff(u32 aSize);

void sdfDrawTagInit(sdfDrawTag_t *a);

void sdfDrawTagLink(sdfDrawTag_t *aTag, void *aPacket);

void *sdf2DMakeStdPacket(void *aPacket, s32 aLeng);

s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);

sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, s64 aGifRegs, s32 aNumOfPrim);

void *sdf2DGetPrimPtr(void *aPacketPtr);

void sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

void sdfPrimLinkGt4(sdfDrawTag_t *aTag, s32 aPrim,
    s32 aX0, s32 aY0, s32 aU0, s32 aV0, u32 aColor0,
    s32 aX1, s32 aY1, s32 aU1, s32 aV1, u32 aColor1,
    s32 aX2, s32 aY2, s32 aU2, s32 aV2, u32 aColor2,
    s32 aX3, s32 aY3, s32 aU3, s32 aV3, u32 aColor3, s32 aZ, void *aAllocator);

void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, s32 aAllocator);

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, s32 aAllocator);

void *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);

void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);

void sdfMatFreeDelayed(sdfMat_t *aMat);

void *effDebugDrawWireModel(const sdfVector4_t *vertices, u32 *colors, s32 count, s32 alpha);

void sdfPrimSetOpaquePacket(void *aPkt);

void sdfDevSetPrintCB(dds3PrintControl_t *aCB, s32 aX, s32 aY, s32 aZ, u32 aAttr);

void *sdfDevPrintfEx(dds3PrintControl_t *aCB, const char *aStr, ...);

void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, u32 aAttr, const char *aStr, ...);

void sdfGrSetDrawEnv(sdfGrDrawEnv_t *aEnv, sdfGrFrameSet_t *aFrameSet, s32 aEvenOdd);

void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *aPacket, sdf3DScreenParam_t *aParam, sdfMatrix4_t *aCamera);

sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 aW, s32 aH, s32 aPixelFormat);

sdfTexHandle_t *sdfTexNew(s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfMipmap,
    sdfGsMemSlice_t *aPixelGsHandle, s32 aCLUTFormat, s32 aNumOfCLUT, sdfGsMemSlice_t *aCLUTGsHandle);

void sdfTexSetWrapMode(sdfTexHandle_t *aHandle, s32 aMode);

void sdfTexMakeGIFPacket(sdfTexHandle_t *aHandle);

void MultMatrixRev(sdfMatrix4_t *aMatrix);

void Normalize(void);

void dds3RotationArc(void);

void Lerp(f32 aRatio);

void ToMatrix(void);

void evtHokanBlur2(s32 hokan, f32 ratio, dds3Blur2Param_t *key1, dds3Blur2Param_t *key2, dds3Blur2Param_t *d);

void evtHokanMultBlur(s32 hokan, f32 ratio, dds3MultiBlurParam_t *key1, dds3MultiBlurParam_t *key2, dds3MultiBlurParam_t *d);

void evtHokanDistBlur(s32 hokan, f32 ratio, dds3DistBlurParam_t *key1, dds3DistBlurParam_t *key2, dds3DistBlurParam_t *d);

void evtHokanRipBlur(s32 hokan, f32 ratio, dds3RipBlurBody_t *key1, dds3RipBlurBody_t *key2, dds3RipBlurBody_t *d);

void evtHokanFilter(s32 hokan, f32 ratio, dds3Filter2Param_t *key1, dds3Filter2Param_t *key2, dds3Filter2Param_t *d);

void evtHokanMultFilter(s32 hokan, f32 ratio, dds3MFilterParam_t *key1, dds3MFilterParam_t *key2, dds3MFilterParam_t *d);

void dds3InitDrawEnvSub(s32 i);

void dds3InitDrawEnv(void);

void dds3InitRenderDrawEnv(void);

/* Replacing a render texture releases the old texture first. A texture-creation
 * failure leaves the allocated framebuffer handle tracked, as in retail. */
s32 dds3AllocRenderTexture(u16 sx, u16 sy, f32 aspect);

void dds3FreeRenderTexture(void);

s32 dds3RenderTextureON(void);

void dds3RenderTextureOFF(void);

u32 dds3CheckRenderTexture(void);

void *dds3GetRenderTexture(void);

void dds3RunQuake(void);

/* Zero amplitude stops the quake; zero duration leaves it running indefinitely. */
void dds3SetQuake(s32 frame, s32 range);

void dds3StopQuake(void);

/* The two live XYZ2 payload locals follow the matched DDS engine source.
 * Creating each before its RGBAQ pair preserves the native hoisted constants. */
void dds3InitZClearDrawTag(void);

sdfDrawTag_t *dds3LinkZClearDrawTag(s32 sw);

void dds3FadeClear(void);

void dds3FadeSet(s32 r, s32 g, s32 b, s32 a);

void dds3FadeColorSet(s8 r, s8 g, s8 b);

void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

void dds3FadeIn2(s32 frame);

void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

void dds3FadeOut2(s32 frame);

s32 dds3FadeCheck(void);

void dds3FadeUpdate(void);

void dds3DrawDistRipOverError(void);

void dds3SetScreenFovy(f32 fovy);

void dds3SetAmbient(sdfVector4_t *ambient);

void dds3SetFog(s32 *fog);

void dds3SetLight(void *light, s32 no);

void setdraw_xy(s32 *xy);

void dds3SetFilter2Para(s32 blend);

void dds3SetFilter2RGBA(u32 rgba);

void dds3Filter2Swap(s32 frame);

void dds3Filter2On(s32 frame);

void dds3Filter2Off(s32 frame);

void dds3SetMFilterPara(s32 blend);

void dds3SetMFilterRGBA(u32 rgba);

void dds3SetMFilterArea(s32 area, s32 posx, s32 posy);

void dds3MFilterSwap(s32 frame);

void dds3MFilterOn(s32 frame);

void dds3MFilterOff(s32 frame);

void dds3SetBlur(s32 frame, s32 alpha, s32 scale);

void dds3SetBlur2Para(f32 rotate, f32 scale, s32 blend);

void dds3SetBlur2RGBA(u32 rgba);

void dds3SetBlur2Area(s32 posx, s32 posy);

void dds3Blur2Swap(s32 frame);

void dds3Blur2On(s32 frame);

void dds3Blur2Off(s32 frame);

void dds3SetMultiBlurPara(f32 rotate, f32 scale, s32 blend);

void dds3SetMultiBlurRGBA(u32 rgba);

void dds3SetMultiBlurArea(s32 area, s32 posx, s32 posy);

void dds3MultiBlurSwap(s32 frame);

void dds3MultiBlurOn(s32 frame);

void dds3MultiBlurOff(s32 frame);

void dds3SetDistBlurPara(s32 number, s32 width, s32 intval, f32 rotate,
                         f32 distor, f32 blend, s32 speed);

void dds3SetDistBlurRGBA(u32 rgba);

void dds3SetDistBlurArea(s32 area, s32 posx, s32 posy);

void dds3DistBlurSwap(s32 frame);

void dds3DistBlurOn(s32 frame);

void dds3DistBlurOff(s32 frame);

void dds3SetRipBlurPara(s32 number, f32 intval, f32 rotate, f32 distor, f32 distor_s, f32 blend, s32 speed);

void dds3SetRipBlurRGBA(u32 rgba);

void dds3SetRipBlurArea(s32 area, s32 posx, s32 posy);

void dds3RipBlurSwap(s32 frame);

void dds3RipBlurOn(s32 frame);

void dds3RipBlurOff(s32 frame);

void dds3BlurUpdate(void);

/* vu0 routine: shortest-arc quaternion rotating VF10 to VF11. */
void dds3RotationArc(void);

/* vu0 routine: stage an immediate or interpolated parallel light color. */
void dds3SetLightColor(s32 frame, s32 no, f32 *col);

void dds3SetLightDir(s32 frame, s32 no, f32 *dir);

/* vu0 routine: stage an immediate or interpolated ambient color. */
void dds3SetAmbientColor(s32 frame, f32 *col);

/* vu0 routine: stage an immediate or interpolated packed color. */
void dds3SetFogColor(s32 frame, f32 *col);

void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);

/* SDK float-input color conversion, shared by the six light/ambient/fog inputs. */
static inline void dds3UnpackLightColor(u32 color, f32 unit) {
    u32 rgba[4] __attribute__((aligned(16)));
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* vu0 routine: advance staged light, ambient, fog and face divide blends. */
void dds3SceneLightUpdateFirst(void);

void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias);

void eglInit(void);

void eglSetOt(u32 ot);

void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

void eglLinkAlpha(s32 mode);

void eglLinkSprite(s32 x, s32 y, s32 w, s32 h,
                   s32 u, s32 v, s32 uw, s32 vh,
                   u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th);

void eglLinkBox(s32 x, s32 y, s32 w, s32 h,
                s32 rgba0, s32 rgba1, s32 rgba2, s32 rgba3);

void eglLinkTri(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 rgba0, s32 rgba1, s32 rgba2);

/* Preserve native vertex order 0,2,3,1; W components are not initialized. */
void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 color0, u32 color1, u32 color2, u32 color3);

/* Preserve native vertex order 0,2,3,1; W components are not initialized. */
void eglLinkTexPoly4(s32 color0, f32 x0, f32 y0, f32 z0, s32 color1, f32 x1, f32 y1, f32 z1, s32 color2, f32 x2, f32 y2, f32 z2, s32 color3, f32 x3, f32 y3, f32 z3, sdfTexHandle_t *texture, f32 u0, f32 v0, f32 u1, f32 v1);

void eglLinkLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 color0, u32 color1);

void eglLinkPrint(s32 x, s32 y, u32 col, const char *str);

void eglLinkKanji(s32 x, s32 y, s32 rgba, const char *moji);

/* vu0 routine: project a world point into viewport pixel coordinates. */
void egl3Dto2D(s32 *x3, s32 *y3, f32 z3, f32 x2, f32 y2);

extern u32 egl_OT_Offset;

extern u32 dds3BackColor;

extern u32 dds3RenderTex;

extern sdfGsMemSlice_t *dds3RenderTexHandle;

extern sdfTexHandle_t *dds3RenderTexture;

extern u16 dds3RenderTexWidth;

extern u16 dds3RenderTexHeight;

extern u32 dds3RenderDrawEnv;

extern u32 dds3DrawFlag;

extern u8 dds3ScreenBlurFlag;

extern s16 dds3ScreenBlurAlpha;

extern s16 dds3ScreenBlurScale;

extern s32 dds3ScreenBlurOfsX[2];

extern u16 dds3QuakeFlag;

extern s16 dds3QuakeFrame;

extern u16 dds3QuakeXRange;

extern u16 dds3QuakeYRange;

extern u8 fadergba[4];

extern u32 distblurovercnt;

extern u32 ripblurovercnt;

extern dds3QuakeGain_t dds3QuakeGainTable[1];
/* End of declarations from src/smt3/kernel/dds3KernelDraw.c */

/* Declarations from dds-decomp src/dds1/game/code_00102ED8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_dds3Blur2Param 1

#define SMT3_DEFINES_dds3Blur2Param_t 1

#define SMT3_DEFINES_dds3DistBlurParam 1

#define SMT3_DEFINES_dds3DistBlurParam_t 1

#define SMT3_DEFINES_dds3FadePacket_t 1

#define SMT3_DEFINES_dds3Filter2Param 1

#define SMT3_DEFINES_dds3Filter2Param_t 1

#define SMT3_DEFINES_dds3FogRange_t 1

#define SMT3_DEFINES_dds3GouraudRegister_t 1

#define SMT3_DEFINES_dds3GsCommand_t 1

#define SMT3_DEFINES_dds3MFilterParam 1

#define SMT3_DEFINES_dds3MFilterParam_t 1

#define SMT3_DEFINES_dds3MultiBlurParam 1

#define SMT3_DEFINES_dds3MultiBlurParam_t 1

#define SMT3_DEFINES_dds3PrintControl_t 1

#define SMT3_DEFINES_dds3QuakeGain_t 1

#define SMT3_DEFINES_dds3RipBlurBody 1

#define SMT3_DEFINES_dds3RipBlurBody_t 1

#define SMT3_DEFINES_dds3RipBlurParam 1

#define SMT3_DEFINES_dds3RipBlurParam_t 1

#define SMT3_DEFINES_dds3ZClearPacket_t 1

#include "common.h"

#include "sdk_libc.h"

#include "dds/dds3_process_registration.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_task_update_count.h"

#include "dds/eff_random_mod.h"

#include "dds/kwln.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_gs_scene_state.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_pad_motor_api.h"

#include "dds/sdf_texture_lists.h"

#include "dds/sdf_texture_registry.h"

#include "dds/sdk_libm.h"

f32 acosf(f32 x);

/* Test membership in the starting, running, and destruction-pending queues. */
dds3ProcessRegistrationResult_t dds3ExistProcess(const struct dds3ProcessID_t *task);

/* The API returns this count; it resets on creation/activation and increments
 * after callback handling, except when an active callback finishes immediately. */
dds3TaskUpdateCount_t dds3GetProcessRunTime(const dds3ProcessID_t *task);

/* Return the next caller-owned or shared state value reduced modulo `modulus`. */
u32 dds3GetRandIntA(EffRandState *state, u32 modulus);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

void *sdfChipAllocDirect(s32 size);

void sdfGrSetClearPacket(SdfGsCenteredBoundsRegisters *packet,
    s32 width, s32 height, s32 unused0, s32 unused1);

/* Return borrowed endpoints; callers may inspect list nodes but do not own them. */
sdfGsMemSlice_t *sdfGsMemGetLinkHead(void);

void sdfPadSetAct(SdfPadIndex padIndex, s32 smallMotor, s32 largeMotor);

/* These APIs index the two-entry logical-pad table without bounds checks. */
void sdfPadSetAct0(SdfPadIndex padIndex, u16 strength);

void sdfPadSetAct1(SdfPadIndex padIndex, u8 strength);

void sdfPrimLinkLine(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkRect(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkSprite(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
    s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth,
    s32 (*allocatePacket)(s32));

/* The active resource registry contains sdfTexHandle_t nodes; lookup compares the
 * texture's resource key and follows its previous-node link. A match is borrowed:
 * lookup does not retain it, and callers must not release it as an acquired ref. */
extern struct sdfTexHandle_t *sdfTexHandleLink;

#include "dds/sdk_ps2.h"

#include "dds/sdf_scene_packet.h"

#include "dds/sdf_gs_scene_state.h"

#include "dds/common.h"

#include "dds/file_config_option_api.h"

#include "dds/sdf_vu0.h"

#include "dds/dds3_background_fade_api.h"

#include "dds/sdf_pad_motor_api.h"

#include "dds/dds3Admin.h"

#include "dds/kernel.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/eff_random_mod.h"

#include "dds/eff_random.h"

#include "dds/sdf_cons_matrix_packet.h"

#include "dds/sdf_dma_packet_header.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_tex_packet.h"

#include "dds/kwln.h"

#include "dds/dds3_task_update_count.h"

#include "dds/dds3_process_registration.h"

#include "dds/dds3_named_slot.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_runner.h"

#include "dds/dds3_fade.h"

#include "dds/dds3_fade_timing.h"

#include "dds/sdf.h"

#include "dds/sdf_texture_lists.h"

#include "dds/sdf_texture_registry.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_linked_packet.h"

#include "dds/sdf_draw.h"

#include "dds/sdf_gs_blend.h"

#include "dds/sdf_sif_command.h"

#include "dds/pcp_vu0.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/sdf_dma_tag.h"

#include "dds/sdf_gs_packet.h"

#include "dds/btl.h"

#include "dds/dds3_fade_color.h"

#include "dds/evt_frame_state_packet.h"

extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);

extern void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);

extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

extern u16 D_003BA918;

extern s16 D_003BA91A;

extern u16 D_003BA91C;

extern u16 D_003BA91E;

extern sdfGsMemSlice_t *D_003BA8F4;

extern u32 dds3TextureViewerPageIndex;

extern u8 D_r1_0046B1E0;

extern s8 D_003BA7FC;

extern s8 dds3BackgroundFadeMode;

extern s16 dds3BackgroundFadeFirstRamp;

extern s16 dds3BackgroundFadeSecondRamp;

extern u8 dds3BackgroundFadeColor[4];

extern s8 D_r1_003AD3BB[];

extern f32 D_003245EC[];

extern sdfGrOt_t sdfDebugGrOt;

extern u8 dds3PadVibrationState[2];

extern s32 D_003BD6A0[2];

extern u8 kwlnLargeMotorTarget;

extern u16 dds3BackgroundFadeCounter;

extern u16 dds3BackgroundFadeDuration;

extern sdfTexHandle_t *dds3CurrentIncompleteResource;

extern EffRandState effSharedRandomState[];

extern void sdfRandomize(void *data, u32 tag);

extern s32 dds3TextureViewerHandlePad(void);

extern void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image);

extern void dds3TextureAttachTask(sdfGrOt_t *surface);

extern void dds3InitializeFrameDrawPackets(s32 bufferIndex);

#include "dds/sdf_packet_alloc.h"

extern void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

extern void dds3PadVibrationSet(u32, u8, s32);

extern s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);

extern dds3ProcessID_t *dds3DebugGraphTask;

extern s32 dds3TextureCountIncompleteResources(void);

extern s32 D_003BA724;

extern s8 dds3PadBitTable[32];

extern s32 dds3SelectIncompleteResourceAtPageIndex(s32);

extern sdfGrOt_t D_00325708;

extern char dds3AdminTaskName[];

extern dds3TaskUpdateResult_t dds3AdminiNeoProcess(dds3ProcessID_t *task);

extern void dds3AdminiNeoShutdown(dds3ProcessID_t *task);

void dds3InitAdmini(void);

extern fclNamedSlot_t D_003C1C90[];

extern char D_0039E018[];

extern s32 D_003BA850;

extern s32 D_003BA854;

extern s32 D_003BA858;

extern s32 D_003BA85C;

s32 fclEncycFreeEntry(const char *name);

s32 fclEncycFindFreeSlot(const char *name);

/* One shared initializer for the three configure modes and unregister. */
static inline void dds3SetNamedSlotState(s32 index, s32 kind, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy,
                                         void (*setState)(s32), void *userValue) {
    D_003C1C90[index].kind = kind;
    D_003C1C90[index].task = NULL;
    D_003C1C90[index].update = update;
    D_003C1C90[index].destroy = destroy;
    D_003C1C90[index].setState = setState;
    D_003C1C90[index].enabled = 0;
    D_003C1C90[index].userValue = userValue;
}

#define DDS3_PAD_INPUTS_PER_SET 16

#define DDS3_PAD_REPEAT_BIT 2

#define DDS3_ALT_CONFIRM_INPUT 0x11

#define DDS3_ALT_CANCEL_INPUT 0x13

#define DDS3_PAD_SMALL_MOTOR 0

#define DDS3_PAD_LARGE_MOTOR 1

#define DDS3_PAD_MOTOR_COUNT 2

#define DDS3_DEBUG_RUMBLE_PERIOD 0x4650

#define DDS3_DEBUG_SMALL_PULSE_FRAMES 0xF

#define DDS3_DEBUG_LARGE_PULSE_FRAMES 30

#define DDS3_DEBUG_RUMBLE_VARIATION 150

#define DDS3_DEBUG_RUMBLE_BASE 100

#define DDS3_SHARED_RANDOM_SEED 0x12345678

#define DDS3_CELL_COLUMN_SPAN 0xC0

#define DDS3_CELL_ROW_SPAN 0x60

#define DDS3_PREVIEW_MAX_TEXELS 0x100

#define DDS3_PREVIEW_WRAP_TEXELS 0x400

#define DDS3_DIAG_DEPTH 0x0FFFFF80

/* Find an exact NUL-terminated name along the linked slot indices, or return -1. */
s32 fclEncycFreeEntry(const char *name);

/* Unregister a named slot: stop its task, clear it and unlink it from the slot chain. */
s32 fclEncycFreeEntries(const char *name);

s32 kwlnConfigureNamedSlotMode3(const char *name, void (*setState)(s32));

s32 fclEncycAllocEntry(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue);

s32 kwlnConfigureNamedSlotMode2(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue);

void dds3InitDebugSystem(void);

extern s8 dds3PadButtonState[64];

/* Fresh presses wrap; repeat-bit input stops at the implemented end bounds.
 * Without scrolling, forward motion is bounded by visibleRows, not itemCount. */
void SelectSub(s32 *scrollOffset, s32 *cursorRow, s32 itemCount, s32 visibleRows, s32 padSet, s32 forwardInput, s32 backwardInput);

/* Step the primary then secondary cursor pair; return 1/-1 for confirm/cancel.
 * Any nonzero padSet tests set 1's confirm/cancel, while movement uses padSet. */
s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible, s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll, s32 *secondaryCursor, s32 *primaryCursor);

/* Append a cell rectangle; rowSpan also supplies the horizontal padding. */
void dds3ConsPanelTagLink(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount);

/* Append the same cell rectangle with caller-selected depth. */
void dds3ConsPanelTagLinkZ(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth);

/* Periodically pulses both pad motors, varying the large-motor level. */
s32 dds3PadVibrationTest(dds3ProcessID_t *task);

extern u32 dds3PadButtonHoldCounters[4][16];

extern u32 dds3PadStickHoldCounters[2][2];

extern s8 dds3PadButtonPrev[2][2][16];

extern u8 D_003BA860[2][2][2];

extern s8 D_003BA868[2][2];

extern u8 D_003BA870[2][2][2];

extern u8 D_003BA878[2][2];

extern u8 D_003BD698[2];

void dds3PadInitialize(void);

extern u8 sdfPadAnalogSticks[8];

extern void dds3UpdateVibrationSlots(void);

void kwlnUpdatePadInputState(void);

void kwlnPadStepLargeMotorLevel(void);

/* Honor the saved rumble option, normalize small-motor strength and clamp
 * motor indices to the large motor. Preserve target update before output. */
void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

/* Clear motor levels and countdowns, then send both zero strengths to pad 0. */
void kwlnPadResetMotorLevelsAndOutput(void);

/* Tick both rumble countdowns; stop expired motors and refresh the active large motor. */
void dds3UpdateVibrationSlots(void);

s32 dds3TaskNoopCallback(dds3ProcessID_t *task);

/* Seed the shared effect random state with the fixed initialization seed. */
void dds3InitRandCB(void);

extern s16 D_r1_0046B128;

extern u16 sdfDrawFrameCounter;

extern u16 D_r1_0046B1D0;

extern u16 D_003BA882;

extern u16 D_r1_0046B1D4;

extern u16 D_003BA886;

extern char D_r1_0046B1D8[];

s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);

void dds3DebugGraphTaskDestroy(dds3ProcessID_t *task);

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void dds3DebugGraphSetEnabled(s8 enabled);

/* Scale the longer preview side to at most 256 texels, preserving division
 * before coordinate scaling; UV endpoints at 1024 texels are reduced by one. */
void dds3DrawImageOutline(sdfDrawTag_t *packetList, sdfTexHandle_t *image);

extern u32 dds3TextureGetPageIndex(void);

/* Append the zero-based viewer page and final page index; an empty list prints -1. */
void dds3TextureDrawPageCounter(sdfDrawTag_t *packetList);

extern const char *D_r1_003AD3E0[];

extern char D_0039E170[];

extern char D_0039E188[];

void dds3DrawTextureResourceDetails(sdfDrawTag_t *packetList, sdfTexHandle_t *texture);

#define DDS3_DIAG_PACKET_LIST_BYTES 0x20

#define DDS3_VIEWER_FAST_PAGE_STEP 10

#define DDS3_MAP_ROW_UNITS 0x1000

#define DDS3_MAP_ROW_SHIFT 12

#define DDS3_MAP_ROW_MASK 0xFFF

#define DDS3_MAP_COLUMN_SHIFT 5

#define DDS3_MAP_COLUMN_UNITS 32

#define DDS3_MAP_COLUMN_COUNT 128

#define DDS3_MAP_ROW_SPAN 0x800

#define DDS3_MAP_BORDER_WIDTH 0x840

#define DDS3_MAP_CELL_X_STEP 16

#define DDS3_MAP_ROW_Y_STEP 8

#define DDS3_MAP_CELL_X_SHIFT 4

#define DDS3_MAP_GREEN 0x80008000

#define DDS3_MAP_YELLOW 0x80008080

#define DDS3_MAP_RED 0x80000080

#define DDS3_MAP_BLUE 0x80800000

#define DDS3_MAP_CYAN 0x80808000

#define DDS3_HELD_TEXTURE_CLAMP_MODE 5

/* Count texture entries without a reference clone source. */
s32 dds3TextureCountIncompleteResources(void);

u32 dds3TextureGetPageIndex(void);

/* Clamp a negative request and return the reached index. DDS1 index zero
 * selects the head without scanning clone-source links; tail exits retain prior writes. */
s32 dds3SelectIncompleteResourceAtPageIndex(s32 requestedIndex);

/* Exit input returns 0 immediately; otherwise apply one prioritized page step,
 * select its reached index, then optionally toggle the viewer mode and return 1. */
s32 dds3TextureViewerHandlePad(void);

extern s32 D_r1_0046B230;

void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode);

void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image);

/* With a nonempty list, invoke its renderer even when input reports exit.
 * The surface owns the packet submission callback; return input status. */
s32 dds3SwapActiveResource(sdfGrOt_t *surface);

/* Use the default surface: empty list returns 0, exit input -1, drawing 0. */
s32 dds3LoadDefaultResource(void);

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode);

/* Draw the allocation-map border, then walk blocks through their prev links. */
void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);

/* Build an allocation-map packet list and submit it through the surface callback. */
void dds3TextureAttachTask(sdfGrOt_t *surface);

/* Submit the default surface's map only when the control array's first byte is zero. */
s32 dds3EnsureDefaultResource(void);

#include "dds/dds3_frame_draw_bank.h"

extern u32 D_003BA8EC;

extern sdf3DLight_t *D_00324770[3];

extern f32 kwlnDefaultColorVector[4];

extern Matrix4 D_00329790;

extern sdf3DLight_t *kwlnOverlayLightSources[3];

extern f32 kwlnOverlayAmbientColor[4];

/* Initialize the selected frame bank's scene and overlay packet chains. */
void dds3InitializeFrameDrawPackets(s32 bufferIndex);

#include "dds/dds3_texture_bank.h"

extern u16 D_003BA8FC;

extern u16 D_003BA8FE;

extern u16 D_003BA8FC;

extern u16 D_003BA8FE;

#include "dds/dds3_texture_reference_flag_api.h"

extern f32 sdfViewShakeVector[];

extern f32 D_003BA940[];

/* Advance the two bounded shake offsets and taper finite-duration amplitudes. */
void kwlnAdvanceShakeOffsets(void);

#define DDS3_FADE_TICKS_PER_FRAME 10

#define DDS3_FADE_MARK_NUMERATOR 4

#define DDS3_FADE_COUNT_DOWN_BIT 1

#define DDS3_FADE_COUNT_UP_BIT 2

#define DDS3_FADE_DIRECTION_BITS 3

#define DDS3_FADE_MAX_ALPHA 0x80

#define DDS3_FADE_ALPHA_SHIFT 7

#define DDS3_COLOR_ALPHA_CHANNEL 3

#define DDS3_BGFADE_COUNT_DOWN_BIT 0x04000000

#define DDS3_BGFADE_COUNT_UP_BIT 0x08000000

#define DDS3_BGFADE_DIRECTION_BITS 0x0C000000

#define DDS3_BGFADE_KEEP_OTHER_BITS 0xF3FFFFFF

#define DDS3_BGFADE_CLEAR_UP_MASK 0xF7FFFFFF

#define DDS3_BGFADE_CLEAR_DOWN_MASK 0xFBFFFFFF

#define DDS3_BGFADE_FIRST_RAMP_MAX 0x31

#define DDS3_BGFADE_SECOND_RAMP_MAX 0x4F

#define DDS3_BGFADE_FIRST_RAMP_SCALE 49.0f

#define DDS3_BGFADE_SECOND_RAMP_SCALE 79.0f

/* Return the live color address through outColor, rather than copying channels. */
void dds3FadeColorGet(dds3FadeColor_t **outColor);

/* Cancel background direction, zero RGBA and restore the inactive ramp maxima.
 * This reset does not rewrite the background counter or duration. */
void dds3FadeResetBackground(void);

/* Start the background countdown; zero duration performs the full background reset. */
void dds3FadeBackgroundStartOut(s32 duration);

/* Start the background count-up; zero duration leaves max alpha and zero ramps. */
void dds3FadeBackgroundStartIn(s32 duration);

/* Direction flags take precedence; idle visibility uses alpha in mode zero,
 * otherwise it tests whether the first ramp differs from its inactive maximum. */
dds3BackgroundOverlayActivity_t dds3FadeIsBackgroundOverlayActive(void);

/* Store the visibility mode; its stored zero value clears ramps, otherwise
 * maximum alpha is selected. Counter direction and timing are unchanged. */
void dds3FadeSetMode(s32 visibilityMode);

/* Step the u16 counter, then derive integer alpha and f32 ramp samples.
 * Completion clears timing/flags after sampling, retaining endpoint values. */
void dds3StepBackgroundFade(void);

void dds3DrawBackgroundFadeBands(void);
/* End of declarations from src/dds1/game/code_00102ED8.c */

/* Declarations from dds-decomp src/dds1/game/code_00107FD8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "event.h"

#include "scr.h"

#include "sdf_packet_alloc.h"

#include "sdk_libc.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_task_priority_query.h"

#include "dds/fld_scene_sequence_api.h"

#include "dds/mnu_camp_state_ack_api.h"

#include "dds/scr.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_dev_cons.h"

#include "dds/sdf_dev_console_node.h"

#include "dds/sdf_general_heap_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

s32 DIntr();

s32 EIntr(void);

/* Consume CLEANED_UP back to IDLE, then report whether camp remains active. */
MnuCampStateAcknowledgementResult campCheckExitState(void);

/* Set bit 1 in the active administration task's flags; that task must exist. */
void dds3AdminiExitSequence(void);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

/* Returns a borrowed task from the scheduler queues, or NULL when none has this priority. */
dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

/* The packet is borrowed for this call. Mode zero copies its fields into
 * persistent scene work before asynchronous tasks start. */
void fldProcTrigSequence(const struct fldSceneParam_t *record, u32 mode);

extern scrProcessWork_s *scrHandleFirst;

extern scrProcessWork_s *scrHandleLast;

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

void sdfChipGetInfo(SdfChipHeapStats *stats);

void sdfDevConsAttribute(sdfDevConsCB_tag *console, u8 attribute);

void sdfDevConsClose(sdfDevConsCB_tag *node);

sdfDevConsCB_tag *sdfDevConsOpen(u32 screenOriginX, u32 screenOriginY, s32 columns, s32 rows);

s32 sdfDevConsPrintf(sdfDevConsCB_tag *console, const char *format, ...);

/* Update the console state stored at the corresponding field offsets. */
void sdfDevConsSetMode(sdfDevConsCB_tag *console, u8 controlByte);

void sdfDrawTagLinkRefGif(struct sdfDrawTag_t *list, u32 packetAddress);

/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);

void sdfMemGetInfo(SdfGeneralHeapStats *stats);

/* Return NULL when the following link is the end marker. */
const struct sdfMemHandle_t *sdfMemGetNext(const struct sdfMemHandle_t *block);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

s32 sdfMemGetSize(const struct sdfMemHandle_t *block);

/* Borrowed read-only descriptors; exclude heap mutation while traversing. */
const struct sdfMemHandle_t *sdfMemGetTop(void);

/* Return free/used/end-sentinel state, not a resource kind. */
SdfMemBlockState sdfMemGetType(const struct sdfMemHandle_t *block);

s32 vsprintf(char *dst, const char *format, void *args);

#include "dds/sdk_ps2.h"

#include "dds/evt_sel_state.h"

#include "dds/evt_start_event_api.h"

#include "dds/evt_script_event_id.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/sdf_gs_header.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/sdf_dev_cons.h"

#include "dds/sdf_dev_console_node.h"

#include "dds/sdf_gs_blend.h"

#include "dds/ee_mmi.h"

#include "dds/eff_quaternion_nlerp.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/pcp_vu0.h"

#include "dds/common.h"

#include "dds/file_menu_start_mode.h"

#include "dds/sdf_vu0.h"

#include "dds/dds3_background_fade_api.h"

#include "dds/mnu_camp_state_ack_api.h"

#include "dds/fld_lmap_task_presence.h"

#include "dds/fld_terminal_scene_state.h"

#include "dds/sdf_heap_overlay_block.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_frame_draw_buffer.h"

#include "dds/evt_gradient_rect.h"

#include "dds/sdf_textured_rect.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_packet_append.h"

#include "dds/fr_font.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/kwln.h"

#include "dds/dds3_task_priority_query.h"

#include "dds/dds3_task_create.h"

#include "dds/sdf.h"

#include "dds/sdf_asset_state.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_general_heap_api.h"

#include "dds/sdf_sif_command.h"

#include "dds/scr.h"

#include "dds/scr_instruction_stream_api.h"

#include "dds/scr.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/brs_task.h"

#include "dds/fld_resource_wait_api.h"

#include "dds/fld_scene_sequence_api.h"

#include "dds/itf.h"

/* Declared before first use (were implicit int). */
extern void btlCreateStageTestTask(void);

extern s32 btlExitWhenAudioAndTasksIdle(void);

extern void evtBattleStageTestStopTask(void);

extern void evtFreeStage(void);

extern void evtInitEventTest(void);

extern void evtViewerCreateTaskWithSky(void);

extern void fileMenuReleaseNoOp(void);

extern void fileReleaseMenuResources(void);

extern void fldCleanupFieldScene(void);

extern void fldProcNextJump(void);

extern void fldProc_CallSisetuReturn(void);

extern void fldRunPendingSceneAction(void);

extern void fldStopLmapTask(void);

extern void fldStopSceneTasks(void);

extern void itfInitializeAdminInterfaceNoop(void);

extern void itfShutdownAdminInterfaceNoop(void);

extern void itfStartFontTestScene(void);

extern void mnuCampDestroyPanelTasks(void);

extern void dds3ConfigProcessEnd(void);

extern void mnuCreateCampTasks(void);

extern void InitMovieViewer(void);

extern void mnuDestroyCampTasks(void);

extern void FinishMovieViewer(void);

extern s32 mnuDestroyTitleMenuTask(void);

extern s32 mnuOpenShopSceneWithInitialSelection(s32 *initialSelection);

extern u32 mnuOpenTitleMenuForMode(s32 mode);

extern dds3ProcessID_t *mnuStaffCreateTasks(void);

extern u32 mnuStaffDestroyTasks(void);

extern u32 mnuStartStaffMovieRequest(void);

extern s32 mnuStopStaffTasks(void);

extern s32 mnuTerminalCreateTasks(s32 reduced, s32 slot);

extern void dds3ConfigProcessStart(s32 mode);

enum {
    EVT_PACKET_LIST_BYTES = 0x20,
    EVT_GS_COMMAND_BYTES = 0x30,
    EVT_GS_TEST_PRIMARY = 0x47,
    EVT_GS_TEST_SECONDARY = 0x48,
    EVT_GS_ALPHA_PRIMARY = 0x42,
    EVT_FIXED_ALPHA_INTERPOLATION = 0x64,
    EVT_QUAD_DEFAULT_DEPTH = 0xFFFFFF,
    EVT_FRAME_REFERENCE_PACKET_BYTES = 0x40,
    EVT_FRAME_DRAW_RECORD_BYTES = 0x1F40,
    EVT_BACKGROUND_BLEND_FLAG = 0x100,
    EVT_DRAW_COLOR_BLEND_FLAG = 0x200,
    EVT_DRAW_VECTOR_BLEND_FLAG = 0x400,
    EVT_SAVED_VECTOR_BLEND_FLAG = 0x4000,
    EVT_QUAD_DESCRIPTOR_BYTES = 0x2C,
    EVT_QUAD_VERTEX_COUNT = 4,
    EVT_SELECTION_ALLOCATION_BYTES = 0x28,
    EVT_DEV_CONSOLE_BUFFER_BYTES = 0x200,
    BF_FLW0_MAGIC = 0x30574C46
};

extern sdfDevConsCB_tag *evtConsoleFontResourceChain;

extern u8 fldLmapTaskExists(void);

extern FldTerminalScenePollResult fldPollSceneState(void);

extern s32 mnuPollTaskState(void);

extern u32 D_003BA958;

extern s32 checkConfigState(void);

extern u32 D_003BA730;

extern s32 fileMenuTaskExists(void);

extern void fileEnterMcPackScene(FileMenuStartMode mode);

extern s32 fileMenuOperationSucceeded(void);

extern void fldMainLoadStart(void);

extern void btlStart(s32 mode, s32 pack, s32 encounter);

extern void evtEventViewerDestroyTask(void);

extern void evtChkEndProcess(void);

extern void evtExitSky(void);

#include "dds/sdf_asset_create.h"

extern s8 evtSelectionStateActive;

extern EvtSelState *evtSelectionState;

extern char D_003BA950[];

extern void *D_003BD6B0;

extern u16 D_003BD706;

extern f32 D_003BD708;

extern f32 D_003BD70C;

extern f32 D_003BD710;

extern f32 D_003BD714;

extern u16 D_003BD704;

extern char D_0039E1F0[];

extern char D_0039E200[];

extern dds3ProcessID_t *evtInitSky(void);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

/* The allocated draw state's float at +0x1C is initialized to 1. */
typedef struct EvtDrawState {
    u8 pad00[0x1C];
    f32 value1C;
} EvtDrawState;

extern KwlnDrawVectorParams kwlnDrawVector;

extern DrawVec4 D_003C2C20;

extern DrawVec4 D_003C2C30;

extern u16 D_003BD6FA;

extern u16 D_003BD6F8;

extern void evtUpdateSelectionStateInterpolation(void);

extern void evtSelStateDestroy(void);

extern s32 D_0032E3C0[];

extern void fldStartLmapTask(s32 mode);

extern u32 scrHandleCount;

extern void evtDrawHeapUsageOverlay(sdfGrOt_t *);

extern s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *);

extern void *D_003BD764;

typedef struct dds3ResourceNode_t {
    s32 unk0;
    struct dds3ResourceNode_t *next;
    s32 *ready;
} dds3ResourceNode_t;

extern s32 sdfPrintIopFlag;

extern s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);

extern void *D_003BD768;

extern char D_0039E238[];

extern u16 D_003BD6C8[4];

extern u16 D_003BD6D0[4];

extern u32 D_003C2BA0[4];

extern u32 D_003C2BB0[4];

/* Normalize the requested light direction; replace it immediately or prepare its blend. */
extern u16 D_003BD6D8[4];

extern u16 D_003BD6E0[4];

extern f32 D_003C2BC0[][4];

extern f32 D_003C2BF0[][4];

extern u16 D_003BD6E6;

extern u16 D_003BD6E8;

extern u32 D_003BD6EC;

extern u32 D_003BD6F0;

extern u32 D_003247A0[];

extern u16 D_003BD6F4;

extern u16 D_003BD6F6;

extern u32 D_003BD6FC;

extern u32 D_003BD700;

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct EvtGsCommand {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} EvtGsCommand;

#include "dds/sdf_packet_alloc.h"

/* Submit the same TEST bit layout for the secondary GS context, with ZTE forced on. */
void evtSubmitSecondaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

/* Submit ALPHA_1 = (Cs - Cd) * FIX / 128 + Cd; retain the native unmasked FIX input. */
void evtSubmitFixedAlphaBlend(s32 fixedAlpha);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

void evtSubmitPrimaryFramePacket(void);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

void evtSubmitAlternateFramePacket(void);

void evtSubmitTexturedGradientQuad(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture);

/* Native rectangle emitter: x/y/width/height, explicit depth, then TL/TR/BR/BL colors. */
void evtSubmitGradientRectAtDepth(s32 x, s32 y, s32 w, s32 h, u32 depth, s32 color0, s32 color1, s32 color2, s32 color3);

extern u64 D_003245A0[];

extern f32 D_003245B0[][4];

extern u32 D_003245D0[];

/* Zero frames leaves the prior state untouched; zero countLimit selects the -1 sentinel. */
s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue);

/* Return zero only when inactive on entry; updating or destroying active state returns one. */
u32 evtCheckSelectionState(void);

void evtDestroySelectionState(void);

void evtSelStateDestroy(void);

dds3ProcessID_t *CallDebug(void);

u32 ExitDebug(void);

/* Admin mode 0 cleanup: zero leaves the mode pending. */
u32 CheckDebug(void);

void CallBootMc(void);

u32 ExitBootMc(void);

u8 CheckBootMc(void);

u32 CallTitle(u32 unused, u32 *modeOverride);

u32 ExitTitle(void);

/* Admin mode 2 cleanup: zero leaves the mode pending. */
u32 CheckTitle(void);

void CallLoad(void);

s32 ExitLoad(void);

u8 CheckLoad_dds3SequenceList(void);

u32 CallStaff(void);

u32 ExitStaff(void);

/* Admin mode 12 cleanup: zero leaves the mode pending. */
u32 CheckStaff(void);

void CallEndSave(void);

u32 ExitEndSave(void);

u8 CheckEndSave(void);

void CallField(u32 selection, const struct fldSceneParam_t *record);

u32 ExitField(void);

/* Admin mode 5 cleanup: zero leaves the mode pending. */
u32 CheckField(void);

void evtStartSelectionFadeOut(void);

/* Admin mode 11 destroy callback: no work, zero transition result. */
u32 evtDestroySelectionFadeOutNoOp(void);

/* Admin mode 11 cleanup: zero leaves the mode pending. */
u32 evtKeepSelectionFadeOutPending(void);

/* Source zero takes an explicit event ID; source one uses the pending event ID. */
void CallEvent(s32 selectionSource, EvtScriptEventId *eventParams);

u32 ExitEvent(void);

/* Admin mode 6 cleanup: zero leaves the mode pending. */
u32 CheckEvent(void);

void CallCamp(void);

u32 ExitCamp(void);

u8 CheckCamp(void);

/* Admin mode 8 entry intentionally performs no work. */
void CallRecover(void);

/* Admin mode 8 destroy callback: no work, zero transition result. */
u32 ExitRecover(void);

/* Admin mode 8 cleanup: one immediately reports completion. */
u32 CheckRecover(void);

/* Admin mode 9 entry intentionally performs no work. */
void CallTerminal(void);

/* Admin mode 9 destroy: no work, zero transition result. */
u32 ExitTerminal(void);

/* Admin mode 9 cleanup: one immediately reports completion. */
u32 CheckTerminal(void);

s32 CallResult2(void);

u32 ExitResult2(void);

u8 CheckResult2(void);

void CallConfig(void);

u32 ExitConfig(void);

u8 CheckConfig(void);

/* Source zero forwards an optional parameter pair; other sources resume deferred field work. */
void evtDispatchSelectionCommand(s32 commandSource, s32 *commandParams);

/* Check battle shutdown; the callee exits only after audio and task work become idle. */
s32 evtCheckBattleShutdown(void);

u32 CheckNewBattleTest(void);

void evtCreateBattleStageTestTask(void);

/* Stop the battle-stage test task and return the scene callback's native zero status. */
u32 evtEndBattleStageTest(void);

/* Admin mode 15 cleanup: zero leaves the mode pending. */
u32 evtKeepBattleStageTestModePending(void);

dds3ProcessID_t *evtCreateTestAndSkyTasks(void);

u32 evtInitializeSelectionScene(void);

/* Admin mode 16 cleanup: zero leaves the mode pending. */
u32 evtKeepTestSkyModePending(void);

void evtClearSecondaryWorldAndViewer(void);

u32 evtInitializeSelectionScreen(void);

/* Admin mode 17 cleanup: zero leaves the mode pending. */
u32 evtKeepEventViewerModePending(void);

/* Admin mode 18 entry intentionally performs no work. */
void evtAdminModeEighteenEntryNoOp(void);

/* Admin mode 18 destroy: no work, zero transition result. */
u32 evtAdminModeEighteenDestroyNoOp(void);

/* Admin mode 18 cleanup: zero leaves the mode pending. */
u32 evtKeepAdminModeEighteenPending(void);

void evtCreateCampTasksAlternate(void);

u32 evtExitCampTaskGroup(void);

u8 mnuCheckCampStateAndAcknowledge(void);

s32 evtCreateSkyAndCampTasks(u32 unused, u32 campMode);

u32 evtDestroySkyAndCampTasks(void);

u8 CheckRecoverTest(void);

s32 CallTerminalTest(u32 unused, u32 *fieldArgs);

u32 ExitTerminalTest(void);

u8 CheckTerminalTest(void);

s32 evtCreateStaffTasksAlternate(void);

u32 evtDestroyStaffTasksAlternate(void);

u8 evtWaitStaffTaskDoneAlternate(void);

/* Admin mode 23 entry intentionally performs no work. */
void CallSoundTest(void);

/* Admin mode 23 destroy: no work, zero transition result. */
u32 ExitSoundTest(void);

/* Admin mode 23 cleanup: zero leaves the mode pending. */
u32 CheckSoundTest(void);

/* Admin mode 24 entry intentionally performs no work. */
void CallBillboardTest(void);

/* Admin mode 24 destroy: no work, zero transition result. */
u32 ExitBillboardTest(void);

/* Admin mode 24 cleanup: zero leaves the mode pending. */
u32 CheckBillboardTest(void);

void CallFontTest(void);

/* Admin mode 25 destroy callback: no work, zero transition result. */
u32 ExitFontTest(void);

u32 CheckFontTest(void);

void evtCreateMovieViewerTask(void);

u32 evtDestroyMovieViewerTask(void);

/* Admin mode 26 cleanup: zero leaves the mode pending. */
u32 evtKeepMovieViewerModePending(void);

void evtEnterInterfaceMode(void);

u32 evtDestroyInterfaceMode(void);

u32 evtKeepInterfaceModePending(void);

void evtSelectFontResource(s32 unused, s32 *lmapArgs);

u32 evtDestroyLmapTask(void);

u8 evtWaitLmapTaskGone(void);

u32 evtEnsureFontResourceChain(void);

void evtDestroyFontResourceChain(void);

void datDebugPrintf(const char *format, ...);

extern u16 D_003BA970;

extern u16 D_003BA972;

extern char evtGeneralHeapPercentFormat[];

s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task);

void evtGeneralHeapGraphTaskDestroy(dds3ProcessID_t *task);

void evtToggleDebugTimeGraphTask(s8 mode);

extern u32 D_003BA97C;

/* Render an allocation span as partial rows and complete heap-map rows. */
void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 blockState);

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);

extern void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);

extern char D_003BA980[];

extern char D_003BA988[];

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(sdfGrOt_t *surface);

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void);

extern s32 frGetFontHandleNum(void);

extern s32 frGetTextHandleNum(void);

extern s32 effGetFontListCount(void);

extern const char D_0039E220[];

s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);

void evtResourceCountDebugTaskDestroy(dds3ProcessID_t *task);

void evtToggleAlternateDebugTimeGraphTask(s8 mode);

/* Append to the event-work doubly linked list, maintaining both endpoints. */
void scrLinkHandle(scrProcessWork_s *node);

/* Detach from either end or the middle, and clear the old links. */
void scrUnlinkHandle(scrProcessWork_s *node);

extern ItfMesWindowIndex itfMesMngInitialize(ItfMesSub *data);

scrProcessWork_s *scrPrintProcedureStart(const scrHeader_t *header, const scrTypeTable_t *procedureSection,
                        const scrProcTable_t *procedures, const scrLabelTable_t *labels,
                        const scrCode_u *instructions, struct ItfMesSub *auxiliaryData,
                        const char *strings, ScrProcedureIndex procedureIndex);

/* Resolve supported FLW0 sections relative to the header; reject bad magic/unknown kinds.
   Zero-count auxiliary sections are ignored, and absent sections remain null. */
scrProcessWork_s *fldFlowParse(const scrHeader_t *header, ScrProcedureIndex procedureIndex);
/* End of declarations from src/dds1/game/code_00107FD8.c */

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitDrawEnv);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitRenderDrawEnv);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001055C8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001058B0);

/* Replacing a render texture releases the old texture first. A texture-creation
 * failure leaves the allocated framebuffer handle tracked, as in retail. */
s32 dds3AllocRenderTexture(u16 sx, u16 sy, f32 aspect)
{
    sdfTexHandle_t *texture;

    if (dds3RenderTexture != NULL) {
        dds3FreeRenderTexture();
    }
    dds3RenderTex = 0;
    dds3RenderTexWidth = sx;
    dds3RenderTexHeight = sy;
    dds3RenderTexHandle = sdfGsAllocFrameBuff(sx, sy, 0);
    if (dds3RenderTexHandle == NULL) {
        return 0;
    }
    texture = sdfTexNew(sx, sy, 0, 0, dds3RenderTexHandle, 0, 0, NULL);
    if (texture == NULL) {
        return 0;
    }
    dds3RenderTexture = texture;
    sdfTexSetWrapMode(texture, 5);
    sdfTexMakeGIFPacket(texture);
    dds3RenderScreenParam.aspect = aspect;
    dds3RenderScreenParam.width = sx;
    dds3RenderScreenParam.height = sy;
    dds3InitRenderDrawEnv();
    return 1;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FreeRenderTexture);

s32 dds3RenderTextureON(void)
{
    s32 result = 0;

    if (dds3RenderTexture == 0) {
        return result;
    }
    dds3RenderTex = 1;
    return 1;
}

void dds3RenderTextureOFF(void)
{
    dds3RenderTex = 0;
}

u32 dds3CheckRenderTexture(void)
{
    return dds3RenderTex;
}

void *dds3GetRenderTexture(void)
{
    return dds3RenderTexture;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RunQuake);

/* Zero amplitude stops the quake; zero duration leaves it running indefinitely. */
void dds3SetQuake(s32 frame, s32 range)
{
    if (range == 0) {
        dds3QuakeFlag = 0;
        return;
    }
    dds3QuakeFlag = 1;
    if (frame == 0) {
        dds3QuakeFrame = -1;
    } else {
        dds3QuakeFrame = SDF_FRAMES(frame);
    }
    dds3QuakeYRange = range * 10;
    dds3QuakeXRange = dds3QuakeYRange * 4 / 10;
}

void dds3StopQuake(void)
{
    dds3QuakeFlag = 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_00105F58);

/* The two live XYZ2 payload locals follow the matched DDS engine source.
 * Creating each before its RGBAQ pair preserves the native hoisted constants. */
void dds3InitZClearDrawTag(void)
{
    s32 state;
    s32 buffer;
    u64 topLeft;
    u64 bottomRight;

    for (state = 0; state < 3; state++) {
        for (buffer = 0; buffer < 2; buffer++) {
            dds3ZClearDrawTag[state][buffer].dma.qwc = 0xA;
            dds3ZClearDrawTag[state][buffer].dma.pad = 0;
            dds3ZClearDrawTag[state][buffer].dma.id = 0x70;
            dds3ZClearDrawTag[state][buffer].dma.addr = NULL;
            dds3ZClearDrawTag[state][buffer].dma.dat0 = 0;
            dds3ZClearDrawTag[state][buffer].dma.dat1 = 0x5000000A;
            dds3ZClearDrawTag[state][buffer].testGif.parm = 0x1000000000000001;
            dds3ZClearDrawTag[state][buffer].testGif.regs = 0xE;
            dds3ZClearDrawTag[state][buffer].test.reg = 0x47;
            dds3ZClearDrawTag[state][buffer].test.data = 0x32001;
            dds3ZClearDrawTag[state][buffer].clearGif.parm = 0x1003400000000004;
            dds3ZClearDrawTag[state][buffer].clearGif.regs = 0xE;
            topLeft = 0x79007000;
            dds3ZClearDrawTag[state][buffer].colorA.reg = 1;
            dds3ZClearDrawTag[state][buffer].colorA.data = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].positionA.reg = 5;
            dds3ZClearDrawTag[state][buffer].positionA.data = topLeft;
            bottomRight = 0x87009000;
            dds3ZClearDrawTag[state][buffer].colorB.reg = 1;
            dds3ZClearDrawTag[state][buffer].colorB.data = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].positionB.reg = 5;
            dds3ZClearDrawTag[state][buffer].positionB.data = bottomRight;
            dds3ZClearDrawTag[state][buffer].blendGif.parm = 0x1000000000008002;
            dds3ZClearDrawTag[state][buffer].blendGif.regs = 0xE;
            dds3ZClearDrawTag[state][buffer].blend[0].reg = 0x47;
            dds3ZClearDrawTag[state][buffer].blend[0].data = 0x5101B;
            dds3ZClearDrawTag[state][buffer].blend[1].reg = 0x42;
            dds3ZClearDrawTag[state][buffer].blend[1].data = 0x44;
        }
    }
}

sdfDrawTag_t *dds3LinkZClearDrawTag(s32 sw)
{
    sdfDrawTag_t *tag = dds3FontReqAllocDrawTag();

    sdfDrawTagLink(tag, &dds3ZClearDrawTag[sw][dds3GetFrameID()]);
    return tag;
}

void dds3FadeClear(void)
{
    dds3DrawFlag &= ~3;
    fadergba[0] = 0;
    fadergba[1] = 0;
    fadergba[2] = 0;
    fadergba[3] = 0;
}

void dds3FadeSet(s32 r, s32 g, s32 b, s32 a)
{
    dds3DrawFlag &= ~3;
    fadergba[0] = r;
    fadergba[1] = g;
    fadergba[2] = b;
    fadergba[3] = a;
}

/* Return the live color address through outColor, rather than copying channels. */
void dds3FadeColorGet(dds3FadeColor_t **outColor) {
    *outColor = &fadergba;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001061A0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001061B8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061F0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061F8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106200);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106270);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001062D8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106340);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001063B0);

s32 dds3FadeCheck(void)
{
    return (dds3DrawFlag & 3) != 0;
}

void dds3FadeUpdate(void)
{
    if (dds3FadeCheck() != 0) {
        if (dds3DrawFlag & 1) {
            fadenow = fadenow - 1;
        } else {
            fadenow = fadenow + 1;
        }
        fadergba[3] = (fadenow << 7) / fademax;
        if (((dds3DrawFlag & 1) && fadenow == 0) || ((dds3DrawFlag & 2) && fadenow == fademax)) {
            fadenow = 0;
            fademax = 0;
            dds3DrawFlag &= ~3;
        }
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawDistRipOverError);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001064B8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001067A8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3BistaIn);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_00106880);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3BistaOut);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_00106940);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AB8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3ChangeBistaMode);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3BistaUpdate);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawBista);

void dds3SetScreenFovy(f32 fovy)
{
    dds3ScreenParam.fovy = fovy;
}

void dds3SetAmbient(sdfVector4_t *ambient)
{
    SDF_VU0_COPYVECTOR(&dds3LightParam.ambient, ambient);
}

void dds3SetFog(s32 *fog)
{
    memcpy(&dds3LightParam.fog, fog, 0x14);
}

void dds3SetLight(void *light, s32 no)
{
    dds3LightParam.parallel[no] = light;
}

void setdraw_xy(s32 *xy)
{
    xy[2] = 512;
    xy[3] = 448;
    xy[1] = 0;
    xy[0] = 0;
}

void dds3SetFilter2Para(s32 blend)
{
    filter2target.blend = blend;
}

void dds3SetFilter2RGBA(u32 rgba)
{
    filter2target.color = rgba;
}

void dds3Filter2Swap(s32 frame)
{
    memcpy(&dds3Filter2Buf, Eve_FilterGetParams(), 0x18);
    filter2swapnow = 0;
    filter2swapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x100000;
        setdraw_xy(filter2target.rect);
        Eve_FilterSetParam(&filter2target);
    } else {
        dds3DrawFlag |= 0x100000;
    }
}

void dds3Filter2On(s32 frame)
{
    filter2abase = 0;
    filter2now = 0;
    filter2atarget = filter2target.rgba[3];
    filter2max = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x80000;
        setdraw_xy(filter2target.rect);
        Eve_FilterSetParam(&filter2target);
        Eve_FilterON();
    } else {
        dds3DrawFlag |= 0x80000;
    }
}

void dds3Filter2Off(s32 frame)
{
    filter2atarget = 0;
    filter2abase = filter2target.rgba[3];
    filter2now = 0;
    filter2max = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x80000;
        dds3DrawFlag &= ~0x100000;
        Eve_FilterOFF();
    } else {
        dds3DrawFlag |= 0x80000;
    }
}

void dds3SetMFilterPara(s32 blend)
{
    mfiltertarget.blend = blend;
}

void dds3SetMFilterRGBA(u32 rgba)
{
    mfiltertarget.color = rgba;
}

void dds3SetMFilterArea(s32 area, s32 posx, s32 posy)
{
    mfiltertarget.area[0] = area;
    mfiltertarget.area[1] = posx;
    mfiltertarget.area[2] = posy;
}

void dds3MFilterSwap(s32 frame)
{
    memcpy(&dds3MFilterBuf, Eve_PartialMultiFilterGetParams(), 0x24);
    mfilterswapnow = 0;
    mfilterswapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x400000;
        Eve_PartialMultiFilterSetParam(&mfiltertarget);
    } else {
        dds3DrawFlag |= 0x400000;
    }
}

void dds3MFilterOn(s32 frame)
{
    mfilterabase = 0;
    mfilternow = 0;
    mfilteratarget = mfiltertarget.rgba[3];
    mfiltermax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x200000;
        Eve_PartialMultiFilterSetParam(&mfiltertarget);
        Eve_PartialMultiFilterON();
    } else {
        dds3DrawFlag |= 0x200000;
    }
}

void dds3MFilterOff(s32 frame)
{
    mfilteratarget = 0;
    mfilterabase = mfiltertarget.rgba[3];
    mfilternow = 0;
    mfiltermax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x200000;
        dds3DrawFlag &= ~0x400000;
        Eve_PartialMultiFilterOFF();
    } else {
        dds3DrawFlag |= 0x200000;
    }
}

void dds3SetBlur(s32 frame, s32 alpha, s32 scale)
{
    if (frame == 0) {
        dds3ScreenBlurAlpha = alpha;
        dds3ScreenBlurScale = scale;
        if (alpha == 0 && scale == 0) {
            dds3ScreenBlurFlag = 0;
        } else {
            dds3ScreenBlurFlag = 1;
        }
        dds3DrawFlag &= ~0x800;
    } else {
        bluralphabase = dds3ScreenBlurAlpha;
        blurscalebase = dds3ScreenBlurScale;
        bluralphatarget = alpha;
        blurscaletarget = scale;
        blurmax = SDF_FRAMES(frame);
        blurnow = 0;
        dds3DrawFlag |= 0x800;
    }
}

void dds3SetBlur2Para(f32 rotate, f32 scale, s32 blend)
{
    blur2target.rotation = rotate;
    blur2target.scale = scale;
    blur2target.blend = blend;
}

void dds3SetBlur2RGBA(u32 rgba)
{
    blur2target.color = rgba;
}

void dds3SetBlur2Area(s32 posx, s32 posy)
{
    blur2target.area[0] = posx;
    blur2target.area[1] = posy;
}

void dds3Blur2Swap(s32 frame)
{
    memcpy(&ripblurtarget.blur2buf, Eve_BlurGetParam(), 0x28);
    blur2swapnow = 0;
    blur2swapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x40000;
        setdraw_xy(blur2target.rect);
        Eve_BlurSetParam(&blur2target);
    } else {
        dds3DrawFlag |= 0x40000;
    }
}

void dds3Blur2On(s32 frame)
{
    blur2abase = 0;
    blur2now = 0;
    blur2atarget = blur2target.rgba[3];
    blur2max = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x20000;
        setdraw_xy(blur2target.rect);
        Eve_BlurSetParam(&blur2target);
        Eve_BlurON();
    } else {
        dds3DrawFlag |= 0x20000;
    }
}

void dds3Blur2Off(s32 frame)
{
    blur2atarget = 0;
    blur2abase = blur2target.rgba[3];
    blur2now = 0;
    blur2max = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x20000;
        dds3DrawFlag &= ~0x40000;
        Eve_BlurOFF();
    } else {
        dds3DrawFlag |= 0x20000;
    }
}

void dds3SetMultiBlurPara(f32 rotate, f32 scale, s32 blend)
{
    mulblurtarget.rotation = rotate;
    mulblurtarget.scale = scale;
    mulblurtarget.blend = blend;
}

void dds3SetMultiBlurRGBA(u32 rgba)
{
    mulblurtarget.color = rgba;
}

void dds3SetMultiBlurArea(s32 area, s32 posx, s32 posy)
{
    mulblurtarget.extent = area;
    mulblurtarget.x = posx;
    mulblurtarget.y = posy;
}

void dds3MultiBlurSwap(s32 frame)
{
    memcpy(&dds3MultiBlurBuf, Eve_PartialMultiBlurGetParams(), 0x2C);
    mulblurswapnow = 0;
    mulblurswapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x8000;
        Eve_PartialMultiBlurSetParam(&mulblurtarget);
    } else {
        dds3DrawFlag |= 0x8000;
    }
}

void dds3MultiBlurOn(s32 frame)
{
    mulblurabase = 0;
    mulblurnow = 0;
    mulbluratarget = mulblurtarget.rgba[3];
    mulblurmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x1000;
        Eve_PartialMultiBlurSetParam(&mulblurtarget);
        Eve_PartialMultiBlurON();
    } else {
        dds3DrawFlag |= 0x1000;
    }
}

void dds3MultiBlurOff(s32 frame)
{
    mulbluratarget = 0;
    mulblurnow = 0;
    mulblurabase = mulblurtarget.rgba[3];
    mulblurmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x1000;
        dds3DrawFlag &= ~0x8000;
        Eve_PartialMultiBlurSetParam(&mulblurtarget);
        Eve_PartialMultiBlurOFF();
    } else {
        dds3DrawFlag |= 0x1000;
    }
}

void dds3SetDistBlurPara(s32 number, s32 width, s32 intval, f32 rotate,
                         f32 distor, f32 blend, s32 speed)
{
    if (number >= 101)
{
        distblurovercnt++;
        number = 100;
    }
    distblurtarget.count = number;
    distblurtarget.size = width;
    distblurtarget.delaySpread = intval;
    distblurtarget.angleStep = rotate;
    distblurtarget.uvAngle = distor;
    distblurtarget.uvAmplitude = blend;
    distblurtarget.blend = speed;
}

void dds3SetDistBlurRGBA(u32 rgba)
{
    distblurtarget.color = rgba;
}

void dds3SetDistBlurArea(s32 area, s32 posx, s32 posy)
{
    distblurtarget.spread = area;
    distblurtarget.x = posx;
    distblurtarget.y = posy;
}

void dds3DistBlurSwap(s32 frame)
{
    memcpy(&dds3DistBlurBuf, Eve_DistortionBlurGetParams(), 0x2C);
    distblurswapnow = 0;
    distblurswapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x10000;
        Eve_DistortionBlurSetParam(&distblurtarget);
    } else {
        dds3DrawFlag |= 0x10000;
    }
}

void dds3DistBlurOn(s32 frame)
{
    distblurabase = 0;
    distblurnow = 0;
    distbluratarget = distblurtarget.rgba[3];
    distblurmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x2000;
        Eve_DistortionBlurSetParam(&distblurtarget);
        Eve_DistortionBlurON();
    } else {
        dds3DrawFlag |= 0x2000;
        Eve_DistortionBlurSetParam(&distblurtarget);
        Eve_DistortionBlurOFF();
    }
}

void dds3DistBlurOff(s32 frame)
{
    distbluratarget = 0;
    distblurabase = distblurtarget.rgba[3];
    distblurnow = 0;
    distblurmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x2000;
        dds3DrawFlag &= ~0x10000;
        Eve_DistortionBlurOFF();
    } else {
        dds3DrawFlag |= 0x2000;
    }
}

void dds3SetRipBlurPara(s32 number, f32 intval, f32 rotate, f32 distor, f32 distor_s, f32 blend, s32 speed)
{
    if (number >= 0x29) {
        ripblurovercnt++;
        number = 0x28;
    }
    ripblurtarget.ripple.count = number;
    ripblurtarget.ripple.phaseStep = intval;
    ripblurtarget.ripple.spacing = rotate;
    ripblurtarget.ripple.uvAngle = distor;
    ripblurtarget.ripple.uvAmplitude = distor_s;
    ripblurtarget.ripple.angleStep = blend;
    ripblurtarget.ripple.blend = speed;
}

void dds3SetRipBlurRGBA(u32 rgba)
{
    ripblurtarget.ripple.color = rgba;
}

void dds3SetRipBlurArea(s32 area, s32 posx, s32 posy)
{
    ripblurtarget.ripple.area = area;
    ripblurtarget.ripple.x = posx;
    ripblurtarget.ripple.y = posy;
}

void dds3RipBlurSwap(s32 frame)
{
    memcpy(&dds3RipBlurBuf, Eve_RippleBlurGetParams(), 0x2C);
    ripblurswapnow = 0;
    ripblurswapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x1000000;
        Eve_RippleBlurSetParam(&ripblurtarget);
    } else {
        dds3DrawFlag |= 0x1000000;
    }
}

void dds3RipBlurOn(s32 frame)
{
    ripblurabase = 0;
    ripblurnow = 0;
    ripbluratarget = ripblurtarget.ripple.rgba[3];
    ripblurmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x800000;
        Eve_RippleBlurSetParam(&ripblurtarget);
        Eve_RippleBlurON();
    } else {
        dds3DrawFlag |= 0x800000;
        Eve_RippleBlurSetParam(&ripblurtarget);
        Eve_RippleBlurOFF();
    }
}

void dds3RipBlurOff(s32 frame)
{
    ripbluratarget = 0;
    ripblurabase = ripblurtarget.ripple.rgba[3];
    ripblurnow = 0;
    ripblurmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x800000;
        dds3DrawFlag &= ~0x1000000;
        Eve_RippleBlurOFF();
    } else {
        dds3DrawFlag |= 0x800000;
    }
}

void dds3BlurUpdate(void)
{
    s32 alpha;

    if (dds3DrawFlag & 0x800) {
        blurnow++;
        dds3ScreenBlurAlpha = bluralphabase + blurnow * (bluralphatarget - bluralphabase) / blurmax;
        dds3ScreenBlurScale = blurscalebase + blurnow * (blurscaletarget - blurscalebase) / blurmax;
        if (dds3ScreenBlurAlpha == 0 && dds3ScreenBlurScale == 0) {
            dds3ScreenBlurFlag = 0;
        }
        else {
            dds3ScreenBlurFlag = 1;
        }
        if (blurnow >= blurmax) {
            dds3DrawFlag &= 0xfffff7ff;
        }
    }
    if (dds3DrawFlag & 0x40000) {
        if (blur2swapnow < blur2swapmax) {
            blur2swapnow++;
        }
        evtHokanBlur2(1, (f32)blur2swapnow / (f32)blur2swapmax, &ripblurtarget.blur2buf,
                        &blur2target, &dds3Blur2State);
        setdraw_xy(dds3Blur2State.rect);
        Eve_BlurSetParam(&dds3Blur2State);
        if (dds3Blur2State.rgba[3] != 0) {
            Eve_BlurON();
        }
        else {
            Eve_BlurOFF();
        }
        if (blur2swapnow >= blur2swapmax) {
            dds3DrawFlag &= ~0x40000;
        }
    }
    if (dds3DrawFlag & 0x20000) {
        if (blur2now < blur2max) {
            blur2now++;
        }
        dds3Blur2State = blur2target;
        alpha = blur2abase + blur2now *
                (blur2atarget - blur2abase) /
                blur2max;
        dds3Blur2State.color = (dds3Blur2State.color & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3Blur2State.rect);
        Eve_BlurSetParam(&dds3Blur2State);
        if (alpha == 0) {
            Eve_BlurOFF();
        }
        else {
            Eve_BlurON();
        }
        if (blur2now >= blur2max) {
            dds3DrawFlag &= ~0x20000;
        }
    }
    if (dds3DrawFlag & 0x8000) {
        if (mulblurswapnow < mulblurswapmax) {
            mulblurswapnow++;
        }
        evtHokanMultBlur(1, (f32)mulblurswapnow / (f32)mulblurswapmax, &dds3MultiBlurBuf,
                        &mulblurtarget, &key2);
        Eve_PartialMultiBlurSetParam(&key2);
        if (key2.rgba[3] != 0) {
            Eve_PartialMultiBlurON();
        }
        else {
            Eve_PartialMultiBlurOFF();
        }
        if (mulblurswapnow >= mulblurswapmax) {
            dds3DrawFlag &= ~0x8000;
        }
    }
    if (dds3DrawFlag & 0x1000) {
        if (mulblurnow < mulblurmax) {
            mulblurnow++;
        }
        key2 = mulblurtarget;
        alpha = mulblurabase + mulblurnow *
                (mulbluratarget - mulblurabase) /
                mulblurmax;
        key2.color = (key2.color & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiBlurSetParam(&key2);
        if (alpha == 0) {
            Eve_PartialMultiBlurOFF();
        }
        else {
            Eve_PartialMultiBlurON();
        }
        if (mulblurnow >= mulblurmax) {
            dds3DrawFlag &= ~0x1000;
        }
    }
    if (dds3DrawFlag & 0x10000) {
        if (distblurswapnow < distblurswapmax) {
            distblurswapnow++;
        }
        evtHokanDistBlur(1, (f32)distblurswapnow / (f32)distblurswapmax, &dds3DistBlurBuf,
                        &distblurtarget, &dds3DistBlurCurrent);
        Eve_DistortionBlurSetParam(&dds3DistBlurCurrent);
        if (dds3DistBlurCurrent.rgba[3] != 0) {
            Eve_DistortionBlurON();
        }
        else {
            Eve_DistortionBlurOFF();
        }
        if (distblurswapnow >= distblurswapmax) {
            dds3DrawFlag &= ~0x10000;
        }
    }
    if (dds3DrawFlag & 0x2000) {
        if (distblurnow < distblurmax) {
            distblurnow++;
        }
        dds3DistBlurCurrent = distblurtarget;
        alpha = distblurabase + distblurnow *
                (distbluratarget - distblurabase) /
                distblurmax;
        dds3DistBlurCurrent.color = (dds3DistBlurCurrent.color & 0xFFFFFF) | (alpha << 24);
        Eve_DistortionBlurSetParam(&dds3DistBlurCurrent);
        if (alpha == 0) {
            Eve_DistortionBlurOFF();
        }
        else {
            Eve_DistortionBlurON();
        }
        if (distblurnow >= distblurmax) {
            dds3DrawFlag &= ~0x2000;
        }
    }
    if (dds3DrawFlag & 0x1000000) {
        if (ripblurswapnow < ripblurswapmax) {
            ripblurswapnow++;
        }
        evtHokanRipBlur(1, (f32)ripblurswapnow / (f32)ripblurswapmax, &dds3RipBlurBuf,
                        &ripblurtarget.ripple, &dds3BlurState.ripple);
        Eve_RippleBlurSetParam(&dds3BlurState.ripple);
        if (dds3BlurState.ripple.rgba[3] != 0) {
            Eve_RippleBlurON();
        }
        else {
            Eve_RippleBlurOFF();
        }
        if (ripblurswapnow >= ripblurswapmax) {
            dds3DrawFlag &= ~0x1000000;
        }
    }
    if (dds3DrawFlag & 0x800000) {
        if (ripblurnow < ripblurmax) {
            ripblurnow++;
        }
        dds3BlurState.ripple = ripblurtarget.ripple;
        alpha = ripblurabase + ripblurnow *
                (ripbluratarget - ripblurabase) /
                ripblurmax;
        dds3BlurState.ripple.color = (dds3BlurState.ripple.color & 0xFFFFFF) | (alpha << 24);
        Eve_RippleBlurSetParam(&dds3BlurState.ripple);
        if (alpha == 0) {
            Eve_RippleBlurOFF();
        }
        else {
            Eve_RippleBlurON();
        }
        if (ripblurnow >= ripblurmax) {
            dds3DrawFlag &= ~0x800000;
        }
    }
    if (dds3DrawFlag & 0x100000) {
        if (filter2swapnow < filter2swapmax) {
            filter2swapnow++;
        }
        evtHokanFilter(1, (f32)filter2swapnow / (f32)filter2swapmax, &dds3Filter2Buf,
                                        &filter2target, &dds3BlurParam);
        setdraw_xy(dds3BlurParam.rect);
        Eve_FilterSetParam(&dds3BlurParam);
        if (dds3BlurParam.rgba[3] != 0) {
            Eve_FilterON();
        }
        else {
            Eve_FilterOFF();
        }
        if (filter2swapnow >= filter2swapmax) {
            dds3DrawFlag &= ~0x100000;
        }
    }
    if (dds3DrawFlag & 0x80000) {
        if (filter2now < filter2max) {
            filter2now++;
        }
        dds3BlurParam = filter2target;
        alpha = filter2abase + filter2now *
                (filter2atarget - filter2abase) /
                filter2max;
        dds3BlurParam.color = (dds3BlurParam.color & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3BlurParam.rect);
        Eve_FilterSetParam(&dds3BlurParam);
        if (alpha == 0) {
            Eve_FilterOFF();
        }
        else {
            Eve_FilterON();
        }
        if (filter2now >= filter2max) {
            dds3DrawFlag &= ~0x80000;
        }
    }
    if (dds3DrawFlag & 0x400000) {
        if (mfilterswapnow < mfilterswapmax) {
            mfilterswapnow++;
        }
        evtHokanMultFilter(1, (f32)mfilterswapnow / (f32)mfilterswapmax, &dds3MFilterBuf,
                        &mfiltertarget, &dds3MFilterCurrent);
        Eve_PartialMultiFilterSetParam(&dds3MFilterCurrent);
        if (dds3MFilterCurrent.rgba[3] != 0) {
            Eve_PartialMultiFilterON();
        }
        else {
            Eve_PartialMultiFilterOFF();
        }
        if (mfilterswapnow >= mfilterswapmax) {
            dds3DrawFlag &= ~0x400000;
        }
    }
    if (dds3DrawFlag & 0x200000) {
        if (mfilternow < mfiltermax) {
            mfilternow++;
        }
        dds3MFilterCurrent = mfiltertarget;
        alpha = mfilterabase + mfilternow *
                (mfilteratarget - mfilterabase) /
                mfiltermax;
        dds3MFilterCurrent.color = (dds3MFilterCurrent.color & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiFilterSetParam(&dds3MFilterCurrent);
        if (alpha == 0) {
            Eve_PartialMultiFilterOFF();
        }
        else {
            Eve_PartialMultiFilterON();
        }
        if (mfilternow >= mfiltermax) {
            dds3DrawFlag &= ~0x200000;
        }
    }
}

/* vu0 routine: shortest-arc quaternion rotating VF10 to VF11. */
void dds3RotationArc(void)
{
    sdfVector4_t identity;
    sdfVector4_t axis;
    f32 dot;
    f32 component;

    memset(&identity, 0, sizeof(identity));
    identity.w = 1.0f;

    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SWAPVECTOR_A_B();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SWAPVECTOR_A_B();

    SDF_VU0_INNERPRODUCT(dot);
    if (dot >= 0.9999f) {
        SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)&identity);
        return;
    }

    if (dot <= -0.9999f) {
        SDF_VU0_GETVECTOR_Y(component);
        if (0.9999f <= fabsf(component)) {
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_SETVECTOR_Z(-1.0f);
        } else {
            SDF_VU0_GETVECTOR_Z(component);
            axis.x = -component;
            axis.y = 0.0f;
            SDF_VU0_GETVECTOR_X(axis.z);
            axis.w = 0.0f;
            SDF_VU0_LOADVECTOR(&axis);
            SDF_VU0_NORMALIZE();
        }

        SDF_VU0_INNERPRODUCT(dot);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_SETVECTOR_W(dot);
        return;
    }

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_INNERPRODUCT(dot);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(&axis);
    SDF_VU0_SETVECTOR_W(dot);
    Normalize();
}

/* vu0 routine: stage an immediate or interpolated parallel light color. */
void dds3SetLightColor(s32 frame, s32 no, f32 *col)
{
    sdf3DLight_t *light = dds3LightParam.parallel[no];
    u32 baseColorWords[4];
    u32 targetColorWords[4];
    u32 packedBaseColor;
    u32 packedTargetColor;
    u32 blendFlag;

    if (light != NULL) {
        blendFlag = 4 << (no * 2);
        if (frame == 0) {
            dds3DrawFlag &= ~blendFlag;
            SDF_VU0_LOADVECTOR(col);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_STOREVECTOR(&dds3LightParam.parallel[no]->color);
        } else {
            lightcolormax[no] = SDF_FRAMES(frame);
            lightcolornow[no] = 0;
            dds3DrawFlag |= blendFlag;
            SDF_VU0_LOADVECTOR(light);
            MMI_RGBA_PACK_128(packedBaseColor);
            baseColorWords[0] = packedBaseColor;
            dds3LightColor[no] = packedBaseColor;
            SDF_VU0_LOADVECTOR(col);
            SDF_VU0_VECTOR_W_SET_ZERO();
            MMI_RGBA_PACK_128(packedTargetColor);
            dds3LightTargetColor[no] = packedTargetColor;
            targetColorWords[0] = packedTargetColor;
        }
    }
}

void dds3SetLightDir(s32 frame, s32 no, f32 *dir)
{
    u32 blendFlag;

    if (dds3LightParam.parallel[no] != NULL) {
        blendFlag = 8 << (no * 2);
        if (frame == 0) {
            dds3DrawFlag &= ~blendFlag;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_NORMALIZE();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_STOREVECTOR(&dds3LightParam.parallel[no]->v);
        } else {
            lightdirmax[no] = SDF_FRAMES(frame);
            lightdirnow[no] = 0;
            dds3DrawFlag |= blendFlag;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[no]->v);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_NORMALIZE();
            /* No memory clobber here: retail recomputes the slot offset after the call. */
            SDF_VU0_STOREVECTOR_NOMEM(dds3LightArcFrom[no]);
            dds3RotationArc();
            SDF_VU0_STOREVECTOR(dds3LightArcTo[no]);
        }
    }
}

/* vu0 routine: stage an immediate or interpolated ambient color. */
void dds3SetAmbientColor(s32 frame, f32 *col)
{
    u32 baseColorWords[4];
    u32 targetColorWords[4];
    u32 packedColor;

    if (frame == 0) {
        dds3DrawFlag &= ~0x100;
        SDF_VU0_LOADVECTOR(col);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&dds3LightParam.ambient);
    } else {
        ambientmax = SDF_FRAMES(frame);
        ambientnow = 0;
        dds3DrawFlag |= 0x100;
        SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
        MMI_RGBA_PACK_128(packedColor);
        baseColorWords[0] = packedColor;
        ambientbase = packedColor;
        SDF_VU0_LOADVECTOR(col);
        SDF_VU0_VECTOR_W_SET_ONE();
        MMI_RGBA_PACK_128(packedColor);
        targetColorWords[0] = packedColor;
        ambienttarget = packedColor;
    }
}

/* vu0 routine: stage an immediate or interpolated packed color. */
void dds3SetFogColor(s32 frame, f32 *col)
{
    u32 targetColorWords[4];
    u32 packedTargetColor;

    SDF_VU0_LOADVECTOR(col);
    SDF_VU0_VECTOR_W_SET_ZERO();
    MMI_RGBA_PACK_255(packedTargetColor);
    targetColorWords[0] = packedTargetColor;
    if (frame == 0) {
        dds3DrawFlag &= ~0x200;
        dds3LightParam.fog.color = targetColorWords[0];
    } else {
        dds3DrawFlag |= 0x200;
        fogrgbmax = SDF_FRAMES(frame);
        fogrgbtarget = targetColorWords[0];
        fogrgbbase = dds3LightParam.fog.color;
        fogrgbnow = 0;
    }
}

void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far)
{
    if (frame == 0) {
        dds3DrawFlag &= ~0x400;
        dds3FogState.fogMax = max;
        dds3FogState.fogMin = min;
        dds3FogState.fogNear = near;
        dds3FogState.fogFar = far;
    } else {
        f32 oldMax = dds3FogState.fogMax;
        f32 oldMin = dds3FogState.fogMin;
        f32 oldNear = dds3FogState.fogNear;
        f32 oldFar = dds3FogState.fogFar;
        dds3DrawFlag |= 0x400;
        fogparamax = SDF_FRAMES(frame);
        dds3FogParam.fogMax = oldMax;
        dds3FogParam.fogMin = oldMin;
        dds3FogParam.fogNear = oldNear;
        dds3FogParam.fogFar = oldFar;
        dds3FogColor.fogMax = max;
        dds3FogColor.fogMin = min;
        dds3FogColor.fogNear = near;
        dds3FogColor.fogFar = far;
        fogparanow = 0;
    }
}

/* vu0 routine: advance staged light, ambient, fog and face divide blends. */
void dds3SceneLightUpdateFirst(void) {
    s32 i;
    u32 mask;
    f32 ratio;

    for (i = 0, mask = 4; i < 3; i++, mask <<= 2) {
        if (dds3LightParam.parallel[i] == NULL) {
            dds3DrawFlag &= ~(mask | (mask << 1));
        } else {
            if (dds3DrawFlag & mask) {
                ++lightcolornow[i];
                ratio = (f32)lightcolornow[i] / lightcolormax[i];
                dds3UnpackLightColor(dds3LightColor[i], 0.0078125f);
                SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
                SDF_VU0_COPYVECTOR_A_TO_B();
                dds3UnpackLightColor(dds3LightTargetColor[i], 0.0078125f);
                SDF_VU0_SCALEVECTOR_V0(ratio);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(dds3LightParam.parallel[i]);
                if (lightcolornow[i] >= lightcolormax[i]) {
                    dds3DrawFlag &= ~mask;
                }
            }
            if (dds3DrawFlag & (mask << 1)) {
                ++lightdirnow[i];
                ratio = (f32)lightdirnow[i] / lightdirmax[i];
                SDF_VU0_LOADIDENTITYVECTOR();
                SDF_VU0_VECTOR_W_SET_ONE();
                SDF_VU0_LOADVECTOR_B(&dds3LightArcTo[i]);
                Lerp(ratio);
                ToMatrix();
                SDF_VU0_LOADVECTOR(&dds3LightArcFrom[i]);
                SDF_VU0_APPLYMATRIX();
                SDF_VU0_STOREVECTOR_NOMEM(&dds3LightParam.parallel[i]->v);
                if (lightdirnow[i] >= lightdirmax[i]) {
                    dds3DrawFlag &= ~(mask << 1);
                }
            }
        }
    }
    if (dds3DrawFlag & 0x100) {
        ++ambientnow;
        ratio = (f32)ambientnow / ambientmax;
        dds3UnpackLightColor(ambientbase, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        dds3UnpackLightColor(ambienttarget, 0.0078125f);
        SDF_VU0_SCALEVECTOR_V0(ratio);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR_NOMEM(&dds3AmbientColor);
        if (ambientnow >= ambientmax) {
            dds3DrawFlag &= ~0x100;
        }
    }
    if (dds3DrawFlag & 0x200) {
        ++fogrgbnow;
        ratio = (f32)fogrgbnow / fogrgbmax;
        dds3UnpackLightColor(fogrgbbase, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        dds3UnpackLightColor(fogrgbtarget, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(ratio);
        SDF_VU0_ADDVECTOR();
        {
            u32 rgba[4] __attribute__((aligned(16)));
            MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
            dds3FogState.color = rgba[0];
        }
        if (fogrgbnow >= fogrgbmax) {
            dds3DrawFlag &= ~0x200;
        }
    }
    if (dds3DrawFlag & 0x400) {
        ++fogparanow;
        ratio = (f32)fogparanow / fogparamax;
        SDF_VU0_LOADVECTOR(&dds3FogParam);
        SDF_VU0_LOADVECTOR_B(&dds3FogColor);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_SCALEVECTOR_B_V0(ratio);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(dds3FogState.fogMax);
        SDF_VU0_GETVECTOR_Y(dds3FogState.fogMin);
        SDF_VU0_GETVECTOR_Z(dds3FogState.fogNear);
        SDF_VU0_GETVECTOR_W(dds3FogState.fogFar);
        if (fogparanow >= fogparamax) {
            dds3DrawFlag &= ~0x400;
        }
    }
    if (dds3DrawFlag & 0x4000) {
        ++sbnow;
        ratio = (f32)sbnow / sbmax;
        sdfFaceDivSoften = softenbase * (1.0f - ratio) + softentarget * ratio;
        sdfFaceDivBias = biasbase * (1.0f - ratio) + biastarget * ratio;
        if (sbnow >= sbmax) {
            dds3DrawFlag &= ~0x4000;
        }
    }
}

void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias)
{
    if (frame == 0) {
        dds3DrawFlag &= ~0x4000;
        sdfFaceDivSoften = soften;
        sdfFaceDivBias = bias;
    } else {
        f32 psoften = sdfFaceDivSoften;
        f32 pbias = sdfFaceDivBias;
        dds3DrawFlag |= 0x4000;
        sbmax = SDF_FRAMES(frame);
        softenbase = psoften;
        softentarget = soften;
        biasbase = pbias;
        biastarget = bias;
        sbnow = 0;
    }
}

void eglInit(void)
{
    if (eglMaterial == NULL) {
        eglMaterial = sdfMatNew();
        eglMaterial->constant = 1.0f;
    }
}

void eglSetOt(u32 ot)
{
    egl_OT_Offset = ot;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001093D0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001094E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkTest);

/* Submit ALPHA_1 = (Cs - Cd) * FIX / 128 + Cd; retain the native unmasked FIX input. */
void evtSubmitFixedAlphaBlend(s32 fixedAlpha) {
    void *list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdf2DMakeStdPacket(packet, EVT_GS_COMMAND_BYTES);
    command->data = ((u64)fixedAlpha << 32) | EVT_FIXED_ALPHA_INTERPOLATION;
    command->registerId = EVT_GS_ALPHA_PRIMARY;
    sdfDrawTagLink(list, (u32)(packet));
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_001097D8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_00109870);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_r1_00109908);
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B238);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B23C);

u32 dds3RenderTex = 0;

sdfGsMemSlice_t *dds3RenderTexHandle = NULL;

sdfTexHandle_t *dds3RenderTexture = NULL;

u16 dds3RenderTexWidth = 0x80;

u16 dds3RenderTexHeight = 0x80;

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B250);

u32 dds3DrawFlag = 0;

u8 dds3ScreenBlurFlag = 0;

s16 dds3ScreenBlurAlpha = 0x60;

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B25C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B260);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B264);

u16 dds3QuakeFlag = 0;

s16 dds3QuakeFrame = 0;

u16 dds3QuakeXRange = 0;

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B26E);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", Zoom);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B278);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B27B);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B27C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE37D);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B280);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B283);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B284);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B286);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B288);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B28A);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_r1_0046B28C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE390);

u32 distblurovercnt = 0;

u32 ripblurovercnt = 0;

