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

/* Declarations from dds-decomp src/dds1/game/code_00107FD8.c */
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

#include "event.h"

#include "scr.h"

#include "sdf_packet_alloc.h"

#include "sdk_libc.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_task_priority_query.h"

#include "dds/fld_scene_sequence_api.h"

#include "dds/kwln.h"

#include "dds/mnu_camp_state_ack_api.h"

#include "dds/scr.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_dev_cons.h"

#include "dds/sdf_dev_console_node.h"

#include "dds/sdf_general_heap_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

s32 DIntr();

s32 EIntr(void);

/* Consume CLEANED_UP back to IDLE, then report whether camp remains active. */
MnuCampStateAcknowledgementResult campCheckExitState(void);

/* Set bit 1 in the active administration task's flags; that task must exist. */
void dds3AdminiExitSequence(void);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

/* Returns a borrowed task from the scheduler queues, or NULL when none has this priority. */
dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* The packet is borrowed for this call. Mode zero copies its fields into
 * persistent scene work before asynchronous tasks start. */
void fldProcTrigSequence(const struct fldSceneParam_t *record, u32 mode);

extern scrProcessWork_s *scrHandleFirst;

extern scrProcessWork_s *scrHandleLast;

void *sdfChipAllocDirect(s32 size);

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

void sdfPrimLinkLine(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkRect(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

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

extern s32 dds3TaskNoopCallback(dds3ProcessID_t *task);

extern void fldMainLoadStart(void);

extern void btlStart(s32 mode, s32 pack, s32 encounter);

extern void evtEventViewerDestroyTask(void);

extern void evtChkEndProcess(void);

extern void evtExitSky(void);

#include "dds/sdf_asset_create.h"

extern s8 evtSelectionStateActive;

extern EvtSelState *evtSelectionState;

extern u8 sdfViewShakeVector[];

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

extern sdfGrOt_t sdfDebugGrOt;

extern s8 D_0032453B[];

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

extern f32 *D_00324770[];

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

extern u8 kwlnDefaultColorVector[];

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

/* Submit primary-context GS TEST settings. Z testing is always enabled;
   unusedZte is a retained native formal, not the source of the ZTE bit. */
void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

/* Submit the same TEST bit layout for the secondary GS context, with ZTE forced on. */
void evtSubmitSecondaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

/* Select a primary-context ALPHA equation by mode; unknown modes use 0x44. */
void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);

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

/* Submit the four-corner gradient rectangle with the native default depth.
   The inserted 0xFFFFFF is depth, not a white color. */
void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);

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

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

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

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010A8C8);

/* Submit the four-corner gradient rectangle with the native default depth.
   The inserted 0xFFFFFF is depth, not a white color. */
void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor) {
    evtSubmitGradientRectAtDepth(x, y, width, height, EVT_QUAD_DEFAULT_DEPTH, topLeftColor, topRightColor, bottomRightColor, bottomLeftColor);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010ABC0);

void eglLinkTri(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 rgba0, s32 rgba1, s32 rgba2)
{
    s32 coords[3][2];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    dds3GouraudRegister_t *registers;
    s32 i;
    sdfGrOt_t *descriptor;

    u32 r0 = rgba0 & 0xFF;
    u32 g0 = (rgba0 >> 8) & 0xFF;
    u32 b0 = (rgba0 >> 16) & 0xFF;
    u32 a0 = (u32)rgba0 >> 24;
    u32 r1 = rgba1 & 0xFF;
    u32 g1 = (rgba1 >> 8) & 0xFF;
    u32 b1 = (rgba1 >> 16) & 0xFF;
    u32 a1 = (u32)rgba1 >> 24;
    u32 r2 = rgba2 & 0xFF;
    u32 g2 = (rgba2 >> 8) & 0xFF;
    u32 b2 = (rgba2 >> 16) & 0xFF;
    u32 a2 = (u32)rgba2 >> 24;
    coords[0][0] = x0 * 16;
    coords[0][1] = y0 * 8;
    coords[1][0] = x1 * 16;
    coords[1][1] = y1 * 8;
    coords[2][0] = x2 * 16;
    coords[2][1] = y2 * 8;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 3));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 3);
    registers = (dds3GouraudRegister_t *)sdf2DGetPrimPtr(packet);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            registers->low = (u64)r0 | ((u64)g0 << 32);
            registers->high = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            registers->low = (u64)r1 | ((u64)g1 << 32);
            registers->high = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            registers->low = (u64)r2 | ((u64)g2 << 32);
            registers->high = (u64)b2 | ((u64)a2 << 32);
        }
        registers++;
        registers->high = 0xFFFFFF;
        registers->low = (u64)(u32)(coords[i][0] + 0x7000) |
            ((u64)(coords[i][1] + 0x7900) << 32);
        registers++;
    }
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010B0E0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010B350);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010B4D8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010B538);

/* Preserve native vertex order 0,2,3,1; W components are not initialized. */
void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 color0, u32 color1, u32 color2, u32 color3)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    void *list;
    sdfGrOt_t *surface;

    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, eglMaterial, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = eglPolyStripWord[0];
    sdfDrawTagLink(list, sdf3DPrimMakePoly(&desc));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010B878);

/* Preserve native vertex order 0,2,3,1; W components are not initialized. */
void eglLinkTexPoly4(s32 color0, f32 x0, f32 y0, f32 z0, s32 color1, f32 x1, f32 y1, f32 z1, s32 color2, f32 x2, f32 y2, f32 z2, s32 color3, f32 x3, f32 y3, f32 z3, sdfTexHandle_t *texture, f32 u0, f32 v0, f32 u1, f32 v1)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    sdfVector2_t uvs[4];
    sdfMat_t *asset;
    void *list;
    sdfGrOt_t *surface;

    asset = sdfMatNew();
    sdfMatSetTex(asset, texture);
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, asset, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    desc.uv = uvs;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = eglPolyStripWord[0];
    uvs[0].x = u0;
    uvs[0].y = v0;
    uvs[1].x = u0;
    uvs[1].y = v1;
    uvs[2].x = u1;
    uvs[2].y = v1;
    uvs[3].x = u1;
    uvs[3].y = v0;
    sdfDrawTagLink(list, sdf3DPrimMakePoly(&desc));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    sdfMatFreeDelayed(asset);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010BC70);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010BE98);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010BED8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010BEF0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", func_0010C0F8);

void eglLinkLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 color0, u32 color1)
{
    void *list;
    eglLineVerts[0].x = x0;
    eglLineVerts[0].y = y0;
    eglLineVerts[0].z = z0;
    eglLineVerts[1].x = x1;
    eglLineVerts[1].y = y1;
    eglLineVerts[1].z = z1;
    eglLineColors[1] = color1;
    eglLineColors[0] = color0;
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, effDebugDrawWireModel(eglLineVerts, eglLineColors, 2, 0x80));
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", eglLinkPrint);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw_0010A8C8", eglLinkKanji);
