#include "common.h"
#include "draw.h"
#include "draw3d.h"
#include "kernel_draw.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdfDraw.h"
#include "sdfMat.h"
#include "dds3KernelDraw.h"
#include "sdfTex.h"
#include "sdfPacketBuff.h"
#include "sdfQuat.h"
#include "sdfGsMemory.h"
#include "sdk_libm.h"
#include "effEvent.h"
#include "evtPolygonMovie.h"
#include "sdf2D.h"
#include "dds3Randomize.h"
#include "dds3KernelMain.h"
#include "sdfPrim.h"
#include "code_r1_00104370.h"
#include "sdfDevCons.h"
#include "sdfGraph.h"

sdf3DLight_t mdlData3 = {{1.0f, 1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}};
sdf3DLight_t mdlData4 = {{0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}};
sdf3DLight_t dds3RefCameraTMData = {{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}};
dds3LightState_t dds3LightParam = {{&mdlData3, &mdlData4, &dds3RefCameraTMData, NULL},
                                   {0.2f, 0.2f, 0.2f, 1.0f},
                                   {255.0f, 255.0f, 3000.0f, 20000.0f, 0x00605040}};


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

extern sdfFrameU16_t fadenow;
extern sdfFrameU16_t fademax;

typedef struct
{
    f32 fogMax;
    f32 fogMin;
    f32 fogNear;
    f32 fogFar;
} dds3FogRange_t;

extern sdf3DFogParam_t dds3FogState;
extern dds3Effect_Filter_t dds3Filter2Buf;
extern dds3Effect_Filter_t filter2target;
extern u16 filter2now;
extern u16 filter2max;
extern u16 filter2abase;
extern u16 filter2atarget;
extern u16 filter2swapnow;
extern u16 filter2swapmax;

extern dds3Effect_PartialMultiFilterParam_t dds3MFilterBuf;
extern dds3Effect_PartialMultiFilterParam_t mfiltertarget;
extern u16 mfilternow;
extern u16 mfiltermax;
extern u16 mfilterabase;
extern u16 mfilteratarget;
extern u16 mfilterswapnow;
extern u16 mfilterswapmax;

/* The ripple copies move as doublewords. */
typedef dds3Effect_RippleBlurParam_t dds3RipBlurBody_t __attribute__((aligned(8)));
typedef struct dds3RipBlurParam {
    /* 0x00 */ dds3RipBlurBody_t ripple;
    /* 0x2C */ u32 unk2C;
    /* 0x30 */ dds3Effect_Blur_t blur2buf;
} dds3RipBlurParam_t;

extern dds3RipBlurParam_t ripblurtarget;

extern dds3Effect_PartialMultiBlurParam_t dds3MultiBlurBuf;
extern dds3Effect_PartialMultiBlurParam_t mulblurtarget;
extern u16 mulblurnow; extern u16 mulblurmax; extern u16 mulblurabase;
extern u16 mulbluratarget; extern u16 mulblurswapnow; extern u16 mulblurswapmax;
extern dds3Effect_DistortionBlurParam_t dds3DistBlurBuf;
extern dds3Effect_DistortionBlurParam_t distblurtarget;
extern u16 distblurnow; extern u16 distblurmax; extern u16 distblurabase;
extern u16 distbluratarget; extern u16 distblurswapnow; extern u16 distblurswapmax;
extern u32 distblurovercnt;
extern dds3RipBlurBody_t dds3RipBlurBuf;
extern u16 ripblurnow; extern u16 ripblurmax; extern u16 ripblurabase;
extern u16 ripbluratarget; extern u16 ripblurswapnow; extern u16 ripblurswapmax;
extern u32 ripblurovercnt;
s32 *Eve_PartialMultiBlurGetParams(void);
s32 *Eve_DistortionBlurGetParams(void);
extern dds3Effect_Blur_t blur2target;
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
void *Eve_FilterGetParams(void);
void dds3FadeClear(void);
s32 dds3FadeCheck(void);
extern sdf3DScreenParam_t dds3ScreenParam;
extern u16 dds3QuakeFlag;
extern s16 dds3QuakeFrame;
extern u16 dds3QuakeXRange;
extern u16 dds3QuakeYRange;
extern u32 egl_OT_Offset;

/* The quake's two axis gains are copied in by value, which is the unaligned
 * eight-byte block move at the entry. */
typedef struct
{
    f32 axis[2];
} dds3QuakeGain_t;

void dds3InitZClearDrawTag(void);
extern sdfVector4_t dds3AmbientColor;
extern u32 dds3BackColor;
void eglInit(void);
extern sdf3DScreenParam_t dds3RenderScreenParam;
extern dds3ZClearPacket_t dds3ZClearDrawTag[3][2];
extern u16 dds3RenderTexWidth;
extern u16 dds3RenderTexHeight;
extern u32 dds3RenderDrawEnv;
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
extern dds3Effect_PartialMultiBlurParam_t key2;
extern dds3Effect_DistortionBlurParam_t dds3DistBlurCurrent;
extern dds3RipBlurParam_t dds3BlurState;
extern dds3Effect_Blur_t dds3Blur2State;
extern dds3Effect_Filter_t dds3BlurParam;
extern dds3Effect_PartialMultiFilterParam_t dds3MFilterCurrent;
u32 dds3GetFrameID(void);
void dds3FreeRenderTexture(void);
sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

