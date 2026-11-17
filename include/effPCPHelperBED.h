#ifndef EFFPCPHELPERBED_H
#define EFFPCPHELPERBED_H

#include "common.h"

struct effBTL_t;
struct effEveBEDCut_t;
struct effPartHandle_t;
struct sdfMemHandle_t;

/* Serialized model placement and fade parameters copied into the owner. */
typedef struct effPCPHelperBEDParam_t {
    f32 pos[4];
    s32 fade_in;
    s32 fade_out;
    f32 speed;
} effPCPHelperBEDParam_t;

/* One event per model-map position. Init seeds the entry, then the updater
 * latches its initial position and advances the event through its frame span. */
typedef struct effPCPHelperBEDPart_tag {
    f32 pos[4];
    u8 move;
    u8 pad11[3];
    s32 frame;
    s32 life;
    struct effEveBEDCut_t *hBEDCut;
} effPCPHelperBEDPart_tag;

/* The factory allocates this owner as one 0x40-byte size-class block; the
 * per-position entries are a separate count * 0x20 allocation. */
typedef struct effPCPHelperBED_tag {
    effPCPHelperBEDParam_t param;
    effPCPHelperBEDPart_tag *pPart;
    struct sdfMemHandle_t *hPartMem;
    struct effBTL_t *hBED;
    struct effPartHandle_t *hHelper;
    f32 scale;
    s32 frame;
    s32 frameLimit;
    u32 count;
    u32 color;
} effPCPHelperBED_tag;

#endif
