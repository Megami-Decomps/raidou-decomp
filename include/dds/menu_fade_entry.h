#ifndef DDS_MENU_FADE_ENTRY_H

#define DDS_MENU_FADE_ENTRY_H
#include "common.h"

struct MenuWindowContainer;

/* Shared fade-list entry. The unobserved word at 0x04 remains opaque. */
typedef struct MenuFadeEntry {
    u32 active; /* 0x00 */
    u32 unknown_04;
    s32 x; /* 0x08: stored draw position. */
    s32 y; /* 0x0C: stored draw position. */
    struct MenuWindowContainer *window; /* 0x10 */
    s32 fade; /* 0x14 */
} MenuFadeEntry;

typedef char MenuFadeEntry_extent_must_be_0x18[
    (sizeof(MenuFadeEntry) == 0x18) ? 1 : -1];

#endif /* DDS_MENU_FADE_ENTRY_H */
