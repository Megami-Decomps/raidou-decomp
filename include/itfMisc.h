#ifndef ITFMISC_H
#define ITFMISC_H

#include "common.h"
#include "interface.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "sdfMemory.h"

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

u32 itfCamp(void);
void itfMiscLinkInitialize(itfmisclinkstack_t *pool, itfLink_t *nodes, s32 count, s32 stride);
itfLink_t *itfMiscLinkTake(itfmisclinkstack_t *pstack);
void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);
void itfMiscDrawLineStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);
void itfMiscDrawRectanglePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);
void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command);
void itfMiscDrawTriangleStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);
sdfTexHandle_t *itfMiscLoadTexture(const char *filename);
void itfMiscSetRatioRGB(itfColor_t *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const itfColor_t *input, s32 count);
void itfDrawThreePanelBorderBands(sdfIVector2_t *bounds, itfColor_t *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);

s32 itfQuickSaveExeCheck(void);

#endif
