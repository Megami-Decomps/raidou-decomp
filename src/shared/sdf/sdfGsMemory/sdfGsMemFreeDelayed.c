#include "shared/sdf/sdfGsMemory/decls.h"

void sdfGsMemFreeDelayed(sdfGsMemSlice_t *aHandle) {
    sdfDelayedResrcEntry(&gsMemDelayedResrcCB, aHandle);
}
