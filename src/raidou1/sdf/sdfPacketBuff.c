#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdf/sdfPacketBuff.c */
#include "common.h"

#include "sdf_types.h"

#include "sdf_resource.h"

/* Declared here because no header carries these. */
extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

struct sdfMemHandle_t;

#define SDF_QWORD_ALIGNMENT_MASK 0xF

#define SDF_PACKET_BUFFER_ALIGNMENT_MASK 0x7F

#define SDF_PACKET_BUFFER_COUNT 2

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

extern struct sdfMemHandle_t *sdfGrPacketBuffHandle;

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

extern s32 sdfGrPacketBuffSize;

extern s32 sdfGrPacketBuffBase[2];

extern s32 sdfGrPacketBuffPtr;

extern s32 sdfGrPacketBuffLimit;

typedef struct effDebugPolyParams_t {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} effDebugPolyParams_t;

void sdfGrMakePacketBuff(s32 aPacketBuffSize);

void sdfGrSetupPacketBuff(s32 aFrameId);

s32 sdfGrAllocPacketBuff(s32 aSize);

void sdfGrClosePacketBuff(saddr_t aBuffPtr);

void sdfGrMakePacketBuff(s32 aPacketBuffSize);

void sdfGrSetupPacketBuff(s32 aFrameId);

s32 sdfGrGetPacketBuffRemain(void);

s32 sdfGrAllocPacketBuff(s32 aSize);

s32 sdfGrOpenPacketBuff(void);

void sdfGrClosePacketBuff(saddr_t aBuffPtr);

extern struct sdfMemHandle_t *sdfGrPacketBuffHandle;

extern s32 sdfGrPacketBuffSize;

extern s32 sdfGrPacketBuffBase[2];

extern s32 sdfGrPacketBuffPtr;

extern s32 sdfGrPacketBuffLimit;

extern u32 D_00551880;
/* End of declarations from src/smt3/sdf/sdfPacketBuff.c */

void sdfGrMakePacketBuff(s32 aPacketBuffSize) {
    s32 allocationAddress;
    if (sdfGrPacketBuffHandle != 0) {
        sdfMemFreeDirect(sdfGrPacketBuffHandle);
        sdfGrPacketBuffHandle = 0;
    }
    aPacketBuffSize = (aPacketBuffSize + SDF_PACKET_BUFFER_ALIGNMENT_MASK) & ~SDF_PACKET_BUFFER_ALIGNMENT_MASK;
    sdfGrPacketBuffSize = aPacketBuffSize;
    sdfGrPacketBuffHandle = sdfMemAllocDirect(aPacketBuffSize * SDF_PACKET_BUFFER_COUNT);
    allocationAddress = sdfMemLock(sdfGrPacketBuffHandle);
    sdfGrPacketBuffBase[0] = allocationAddress;
    sdfGrPacketBuffBase[1] = allocationAddress + aPacketBuffSize;
}

INCLUDE_ASM(const s32, "sdf/sdfPacketBuff", sdfGrSetupPacketBuff);

s32 sdfGrGetPacketBuffRemain(void) {
    return sdfGrPacketBuffLimit - sdfGrPacketBuffPtr;
}

s32 sdfGrAllocPacketBuff(s32 aSize) {
    s32 packet;

    packet = sdfGrPacketBuffPtr;
    sdfGrPacketBuffPtr = sdfGrPacketBuffPtr + ((aSize + 0xfU) & 0xfffffff0);
    return packet;
}
struct sdfMemHandle_t *sdfGrPacketBuffHandle = NULL;

s32 sdfGrPacketBuffSize = 0;

s32 sdfGrPacketBuffBase[2] = {0, 0};

s32 sdfGrPacketBuffPtr = 0;

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", D_0046F2B4);

volatile u8 sdfDrawFrameFlag = 0;

volatile u8 sdfDrawSyncFlag = 0;

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", D_0046F2BE);

volatile u8 sdfDrawPublishFlag = 0;

u8 sdfGrDrawFrameId = 0;

u8 sdfGrBufferSlot = 0;

volatile s8 sdfReqDrawDelayCount = 0;

