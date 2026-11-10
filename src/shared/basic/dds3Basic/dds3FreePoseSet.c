#include "shared/basic/dds3Basic/decls.h"

void dds3FreePoseSet(dds3Basic_t *basic)
{
    if (basic != NULL && basic->poseset != NULL) {
        sdfChipFreeDirect(basic->poseset);
        basic->poseset = NULL;
    }
}