void dds3RotationArc(void);
void dds3InitRenderDrawEnv(void);
s32 dds3AllocRenderTexture(u16 sx, u16 sy, f32 aspect);
s32 dds3RenderTextureON(void);
void dds3RenderTextureOFF(void);
u32 dds3CheckRenderTexture(void);
void dds3SetQuake(s32 frame, s32 range);
void dds3StopQuake(void);
sdfDrawTag_t *dds3LinkZClearDrawTag(s32 sw);
void dds3FadeSet(s32 r, s32 g, s32 b, s32 a);
void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);
void dds3FadeIn2(s32 frame);
void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);
void dds3FadeOut2(s32 frame);
void dds3FadeUpdate(void);
void dds3SetScreenFovy(f32 fovy);
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
void dds3SetLightColor(s32 frame, s32 no, f32 *col);
void dds3SetLightDir(s32 frame, s32 no, f32 *dir);
void dds3SetAmbientColor(s32 frame, f32 *col);
void dds3SetFogColor(s32 frame, f32 *col);
void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);

/* SDK float-input color conversion, shared by the six light/ambient/fog inputs. */
static inline void dds3UnpackLightColor(u32 color, f32 unit) {
    u32 rgba[4] __attribute__((aligned(16)));
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

void dds3SceneLightUpdateFirst(void);
void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias);
extern s32 dds3ScreenBlurOfsX[2];

#include "sdk_libc.h"
#include "include_asm.h"
#include "kernel.h"
#include "newdata.h"
#include "dds3PadManager.h"

/* The screen fade's colour. Its setters store each component; the fade
 * overlay reads each one unsigned
 * (lbu), and its alpha as 0x80 minus the component. */
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} dds3FadeColor_t;

extern fclNamedSlot_t D_003C1C90[];

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

void dds3FadeColorGet(dds3FadeColor_t **outColor);

#include "event.h"
#include "scr.h"
#include "sdf_packet_alloc.h"

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

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

/* The allocated draw state's float at +0x1C is initialized to 1. */
typedef struct evtDrawState_t {
    u8 pad00[0x1C];
    f32 value1C;
} evtDrawState_t;

typedef struct dds3ResourceNode_t {
    s32 unk0;
    struct dds3ResourceNode_t *next;
    s32 *ready;
} dds3ResourceNode_t;

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct evtGsCommand_t {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} evtGsCommand_t;

void evtSubmitFixedAlphaBlend(s32 fixedAlpha);

void dds3InitDrawEnv(void)
{
    s8 savedParamFlag;

    sdfGrSetScreen(1);
    sdf3MakeScreenParam(&dds3ScreenParam);
    savedParamFlag = sdf3DScreenAdjust;
    sdf3DScreenAdjust = 0;
    sdf3MakeScreenParam(&DrawEnv);
    sdf3DScreenAdjust = savedParamFlag;
    dds3InitDrawEnvSub(0);
    dds3InitDrawEnvSub(1);
    dds3InitZClearDrawTag();
    dds3DrawProcessFlag = 0;
    eglInit();
}

/* NON_MATCHING: 85 of 88 words match.  */
#ifdef NON_MATCHING
extern sdfGrFrameSet_t dds3RenderTexId;
extern sdfMatrix4_t dds3RenderViewportMatrix;
extern sdf3DLight_t *dds3RenderLightParallel[4];
extern sdfVector4_t dds3RenderAmbientColor;

void dds3InitRenderDrawEnv(void) {
    s32 i;

    dds3RenderTexId.zbuffHandle = sdfGrFrameSet.zbuffHandle;
    dds3RenderTexId.w = dds3RenderTexWidth;
    dds3RenderTexId.h = dds3RenderTexHeight;
    dds3RenderTexId.buffH = dds3RenderTexHeight;
    dds3RenderTexId.zbuffFormat = 0x30;
    dds3RenderTexId.frameHandle[0] = dds3RenderTexHandle;
    dds3RenderTexId.pixelFormat = 0;
    dds3RenderTexId.frameHandle[1] = NULL;
    sdf3MakeScreenParam(&dds3RenderScreenParam);
    i = 0;
    do {
        sdfDrawTagInit(&dds3DrawEnv[i].renderTag);
        sdfGrSetDrawEnv(&dds3DrawEnv[i].renderEnv, &dds3RenderTexId, 0);
        dds3DrawEnv[i].renderEnv.clear.clearColor = dds3RenderDrawEnv;
        sdfDrawTagLink(&dds3DrawEnv[i].renderTag, EE_ADDR(&dds3DrawEnv[i].renderEnv));
        sdf3DMakeViewportPacket(&dds3DrawEnv[i].renderViewport, &dds3RenderScreenParam, &dds3RenderViewportMatrix);
        sdfDrawTagLink(&dds3DrawEnv[i].renderTag, EE_ADDR(&dds3DrawEnv[i].renderViewport));
        sdf3DMakeLightParallelPacket(&dds3DrawEnv[i].renderLight, dds3RenderLightParallel, &dds3RenderAmbientColor);
        sdfDrawTagLink(&dds3DrawEnv[i].renderTag, EE_ADDR(&dds3DrawEnv[i].renderLight));
        i++;
    } while (i != 2);
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitRenderDrawEnv);
#endif

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RefCameraTM);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RefCameraTM2);

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

