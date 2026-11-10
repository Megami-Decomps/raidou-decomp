#ifndef DDS_EVT_FLOAT_PANEL_ROW_H

#define DDS_EVT_FLOAT_PANEL_ROW_H
#include "common.h"

/* Four-entry XY editor table; the leading halfword is not consumed here. */
typedef struct EvtFloatPanelRow {
    u16 opaque00;
    u16 y;
    s32 commandIndex;
    const char *format;
} EvtFloatPanelRow;

typedef char EvtFloatPanelRow_size[(sizeof(EvtFloatPanelRow) == 0x0C) ? 1 : -1];

#endif /* DDS_EVT_FLOAT_PANEL_ROW_H */
