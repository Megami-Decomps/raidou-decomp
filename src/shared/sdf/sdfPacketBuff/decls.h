/* Declarations of smt3-decomp src/smt3/sdf/sdfPacketBuff.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFPACKETBUFF_H
#define SHARED_SDF_SDFPACKETBUFF_H

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

#endif
