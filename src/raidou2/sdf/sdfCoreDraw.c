#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdf/sdfCoreDraw.c */
#include "common.h"

#include "libdma.h"

#include "eeregs.h"

#include "ps2asm.h"

#include "model.h"

#include "draw3d.h"

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_DMA_CHCR_TTE 0x40

extern addr_t corePktCurrLight;

extern f32 corePktZBias;

extern addr_t corePktCurrFog;

extern u128 corePktFogParm;

extern u8 vu0MicroPacket[];

extern u8 vu1MicroPacket[];

extern saddr_t corePktPointBuff;

extern sdfMemHandle_t *sdfCoreDrawBuffHandle;

extern void *sceDmaGetChan(s32);

extern void sceDmaSendN(void *, void *, s32);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);

extern void sdfMemFreeHandleDirect(sdfMemHandle_t **handle);

extern void *sdfMemLock(sdfMemHandle_t *handle);

extern u8 *sdfModelAllocTmPacket(sdfModelNode_t *node);

void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *root, sdfCorePacket_t *p);

void sdfCoreDrawSetLight(u8 *light);

void sdfCoreDrawSetFog(u8 *fog);

void sdfCoreDrawSetZBias(f32 z);

void sdfCoreDrawReset(void);

void sdfCoreDrawInit(s32 buffSize);

extern sdfMemHandle_t *sdfCoreDrawBuffHandle;

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;
/* End of declarations from src/smt3/sdf/sdfCoreDraw.c */

INCLUDE_ASM(const s32, "sdf/sdfCoreDraw", sdfCorePacketGen);

void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *root, sdfCorePacket_t *p) {
    sdfModelNode_t **objects = root->nodeList->items;
    u16 *indices = (u16 *)(p + 1);
    if ((p->prim.packetBits & SDF_PACKETBIT_BLENDVERT) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                sdfModelAllocTmPacket(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void sdfCoreDrawSetLight(u8 *light) {
    corePktCurrLight = EE_ADDR(light);
}

void sdfCoreDrawSetFog(u8 *fog) {
    if (corePktCurrFog != EE_ADDR(fog)) {
        corePktCurrFog = EE_ADDR(fog);
        SDF_VU0_COPYVECTOR(&corePktFogParm, fog + 0x10);
    }
}

void sdfCoreDrawSetZBias(f32 z) {
    corePktZBias = z;
}

INCLUDE_ASM(const s32, "sdf/sdfCoreDraw", func_003DDF38);

void sdfCoreDrawInit(s32 buffSize) {
    u32 *dmaChannel = sceDmaGetChan(0);
    HW_WRITE(dmaChannel, HW_READ(dmaChannel) & ~SDF_DMA_CHCR_TTE);
    sceDmaSendN(dmaChannel, vu0MicroPacket, (vu1MicroPacket - vu0MicroPacket) >> SDF_DMA_QWORD_SHIFT);
    sceDmaSync((sceDmaChan *)dmaChannel, 0, 0);
    sdfMemFreeHandleDirect(&sdfCoreDrawBuffHandle);
    sdfCoreDrawBuffHandle = sdfMemAllocDirect(buffSize);
    corePktPointBuff = EE_ADDR(sdfMemLock(sdfCoreDrawBuffHandle));
}
