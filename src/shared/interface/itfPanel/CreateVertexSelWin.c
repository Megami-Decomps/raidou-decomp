#include "shared/interface/itfPanel/decls.h"

void CreateVertexSelWin(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[ITF_PANEL_COLUMN_COUNT];
    s32 ys[4];
    s32 i;
    s32 j;

    xs[0] = x0 - 0x100;
    xs[1] = x0 + ITF_PANEL_COLUMN_INSET;
    xs[2] = x1 - ITF_PANEL_COLUMN_INSET;
    xs[3] = x1 + 0x100;
    ys[0] = y0 - 0x80;
    ys[1] = y0 + 0x80;
    ys[2] = y1 - 0x80;
    ys[3] = y1 + 0x80;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            pwork->x = xs[j];
            pwork->y = ys[i];
            pwork++;
        }
    }
    xs[0] = x0 - 0x100;
    xs[1] = x0 + 0x400;
    xs[2] = x1 - 0x400;
    xs[3] = x1 + 0x100;
    ys[0] = y0;
    for (i = 0; i < ITF_PANEL_ROW_COUNT; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            pwork->x = xs[j];
            pwork->y = ys[0] + i * 8;
            pwork++;
        }
    }
    ys[0] = y1;
    for (i = 0; i < ITF_PANEL_ROW_COUNT; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            pwork->x = xs[j];
            pwork->y = ys[0] - i * 8;
            pwork++;
        }
    }
}
