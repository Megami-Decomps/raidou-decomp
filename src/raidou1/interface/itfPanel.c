#include "common.h"

/* Declarations from smt3-decomp src/smt3/interface/itfPanel.c */
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

extern u8 itfPanelMesWinLineVertexIndex[8];

extern u8 itfPanelSelWinLineVertexIndex[8];

extern u8 itfPanelPlainRectVertexIndex[8];

extern u8 itfPanelPlainRectColorIndex[8];

extern u8 itfPanelPera1RectVertexIndex[8];

extern u8 itfPanelPera1RectColorIndex[8];

extern u8 itfPanelLeftFadeStripVertexIndex[8];

extern u8 itfPanelLeftFadeStripColorIndex[8];
/* End of declarations from src/smt3/interface/itfPanel.c */

/* Declarations from dds-decomp src/dds1/game/code_00199AD8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_itfDrawColor_t 1

#define SMT3_DEFINES_itfDrawVertex_t 1

#define SMT3_DEFINES_itfPanelFacilityData_t 1

#define SMT3_DEFINES_itfPanelLeftFadeData_t 1

#define SMT3_DEFINES_itfPanelMesWinData_t 1

#define SMT3_DEFINES_itfPanelNormalData_t 1

#define SMT3_DEFINES_itfPanelPlainData_t 1

#define SMT3_DEFINES_itfPanelSelWinData_t 1

#define SMT3_DEFINES_itfPanelTextureData_t 1

#include "dds/itf_mes_panel_mode.h"

#include "dds/scr_command_timer_api.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_integer_return_value_api.h"

void itfMesMngChangeWindowType(ItfMesWindowIndex window, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);

/* During a command callback, return the number of earlier trace passes that
 * yielded while it was pending. The first invocation sees zero; each yielded
 * pass adds one, and a continuing or finishing handler resets the count. This
 * is callback wait state, not wall-clock time, and is valid only in the callback. */
ScrCommandYieldCount scrGetCommandTimer(void);

/* Read a command parameter as a signed VM integer, converting floats and
 * dereferencing global integer or float references. */
ScrVmInteger scrGetIntPara(ScrCommandParameterIndex parameterIndex);

/* Declared void for the callers that ignore its result: ee-gcc 2.96
 * allocates registers around the call as retail did only then. The
 * definition (scrTraceCode.c) returns the context it set the value in, for
 * the script commands that pass it on as their result; their units declare
 * it so. */
void scrSetIntReturnValue(ScrVmInteger retVal);

#include "dds/mnu.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_command_parameter_index.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/scr_command_timer_api.h"

#include "dds/sdf_packet_list.h"

#include "dds/itf.h"

#include "dds/itf_mes_window.h"

#include "dds/itf_mes_panel_mode.h"

#include "dds/itf_mes_entry_start_api.h"

#include "dds/sdf.h"

#include "dds/itf_panel_draw.h"

#include "dds/itf_text_vector.h"

#include "dds/itf_panel_api.h"

#include "dds/itf_panel_layouts.h"

#include "dds/itf_held_panel_message_step.h"

#define ITF_PANEL_MESSAGE_FADE_TICKS 30

#define ITF_PANEL_MESSAGE_EXIT_TICKS 10

#define ITF_PANEL_TINT_ALPHA_NUMERATOR 0x67

#define ITF_PANEL_TINT_ALPHA_DENOMINATOR 128

#define ITF_PANEL_TINT_BORDER_WIDTH 0x240

extern PanelEntry pinst[];

extern ItfMesWindowIndex scrGetMessageHandle(void);

extern void itfMesBuildOptionList(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);

extern void itfMesMngRequestSelectRelease(s32 window);

extern void itfMesMngRequestMessageRelease(s32 window, s32 releasePrimaryBlock);

extern TextVector itfPanelColorTemplates[];

extern TextVector D_00357B50[];

extern u8 D_003BB1C8[8];

extern u8 D_003BB1D0[8];

#include "dds/sdf_packet_alloc.h"

extern void itfDrawThreePanelBorderBands(PanelRect *rect, u32 *colors, u32 tail, s32 width, sdfDrawTag_t *command);

extern u32 D_r1_003E8AE0[];

extern u8 D_003BB1D8[8];

extern u8 D_003BB1E0[8];

extern TextVector itfPanelGradientColorTemplate[];

extern TextVector itfPanelGradientColorPair[];

extern u8 D_00357BF0[], D_00357C08[], D_00357C20[];

