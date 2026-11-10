#ifndef DDS_SDF_GS_CONTEXT_H

#define DDS_SDF_GS_CONTEXT_H
#include "sdf_gs_packet.h"

#include "sdf_gs_context_selector.h"

/* Four A+D writes define one GS drawing context, without a DMA/GIF header. */
typedef struct SdfGsContextRegisters {
    SdfGsRegisterWrite frame;
    SdfGsRegisterWrite zbuf;
    SdfGsRegisterWrite xyoffset;
    SdfGsRegisterWrite scissor;
} SdfGsContextRegisters;

typedef char SdfGsContextRegisters_size_must_be_0x40[
    (sizeof(SdfGsContextRegisters) == 0x40) ? 1 : -1];

/* Accessed output span of the common scene-context builder. */
typedef struct SdfGsContextPacket {
    SdfGsPacketHeader header;
    SdfGsContextRegisters registers;
} SdfGsContextPacket;

typedef char SdfGsContextPacket_size_must_be_0x60[
    (sizeof(SdfGsContextPacket) == 0x60) ? 1 : -1];

#endif
