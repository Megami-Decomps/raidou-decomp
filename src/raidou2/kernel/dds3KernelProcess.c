#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "kernel_draw.h"
#include "model.h"
#include "ps2asm.h"
#include "sdfFrame.h"

extern dds3Basic_t *dds3TheWorld;
extern void dds3CallBasicMainProc(dds3Basic_t *pBasic);

typedef struct sdfPreDrawChain_t sdfPreDrawChain_t;

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

extern u8 dds3ScreenBlurFlag, dds3DrawProcessFlag;
extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;
extern sdfDrawTag_t *dds3ClosedDrawTag;
s32 dds3BasicMainProcess(dds3ProcessID_t *id);

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3FirstProcess);

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawFirstProcess);

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawProcess);

s32 dds3BasicMainProcess(dds3ProcessID_t *id) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3BasicDrawProcess);

INCLUDE_SDATA(const s32, "kernel/dds3KernelProcess", dds3ClosedDrawTag);

