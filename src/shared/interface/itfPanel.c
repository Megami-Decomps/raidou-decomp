#include "common.h"
#include "interface.h"
#include "sdfMemory.h"
#include "itfMesManager.h"
#include "itfMisc.h"
#include "itfPanel.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdf2D.h"

typedef struct itfPanelFrameData_t {
    sdfTexHandle_t *texture;
} itfPanelFrameData_t;

typedef struct itfPanelFrameInset_t {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} itfPanelFrameInset_t;
/* 0x3E8A40: nine uv rectangles; 0x3E8AD0: the four edge sizes. */
extern s32 itfCallMiscDrawRectanglePrimData[9][4];
extern itfPanelFrameInset_t itfCallMiscDrawRectanglePrimData2;

typedef struct itfPanelFrameColors_t {
    itfColor_t colors[5];
} itfPanelFrameColors_t;

typedef struct itfPanelQuadIndex_t {
    u8 index[4];
} itfPanelQuadIndex_t;

extern itfPanelFrameColors_t itfCallMiscDrawRectanglePrimTable;
extern itfPanelQuadIndex_t itfCallMiscDrawRectanglePrimData3;
extern itfPanelQuadIndex_t itfCallMiscDrawRectanglePrimData4;

/* UV rectangles of a 3x2 sprite panel: top row left, middle, right, then the bottom row. */
typedef struct itfPanelSpriteUv_t {
    s32 uv[6][4];
} itfPanelSpriteUv_t;

extern itfPanelSpriteUv_t itfCallMiscDrawSpritePrimTable;

typedef sdfIVector2_t itfDrawVertex_t;

typedef struct
{
    sdfIVector2_t vertices[8];
    itfColor_t colors[2];
} itfPanelNormalData_t;

typedef struct
{
    sdfIVector2_t vertices[40];
    itfColor_t colors[8];
} itfPanelMesWinData_t;

typedef struct
{
    sdfIVector2_t vertices[32];
    itfColor_t colors[6];
} itfPanelSelWinData_t;

typedef struct
{
    sdfIVector2_t vertices[4];
    itfColor_t colors[4];
} itfPanelPlainData_t;

typedef struct
{
    sdfIVector2_t vertices[6];
    itfColor_t colors[2];
} itfPanelLeftFadeData_t;

/* Kind 8 owns a distinct 0x64-byte buffer, independently selected by the
 * native allocation, layout-initializer and color-initializer tables. */
typedef struct itfPanelFacilityData_t {
    sdfTexHandle_t *texture;
    itfDrawVertex_t vertices[8];
    itfColor_t colors[2];
} itfPanelFacilityData_t;

/* Kind 7's allocation is exactly this texture pointer; its factory transports
 * the pointer through the existing u32 value argument. */
typedef struct itfPanelTextureData_t {
    sdfTexHandle_t *texture;
} itfPanelTextureData_t;


#define ITF_PANEL_PACKET_LIST_BYTES 0x20

