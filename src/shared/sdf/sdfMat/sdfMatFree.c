#include "shared/sdf/sdfMat/decls.h"

void sdfMatFree(sdfMat_t *aMat)
{
    if (aMat == NULL) {
        return;
    }
    sdfMatAllocCount--;
    sdfChipFreeDirect(aMat->packet[0]);
    sdfChipFreeDirect(aMat->packet[1]);
    sdfChipFreeDirect(aMat->uvwParm);
    sdfChipFreeDirect(aMat->multiTexUvwParm);
    sdfChipFreeDirect(aMat);
}
