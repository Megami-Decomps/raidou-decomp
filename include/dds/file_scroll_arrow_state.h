#ifndef DDS_FILE_SCROLL_ARROW_STATE_H

#define DDS_FILE_SCROLL_ARROW_STATE_H
#include "common.h"

/* Shared menu scroll-arrow animation state. The client updates the angle and
 * directional alpha words; the word at +8 remains opaque. */
typedef struct FileScrollArrowState {
    s32 angle;
    s32 upAlpha;
    s32 unk08;
    s32 downAlpha;
} FileScrollArrowState;

typedef char FileScrollArrowStateSizeCheck[
    (sizeof(FileScrollArrowState) == 0x10) ? 1 : -1];

#endif /* DDS_FILE_SCROLL_ARROW_STATE_H */
