#ifndef DDS_SDF_DEV_CONS_H

#define DDS_SDF_DEV_CONS_H
#include "common.h"

struct sdfMemHandle_t;

#if !defined(SMT3_DEFINES_sdfDevConsCB_tag)
typedef struct sdfDevConsCB_tag {
    struct sdfDevConsCB_tag *next; /* 0x00: forward console-list link. */
    struct sdfDevConsCB_tag *previousNode; /* 0x04: linked-list predecessor; the global head is the tail. */
    u16 screenOriginX; /* 0x08: GS 12.4 X coordinate used for the console sprite/grid. */
    u16 screenOriginY; /* 0x0A: GS 12.4 Y coordinate used for the console sprite/grid. */
    s16 columns; /* 0x0C */
    s16 rows; /* 0x0E */
    s16 cursorColumn; /* 0x10 */
    s16 cursorRow; /* 0x12 */
    u8 controlByte; /* 0x14 */
    u8 pad15;
    u8 textAttribute; /* 0x16 */
    u8 tabWidth; /* 0x17: used to advance tabs to the next column stop */
    struct sdfMemHandle_t *bufferAllocation; /* 0x18: owns the cell buffer. */
    u8 *cells; /* 0x1C: two bytes per character cell */
} sdfDevConsCB_tag;
#endif

#if !defined(SMT3_DEFINES_sdfDevConsDrawCB_t)
typedef struct sdfDevConsDrawCB_t {
    u8 *writeCursor; /* 0x00: next glyph-data write */
    u8 *packetStart; /* 0x04: beginning of the DMA range */
    u8 *currentTag; /* 0x08: reserved, not-yet-finalized GIF tag */
    vu32 *dmaChannel; /* 0x0C: DMA channel registers; CHCR is the first word. */
    u8 *bufferAddresses[2]; /* 0x10: start of each alternating packet buffer */
    s32 bufferBytes; /* 0x18: byte capacity of each buffer */
    u16 activeBufferIndex; /* 0x1C */
    u16 rowStep; /* 0x1E: added when the renderer advances to the next row */
} sdfDevConsDrawCB_t;
#endif

typedef char DevConsState_size_must_be_0x20[(sizeof(sdfDevConsCB_tag) == 0x20) ? 1 : -1];

typedef char ConsBuf_size_must_be_0x20[(sizeof(sdfDevConsDrawCB_t) == 0x20) ? 1 : -1];

#endif