extern u8 D_00357C38[], D_00357C50[], D_00357C68[];

extern u8 D_003BB1A8[8], D_003BB1B0[8];

extern u8 D_00357C78[];

extern u8 D_00357C88[];

extern u8 D_003BB1B8[8];

extern u8 D_r1_003BB1C0[8];

extern PanelHold itfHeldPanelCursor;

extern s32 D_00357AE0[4];

extern u8 D_00357BD0[];

extern u8 D_00357BE0[];

/*BEGIN itfDrawFramedPanelWithTextureStrips*/
extern u32 D_00357C98[];

extern DrawColorRec D_00357D08;

extern DrawColorRec D_00357D18;

/* Draw the framed panel: two bordered bands, then six texture-bound strips whose
 * rectangles are rebuilt from the live panel bounds. */
void itfDrawFramedPanelWithTextureStrips(dds_itfpanel_t *panel, sdfDrawTag_t *command);

extern DrawColorRec D_00357D28;

extern DrawColorRec D_00357D38;

extern u8 D_00357D48[];

extern u8 D_00357D58[];

extern DrawColorRec D_00357A78;

extern DrawColorRec D_00357A88;

extern DrawColorRec D_00357A98;

extern DrawColorRec D_00357AA8;

/* Copy bounds and draw the tinted border; alpha uses the native signed 103/128 scale. */
void itfDrawTintedPanelRect(dds_itfpanel_t *panel, sdfDrawTag_t *command);

/* Poll/start an entry: one ends the command, zero keeps waiting.
   A negative signed status is consumed by clearing it; positive status keeps waiting. */
s32 itfMesMngMessage(void);

/* Forward this record's layout sprite address and the supplied value; index is unchecked. */
void itfMesMngSetOffsetY1(s32 windowIndex, s32 topDelta);

/* Return the signed byte entry status; this is separate from the option-block s16 status. */
s8 itfMesMngGetPhase(s32 windowIndex);

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 windowIndex, s8 status);

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */
s32 itfPanelAcquireHold(void);

/* Clean up the active held window and reset its queue; an inactive hold is a no-op. */
s32 itfPanelReleaseHold(void);

/* Queue script words in native second/first/third field order; first and third narrow to s16.
   Inactive/full queues are ignored with status one; negative queuedCount is not checked. */
s32 itfCommandQueueHeldPanelEntry(void);

ItfHeldPanelMessageStepResult itfStepHeldPanelMessages(void);

s32 itfMesMngSelect(void);

extern void dds3SetFilter2Para(u32);

extern void dds3SetFilter2RGBA(u32);

extern void dds3Filter2On(s32);

extern void dds3Filter2Off(s32);

extern s32 D_003BD820;

/* Configure the fade on timer zero, wait thirty ticks, then consume a negative option status.
   Keep both timer reads: setup tests a second read, while later comparisons use the cached one. */
s32 MesMngSelect(void);

/* Return the option-block signed status word, not the separate entry status byte;
 * its callers read the sign-extended halfword as a word. */
s32 itfMesMngGetSelPhase(s32 windowIndex);

/* Store the option-block signed status word without validating the slot. */
void itfMesMngSetSelPhase(s32 windowIndex, s16 status);

/* Return the same signed word forwarded as the script result when an option completes;
 * its callers read the sign-extended halfword as a word. */
s32 itfMesMngGetSelectNo(s32 windowIndex);
/* End of declarations from src/dds1/game/code_00199AD8.c */

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
    void (*handler)() = CreateVertexTable[handle->kind];

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

INCLUDE_ASM(const s32, "interface/itfPanel", func_r1_00185388);

INCLUDE_ASM(const s32, "interface/itfPanel", func_r1_00185408);

INCLUDE_ASM(const s32, "interface/itfPanel", func_r1_00185430);

INCLUDE_ASM(const s32, "interface/itfPanel", func_r1_00185458);

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
    itfMiscSetRatioRGB((s32 *)pwork->colors[2].components, red, green, blue, alpha, itfPanelMesWinColorRatio, 6);
}

/* Write the two color rows, then scale four template rows; the first fourth component is one. */
void CreateColorSelWin(itfPanelSelWinData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, 1);
    colorRow = pwork->colors[1].components;
    SetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((s32 *)pwork->colors[2].components, red, green, blue, alpha, itfPanelSelWinColorRatio, 4);
}

