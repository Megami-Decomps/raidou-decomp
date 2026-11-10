#include "shared/sdf/sdfGraph/decls.h"

void sdfGrFreeFrameSet(sdfGrFrameSet_t *aFrame)
{
    sdfGsMemFree(aFrame->frameHandle[0]);
    aFrame->frameHandle[0] = NULL;
    sdfGsMemFree(aFrame->frameHandle[1]);
    aFrame->frameHandle[1] = NULL;
    sdfGsMemFree(aFrame->zbuffHandle);
    aFrame->zbuffHandle = NULL;
}
