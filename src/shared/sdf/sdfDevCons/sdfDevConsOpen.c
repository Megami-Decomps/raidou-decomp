#include "shared/sdf/sdfDevCons/decls.h"

sdfDevConsCB_tag *sdfDevConsOpen(s32 aX, s32 aY, s32 aW, s32 aH)
{
    sdfDevConsCB_tag *hndl;

    sdfDevConsInit();
    hndl = sdfChipAllocDirect(sizeof(sdfDevConsCB_tag));
    hndl->x = aX;
    hndl->y = aY;
    hndl->w = aW;
    hndl->h = aH;
    hndl->tabwidth = 8;
    hndl->mode = 0;
    hndl->attr = 0;
    hndl->bufferHandle = sdfMemAllocDirect(aW * aH * 2);
    hndl->bufferPtr = sdfMemLock(hndl->bufferHandle);
    sdfDevConsClearInside(hndl);
    sdfDevConsLinkToTop(hndl);
    return hndl;
}
