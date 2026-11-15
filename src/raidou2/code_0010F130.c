#include "common.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdk_libkernl.h"
#include "dds3GlobalWork.h"
#include "sdfPrim.h"
#include "frFont.h"
#include "sdfChip.h"
#include "sdfMemory.h"
s32 DIntr();
s32 EIntr(void);

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "event.h"
#include "kernel_draw.h"
#include "scr.h"
#include "sdfDevCons.h"
#include "sdf_packet_alloc.h"
#include "sdk_libc.h"
#include "dds/sdf_dev_cons.h"
#include "dds/sdf_general_heap_api.h"
#include "dds/sdf_heap_api.h"

void sdfDevConsAttribute(sdfDevConsCB_tag *console, u8 attribute);
void sdfDevConsClose(sdfDevConsCB_tag *node);
sdfDevConsCB_tag *sdfDevConsOpen(u32 screenOriginX, u32 screenOriginY, s32 columns, s32 rows);

/* Update the console state stored at the corresponding field offsets. */
void sdfDevConsSetMode(sdfDevConsCB_tag *console, u8 controlByte);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);

/* Return NULL when the following link is the end marker. */

/* Return the descriptor's payload base as a byte address, not the descriptor. */

/* Borrowed read-only descriptors; exclude heap mutation while traversing. */

/* Return free/used/end-sentinel state, not a resource kind. */




#include "dds/sdf_heap_overlay_block.h"
#include "dds/ee_mmi.h"

enum {
    EVT_PACKET_LIST_BYTES = 0x20,
    EVT_GS_COMMAND_BYTES = 0x30,
    EVT_GS_TEST_PRIMARY = 0x47,
    EVT_GS_TEST_SECONDARY = 0x48,
    EVT_GS_ALPHA_PRIMARY = 0x42,
    EVT_FIXED_ALPHA_INTERPOLATION = 0x64,
    EVT_QUAD_DEFAULT_DEPTH = 0xFFFFFF,
    EVT_FRAME_REFERENCE_PACKET_BYTES = 0x40,
    EVT_FRAME_DRAW_RECORD_BYTES = 0x1F40,
    EVT_BACKGROUND_BLEND_FLAG = 0x100,
    EVT_DRAW_COLOR_BLEND_FLAG = 0x200,
    EVT_DRAW_VECTOR_BLEND_FLAG = 0x400,
    EVT_SAVED_VECTOR_BLEND_FLAG = 0x4000,
    EVT_QUAD_DESCRIPTOR_BYTES = 0x2C,
    EVT_QUAD_VERTEX_COUNT = 4,
    EVT_SELECTION_ALLOCATION_BYTES = 0x28,
    EVT_DEV_CONSOLE_BUFFER_BYTES = 0x200,
    BF_FLW0_MAGIC = 0x30574C46
};

extern sdfDevConsCB_tag *evtConsoleFontResourceChain;
extern sdfGrOt_t sdfDebugGrOt;
extern s8 jmsChkCommonInputData4[];
extern void evtDrawHeapUsageOverlay(sdfGrOt_t *);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

/* The allocated draw state's float at +0x1C is initialized to 1. */
typedef struct EvtDrawState {
    u8 pad00[0x1C];
    f32 value1C;
} EvtDrawState;

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct EvtGsCommand {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} EvtGsCommand;

extern s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *);

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

u32 evtEnsureFontResourceChain(void);
void evtDestroyFontResourceChain(void);
extern u16 D_00435D40;
extern u16 D_00435D42;
extern char evtGeneralHeapPercentFormat[];
s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task);
extern u32 D_00435D4C;

void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 blockState);

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);
extern char D_00435D50[];
extern char D_00435D58[];
extern void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);
void evtDrawHeapUsageOverlay(sdfGrOt_t *surface);
s32 evtDrawConditionalHeapUsageOverlay(void);
extern s32 effGetFontListCount(void);
extern const char D_004113A0[];
s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);
extern s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);

u32 evtEnsureFontResourceChain(void) {
    if (evtConsoleFontResourceChain == 0) {
        evtConsoleFontResourceChain = sdfDevConsOpen(0x7100, 0x7A60, 0x28, 0x14);
        sdfDevConsSetMode(evtConsoleFontResourceChain, 2);
        sdfDevConsAttribute(evtConsoleFontResourceChain, 7);
    }
    return 0;
}

