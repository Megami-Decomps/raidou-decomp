#ifndef DDS_FR_FONT_MEASURE_H

#define DDS_FR_FONT_MEASURE_H
#include "common.h"

struct _FRQ;

s32 frFontMeasureLineWidth(s32 line, const struct _FRQ *glyph);

s32 frFontMeasureGlyphStripWidth(s32 line, const struct _FRQ *glyph);

#endif /* DDS_FR_FONT_MEASURE_H */
