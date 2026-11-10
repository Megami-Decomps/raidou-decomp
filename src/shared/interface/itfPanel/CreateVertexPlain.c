#include "shared/interface/itfPanel/decls.h"

/* Write the rectangle corners in top-left, top-right, bottom-right, bottom-left order. */
void CreateVertexPlain(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1) {
    sdfIVector2_t *p;

    pwork->x = x0;
    pwork->y = y0;
    p = pwork + 1;
    p->x = x1;
    p->y = y0;
    p = pwork + 2;
    p->x = x1;
    p->y = y1;
    p = pwork + 3;
    p->x = x0;
    p->y = y1;
}