void CreateVertexNormal(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

#define ITF_PANEL_COLUMN_COUNT 4

#define ITF_PANEL_COLUMN_INSET 0x200

void CreateVertexMesWin(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);
extern s32 itfPanelMesWinRowOffset[4];
void CreateVertexSelWin(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

#define ITF_PANEL_ROW_COUNT 2

void CreateVertexPlain(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);
void CreateVertexLeftFade(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

#define ITF_PANEL_CORNER_COLUMN_COUNT 3

#define ITF_PANEL_CORNER_STRIP_WIDTH 0x5A0

extern itfColor_t itfPanelMesWinColorRatio[];
extern itfColor_t itfPanelSelWinColorRatio[];
extern itfColor_t itfPanelPlainColorRatio[];

#define ITF_PANEL_FULL_COLOR_SCALE 0x80

extern itfColor_t itfPanelPlainFullColorRatio[];
void DrawNormal(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

#define ITF_PANEL_WIDE_PASS_COUNT 3

extern u8 itfPanelNormalVertexIndex[];

#define ITF_PANEL_QUAD_INDEX_COUNT 4

extern u8 itfPanelNormalColorIndex[];
void DrawMesWin(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

#define ITF_PANEL_SEVEN_COLOR_COUNT 7

#define ITF_PANEL_WIDE_INDEX_COUNT 8

#define ITF_PANEL_SEVEN_SIDE_PASS_COUNT 6

extern u8 itfPanelMesWinStripVertexIndex[], itfPanelMesWinStripColorIndex[], itfPanelMesWinLineColorIndex[];
void DrawSelWin(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

#define ITF_PANEL_FIVE_COLOR_COUNT 5

#define ITF_PANEL_FIVE_SIDE_PASS_COUNT 2

extern u8 itfPanelMesWinLineVertexIndex[8], itfPanelSelWinLineVertexIndex[8];
extern u8 itfPanelSelWinStripVertexIndex[], itfPanelSelWinStripColorIndex[], itfPanelSelWinLineColorIndex[];
void DrawPlain(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);
extern u8 itfPanelPlainRectVertexIndex[8];
extern u8 itfPanelPlainRectColorIndex[8];
extern u8 itfPanelPlainLineVertexIndex[];
extern u8 itfPanelPlainLineColorIndex[];
void DrawPera1(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);
extern u8 itfPanelPera1RectVertexIndex[8];
extern u8 itfPanelPera1RectColorIndex[8];
void DrawLeftFade(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);
extern u8 itfPanelLeftFadeStripVertexIndex[8];
extern u8 itfPanelLeftFadeStripColorIndex[8];

#define ITF_PANEL_STATE_PACKET_BYTES 0x40

extern void (*CreateVertexTable[])(u32 *work);
extern void (*CreateColorTable[])(u32 *work);

static inline void SetVec4(u32 *vec, u32 red, u32 green, u32 blue, u32 alpha) {
    vec[0] = red;
    vec[1] = green;
    vec[2] = blue;
    vec[3] = alpha;
}

extern s32 itfPanelPayloadSize[];
void CreateColorNormal(itfPanelNormalData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);
void CreateColorMesWin(itfPanelMesWinData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);
void CreateColorSelWin(itfPanelSelWinData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);
void CreateColorPlain(itfPanelPlainData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);
void CreateColorPera1(itfPanelPlainData_t *pwork, s32 red, s32 green, s32 blue, s32 alpha);
void CreateColorLeftFade(itfPanelLeftFadeData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);
extern u8 itfPanelMesWinLineVertexIndex[8];
extern u8 itfPanelSelWinLineVertexIndex[8];

#include "include_asm.h"
#include "basic.h"
#include "evt_world.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "font.h"

#define ITF_PANEL_MESSAGE_FADE_TICKS 30

#define ITF_PANEL_MESSAGE_EXIT_TICKS 10

#define ITF_PANEL_TINT_ALPHA_NUMERATOR 0x67

#define ITF_PANEL_TINT_ALPHA_DENOMINATOR 128

#define ITF_PANEL_TINT_BORDER_WIDTH 0x240

extern itfColor_t itfDrawTintedPanelRectData[];
extern u8 D_00357BF0[], D_00357C08[], D_00357C20[];
extern u8 D_00357C38[], D_00357C50[], D_00357C68[];
extern u8 D_003BB1A8[8], D_003BB1B0[8];
void itfDrawTintedPanelRect(itfpanel_t *panel, sdfDrawTag_t *command);

/* Poll/start an entry: one ends the command, zero keeps waiting.
   A negative signed status is consumed by clearing it; positive status keeps waiting. */

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */

/* Queue script words in native second/first/third field order; first and third narrow to s16.
   Inactive/full queues are ignored with status one; negative queuedCount is not checked. */

/* Configure the fade on timer zero, wait thirty ticks, then consume a negative option status.
   Keep both timer reads: setup tests a second read, while later comparisons use the cached one. */

/* Allocate the common record and the kind's payload; kinds 6 to 8 take the
 * caller's value as the first payload word. */
itfpanel_t *itfPnlRegist2(s32 type, saddr_t texhandle) {
    sdfMemHandle_t *allocation;
    itfpanel_t *panel;
    u32 *payload;

    allocation = sdfMemAllocDirect(sizeof(itfpanel_t));
    panel = (itfpanel_t *)sdfMemLock(allocation);
    panel->kind = type;
    panel->allocation = allocation;
    allocation = sdfMemAllocDirect(itfPanelPayloadSize[type]);
    panel->payloadAllocation = allocation;
    payload = (u32 *)sdfMemLock(allocation);
    panel->pwork = payload;
    if (texhandle != 0) {
        switch (type) {
        case 6:
            payload[0] = texhandle;
            break;
        case 7:
            payload[0] = texhandle;
            break;
        case 8:
            payload[0] = texhandle;
            break;
        }
    }
    return panel;
}

/* A sprite with a retained payload owns a secondary allocation. Release that
 * before its primary allocation; NULL is a no-op. */
void itfPnlRelease(itfpanel_t *handle) {
    if (handle != NULL) {
        if (handle->pwork != NULL) {
            sdfMemFreeDirect(handle->payloadAllocation);
        }
        sdfMemFreeDirect(handle->allocation);
    }
}

/* Store rectangle corners and the caller's render value before notifying the
 * kind-specific handler with context only. No kind-range or context checks. */
void itfPnlSetPosition(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z) {
    handle->z = z;
    handle->left = x0;
    handle->top = y0;
    handle->right = x1;
    handle->bottom = y1;
    if (CreateVertexTable[handle->kind] != 0) {
        CreateVertexTable[handle->kind](handle->pwork);
    }
}

/* Add independent corner/render-value deltas, then notify with the updated
 * corners as well as context. */
void itfPnlSetPositionOffset(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z) {
    void (*handler)(u32 *, s32, s32, s32, s32) = (void (*)(u32 *, s32, s32, s32, s32))CreateVertexTable[handle->kind];

    handle->left += x0;
    handle->top += y0;
    handle->right += x1;
    handle->bottom += y1;
    handle->z += z;
    if (handler != 0) {
        handler(handle->pwork, handle->left, handle->top, handle->right, handle->bottom);
    }
}

/* Store the RGBA words in packet order, then notify with context.
 * The panel renderers consume these channels directly. */
void itfPnlSetColor(itfpanel_t *handle, s32 r, s32 g, s32 b, s32 a) {
    handle->red = r;
    handle->green = g;
    handle->blue = b;
    handle->alpha = a;
    if (CreateColorTable[handle->kind] != 0) {
        CreateColorTable[handle->kind](handle->pwork);
    }
}

void itfPnlDraw(itfpanel_t *handle, itfMesShake_t *offset, sdfGrOt_t *pot)
{
    sdfDrawTag_t *packetList = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(ITF_PANEL_PACKET_LIST_BYTES));

    sdfDrawTagInit(packetList);
    DrawTestAlphaPrim(packetList);
    itfPnlDrawSub(handle, offset, packetList);
    pot->regFunc(pot, packetList);
}

void itfLeaf6(s32 *src, s32 *dst)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

void itfLeaf7(s32 *dst, s32 *src)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

void itfPnlDrawSub(itfpanel_t *handle, itfMesShake_t *offset, sdfDrawTag_t *pdrawtag)
{
    s32 rect[4];

    itfLeaf6(&handle->left, rect);
    handle->left += offset->dist[0];
    handle->top += offset->dist[1];
    handle->right += offset->dist[0];
    handle->bottom += offset->dist[1];
    DrawTable_itfPanel[handle->kind](handle, pdrawtag);
    itfLeaf7(&handle->left, rect);
}

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

/* Emit the frame grid and three table-offset rows above and below it. */
void CreateVertexMesWin(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1) {
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
    for (i = 0; i < 3; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            pwork->x = xs[j];
            pwork->y = ys[0] + itfPanelMesWinRowOffset[i];
            pwork++;
        }
    }
    ys[0] = y1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            pwork->x = xs[j];
            pwork->y = ys[0] - itfPanelMesWinRowOffset[i];
            pwork++;
        }
    }
}

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

/* Populate RGBA rows at bytes 0x40/0x50; the first row is transparent. */
void CreateColorNormal(itfPanelNormalData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, 0);
    colorRow = pwork->colors[1].components;
    SetVec4(colorRow, red, green, blue, alpha);
}

/* Write the two color rows, then scale six template rows; the first fourth component is one. */
void CreateColorMesWin(itfPanelMesWinData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, 1);
    colorRow = pwork->colors[1].components;
    SetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB(&pwork->colors[2], red, green, blue, alpha, itfPanelMesWinColorRatio, 6);
}