/* body: matched in smt3 @ 9401f79e: src/smt3/kernel/dds3KernelDraw.c:463, near, same function 0.67 (words 13/13, calls 0.00, shape 1.00) */
void dds3FreeRenderTexture(void)
{
    if (dds3RenderTexture != NULL) {
        sdfTexReleaseReference(dds3RenderTexture);
    }
    dds3RenderTexture = NULL;
    dds3RenderTexHandle = 0;
    dds3RenderTex = 0;
}

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

sdfTexHandle_t *dds3GetRenderTexture(void)
{
    return dds3RenderTexture;
}

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} dds3BistaColor_t;

extern dds3BistaColor_t dds3Data30;
extern s8 dds3DrawBistaData;
extern s16 dds3ChangeBistaModeData;
extern s16 dds3Data32;
extern u16 dds3BistaUpdateWork;
extern u16 dds3Work;
#ifdef VERSION_RAIDOU2
extern s32 dds3DrawBistaData3;
#endif

extern void uiDrawSurfaceAtNearDepth(u32 surface);
extern void fclDrawPanelSprite(s32 a, s32 b);
extern void cmpDrawBoxF(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, u32 g);
extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);
extern dds3FadePacket_t dds3FadePacket[2];
extern u8 dds3Data29;
extern sdfGrOt_t dds3CallGetFrameIDData;
extern sdfDrawTag_t *dds3CallGrAllocPacketBuff9(void);

/* NON_MATCHING: 49 of 125 words match. Loop register use and float scheduling differ; body from SMT3, no frame gating. */
#ifdef NON_MATCHING
extern dds3QuakeGain_t dds3RunQuakeData[];
extern f32 gMapScrollReset[4];

void dds3RunQuake(void)
{
    f32 range[2];
    dds3QuakeGain_t gain = dds3RunQuakeData[0];
    f32 amplitude;
    f32 kick;
    s32 i;

    if (dds3QuakeFlag == 0) {
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        return;
    }
    if (dds3QuakeFrame == 0) {
        dds3QuakeFlag = 0;
        return;
    }
    range[0] = dds3QuakeXRange * 0.01f;
    range[1] = dds3QuakeYRange * 0.01f;
    for (i = 0; i < 2; i++) {
        if (range[i] != 0.0f) {
            amplitude = (dds3GetRandFloatA(0) * range[i]) * 0.5f + range[i] * 0.5f;
            if (range[i] * gain.axis[i] > fabsf(gMapScrollReset[i])) {
                kick = amplitude * (dds3GetRandFloatA(0) - 0.5f);
                gMapScrollReset[i] = gMapScrollReset[i] + (kick + kick);
            } else if (gMapScrollReset[i] > 0.0f) {
                gMapScrollReset[i] = gMapScrollReset[i] - amplitude;
            } else {
                gMapScrollReset[i] = gMapScrollReset[i] + amplitude;
            }
            if (gMapScrollReset[i] < -range[i]) {
                gMapScrollReset[i] = -range[i];
            }
            if (range[i] < gMapScrollReset[i]) {
                gMapScrollReset[i] = range[i];
            }
        }
    }
    if (dds3QuakeFrame > 0) {
        s32 divisor = dds3QuakeFrame + 5;

        dds3QuakeXRange -= dds3QuakeXRange / divisor;
        dds3QuakeYRange -= dds3QuakeYRange / divisor;
        dds3QuakeFrame--;
    }
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RunQuake);
#endif

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", sbtlResetJump);

/* Creating each of the two live XYZ2 payload locals before its RGBAQ pair preserves the native hoisted constants. */
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

    sdfDrawTagLink(tag, (addr_t)&dds3ZClearDrawTag[sw][dds3GetFrameID()]);
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
    *outColor = (dds3FadeColor_t *)&fadergba;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/kernel/dds3KernelDraw.c:642, match, same function 1.00 (words 5/5, calls 1.00, shape 1.00) */
void dds3FadeSet_2(s8 r, s8 g, s8 b)
{
    fadergba[0] = r;
    fadergba[1] = g;
    fadergba[2] = b;
}

void dds3FadeModeSet(s32 mode)
{
    dds3Data29 = mode;
}

#ifdef VERSION_RAIDOU1
void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame)
{
    fadergba[0] = r;
    fadergba[1] = g;
    fadergba[2] = b;
    fadergba[3] = -128;
    if (frame == 0) {
        fadergba[3] = 0;
        fadenow = 0;
        fademax = 0;
        dds3FadeClear();
        return;
    }
    fademax = SDF_FRAMES(frame);
    fadenow = SDF_FRAMES(frame);
    dds3DrawFlag = (dds3DrawFlag | 1) & ~2;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeTypeGet);
#endif

