#include "common.h"
#include "sdf_types.h"
#include "sdf_resource.h"
#include "sdfPacketBuff.h"
#include "sdfMemory.h"

/* Declared here because no header carries these. */
extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

struct sdfMemHandle_t;

#define SDF_QWORD_ALIGNMENT_MASK 0xF

#define SDF_PACKET_BUFFER_ALIGNMENT_MASK 0x7F

#define SDF_PACKET_BUFFER_COUNT 2

extern struct sdfMemHandle_t *sdfGrPacketBuffHandle;
extern s32 sdfGrPacketBuffSize;
extern s32 sdfGrPacketBuffBase[2];
extern s32 sdfGrPacketBuffPtr;
extern s32 sdfGrPacketBuffLimit;

void sdfGrMakePacketBuff(s32 aPacketBuffSize);
void sdfGrSetupPacketBuff(s32 aFrameId);
s32 sdfGrAllocPacketBuff(s32 aSize);
s32 sdfGrGetPacketBuffRemain(void);

void sdfGrMakePacketBuff(s32 aPacketBuffSize) {
    s32 allocationAddress;
    if (sdfGrPacketBuffHandle != 0) {
        sdfMemFreeDirect(sdfGrPacketBuffHandle);
        sdfGrPacketBuffHandle = 0;
    }
    aPacketBuffSize = (aPacketBuffSize + SDF_PACKET_BUFFER_ALIGNMENT_MASK) & ~SDF_PACKET_BUFFER_ALIGNMENT_MASK;
    sdfGrPacketBuffSize = aPacketBuffSize;
    sdfGrPacketBuffHandle = sdfMemAllocDirect(aPacketBuffSize * SDF_PACKET_BUFFER_COUNT);
    allocationAddress = EE_ADDR_S(sdfMemLock(sdfGrPacketBuffHandle));
    sdfGrPacketBuffBase[0] = allocationAddress;
    sdfGrPacketBuffBase[1] = allocationAddress + aPacketBuffSize;
}

void sdfGrSetupPacketBuff(s32 aFrameId) {
    sdfGrPacketBuffPtr = sdfGrPacketBuffBase[aFrameId];
    sdfGrPacketBuffLimit = sdfGrPacketBuffBase[aFrameId] + sdfGrPacketBuffSize;
}

s32 sdfGrGetPacketBuffRemain(void) {
    return sdfGrPacketBuffLimit - sdfGrPacketBuffPtr;
}

s32 sdfGrAllocPacketBuff(s32 aSize) {
    s32 packet;

    packet = sdfGrPacketBuffPtr;
    sdfGrPacketBuffPtr = sdfGrPacketBuffPtr + ((aSize + 0xfU) & 0xfffffff0);
    return packet;
}

s32 sdfGrOpenPacketBuff(void) {
    return sdfGrPacketBuffPtr;
}

void sdfGrClosePacketBuff(saddr_t aBuffPtr) {
    sdfGrPacketBuffPtr = (aBuffPtr + SDF_QWORD_ALIGNMENT_MASK) & ~SDF_QWORD_ALIGNMENT_MASK;
}
struct sdfMemHandle_t *sdfGrPacketBuffHandle = NULL;

s32 sdfGrPacketBuffSize = 0;

s32 sdfGrPacketBuffBase[2] = {0, 0};

s32 sdfGrPacketBuffPtr = 0;

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffLimit);

