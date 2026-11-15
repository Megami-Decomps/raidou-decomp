#include "common.h"

#include "libdma.h"
#include "sdfDevCons.h"
#include "sdf_packet_alloc.h"
#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "libsdf.h"

typedef struct sdfDevConsDrawCB_t
{
    u8 *cur;
    u8 *start;
    u8 *tag;
    sceDmaChan *dmaCh;
    u8 *buf[2];
    s32 buffSize;
    u16 dblBuffId;
    u16 stepY;
} sdfDevConsDrawCB_t;

/* SMT3 DrawAll reserves this same 0x1040-byte stack
 * allocation: only the central 0x1000 bytes are passed as DMA storage. */
typedef struct
{
    u8 opaquePrefix[0x30];
    u8 data[0x1000];
    u8 opaqueSuffix[0x10];
} sdfDevConsStackPackets_t;

typedef struct
{
    u16 quadwordCount;
    u8 reserved02;
    u8 tagKind;
    u32 nextAddress;
    u32 vifNop;
    u32 vifDirect;
    u64 setupTag;
    u64 setupRegisters;
    u64 textureClamp;
    u64 textureClampRegister;
    u64 textureFilter;
    u64 textureFilterRegister;
    u64 textureState;
    u64 textureStateRegister;
    u64 glyphTag;
    u64 glyphRegisters;
} sdfDevConsPacketHeader_t;

typedef struct
{
    u64 color;
    u64 firstUv;
    u64 firstPosition;
    u64 secondUv;
    u64 secondPosition;
} sdfDevConsGlyph_t;

extern u32 sdfDevConsColorTable[];

/* Mutable colors used by the device console and packet command setter. */
extern u32 sdfDevConsColorTable[16];
void sdfPktSetCmd(sdfDevPrintCB_t *aCB, s32 aMode);

void sdfPktSetCmd(sdfDevPrintCB_t *aCB, s32 aMode)
{
    aCB->attr = sdfDevConsColorTable[aMode];
}

void sdfDevSetPrintCB(sdfDevPrintCB_t *aCB, u32 aX, u32 aY, u32 aZ, s32 aAttr)
{
    aCB->x = aX;
    aCB->y = aY;
    aCB->z = aZ;
    sdfPktSetCmd(aCB, aAttr);
}