void evtDestroyFontResourceChain(void) {
    if (evtConsoleFontResourceChain != 0) {
        sdfDevConsClose(evtConsoleFontResourceChain);
        evtConsoleFontResourceChain = 0;
    }
}

INCLUDE_ASM(const s32, "code_0010F130", datDebugPrintf);

INCLUDE_ASM(const s32, "code_0010F130", datDEBUGCONSOLE);

s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task) {
    sdfMemInfo_t heapStats;
    sdfDrawTag_t *list;
    void *packet;
    s32 freePercent;
    s32 originX = 0x7100;
    s32 currentColumn;
    s32 peakColumn;

    sdfMemGetInfo(&heapStats);
    freePercent = heapStats.freeSize * 100 / heapStats.total;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70D0, 0x85A8, 0xFFFF7F, 0x1260, 0xC0, 0x20000000, 0x40806040));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x85C0, 0xFFFF80, 0, evtGeneralHeapPercentFormat, freePercent));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x8080, 0x85C0, 0xFFFF80, 6, evtGeneralHeapPercentFormat, D_00435D40));
    currentColumn = (s32)((f32)freePercent / 100.0f * 200.0f) + 0x2A;
    sdfPrimLinkRect(list, 0x8000C000, 0, 0x73A0, 0x85F0, currentColumn * 16 + originX, 0x8600, 0xFF0000, 0);
    peakColumn = (s32)((f32)D_00435D40 / 100.0f * 200.0f) + 0x2B;
    sdfPrimLinkRect(list, 0x8000FFFF, 0, peakColumn * 16 + originX, 0x85D0, (peakColumn + 2) * 16 + originX, 0x8610, 0xFF0000, 0);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (D_00435D42 >= 0x47 || D_00435D40 < freePercent) {
        D_00435D40 = freePercent;
        D_00435D42 = 0;
    } else {
        D_00435D42 = D_00435D42 + 1;
    }
    return 0;
}

void datLeaf41(void) {
}

INCLUDE_ASM(const s32, "code_0010F130", evtToggleDebugTimeGraphTask);

