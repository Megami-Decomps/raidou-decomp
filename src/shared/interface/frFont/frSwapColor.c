#include "shared/interface/frFont/decls.h"

/* Reverse the four byte positions of a 32-bit word. */
u32 frSwapColor(u32 Col) {
    u32 shiftedWord = Col >> FR_FONT_BYTE_SHIFT;
    u32 lowerMiddleByte = Col & FR_FONT_MIDDLE_BYTE_MASK;
    u32 outerBytes = Col << FR_FONT_OUTER_BYTE_SHIFT;

    Col >>= FR_FONT_OUTER_BYTE_SHIFT;
    shiftedWord &= FR_FONT_MIDDLE_BYTE_MASK;
    lowerMiddleByte <<= FR_FONT_BYTE_SHIFT;
    outerBytes |= Col;
    shiftedWord |= lowerMiddleByte;
    return outerBytes | shiftedWord;
}
