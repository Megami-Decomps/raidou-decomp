#include "common.h"
#include "libdma.h"
#include "eeregs.h"
#include "ps2asm.h"
#include "model.h"
#include "draw3d.h"
#include "sdfMemory.h"
#include "sdfModel.h"
#include "sdfCoreDraw.h"

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
void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *root, sdfCorePacket_t *p);
void sdfCoreDrawSetLight(u8 *light);
void sdfCoreDrawSetFog(u8 *fog);
void sdfCoreDrawSetZBias(f32 z);
void sdfCoreDrawInit(s32 buffSize);

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

void sdfLeaf25(void) {
    corePktCurrLight = 0;
    corePktCurrFog = 0;
}

void sdfCoreDrawInit(s32 buffSize) {
    sceDmaChan *dmaChannel = sceDmaGetChan(0);
    HW_WRITE(&dmaChannel->chcr, HW_READ(&dmaChannel->chcr) & ~SDF_DMA_CHCR_TTE);
    sceDmaSendN(dmaChannel, vu0MicroPacket, (vu1MicroPacket - vu0MicroPacket) >> SDF_DMA_QWORD_SHIFT);
    sceDmaSync(dmaChannel, 0, 0);
    sdfMemFreeHandleDirect(&sdfCoreDrawBuffHandle);
    sdfCoreDrawBuffHandle = sdfMemAllocDirect(buffSize);
    corePktPointBuff = EE_ADDR(sdfMemLock(sdfCoreDrawBuffHandle));
}

INCLUDE_SDATA(const s32, "sdf/sdfCoreDraw", sdfCoreDrawBuffHandle);

f32 sdfFaceDivSoften = 7.0f;

f32 sdfFaceDivBias = 0.0f;