#ifdef VERSION_RAIDOU1
void dds3FadeIn2(s32 frame)
{
    fadergba[3] = -128;
    if (frame == 0) {
        fadergba[3] = 0;
        dds3DrawFlag &= ~3;
        fadenow = 0;
        fademax = 0;
    } else {
        fademax = SDF_FRAMES(frame);
        fadenow = SDF_FRAMES(frame);
        dds3DrawFlag = (dds3DrawFlag | 1) & ~2;
    }
}
#else
void FadeTypeSet(DDS3_FADETYPE eType)
{
    fadetype = eType;
}
#endif

#ifdef VERSION_RAIDOU1
void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame)
{
    fadergba[0] = r;
    fadergba[1] = g;
    fadergba[2] = b;
    fadergba[3] = 0;
    if (frame == 0) {
        fadenow = 0;
        fademax = 0;
        fadergba[3] = -128;
        dds3DrawFlag &= ~3;
        return;
    }
    fademax = SDF_FRAMES(frame);
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~1) | 2;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeIn);
#endif

#ifdef VERSION_RAIDOU1
void dds3FadeOut2(s32 frame)
{
    fadergba[3] = 0;
    if (frame == 0) {
        fadenow = 0;
        fadergba[3] = -128;
        dds3DrawFlag &= ~3;
        fademax = 0;
    } else {
        fademax = SDF_FRAMES(frame);
        fadenow = 0;
        dds3DrawFlag = (dds3DrawFlag & ~1) | 2;
    }
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeIn2);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RefDrawFlag);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeOut);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RefDrawFlag2);
#endif

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

void dds3DrawDistRipOverError(void)
{
    sdfDrawTag_t *tag;

    if (dds3DebugSign != 0) {
        if (distblurovercnt != 0 || ripblurovercnt != 0) {
            tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
            sdfDrawTagInit(tag);
            if ((s32)distblurovercnt > 0) {
                sdfDrawTagLink(tag, (addr_t)sdfDevPrintf(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, "DISTBLUR_NUMERR:%d", distblurovercnt));
            }
            if ((s32)ripblurovercnt > 0) {
                sdfDrawTagLink(tag, (addr_t)sdfDevPrintf(0x73C0, 0x7B40, 0xFEFFFF, 4, "RIPBLUR_NUMERR :%d", ripblurovercnt));
            }
            dds3Data10.regFunc(&dds3Data10, tag);
        }
    }
}

/* NON_MATCHING: 37 of 188 words match. Stack spills and register allocation differ; fade packet draw from SMT3 dds3DrawFade with bista offsets. */
#ifdef NON_MATCHING
void dds3CallGetFrameID(void)
{
    sdfDrawTag_t *list;
    u32 frameId;
    s32 alpha;
    u32 top;
    u32 bottom;
    u64 color;

    frameId = dds3GetFrameID();
    alpha = 0x80 - fadergba[3];
    if (alpha < 0x80) {
        dds3FadePacket[frameId].dma.qwc = 0xA;
        dds3FadePacket[frameId].dma.pad = 0;
        dds3FadePacket[frameId].dma.id = 0x70;
        dds3FadePacket[frameId].dma.addr = NULL;
        dds3FadePacket[frameId].dma.dat0 = 0;
        dds3FadePacket[frameId].dma.dat1 = 0x5000000A;
        dds3FadePacket[frameId].stateGif.parm = 0x1000000000000004;
        dds3FadePacket[frameId].stateGif.regs = 0xE;
        dds3FadePacket[frameId].state[0].reg = 0x42;
        dds3FadePacket[frameId].state[0].data = 1;
        dds3FadePacket[frameId].state[1].reg = 0x49;
        dds3FadePacket[frameId].state[1].data = 0;
        dds3FadePacket[frameId].state[2].reg = 0x4A;
        dds3FadePacket[frameId].state[2].data = 0;
        dds3FadePacket[frameId].state[3].reg = 0x47;
        dds3FadePacket[frameId].state[3].data = 0x31003;
        dds3FadePacket[frameId].spriteGif.parm = 0x1023400000008004;
        dds3FadePacket[frameId].spriteGif.regs = 0xE;
        top = 0x7900;
        bottom = 0x8700;
        if (dds3Data29 == 0 && dds3BistaCheck()) {
            top = 0x7900 + (dds3DrawBistaData2 - dds3ChangeBistaModeData) * 8;
            bottom = 0x8700 + (dds3Data32 - dds3Data33) * 8;
        }
        color = 0x3F80000000000000 | (u64)(s32)((u32)alpha << 24) | ((u64)fadergba[2] << 16) | ((u64)fadergba[1] << 8) | fadergba[0];
        dds3FadePacket[frameId].sprite[0].reg = 1;
        dds3FadePacket[frameId].sprite[0].data = color;
        dds3FadePacket[frameId].sprite[1].reg = 5;
        dds3FadePacket[frameId].sprite[1].data = 0xFEFFFF00007000 | ((u64)top << 16);
        dds3FadePacket[frameId].sprite[2].reg = 1;
        dds3FadePacket[frameId].sprite[2].data = color;
        dds3FadePacket[frameId].sprite[3].reg = 5;
        dds3FadePacket[frameId].sprite[3].data = 0xFEFFFF00009000 | ((u64)bottom << 16);
        list = dds3CallGrAllocPacketBuff9();
        sdfDrawTagLink(list, EE_ADDR(&dds3FadePacket[frameId]));
        dds3CallGetFrameIDData.regFunc(&dds3CallGetFrameIDData, list);
    }
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3CallGetFrameID);
#endif

