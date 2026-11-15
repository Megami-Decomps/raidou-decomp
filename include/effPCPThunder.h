#ifndef EFFPCPTHUNDER_H
#define EFFPCPTHUNDER_H

#include "common.h"

/* Parameter head (0x54 bytes) of the fragment effect, copied verbatim into the work. */
typedef struct effPCP_ThunderPolyParam_t {
    f32 start[4];
    f32 end[4];
    u16 blend;         /* 0x20 */
    u8 pad22[2];
    u32 number;       /* 0x24 */
    u32 restartFrameLimit;  /* 0x28: zero permits unlimited restarts */
    f32 waveAmplitude;      /* 0x2C: sinusoidal displacement */
    u32 startDelayRange;     /* 0x30 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x34 modulus of activeFrames before adding one */
    u16 split;            /* 0x38 */
    u8 pad3A[2];
    f32 coreWidth;          /* 0x3C: half-width of the primary strip */
    u32 color_center;     /* 0x40: packed center color used by symmetric cell ramps */
    f32 width_border;          /* 0x44: width of the continued strip */
    u32 color_border;    /* 0x48: packed middle color used by symmetric cell ramps */
    f32 width_edge;          /* 0x4C: added outer margin */
    u32 color_edge;      /* 0x50: packed edge color used by symmetric cell ramps */
} effPCP_ThunderPolyParam_t;

/* Paired-chain parameters; the points are filled before rendering. */
typedef struct effPCP_ThunderPolyListParam_t {
    f32 points[10][4];
    s32 count;
    effPCP_ThunderPolyParam_t TPParam;
} effPCP_ThunderPolyListParam_t;

typedef struct effPCP_ThunderPolyList_t effPCP_ThunderPolyList_t;

effPCP_ThunderPolyList_t *effPCP_ThunderPolyListCreate(effPCP_ThunderPolyListParam_t *src);
void effPCP_ThunderPolyListDelete(effPCP_ThunderPolyList_t *hData);
void effPCP_ThunderPolyListDraw(effPCP_ThunderPolyList_t *hData);

#endif