/* Write the two color rows, then scale four template rows; the first fourth component is one. */
void CreateColorSelWin(itfPanelSelWinData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, 1);
    colorRow = pwork->colors[1].components;
    SetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB(&pwork->colors[2], red, green, blue, alpha, itfPanelSelWinColorRatio, 4);
}

/* Set the base color, one alpha-scaled gradient row, and two rows at full color scale. */
void CreateColorPlain(itfPanelPlainData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB(&pwork->colors[1], red, green, blue, alpha, itfPanelPlainColorRatio, 1);
    itfMiscSetRatioRGB(&pwork->colors[2], red, green, blue, ITF_PANEL_FULL_COLOR_SCALE, itfPanelPlainFullColorRatio, 2);
}

/* Set the four color words at byte 0x20, consumed as a color record by the quad path. */
void CreateColorPera1(itfPanelPlainData_t *pwork, s32 red, s32 green, s32 blue, s32 alpha) {
    u32 *color = pwork->colors[0].components;

    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}

/* Populate RGBA rows at bytes 0x30/0x40; the first row is transparent. */
void CreateColorLeftFade(itfPanelLeftFadeData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, 0);
    colorRow = pwork->colors[1].components;
    SetVec4(colorRow, red, green, blue, alpha);
}

/* Draw the panel's three flat quads from one vertex/color buffer; each quad uses its own 4-byte index rows. */
void DrawNormal(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    itfPanelNormalData_t *data = (itfPanelNormalData_t *)handle->pwork;
    sdfIVector2_t *drawBuffer = data->vertices;
    itfColor_t *colors = data->colors;
    s32 quadIndex;

    colors[1].components[3] = handle->alpha;
    for (quadIndex = 0; quadIndex < ITF_PANEL_WIDE_PASS_COUNT; quadIndex++) {
        itfMiscDrawRectanglePrim(drawBuffer, colors, &itfPanelNormalVertexIndex[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], &itfPanelNormalColorIndex[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], handle->z, pdrawtag);
    }
}