void dds3BistaClear(void)
{
    dds3Data30.r = 0;
    dds3Data30.g = 0;
    dds3Data30.b = 0;
    dds3Data30.a = 0;
    dds3DrawFlag &= ~0x0C000000;
    dds3ChangeBistaModeData = dds3DrawBistaData2;
    dds3Data32 = dds3Data33;
    dds3DrawBistaData = 0;
#ifdef VERSION_RAIDOU2
    dds3DrawBistaData3 = 0;
#endif
}

void dds3BistaIn(u32 frame)
{
    if (dds3Data30.a != 0) {
        dds3Data30.r = 0;
        dds3Data30.g = 0;
        dds3Data30.b = 0;
        dds3Data30.a = 0x80;
        if (frame == 0) {
            dds3Data30.a = 0;
            dds3ChangeBistaModeData = dds3DrawBistaData2;
            dds3Data32 = dds3Data33;
            dds3BistaUpdateWork = 0;
            dds3Work = 0;
            dds3BistaClear();
        } else {
            dds3Work = frame;
            dds3BistaUpdateWork = frame;
            dds3DrawFlag = (dds3DrawFlag | 0x4000000) & ~0x8000000;
        }
        dds3ScreenParam.oy = 2048.0f;
    }
}

void dds3BistaOutEx(u32 frame, u32 height)
{
    if (dds3DrawBistaData != 3) {
        dds3Data30.r = 0;
        dds3Data30.g = 0;
        dds3Data30.b = 0;
        dds3Data30.a = 0;
    }
    if (height > 0x80) {
        height = 0x80;
    }
    dds3Data33 = height;
    dds3DrawBistaData2 = 0x80 - height;
    if (frame == 0) {
        dds3BistaUpdateWork = 0;
        dds3Work = 0;
        dds3ChangeBistaModeData = 0;
        dds3Data30.a = 0x80;
        dds3DrawFlag &= ~0x0C000000;
        dds3Data32 = 0;
    } else {
        dds3DrawFlag = (dds3DrawFlag & ~0x04000000) | 0x08000000;
        dds3Work = frame;
        dds3BistaUpdateWork = 0;
    }
    dds3ScreenParam.oy = 2048.0f;
}

void dds3BistaOut(u32 frame)
{
    dds3BistaOutEx(frame, 0x4F);
}

#ifdef VERSION_RAIDOU1
s32 dds3BistaCheck(void)
{
    if (dds3DrawFlag & 0x0C000000) {
        return 1;
    }
    switch (dds3DrawBistaData) {
    case 0:
    case 3:
        if (0x80 - dds3Data30.a < 0x80) {
            break;
        }
        return 0;
    case 1:
    case 2:
        if (dds3ChangeBistaModeData != dds3DrawBistaData2) {
            break;
        }
        return 0;
    }
    return 1;
}
#else
/* NON_MATCHING: 25 of 32 words match.  */
#ifdef NON_MATCHING
s32 dds3BistaCheck(void) {
    if (dds3DrawFlag & 0x0C000000) {
        return 1;
    }
    switch (dds3DrawBistaData) {
    case 0:
    case 3:
        if (0x80 - dds3Data30.a < 0x80) {
            return 1;
        }
        break;
    case 1:
    case 2:
        if (dds3ChangeBistaModeData != dds3DrawBistaData2) {
            return 1;
        }
        break;
    case 4:
        break;
    default:
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3BistaCheck);
#endif
#endif
#ifdef VERSION_RAIDOU2

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3IsBistaZBufferClear);
#endif

void dds3ChangeBistaMode(BISTA_MODE mode)
{
    dds3DrawBistaData = mode;
    switch (dds3DrawBistaData) {
    case BISTA_MODE_ALPHA:
    case BISTA_MODE_FADE:
        dds3ChangeBistaModeData = 0;
        dds3Data32 = 0;
        break;
    case BISTA_MODE_MOVE:
    case BISTA_MODE_TALKEVENT:
        dds3Data30.a = 0x80;
        break;
    }
}

void dds3BistaUpdate(void)
{
    f32 rate;

    if (dds3DrawFlag & 0x0C000000) {
        if (dds3DrawFlag & 0x04000000) {
            dds3BistaUpdateWork--;
        } else {
            dds3BistaUpdateWork++;
        }
        dds3Data30.a = (dds3BistaUpdateWork << 7) / dds3Work;
        rate = (f32)dds3BistaUpdateWork / (f32)dds3Work;
        dds3ChangeBistaModeData = (s32)((f32)dds3DrawBistaData2 - (f32)dds3DrawBistaData2 * rate);
        dds3Data32 = (s32)((f32)dds3Data33 - (f32)dds3Data33 * rate);
        if (((dds3DrawFlag & 0x04000000) && dds3BistaUpdateWork == 0) || ((dds3DrawFlag & 0x08000000) && dds3BistaUpdateWork == dds3Work)) {
            dds3BistaUpdateWork = 0;
            dds3Work = 0;
            dds3DrawFlag &= ~0x0C000000;
        }
    }
}

