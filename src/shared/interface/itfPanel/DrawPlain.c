#include "shared/interface/itfPanel/decls.h"

void DrawPlain(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    sdfIVector2_t vertices[12];
    itfPanelPlainData_t *data = (itfPanelPlainData_t *)handle->pwork;
    sdfIVector2_t *buf = data->vertices;
    itfDrawColor_t *colors = data->colors;
    sdfIVector2_t *input = data->vertices;
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 xOffset = i * 16;
        s32 yOffset = i * 8;
        sdfIVector2_t *out = &vertices[i * 4];

        out[0].x = input[0].x + xOffset;
        out[0].y = input[0].y + yOffset;
        out[1].x = input[1].x - xOffset;
        out[1].y = input[1].y + yOffset;
        out[2].x = input[2].x - xOffset;
        out[2].y = input[2].y - yOffset;
        out[3].x = input[3].x + xOffset;
        out[3].y = input[3].y - yOffset;
    }

    itfMiscDrawRectanglePrim(buf, colors, itfPanelPlainRectVertexIndex, itfPanelPlainRectColorIndex, handle->z, pdrawtag);
    for (i = 0; i < 3; i++) {
        itfMiscDrawLineStripPrim(vertices, colors, &itfPanelPlainLineVertexIndex[i * 5], &itfPanelPlainLineColorIndex[i * 5], 5, handle->z, pdrawtag);
    }
}
