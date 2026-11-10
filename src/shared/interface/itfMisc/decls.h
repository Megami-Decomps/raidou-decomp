/* Declarations of smt3-decomp src/smt3/interface/itfMisc.c, for its bodies in this directory. */
#ifndef SHARED_INTERFACE_ITFMISC_H
#define SHARED_INTERFACE_ITFMISC_H

#include "common.h"

#include "interface.h"

#include "libsdf.h"

#include "kernel.h"

/* Font chains and their children have different pool strides. Only the
 * coordinates, color word and links used by this unit are exposed. */
typedef struct frFontChild_t
{
    u8 unk00[0x10];
    union { u32 renderWord; } parentDimensionsOrRenderWord;
    u8 unk14[0x14];
    struct frFontChild_t *next;
} frFontChild_t;

typedef struct frFontRequest_t
{
    u32 unk00;
    s32 x;
    s32 y;
    u8 unk0C[0x10];
    frFontChild_t *firstChild;
    void *lastChild;
    struct frFontRequest_t *prev;
    u8 unk28[0x1C];
} frFontRequest_t;

typedef struct itfLink_t
{
    struct itfLink_t *previous;
    struct itfLink_t *next;
    s32 index;
    void *state;
    sdfMemHandle_t *resourceHandle;
} itfLink_t;

typedef struct
{
    itfLink_t *puse_top, *puse_tail, *pdisuse_top, *pdisuse_tail;
} itfmisclinkstack_t;

typedef struct { s32 x, y, z, w; } itfColorVector_t;

typedef struct { u8 unk00[0x20]; u64 value, registerCode; } itfGsState_t;

extern frFontRequest_t *frTestFontHandle;

extern u64 itfAlphaValueTable[];

extern u8 itfGaugeQuadVertexIndex[8], itfGaugeQuadColorIndex[8], itfGaugeHighlightVertexIndex[], itfGaugeHighlightColorIndex[];

extern itfColorVector_t itfGaugeHighlightVectors[];

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);

extern s32 sdfGrAllocPacketBuff(u32 aSize);

extern void sdf2DMakePrimPacketHeader(void *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, u64 aGifRegs, s32 aNumOfPrim);

extern saddr_t sdf2DGetPrimPtr(saddr_t aPacketPtr);

extern u32 sdf2DMakeStdPacket(u32 aPacket, s32 aLeng);

extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *, sdfTexHandle_t *, s32);

extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t aPacket);

extern void dds3AdminiExitSequence(void);

#include "sdk_libc.h"

#define ITF_BYTE_MASK 0xFF

#define ITF_VECTOR_WORD_COUNT 4

#define ITF_FIXED_FRACTION_BITS 16

#define ITF_FIXED_COMPONENT_MAX 0xff0000

#define ITF_GS_X_BIAS 0x7000

#define ITF_GS_Y_BIAS 0x7900

#define ITF_TRIANGLE_VERTEX_COUNT 3

#define ITF_QUAD_VERTEX_COUNT 4

#define ITF_STATE_PACKET_BYTES 0x30

#define ITF_TEST_FIRST_CONTEXT 0x47

#define ITF_TEST_SECOND_CONTEXT 0x48

#define ITF_ALPHA_FIRST_CONTEXT 0x42

#define ITF_ALPHA_SECOND_CONTEXT 0x43

#define ITF_TRIANGLE_PRIMITIVE_BITS 0x4B

#define ITF_QUAD_FAN_PRIMITIVE_BITS 0x4D

#define ITF_TEXTURED_FAN_PRIMITIVE_BITS 0x5D

#define ITF_TEXTURED_SPRITE_PRIMITIVE_BITS 0x156

#define ITF_COLORED_SPRITE_PRIMITIVE_BITS 0x15E

#define ITF_SPRITE_PRIMITIVE_BITS 0x46

#define ITF_TRIANGLE_STRIP_PRIMITIVE_BITS 0x4C

#define ITF_LINE_STRIP_PRIMITIVE_BITS 0x14A

#define ITF_LINES_PRIMITIVE_BITS 0x49

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfMiscLinkInitialize(itfmisclinkstack_t *pool, itfLink_t *nodes, s32 count, s32 stride);

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
itfLink_t *itfMiscLinkTake(itfmisclinkstack_t *pstack);

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfMiscLinkGive(itfLink_t *pelem, itfmisclinkstack_t *pstack);

/* Multiply XYZ, cap only the positive product at 255 << 16, then shift by 16.
 * Negative products are not clamped; W comes from the argument. */
void itfMiscSetRatioRGB(itfColorVector_t *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const itfColorVector_t *input, s32 count);

/* Replace each child color; parent color words are not modified. */
void itfMiscChangeFontVertexColor(frFontRequest_t *pfont, u32 color);

/* Clear each child's low byte, then OR unmasked bits into the full color word. */
void itfMiscChangeFontAlpha(frFontRequest_t *pfont, u32 alpha);

/* Translate parent entries only; child coordinates are left unchanged. */
void itfMiscMoveFontPosition(frFontRequest_t *pfont, u32 ofsx, u32 ofsy);

/* Keep the sprite texture handle while releasing the temporary file allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *filename);

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfMiscDrawTrianglePrim(sdfIVector2_t *pvertex, itfColor_t *pcolor, s32 x, s32 y, u32 z, sdfDrawTag_t *pdrawtag);

/* Emit an indexed four-vertex triangle fan with a color record per vertex. */
void itfMiscDrawRectanglePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);

/* Emit a textured triangle fan: each four-float input supplies S/T/Q only.
 * The fourth float is skipped, and the context word is shifted without masking. */
void itfMiscDrawTexPoly(sdfIVector2_t *pvertex, f32 *pstq, itfColor_t *pcolor, u32 z, s32 context, sdfDrawTag_t *pdrawtag);

/* Bind the texture and emit one two-corner sprite using two packed UV pairs.
 * Unwritten padding words in the native packet remain untouched. */
void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command);

/* Emit a colored textured sprite without binding a texture in this function. */
void itfMiscDrawSpritePrim2(sdfIVector2_t *pos, itfColor_t *uv, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag);

/* Emit an untextured two-corner GS sprite. */
void itfMiscDrawSpritePlanePrim(sdfIVector2_t *pos, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag);

/* Emit an indexed triangle strip, packing two vertices per GIF loop.
 * Header loop count truncates odd vertex counts; the native writer does not. */
void itfMiscDrawTriangleStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

/* Emit indexed RGBA/XYZ2 records with the line-strip primitive. */
void itfMiscDrawLineStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

/* Emit indexed RGBA/XYZ2 records with independent line primitives. */
void itfMiscDrawLinePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfMiscDrawTestPrim(sdfDrawTag_t *pdrawtag, u64 para, s32 context);

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *pdrawtag, s32 context, s32 wms, s32 wmt, s32 min_u, s32 max_u, s32 min_v, s32 max_v);

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 portion, s32 entire, s32 shadow,
                   sdfIVector2_t *pos, u32 z, itfColor_t *pcolor_portion,
                   itfColor_t *pcolor_entire, sdfDrawTag_t *pdrawtag);

/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *proc_id);

u32 itfCamp(void);

/* Exit the active sequence unless already in camp, then report the halt. */
void itfCampStartProcess(void);

#endif
