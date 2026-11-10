#include "shared/sdf/sdfSystem/decls.h"

s32 sdfFreeAuto(void *allocation)
{
    if (allocation != NULL) {
        if (sdfChipCheck(allocation)) {
            sdfChipFreeDirect(allocation);
        } else {
            sdfMemFreePtrDirect(allocation);
        }
    }
}
