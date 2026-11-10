#include "shared/interface/itfPanel/decls.h"

/* Write two rows of three columns; the middle column is the right edge minus a fixed strip width. */
void CreateVertexLeftFade(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 columnX[ITF_PANEL_CORNER_COLUMN_COUNT];
    s32 rowY[ITF_PANEL_ROW_COUNT];
    s32 rowIndex;
    s32 columnIndex;

    columnX[0] = x0;
    columnX[1] = x1 - ITF_PANEL_CORNER_STRIP_WIDTH;
    columnX[2] = x1;
    rowY[0] = y0;
    rowY[1] = y1;
    for (rowIndex = 0; rowIndex < ITF_PANEL_ROW_COUNT; rowIndex++) {
        for (columnIndex = 0; columnIndex < ITF_PANEL_CORNER_COLUMN_COUNT; columnIndex++) {
            pwork->x = columnX[columnIndex];
            pwork->y = rowY[rowIndex];
            pwork++;
        }
    }
}
