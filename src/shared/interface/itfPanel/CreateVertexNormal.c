#include "shared/interface/itfPanel/decls.h"

/* Emit top/bottom vertices for four columns; collapse overlapping inner columns to the midpoint.
   Coordinates and inset remain in the renderer's native integer units. */
void CreateVertexNormal(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 columnX[ITF_PANEL_COLUMN_COUNT];
    s32 columnIndex;

    columnX[0] = x0;
    columnX[1] = x0 + ITF_PANEL_COLUMN_INSET;
    columnX[2] = x1 - ITF_PANEL_COLUMN_INSET;
    columnX[3] = x1;
    if (columnX[2] < columnX[1]) {
        columnX[1] = ((x1 - x0) >> 1) + x0;
        columnX[2] = columnX[1];
    }
    for (columnIndex = 0; columnIndex < ITF_PANEL_COLUMN_COUNT; columnIndex++) {
        pwork->x = columnX[columnIndex];
        pwork->y = y0;
        pwork++;
        pwork->x = columnX[columnIndex];
        pwork->y = y1;
        pwork++;
    }
}