/* Render an allocation span as partial rows and complete heap-map rows. */
void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 blockState) {
    s32 color;
    u32 byteOffset;
    s32 rowIndex;
    s32 columnIndex;
    s32 leftX;
    s32 topY;
    u32 bytesPerRow;
    u32 bytesPerCell;

    switch (blockState) {
    case SDF_MEM_BLOCK_STATE_FREE:
        color = 0x80000000;
        break;
    case SDF_MEM_BLOCK_STATE_USED:
        color = 0x80008080;
        break;
    case SDF_MEM_BLOCK_STATE_END_SENTINEL:
        color = 0x80000080;
        break;
    default:
        color = 0x80800000;
        break;
    }
    byteOffset = blockAddress - heapBase;
    bytesPerRow = D_00435D4C >> 8;
    bytesPerCell = D_00435D4C >> 14;
    rowIndex = byteOffset / bytesPerRow;
    columnIndex = (byteOffset % bytesPerRow) / bytesPerCell;
    if (D_00435D4C < blockSize || blockSize < 0) {
        return;
    }

    topY = y + rowIndex * 8;
    leftX = x + columnIndex * 16;
    if (columnIndex > 0) {
        s32 remainingCells = 64 - columnIndex;

        if (blockSize >= remainingCells / bytesPerCell) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + remainingCells * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize -= remainingCells / (D_00435D4C >> 14);
        } else {
            s32 cellCount = blockSize / bytesPerCell;

            if (cellCount == 0 && blockSize > 0) {
                cellCount = 1;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize = 0;
        }
        topY += 8;
    }
    while (blockSize >= (D_00435D4C >> 8)) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + 0x400, topY + 8, 0x0FFFFF80, NULL);
        blockSize -= D_00435D4C >> 8;
        topY += 8;
    }
    if (blockSize > 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + (blockSize / (D_00435D4C >> 14)) * 16,
                                     topY + 8, 0x0FFFFF80, NULL);
    }
}

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y) {
    SdfHeapOverlayBlock *snapshot;
    SdfHeapOverlayBlock *entry;
    sdfMemHandle_t *block;
    u32 count;
    u32 i;
    u32 heapBase;
    u32 offset;
    s32 restoreInterrupts;
    s32 markerX;
    s32 markerY;

    snapshot = (SdfHeapOverlayBlock *)sdfGrOpenPacketBuff();
    restoreInterrupts = DIntr();
    block = sdfMemGetTop();
    entry = snapshot;
    heapBase = EE_ADDR(sdfMemGetPtr(block));
    /* Count only the interrupt-protected snapshot traversal. */
    count = 0;
    do {
        count++;
        entry->address = EE_ADDR(sdfMemGetPtr(block));
        entry->blockState = (u8)sdfMemGetType(block);
        entry->size = sdfMemGetSize(block);
        entry++;
        block = sdfMemGetNext(block);
    } while (block != NULL);
    if (restoreInterrupts) {
        EIntr();
    }
    sdfGrClosePacketBuff((s32)entry);

    entry = snapshot;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
        0x0FFFFF7F, 0x440, (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x20,
        0x80000000, 0x80806020));
    for (i = 0; i < count; i++) {
        sdfDrawHeapSpanOverlay(packetList, x, y, heapBase, entry->address,
            entry->size, entry->blockState);
        entry++;
    }
    markerY = y;
    markerX = x + 0x420;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        x + 0x4C0, markerY, 0x0FFFFF80, NULL);
    for (offset = 0x100000; offset < D_00435D4C; offset += 0x100000) {
        markerY = y + (offset / (D_00435D4C >> 8)) * 8;
        sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
            x + 0x470, markerY, 0x0FFFFF80, NULL);
    }
    markerY = y + (D_00435D4C / (D_00435D4C >> 8)) * 8;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        markerX + 0xA0, markerY, 0x0FFFFF80, NULL);
}

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(sdfGrOt_t *surface) {
    sdfMemInfo_t generalHeapStats;
    sdfChipInfo_t chipHeapStats;
    char statusText[100];
    sdfDrawTag_t *packetList;

    sdfMemGetInfo(&generalHeapStats);
    D_00435D4C = generalHeapStats.total;
    sdfChipGetInfo(&chipHeapStats);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawHeapMapOverlay(packetList, 0x8AC0, 0x79C0);
    sprintf(statusText, D_00435D50, generalHeapStats.freeSize);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A00, 0x0FFFFF80, 0, statusText));
    sprintf(statusText, D_00435D58, chipHeapStats.freeSize);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A60, 0x0FFFFF80, 0, statusText));
    surface->regFunc(surface, packetList);
}

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void) {
    if (jmsChkCommonInputData4[0] != 0) {
        return 0;
    }
    evtDrawHeapUsageOverlay(&sdfDebugGrOt);
    return 0;
}

s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task) {
    sdfMemInfo_t heapStats;
    s32 heapRatio;
    sdfDrawTag_t *list;
    void *packet;
    s32 fontCount;
    s32 textCount;
    s32 gsCount;

    sdfMemGetInfo(&heapStats);
    /* The unused heap ratio retains the native zero-divisor check. */
    heapRatio = heapStats.freeSize / heapStats.total;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70d0, 0x7968, 0xffff7f,
                                          0xf70, 0x98, 0x20000000, 0x40806040));
    fontCount = frGetFontHandleNum();
    textCount = frGetTextHandleNum();
    gsCount = effGetFontListCount();
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x7980,
                    0xffff80, 0, D_004113A0, fontCount, textCount, gsCount));
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    return 0;
}

INCLUDE_RODATA(const s32, "code_0010F130", evtStrFh03Th03Gs03);

INCLUDE_SDATA(const s32, "code_0010F130", evtConsoleFontResourceChain);

INCLUDE_SDATA(const s32, "code_0010F130", datData);

INCLUDE_SDATA(const s32, "code_0010F130", datDEBUGCONSOLEData);

INCLUDE_SDATA(const s32, "code_0010F130", evtDrawGeneralHeapUsageGraphData);

INCLUDE_SDATA(const s32, "code_0010F130", evtGeneralHeapPercentFormat);

INCLUDE_SDATA(const s32, "code_0010F130", sdfData2);

INCLUDE_SDATA(const s32, "code_0010F130", evtDrawHeapUsageOverlayData);

INCLUDE_SDATA(const s32, "code_0010F130", evtDrawHeapUsageOverlayData2);

