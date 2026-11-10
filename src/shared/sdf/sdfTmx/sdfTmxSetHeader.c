#include "shared/sdf/sdfTmx/decls.h"

void sdfTmxSetHeader(TMX_HEADER_t *aHeader, s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfCLUT, s32 aCLUTFormat) {
    memset(aHeader, 0, sizeof(TMX_HEADER_t));
    aHeader->CLUTFormat = aCLUTFormat;
    aHeader->width = aWidth;
    aHeader->height = aHeight;
    aHeader->pixelFormat = aPixelFormat;
    aHeader->numOfCLUT = aNumOfCLUT;
    aHeader->formatId = SDF_TMX_MAGIC;
    aHeader->fileType = 2;
}
