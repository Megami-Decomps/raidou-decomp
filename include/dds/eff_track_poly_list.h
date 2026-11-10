#ifndef DDS_EFF_TRACK_POLY_LIST_H

#define DDS_EFF_TRACK_POLY_LIST_H
#include "eff.h"

#if !defined(SMT3_DEFINES_effMTPolyListHandle_t)
/* Retained pointer array used by particle kind 4. */
typedef struct effMTPolyListHandle_t {
    EffTrackPolyWork **items;
    u32 count;
    sdfMemHandle_t *handle;
} effMTPolyListHandle_t;
#endif

typedef char EffTrackPolyListSizeCheck[
    (sizeof(effMTPolyListHandle_t) == 0x0C) ? 1 : -1];

#endif
