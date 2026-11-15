#ifndef ITFMISC_H
#define ITFMISC_H

#include "common.h"
#include "interface.h"
#include "libsdf.h"
#include "sdf_resource.h"

u32 itfCamp(void);
void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);
void itfMiscDrawLineStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);
void itfMiscDrawRectanglePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);
void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command);
void itfMiscDrawTriangleStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);
sdfTexHandle_t *itfMiscLoadTexture(const char *filename);
void itfMiscSetRatioRGB(itfColor_t *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const itfColor_t *input, s32 count);
void itfDrawThreePanelBorderBands(sdfIVector2_t *bounds, itfColor_t *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);

#endif
