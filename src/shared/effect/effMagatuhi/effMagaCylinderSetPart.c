#include "shared/effect/effMagatuhi/decls.h"

/* Store a per-slot packed color; the native path does not validate the index. */
void effMagaCylinderSetPart(dds3MagatuhiBill_tag *work, s32 index, u32 color) {
    work->slotColors[index] = color;
}
