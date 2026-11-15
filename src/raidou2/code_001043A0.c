#include "common.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "dds3GlobalWork.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "libsdf.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "dds/btl.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/evt_frame_state_packet.h"

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);


/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

/* Return borrowed endpoints; callers may inspect list nodes but do not own them. */
sdfGsMemSlice_t *sdfGsMemGetLinkHead(void);

void sdfPrimLinkLine(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkRect(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);

/* The active resource registry contains sdfTexHandle_t nodes; lookup compares the
 * texture's resource key and follows its previous-node link. A match is borrowed:
 * lookup does not retain it, and callers must not release it as an acquired ref. */
extern struct sdfTexHandle_t *sdfTexHandleLink;

#include "dds/dds3_named_slot.h"

extern u32 dds3TextureViewerPageIndex;
extern u8 D_003BA890;
extern s8 jmsChkCommonInputData4[];
extern sdfGrOt_t sdfDebugGrOt;
extern sdfTexHandle_t *dds3CurrentIncompleteResource;
extern s32 dds3TextureViewerHandlePad(void);
extern void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image);
extern void dds3TextureAttachTask(sdfGrOt_t *surface);
extern void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);
extern s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);
extern dds3ProcessID_t *dds3DebugGraphTask;
extern s32 dds3TextureCountIncompleteResources(void);
extern s32 dds3SelectIncompleteResourceAtPageIndex(s32);
extern fclNamedSlot_t D_003C1C90[];

/* One shared initializer for the three configure modes and unregister. */
static inline void dds3SetNamedSlotState(s32 index, s32 kind, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy,
                                         void (*setState)(s32), void *userValue) {
    D_003C1C90[index].kind = kind;
    D_003C1C90[index].task = NULL;
    D_003C1C90[index].update = update;
    D_003C1C90[index].destroy = destroy;
    D_003C1C90[index].setState = setState;
    D_003C1C90[index].enabled = 0;
    D_003C1C90[index].userValue = userValue;
}

#define DDS3_PAD_INPUTS_PER_SET 16

#define DDS3_PAD_REPEAT_BIT 2

#define DDS3_ALT_CONFIRM_INPUT 0x11

#define DDS3_ALT_CANCEL_INPUT 0x13

#define DDS3_PAD_SMALL_MOTOR 0

#define DDS3_PAD_LARGE_MOTOR 1

#define DDS3_PAD_MOTOR_COUNT 2

#define DDS3_DEBUG_RUMBLE_PERIOD 0x4650

#define DDS3_DEBUG_SMALL_PULSE_FRAMES 0xF

#define DDS3_DEBUG_LARGE_PULSE_FRAMES 30

#define DDS3_DEBUG_RUMBLE_VARIATION 150

#define DDS3_DEBUG_RUMBLE_BASE 100

#define DDS3_SHARED_RANDOM_SEED 0x12345678

#define DDS3_CELL_COLUMN_SPAN 0xC0

#define DDS3_CELL_ROW_SPAN 0x60

#define DDS3_PREVIEW_MAX_TEXELS 0x100

#define DDS3_PREVIEW_WRAP_TEXELS 0x400

#define DDS3_DIAG_DEPTH 0x0FFFFF80

extern s16 dds3FrameRunTime;
extern u16 sdfDrawFrameCounter;
extern u16 D_003BA880;
extern u16 D_003BA882;
extern u16 D_003BA884;
extern u16 D_003BA886;
extern char D_003BA888[];
s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);
void dds3DebugGraphSetEnabled(s8 enabled);

/* Scale the longer preview side to at most 256 texels, preserving division
 * before coordinate scaling; UV endpoints at 1024 texels are reduced by one. */
void dds3DrawImageOutline(sdfDrawTag_t *packetList, sdfTexHandle_t *image);
extern u32 dds3TextureGetPageIndex(void);
void dds3TextureDrawPageCounter(sdfDrawTag_t *packetList);
extern const char *dds3DrawTextureResourceDetailsData[];
void dds3DrawTextureResourceDetails(sdfDrawTag_t *packetList, sdfTexHandle_t *texture);

#define DDS3_DIAG_PACKET_LIST_BYTES 0x20

