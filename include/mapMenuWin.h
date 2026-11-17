#ifndef MAPMENUWIN_H
#define MAPMENUWIN_H

#include "common.h"

typedef struct mapMenuWin_t {
    s32 state;      /* 0 active, 1 hidden */
    f32 scroll;
    s32 sel;
    s32 devilBase;
    f32 phase;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 unk24;
    s32 rows;       /* sel wraps modulo this */
    s32 unk2C;
} mapMenuWin_t;

extern mapMenuWin_t mapWork11[];

#endif /* MAPMENUWIN_H */
