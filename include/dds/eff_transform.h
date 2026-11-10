#ifndef DDS_EFF_TRANSFORM_H

#define DDS_EFF_TRANSFORM_H
#include "../basic.h"
#include "common.h"

/* D_003299C0 selects six-word tables; D_00329A20 is kind 1.
 * The last two words in every populated retail table are zero. */
typedef struct EffWorldOps {
    s32 (*create)(dds3Basic_t *); /* dds3AllocBasic accepts exactly 1. */
    void (*destroy)(dds3Basic_t *);
    s32 (*update)(dds3Basic_t *);
    s32 (*draw)(dds3Basic_t *);
    u32 unused10;
    u32 unused14;
} EffWorldOps;

/* Kind-1 value cursors use these halfwords at 00110490. The same prefix is
 * used by separate 0x10-byte world index nodes (0010FEC8). */
typedef struct dds_dds3BasicArrayHandle_t {
    s16 headIndex;
    s16 tailIndex;
    s16 cursorIndex;
    u16 entryCount;
} dds_dds3BasicArrayHandle_t;

typedef char WorldValueIndices_size_must_be_0x08[(sizeof(dds_dds3BasicArrayHandle_t) == 0x08) ? 1 : -1];

typedef char EffWorldOps_size_must_be_0x18[(sizeof(EffWorldOps) == 0x18) ? 1 : -1];

#endif
