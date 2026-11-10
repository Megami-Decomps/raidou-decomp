/* Declarations of smt3-decomp src/smt3/interface/itfPanel.c, for its bodies in this directory. */
#ifndef SHARED_INTERFACE_ITFPANEL_H
#define SHARED_INTERFACE_ITFPANEL_H

#include "interface.h"

/* A color row is also a four-component vector for the template scaler. */
typedef union
{
    u32 components[4];
    struct
    {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    } rgba;
} itfDrawColor_t;

typedef sdfIVector2_t itfDrawVertex_t;

typedef struct
{
    sdfIVector2_t vertices[8];
    itfDrawColor_t colors[2];
} itfPanelNormalData_t;

typedef struct
{
    sdfIVector2_t vertices[40];
    itfDrawColor_t colors[8];
} itfPanelMesWinData_t;

typedef struct
{
    sdfIVector2_t vertices[32];
    itfDrawColor_t colors[6];
} itfPanelSelWinData_t;

typedef struct
{
    sdfIVector2_t vertices[4];
    itfDrawColor_t colors[4];
} itfPanelPlainData_t;

typedef struct
{
    sdfIVector2_t vertices[6];
    itfDrawColor_t colors[2];
} itfPanelLeftFadeData_t;

/* Kind 8 owns a distinct 0x64-byte buffer, independently selected by the
 * native allocation, layout-initializer and color-initializer tables. */
typedef struct itfPanelFacilityData_t {
    sdfTexHandle_t *texture;
    itfDrawVertex_t vertices[8];
    itfDrawColor_t colors[2];
} itfPanelFacilityData_t;

/* Kind 7's allocation is exactly this texture pointer; its factory transports
 * the pointer through the existing u32 value argument. */
typedef struct itfPanelTextureData_t {
    sdfTexHandle_t *texture;
} itfPanelTextureData_t;