#define DDS3_VIEWER_FAST_PAGE_STEP 10

#define DDS3_MAP_ROW_UNITS 0x1000

#define DDS3_MAP_ROW_SHIFT 12

#define DDS3_MAP_ROW_MASK 0xFFF

#define DDS3_MAP_COLUMN_SHIFT 5

#define DDS3_MAP_COLUMN_UNITS 32

#define DDS3_MAP_COLUMN_COUNT 128

#define DDS3_MAP_ROW_SPAN 0x800

#define DDS3_MAP_BORDER_WIDTH 0x840

#define DDS3_MAP_CELL_X_STEP 16

#define DDS3_MAP_ROW_Y_STEP 8

#define DDS3_MAP_CELL_X_SHIFT 4

#define DDS3_MAP_GREEN 0x80008000

#define DDS3_MAP_YELLOW 0x80008080

#define DDS3_MAP_RED 0x80000080

#define DDS3_MAP_BLUE 0x80800000

#define DDS3_MAP_CYAN 0x80808000

#define DDS3_HELD_TEXTURE_CLAMP_MODE 5

/* Count texture entries without a reference clone source. */
s32 dds3TextureCountIncompleteResources(void);
u32 dds3TextureGetPageIndex(void);

/* Clamp a negative request and return the reached index. DDS1 index zero
 * selects the head without scanning clone-source links; tail exits retain prior writes. */
s32 dds3SelectIncompleteResourceAtPageIndex(s32 requestedIndex);

/* Exit input returns 0 immediately; otherwise apply one prioritized page step,
 * select its reached index, then optionally toggle the viewer mode and return 1. */
s32 dds3TextureViewerHandlePad(void);
extern s32 D_003BA8E0;
void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode);
void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image);
s32 dds3SwapActiveResource(sdfGrOt_t *surface);
s32 dds3LoadDefaultResource(void);
void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);
void dds3TextureAttachTask(sdfGrOt_t *surface);
s32 dds3EnsureDefaultResource(void);

#define DDS3_FADE_TICKS_PER_FRAME 10

#define DDS3_FADE_MARK_NUMERATOR 4

#define DDS3_FADE_COUNT_DOWN_BIT 1

#define DDS3_FADE_COUNT_UP_BIT 2

#define DDS3_FADE_DIRECTION_BITS 3

#define DDS3_FADE_MAX_ALPHA 0x80

#define DDS3_FADE_ALPHA_SHIFT 7

#define DDS3_COLOR_ALPHA_CHANNEL 3

#define DDS3_BGFADE_COUNT_DOWN_BIT 0x04000000

#define DDS3_BGFADE_COUNT_UP_BIT 0x08000000

#define DDS3_BGFADE_DIRECTION_BITS 0x0C000000

#define DDS3_BGFADE_KEEP_OTHER_BITS 0xF3FFFFFF

#define DDS3_BGFADE_CLEAR_UP_MASK 0xF7FFFFFF

#define DDS3_BGFADE_CLEAR_DOWN_MASK 0xFBFFFFFF

#define DDS3_BGFADE_FIRST_RAMP_MAX 0x31

#define DDS3_BGFADE_SECOND_RAMP_MAX 0x4F

#define DDS3_BGFADE_FIRST_RAMP_SCALE 49.0f

#define DDS3_BGFADE_SECOND_RAMP_SCALE 79.0f