/* Update seven alpha words, then emit three wide and six side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawMesWin(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    itfPanelMesWinData_t *data = (itfPanelMesWinData_t *)handle->pwork;
    sdfIVector2_t *drawBuffer = data->vertices;
    itfColor_t *colors = data->colors + 1;
    s32 alpha = handle->alpha;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_COLOR_COUNT; rowIndex++, colors++) {
        colors->components[3] = alpha;
    }
    colors = data->colors;
    for (rowIndex = 0; rowIndex < ITF_PANEL_WIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawTriangleStripPrim(drawBuffer, colors, &itfPanelMesWinStripVertexIndex[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], &itfPanelMesWinStripColorIndex[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], ITF_PANEL_WIDE_INDEX_COUNT, handle->z, pdrawtag);
    }
    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_SIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawLineStripPrim(drawBuffer + 16 + rowIndex * 4, colors, itfPanelMesWinLineVertexIndex, &itfPanelMesWinLineColorIndex[rowIndex * ITF_PANEL_QUAD_INDEX_COUNT], ITF_PANEL_QUAD_INDEX_COUNT, handle->z, pdrawtag);
    }
}

/* Update five alpha words, then emit three wide and two side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawSelWin(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    itfPanelSelWinData_t *data = (itfPanelSelWinData_t *)handle->pwork;
    sdfIVector2_t *drawBuffer = data->vertices;
    itfColor_t *colors = data->colors + 1;
    s32 alpha = handle->alpha;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_COLOR_COUNT; rowIndex++, colors++) {
        colors->components[3] = alpha;
    }
    colors = data->colors;
    for (rowIndex = 0; rowIndex < ITF_PANEL_WIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawTriangleStripPrim(drawBuffer, colors, &itfPanelSelWinStripVertexIndex[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], &itfPanelSelWinStripColorIndex[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], ITF_PANEL_WIDE_INDEX_COUNT, handle->z, pdrawtag);
    }
    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_SIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawLineStripPrim(drawBuffer + 16 + rowIndex * 4, colors, itfPanelSelWinLineVertexIndex, &itfPanelSelWinLineColorIndex[rowIndex * ITF_PANEL_QUAD_INDEX_COUNT], ITF_PANEL_QUAD_INDEX_COUNT, handle->z, pdrawtag);
    }
}

void DrawPlain(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    sdfIVector2_t vertices[12];
    itfPanelPlainData_t *data = (itfPanelPlainData_t *)handle->pwork;
    sdfIVector2_t *buf = data->vertices;
    itfColor_t *colors = data->colors;
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

/* Update the first color row's alpha and emit one indexed quad. */
void DrawPera1(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    itfPanelPlainData_t *data = (itfPanelPlainData_t *)handle->pwork;
    sdfIVector2_t *drawBuffer = data->vertices;
    itfColor_t *colors = data->colors;

    colors[0].components[3] = handle->alpha;
    itfMiscDrawRectanglePrim(drawBuffer, colors, itfPanelPera1RectVertexIndex, itfPanelPera1RectColorIndex, handle->z, pdrawtag);
}

