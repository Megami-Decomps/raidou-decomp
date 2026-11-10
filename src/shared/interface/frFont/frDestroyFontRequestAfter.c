#include "shared/interface/frFont/decls.h"

/* Link without repositioning into the draw-buffer-indexed queue; return 0.
 * The low-byte buffer index is used directly, without a two-slot bounds check. */
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ) {
    frFontHandle_t **queueSlot = &gFT.glyphSlots[dds3GetFrameID() & 0xFF];

    *queueSlot = frCombineFontRequestEx(*queueSlot, pFRQ, 0);
    return 0;
}
