#include "shared/interface/itfMesManager/decls.h"

/* Discard (to - from - 1) preceding rows when positive, retain the next
 * complete y-group and queue everything after it. Requires non-NULL input;
 * return NULL if discarding preceding rows exhausts the chain. */
frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *pfont, s32 index, s32 lines) {
    s32 rowsToDiscard = lines - index - 1;
    s32 rowY = pfont->y;
    frFontHandle_t *nextNode;
    frFontHandle_t *rowHead;
    frFontHandle_t *rowTail;

    while (rowsToDiscard > 0) {
        while (rowY == pfont->y) {
            nextNode = pfont->previous;
            pfont->next = NULL;
            pfont->chainHead = pfont;
            pfont->previous = NULL;
            frDestroyFontRequestAfter(pfont);
            pfont = nextNode;
            if (pfont == NULL) {
                return NULL;
            }
        }
        rowsToDiscard--;
        rowY = pfont->y;
    }
    rowHead = pfont;
    do {
        rowTail = pfont;
        pfont = pfont->previous;
    } while (pfont != NULL && rowY == pfont->y);
    while (pfont != NULL) {
        nextNode = pfont->previous;
        pfont->next = NULL;
        pfont->chainHead = pfont;
        pfont->previous = NULL;
        frDestroyFontRequestAfter(pfont);
        pfont = nextNode;
    }
    rowHead->next = NULL;
    rowTail->previous = NULL;
    for (pfont = rowHead; pfont != NULL; pfont = pfont->previous) {
        /* Despite its current name, this link points to the retained row tail. */
        pfont->chainHead = rowTail;
    }
    return rowHead;
}