/* Bracket the six-index panel draw with table-state two and its native reset to zero. */
void DrawLeftFade(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    itfPanelLeftFadeData_t *data = (itfPanelLeftFadeData_t *)handle->pwork;
    sdfIVector2_t *drawBuffer = data->vertices;
    itfColor_t *colors = data->colors;

    colors[1].components[3] = handle->alpha;
    itfMiscDrawAlphaPrim(pdrawtag, 2, 0);
    itfMiscDrawTriangleStripPrim(drawBuffer, colors, itfPanelLeftFadeStripVertexIndex, itfPanelLeftFadeStripColorIndex, 6, handle->z, pdrawtag);
    itfMiscDrawAlphaPrim(pdrawtag, 0, 0);
}

/* NON_MATCHING: the register use differs from retail. */
#ifdef NON_MATCHING
/* Draw a nine-patch frame: a filled background rectangle, then four corners, four edges and the centre. */
void itfCallMiscDrawRectanglePrim(itfpanel_t *panel, sdfDrawTag_t *tag) {
    itfPanelFrameColors_t frame = itfCallMiscDrawRectanglePrimTable;
    itfPanelQuadIndex_t vertexIndex = itfCallMiscDrawRectanglePrimData3;
    itfPanelQuadIndex_t colorIndex = itfCallMiscDrawRectanglePrimData4;
    itfColor_t *colors = frame.colors;
    sdfIVector2_t vertices[4];
    s32 rect[4];
    itfPanelFrameData_t *data = (itfPanelFrameData_t *)panel->pwork;
    s32 left, top, right, bottom;
    s32 width, height;
    s32 frameAlpha;
    s32 i;

    for (i = 1; i < 5; i++) {
        frameAlpha = colors[i].components[3];
        colors[i].components[3] = frameAlpha * panel->alpha / 128;
    }
    left = panel->left;
    top = panel->top;
    right = panel->right;
    bottom = panel->bottom;
    vertices[0].x = left + 128;
    vertices[0].y = top + 56;
    vertices[1].x = left + 7008;
    vertices[1].y = top + 56;
    vertices[2].x = left + 7008;
    vertices[2].y = bottom - 80;
    vertices[3].x = left + 128;
    vertices[3].y = bottom - 80;
    itfMiscDrawRectanglePrim(vertices, colors, vertexIndex.index, colorIndex.index, panel->z, tag);

    width = right - left - itfCallMiscDrawRectanglePrimData2.left - itfCallMiscDrawRectanglePrimData2.right;
    height = bottom - top - itfCallMiscDrawRectanglePrimData2.top - itfCallMiscDrawRectanglePrimData2.bottom;
    rect[0] = left;
    rect[1] = top;
    rect[2] = left + itfCallMiscDrawRectanglePrimData2.left;
    rect[3] = top + itfCallMiscDrawRectanglePrimData2.top;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[0], colors, panel->z, data->texture, 0, tag);
    rect[0] = right - itfCallMiscDrawRectanglePrimData2.right;
    rect[2] = right;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[1], colors, panel->z, data->texture, 0, tag);
    rect[0] = left;
    rect[1] = bottom - itfCallMiscDrawRectanglePrimData2.bottom;
    rect[2] = left + itfCallMiscDrawRectanglePrimData2.left;
    rect[3] = bottom;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[2], colors, panel->z, data->texture, 0, tag);
    rect[0] = right - itfCallMiscDrawRectanglePrimData2.right;
    rect[2] = right;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[3], colors, panel->z, data->texture, 0, tag);
    rect[0] = left + itfCallMiscDrawRectanglePrimData2.left;
    rect[1] = top;
    rect[2] = rect[0] + width;
    rect[3] = top + itfCallMiscDrawRectanglePrimData2.top;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[4], colors, panel->z, data->texture, 0, tag);
    rect[0] = left;
    rect[1] = top + itfCallMiscDrawRectanglePrimData2.top;
    rect[2] = left + itfCallMiscDrawRectanglePrimData2.left;
    rect[3] = rect[1] + height;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[5], colors, panel->z, data->texture, 0, tag);
    rect[0] = right - itfCallMiscDrawRectanglePrimData2.right;
    rect[2] = right;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[6], colors, panel->z, data->texture, 0, tag);
    rect[0] = left + itfCallMiscDrawRectanglePrimData2.left;
    rect[1] = bottom - itfCallMiscDrawRectanglePrimData2.bottom;
    rect[2] = rect[0] + width;
    rect[3] = bottom;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[7], colors, panel->z, data->texture, 0, tag);
    rect[0] = left + itfCallMiscDrawRectanglePrimData2.left;
    rect[1] = top + itfCallMiscDrawRectanglePrimData2.top;
    rect[2] = rect[0] + width;
    rect[3] = rect[1] + height;
    colors[0].components[3] = panel->alpha;
    itfMiscDrawSpritePrim(rect, itfCallMiscDrawRectanglePrimData[8], colors, panel->z, data->texture, 0, tag);
}
#else
INCLUDE_ASM(const s32, "interface/itfPanel", itfCallMiscDrawRectanglePrim);
#endif