/* NON_MATCHING: 20 of 171 words match. Register allocation and an unsigned mode read differ. */
#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
void dds3DrawBista(void)
{
    s32 fade;
    s32 top;
    s32 bottom;
    s32 draw;
    s32 color;
    s32 shade;

    fade = 0;
    top = 0;
    bottom = 0;
    if (dds3DrawBistaData >= 3) {
        draw = 1;
        if (dds3DrawBistaData == 3) {
            fade = 0x80 - dds3Data30.a;
            draw = fade < 0x80;
        }
    } else if (dds3DrawBistaData > 0) {
        top = dds3ChangeBistaModeData;
        bottom = dds3Data32;
        if (dds3ChangeBistaModeData == dds3DrawBistaData2) {
            return;
        }
        draw = fade < 0x80;
    } else {
        draw = 1;
        if (dds3DrawBistaData == 0) {
            fade = 0x80 - dds3Data30.a;
            draw = fade < 0x80;
        }
    }
    if (draw) {
        color = (0x80 - fade) << 24;
        if (dds3DrawBistaData < 3 && dds3DrawBistaData >= 0) {
            uiDrawSurfaceAtNearDepth(0x5A);
            eglSetOt(97);
            eglLinkAlpha(0);
            eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            evtSubmitDefaultDepthGradientRect(0, -2 - top, 512, dds3DrawBistaData2 + 2, color, color, color, color);
            evtSubmitDefaultDepthGradientRect(0, bottom - dds3Data33 + 448, 512, dds3Data33 + 2, color, color, color, color);
        }
        if (dds3DrawBistaData == 2) {
            shade = (55 * (dds3DrawBistaData2 - top) / dds3DrawBistaData2) << 24;
            eglSetOt(85);
            eglLinkTest(1, 0, 0x80, 3, 0, 0, 0, 1);
            evtSubmitDefaultDepthGradientRect(0, 0, 512, 448, shade, shade, shade, shade);
        } else if (dds3DrawBistaData == 3) {
            fclDrawPanelSprite(0x30000, 84);
            cmpDrawBoxF(0, 0, 0, 0x2000, 3584, 0, 0x54);
            eglSetOt(85);
            eglLinkAlpha(0);
            eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            evtSubmitDefaultDepthGradientRect(0, 0, 512, 448, color, color, color, color);
        }
    }
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawBista);
#endif
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawBista);
#endif

void dds3SetScreenFovy(f32 fovy)
{
    dds3ScreenParam.fovy = fovy;
}

void dds3SetAmbient(sdfVector4_t *ambient)
{
    SDF_VU0_COPYVECTOR(&dds3LightParam.ambient, ambient);
}

void dds3SetFog(sdf3DFogParam_t *fog)
{
    memcpy(&dds3LightParam.fog, fog, 0x14);
}

void dds3SetLight(sdf3DLight_t *light, s32 no)
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
    filter2target.abgr = rgba;
}

void dds3Filter2Swap(s32 frame)
{
    memcpy(&dds3Filter2Buf, Eve_FilterGetParams(), 0x18);
    filter2swapnow = 0;
    filter2swapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x100000;
        setdraw_xy(filter2target.xy[0]);
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
        setdraw_xy(filter2target.xy[0]);
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
    mfiltertarget.Filter.blendControl = blend;
}

void dds3SetMFilterRGBA(u32 rgba)
{
    mfiltertarget.Filter.rgba = rgba;
}

void dds3SetMFilterArea(s32 area, s32 posx, s32 posy)
{
    mfiltertarget.area = area;
    mfiltertarget.center[0] = posx;
    mfiltertarget.center[1] = posy;
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
    mfilteratarget = mfiltertarget.Filter.color[3];
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
    mfilterabase = mfiltertarget.Filter.color[3];
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
    blur2target.rotate = rotate;
    blur2target.scale = scale;
    blur2target.blend = blend;
}

void dds3SetBlur2RGBA(u32 rgba)
{
    blur2target.abgr = rgba;
}

void dds3SetBlur2Area(s32 posx, s32 posy)
{
    blur2target.center[0] = posx;
    blur2target.center[1] = posy;
}

void dds3Blur2Swap(s32 frame)
{
    memcpy(&ripblurtarget.blur2buf, Eve_BlurGetParam(), 0x28);
    blur2swapnow = 0;
    blur2swapmax = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x40000;
        setdraw_xy(blur2target.xy[0]);
        Eve_BlurSetParam(&blur2target);
    } else {
        dds3DrawFlag |= 0x40000;
    }
}

void dds3Blur2On(s32 frame)
{
    blur2abase = 0;
    blur2now = 0;
    blur2atarget = (blur2target.abgr >> 24);
    blur2max = SDF_FRAMES(frame);
    if (frame == 0) {
        dds3DrawFlag &= ~0x20000;
        setdraw_xy(blur2target.xy[0]);
        Eve_BlurSetParam(&blur2target);
        Eve_BlurON();
    } else {
        dds3DrawFlag |= 0x20000;
    }
}

void dds3Blur2Off(s32 frame)
{
    blur2atarget = 0;
    blur2abase = (blur2target.abgr >> 24);
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
    mulblurtarget.Blur.rotate = rotate;
    mulblurtarget.Blur.scale = scale;
    mulblurtarget.Blur.blend = blend;
}

