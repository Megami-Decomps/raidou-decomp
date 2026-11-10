#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsNewLine(sdfDevConsCB_tag *aHndl)
{
    s32 lineSize;
    s32 size;

    if (aHndl->locY == aHndl->h - 1) {
        lineSize = aHndl->w * 2;
        size = lineSize * aHndl->locY;
        if (aHndl->locY > 0) {
            memmove(aHndl->bufferPtr, aHndl->bufferPtr + lineSize, size);
        }
        memset(aHndl->bufferPtr + size, 0, lineSize);
    } else {
        aHndl->locY++;
    }
    aHndl->locX = 0;
}