void itfCallMiscDrawSpritePrim(itfpanel_t *panel, sdfDrawTag_t *tag)
{
    s32 rect[4];
    itfPanelSpriteUv_t uv;
    itfPanelFrameData_t *data;
    s32 w;
    s32 h;

    data = (itfPanelFrameData_t *)panel->pwork;
    uv = itfCallMiscDrawSpritePrimTable;
    w = 384;
    h = 120;
    rect[0] = panel->left;
    rect[1] = panel->top;
    rect[2] = panel->left + w;
    rect[3] = panel->top + h;
    itfMiscDrawSpritePrim(rect, uv.uv[0], &panel->red, panel->z, data->texture, 0, tag);
    rect[0] = panel->left;
    rect[1] = panel->top + h;
    rect[2] = panel->left + w;
    rect[3] = panel->top + 240;
    itfMiscDrawSpritePrim(rect, uv.uv[3], &panel->red, panel->z, data->texture, 0, tag);
    rect[0] = panel->right;
    rect[1] = panel->top;
    rect[2] = panel->right + w;
    rect[3] = panel->top + h;
    itfMiscDrawSpritePrim(rect, uv.uv[2], &panel->red, panel->z, data->texture, 0, tag);
    rect[0] = panel->right;
    rect[1] = panel->top + h;
    rect[2] = panel->right + w;
    rect[3] = panel->top + 240;
    itfMiscDrawSpritePrim(rect, uv.uv[5], &panel->red, panel->z, data->texture, 0, tag);
    rect[0] = panel->left + w;
    rect[1] = panel->top;
    rect[2] = panel->right;
    rect[3] = panel->top + h;
    itfMiscDrawSpritePrim(rect, uv.uv[1], &panel->red, panel->z, data->texture, 0, tag);
    rect[0] = panel->left + w;
    rect[1] = panel->top + h;
    rect[2] = panel->right;
    rect[3] = panel->top + 240;
    itfMiscDrawSpritePrim(rect, uv.uv[4], &panel->red, panel->z, data->texture, 0, tag);
}

