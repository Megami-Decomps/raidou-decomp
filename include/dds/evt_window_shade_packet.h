#ifndef DDS_EVT_WINDOW_SHADE_PACKET_H

#define DDS_EVT_WINDOW_SHADE_PACKET_H
#include "sdf_dma_tag.h"

/* One A+D register write: the value quadword followed by the GS register id. */
typedef struct EvtShadeRegister {
    u64 value;
    u64 registerId;
} EvtShadeRegister;

/* Per-draw-buffer DMA packet: one tag followed by ten A+D writes. */
typedef struct EvtWindowShadePacket {
    SdfDmaTagHeader tag;
    EvtShadeRegister regs[10];
} EvtWindowShadePacket;

typedef char EvtShadeRegisterSizeCheck[(sizeof(EvtShadeRegister) == 0x10) ? 1 : -1];

typedef char EvtWindowShadePacketSizeCheck[(sizeof(EvtWindowShadePacket) == 0xB0) ? 1 : -1];

#endif
