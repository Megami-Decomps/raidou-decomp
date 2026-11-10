#include "shared/sdf/sdfSystem/decls.h"

s32 sdfFreeAutoDelayed(void *allocation)
{
    if (allocation != NULL) {
        if (sdfChipCheck(allocation)) {
            sdfChipFreeDelayedDirect(allocation);
        } else {
            sdfMemFreeDelayedDirect(sdfMemGetHandleByPtr(allocation));
        }
    }
}