/* Copy bounds and draw the tinted border; alpha uses the native signed 103/128 scale. */
void itfDrawTintedPanelRect(itfpanel_t *panel, sdfDrawTag_t *command) {
    sdfIVector2_t rect[2];

    itfDrawTintedPanelRectData[0].components[3] = panel->alpha * ITF_PANEL_TINT_ALPHA_NUMERATOR / ITF_PANEL_TINT_ALPHA_DENOMINATOR;
    rect[0].x = panel->left;
    rect[0].y = panel->top;
    rect[1].x = panel->right;
    rect[1].y = panel->bottom;
    itfDrawThreePanelBorderBands(rect, itfDrawTintedPanelRectData, panel->z, ITF_PANEL_TINT_BORDER_WIDTH, command);
}

void DrawTestAlphaPrim(sdfDrawTag_t *pdrawtag) {
    s32 packet = sdfGrAllocPacketBuff(ITF_PANEL_STATE_PACKET_BYTES);
    u64 *stateWords = EE_ADDR_AS(u64 *, sdf2DMakeStdPacket(packet, ITF_PANEL_STATE_PACKET_BYTES));

    stateWords[4] = 0x4101B;
    stateWords[5] = 0x47;
    stateWords[6] = 0x44;
    stateWords[7] = 0x42;
    sdfDrawTagLink(pdrawtag, packet);
}

INCLUDE_SDATA(const s32, "interface/itfPanel", itfPanelSelWinLineVertexIndex_itfPanel);

INCLUDE_SDATA(const s32, "interface/itfPanel", itfPanelSelWinLineVertexIndex);

u8 itfPanelPlainRectVertexIndex[8] = {0, 1, 2, 3, 0, 0, 0, 0};

u8 itfPanelPlainRectColorIndex[8] = {0, 0, 1, 1, 0, 0, 0, 0};

INCLUDE_RODATA(const s32, "interface/itfPanel", itfCallMiscDrawRectanglePrimTable);

INCLUDE_RODATA(const s32, "interface/itfPanel", itfCallMiscDrawSpritePrimTable);

INCLUDE_SDATA(const s32, "interface/itfPanel", itfPanelPera1RectVertexIndex);

INCLUDE_SDATA(const s32, "interface/itfPanel", itfPanelPera1RectColorIndex);

INCLUDE_SDATA(const s32, "interface/itfPanel", itfPanelLeftFadeStripVertexIndex);

INCLUDE_SDATA(const s32, "interface/itfPanel", itfPanelLeftFadeStripColorIndex);

INCLUDE_SDATA(const s32, "interface/itfPanel", itfCallMiscDrawRectanglePrimData3);

INCLUDE_SDATA(const s32, "interface/itfPanel", itfCallMiscDrawRectanglePrimData4);

