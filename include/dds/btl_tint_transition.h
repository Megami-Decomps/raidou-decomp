#ifndef DDS_BTL_TINT_TRANSITION_H

#define DDS_BTL_TINT_TRANSITION_H
#include "common.h"

/* Observed view of the battle tint state; bytes +0x04..+0x17 remain opaque. */
typedef struct BtlTintTransition {
    u32 currentColor;          /* 0x00 */
    u8 opaque04[0x14];         /* 0x04 */
    u32 sourceColor;           /* 0x18 */
    u32 targetColor;           /* 0x1C */
    u16 framesRemaining;       /* 0x20 */
    u16 durationFrames;        /* 0x22 */
} BtlTintTransition;

#endif /* DDS_BTL_TINT_TRANSITION_H */
