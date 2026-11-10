#include "shared/interface/fmGslCont/decls.h"

/* Insert after the list's fixed anchor and update the entry count. */
void fmRetFontMem(FontMem_t *pFontArea) {
    FontMem_t *anchor = gFGC.root;
    FontMem_t *nextNode = anchor->next;

    pFontArea->prev = anchor;
    pFontArea->next = nextNode;
    gFGC.unit_nums += 1;
    anchor->next = pFontArea;
    nextNode->prev = pFontArea;
}
