#include "common.h"
#include "fldProcess.h"
#include "code_0016A558.h"

void mapIsFldKeyFreeSub(void) {
    mapCallSetOt(1);
}

INCLUDE_ASM(const s32, "code_0016A558", mapLeaf2);

void mapIfMenuInit(void) {
    mapMenuWin_t *win = &mapWork11[1];

    mapBadMafuHajikiData2 = 4;
    win->state = 1;
    win->scroll = 0;
    win->sel = 0;
    win->devilBase = 0;
    win->phase = 0;
    win->x = 300;
    win->y = 32;
    win->w = 192;
    win->h = 176;
    win->unk24 = 2;
    win->rows = 7;
}

INCLUDE_ASM(const s32, "code_0016A558", mapIgnite2);
