#ifndef DDS_FLD_NPC_PALETTE_H

#define DDS_FLD_NPC_PALETTE_H
#include "common.h"

typedef struct FldNpcPaletteEntry {
    u8 colorA[3];
    u8 colorB[3];
    u8 textPair;
    u8 phase;
} FldNpcPaletteEntry;

typedef struct FldNpcPalette {
    FldNpcPaletteEntry entries[0x40];
} FldNpcPalette;

void fldSetNpcPalette(const FldNpcPalette *source);

#endif /* DDS_FLD_NPC_PALETTE_H */