void itfPnlRelease(itfpanel_t *handle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

void itfPnlSetPosition(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

void itfPnlSetPositionOffset(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

void itfPnlSetColor(itfpanel_t *handle, s32 r, s32 g, s32 b, s32 a);

void itfPnlDraw(itfpanel_t *handle, sdfGrOt_t *pot);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

#define ITF_PANEL_PACKET_LIST_BYTES 0x20

extern void sdfDrawTagInit(sdfDrawTag_t *a);

void DrawTestAlphaPrim(sdfDrawTag_t *pdrawtag);

void itfPnlDrawTag(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

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

void CreateVertexFacility(itfPanelFacilityData_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

extern void itfMiscSetRatioRGB(s32 *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const s32 *input, s32 count);

extern s32 itfPanelMesWinColorRatio[];

extern s32 itfPanelSelWinColorRatio[];

extern s32 itfPanelPlainColorRatio[];

#define ITF_PANEL_FULL_COLOR_SCALE 0x80

extern s32 itfPanelPlainFullColorRatio[];

void CreateColorFacility(itfPanelFacilityData_t *base);

void DrawNormal(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

#define ITF_PANEL_WIDE_PASS_COUNT 3

extern void itfMiscDrawRectanglePrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);

extern u8 itfPanelNormalVertexIndex[];

#define ITF_PANEL_QUAD_INDEX_COUNT 4

extern u8 itfPanelNormalColorIndex[];

void DrawMesWin(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

#define ITF_PANEL_SEVEN_COLOR_COUNT 7

extern void itfMiscDrawTriangleStripPrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

#define ITF_PANEL_WIDE_INDEX_COUNT 8

#define ITF_PANEL_SEVEN_SIDE_PASS_COUNT 6

extern void itfMiscDrawLineStripPrim(void *, void *, u8 *, u8 *, s32, u32, sdfDrawTag_t *);

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

void DrawSpkWin2(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

extern void itfMiscDrawSpritePrim(void *, void *, void *, s32, sdfTexHandle_t *, s32, sdfDrawTag_t *);

/*END itfDrawFramedPanelWithTextureStrips*/
extern itfDrawColor_t itfPanelSpkCapColor;

extern void itfMiscDrawSpritePrim2(itfDrawVertex_t *, itfDrawColor_t *, itfDrawColor_t *, u32, s32, sdfDrawTag_t *);

extern itfDrawColor_t itfPanelSpkCenterColor;

void DrawFacility(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

extern u8 itfPanelFacilityVertexIndex[];

extern u8 itfPanelFacilityColorIndex[];

extern itfDrawColor_t itfPanelBandTopColor;

extern itfDrawColor_t itfPanelBandStretchColor;

extern itfDrawColor_t itfPanelBandBottomColor;

extern itfDrawColor_t itfPanelBandShortColor;

#define ITF_PANEL_STATE_PACKET_BYTES 0x40

extern u32 sdf2DMakeStdPacket(u32, s32);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void (*CreateVertexTable[])();

extern void (*CreateColorTable[])();

extern void (*DrawTable[])(itfpanel_t *, sdfDrawTag_t *);

static inline void SetVec4(u32 *vec, u32 red, u32 green, u32 blue, u32 alpha) {
    vec[0] = red;
    vec[1] = green;
    vec[2] = blue;
    vec[3] = alpha;
}

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern s32 itfPanelPayloadSize[];

/* Allocate the common record and the kind's payload; kinds 6 to 8 take the
 * caller's value as the first payload word. */
itfpanel_t *itfPnlRegist2(s32 type, saddr_t texhandle);

/* A sprite with a retained payload owns a secondary allocation. Release that
 * before its primary allocation; NULL is a no-op. */
void itfPnlRelease(itfpanel_t *handle);

/* Store rectangle corners and the caller's render value before notifying the
 * kind-specific handler with context only. No kind-range or context checks. */
void itfPnlSetPosition(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

/* Add independent corner/render-value deltas, then notify with the updated
 * corners as well as context. */
void itfPnlSetPositionOffset(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

/* Store the RGBA words in packet order, then notify with context.
 * The panel renderers consume these channels directly. */
void itfPnlSetColor(itfpanel_t *handle, s32 r, s32 g, s32 b, s32 a);

/* Build the panel list and append it to the SDK draw-surface pool. */
void itfPnlDraw(itfpanel_t *handle, sdfGrOt_t *pot);

/* Invoke the indexed panel handler; panel and kind are trusted. */
void itfPnlDrawTag(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Emit top/bottom vertices for four columns; collapse overlapping inner columns to the midpoint.
   Coordinates and inset remain in the renderer's native integer units. */
void CreateVertexNormal(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

/* Emit the frame grid and three table-offset rows above and below it. */
void CreateVertexMesWin(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

void CreateVertexSelWin(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

/* Write the rectangle corners in top-left, top-right, bottom-right, bottom-left order. */
void CreateVertexPlain(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

/* Write two rows of three columns; the middle column is the right edge minus a fixed strip width. */
void CreateVertexLeftFade(sdfIVector2_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

/* Write four fixed horizontal columns with inset top/bottom coordinates.
   x0 and x1 are intentionally unused; do not replace the fixed horizontal positions. */
void CreateVertexFacility(itfPanelFacilityData_t *pwork, s32 x0, s32 y0, s32 x1, s32 y1);

/* Populate RGBA rows at bytes 0x40/0x50; the first row is transparent. */
void CreateColorNormal(itfPanelNormalData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);

/* Write the two color rows, then scale six template rows; the first fourth component is one. */
void CreateColorMesWin(itfPanelMesWinData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);

/* Write the two color rows, then scale four template rows; the first fourth component is one. */
void CreateColorSelWin(itfPanelSelWinData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);

/* Set the base color, one alpha-scaled gradient row, and two rows at full color scale. */
void CreateColorPlain(itfPanelPlainData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);

/* Set the four color words at byte 0x20, consumed as a color record by the quad path. */
void CreateColorPera1(itfPanelPlainData_t *pwork, s32 red, s32 green, s32 blue, s32 alpha);

/* Populate RGBA rows at bytes 0x30/0x40; the first row is transparent. */
void CreateColorLeftFade(itfPanelLeftFadeData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha);

/* Set both panel colors to the same blue tint with zero alpha. */
void CreateColorFacility(itfPanelFacilityData_t *base);

/* Draw the panel's three flat quads from one vertex/color buffer; each quad uses its own 4-byte index rows. */
void DrawNormal(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Update seven alpha words, then emit three wide and six side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawMesWin(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Update five alpha words, then emit three wide and two side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawSelWin(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

void DrawPlain(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Update the first color row's alpha and emit one indexed quad. */
void DrawPera1(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Bracket the six-index panel draw with table-state two and its native reset to zero. */
void DrawLeftFade(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Split a tall texture into top, stretch and bottom bands; the short path
 * submits the live rectangle in the panel. */
void DrawMesWin2(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Bind the texture for the left cap, then draw the center and reversed right
 * cap. Insets are captured before the first submission; Y bounds are live. */
void DrawSpkWin2(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Draw the three inset flat quads, then split a tall texture into top, stretch
 * and bottom bands. The short path uses the live rectangle in the panel. */
void DrawFacility(itfpanel_t *handle, sdfDrawTag_t *pdrawtag);

/* Append two GS A+D state writes: TEST_1 (0x47), then ALPHA_1 (0x42). */
void DrawTestAlphaPrim(sdfDrawTag_t *pdrawtag);

#endif