s32 dds3DebugGraphUpdate(dds3ProcessID_t *task) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfPrimTransPacket_t *blendPacket;
    s32 originX = 0x7100;
    /* Keep the four real conversion epochs in one local; see docs/idioms.md,
     * "Trace-justified pixel conversion reuse: dds3DebugGraphUpdate". */
    s32 pixels;

    sdfDrawTagInit(packetList);
    blendPacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(blendPacket);
    sdfDrawTagLink(packetList, (u32)blendPacket);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x70D0, 0x85A8,
                    0xFFFF7F, 0x1260, 0xF0, 0x20000000, 0x40806040));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(originX,
                    0x85C0, 0xFFFF80, 0, D_003BA888, (u16)dds3FrameRunTime));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(originX,
                    0x8620, 0xFFFF80, 0, D_003BA888, sdfDrawFrameCounter));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x85C0, 0xFFFF80, 6, D_003BA888, D_003BA880));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x8620, 0xFFFF80, 6, D_003BA888, D_003BA882));
    pixels = (s32)((f32)(u16)dds3FrameRunTime / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000C000, 0, 0x73A0, 0x85F0,
                    originX + ((pixels + 0x2A) << 4),
                    0x8600, 0xFF0000, NULL);
    pixels = (s32)((f32)sdfDrawFrameCounter / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x80000080, 0, 0x73A0, 0x8650,
                    originX + ((pixels + 0x2A) << 4),
                    0x8660, 0xFF0000, NULL);
    pixels = (s32)((f32)D_003BA880 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x85D0,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8610, 0xFF0000, NULL);
    pixels = (s32)((f32)D_003BA882 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x8630,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8670, 0xFF0000, NULL);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packetList);
    if (D_003BA884 >= 0x47 || D_003BA880 < (u16)dds3FrameRunTime) {
        D_003BA880 = dds3FrameRunTime;
        D_003BA884 = 0;
    } else {
        D_003BA884++;
    }
    if (D_003BA886 >= 0x47 || D_003BA882 < sdfDrawFrameCounter) {
        D_003BA882 = sdfDrawFrameCounter;
        D_003BA886 = 0;
    } else {
        D_003BA886++;
    }
    return 0;
}

void dds3DebugGraphTaskDestroy(void) {
}

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void dds3DebugGraphSetEnabled(s8 enabled) {
    if (enabled == 1) {
        dds3DebugGraphTask = dds3StartProcess("DebugTimeGrph", 0x2710, 1, 1, dds3DebugGraphUpdate, dds3DebugGraphTaskDestroy, NULL);
    } else if (enabled == 0) {
        dds3KillProcess(dds3DebugGraphTask, 0);
    }
}

INCLUDE_ASM(const s32, "code_001043A0", dds3CallDLinkTexPacket);

/* Append the zero-based viewer page and final page index; an empty list prints -1. */
void dds3TextureDrawPageCounter(sdfDrawTag_t *packetList) {
    char pageText[0x70];
    s32 pageIndex = dds3TextureGetPageIndex();
    s32 resourceCount = dds3TextureCountIncompleteResources();
    sprintf(pageText, "TEX VIEWER [%d/%d]", pageIndex, resourceCount - 1);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x79C0, DDS3_DIAG_DEPTH, 0, pageText));
}

INCLUDE_RODATA(const s32, "code_001043A0", dds3StrPsmct16S);

void dds3DrawTextureResourceDetails(sdfDrawTag_t *packetList, sdfTexHandle_t *texture) {
    char text[0x70];
    s32 formatIndex = 0;
    s32 formatMode = 0;

    sprintf(text, "W:%4d H:%4d", texture->width, texture->height);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x82A0, 0x0FFFFF80, 0, text));
    switch (texture->pixelFormat) {
    case 0: formatIndex = 0; formatMode = 6; break;
    case 1: formatIndex = 1; formatMode = 6; break;
    case 2: formatIndex = 2; formatMode = 6; break;
    case 10: formatIndex = 3; formatMode = 6; break;
    case 19: formatIndex = 4; formatMode = 4; break;
    case 20: formatIndex = 5; formatMode = 5; break;
    case 27: formatIndex = 6; formatMode = 4; break;
    case 36: formatIndex = 7; formatMode = 5; break;
    case 44: formatIndex = 8; formatMode = 5; break;
    default: break;
    }
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x8300, 0x0FFFFF80,
                                                                formatMode, dds3DrawTextureResourceDetailsData[formatIndex]));
    sprintf(text, "ID[%2d,%2d]", texture->texId, texture->clutId);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x77C0, 0x8300, 0x0FFFFF80, 0, text));
}

/* Count texture entries that are not duplicates of another texture. */
s32 dds3TextureCountIncompleteResources(void) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *nextTexture;
    s32 incompleteCount = 0;
    while (texture != NULL) {
        while (texture->ref != NULL && texture->ref->origin != NULL) {
            nextTexture = texture->next;
            if (nextTexture == NULL) {
                return incompleteCount;
            }
            texture = nextTexture;
        }
        incompleteCount++;
        texture = texture->next;
    }
    return incompleteCount;
}