void dds3SetMultiBlurRGBA(u32 rgba)
{
    mulblurtarget.Blur.abgr = rgba;
}

void dds3SetMultiBlurArea(s32 area, s32 posx, s32 posy)
{
    mulblurtarget.area = area;
    mulblurtarget.Blur.center[0] = posx;
    mulblurtarget.Blur.center[1] = posy;
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
    mulbluratarget = (mulblurtarget.Blur.abgr >> 24);
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
    mulblurabase = (mulblurtarget.Blur.abgr >> 24);
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
    distblurtarget.number = number;
    distblurtarget.width = width;
    distblurtarget.interval = intval;
    distblurtarget.speed = rotate;
    distblurtarget.rotate = distor;
    distblurtarget.distortion = blend;
    distblurtarget.blend = speed;
}

void dds3SetDistBlurRGBA(u32 rgba)
{
    distblurtarget.abgr = rgba;
}

void dds3SetDistBlurArea(s32 area, s32 posx, s32 posy)
{
    distblurtarget.area = area;
    distblurtarget.center[0] = posx;
    distblurtarget.center[1] = posy;
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
    distbluratarget = (distblurtarget.abgr >> 24);
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
    distblurabase = (distblurtarget.abgr >> 24);
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
    ripblurtarget.ripple.number = number;
    ripblurtarget.ripple.speed = intval;
    ripblurtarget.ripple.interval = rotate;
    ripblurtarget.ripple.rotate = distor;
    ripblurtarget.ripple.power = distor_s;
    ripblurtarget.ripple.frequency = blend;
    ripblurtarget.ripple.blend = speed;
}

void dds3SetRipBlurRGBA(u32 rgba)
{
    ripblurtarget.ripple.abgr = rgba;
}

