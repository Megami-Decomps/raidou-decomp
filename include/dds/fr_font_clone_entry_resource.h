#ifndef DDS_FR_FONT_CLONE_ENTRY_RESOURCE_H

#define DDS_FR_FONT_CLONE_ENTRY_RESOURCE_H
#include "common.h"

/* Decoding allocates a chip-heap clone; upload may borrow it or take over its
 * release. The deferred upload worker frees transferred chip buffers. */
typedef s8 FrFontPixelOwnership;

#define FR_FONT_PIXELS_BORROWED 0

#define FR_FONT_PIXELS_TRANSFERRED_TO_UPLOAD 1

#endif /* DDS_FR_FONT_CLONE_ENTRY_RESOURCE_H */