u32 dds3TextureGetPageIndex(void) {
    return dds3TextureViewerPageIndex;
}

/* Clamp a negative request and return the reached index; index zero selects the head
 * without scanning duplicates, and tail exits keep the prior selection writes. */
s32 dds3SelectIncompleteResourceAtPageIndex(s32 requestedIndex) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *nextTexture;
    s32 visitedIndex = 0;

    if (requestedIndex < 0) {
        requestedIndex = 0;
    }
    while (texture != NULL && visitedIndex != requestedIndex) {
        dds3CurrentIncompleteResource = texture;
        while (texture->ref != NULL && texture->ref->origin != NULL) {
            nextTexture = texture->next;
            if (nextTexture == NULL) {
                return visitedIndex;
            }
            texture = nextTexture;
        }
        nextTexture = texture->next;
        if (nextTexture == NULL) {
            return visitedIndex;
        }
        visitedIndex++;
        texture = nextTexture;
    }
    dds3CurrentIncompleteResource = texture;
    return visitedIndex;
}

INCLUDE_ASM(const s32, "code_001043A0", dds3Func13);

void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image) {
    sdfDrawTag_t *packetList;
    s32 counter;
    s32 mode;

    if (image == NULL) {
        return;
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x79B0, 0x0FFFFF7F, 0x1040, 0x80, 0x80000000, 0x80806020));
    dds3TextureDrawPageCounter(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x7A50, 0x0FFFFF7F, 0x1040, 0x820, 0x80000000, 0x80806020));
    dds3DrawImageOutline(packetList, image);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x8290, 0x0FFFFF7F, 0x1040, 0xE0, 0x80000000, 0x80806020));
    dds3DrawTextureResourceDetails(packetList, image);

    if (D_003BA890 != 0) {
        dds3DrawTextureListDiagnostic(packetList, 0x8240, 0x7A60);
        counter = D_003BA8E0;
        mode = (counter >> 4) & 1;
        if (image->pixelGsHandle != NULL) {
            dds3DrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->pixelGsHandle, mode);
        }
        counter = D_003BA8E0;
        /* Boolean highlight selection; see docs/idioms.md, "Trace-justified boolean extraction: texture viewer highlight". */
        mode = (counter & 0x10) ? 2 : 0;
        if (image->clutGsHandle != NULL) {
            dds3DrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->clutGsHandle, mode);
        }
        D_003BA8E0++;
    }
    surface->regFunc(surface, packetList);
}

/* With a nonempty list, invoke its renderer even when input reports exit.
 * The surface owns the packet submission callback; return input status. */
s32 dds3SwapActiveResource(sdfGrOt_t *surface) {
    s32 inputStatus;

    if (sdfTexHandleLink == 0) {
        return 0;
    }
    inputStatus = dds3TextureViewerHandlePad();
    dds3DrawTextureViewerFrame(surface, dds3CurrentIncompleteResource);
    return inputStatus;
}

/* Use the default surface: empty list returns 0, exit input -1, drawing 0. */
s32 dds3LoadDefaultResource(void) {
    if (sdfTexHandleLink == 0) {
        return 0;
    }
    if (dds3TextureViewerHandlePad() == 0) {
        return -1;
    }
    dds3DrawTextureViewerFrame(&sdfDebugGrOt, dds3CurrentIncompleteResource);
    return 0;
}