void dds3SetRipBlurArea(s32 area, s32 posx, s32 posy)
{
    ripblurtarget.ripple.area = area;
    ripblurtarget.ripple.center[0] = posx;
    ripblurtarget.ripple.center[1] = posy;
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
    ripbluratarget = (ripblurtarget.ripple.abgr >> 24);
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
    ripblurabase = (ripblurtarget.ripple.abgr >> 24);
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
        setdraw_xy(dds3Blur2State.xy[0]);
        Eve_BlurSetParam(&dds3Blur2State);
        if ((dds3Blur2State.abgr >> 24) != 0) {
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
        dds3Blur2State.abgr = (dds3Blur2State.abgr & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3Blur2State.xy[0]);
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
        if ((key2.Blur.abgr >> 24) != 0) {
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
        key2.Blur.abgr = (key2.Blur.abgr & 0xFFFFFF) | (alpha << 24);
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
        if ((dds3DistBlurCurrent.abgr >> 24) != 0) {
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
        dds3DistBlurCurrent.abgr = (dds3DistBlurCurrent.abgr & 0xFFFFFF) | (alpha << 24);
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
        if ((dds3BlurState.ripple.abgr >> 24) != 0) {
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
        dds3BlurState.ripple.abgr = (dds3BlurState.ripple.abgr & 0xFFFFFF) | (alpha << 24);
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
        setdraw_xy(dds3BlurParam.xy[0]);
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
        dds3BlurParam.abgr = (dds3BlurParam.abgr & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3BlurParam.xy[0]);
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
        if (dds3MFilterCurrent.Filter.color[3] != 0) {
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
        dds3MFilterCurrent.Filter.rgba = (dds3MFilterCurrent.Filter.rgba & 0xFFFFFF) | (alpha << 24);
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

/* body: matched in smt3 @ 9401f79e: src/smt3/kernel/dds3KernelDraw.c:1834, match, same function 1.00 (words 69/69, calls 1.00, shape 1.00) */
void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst)
{
    void *list = (void *)sdfGrAllocPacketBuff(0x20);
    void *packet;
    dds3GsCommand_t *command;
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x30);
    command = (dds3GsCommand_t *)sdf2DMakeStdPacket((u32)packet, 0x30);
    command->write.data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->write.reg = 0x47;
    sdfDrawTagLink(list, (addr_t)packet);
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/newbattle/nbPanelProcess.c:615, adapt, same function 1.00 (words 69/69, calls 1.00, shape 1.00) */
void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst)
{
    sdfDrawTag_t *list;
    u64 *buff;
    u64 *packet;
    sdfGrOt_t *ot;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    buff = (u64 *)sdfGrAllocPacketBuff(0x30);
    packet = (u64 *)sdf2DMakeStdPacket(EE_ADDR(buff), 0x30);
    packet[4] = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) |
                (aref << 4) | (atst << 1) | ate;
    packet[5] = 0x48;
    sdfDrawTagLink(list, EE_ADDR(buff));
    ot = &sdfGrOtTable[egl_OT_Offset];
    ot->regFunc(ot, list);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/kernel/dds3KernelDraw.c:1853, near, same function 0.92 (words 77/65, calls 1.00, shape 0.92) */
void eglLinkAlpha(s32 mode)
{
    void *list = (void *)sdfGrAllocPacketBuff(0x20);
    void *packet;
    dds3GsCommand_t *command;

    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x30);
    command = (dds3GsCommand_t *)sdf2DMakeStdPacket((u32)packet, 0x30);
    switch (mode) {
    case 1:
        command->write.data = 0x48;
        break;
    case 2:
        command->write.data = 0x42;
        break;
    case 3:
        command->write.data = 0x4A;
        break;
    case 4:
        command->write.data = 0x80000000A8;
        break;
    case 5:
        command->write.data = 0x84;
        break;
    case 6:
        command->write.data = 0x06;
        break;
    case 7:
        command->write.data = 0x89;
        break;
    case 8:
        command->write.data = 0x42;
        break;
    case 9:
        command->write.data = 0x54;
        break;
    case 10:
        command->write.data = 0x2A;
        break;
    case 11:
        command->write.data = 0x56;
        break;
    case 12:
        command->write.data = 0x16;
        break;
    case 13:
        command->write.data = 0x58;
        break;
    default:
        command->write.data = 0x44;
        break;
    }
    command->write.reg = 0x42;
    sdfDrawTagLink(list, (addr_t)packet);
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
    }
}

/* Submit ALPHA_1 = (Cs - Cd) * FIX / 128 + Cd; retain the native unmasked FIX input. */
void evtSubmitFixedAlphaBlend(s32 fixedAlpha) {
    void *list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    void *packet;
    evtGsCommand_t *command;
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(EVT_GS_COMMAND_BYTES);
    command = (evtGsCommand_t *)sdf2DMakeStdPacket((u32)packet, EVT_GS_COMMAND_BYTES);
    command->data = ((u64)fixedAlpha << 32) | EVT_FIXED_ALPHA_INTERPOLATION;
    command->registerId = EVT_GS_ALPHA_PRIMARY;
    sdfDrawTagLink(list, (u32)(packet));
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
    }
}

void dds3CallGrAllocPacketBuff3(void)
{
    sdfDrawTag_t *tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfGrRefGifPacketHeader_t *buffer;
    sdfGrOt_t *otp;

    sdfDrawTagInit(tag);
    buffer = EE_ADDR_AS(sdfGrRefGifPacketHeader_t *, sdfGrAllocPacketBuff(0x40));
    sdfGrSetFrameMaskRGB((const u8 *)&dds3DrawEnv[dds3GetFrameID()].stdEnv, buffer, 0);
    sdfDrawTagLinkRefGif(tag, EE_ADDR(buffer));
    otp = &sdfGrOtTable[egl_OT_Offset];
    otp->regFunc(otp, tag);
}

void dds3CallGrAllocPacketBuff4(void)
{
    sdfDrawTag_t *tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfGrRefGifPacketHeader_t *buffer;
    sdfGrOt_t *otp;

    sdfDrawTagInit(tag);
    buffer = EE_ADDR_AS(sdfGrRefGifPacketHeader_t *, sdfGrAllocPacketBuff(0x40));
    sdfGrSetFrameMaskReset((const u8 *)&dds3DrawEnv[dds3GetFrameID()].stdEnv, buffer, 0);
    sdfDrawTagLinkRefGif(tag, EE_ADDR(buffer));
    otp = &sdfGrOtTable[egl_OT_Offset];
    otp->regFunc(otp, tag);
}

void eglLinkSprite(s32 x, s32 y, s32 w, s32 h,
                   s32 u, s32 v, s32 uw, s32 vh,
                   u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = dds3FontReqAllocDrawTag();
    sdf2DLinkTexPacket(list, th, 0);
    left = (x << 4) + 0x7000;
    top = (y << 3);
    right = (x << 4) + (w << 4) + 0x7000;
    bottom = top + (h << 3) + 0x7900;
    top += 0x7900;
    uLeft = (u << 4);
    vTop = (v << 4);
    uRight = uLeft + (uw << 4);
    vBottom = vTop + (vh << 4);
    sdfPrimLinkGt4(list, 0x40,
        left, top, uLeft, vTop, rgba0,
        right, top, uRight, vTop, rgba1,
        left, bottom, uLeft, vBottom, rgba3,
        right, bottom, uRight, vBottom, rgba2,
        -1, NULL);
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->regFunc(surface, list);
}
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", egl_OT_Offset);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3BackColor);

u32 dds3RenderTex = 0;

sdfGsMemSlice_t *dds3RenderTexHandle = NULL;

sdfTexHandle_t *dds3RenderTexture = NULL;

u16 dds3RenderTexWidth = 0x80;

u16 dds3RenderTexHeight = 0x80;

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderDrawEnv);

u32 dds3DrawFlag = 0;

u8 dds3ScreenBlurFlag = 0;

s16 dds3ScreenBlurAlpha = 0x60;

#ifdef VERSION_RAIDOU1
s16 dds3ScreenBlurScale = 0x10;
#else
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurScale);
#endif

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurOfsX);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", pbDebugPrintPanel2Data2);

u16 dds3QuakeFlag = 0;

s16 dds3QuakeFrame = 0;

u16 dds3QuakeXRange = 0;

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3QuakeYRange);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", Zoom);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", fadergba);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3Data28);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3Data29);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", fadetype);

#endif
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3Data30);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3Data31);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3DrawBistaData);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ChangeBistaModeData);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3Data32);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3DrawBistaData2);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3Data33);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3DrawBistaData3);

#endif
u32 distblurovercnt = 0;

u32 ripblurovercnt = 0;
