#include "shared/interface/itfMesManager/decls.h"

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesMngSetMesPosition(s32 id, s32 x, s32 y) {
    message_tag *message = &pinst[id].mes->message;
    s32 delta[2];

    delta[0] = x - message->x;
    delta[1] = y - message->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(message->pfont, delta[0], delta[1]);
    message->x = x;
    message->y = y;
}