INCLUDE_ASM(const s32, "code_001043A0", dds3Leaf23);

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode) {
    s32 color;
    u32 remainingUnits;
    s32 columnIndex;
    s32 rowUnits;
    s32 leftX;
    s32 topY;
    s32 cellCount;

    if (mode == 0) {
        switch ((s32)block->type) {
        case 1:
            color = DDS3_MAP_GREEN;
            break;
        case 2:
            color = DDS3_MAP_YELLOW;
            break;
        case 3:
            color = DDS3_MAP_RED;
            break;
        default:
            color = 0;
            break;
        }
    } else {
        color = mode == 1 ? DDS3_MAP_BLUE : DDS3_MAP_CYAN;
    }
    remainingUnits = block->size;
    topY = y + (block->addr >> DDS3_MAP_ROW_SHIFT) * DDS3_MAP_ROW_Y_STEP;
    columnIndex = (block->addr & DDS3_MAP_ROW_MASK) >> DDS3_MAP_COLUMN_SHIFT;
    leftX = x + columnIndex * DDS3_MAP_CELL_X_STEP;
    if (columnIndex > 0) {
        rowUnits = (DDS3_MAP_COLUMN_COUNT - columnIndex) * DDS3_MAP_COLUMN_UNITS;
        if (remainingUnits >= rowUnits) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + (DDS3_MAP_COLUMN_COUNT - columnIndex) * DDS3_MAP_CELL_X_STEP, topY + DDS3_MAP_ROW_Y_STEP,
                                         DDS3_DIAG_DEPTH, NULL);
            remainingUnits -= rowUnits;
        } else {
            cellCount = remainingUnits >> DDS3_MAP_COLUMN_SHIFT;
            if (cellCount == 0) {
                cellCount = remainingUnits != 0;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * DDS3_MAP_CELL_X_STEP, topY + DDS3_MAP_ROW_Y_STEP,
                                         DDS3_DIAG_DEPTH, NULL);
            remainingUnits = 0;
        }
        topY += DDS3_MAP_ROW_Y_STEP;
    }
    while (remainingUnits >= DDS3_MAP_ROW_UNITS) {
        sdfPrimLinkLine(packetList, color, 0, x, topY, x + DDS3_MAP_ROW_SPAN, topY + DDS3_MAP_ROW_Y_STEP,
                                     DDS3_DIAG_DEPTH, NULL);
        remainingUnits -= DDS3_MAP_ROW_UNITS;
        topY += DDS3_MAP_ROW_Y_STEP;
    }
    if (remainingUnits != 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + ((remainingUnits >> DDS3_MAP_COLUMN_SHIFT) << DDS3_MAP_CELL_X_SHIFT), topY + DDS3_MAP_ROW_Y_STEP,
                                     DDS3_DIAG_DEPTH, NULL);
    }
}

/* Draw the allocation-map border, then walk blocks through their prev links. */
void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y) {
    sdfGsMemSlice_t *block = sdfGsMemGetLinkHead();

    if (block != NULL) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
                                          0x0FFFFF7F, DDS3_MAP_BORDER_WIDTH, 0x820,
                                          0x80000000, 0x80806020));
        do {
            dds3DrawTextureAllocationMapBlock(packetList, x, y, block, 0);
            block = block->next;
        } while (block != NULL);
    }
}

/* Build an allocation-map packet list and submit it through the surface callback. */
void dds3TextureAttachTask(sdfGrOt_t *surface) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(DDS3_DIAG_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    dds3DrawTextureListDiagnostic(packetList, 0x7180, 0x79C0);
    surface->regFunc(surface, packetList);
}

/* Submit the default surface's map only when the control array's first byte is zero. */
s32 dds3EnsureDefaultResource(void) {
    if (jmsChkCommonInputData4[0] != 0) {
        return 0;
    }
    dds3TextureAttachTask(&sdfDebugGrOt);
    return 0;
}

INCLUDE_ASM(const s32, "code_001043A0", dds3InitDrawEnvSub);
INCLUDE_SDATA(const s32, "code_001043A0", dds3DebugGraphUpdateData);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DebugGraphUpdateData2);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DebugGraphUpdateData3);

INCLUDE_SDATA(const s32, "code_001043A0", dds3Func13Data);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureResourceDetailsDataEntry);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureResourceDetailsDataEntry2);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureResourceDetailsDataEntry3);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureResourceDetailsDataEntry4);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureResourceDetailsDataEntry5);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureResourceDetailsDataEntry6);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureResourceDetailsDataEntry7);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureResourceDetailsDataEntry8);

INCLUDE_SDATA(const s32, "code_001043A0", dds3Data27);

INCLUDE_SDATA(const s32, "code_001043A0", dds3Func13Data2);

INCLUDE_SDATA(const s32, "code_001043A0", dds3DrawTextureViewerFrameData);

INCLUDE_SDATA(const s32, "code_001043A0", dds3Data48);