/* Set the base color, one alpha-scaled gradient row, and two rows at full color scale. */
void CreateColorPlain(itfPanelPlainData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((s32 *)pwork->colors[1].components, red, green, blue, alpha, itfPanelPlainColorRatio, 1);
    itfMiscSetRatioRGB((s32 *)pwork->colors[2].components, red, green, blue, ITF_PANEL_FULL_COLOR_SCALE, itfPanelPlainFullColorRatio, 2);
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
    itfDrawColor_t *colors = data->colors;
    s32 quadIndex;

    colors[1].rgba.alpha = handle->alpha;
    for (quadIndex = 0; quadIndex < ITF_PANEL_WIDE_PASS_COUNT; quadIndex++) {
        itfMiscDrawRectanglePrim(drawBuffer, colors, &itfPanelNormalVertexIndex[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], &itfPanelNormalColorIndex[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], handle->z, pdrawtag);
    }
}

/* Update seven alpha words, then emit three wide and six side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawMesWin(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    itfPanelMesWinData_t *data = (itfPanelMesWinData_t *)handle->pwork;
    sdfIVector2_t *drawBuffer = data->vertices;
    itfDrawColor_t *colors = data->colors + 1;
    s32 alpha = handle->alpha;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_COLOR_COUNT; rowIndex++, colors++) {
        colors->rgba.alpha = alpha;
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
    itfDrawColor_t *colors = data->colors + 1;
    s32 alpha = handle->alpha;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_COLOR_COUNT; rowIndex++, colors++) {
        colors->rgba.alpha = alpha;
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

/* Update the first color row's alpha and emit one indexed quad. */
void DrawPera1(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    itfPanelPlainData_t *data = (itfPanelPlainData_t *)handle->pwork;
    sdfIVector2_t *drawBuffer = data->vertices;
    itfDrawColor_t *colors = data->colors;

    colors[0].rgba.alpha = handle->alpha;
    itfMiscDrawRectanglePrim(drawBuffer, colors, itfPanelPera1RectVertexIndex, itfPanelPera1RectColorIndex, handle->z, pdrawtag);
}

/* Bracket the six-index panel draw with table-state two and its native reset to zero. */
void DrawLeftFade(itfpanel_t *handle, sdfDrawTag_t *pdrawtag) {
    itfPanelLeftFadeData_t *data = (itfPanelLeftFadeData_t *)handle->pwork;
    sdfIVector2_t *drawBuffer = data->vertices;
    itfDrawColor_t *colors = data->colors;

    colors[1].rgba.alpha = handle->alpha;
    itfMiscDrawAlphaPrim(pdrawtag, 2, 0);
    itfMiscDrawTriangleStripPrim(drawBuffer, colors, itfPanelLeftFadeStripVertexIndex, itfPanelLeftFadeStripColorIndex, 6, handle->z, pdrawtag);
    itfMiscDrawAlphaPrim(pdrawtag, 0, 0);
}

INCLUDE_ASM(const s32, "interface/itfPanel", func_r1_00185F88);

INCLUDE_ASM(const s32, "interface/itfPanel", func_r1_00186540);

/* Copy bounds and draw the tinted border; alpha uses the native signed 103/128 scale. */
void itfDrawTintedPanelRect(dds_itfpanel_t *panel, sdfDrawTag_t *command) {
    PanelRect rect;

    D_r1_003E8AE0[3] = panel->alpha * ITF_PANEL_TINT_ALPHA_NUMERATOR / ITF_PANEL_TINT_ALPHA_DENOMINATOR;
    rect.x0 = panel->left;
    rect.y0 = panel->top;
    rect.x1 = panel->right;
    rect.y1 = panel->bottom;
    itfDrawThreePanelBorderBands(&rect, D_r1_003E8AE0, panel->renderValue, ITF_PANEL_TINT_BORDER_WIDTH, command);
}

INCLUDE_ASM(const s32, "interface/itfPanel", DrawTestAlphaPrim);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_r1_0046C208);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_r1_0046C210);

u8 itfPanelPlainRectVertexIndex[8] = {0, 1, 2, 3, 0, 0, 0, 0};

u8 itfPanelPlainRectColorIndex[8] = {0, 0, 1, 1, 0, 0, 0, 0};

INCLUDE_RODATA(const s32, "interface/itfPanel", D_r1_0044A4D8);

INCLUDE_RODATA(const s32, "interface/itfPanel", D_r1_0044A528);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_r1_0046C228);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_r1_0046C230);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_r1_0046C238);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_r1_0046C240);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_r1_0046C248);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_r1_0046C250);

