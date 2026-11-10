#include "shared/sdf/sdfChip/decls.h"

void sdfChipFreeDelayedDirect(void *aPtr)
{
    sdfDelayedResrcEntry(&dds3CleanupList, aPtr);
}
