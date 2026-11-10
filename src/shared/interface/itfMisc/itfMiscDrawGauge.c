#include "shared/interface/itfMisc/decls.h"

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 portion, s32 entire, s32 shadow,
                   sdfIVector2_t *pos, u32 z, itfColor_t *pcolor_portion,
                   itfColor_t *pcolor_entire, sdfDrawTag_t *pdrawtag) {
    sdfIVector2_t vertices[6];
    itfColorVector_t highlightColors[4];
    s32 left = pos[0].x;
    s32 right = pos[1].x;
    s32 width = (right - left) >> 4;
    s32 filledWidth;

    if (entire > 0) {
        filledWidth = (((portion << 16) / entire) * width) >> 12;
    } else {
        filledWidth = 0;
    }
    vertices[0].x = left;
    vertices[0].y = pos[0].y;
    vertices[1].x = filledWidth + left;
    vertices[1].y = pos[0].y;
    vertices[2].x = right;
    vertices[2].y = pos[0].y;
    vertices[3].x = left;
    vertices[3].y = pos[1].y;
    vertices[4].x = filledWidth + left;
    vertices[4].y = pos[1].y;
    vertices[5].x = right;
    vertices[5].y = pos[1].y;
    itfMiscDrawRectanglePrim(vertices, pcolor_portion, itfGaugeQuadVertexIndex, itfGaugeQuadColorIndex, z, pdrawtag);
    itfMiscDrawRectanglePrim(vertices, pcolor_entire, itfGaugeQuadVertexIndex + 4, itfGaugeQuadColorIndex, z, pdrawtag);
    if (shadow != 0 && filledWidth >= 3) {
        itfMiscSetRatioRGB(highlightColors, pcolor_portion->components[0],
                        pcolor_portion->components[1], pcolor_portion->components[2],
                        pcolor_portion->components[3], itfGaugeHighlightVectors, 4);
        itfMiscDrawLineStripPrim(vertices, (itfColor_t *)highlightColors,
                        itfGaugeHighlightVertexIndex, itfGaugeHighlightColorIndex, 3, z, pdrawtag);
        itfMiscDrawLineStripPrim(vertices, (itfColor_t *)highlightColors,
                        itfGaugeHighlightVertexIndex + 3, itfGaugeHighlightColorIndex + 3, 3, z, pdrawtag);
    }
}
