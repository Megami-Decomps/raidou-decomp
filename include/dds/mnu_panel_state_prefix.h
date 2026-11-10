#ifndef DDS_MNU_PANEL_STATE_PREFIX_H

#define DDS_MNU_PANEL_STATE_PREFIX_H
#include "mnu_panel_state.h"

typedef struct MenuPanelStatePrefix {
    u8 pad00[0xC];
    s32 width;
    s32 height;
    MenuPanelSelectionIndex selectedIndex;
    u32 firstValueA;
    u32 firstValueB;
} MenuPanelStatePrefix;

typedef char MenuPanelStatePrefixSizeCheck[(sizeof(MenuPanelStatePrefix) == 0x20) ? 1 : -1];

#endif
