#ifndef DDS_FILE_LOAD_MENU_STATE_H

#define DDS_FILE_LOAD_MENU_STATE_H
#include "common.h"

/* Save-slot selection motion: a one-tick-at-a-time scroll advances or retreats
 * the selected slot while the visible rows slide in the opposite direction. */
typedef s8 FileLoadMenuSlideMode;

enum {
    FILE_LOAD_MENU_SLIDE_IDLE = 0,
    FILE_LOAD_MENU_SLIDE_SELECTION_DOWN = 1,
    FILE_LOAD_MENU_SLIDE_SELECTION_UP = 2
};

typedef struct FileLoadMenuState {
    u8 unk00[4];              /* 0x00: opaque */
    s32 transitionTickCount;  /* 0x04 */
    s32 inputRepeatTimer;     /* 0x08 */
    s32 standardRowAlpha;     /* 0x0C: clamped to 0..0x80 for ordinary rows */
    FileLoadMenuSlideMode slideMode; /* 0x10: idle or selection-scroll direction */
    u8 pad11[3];              /* 0x11 */
    s32 rowVerticalOffset;    /* 0x14 */
    s32 leadingRowAlpha;      /* 0x18 */
    s32 trailingRowAlpha;     /* 0x1C */
} FileLoadMenuState;

typedef char FileLoadMenuState_size_must_be_0x20[
    (sizeof(FileLoadMenuState) == 0x20) ? 1 : -1];

#endif
