#include "shared/interface/itfMesManager/decls.h"

/* Translate primary text and update its cached position, including zero deltas. */
void itfMesMngSetSpeakerPositionOffset(s32 id, s32 x, s32 y) {
    Speaker_tag *primaryTextBlock = &pinst[id].mes->speaker;

    ChangeFontPosition(primaryTextBlock->pfont, x, y);
    primaryTextBlock->x += x;
    primaryTextBlock->y += y;
}
