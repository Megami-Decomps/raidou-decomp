#include "common.h"

#include "libdma.h"
#include "sdfDevCons.h"
#include "sdf_packet_alloc.h"
#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "libsdf.h"

typedef struct sdfDevConsCB_tag
{
    struct sdfDevConsCB_tag *prev;
    struct sdfDevConsCB_tag *next;
    u16 x;
    u16 y;
    s16 w;
    s16 h;
    s16 locX;
    s16 locY;
    u8 mode;
    u8 pad;
    u8 attr;
    u8 tabwidth;
    sdfMemHandle_t *bufferHandle;
    u8 *bufferPtr;
} sdfDevConsCB_tag;

typedef struct sdfDevConsDrawCB_t
{
    u8 *cur;
    u8 *start;
    u8 *tag;
    u32 *dmaCh;
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

/* The four print-control words are signed XYZ and packed RGBA when drawing. */
typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 attr;
} sdfDevPrintCB_t;

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
void sdfDevSetPrintCB(u32 *aCB, u32 aX, u32 aY, u32 aZ, s32 aAttr);
void sdfPktSetCmd(u32 *aCB, s32 aMode);

void sdfPktSetCmd(u32 *aCB, s32 aMode)
{
    aCB[3] = sdfDevConsColorTable[aMode];
}

void sdfDevSetPrintCB(u32 *aCB, u32 aX, u32 aY, u32 aZ, s32 aAttr)
{
    aCB[0] = aX;
    aCB[1] = aY;
    aCB[2] = aZ;
    sdfPktSetCmd(aCB, aAttr);
}
