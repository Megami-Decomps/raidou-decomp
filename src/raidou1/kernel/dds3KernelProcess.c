#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3KernelProcess.c */
#include "common.h"

#include "basic.h"

#include "kernel.h"

#include "kernel_draw.h"

#include "model.h"

#include "ps2asm.h"

#include "sdfFrame.h"

extern dds3Basic_t *dds3TheWorld;

extern u32 dds3GetKernelCount(void);

extern void dds3CallBasicMainProc(dds3Basic_t *pBasic);

extern void dds3CallBasicDrawProc(dds3Basic_t *basic);

typedef struct sdfPreDrawChain_t sdfPreDrawChain_t;

extern dds3DrawBank_t dds3DrawEnv[2];

extern sdfPreDrawChain_t dds3PreDrawChain;

extern sdfGrOt_t sdfGrOtTable[85];

extern u32 dds3GetFrameID(void);

extern void sdfGrInitNewFrame(void);

extern void sdfGrOtInit(sdfGrOt_t *ot, s32 count);

extern void sdfPreDrawChainInit(sdfPreDrawChain_t *chain);

extern void dds3InitDrawEnvSub(s32 frame);

extern void sdfModelSetDrawEnv(sdfModelDrawEnv_t *env);

s32 dds3FirstProcess(dds3ProcessID_t *id);

typedef struct sdfGrRefPacketHeader_t sdfGrRefPacketHeader_t;

/* Sprite GIF payload: RGBAQ followed by UV/XYZ2 for each corner.
 * The GS ignores the unused lanes; native writes XYZ's 16-bit ADC lane. */
typedef struct {
    s32 u, v;
    u8 unusedUV[8];
    s32 x, y, z;
    s16 adc;
    u8 unusedXYZ[2];
} dds3BlurCorner_t;

typedef struct {
    s32 r, g, b, a;
    dds3BlurCorner_t corner[2];
} dds3BlurSprite_t;

extern sdfModelOt_t sdfModelOtTable[];

extern sdfGrOt_t D_002F6938;

extern u8 dds3ScreenBlurFlag, dds3DrawProcessFlag;

extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;

extern s32 dds3ScreenBlurOfsX[2];

extern sdfDrawTag_t *dds3ClosedDrawTag;

extern u32 dds3DrawFlag;

extern s8 sdfDrawPublishFlag;

extern void dds3DrawDistRipOverError(void);

extern void dds3FadeUpdate(void);

extern void dds3DrawFade(void);

extern void dds3BlurUpdate(void);

extern u32 dds3NullDrawProc(void);

extern void sdfModelDrawEnvLinkOt(sdfGrOt_t **, sdfModelDrawEnv_t *);

extern s32 sdfGrAllocPacketBuff(s32);

extern void sdfDrawTagInit(sdfDrawTag_t *);

extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t);

extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *, addr_t, sdfGrRefPacketHeader_t *);

extern sdfGrPacketHeader_t *sdf2DAllocSprite(s32);

extern void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *);

extern sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *);

extern void sdfGrFlushOtChain(u32 *);

extern void sdfGrReqDraw(sdfDrawTag_t *, sdfPreDrawChain_t *);

/* DDS1 dds3DrawProcess donor, with SMT3's eleven model groups and draw bank. */
s32 dds3DrawProcess(dds3ProcessID_t *task);

s32 dds3BasicMainProcess(dds3ProcessID_t *id);

s32 dds3BasicDrawProcess(dds3ProcessID_t *id);
/* End of declarations from src/smt3/kernel/dds3KernelProcess.c */

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3FirstProcess);

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawFirstProcess);

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawProcess);

s32 dds3BasicMainProcess(dds3ProcessID_t *id) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

s32 dds3BasicDrawProcess(dds3ProcessID_t *id) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}

INCLUDE_SDATA(const s32, "kernel/dds3KernelProcess", D_r1_0046B194);

