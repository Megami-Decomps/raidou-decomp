#include "shared/interface/itfMesManager/decls.h"

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesMngSetSpeakerPosition(s32 id, s32 x, s32 y) {
    Speaker_tag *primaryTextBlock = &pinst[id].mes->speaker;
    s32 delta[2];

    delta[0] = x - primaryTextBlock->x;
    delta[1] = y - primaryTextBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(primaryTextBlock->pfont, delta[0], delta[1]);
    primaryTextBlock->x = x;
    primaryTextBlock->y = y;
}
